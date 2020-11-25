#include "base.h"
#include "client.h"
#include "packet.h"
#include "cJSON.h"
#include "device.h"
#include "task.h"
#include "queue.h"
#include "bt_client.h"
#include "adlist.h"

struct client m_client;
static int server_s = -1;

extern struct device_info dev_info;
extern QUEUE task_queue;

int first_time = 1; //第一次登陆

struct desktop_group m_group[8] = {0};

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

static int send_get_desktop_group_list(struct client *cli);

void client_connect()
{
	DEBUG("client_connect");
	char head[HEAD_LEN] = {0};
	set_request_head(head, 0, CLIENT_CONNECT_PIPE, 0);
	write(pipe_event[1], head, HEAD_LEN);
	write(pipe_qt[1], head, HEAD_LEN);
}

void client_disconnect()
{
	DEBUG("client_disconnect");
	char head[HEAD_LEN] = {0};
	set_request_head(head, 0, CLIENT_DISCONNECT_PIPE, 0);
	write(pipe_qt[1], head, HEAD_LEN);
}

void client_online()
{
	DEBUG("client_online");
	char head[HEAD_LEN] = {0};
	set_request_head(head, 0, CLIENT_ONLINE_PIPE, 0);
	write(pipe_qt[1], head, HEAD_LEN);
}

static int set_packet_token(struct client *cli)
{
	if (cli->token)
		free(cli->token);
	cli->token = malloc(read_packet_token(cli->packet) + 1);
	cli->token_size = read_packet_token(cli->packet);
	memcpy(cli->token, cli->data_buf, cli->token_size);
}

#if 0
static int set_heartbeat(struct client *cli)
{
	//if(cli->token)
	//	free(cli->token);
	//cli->token = malloc(read_packet_token(cli->packet) + 1);
	//cli->token_size = read_packet_token(cli->packet);
	//memcpy(cli->token, cli->data_buf, cli->token_size);
	if(cli->)
		free(cli->);
	cli->
}
#endif

int send_upload_log(struct client *cli)
{
	int ret;
	if (!cli->login_flag)
		return ERROR;

	if (cli->data_buf)
		free(cli->data_buf);

	ret = upload_logs(&cli->data_buf, &cli->data_size);

	set_packet_head(cli->packet, UPLOAD_LOG, cli->data_size, BYTE_TYPE, 0);
	return send_packet(cli);
}

static int get_desktop(char *group_uuid)
{
	int i = 0;
	for (i = 0; i < MAX_GROUP; i++)
	{
		DEBUG("group: %s", m_group[i].desktop_group_uuid);
		if (strlen(m_group[i].desktop_group_uuid) != 0 && STRPREFIX(group_uuid, m_group[i].desktop_group_uuid))
		{
			DEBUG("m_group[i].os_uuid: %s", m_group[i].os_uuid);
			DEBUG("m_group[i].data_uuid: %s", m_group[i].data_uuid);
			DEBUG("m_group[i].share_uuid: %s", m_group[i].share_uuid);
			return i;
		}
	}
	return -1;
}

static int set_heartbeat(struct client *cli)
{
	int ret;

	cJSON *root = cJSON_CreateObject();
	if (root)
	{
		int len = 0;
		char *temp = &cli->heartbeat_buf[0];

		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		char *buf = cJSON_Print(root);

		set_packet_head(cli->packet, HEARTBEAT, strlen(buf), JSON_TYPE, 0);
		memcpy(temp, cli->packet, PACKET_LEN);
		temp += PACKET_LEN;
		len += PACKET_LEN;

		memcpy(temp, cli->token, cli->token_size);
		temp += cli->token_size;
		len += cli->token_size;

		memcpy(temp, buf, strlen(buf));
		len += strlen(buf);
		free(buf);
		cli->heartbeat_len = len;
	}
	else
	{
		return ERROR;
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_heartbeat(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];

	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *code = cJSON_GetObjectItem(root, "code");
		cJSON *data = cJSON_GetObjectItem(root, "data");

		if (code && code->valueint == SUCCESS)
		{
			if (!data)
				return ERROR;

			online = 2;

			cJSON *date_time = cJSON_GetObjectItem(data, "datetime");
			DEBUG("data_time: %s", date_time->valuestring);
			//修改时间
			ret = SUCCESS;
		}
		cJSON_Delete(root);
	}
	return ret;
}

int send_heartbeat(struct client *cli)
{
	int ret;
	DEBUG("---------send_heartbeat-------------");

	if (!cli->login_flag)
		return ERROR;

	if (cli->heartbeat_len > 0)
	{
		send_msg(cli->fd, cli->heartbeat_buf, cli->heartbeat_len);
	}

#if 0
	cJSON *root = cJSON_CreateObject();
	if(root)
	{
		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, HEARTBEAT, cli->data_size, JSON_TYPE, 0);
		ret = send_packet(cli);
	}	
	else
	{
		return ERROR;
	}
	if(root)
		cJSON_Delete(root);
	return ret;
#endif
	return SUCCESS;
}

static int send_upgrad(struct client *cli)
{
	return SUCCESS;
}

static int recv_upgrad(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		cJSON *upgrade_package = cJSON_GetObjectItem(root, "upgrade_package");
		cJSON *version = cJSON_GetObjectItem(root, "version");
		cJSON *os_name = cJSON_GetObjectItem(root, "os_name");

		if (!os_name || !upgrade_package || !version)
		{
			return ERROR;
		}

		if (STRPREFIX(os_name->valuestring, "linux"))
		{
			ret = upgrad_programe(upgrade_package->valuestring, version->valuestring, 1);
		}
		else
		{
			ret = upgrad_programe(upgrade_package->valuestring, version->valuestring, 2);
		}
		ret = send_upgrad(cli);
		cJSON_Delete(root);
	}
	return ret;
}

static int send_delete(struct client *cli, int batch_no)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, DELETE, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
		client_disconnect();
		client_connect();
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		ret = ERROR;
	}
	if (root)
		cJSON_Delete(root);

	return ret;
}

static int recv_delete(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		ret = send_delete(cli, batch_no->valueint);
		cJSON_Delete(root);
	}
	return ret;
}

static int send_update_desktop_group_info(struct client *cli, int batch_no)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, UPDATA_DESKTOP_GROUP_INFO, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
		ret = send_get_desktop_group_list(cli);
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		ret = ERROR;
	}
	if (root)
		cJSON_Delete(root);

	return ret;
}

static int recv_update_desktop_group_info(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		ret = send_update_desktop_group_info(cli, batch_no->valueint);
		cJSON_Delete(root);
	}
	return ret;
}

