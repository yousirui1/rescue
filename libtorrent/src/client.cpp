// ltclient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
 
#include <iostream>
#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/torrent_info.hpp"
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_status.hpp>
#include <chrono>
 

#ifdef __cplusplus
extern "C" 
{
#endif

int start_bt(char *torrent, char *dst, uint64_t physic_offset);

#ifdef __cplusplus
}
#endif


 
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

time_t current_time, last_time;
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
			lt::torrent_status s = th.status(lt::torrent_handle::query_save_path);
			//lt::torrent_status s = p->status[0];//如果只有一个torrent文件，就直接取status[0]
			//sprintf_s(cinfo, sizeof(cinfo), "%s download rate %dKB /s, total_download %dKB, uprate %dKB /s, total_up %dKB, progress %.1lf, progress_ppm %lld progress %lld%%  %d%%"
			//, state(s.state),s.download_payload_rate/1000, s.total_done/1000, s.upload_rate/1000, s.total_upload/1000, s.progress, s.progress_ppm, int(s.progress_ppm/10000), file_progress[0]*100 / downloadsize);

			if(current_time - last_time > 3)
			{
			cout << state(s.state) << " download rate " << s.download_payload_rate / 1000 << "KB /s, total_download " << s.total_done / 1000 << "KB, uprate " << s.upload_rate / 1000 << "KB /s, total_up " << s.total_upload / 1000
				<< "KB, progress " << s.progress << " progress_ppm " << s.progress_ppm << " progress " << s.progress_ppm / 10000 << "  " << file_progress[0] * 100 / downloadsize << endl;
			//sprintf_s(cinfo, "%s download rate %dKB /s, total_download %dKB, uprate %dKB /s, total_up %dKB, curdown %lld, filesize %lld, downprogress %lld%%"
			//	, state(s.state), s.download_payload_rate / 1000, s.total_done / 1000, s.upload_rate / 1000, s.total_upload / 1000, file_progress[0], downloadsize, file_progress[0] * 100 / downloadsize);
			cout << cinfo << endl;
			std::cout.flush();
			last_time = current_time; 
			} 
			//lt::torrent_status st = th.status(lt::torrent_handle::query_save_path);
			//std::cout << "\r" << state(st.state) << " "
			//	<< (st.download_payload_rate / 1000) << " kB/s "
			//	<< (st.total_done / 1000) << " kB ("
			//	<< (st.progress_ppm / 10000) << "%) downloaded";
			//std::cout.flush();
		}
	}
	return true;
}

int main(int argc, char *argv[])
{


}


int start_bt(char *torrent, char *save_path, uint64_t physic_offset)
//int main(int argc, char*argv[])
{
	int nTimedOut = 200000; //设置下载超时时间
	//std::string save_path("/dev/vdc/");//保存文件路径
	int torrent_upload_limit = 100000 * 1000; //上传速度限制
	int torrent_download_limit = 100000*1000; //下载速度限制 单位是字节

	//BT下载客户端代码
	lt::settings_pack pack;
	pack.set_int(lt::settings_pack::alert_mask
		, lt::alert::error_notification
		| lt::alert::storage_notification
		| lt::alert::status_notification);
	pack.set_str(lt::settings_pack::user_agent, "ltclient/""test");
	lt::session ses(pack);
 
	//设置下载参数
	lt::add_torrent_params params;
	params.save_path = save_path;
	//params.save_path = argv[2];
	params.ti = std::make_shared<lt::torrent_info>(torrent);
	//params.ti = std::make_shared<lt::torrent_info>(argv[1]);
	params.download_limit = torrent_download_limit;
	params.upload_limit = torrent_upload_limit;
 
	auto start = system_clock::now();
	lt::torrent_handle th = ses.add_torrent(params);
	static auto end = system_clock::now();
	static auto duration = duration_cast<microseconds>(end - start);;

	//params.ti->set_physicaldrive_offset(2099367 * 512);	
	params.ti->set_physicaldrive_offset(physic_offset);	
	(void)time(&current_time);
	last_time = current_time;
	while (true)
	{
		//*************************************************************************
		//std::thread printprogress([&ses,th]
		//{
		//	lt::torrent_status st = th.status(lt::torrent_handle::query_save_path);
		//	std::cout << "\r" << state(st.state) << " "
		//		<< (st.download_payload_rate / 1000) << " kB/s "
		//		<< (st.total_done / 1000) << " kB ("
		//		<< (st.progress_ppm / 10000) << "%) downloaded";
		//	std::cout.flush();
		//});
		//
		//printprogress.join();
		//***********************************************************************
		downloadsize = params.ti->files().file_size(0); //下载文件的总大小
		std::vector<lt::alert*> alerts;
		ses.pop_alerts(&alerts);

		(void)time(&current_time);
		for (auto a : alerts)
		{
			//if (::handle_alter(ses, a, th))
			//{
			//	continue;
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
			//break;
		}
#endif
		if ((double(duration.count())*microseconds::period::num / microseconds::period::den) > nTimedOut)//判断是否超时
		{
			cout << "download timed out" << endl;
			break;
		}
		//std::this_thread::sleep_for(std::chrono::microseconds(3000));
		ses.post_torrent_updates();
	}
	std::string title = params.ti->files().file_name(0).to_string();

		cout << "download "<<title<<" cost " << double(duration.count())*microseconds::period::num / microseconds::period::den<<"s"<<endl;
	
		

	while (1)
	{
		char c = getchar();
		if (c == 'p')
			break;
	}
	return 0;
}
 
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单
 
// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

