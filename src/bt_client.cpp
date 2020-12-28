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

pthread_mutex_t bt_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bt_cond = PTHREAD_COND_INITIALIZER;

static int run_flag = 0;
uint32_t m_torrent_id = 0;
std::shared_ptr<lt::session> m_ses;

// return the name of a torrent status enum
char const* state(lt::torrent_status::state_t s)
{
    switch(s) 
	{
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

int del_torrent(uint32_t torrent_id)
{
	if(!m_ses || 0 == m_torrent_id)
		return ERROR;

    std::vector<lt::torrent_handle> torrents = m_ses->get_torrents();
    for(auto h : torrents)
    {   
        if(h.id() == torrent_id)
        {   
            m_ses->remove_torrent(h);
        }   
    }   
    return SUCCESS;
}

#if 0
uint32_t add_torrent(const char *torrent, const char *save_path, char *file_name, uint64_t physical_offset) try
{
    lt::error_code ec;
    lt::add_torrent_params params;
    params.save_path = save_path;
    params.ti = std::make_shared<lt::torrent_info>(torrent, ec);
    params.download_limit = 0;
    params.upload_limit = 0;
	params.ti->set_physicaldrive_offset(physical_offset);

	if(m_ses)
	{
    	lt::torrent_handle th = m_ses->add_torrent(std::move(params));
		return th.id();
	}
	else
	{
		return ERROR;
	}
}
catch(std::exception &e) 
{
    DEBUG("catch bt error: %s", e.what());
    return -1; 
}
#endif

#if 0
int download_loop()
{
#if 0
	for()
	{
        std::vector<lt::alert*> alerts;
        m_ses->pop_alerts(&alerts);

        for (lt::alert const* a : alerts) 
		{
            std::cout << a->message() << std::endl;
            // if we receive the finished alert or an error, we're done
            if (auto at = lt::alert_cast<lt::torrent_finished_alert>(a)) 
			{
				if(m_torrent_id == at->handle.id())
					m_torrent_id = 0;	

				pthread_cond_signal(&bt_cond);
            }   
            if (auto at = lt::alert_cast<lt::torrent_error_alert>(a)) 
			{
				if(m_torrent_id == at->handle.id())
					m_torrent_id = 0;	
				m_ses->remove_torrent(at->handle);
				pthread_cond_signal(&bt_cond);
            }   

			if(auto st = lt::alert_cast<lt::state_update_alert>(a))
			{
                if(st->status.empty() || m_torrent_id == 0)
                    continue;
                /* we only have a single torrent. so we know which one 
                 * the status is for */
                (void *)time(&current_time);
                if(current_time - last_time >= 1)
                {   
                    lt::torrent_status const & s = st->status[0];
					last_time = current_time;
                    if(m_torrent_id == s.handle.id())
                    {   
						DEBUG("%s download rate %lu KB/s, total_download %lu KB, uprate %lu KB/s, total_up %lu KB, progress %d",
                        state(s.state), s.download_payload_rate / 1000, s.total_done / 1000, s.upload_rate/1000, 
                        s.total_upload / 1000, s.progress_ppm / 10000);

                       	if(STRPREFIX(state(s.state), "seeding"))			//seeding
						{
							pthread_cond_signal(&bt_cond);
							//m_torrent = 0;
						}
						else			//download check
						{
							if(s.total_done == s.total_wanted)
							{
								pthread_cond_signal(&bt_cond);
								//pthread_
								//m_torrent = 0;
							}
						}
                    }   
				}
			}
        }   
		m_ses->post_torrent_updates();
        m_ses->post_dht_stats();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
#endif
done:


	return SUCCESS;
}
#endif


int bt_client() try
{
	time_t last_time;
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
	return SUCCESS;
}
catch(std::exception &e)
{
	DEBUG("");
	return ERROR;
}


void stop_torrent()
{
	run_flag = 0;
	//info->progress = 0;

	del_torrent(m_torrent_id);
	DEBUG("recv msg stop bt download task");
}


void check_torrent(char *torrent)
{
    std::vector<lt::torrent_handle> torrents = m_ses->get_torrents();
    for(auto h : torrents)
    {   
		if(STRPREFIX(&torrent[6], h.get_torrent_info().files().name().c_str()))
		{
			m_ses->remove_torrent(h);
			break;
		}
	}
}

int add_torrent(char *torrent, char *save_path, uint64_t physical_offset) try
{
    lt::add_torrent_params params;
 	params.save_path = save_path;
    params.ti = std::make_shared<lt::torrent_info>(torrent);
	params.ti->set_physicaldrive_offset(physical_offset);
    params.download_limit = 0;
    params.upload_limit = 0;

	check_torrent(torrent);
	if(!m_ses)
		return ERROR;

	/* this is the handle we'll set once we get the notification of it being added */
	lt::torrent_handle h = m_ses->add_torrent(params);
	
	m_torrent_id = h.id();
	return SUCCESS;
}
catch(std::exception &e) 
{
    DEBUG("catch bt error: %s", e.what());
    return ERROR;
}


//int start_torrent(char *torrent, char *save_path, char *file_name, int diff_mode, uint64_t physical_offset) try
int start_torrent(char *torrent, char *pipe_buf, int diff_mode) try
{
	int ret = ERROR;
	time_t last_time;
	
 	progress_info *info = (progress_info *)&pipe_buf[HEAD_LEN];
	
	info->type = 0x00;
	*(int *)&(info->storage[0]) = diff_mode;
			
	sscanf(torrent, "/root/%s", info->image_name);

	DEBUG("torrent %s", torrent);
	
	run_flag = 1;
	while(run_flag)
	{
		std::vector<lt::alert *>alerts;
		m_ses->pop_alerts(&alerts);
		
		for(lt::alert const *a : alerts)
		{
			/* if we recevie the finished alert or an error we're done */
			if(auto at = lt::alert_cast<lt::torrent_finished_alert>(a))
			{
				if(m_torrent_id != 0 && m_torrent_id == at->handle.id())
				{
					DEBUG("download finish done");
					m_torrent_id = 0;	
					ret = SUCCESS;
					info->progress = 99;
					goto done;
				}
			}
			if(auto at = lt::alert_cast<lt::torrent_error_alert>(a))
			{
				if(m_torrent_id != 0 && m_torrent_id == at->handle.id())
				{
					DEBUG("bt error: %s", a->message().c_str());
					m_torrent_id = 0;	
					info->progress = 0;
					ret = ERROR;
					goto done;
				}
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
					last_time = current_time;
					lt::torrent_status const & s = st->status[0];

					DEBUG("%s download rate %lu KB/s, total_download %lu KB, uprate %lu KB/s, total_up %lu KB, progress %d",
                    state(s.state), s.download_payload_rate / 1000, s.total_done / 1000, s.upload_rate/1000, 
                    s.total_upload / 1000, s.progress_ppm / 10000);	
					DEBUG("torrent_id %lu s.handle.id() %lu", m_torrent_id, s.handle.id());
					if(m_torrent_id != 0 && m_torrent_id == s.handle.id())
					{
					 	strcpy(info->state, state(s.state));
                    	info->file_size = s.total_wanted;
                    	info->progress = s.progress_ppm / 10000;
                    	info->download_rate = s.download_payload_rate;
                    	info->upload_rate = s.upload_rate;
                    	info->total_size = s.total_done;
                    	send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);
                    	last_time = current_time;

						if(s.total_done == s.total_wanted && STRPREFIX(state(s.state), "seeding"))
						{
							DEBUG("download finish done");
							info->progress = 99;
							ret = SUCCESS;
							m_torrent_id = 0;
							goto done;
						}
					}
				}
			}	
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));	
		/* ask the sesson to post a state_update_alert, to update our
	 	 * state output for the torrent */
		m_ses->post_torrent_updates();
	}

done:
	DEBUG("bt download ret : %d", ret);
	return ret;
}
catch(std::exception &e)
{
	DEBUG("catch bt error: %s", e.what());
	return ERROR;
}

#if 0

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

					last_time = current_time;
					if(s.total_done == s.total_wanted)
					{
						DEBUG("download finish done");
						info->progress = 99;
						ret = SUCCESS;
						goto done;
					}
#endif