static int send_cancel_send_desktop(struct client *cli, int batch_no)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, CANCEL_SEND_DESKTOP, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
		//stop_torrent();
		clear_task(); // 清除下载任务
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		ret = ERROR;
	}
	if (root)
		cJSON_Delete(root);

	return ret;
}

static int recv_cancel_send_desktop(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		ret = send_cancel_send_desktop(cli, batch_no->valueint);
		cJSON_Delete(root);
	}
	return ret;
}

static int recv_p2v_progress(struct client *cli)
{
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	DEBUG("%s", buf);

	return SUCCESS;
}

int send_p2v_progress(struct client *cli, char *buf)
{
	int ret;
	struct progress_info *info = (struct progress_info *)buf;

	if (!cli->login_flag)
		return ERROR;

	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	if (root)
	{
		if (info->type == 0) //bt 下载
		{
			char buf[128] = {0};

			cJSON_AddStringToObject(root, "torrent_name", info->image_name);
			cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
			cJSON_AddNumberToObject(root, "progress", info->progress);
			cJSON_AddStringToObject(root, "state", info->state);

			sprintf(buf, "%llu", info->download_rate);
			cJSON_AddStringToObject(root, "download_rate", buf);
			sprintf(buf, "%llu", info->upload_rate);
			cJSON_AddStringToObject(root, "upload_rate", buf);
			sprintf(buf, "%llu", info->total_size);
			cJSON_AddStringToObject(root, "total_size", buf);
			sprintf(buf, "%llu", info->file_size);
			cJSON_AddStringToObject(root, "file_size", buf);

			cli->data_buf = cJSON_Print(root);
			cli->data_size = strlen(cli->data_buf);
			set_packet_head(cli->packet, BT_TASK_STATE, cli->data_size, JSON_TYPE, 0);
			ret = send_packet(cli);
		}
		else if (info->type == 1) //p2v 上传
		{
			cJSON_AddStringToObject(root, "os_type", info->file_name);
			cJSON_AddStringToObject(root, "image_name", info->image_name);
			cJSON_AddNumberToObject(root, "progress", info->progress);
			cJSON_AddNumberToObject(root, "status", 1);
			cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
			cJSON_AddStringToObject(root, "storage", info->storage);

			cli->data_buf = cJSON_Print(root);
			cli->data_size = strlen(cli->data_buf);

			set_packet_head(cli->packet, P2V_PROGRESS, cli->data_size, JSON_TYPE, 0);
			ret = send_packet(cli);
		}
		free(cli->data_buf);
		cli->data_buf = NULL;
	}
	else
	{
		ret = ERROR;
	}

	if (root)
		cJSON_Delete(root);

	return ret;
}

static int send_cancel_p2v(struct client *cli, int batch_no)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, CANCEL_P2V, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		return ERROR;
	}

	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_cancel_p2v(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));

	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		cancel_conversion();
		ret = send_cancel_p2v(cli, batch_no->valueint);
		cJSON_Delete(root);
	}

	return ret;
}

static int recv_p2v_transform(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	DEBUG("%s", buf);
	if (root)
	{
		cJSON *code = cJSON_GetObjectItem(root, "code");
		if (code && code->valueint == SUCCESS)
		{
			cJSON *data = cJSON_GetObjectItem(root, "data");
			cJSON *user = cJSON_GetObjectItem(data, "user");
			cJSON *password = cJSON_GetObjectItem(data, "password");
			cJSON *storage = cJSON_GetObjectItem(data, "storage");
			cJSON *image_name = cJSON_GetObjectItem(data, "image_names");

			int i;
			cJSON *item;
			for (i = 0; i < cJSON_GetArraySize(image_name); i++)
			{
				item = cJSON_GetArrayItem(image_name, i);
			}
			DEBUG("item %s", item->valuestring);

			struct server_info *server = &(conf.server);
			struct p2v_task task = {0};

			strcpy(&task.server_ip, server->ip);
			if (user)
				strcpy(&task.user, user->valuestring);
			if (password)
				strcpy(&task.passwd, password->valuestring);
			if (storage)
				strcpy(&task.storage, storage->valuestring);
			if (item)
				strcpy(&task.image_name, item->valuestring);
			//if(template_name)
			//strcpy(&task.template_name, );

			DEBUG("user->valuestring %s password->valuestring %s storage->valuestring %s", user->valuestring,
				  password->valuestring, storage->valuestring);

			en_queue(&task_queue, (char *)&task, sizeof(struct p2v_task), 0x2);
			ret = SUCCESS;
		}
		else
		{
			send_error_msg(P2V_NAME_ERR);
		}
		cJSON_Delete(root);
	}
	return ret;
}

int send_p2v_transform(struct client *cli, char *data)
{
	int ret;
	if (!cli->login_flag)
		return ERROR;

	if (cli->data_buf)
		free(cli->data_buf);

	struct template_info *info = (struct template_info *)data;

	cJSON *root = cJSON_CreateObject();
	cJSON *system_disk = cJSON_CreateObject();
	cJSON *data_disks = cJSON_CreateObject();

	if (root && system_disk && data_disks)
	{
		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		cJSON_AddStringToObject(root, "name", info->template_name);
		cJSON_AddStringToObject(root, "desc", info->template_desc);
		cJSON_AddNumberToObject(root, "classify", 1);

		cJSON_AddItemToObject(root, "system_disk", system_disk);

		cJSON_AddNumberToObject(system_disk, "size", conf.terminal.disk_size);
		cJSON_AddNumberToObject(system_disk, "real_size", conf.terminal.disk_size);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);

		set_packet_head(cli->packet, P2V_OS_TRANSFORM, cli->data_size, JSON_TYPE, 0);
		ret = send_packet(cli);

		free(cli->data_buf);
		cli->data_buf = NULL;
	}
	else
	{
		if (system_disk)
			cJSON_Delete(system_disk);
		if (data_disks)
			cJSON_Delete(data_disks);
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int send_desktop(struct client *cli, int batch_no, int flag)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	DEBUG("batch_no: %d ret: %d", batch_no, flag);

	if (root && data)
	{
		if (flag)
		{
			cJSON_AddNumberToObject(root, "code", 10001);
			cJSON_AddStringToObject(root, "msg", "voi os is already exist ");
		}
		else
		{
			cJSON_AddNumberToObject(root, "code", 0);
			cJSON_AddStringToObject(root, "msg", "Success");
		}

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, SEND_DESKTOP, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		return ERROR;
	}

	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_desktop(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	//DEBUG("%s", buf);
	update_desktop(buf);
	if (root)
	{
		cJSON *desktop = cJSON_GetObjectItem(root, "desktop");
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		if (desktop)
		{
			cJSON *desktop_group_name = cJSON_GetObjectItem(desktop, "desktop_group_name");

			cJSON *disks = cJSON_GetObjectItem(desktop, "disks");
			int i, flag = 0;
			cJSON *uuid, *type, *prefix, *dif_level, *real_size, *reserve_size, *torrent_file, *restore_flag;
			for (i = 0; i < cJSON_GetArraySize(disks); i++)
			{
				cJSON *item = cJSON_GetArrayItem(disks, i);

				uuid = cJSON_GetObjectItem(item, "uuid");
				type = cJSON_GetObjectItem(item, "type");
				prefix = cJSON_GetObjectItem(item, "prefix");
				dif_level = cJSON_GetObjectItem(item, "dif_level");
				real_size = cJSON_GetObjectItem(item, "real_size");
				reserve_size = cJSON_GetObjectItem(item, "reserve_size");
				torrent_file = cJSON_GetObjectItem(item, "torrent_file");
				restore_flag = cJSON_GetObjectItem(item, "restore_flag");

				if (uuid && dif_level)
				{
					//if(scan_qcow2(uuid->valuestring, dif_level->valueint))
					//if(desktop_group_name)
					//	strcpy(m_desktop_group_name[current_group++], desktop_group_name->valuestring);

					del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);
					//else
					//	return send_desktop(cli, batch_no->valueint, ERROR);
				}
			}
			ret = send_desktop(cli, batch_no->valueint, SUCCESS);
		}
		cJSON_Delete(root);
	}
	return ret;
}

