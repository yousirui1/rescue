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

