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

std::shared_ptr<lt::session> m_ses;

extern int pipe_bt[2];

static time_t last_time;
static progress_info *info = NULL;
static char pipe_buf[HEAD_LEN + sizeof(progress_info) + 1];
static int run_flag = 0;
static int total_done_count = 0;

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

void clear_torrent()
{
    std::vector<lt::torrent_handle> torrents = m_ses->get_torrents();
    for(auto h : torrents)
    {
        m_ses->remove_torrent(h);
    }
}

void bt_client()
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

	m_ses = std::make_shared<lt::session>(pack);
}


void stop_torrent()
{   
    run_flag = 0;
    DEBUG("recv msg stop bt download task");
}

void destory_bt()
{
	stop_torrent();
	clear_torrent();	
	m_ses->abort();
}


int start_torrent(char *torrent, char *save_path, char *file_name, int diff_mode, uint64_t physical_offset) try
{
    int ret = 1;
    info = (progress_info *)&pipe_buf[HEAD_LEN];
    memset(info, 0, sizeof(progress_info));
    info->type = 0x00;
	*(int *)&(info->storage[0]) = diff_mode;


    if(file_name)
        strcpy(info->file_name, file_name);
    sscanf(torrent, "/root/%s", info->image_name);

    lt::settings_pack pack;

    lt::add_torrent_params params;
    params.save_path = save_path;
    params.ti = std::make_shared<lt::torrent_info>(torrent);
    params.ti->set_physicaldrive_offset(physical_offset);
    params.download_limit = 0;
    params.upload_limit = 0;
	total_done_count = 0;

    /* this is the handle we'll set once we get the notification of it being added */
    //ses.async_add_torrent(std::move(params));
    lt::torrent_handle h = m_ses->add_torrent(params);
	uint32_t current_id = h.id();
    //lt::torrent_handle h = ses.add_torrent(params);
    run_flag = 1;
    while(run_flag)
    {
        std::vector<lt::alert *>alerts;
        m_ses->pop_alerts(&alerts);

        for(lt::alert const *a : alerts)
        {
            if(auto at = lt::alert_cast<lt::add_torrent_alert>(a))
            {
                //h = at->handle;
            }
            /* if we recevie the finished alert or an error we're done */
            if(lt::alert_cast<lt::torrent_finished_alert>(a))
            {
                DEBUG("download finish done");
				ret = SUCCESS;
                run_flag = 0;
            }
            if(lt::alert_cast<lt::torrent_error_alert>(a))
            {
                DEBUG("bt error: %s", a->message().c_str());
				info->progress = 0;
                m_ses->remove_torrent(h);
				ret = ERROR;
                run_flag = 0;
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

                    DEBUG("id % lu %s download rate %lu KB/s, total_download %llu, total_wanted %llu, "
							"uprate %lu KB/s, total_up %lu KB, progress %d", 
						s.handle.id(), state(s.state), s.download_payload_rate / 1000, s.total_done, s.total_wanted,
						s.upload_rate/1000, s.total_upload / 1000, s.progress_ppm / 10000);
                    last_time = current_time;

					if(current_id == s.handle.id())
					{
				  		strcpy(info->state, state(s.state));
                    	info->file_size = s.total_wanted;
                    	info->progress = s.progress_ppm / 10000;
                    	info->download_rate = s.download_payload_rate;
                    	info->upload_rate = s.upload_rate;
                    	info->total_size = s.total_done;

						if(STRPREFIX(state(s.state), "seeding") ||(s.total_done == s.total_wanted && total_done_count ++ >= 30))
						{
							ret = SUCCESS;
							info->progress = 100;			
                			run_flag = 0;
						}
						DEBUG("send progress start");	
                    	send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);
						DEBUG("send progress end");	
					}
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        /* ask the sesson to post a state_update_alert, to update our
         * state output for the torrent */
        m_ses->post_torrent_updates();
    }

    strcpy(info->state, "finished");
    send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);
    return ret;
}
catch(std::exception &e)
{
    DEBUG("catch bt error: %s", e.what());
    return -1;
}




