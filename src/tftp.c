#include "base.h"
#include "tftp.h"

int tftp_get(char *server_ip, char *remote_file, char *local_file)
{
	struct tftp_packet send_packet, recv_packet;
	const int blocksize = 512;
    int ret = 0;
    int time_wait_data;
    unsigned short block = 1;
	
	struct sock_udp udp = create_udp(server_ip, 69, 0);
	socklen_t addr_len = sizeof(struct sockaddr_in);
    
    /* send request. */
    send_packet.cmd = htons(CMD_RRQ);
    sprintf(send_packet.file_name, "%s%c%s%c%d%c", remote_file, 0, "octet", 0, blocksize, 0); 
    sendto(udp.fd, &send_packet, sizeof(struct tftp_packet), 0, (struct sockaddr*)&udp.send_addr, sizeof(struct sockaddr_in));
    
    FILE *fp = fopen(local_file, "w");
    if(fp == NULL){
        printf("Create file \"%s\" error.\n", local_file);
        return;
    }   
    /* receive data. */
    send_packet.cmd = htons(CMD_ACK);
    do{ 
        for(time_wait_data = 0; time_wait_data < PKT_RECV_TIMEOUT * PKT_MAX_RXMT; time_wait_data += 10000){
            ret = recvfrom(udp.fd, &recv_packet, sizeof(struct tftp_packet), MSG_DONTWAIT,
                    (struct sockaddr *)&udp.recv_addr, &addr_len);

            if(ret > 0 && ret < 4){ 
                printf("Bad packet: ret=%d\n", ret);
            }   
            if(ret >= 4 && recv_packet.cmd == htons(CMD_DATA) && recv_packet.block == htons(block)){
                printf("DATA: block=%d, data_size=%d\n", ntohs(recv_packet.block), ret - 4); 
                // Send ACK.
                send_packet.block = recv_packet.block;
                sendto(udp.fd, &send_packet, sizeof(struct tftp_packet), 0, (struct sockaddr*)&udp.recv_addr, addr_len);
                fwrite(recv_packet.data, 1, ret - 4, fp);
                break;
            }   
            usleep(5);
        }   
        if(time_wait_data >= PKT_RECV_TIMEOUT * PKT_MAX_RXMT){
            printf("Wait for DATA #%d timeout.\n", block);
			//close_fd(udp.fd);	
			//fclose(fp);
			//return ERROR;
        }   
        block ++; 
    }while(ret == blocksize + 4); 
       
	close_fd(udp.fd);	
    fclose(fp);
	return SUCCESS;
}

int tftp_put()
{


}