static int send_down_torrent(struct client *cli, char *task_uuid, int flag)
{
	int ret;

	if (cli->data_buf)
		free(cli->data_buf);

	DEBUG("task_uuid %s flag %d", task_uuid, flag);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		if (flag)
		{
			cJSON_AddNumberToObject(root, "code", 10002);
			cJSON_AddStringToObject(root, "msg", "bt torrent file error");
		}
		else
		{
			cJSON_AddNumberToObject(root, "code", 0);
			cJSON_AddStringToObject(root, "msg", "Success");
		}

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddStringToObject(data, "batch_no", task_uuid);

		cli->data_buf = cJSON_Print(root);
		DEBUG("%s", cli->data_buf);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, SEND_DOWN_TORRENT, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		return ERROR;
	}

	if (root)
		cJSON_Delete(root);
	return ret;
}


static int recv_down_torrent(struct client *cli)
{
	int ret;
	char torrent_file[128] = {0};
	char task_uuid[36 + 1] = {0};
	char uuid[36 + 1] = {0};
	//int current_group = -1;
	struct desktop_group *current_group = NULL;

	yzy_torrent *torrent = (yzy_torrent *)&cli->data_buf[read_packet_supplementary(cli->packet) +
														 read_packet_token(cli->packet)];
	DEBUG("torrent->uuid %s", torrent->uuid);
	DEBUG("torrent->type %d", torrent->type);
	DEBUG("torrent->sys_type %d", torrent->sys_type);
	DEBUG("torrent->dif_level %d", (torrent->dif_level));
	DEBUG("torrent->real_size %lld", torrent->real_size);
	DEBUG("torrent->space_size %lld", torrent->space_size);
	DEBUG("torrent->file_size %lld", torrent->file_size);
	DEBUG("torrent->data_len %lld", torrent->data_len);
	DEBUG("torrent->operate_id %d", torrent->operate_id);
	DEBUG("torrent->group_uuid %s", torrent->group_uuid);

	memcpy(task_uuid, torrent->task_uuid, 36);
	memcpy(uuid, torrent->uuid, 36);
	DEBUG("task_uuid %s", task_uuid);

	char *data = &cli->data_buf[read_packet_supplementary(cli->packet) +
								read_packet_token(cli->packet) + sizeof(yzy_torrent)];

	sprintf(torrent_file, "/root/voi_%d_%s.torrent", torrent->dif_level, uuid);
	FILE *fp = fopen(torrent_file, "wb");

	ret = get_desktop(torrent->group_uuid);
	if (ret != -1)
		current_group = &m_group[ret];
	else
	{
		DEBUG("no found group info error");
		return ERROR;
	}

	if (fp)
	{
		ret = fwrite(data, 1, torrent->data_len, fp);
		fflush(fp);
		fclose(fp);

#if 0
		if (torrent->type == 1)
		{
			memcpy(temp_uuid, current_group->share_uuid, 36);
		}
		else if (torrent->type == 2)
		{
			memcpy(temp_uuid, current_group->data_uuid, 36);
		}
		else
		{
			memcpy(temp_uuid, current_group->os_uuid, 36);
		}
#endif

		if (ret == torrent->data_len)
		{
			uint64_t offset = 0;
			if (torrent->dif_level == 1)
			{
				if (current_group->diff_mode == 1)
				{
					DEBUG("------------- diff mode 1 --------------");
#if 0
					offset = add_qcow2(dev_info.mini_disk->dev, torrent->uuid, torrent->dif_level,
								(uint64_t)(torrent->file_size) + 1024 * 1024 * 4,  
								torrent->real_size, torrent->sys_type, torrent->type, torrent->operate_id);
#else
					offset = add_qcow2(dev_info.mini_disk->dev, uuid, torrent->dif_level,
									   (uint64_t)(torrent->real_size),
									   torrent->real_size, torrent->sys_type, torrent->type, torrent->operate_id, INCRMENT_MODE);
#endif
				}
				else
				{
					DEBUG("---------- diff mode 0 --------------");
					offset = add_qcow2(dev_info.mini_disk->dev, uuid, torrent->dif_level,
									   (uint64_t)(torrent->file_size) + 1024 * 2,
									   torrent->real_size, torrent->sys_type, torrent->type, torrent->operate_id, COVERAGE_MODE);
				}
			}
			else
			{
				offset = add_qcow2(dev_info.mini_disk->dev, uuid, torrent->dif_level,
								   (uint64_t)(torrent->file_size),
								   torrent->real_size, torrent->sys_type, torrent->type, torrent->operate_id, 0);
			}

			if (offset != 0)
			{
				struct torrent_task task = {0};
				memcpy(task.uuid, torrent->uuid, 36);
				memcpy(task.group_uuid, torrent->group_uuid, 36);
				memcpy(task.torrent_file, torrent_file, strlen(torrent_file));

				if (torrent->dif_level == 3)
					torrent->dif_level = 2;

				if (torrent->type == 0)
				{
					sprintf(task.file_name, "%s_系统盘_%d", current_group->desktop_group_name, torrent->dif_level);
				}
				else if (torrent->type == 1)
				{
					sprintf(task.file_name, "%s_数据盘_%d", current_group->desktop_group_name, torrent->dif_level);
				}
				else if (torrent->type == 2)
				{
					strcpy(task.file_name, "共享盘");
				}

				task.diff = torrent->dif_level;
				task.disk_type = torrent->type;
				task.offset = offset;
				en_queue(&task_queue, (char *)&task, sizeof(struct torrent_task), 0x0);
				return send_down_torrent(cli, task_uuid, SUCCESS);
			}
			else
			{
				DEBUG("add_qcow2 fail torrent->file_size %lld torrent->real_size %lld", torrent->file_size, torrent->real_size);
				clear_task();
				send_error_msg(BT_DISK_FULL_ERR);
				return send_down_torrent(cli, task_uuid, ERROR);
			}
		}
		else
		{
			DEBUG("ret %d != torrent->data_len %d", ret, torrent->data_len);
			return send_down_torrent(cli, task_uuid, ERROR);
		}
	}
	return ERROR;
}