#if 0
void stop_torrent()
{
	char s = 'S';

	DEBUG("stop torrent ");
	write(pipe_bt[1], &s, sizeof(s));
}

					if(STRPREFIX(state(s.state), "seeding"))
                    {   
						if(current_id == s.handle.id())
						{
			         		strcpy(info->state, state(s.state));
                    		info->file_size = s.total_wanted;
                    		info->progress = s.progress_ppm / 10000;
                    		info->download_rate = s.download_payload_rate;
                    		info->upload_rate = s.upload_rate;
                    		info->total_size = s.total_done;

							ret = SUCCESS;
							info->progress = 100;			
                			run_flag = 0;

							DEBUG("sending finish done");
						}
                    }
					else		//"check download"
					{
                    	strcpy(info->state, state(s.state));
                    	info->file_size = s.total_wanted;
                    	info->progress = s.progress_ppm / 10000;
                    	info->download_rate = s.download_payload_rate;
                    	info->upload_rate = s.upload_rate;
                    	info->total_size = s.total_done;

						if(s.total_done == s.total_wanted)
						{
							if(total_done_count ++ >= 30)
							{
								run_flag = 0;
								DEBUG("download timeout finish ");
							}
						}
                    	send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);
					}

int start_torrent(char *torrent, char *save_path, char *file_name, uint64_t physical_offset) try
{
    char buf[1024] = {0};
    int ret = -1, maxfd = -1, nready;
    struct timeval tv;

    fd_set reset, allset;
    FD_ZERO(&allset);
    FD_SET(pipe_bt[0], &allset);

    maxfd = maxfd > pipe_bt[0] ? maxfd : pipe_bt[0];

    info = (progress_info *)&pipe_buf[HEAD_LEN];
    memset(info, 0, sizeof(progress_info));
    info->type = 0x00;

    lt::add_torrent_params params;
	//lt::torrent_handle h;
    params.save_path = save_path;
    params.ti = std::make_shared<lt::torrent_info>(torrent);
    params.ti->set_physicaldrive_offset(physical_offset);
    params.download_limit = 0;
    params.upload_limit = 0;
	lt::torrent_handle h = m_ses->add_torrent(params);
	//m_ses->async_add_torrent(std::move(params));
	uint32_t current_id = h.id();
	
	for(;;)
	{
        reset = allset;
        tv.tv_sec = 0;
        tv.tv_usec = 200;
        nready = select(maxfd + 1, &reset, NULL, NULL, &tv);
        if(nready == -1)
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
        if(FD_ISSET(pipe_bt[0], &reset))
        {
            nready = read(pipe_bt[0], (void *)buf, sizeof(buf));
            if(nready == 1 && buf[0] == 'S')
            {
                DEBUG("pipe end bt client thread");
                break;
            }
        }

        std::vector<lt::alert *> alerts;
        m_ses->pop_alerts(&alerts);
        for(lt::alert const *a : alerts)
        {
            if(auto at = lt::alert_cast<lt::add_torrent_alert>(a))
            {   
                //h = at->handle;
            }

			if(lt::alert_cast<lt::torrent_finished_alert>(a))
            {   
                DEBUG("download finish done");
				info->progress = 100;			
				stop_torrent();
				ret = SUCCESS;
            }   
            if(lt::alert_cast<lt::torrent_error_alert>(a))
            {   
                DEBUG("bt error: %s", a->message().c_str());
                m_ses->remove_torrent(h);
				stop_torrent();
				ret = ERROR;
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
					
					if(STRPREFIX(info->state, "seeding") && current_id == s.handle.id())
                    {   
                        DEBUG("torrent seeding state");
						info->progress = 100;			
						stop_torrent();
						ret = SUCCESS;
                    }
                }
            }
		}
		/* ask the sesson to post a state_update_alert, to update our
         * state output for the torrent */
        m_ses->post_torrent_updates();
	}
	
    strcpy(info->state, "finished");
    info->progress = 0;
    info->download_rate = 0;
    info->upload_rate = 0;
    info->total_size = 0;
    send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);
	return ret;
}

catch(std::exception &e)
{
    DEBUG("catch bt error: %s", e.what());
    return -1;
}
#endif

