#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>
#include "bt_client.h"

extern "C"
{
	#include "base.h"
	#include "packet.h"
}

time_t last_time;
static progress_info *info = NULL;
static char pipe_buf[HEAD_LEN + sizeof(progress_info) + 1];
static int run_flag = 0;

extern int pipe_bt[2];

std::shared_ptr<lt::session> m_ses;
std::vector<lt::torrent_handle> task_torrent;

// return the name of a torrent status enum
char const* state(lt::torrent_status::state_t s)
{
    switch(s) {
        case lt::torrent_status::checking_files: return "checking";
        case lt::torrent_status::downloading_metadata: return "dl metadata";
        case lt::torrent_status::downloading: return "downloading";
        case lt::torrent_status::finished: return "finished";
        case lt::torrent_status::seeding: return "seeding";
        case lt::torrent_status::allocating: return "allocating";
        case lt::torrent_status::checking_resume_data: return "checking resume";
        default: return "<>";
    }   
}

std::vector<char> load_file(std::string const &filename)
{
    std::fstream in;
    in.exceptions(std::ifstream::failbit);
    in.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
    in.seekg(0, std::ios_base::end);
    size_t const size = size_t(in.tellg());
    in.seekg(0, std::ios_base::beg);
    std::vector<char> ret(size);
    in.read(ret.data(), ret.size());
    return ret;
}

std::string branch_path(std::string const &f)
{
    if(f.empty())
        return f;
#ifdef TORRENT_WINDOWS
    if(f == "\\\\") return "";
#endif
    if(f == "/") return "";

    int len = int(f.size());
    /* if the last character is /o r \ ignore it */
    if(f[len - 1] == '/' || f[len-1] == '\\')
        --len;
    while(len > 0)
    {
        --len;
        if(f[len] == '/' || f[len] == '\\')
            break;
    }
    if(f[len] == '/' || f[len] == '\\') ++len;
    return std::string(f.c_str(), len);
}

bool file_filter(std::string const &f)
{
    if(f.empty())
        return false;

    char const *first = f.c_str();
    char const *sep = strrchr(first, '/');
#if defined(TORRENT_WINDOWS) || defined(TORRENT_OS2)
    char const* altsep = strrchr(first, '\\');
    if (sep == nullptr || altsep > sep) sep = altsep;
#endif

    /* if there is no parent path, just set 'sep'
     * to point to the filename, if there is a 
     * parent path, skip the '/' character */
    if(sep == nullptr) sep = first;
    else ++sep;

    /* return false if the first character of the filename is a */
    if(sep[0] == '.')
        return false;
    std::cerr<<f<<"\n";
    return true;
}