static int send_clear_all_desktop(struct client *cli, int batch_no)
{
	int ret;
	free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, CLEAR_ALL_DESKTOP, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		ret = ERROR;
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_clear_all_desktop(struct client *cli)
{
	int ret;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	DEBUG("clear_all_desktop");
	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		//stop_torrent();
		//stop_torrent
		clear_task();
		init_qcow2(dev_info.mini_disk->dev, 0);
		ret = send_clear_all_desktop(cli, batch_no->valueint);
		cJSON_Delete(root);
	}
	return ret;
}

static int send_update_diff_disk(struct client *cli, int batch_no)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, UPDATE_DIFF_DISK, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		ret = ERROR;
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_update_diff_disk(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];

	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");

		cJSON *disk_info_list = cJSON_GetObjectItem(root, "disk_info_list");
		if (!disk_info_list)
			goto end;

		int i;
		cJSON *name, *uuid, *version, *download_url;
		for (i = 0; i < cJSON_GetArraySize(disk_info_list); i++)
		{
			cJSON *item = cJSON_GetArrayItem(disk_info_list, i);
			name = cJSON_GetObjectItem(item, "name");
			uuid = cJSON_GetObjectItem(item, "uuid");
			version = cJSON_GetObjectItem(item, "version");
			download_url = cJSON_GetObjectItem(item, "download_url");
		}
		ret = send_update_diff_disk(cli, batch_no->valueint);
	}
end:
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int send_update_config(struct client *cli, int batch_no)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, UPDATE_CONFIG, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
		client_disconnect();
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		ret = ERROR;
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_update_config(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];

	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");

		cJSON *mode = cJSON_GetObjectItem(root, "mode");
		if (!mode)
			goto end;

		cJSON *show_desktop_type = cJSON_GetObjectItem(mode, "show_desktop_type");
		cJSON *auto_desktop = cJSON_GetObjectItem(mode, "auto_desktop");

		cJSON *program = cJSON_GetObjectItem(root, "program");
		if (!program)
			goto end;

		cJSON *server_ip = cJSON_GetObjectItem(program, "server_ip");

		if (show_desktop_type)
			conf.terminal.desktop_type = show_desktop_type->valueint;
		if (auto_desktop)
			conf.terminal.auto_desktop = auto_desktop->valueint;
		if (server_ip)
			strcpy(conf.server.ip, server_ip->valuestring);
		save_config();
		send_config_pipe();
		ret = send_update_config(cli, batch_no->valueint);
	}

end:
	if (root)
		cJSON_Delete(root);

	return ret;
}

static int send_update_ip(struct client *cli, int batch_no)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, UPDATE_IP, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		return ERROR;
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_update_ip(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];

	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		cJSON *ip = cJSON_GetObjectItem(root, "ip");
		cJSON *mask = cJSON_GetObjectItem(root, "mask");
		cJSON *gateway = cJSON_GetObjectItem(root, "gateway");
		cJSON *dns1 = cJSON_GetObjectItem(root, "dns1");
		cJSON *dns2 = cJSON_GetObjectItem(root, "dns2");
		cJSON *conf_version = cJSON_GetObjectItem(root, "conf_version");

		if (ip)
			strcpy(conf.netcard.ip, ip->valuestring);
		if (mask)
			strcpy(conf.netcard.netmask, mask->valuestring);
		if (gateway)
			strcpy(conf.netcard.gateway, gateway->valuestring);
		if (dns1)
			strcpy(conf.netcard.dns1, dns1->valuestring);
		if (dns2)
			strcpy(conf.netcard.dns2, dns2->valuestring);
		if (conf_version)
			conf.config_ver = conf_version->valueint;
		save_config();
		set_network(ip->valuestring, mask->valuestring);
		send_config_pipe();
		ret = send_update_ip(cli, batch_no->valueint);
		cJSON_Delete(root);
	}
	return ret;
}

static int send_update_name(struct client *cli, int batch_no)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, UPDATE_NAME, cli->data_size, JSON_TYPE, 1);
		ret = send_packet(cli);
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		ret = ERROR;
	}

	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_update_name(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		cJSON *name = cJSON_GetObjectItem(root, "name");
		cJSON *conf_version = cJSON_GetObjectItem(root, "conf_version");

		if (name)
			strcpy(conf.terminal.name, name->valuestring);
		if (conf_version)
			conf.config_ver = conf_version->valueint;

		save_config();
		send_config_pipe();
		ret = send_update_name(cli, batch_no->valueint);
		cJSON_Delete(root);
	}
	return ret;
}

static int send_reboot(struct client *cli, int batch_no, int flag)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	cJSON *data = cJSON_CreateObject();

	if (root && data)
	{
		cJSON_AddNumberToObject(root, "code", 0);
		cJSON_AddStringToObject(root, "msg", "Success");

		cJSON_AddItemToObject(root, "data", data);

		cJSON_AddStringToObject(data, "mac", conf.netcard.mac);
		cJSON_AddNumberToObject(data, "batch_no", batch_no);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		char head[HEAD_LEN] = {0};
		if (flag)
		{
			set_packet_head(cli->packet, RESTART, cli->data_size, JSON_TYPE, 1);
			ret = send_packet(cli);
			ret = send_pipe(head, REBOOT_PIPE, 0, PIPE_EVENT);
		}
		else
		{
			set_packet_head(cli->packet, SHUTDONW, cli->data_size, JSON_TYPE, 1);
			ret = send_packet(cli);
			ret = send_pipe(head, SHUTDOWN_PIPE, 0, PIPE_EVENT);
		}
	}
	else
	{
		if (data)
			cJSON_Delete(data);
		ret = ERROR;
	}
	if (root)
		cJSON_Delete(root);

	return SUCCESS;
}

