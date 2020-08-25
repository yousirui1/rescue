extern "C"
{
#include "base.h"
}
#include "torrent.h"
#include <iostream>
#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/torrent_info.hpp"
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_status.hpp>
#include <chrono>

using namespace std;
using namespace chrono;
static int count;
int64_t downloadsize = 0;
int run_flag = 0;

char const* state(lt::torrent_status::state_t s)
{   
    switch (s) {
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

time_t last_time;
extern time_t current_time;


static progress_info *info;
static char pipe_buf[HEAD_LEN + sizeof(progress_info) + 1] = {0};

void set_request_head(char *buf, char encrypt_flag, short cmd, int data_size)
{
    int ret = -1; 
    req_head *head = (req_head *)buf;
    head->syn = 0xff;
    head->encrypt_flag = encrypt_flag;
    head->cmd = cmd;
    head->data_size = data_size;
}

static int send_pipe(char *buf, short cmd, int size)
{
    set_request_head(buf, 0x0, cmd, size);
    return write(pipe_event[1], buf, size + HEAD_LEN); 
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

// return the name of a torrent status enum
bool handle_alter(lt::session& ses, lt::alert* a, lt::torrent_handle &th)
{
    using namespace lt;

    if (add_torrent_alert *p = alert_cast<add_torrent_alert>(a))
    {
        if (p->error)
        {
            std::fprintf(stderr, "failed to add torrent:%s %s\n"
                ,p->params.ti?p->params.ti->name().c_str():p->params.name.c_str()
            ,p->error.message().c_str());
        }
    }
    else if (torrent_finished_alert *p = alert_cast<torrent_finished_alert>(a))
    {
        cout << "torrent finished" << endl;
    }
    else if (state_update_alert *p = alert_cast<state_update_alert>(a))
    {
        char cinfo[1024] = { 0x00 };
        std::vector<std::int64_t> file_progress;
        th.file_progress(file_progress); // 获取文件下载进度
        int const idx = static_cast<int>(0);
        if(!p->status.empty())
        {
			(void *)time(&current_time);
			if(current_time - last_time >= 1)
			{
				last_time = current_time ;

            	lt::torrent_status s = th.status(lt::torrent_handle::query_save_path);   
				cout << state(s.state) << " download rate " << s.download_payload_rate / 1000 << "KB /s, total_download " << s.total_done / 1000 << "KB, uprate " << s.upload_rate / 1000 << "KB /s, total_up " << s.total_upload / 1000
                << "KB, progress " << s.progress << " progress_ppm " << s.progress_ppm << " progress " << s.progress_ppm / 10000 << "  " << file_progress[0] * 100 / downloadsize << endl;
				strcpy(info->state, state(s.state));
				info->progress = s.progress_ppm / 10000;
				info->download_rate = s.download_payload_rate;
				info->upload_rate = s.upload_rate;
				info->total_size = s.total_done;
				send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info));
					
            	std::cout.flush();
			}
        }
    }
    return true;
}

void stop_torrent()
{
	cout<<"stop_torrent"<<endl;
	run_flag = 1;
}

int start_torrent(char *torrent, char *save_path, char *file_name, uint64_t physical_offset)
try
{

	int complete = 0;
	memset(pipe_buf, 0, HEAD_LEN + sizeof(progress_info) + 1);
	info = (progress_info *)&pipe_buf[HEAD_LEN];

    int nTimedOut = 2000; //设置下载超时时间
    //std::string save_path("/dev/vdc/");//保存文件路径
    //int torrent_upload_limit = 1024 * 1024  * 5; //上传速度限制
    //int torrent_download_limit = 100000*1000; //下载速度限制 单位是字节

    int torrent_upload_limit = 0;  //上传速度限制
    int torrent_download_limit = 0; //下载速度限制 单位是字节
    //BT下载客户端代码

	lt::settings_pack pack;
	pack.set_int(lt::settings_pack::choking_algorithm, lt::settings_pack::rate_based_choker);	
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
        | lt::alert_category::file_progress);
	pack.set_str(lt::settings_pack::user_agent, "ltclient/""test");

    pack.set_int(lt::settings_pack::active_dht_limit, 600);
    pack.set_int(lt::settings_pack::active_tracker_limit, 4000);
    pack.set_bool(lt::settings_pack::allow_multiple_connections_per_ip, 1); 
    pack.set_int(lt::settings_pack::allowed_fast_set_size, 0); 
    pack.set_int(lt::settings_pack::cache_buffer_chunk_size, 128);
    pack.set_int(lt::settings_pack::cache_expiry, 30);
    pack.set_int(lt::settings_pack::cache_size, 65536);
    pack.set_int(lt::settings_pack::checking_mem_usage, 320);
    pack.set_int(lt::settings_pack::connection_speed, 500);
    pack.set_int(lt::settings_pack::connections_limit, 8000);
    pack.set_int(lt::settings_pack::dht_upload_rate_limit, 20000);
    pack.set_int(lt::settings_pack::file_pool_size, 500);
    pack.set_int(lt::settings_pack::inactivity_timeout, 20);
    pack.set_int(lt::settings_pack::listen_queue_size, 3000);
    pack.set_bool(lt::settings_pack::low_prio_disk, 0); 
    pack.set_int(lt::settings_pack::max_allowed_in_request_queue, 2000);
    pack.set_int(lt::settings_pack::max_failcount, 1); 
    pack.set_int(lt::settings_pack::max_http_recv_buffer_size, 6291456);
    pack.set_int(lt::settings_pack::max_out_request_queue, 1500);
    pack.set_int(lt::settings_pack::max_queued_disk_bytes, 7340032);
    pack.set_int(lt::settings_pack::max_rejects, 10);
    pack.set_int(lt::settings_pack::mixed_mode_algorithm, 10);
    pack.set_int(lt::settings_pack::peer_timeout, 20);
    pack.set_int(lt::settings_pack::recv_socket_buffer_size, 1048576);
    pack.set_int(lt::settings_pack::request_timeout, 10);
    pack.set_int(lt::settings_pack::send_buffer_low_watermark, 1048576);
    pack.set_int(lt::settings_pack::send_buffer_watermark, 3145728);
    pack.set_int(lt::settings_pack::send_buffer_watermark_factor, 150);
    pack.set_int(lt::settings_pack::send_socket_buffer_size, 1048576);
    pack.set_int(lt::settings_pack::suggest_mode, 1); 
    pack.set_bool(lt::settings_pack::use_disk_cache_pool, 1); 
    pack.set_int(lt::settings_pack::write_cache_line_size, 256);


	if(file_name)
		strcpy(info->file_name, file_name);

	sscanf(torrent, "/root/%s", info->image_name);	

	info->type = 0x00;

    lt::session ses(pack);

    //设置下载参数
    lt::add_torrent_params params;
    params.save_path = save_path;
    params.ti = std::make_shared<lt::torrent_info>(torrent);
    params.download_limit = torrent_download_limit;
    params.upload_limit = torrent_upload_limit;
	
    auto start = system_clock::now();
    lt::torrent_handle th = ses.add_torrent(params);
    static auto end = system_clock::now();
    static auto duration = duration_cast<microseconds>(end - start);;
    params.ti->set_physicaldrive_offset(physical_offset);
	run_flag = 0;
    while (!run_flag)
    {
        downloadsize = params.ti->files().file_size(0); //下载文件的总大小
		info->file_size = downloadsize;
        std::vector<lt::alert*> alerts;
        ses.pop_alerts(&alerts);
        for (auto a : alerts)
        {
            //if (::handle_alter(ses, a, th))
            //{
            //  continue;
            //}
            handle_alter(ses, a, th);
        }
        std::vector<std::int64_t> file_progress;
        th.file_progress(file_progress); // 获取文件下载进度
        int const idx = static_cast<int>(0);
        complete = file_progress[idx] == downloadsize; //判断文件是否下载完成
        if (complete)
        {
            ses.post_torrent_updates();
            cout << "\ndownload is complete" << endl;
            break;
        }
        end = system_clock::now();
        duration = duration_cast<microseconds>(end - start);
#if 0
        if ((double(duration.count())*microseconds::period::num / microseconds::period::den) > 100 && file_progress[idx] == 0)
        {
            cout << "download failed,check" << endl;
            break;
        }
        if ((double(duration.count())*microseconds::period::num / microseconds::period::den) > nTimedOut)//判断是否超时
        {
            cout << "download timed out" << endl;
            break;
        }
#endif
        //std::this_thread::sleep_for(std::chrono::microseconds(3000));
        ses.post_torrent_updates();
    }
    std::string title = params.ti->files().file_name(0).to_string();
    cout << "download "<<title<<" cost " << double(duration.count())*microseconds::period::num / microseconds::period::den<<"s"<<endl;
	if(complete)
	{
		strcpy(info->state, "finished");
		info->progress = 100;
		info->download_rate = 0;
		send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info));
		return 0;
	}
	else
	{
		strcpy(info->state, "finished");
		strcpy(info->storage, title.c_str());
		info->progress = 0;
		info->download_rate = 0;
		send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info));
    	return 1;
	}
}
catch (std::exception const &e) {
    std::cerr << "ERROR: " << e.what() << "\n";
}