#if 0
int make_torrent(char *file_path, char *torrent_path, char *track)
{
    std::string creator_str = "libtorrent";
    std::string comment_str;

    std::vector<std::string> web_seeds;
    std::vector<std::string> trackers;
    std::vector<std::string> collections;
    std::vector<lt::sha1_hash> similar;
    int pad_file_limit = -1;
    int piece_size = 0;
    lt::create_flags_t flags = {};
    std::string root_cert;
    std::string outfile = torrent_path;
    std::string merklefile;
    std::string track_ip = track;

    std::cerr<<torrent_path;

#ifdef TORRENT_WINDOWS
    //outfile = "a.torrent";
#endif

    std::string full_path = file_path;
    lt::file_storage fs;
    lt::add_files(fs, full_path, file_filter, flags);
    if(fs.num_files() == 0)
    {
        //DEBUG("file size error");
        return ERROR;
    }

    lt::create_torrent t(fs, piece_size, pad_file_limit, flags);

    std::string http_track ="http://"+ track_ip +"/announce";
    std::string udp_track ="udp://"+ track_ip + "/announce";

    trackers.push_back(http_track);
    trackers.push_back(udp_track);

    int tier = 0;
    for(std::string const& tr:trackers)
    {
        if(tr == "-") ++ tier;
        else t.add_tracker(tr, tier);
    }
    for(std::string const & ws : web_seeds)
        t.add_url_seed(ws);

    for(std::string const & c : collections)
        t.add_collection(c);
#endif


std::string add_suffix_float(double val, char const* suffix)
{
    if (val < 0.001)
    {   
        std::string ret;
        ret.resize(4 + 2, ' ');
        if (suffix) ret.resize(4 + 2 + strlen(suffix), ' ');
        return ret;
    }   

    const char* prefix[] = {"kB", "MB", "GB", "TB", "PB"};
    const int num_prefix = sizeof(prefix) / sizeof(const char*);
    int i = 0;
    for (; i < num_prefix - 1; ++i)
    {   
        val /= 1000.;
        if (std::fabs(val) < 1000.) break;
    }   
    char ret[100];
    std::snprintf(ret, sizeof(ret), "%4.*f%s%s", val < 99 ? 1 : 0, val, prefix[i], suffix ? suffix : "");
    return ret;
}

uint32_t del_task(uint32_t torrent_id)
{
    std::vector<lt::torrent_handle> torrents = m_ses->get_torrents();
    for(auto h : torrents)
    {
        if(h.id() == torrent_id)
        {
            m_ses->remove_torrent(h);
        }
    }
#if 0
    for(auto torrent : task_torrent)
    {   
        DEBUG("task torrent_id %u ", torrent.id());
        if(torrent_id != (uint32_t)torrent.id())
        {   
            continue;
        }   
        torrent = task_torrent.erase(torrent);
    
        //task_torrent.erase(vec.begin()+5);
        //for_each(vec.begin(), vec.end(), [](int v) { cout << v << endl; });

        //task_torrent.pu
    } 
#endif
    return SUCCESS;
}

uint32_t add_task(const char *torrent, const char *save_path)
{   
#if 0
    lt::error_code ec;
    lt::add_torrent_params params;
    params.save_path = save_path;
    params.ti = make_shared<lt::torrent_info>(torrent, ec);
    params.download_limit = 0;
    params.upload_limit = 0;
    
    lt::torrent_handle th = m_ses->add_torrent(std::move(params));
    task_torrent.push_back(th);
    return th.id();
#endif
}

#if 0
int clear_task()
{


}
#endif

#if 0
int bt_client() try
{
    lt::settings_pack pack;

    pack.set_int(lt::settings_pack::alert_mask
                , lt::alert_category::error
                | lt::alert_category::peer
                | lt::alert_category::port_mapping
                | lt::alert_category::storage
                | lt::alert_category::tracker
                | lt::alert_category::connect
                | lt::alert_category::status
                | lt::alert_category::ip_block
                | lt::alert_category::performance_warning
                | lt::alert_category::dht
                | lt::alert_category::incoming_request
                | lt::alert_category::dht_operation
                | lt::alert_category::port_mapping_log
                | lt::alert_category::file_progress

                | lt::alert::error_notification 
                | lt::alert::storage_notification
                | lt::alert::status_notification);

    pack.set_str(lt::settings_pack::user_agent, "ltclient/""test");

    m_ses = make_shared<lt::session>(pack);

    char buf[1024] = {0};
    int ret, maxfd = -1, nready;
    struct timeval tv; 

    fd_set reset, allset;
    FD_ZERO(&allset);
    FD_SET(pipe_bt[1], &allset);

    maxfd = maxfd > pipe_bt[1] ? maxfd : pipe_bt[1];

    last_time = current_time;
    (void)time(&current_time);

    lt::torrent_handle h;
    for(;;)
    {   
        reset = allset;
        tv.tv_sec = 0;
        tv.tv_usec = 200000;
        ret = select(maxfd + 1, &reset, NULL, NULL, &tv);

        if(ret == -1)
        {
            if(errno == EINTR)
                continue;
            else if(errno != EBADF)
            {
                //cout("select %s error", strerror(ret));
                continue;
            }
        }
        (void)time(&current_time);
        /* pipe msg */
        if(FD_ISSET(pipe_bt[1], &reset))
        {
            ret = read(pipe_bt[1], (void *)buf, sizeof(buf));
            if(ret == 1 && buf[0] == 'S')
            {
                //cout("pipe end bt client thread");
                break;
            }
            if(--nready <= 0)
                continue;
        }
        std::vector<lt::alert *> alerts;
        m_ses->pop_alerts(&alerts);
        for(lt::alert const *a : alerts)
        {
            if(auto st = lt::alert_cast<lt::state_update_alert>(a))
            {
                if(st->status.empty())
                    continue;
                lt::torrent_status const& s = st->status[0];
            }
        }
        m_ses->post_torrent_updates();
        m_ses->post_dht_stats();
    }
    return 0;
}
catch(std::exception &e)
{
	DEBUG("catch bt error: %s", e.what());
	return -1;
}
#endif

void *thread_bt()
{



}

#if 0
                std::cout << "\r" << state(s.state) << " "
                    << (s.download_payload_rate / 1000) << " kB/s "
                    << (s.total_done / 1000) << " kB ("
                    << (s.progress_ppm / 10000) << "%) downloaded\x1b[K";
                std::cout.flush();
#endif

#if 0
void stop_torrent()
{

}

int start_torrent(char *torrent, char *save_path, char *file_name, uint64_t physical_offset) try
{

}
catch(std::exception &e)
{
	DEBUG("catch bt error: %s", e.what());
	return -1;
}
#endif

#if 1
void stop_torrent()
{
	run_flag = 0;
	DEBUG("recv msg stop bt download task");
}
int start_torrent(char *torrent, char *save_path, char *file_name, uint64_t physical_offset) try
{
	int ret = 1;
	info = (progress_info *)&pipe_buf[HEAD_LEN];
	memset(info, 0, sizeof(progress_info));
	info->type = 0x00;
	if(file_name)
		strcpy(info->file_name, file_name);
	sscanf(torrent, "/root/%s", info->image_name);

    lt::settings_pack pack;

	DEBUG("bt torrent %s", info->image_name);

	pack.set_int(lt::settings_pack::alert_mask,
                  lt::alert::error_notification
                | lt::alert::storage_notification
                | lt::alert::status_notification);

	lt::session ses(pack);
	
    lt::add_torrent_params params;
 	params.save_path = save_path;
    params.ti = std::make_shared<lt::torrent_info>(torrent);
	params.ti->set_physicaldrive_offset(physical_offset);
    params.download_limit = 0;
    params.upload_limit = 0;

	/* this is the handle we'll set once we get the notification of it being added */
	ses.async_add_torrent(std::move(params));
	lt::torrent_handle h;
	//lt::torrent_handle h = ses.add_torrent(params);
	run_flag = 1;
	while(run_flag)
	{
		std::vector<lt::alert *>alerts;
		ses.pop_alerts(&alerts);
		
		for(lt::alert const *a : alerts)
		{
			if(auto at = lt::alert_cast<lt::add_torrent_alert>(a))
			{
				h = at->handle;
			}
			/* if we recevie the finished alert or an error we're done */
			if(lt::alert_cast<lt::torrent_finished_alert>(a))
			{
				DEBUG("download finish done");
				ses.remove_torrent(h);
				ses.abort();
				return 0;
			}
			if(lt::alert_cast<lt::torrent_error_alert>(a))
			{
				DEBUG("bt error: %s", a->message().c_str());
				ses.remove_torrent(h);
				ses.abort();
				return 1;
			}

			if(auto st = lt::alert_cast<lt::state_update_alert>(a))
			{
				if(st->status.empty())
					continue;
				/* we only have a single torrent. so we know which one 
				 * the status is for */
				(void *)time(&current_time);
				if(current_time - last_time >= 1)
				{
					lt::torrent_status const & s = st->status[0];
					DEBUG("%s download rate %lu KB/s, total_download %lu KB, uprate %lu KB/s, total_up %lu KB, progress %d",
                    	state(s.state), s.download_payload_rate / 1000, s.total_done / 1000, s.upload_rate/1000, 
						s.total_upload / 1000, s.progress_ppm / 10000);

					strcpy(info->state, state(s.state));
					info->file_size = s.total_wanted;
					info->progress = s.progress_ppm / 10000;
					info->download_rate = s.download_payload_rate;
					info->upload_rate = s.upload_rate;
					info->total_size = s.total_done;
					send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);
					last_time = current_time;
				}
			}	
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));	
		/* ask the sesson to post a state_update_alert, to update our
	 	 * state output for the torrent */
		ses.post_torrent_updates();
	}
	
	strcpy(info->state, "finished");
	info->progress = 0;
	info->download_rate = 0;
	info->upload_rate = 0;
	info->total_size = 0;
	send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);

	DEBUG("bt cancel download %s", file_name);
	ses.abort();
	return 1;
}
catch(std::exception &e)
{
	DEBUG("catch bt error: %s", e.what());
	return -1;
}
#endif