static int recv_reboot(struct client *cli, int flag)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	DEBUG("reboot %d", flag);
	if (root)
	{
		cJSON *batch_no = cJSON_GetObjectItem(root, "batch_no");
		ret = send_reboot(cli, batch_no->valueint, flag);
		cJSON_Delete(root);
	}
	return ret;
}

static int recv_get_diff_torrent(struct client *cli)
{
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	DEBUG("%s", buf);
	return SUCCESS;
}

int send_get_diff_torrent(struct client *cli, char *group_uuid, char *diff_uuid, int diff)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	if (root)
	{
		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		cJSON_AddStringToObject(root, "desktop_group_uuid", group_uuid);
		cJSON_AddStringToObject(root, "diff_disk_uuid", diff_uuid);
		cJSON_AddNumberToObject(root, "diff_level", diff);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		DEBUG("%s", cli->data_buf);

		set_packet_head(cli->packet, DIFF_DOWN_TORRENT, cli->data_size, JSON_TYPE, 0);
		ret = send_packet(cli);
	}
	else
	{
		DEBUG("cJSON_CreateObject error");
		return ERROR;
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_get_desktop_group_list(struct client *cli)
{
	int ret;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	//DEBUG("%s", buf);

	if (root)
	{
		cJSON *code = cJSON_GetObjectItem(root, "code");
		cJSON *data = cJSON_GetObjectItem(root, "data");

		if (code && code->valueint == SUCCESS && data)
		{
			cJSON *desktop_group_list = cJSON_GetObjectItem(data, "desktop_group_list");
			int i, j;
			cJSON *auto_update_desktop, *default_desktop_group, *desktop_dns1, *desktop_dns2, *desktop_gateway,
				*desktop_group_desc, *desktop_group_name, *desktop_group_restore, *desktop_group_status,
				*desktop_group_uuid, *desktop_ip, *desktop_is_dhcp, *desktop_mask, *desktop_name, *disks,
				*os_sys_type, *show_desktop_info, *diff_mode;

			cJSON *dif_level, *prefix, *real_size, *reserve_size, *type, *uuid, *max_diff, *operate_id, *dif_level_next,
				*operate_id_next;

			for (i = 0; i < cJSON_GetArraySize(desktop_group_list); i++)
			{
				cJSON *item = cJSON_GetArrayItem(desktop_group_list, i);
				if (!item)
					continue;

				auto_update_desktop = cJSON_GetObjectItem(item, "auto_update_desktop");
				desktop_group_uuid = cJSON_GetObjectItem(item, "desktop_group_uuid");
				desktop_group_name = cJSON_GetObjectItem(item, "desktop_group_name");
				disks = cJSON_GetObjectItem(item, "disks");
				diff_mode = cJSON_GetObjectItem(item, "diff_mode");

				m_group[i].auto_update = auto_update_desktop->valueint;
				m_group[i].diff_mode = diff_mode->valueint;

				if (disks && desktop_group_uuid && desktop_group_name)
				{
					memcpy(m_group[i].desktop_group_uuid, desktop_group_uuid->valuestring, 36);
					strcpy(m_group[i].desktop_group_name, desktop_group_name->valuestring);

					for (j = 0; j < cJSON_GetArraySize(disks); j++)
					{
						item = cJSON_GetArrayItem(disks, j);
						if (!item)
							continue;
						max_diff = cJSON_GetObjectItem(item, "max_dif");
						uuid = cJSON_GetObjectItem(item, "uuid");
						dif_level = cJSON_GetObjectItem(item, "dif_level");
						prefix = cJSON_GetObjectItem(item, "prefix");
						real_size = cJSON_GetObjectItem(item, "real_size");
						reserve_size = cJSON_GetObjectItem(item, "reserve_size");
						type = cJSON_GetObjectItem(item, "type");
						operate_id = cJSON_GetObjectItem(item, "operate_id");

						if (!max_diff || !uuid || !dif_level || !prefix || !real_size || !reserve_size || !type || !operate_id)
							continue;

						if (dif_level->valueint >= 3) //防止数据越界
							continue;

						if (type->valueint == 0)
						{
							memcpy(m_group[i].os_uuid, uuid->valuestring, 36);
						}
						else if (type->valueint == 1)
						{
							memcpy(m_group[i].data_uuid, uuid->valuestring, 36);
						}
						else if (type->valueint == 2)
						{
							memcpy(m_group[i].share_uuid, uuid->valuestring, 36); //共享盘特殊处理

							if (operate_id->valueint != get_operate_qcow2(uuid->valuestring, 0)) //暂定没有自动更新标识
							{
								DEBUG("update share qcow2 disk ");
								del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 0);
								del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);

								send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 0);
							}
						}

						//&& auto_update_desktop->valueint
						if (dif_level->valueint == 1 && auto_update_desktop->valueint)
						{
							if (scan_qcow2(uuid->valuestring, 0))
							{
								if (!scan_qcow2(uuid->valuestring, 1)) //不存在 1
								{
									DEBUG("update qcow2 %s no find diff 1 update diff 1 and 2 ", uuid->valuestring);
									del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);
									send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 1);
									send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 2);
									//if(desktop_group_name)
									//	strcpy(m_desktop_group_name[current_group++], desktop_group_name->valuestring);
								}
								else //存在 1
								{

									/* diff mode 切换 */
									if (get_diff_mode_qcow2(uuid->valuestring) != (diff_mode->valueint + 1))
									{
										DEBUG("diff mode %d no equal %d update diff 1 and 2",
											  get_diff_mode_qcow2(uuid->valuestring), diff_mode->valueint);
										del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);
										send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 1);
										send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 2);
										//if(desktop_group_name)
										//	strcpy(m_desktop_group_name[current_group++], desktop_group_name->valuestring);
										continue;
									}

									//item = cJSON_GetArrayItem(disks, j + 1);
									item = cJSON_GetArrayItem(disks, ++j);
									if (item) //diff level 2
									{
										dif_level_next = cJSON_GetObjectItem(item, "dif_level");
										operate_id_next = cJSON_GetObjectItem(item, "operate_id");
									}

									if (!dif_level_next || !operate_id_next) //不存在 2
									{
										if (operate_id->valueint != get_operate_qcow2(uuid->valuestring, 1)) //1 操作号不同
										{
											DEBUG("update qcow2 %s no find 2 update diff only 1 ", uuid->valuestring);
											del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);
											send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 1);

											//	if(desktop_group_name)
											//		strcpy(m_desktop_group_name[current_group++], desktop_group_name->valuestring);
										}
									}
									else //存在 1 2
									{
										DEBUG("dif_level->valueint %d", dif_level->valueint);
										DEBUG("operate_id->valueint %d", operate_id->valueint);
										DEBUG("dif_level_next->valueint %d", dif_level_next->valueint);
										DEBUG("operate_id_next->valueint %d", operate_id_next->valueint);
										if (operate_id->valueint != get_operate_qcow2(uuid->valuestring, 1)) //1 操作号不同
										{
											if (diff_mode->valueint && operate_id->valueint == get_operate_qcow2(uuid->valuestring, 1) + 1 &&
												operate_id_next->valueint == get_operate_qcow2(uuid->valuestring, 2) + 1 &&
												!(scan_qcow2(uuid->valuestring, 3)))

											{

												DEBUG("update qcow2 %s download diff 3 for commit ", uuid->valuestring);
												del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 4);
												del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 5);
												send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 3);

												//if(desktop_group_name)
												//	strcpy(m_desktop_group_name[current_group++], desktop_group_name->valuestring);
											}
											else
											{
												DEBUG("update qcow2 %s download diff 1 and 2 no commit ", uuid->valuestring);
												del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);
												send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 1);
												send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 2);
												//if(desktop_group_name)
												//	strcpy(m_desktop_group_name[current_group++], desktop_group_name->valuestring);
											}
										}
										else if (operate_id_next->valueint != get_operate_qcow2(uuid->valuestring, 2)) //1 相同 2 不同
										{
											DEBUG("update qcow2 %s download diff only 2 ", uuid->valuestring);
											del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 3);
											del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 4);
											del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 5);
											send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 2);

											//if(desktop_group_name)
											//	strcpy(m_desktop_group_name[current_group++], desktop_group_name->valuestring);
										}
										else
										{
											DEBUG("diff 1 and diff 2 operate equal no update");
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (root)
		cJSON_Delete(root);
	return SUCCESS;
}

