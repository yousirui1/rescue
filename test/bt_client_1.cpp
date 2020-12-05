#include <iostream>
#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_status.hpp>
#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/torrent_info.hpp"
#include "libtorrent/storage.hpp"
#include "libtorrent/create_torrent.hpp"
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_status.hpp>
#include "libtorrent/session.hpp"

#include "bt_client.h"

extern int pipe_bt[2];

std::shared_ptr<lt::session> m_ses;
lt::torrent_handle current_th;	

static time_t last_time;
static progress_info *info = NULL;
static char pipe_buf[HEAD_LEN + sizeof(progress_info) + 1];

uint32_t current_id = 0;
int download_finish = 0;

pthread_mutex_t bt_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bt_cond = PTHREAD_COND_INITIALIZER;

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

int add_torrent(const char *torrent, const char *save_path, char *file_name,  uint64_t physical_offset) try
{
    lt::error_code ec; 
    lt::add_torrent_params params;
    params.save_path = save_path;
    params.ti = std::make_shared<lt::torrent_info>(torrent, ec);
    params.download_limit = 0;
    params.upload_limit = 0;
	params.ti->set_physicaldrive_offset(physical_offset);

	if(file_name)
        strcpy(info->file_name, file_name);
    sscanf(torrent, "/root/%s", info->image_name);

	download_finish = 0;

	if(m_ses)
	{
    	lt::torrent_handle th = m_ses->add_torrent(std::move(params));
		current_id = th.id();
    	return SUCCESS; 
	}
	else
		return ERROR;
}
catch(std::exception &e)
{
    DEBUG("catch bt error: %s", e.what());
    return -1; 
}

void clear_torrent()
{
    std::vector<lt::torrent_handle> torrents = m_ses->get_torrents();
	for(auto h : torrents)
	{
		m_ses->remove_torrent(h);
	}
		
	download_finish = 0;
	pthread_cond_signal(&bt_cond);	
	current_id = 0;
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

    for (lt::sha1_hash const& s : similar)
        t.add_similar_torrent(s);

    auto const num = t.num_pieces();
    lt::set_piece_hashes(t, branch_path(full_path), [num]
        (lt::piece_index_t const p) {
        std::cerr<<"\r"<<p<<"/"<<num;
    });
   
    t.set_creator(creator_str.c_str());
    if(!comment_str.empty())
    {
        t.set_comment(comment_str.c_str());
    }

    if (!root_cert.empty()) {
        std::vector<char> const pem = load_file(root_cert);
        t.set_root_cert(std::string(&pem[0], pem.size()));
    }

    // create the torrent and print it to stdout
    std::vector<char> torrent;
    lt::bencode(back_inserter(torrent), t.generate());
    if (!outfile.empty()) {
        std::fstream out;
        out.exceptions(std::ifstream::failbit);
        out.open(outfile.c_str(), std::ios_base::out | std::ios_base::binary);
        out.write(torrent.data(), torrent.size());
    }
    else {
        std::cout.write(torrent.data(), torrent.size());
    }

    if (!merklefile.empty()) {
        std::fstream merkle;
        merkle.exceptions(std::ifstream::failbit);
        merkle.open(merklefile.c_str(), std::ios_base::out | std::ios_base::binary);
        auto const& tree = t.merkle_tree();
        merkle.write(reinterpret_cast<char const*>(tree.data()), tree.size() * 20);
    }
    return SUCCESS;
}

void stop_torrent()
{
    DEBUG("recv msg stop bt download task");
	//send_bt();	
	//clear_bt();
}

//int start_torrent(char *torrent, char *save_path, char *file_name, uint64_t physical_offset) try

int bt_client() try
{
    char buf[1024] = {0};
    int ret, maxfd = -1, nready;
    struct timeval tv; 

    fd_set reset, allset;
    FD_ZERO(&allset);
    FD_SET(pipe_bt[1], &allset);

    maxfd = maxfd > pipe_bt[1] ? maxfd : pipe_bt[1];

    last_time = current_time;
    (void)time(&current_time);
	
	info = (progress_info *)&pipe_buf[HEAD_LEN];
    memset(info, 0, sizeof(progress_info));
    info->type = 0x00;

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
    
    m_ses = std::make_shared<lt::session>(pack);

    for(;;)
    {   
        reset = allset;
        tv.tv_sec = 0;
        tv.tv_usec = 200;   
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
                DEBUG("pipe end bt client thread");
				clear_torrent();
				m_ses->pause();
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

                (void *)time(&current_time);         
                if(current_time - last_time >= 1)
                {   
					//std::vector<lt::torrent_status> st
                    for(lt::torrent_status  s : std::move(st->status)) 
                    {   
						if(s.handle.id() == current_id)
						{
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

							if(STRPREFIX(info->state, "seeding"))
							{
								DEBUG("torrent seeding state");
								download_finish = 1;
								pthread_cond_signal(&bt_cond);	
								current_id = 0;

							}
						}
                    }   
               	} 
            }

            /* if we recevie the finished alert or an error we're done */
            if(lt::alert_cast<lt::torrent_finished_alert>(a))
            {   
                DEBUG("download finish done");
                //lt::torrent_status const& s = st->status[0];
				
				//if(s.handle.id() == current_id)
				//{
				download_finish = 1;
				pthread_cond_signal(&bt_cond);	
				current_id = 0;
				//}
            }   
        }
        m_ses->post_torrent_updates();
        m_ses->post_dht_stats();
    }
run_out:
    return ret;
}
catch(std::exception &e)
{
    DEBUG("catch bt error: %s", e.what());
    return -1;
}


void *thread_bt(void *param)
{
    int ret;
    pthread_attr_t st_attr;
    struct sched_param sched;
    
    ret = pthread_attr_init(&st_attr);
    if(ret)
    {    
        DEBUG("ThreadUdp attr init warning ");
    }    
    ret = pthread_attr_setschedpolicy(&st_attr, SCHED_FIFO);
    if(ret)
    {    
        DEBUG("ThreadUdp set SCHED_FIFO warning");
    }    
    sched.sched_priority = SCHED_PRIORITY_TORRENT;
    ret = pthread_attr_setschedparam(&st_attr, &sched);

    //bt_client();

    return (void *)ret;
}

#if 0
            if(auto st = lt::alert_cast<lt::state_update_alert>(a))
            {


                if(st->status.empty())
                    continue;
                lt::torrent_status const& s = st->status[0];

				(void *)time(&current_time);	
				if(current_time - last_time >= 1 && s.handle.id() == current_id)
                { 
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
#endif
