#include "base.h"
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


progress_info info;
char *pipe_buf = NULL;




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
    return write(pipe_qt[1], buf, size + HEAD_LEN); 
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

#define PROGRESS_PIPE 8

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
				strcpy(info.state, state(s.state));
				info.progress = s.progress_ppm / 10000;
				info.download_rate = s.download_payload_rate;
				info.upload_rate = s.upload_rate;
				info.total_size = s.total_done;
				memcpy(&pipe_buf[HEAD_LEN], &info, sizeof(progress_info));
				send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info));
            	std::cout.flush();
			}
        }
    }
	
    return true;
}



int start_torrent(char *torrent, char *save_path, char *file_name, uint64_t physical_offset)
try
{
	pipe_buf = (char *)malloc(HEAD_LEN + sizeof(progress_info) + 1);

    int nTimedOut = 2000; //设置下载超时时间
    //std::string save_path("/dev/vdc/");//保存文件路径
    //int torrent_upload_limit = 100000 * 1000; //上传速度限制
    //int torrent_download_limit = 100000*1000; //下载速度限制 单位是字节

    int torrent_upload_limit = 0;  //上传速度限制
    int torrent_download_limit = 0; //下载速度限制 单位是字节
    //BT下载客户端代码
    lt::settings_pack pack;
    pack.set_int(lt::settings_pack::alert_mask
        , lt::alert::error_notification
        | lt::alert::storage_notification
        | lt::alert::status_notification);
    pack.set_str(lt::settings_pack::user_agent, "ltclient/""test");
	//pack.set_int();

	if(file_name)
		strcpy(info.file_name, file_name);
	info.type = 0x00;

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
    while (true)
    {
        downloadsize = params.ti->files().file_size(0); //下载文件的总大小
		info.file_size = downloadsize;
        std::vector<lt::alert*> alerts;
        ses.pop_alerts(&alerts);
        for (auto a : alerts)
        {
            //if (::handle_alter(ses, a, th))
            //{
            //  continue;
            //}
            ::handle_alter(ses, a, th);
        }

        std::vector<std::int64_t> file_progress;
        th.file_progress(file_progress); // 获取文件下载进度
        int const idx = static_cast<int>(0);
        bool const complete = file_progress[idx] == downloadsize; //判断文件是否下载完成
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
	strcpy(info.state, "finished");
	info.progress = 100;
	info.download_rate = 0;
	memcpy(&pipe_buf[HEAD_LEN], &info, sizeof(progress_info));
	send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info));
    return 0;
}
catch (std::exception const &e) {
    std::cerr << "ERROR: " << e.what() << "\n";
}