static int send_get_desktop_group_list(struct client *cli)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	DEBUG("send_get_desktop_group_list");
	cJSON *root = cJSON_CreateObject();
	if (root)
	{
		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		cJSON_AddStringToObject(root, "ip", conf.netcard.ip);
		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);

		set_packet_head(cli->packet, GET_DESKTOP_GROUP_LIST, cli->data_size, JSON_TYPE, 0);
		ret = send_packet(cli);
	}
	else
	{
		DEBUG("cJSON_CreateObject error");
		return ERROR;
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_get_config(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *code = cJSON_GetObjectItem(root, "code");
		if (code && code->valueint == SUCCESS)
		{
			cJSON *data = cJSON_GetObjectItem(root, "data");
			if (!data)
				goto run_out;

			cJSON *conf_version = cJSON_GetObjectItem(data, "conf_version");
			cJSON *terminal_id = cJSON_GetObjectItem(data, "terminal_id");
			cJSON *dns1 = cJSON_GetObjectItem(data, "dns1");
			cJSON *dns2 = cJSON_GetObjectItem(data, "dns2");
			cJSON *gateway = cJSON_GetObjectItem(data, "gateway");
			cJSON *ip = cJSON_GetObjectItem(data, "ip");
			cJSON *is_dhcp = cJSON_GetObjectItem(data, "is_dhcp");
			cJSON *mac = cJSON_GetObjectItem(data, "mac");
			cJSON *mask = cJSON_GetObjectItem(data, "mask");
			cJSON *name = cJSON_GetObjectItem(data, "name");
			cJSON *platform = cJSON_GetObjectItem(data, "platform");

			cJSON *setup_info = cJSON_GetObjectItem(data, "setup_info");
			if (!setup_info)
				goto run_out;
			cJSON *mode = cJSON_GetObjectItem(setup_info, "mode");
			if (!mode)
				goto run_out;

			cJSON *auto_desktop = cJSON_GetObjectItem(mode, "auto_desktop");
			cJSON *show_desktop_type = cJSON_GetObjectItem(mode, "show_desktop_type");

			cJSON *program = cJSON_GetObjectItem(setup_info, "program");
			if (!program)
				goto run_out;

			cJSON *server_ip = cJSON_GetObjectItem(program, "server_ip");

			if (conf_version)
				conf.config_ver = conf_version->valueint;

			if (terminal_id)
				conf.terminal.id = terminal_id->valueint;

			if (is_dhcp)
				conf.netcard.is_dhcp = is_dhcp->valueint;

			if (!conf.netcard.is_dhcp)
			{
				if (ip)
					strcpy(conf.netcard.ip, ip->valuestring);
				if (dns1)
					strcpy(conf.netcard.dns1, dns1->valuestring);
				if (dns2)
					strcpy(conf.netcard.dns2, dns2->valuestring);
				if (gateway)
					strcpy(conf.netcard.gateway, gateway->valuestring);
				if (mask)
					strcpy(conf.netcard.netmask, mask->valuestring);
			}
			if (name)
				strcpy(conf.terminal.name, name->valuestring);
			if (platform)
				strcpy(conf.terminal.platform, platform->valuestring);
			if (auto_desktop)
				conf.terminal.auto_desktop = auto_desktop->valueint;
			if (show_desktop_type)
				conf.terminal.desktop_type = show_desktop_type->valueint;
			if (server_ip)
				strcpy(conf.server.ip, server_ip->valuestring);

			DEBUG("is_dhcp->valueint %d", is_dhcp->valueint);
			DEBUG("conf.netcard.ip %s", conf.netcard.ip);
			DEBUG("conf.terminal.name %s", conf.terminal.name);
			DEBUG("conf.install_flag %d", conf.install_flag);
			save_config();
			//update_config();
			send_config_pipe();
			if (conf.install_flag && first_time)
			{
				ret = send_get_desktop_group_list(cli);
				first_time = 0;
			}
			else
				ret = SUCCESS;
		}
	}
run_out:

	if (root)
		cJSON_Delete(root);
	return ret;
}

static int send_get_config(struct client *cli)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	if (root)
	{
		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);

		set_packet_head(cli->packet, GET_CONFIG_INFO, cli->data_size, JSON_TYPE, 0);
		ret = send_packet(cli);
	}
	else
	{
		DEBUG("cJSON_CreateObject error");
		return ERROR;
	}
	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_set_update_config(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];

	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *code = cJSON_GetObjectItem(root, "code");
		if (code && code->valueint == SUCCESS)
		{
			ret = send_get_config(cli);
		}
		cJSON_Delete(root);
	}
	return ret;
}

