#include "base.h"
#include "arp.h"

int ip_check_arp(char *eth, char *ip, unsigned char *mac)
{
	int ret;

#if 0
 unsigned long ulIp = inet_addr("192.168.10.134");
 m_tree.SetItemData(hsubitemtemp, ulIp1);

char *p[20];

 struct   in_addr   inaddr;
 inaddr.S_un.S_addr=ulIp;
 char *p = inet_ntoa(inaddr);
#endif
	DEBUG("eth %s ip %s mac %s", eth, ip, mac);
	unsigned long m_ip = inet_addr("192.168.187.128");		
	unsigned char m_mac[6] = {0x00, 0x0C, 0x29, 0x90, 0xC1, 0x98};
	//DEBUG("");	
	//sscanf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", m_mac[0], m_mac[1], m_mac[2],
	//				m_mac[3], m_mac[4], m_mac[5]);

	//DEBUG("mac, %02x:%02x:%02x:%02x:%02x:%02x", m_mac[0], m_mac[1], m_mac[2],
     //                 m_mac[3], m_mac[4], m_mac[5]);




	pid_t pid = fork();
	if(pid < 0)
	{
		DEBUG("fork err");
		return ERROR;
	}
	else if(pid == 0)
	{
		ret = send_arp("eth0", m_ip, m_mac);
		if(SUCCESS != ret)
		{
			DEBUG("send_arp error");
			return ERROR; 
		}
	}
	else 
	{
		ret = recv_arp("eth0", m_ip);
		if(SUCCESS != ret)
		{
			DEBUG("recv_arp error");	
			return ERROR;
		}
	}


#if 0
	ret = send_arp("eth0", m_ip, m_mac);
	if(SUCCESS != ret)
	{
		DEBUG("send_arp error");
		return ERROR; 
	}

	ret = recv_arp("eth0", m_ip);
	if(SUCCESS != ret)
	{
		DEBUG("recv_arp error");	
		return ERROR;
	}
	return SUCCESS;
#endif
}


int send_arp(char *eth, unsigned long ip, unsigned char *mac)
{
	int sock = -1, ret;
	char buf[MAX_BUFLEN] = {0};
	struct sockaddr_ll send_addr;		//混杂模式
	struct ifreq ethreq;
	unsigned char mac_dst[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	unsigned char ip_src[4] = {192,168,187,132};
	unsigned char ip_dst[4] = {192,168,187,128};

	if((sock = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) == -1)
	{
		DEBUG("socket SOCK_RAW error");
		return ERROR;
	}

	strcpy(ethreq.ifr_name, eth);
	
	if(ioctl(sock, SIOCGIFINDEX, &ethreq) != 0)
	{
		DEBUG("ioctl set SIOCGIFINDEX error");
	}
	
	send_addr.sll_family = AF_PACKET;
	send_addr.sll_protocol = htons(ETH_P_ALL);
	send_addr.sll_ifindex = ethreq.ifr_ifindex;
	
	PARPPACKET arp = (PARPPACKET)buf;
		
	/* ehhdr */
	memcpy(arp->ehhdr.eh_dst, mac_dst, 6);
	memcpy(arp->ehhdr.eh_src, mac, 6);
	arp->ehhdr.eh_type = htons(ETH_P_ARP);

   
	/* arp */
	arp->arphdr.arp_hrd = htons(ARP_HARDWARE);
	arp->arphdr.arp_pro = htons(ETH_P_IP);
	arp->arphdr.arp_hln = 6;					//mac 
	arp->arphdr.arp_pln = 4;					//ipv4
	arp->arphdr.arp_op = htons(ARP_REQUEST);
	memcpy(arp->arphdr.arp_sha, mac, 6);
	arp->arphdr.arp_spa = inet_addr("192.168.187.132");
	//memcpy(arp->arphdr.arp_spa, ip_src, 4);
	memcpy(arp->arphdr.arp_tha, mac_dst, 6);
	//memcpy(arp->arphdr.arp_tpa, ip_dst, 4);
	arp->arphdr.arp_tpa = inet_addr("192.168.187.128");
	
	
	if((sendto(sock, buf, sizeof(ARPPACKET), 0, (struct sockaddr*)&send_addr, sizeof(send_addr))) == -1)
	{
		DEBUG("send arp packet error");
		return ERROR;
	}
	close_fd(sock);
	
	return SUCCESS;
}

int recv_arp(char *eth, char *m_ip)
{
	int sock = -1, ret;
	char buf[MAX_BUFLEN] = {0};
	struct sockaddr_ll recv_addr;		//混杂模式
	struct sockaddr_ll addr;		//混杂模式
	struct ifreq ethreq;
	int addr_len = sizeof(struct sockaddr_ll);
	time_t end_time = time(NULL) + 2; 
	unsigned char ip[4];	

	if((sock = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) == -1)
	{
		DEBUG("socket SOCK_RAW error");
		return ERROR;
	}

	strcpy(ethreq.ifr_name, eth);
	
	if(ioctl(sock, SIOCGIFINDEX, &ethreq) != 0)
	{
		DEBUG("ioctl set SIOCGIFINDEX error");
		return ERROR;
	}
	
	recv_addr.sll_family = AF_PACKET;
	recv_addr.sll_protocol = htons(ETH_P_ALL);
	recv_addr.sll_ifindex = ethreq.ifr_ifindex;

	ret = bind(sock, (struct sockaddr*)&recv_addr, sizeof(struct sockaddr_ll));
	if(ret != 0)
	{
		DEBUG("bind sock error");
		return ERROR;
	}
	
	if(fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
	{
		DEBUG("fcntl F_SETFL error");
	}
	
	while(time(NULL) < end_time)
	{
		ret = recvfrom(sock, buf, MAX_BUFLEN, 0, (struct sockaddr *)&addr, &addr_len);
		//DEBUG("ret %d", ret);
		if(ret == 0)
		{
			usleep(1000);
			continue;
		}
		usleep(100);
		if(*(unsigned short *)&buf[12] != EPT_ARP || buf[21] != ARP_REPLY)		//不是ARP
			continue;
		
		PARPHDR arp = (buf + 14);
		memcpy(ip, (char *)&arp->arp_spa, 4);
		//char *p = inet_ntoa(inaddr);
		DEBUG("used ip:%d.%d.%d.%d",ip[0], ip[1], ip[2], ip[3] );
	}
	close_fd(sock);
	return SUCCESS;
}