static int send_set_update_config(struct client *cli)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);
	cJSON *root = cJSON_CreateObject();
	cJSON *setup_info = cJSON_CreateObject();
	cJSON *program = cJSON_CreateObject();
	cJSON *mode = cJSON_CreateObject();
	if (root && setup_info && program && mode)
	{
		cJSON_AddNumberToObject(root, "terminal_id", conf.terminal.id);
		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		if (strlen(conf.terminal.name) == 0)
			cJSON_AddStringToObject(root, "name", "default");
		else
			cJSON_AddStringToObject(root, "name", conf.terminal.name);
		cJSON_AddStringToObject(root, "ip", conf.netcard.ip);
		cJSON_AddNumberToObject(root, "is_dhcp", conf.netcard.is_dhcp);
		cJSON_AddStringToObject(root, "mask", "255.255.255.0");
		cJSON_AddStringToObject(root, "gateway", conf.netcard.gateway);
		cJSON_AddStringToObject(root, "dns1", conf.netcard.dns1);
		cJSON_AddStringToObject(root, "dns2", conf.netcard.dns2);
		cJSON_AddStringToObject(root, "platform", "x86");

		char tmp[12] = {0};
		sprintf(tmp, "%d.%d", conf.major_ver, conf.minor_ver);
		cJSON_AddStringToObject(root, "soft_version", tmp);
		cJSON_AddNumberToObject(root, "conf_version", conf.config_ver);
		cJSON_AddNumberToObject(root, "disk_residue", conf.terminal.disk_size);

		cJSON_AddItemToObject(root, "setup_info", setup_info);

		cJSON_AddItemToObject(setup_info, "mode", mode);

		cJSON_AddNumberToObject(mode, "show_desktop_type", conf.terminal.desktop_type);
		cJSON_AddNumberToObject(mode, "auto_desktop", conf.terminal.auto_desktop);

		cJSON_AddItemToObject(setup_info, "program", program);

		cJSON_AddItemToObject(program, "server_ip", cJSON_CreateString(conf.server.ip));

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		DEBUG("%s", cli->data_buf);

		set_packet_head(cli->packet, UPDATE_CONFIG_INTO, cli->data_size, JSON_TYPE, 0);
		ret = send_packet(cli);
	}
	else
	{
		DEBUG("cJSON_CreateObject error");
		if (setup_info)
			cJSON_Delete(setup_info);
		if (mode)
			cJSON_Delete(mode);
		ret = ERROR;
	}

	if (root)
		cJSON_Delete(root);

	return ret;
}

static int recv_config_version(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];

	cJSON *root = cJSON_Parse((char *)(buf));
	DEBUG("%s", buf);
	if (root)
	{
		cJSON *code = cJSON_GetObjectItem(root, "code");
		if (code && code->valueint == SUCCESS)
		{
			cJSON *data = cJSON_GetObjectItem(root, "data");
			cJSON *config_ver = cJSON_GetObjectItem(data, "conf_version");

			if (config_ver->valueint == -1 || config_ver->valueint <= conf.config_ver || conf.config_ver == -1) //没有配置上传配置
			{
				DEBUG("upload config data");
				ret = send_set_update_config(cli);
			}
			else if (config_ver->valueint > conf.config_ver) //更新本地配置
			{
				DEBUG("updata load config data");
				ret = send_get_config(cli);
			}
		}
		cJSON_Delete(root);
	}
	return ret;
}

static int send_config_version(struct client *cli)
{
	int ret;
	if (cli->data_buf)
		free(cli->data_buf);

	cJSON *root = cJSON_CreateObject();
	if (root)
	{
		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, GET_CONFIG_VERSION, cli->data_size, JSON_TYPE, 0);

		ret = send_packet(cli);
	}
	else
	{
		DEBUG("cJSON_CreateObject error");
		return ERROR;
	}

	if (root)
		cJSON_Delete(root);
	return ret;
}

static int recv_login(struct client *cli)
{
	int ret = ERROR;
	char *buf = &cli->data_buf[read_packet_token(cli->packet)];
	cJSON *root = cJSON_Parse((char *)(buf));
	if (root)
	{
		cJSON *code = cJSON_GetObjectItem(root, "code");
		if (code && code->valueint == SUCCESS)
		{
			DEBUG("login ok !!!");
			cli->login_flag = 1;
			set_packet_token(cli);
			set_heartbeat(cli);
			ret = send_config_version(cli);
		}
		cJSON_Delete(root);
	}
	return ret;
}

static int send_login(struct client *cli)
{
	int ret;
	cJSON *root = cJSON_CreateObject();
	if (root)
	{
		cJSON_AddStringToObject(root, "mac", conf.netcard.mac);
		cJSON_AddStringToObject(root, "ip", conf.netcard.ip);

		cli->data_buf = cJSON_Print(root);
		cli->data_size = strlen(cli->data_buf);
		set_packet_head(cli->packet, TERMINAL_LOGIN, cli->data_size, JSON_TYPE, 0);
		ret = send_packet(cli);
		DEBUG("%s", cli->data_buf);
	}
	else
	{
		DEBUG("cJSON_CreateObject error");
		return ERROR;
	}

	if (root)
		cJSON_Delete(root);
	return ret;
}

static int process_msg(struct client *cli)
{
	int ret;
	DEBUG("read_packet_order(cli->packet) %d", read_packet_order(cli->packet));
	switch (read_packet_order(cli->packet))
	{
	case TERMINAL_LOGIN:
		ret = recv_login(cli);
		break;
	case GET_CONFIG_VERSION:
		ret = recv_config_version(cli);
		break;
	case UPDATE_CONFIG_INTO:
		ret = recv_set_update_config(cli);
		break;
	case GET_CONFIG_INFO:
		ret = recv_get_config(cli);
		break;
	case SEND_DOWN_TORRENT:
		ret = recv_down_torrent(cli);
		break;
	case SEND_DESKTOP:
		ret = recv_desktop(cli);
		break;
	case SHUTDONW:
		ret = recv_reboot(cli, 0);
		break;
	case RESTART:
		ret = recv_reboot(cli, 1);
		break;
	case UPDATE_IP:
		ret = recv_update_ip(cli);
		break;
	case UPDATE_NAME:
		ret = recv_update_name(cli);
		break;
	case UPDATE_DIFF_DISK:
		ret = recv_update_diff_disk(cli);
		break;
	case UPDATE_CONFIG:
		ret = recv_update_config(cli);
		break;
	case CLEAR_ALL_DESKTOP:
		ret = recv_clear_all_desktop(cli);
		break;
	case GET_DESKTOP_GROUP_LIST:
		ret = recv_get_desktop_group_list(cli);
		break;
	case HEARTBEAT:
		ret = recv_heartbeat(cli);
		break;
	case P2V_OS_TRANSFORM:
		ret = recv_p2v_transform(cli);
		break;
	case P2V_PROGRESS:
		ret = recv_p2v_progress(cli);
		break;
	case DIFF_DOWN_TORRENT:
		ret = recv_get_diff_torrent(cli);
		break;
	case UPDATA_DESKTOP_GROUP_INFO:
		ret = recv_update_desktop_group_info(cli);
		break;
	case CANCEL_SEND_DESKTOP:
		ret = recv_cancel_send_desktop(cli);
		break;
	case CANCEL_P2V:
		ret = recv_cancel_p2v(cli);
		break;
	case DELETE:
		ret = recv_delete(cli);
		break;
	case UPGRAD:
		ret = recv_upgrad(cli);
		break;
	default:
		ret = SUCCESS;
		break;
	}
	return ret;
}

static int tcp_loop(int sockfd)
{
	int ret;
	int maxfd = 0, nready, i, maxi = 0;
	fd_set allset, reset;

	FD_ZERO(&allset);
	FD_SET(sockfd, &allset);
	FD_SET(pipe_tcp[0], &allset);

	maxfd = maxfd > sockfd ? maxfd : sockfd;
	maxfd = maxfd > pipe_tcp[0] ? maxfd : pipe_tcp[0];

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	char stop = 'S';
	char buf[DATA_SIZE] = {0};
	char *tmp = &buf[HEAD_LEN];
	struct client *current = &m_client;

	for (;;)
	{
		tv.tv_sec = 1;
		reset = allset;
		ret = select(maxfd + 1, &reset, NULL, NULL, &tv);
		if (ret == -1)
		{
			if (errno == EINTR)
				continue;
			else if (errno != EBADF)
			{
				DEBUG("select %s", strerror(ret));
				break;
			}
		}
		nready = ret;
		/* pipe msg */
		if (FD_ISSET(pipe_tcp[0], &reset))
		{
			ret = recv(pipe_tcp[0], (void *)buf, sizeof(buf), 0);
			if (ret >= HEAD_LEN)
			{
				//process_pipe(buf, ret);
			}
			else if (ret == 1)
			{
				if (buf[0] == 'S')
				{
					DEBUG("event thread pipe msg exit");
					break;
				}
			}
			if (--nready <= 0)
				continue;
		}
		if (FD_ISSET(sockfd, &reset))
		{
			if (current->has_read_head == 0)
			{
				if ((ret = recv(current->fd, current->packet + current->pos, PACKET_LEN - current->pos, 0)) <= 0)
				{
					if (ret < 0)
					{
						if (errno == EINTR || errno == EAGAIN)
						{
							//pthread_mutex_unlock(&client_mutex);
							continue;
						}
					}
					DEBUG("close fd %d", sockfd);
					break;
				}

				current->pos += ret;
				if (current->pos != PACKET_LEN)
				{
					continue;
				}

				current->has_read_head = 1;
				current->data_size = read_packet_size(current->packet) + read_packet_token(current->packet);
				current->pos = 0;

				if (current->data_size < 0 || current->data_size > CLIENT_BUF)
				{
					current->pos = 0;
					current->has_read_head = 0;
					continue;
				}
				else if (current->data_size > 0)
				{
					if (current->data_buf)
						free(current->data_buf);

					current->data_buf = (unsigned char *)malloc(current->data_size + 1);
					if (!current->data_buf)
					{
						DEBUG("current->data_buf malloc error : %s ", strerror(errno));
						break;
					}
					memset(current->data_buf, 0, current->data_size + 1);
				}
			}
			if (current->has_read_head == 1)
			{
				if (current->pos < current->data_size)
				{
					if ((ret = recv(current->fd, current->data_buf + current->pos, current->data_size - current->pos, 0)) <= 0)
					{
						if (ret < 0)
						{
							if (errno == EINTR || errno == EAGAIN)
							{
								//pthread_mutex_unlock(&client_mutex);
								continue;
							}
						}
						DEBUG("close fd %d", sockfd);
						break;
					}
					current->pos += ret;
				}

				if (current->pos == current->data_size)
				{
					if (process_msg(current))
					{
						DEBUG("client process_msg error programe run_out");
						if (current->head_buf)
							free(current->head_buf);
						if (current->data_buf)
							free(current->data_buf);
						current->head_buf = NULL;
						current->data_buf = NULL;
						break;
					}

					memset(current->head_buf, 0, HEAD_LEN);
					current->data_size = 0;
					current->pos = 0;
					if (current->data_buf)
						free(current->data_buf);
					current->data_buf = NULL;
					current->has_read_head = 0;
				}
				if (current->pos > current->data_size)
				{
					current->pos = 0;
					current->has_read_head = 0;
					continue;
				}
			}
		}
	}
	close_fd(sockfd);
	if (current->head_buf)
		free(current->head_buf);
	if (current->packet)
		free(current->packet);

	current->head_buf = NULL;
	current->packet = NULL;

	current->login_flag = 0;
	client_disconnect();
	client_connect();
}

int init_client()
{
	int ret;
	struct server_info *server = &(conf.server);

	server_s = create_tcp();
	if (server_s == -1)
	{
		DEBUG("create client fd error");
		return ERROR;
	}

	DEBUG("server->ip %s ", server->ip);
	ret = connect_server(server_s, server->ip, 50007, 10);
	if (0 != ret)
	{
		DEBUG("connect server ip: %s port: %d timeout 10s", server->ip, server->port);
		return ERROR;
	}
	memset(&m_client, 0, sizeof(struct client));
	m_client.fd = server_s;
	m_client.head_buf = malloc(HEAD_LEN + 1);
	m_client.packet = malloc(PACKET_LEN + 1);
	memset(m_client.packet, 0, PACKET_LEN);

	ret = send_login(&m_client);
	if (0 != ret)
	{
		close_fd(m_client.fd);
		if (m_client.head_buf)
			free(m_client.head_buf);
		if (m_client.packet)
			free(m_client.packet);

		m_client.head_buf = NULL;
		m_client.packet = NULL;
		DEBUG("login server ip: %s port: %d error", server->ip, server->port);
		return ERROR;
	}

	online = 2;
	//desktop_list = listCreate();
	return SUCCESS;
}

void *thread_client(void *param)
{
	int ret = SUCCESS;
	pthread_attr_t st_attr;
	struct sched_param sched;
	DEBUG("thread_client");

	ret = pthread_attr_init(&st_attr);
	if (ret)
	{
		DEBUG("ThreadUdp attr init warning ");
	}
	ret = pthread_attr_setschedpolicy(&st_attr, SCHED_FIFO);
	if (ret)
	{
		DEBUG("ThreadUdp set SCHED_FIFO warning");
	}
	sched.sched_priority = SCHED_PRIORITY_CLIENT;
	ret = pthread_attr_setschedparam(&st_attr, &sched);

	ret = init_client();
	if (ret != SUCCESS)
	{
		client_connect();
		client_disconnect();
		return (void *)ret;
	}
	client_online();
	tcp_loop(server_s);
	return (void *)ret;
}
