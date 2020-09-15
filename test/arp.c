3.2.1.1 ARP头部数据链路层首部     

 ARP头部固定部分为28个字节，数据结构定义如下：

 typedef struct ARP

   {

unsigned short arp_hardware_type;  

unsigned short arp_p;              

unsigned char arp_hardware_len;    

unsigned char arp_ip_len;          

unsigned short arp_option;         

unsigned char arp_src_hardware[6];  

unsigned char arp_src_ip[4];       

unsigned char arp_dst_hardware[6]; 

unsigned char arp_dst_ip[4];       

  }ArpHead;

各部分的含义：

arp_hardware_type   ：  硬件类型 ，值为0x1表示以太网。

arp_p               ： 上层协议类型 ，IP值为0X0800。

arp_hardware_len    ： 为硬件地址占的字节长度 ，IPv4值为6。

arp_ip_len          ： IP地址占的字节长度，值一般为4。

arp_option          ： ARP操作，当操作 0X1 为请求 ，0x2为应答。

arp_src_hardware[6] ： 源MAC地址。

arp_src_ip[4]       ： 源IP地址。

arp_dst_hardware[6] ： 目的MAC地址。

arp_dst_ip[4]       ： 目的IP地址。

数据链路层头部占14字节，数据结构定义如下：

typedef struct machead

  {

unsigned char mac_dst[6];  

unsigned char mac_src[6];  

unsigned short mac_p;      

  }MacHead;

各部分的含义：

mac_dst[6]   ：目标MAC。

mac_src[6]   ：源MAC。

short mac_p  ：上层协议类型当值为0x0806时为ARP，当值为0X0800时为IP。

 

 

 

3.2.1.3 发送ARP




 

发送ARP主要流程：a.创建原始套接字；b.选择使用哪张网卡；c.获取网卡索引节点；d.封装数据链路层包头；e.封装ARP请求报文部分包头；f.循环封装ARP目标IP和循环发送数据。



 



函数实现如下：


/*********************************/
/* function send arp*/
/*******************************/



#include "include.h"


int sendto_arp(char *eth, unsigned char *ip, unsigned char *maxip, unsigned char *netnumber,unsigned char *mac)
{

 unsigned char mac_dst[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
 unsigned char my_mac[6] = {0x0, 0x30, 0x18, 0xAF, 0x5F, 0x51};
 unsigned char my_ip[4] = {210, 41, 239, 10};
 //unsigned char broad_ip[4] = {210, 41, 239, 2};
 int z;

 struct sockaddr_ll socket_address;
 struct   ifreq ifstruct; 
  int sock;
   //set pointer of eth.hdr
 if ((sock = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) == -1)
  {
  perror("error raw socket");
  return 2;
  }
  strcpy(ifstruct.ifr_name, eth);   //interface eth[0-9]
 
    if ( ioctl(sock,   SIOCGIFINDEX,   &ifstruct) != 0)
 {
  perror("error send _arp ioctl");
  return 1;
 } 

 socket_address.sll_family   = AF_PACKET;
 socket_address.sll_protocol = htons(ETH_P_ALL);
 socket_address.sll_ifindex  = ifstruct.ifr_ifindex;

 

 int i=0, n;
 MacHead *SendMh;
 unsigned char buf[MAXDATASIZE];   //500
 bzero(buf, MAXDATASIZE);    //zero 500

 
 
 //MAC
 SendMh = (MacHead *)buf;
 memcpy(SendMh->mac_dst, mac_dst, 6);
 memcpy(SendMh->mac_src, mac, 6);
 //memcpy(SendMh->mac_src, my_mac, 6);
 SendMh->mac_p = htons(ETH_P_ARP);

 
 //ARP
 ArpHead *ah;
 ah = (ArpHead *)(buf + sizeof(MacHead));
 ah->arp_hardware_type = htons(1);
 ah->arp_p = htons(ETH_P_IP);
 ah->arp_hardware_len = 6;
 ah->arp_ip_len = 4;
 ah->arp_option = htons(1);
 memcpy(ah->arp_src_hardware, mac, 6);
 //memcpy(ah->arp_src_hardware, my_mac, 6);
 //memcpy(ah->arp_dst_hardware, mac_dst, 6);
 //memcpy(ah->arp_src_ip, ip, 4);
 
 memcpy(ah->arp_dst_ip, ip, 4);

  for (i = netnumber[3]; i<maxip[3]; i++)
  {
   ah->arp_dst_ip[3] = i + 1;
    //printf("dst ip address is:");

    //for(z = 0; z<3; z++)
    // printf("%d.", ip[z]);
    // printf("%d", i);
    //printf("/n");
   if((sendto(sock, buf, MAXDATASIZE, 0, (struct sockaddr*)&socket_address, sizeof(socket_address) ))==-1)
    {
     printf("error send i=%d /n",i);
    }
   usleep(1000);

  }
  //perror(" send ok !");

 printf("send arp over/n");
 return 0;
 

}//end sendto

 



 

3.2.1.4 接收ARP应答数据包



 

接收ARP应答数据包主要流程：a.创建原始套接字；b.选择使用网卡；c.获取所使用网卡的索引节点；d.绑定所使用网卡；e.设置为非阻塞套接字；f.循环接收数据；g.存储ARP应答中的源IP。

 

代码：

/*****************************************************************/
/* function receive data and save    */
/****************************************************************/

#include "include.h"

int recv_data(char *eth,  unsigned char *maxip, unsigned char *netnumber) 
{
 
        unsigned char ip[4];
        ArpHead *ah;   //arp head_list
 FILE *fp;
 unsigned char buf[MAXDATASIZE];
  struct sockaddr_ll  adrfrom;
 struct   ifreq ifstruct; 
  struct sockaddr_ll socket_address;
 int sock, i;
 unsigned char used[256];
 int len = sizeof(struct sockaddr_ll);
 time_t end_time = time(NULL) + 2; //2 second

   //set pointer of eth.hdr
 if ((sock=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) == -1)
  {
  perror("error raw socket");
  return 2;
  }

  strcpy(ifstruct.ifr_name, eth);    //eth[0-9]

   if ( ioctl(sock, SIOCGIFINDEX, &ifstruct) != 0)
 {
  perror("error ioctl ");
  return 1;
 }

 socket_address.sll_family   = AF_PACKET;
 socket_address.sll_protocol = htons(ETH_P_ALL);
 socket_address.sll_ifindex  = ifstruct.ifr_ifindex;

       i = bind(sock, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
 if(i != 0)
 {
   perror("Binding False: ");
  return 3;
 }

 if(fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
 {
  perror("fcntl");
 }
 
 bzero(used, sizeof(used));
/*
 if ((fp = fopen("used_ip.txt", "w")) == NULL)
 {
  perror("error open used_ip.txt");
 }
*/
 while(time(NULL) < end_time)
 {
 //printf("receive data /n");
 
   i = recvfrom(sock, buf, MAXDATASIZE, 0,   (struct sockaddr*)&adrfrom, &len);
 if (i == 0)
 {
  usleep(1000);
  continue;
 }
 usleep(100);
   if (buf[12] != 0x08 || buf[13] != 0x06 || buf[21] != 2)
  continue; //不是arp帧，不再往下执行，跳回接收
 //printf("**************************receive answer arp /n");
// answer_arp(buf);//tink arp
        ArpHead *ah;   //arp head_list^M
        ah=(ArpHead *)(buf+14);
        memcpy(ip, ah->arp_src_ip, 4);
 i = ip[3];
 if (used[i] == i)
  continue;
 used[i]  = i;
// fprintf(fp, "%d.%d.%d.%d/n",ip[0], ip[1], ip[2], ip[3] );   
  printf("used ip:%d.%d.%d.%d/n",ip[0], ip[1], ip[2], ip[3] );   

  } //end while
// fclose(fp);

        if ((fp = fopen("/etc/rc.d/find_ip/empty_ip.txt", "w")) == NULL)
        {
                perror("error open empty_ip.txt");
        }
 
 for (i = maxip[3] - 1; i > netnumber[3] + 1; i--)
 {
  if (used [i] == i)
   continue;
  fprintf(fp, "%d.%d.%d.%d/n", maxip[0], maxip[1], maxip[2], i);
//  printf("%d.%d.%d.%d/n", maxip[0], maxip[1], maxip[2], i);
 
 }
 
 fclose(fp);
 
 return 0;

}






main 函数






#include "include.h"


int main(int argc, char *argv[])
{ 
 int n = 0;
 int i;
 pid_t pid;
 FILE *fp;

    unsigned char ip[4] ;
  unsigned char netmask[4];
 unsigned char maxip[4];
  unsigned char netnumber[4];
 unsigned char mac[6];
 char eth[6] = "eth0" ;

 if ( (get_ip_mask_file(ip, netmask, eth)) != 0)
  return 1;

 if((strncmp(eth, "eth", 3)) != 0)
  strcpy(eth, "eth0");

 puts("################################");
 printf("you use netcard is : %s/n", eth);
 puts("################################");

 if((get_mac(mac, eth)) != 0)         //get my mac address
        return 1;
 if((ip_and_mask(ip, netmask, maxip, netnumber)) != 0)   //get maxip and net number
  return 1;
 
 activat_net(ip, netmask, eth);   //activat eth

 pid = fork();
 if(pid < 0)
 {
  perror("fork error");
  return 1;
 }
 else if(pid == 0)   /*child send arp*/
 { 
  usleep(3);
  
  i = sendto_arp(eth, ip, maxip, netnumber, mac);
  if (i != 0)
  {
   perror("error send arp /n");
   return 1;
  }

  printf("send arp reall over /n");
 }
 else
  {
 
  i = recv_data(eth, maxip, netnumber);        /*receive data and save*/
  if (i != 0)
  {
   perror("error recv data /n");
   return 1;
  }

  printf("recv data really over/n");
  printf("now set empty ip /n");

  set_ip();   /*set empty ip*/


  system("rm -rf /etc/rc.d/find_ip/empty_ip.txt");
  return 0;
 }


}



//find my maxip and net netnumber
int ip_and_mask(unsigned char *ip, unsigned char *netmask, unsigned char *maxip, unsigned char *netnumber)
 {
 int i;
 printf("my netnumber is:");
 for(i = 0; i<4; i++)
  { 
  netnumber[i] = (ip[i] & netmask[i]);
  printf("%d.",netnumber[i]);
  maxip[i] = (netnumber[i] + (~netmask[i]));
 }
 printf("/n");
 
 return 0;
}//end ip_and_mask



int get_ip_mask_file(unsigned char ip[4], unsigned char netmask[4], char *eth)
{
 FILE *fp1;
 int i;
 char char_ip[20] ;
 char char_netmask[20] ;
 char dns[20];  //set dns
 char buf[50];
 in_addr_t addr_ip, addr_netmask;
 //char path[100] = "ip_mask.txt";
 //getcwd(path, 100);
 //strcat(path, "/ip_mask.txt");
 //printf("path = %s /n", path);
 if((fp1 = fopen("/etc/rc.d/find_ip/ip_mask.txt", "r")) == NULL)
 { 
  perror("error open ip_mask.txt");
  return 1;
 }
 fscanf(fp1, "%s/n%s/n%s/n%s", char_ip, char_netmask, eth, dns);
 fclose(fp1);

 //dns
 sprintf(buf, "echo 'nameserver %s'>/etc/resolv.conf" , dns);
 system(buf);

 addr_ip = inet_addr(char_ip); 
 memcpy(ip, &addr_ip, 4);
 addr_netmask = inet_addr(char_netmask);
 memcpy(netmask, &addr_netmask, 4);
 printf("my ip address is:");
                                                                                                                                                                                                                                                                                                                                                                                                 
  for(i = 0; i<4; i++)
  printf("%d.", ip[i]);
 printf("/n");
 printf("my netmask is:");
 for(i = 0; i<4; i++)
  printf("%d.", netmask[i]);
 printf("/n");
 return 0;
  
}

/*  activat eth[0-9]  */
int  activat_net(unsigned char *ip, unsigned char *mask, char *eth)
{
 char buf[100];
/* sprintf(buf, "ifconfig %s up %d.%d.%d.%d netmask %d.%d.%d.%d"
 , eth, ip[0], ip[1], ip[2], ip[3], mask[0], mask[1], mask[2], mask[3]);
*/
 sprintf(buf, "ifconfig %s up 172.32.0.0 netmask %d.%d.%d.%d"
 , eth, mask[0], mask[1], mask[2], mask[3]);

 printf("cmd = %s/n", buf);
 system(buf);
 return 0;
}


/*set not used ip address*/
int set_ip()
{
 FILE *fp;
 char ip[20];
 char netmask[20];
 char eth[6] = "eth0"; 
 char cmd[100];
 char gw[20];
 
 
 
 if((fp = fopen("/etc/rc.d/find_ip/ip_mask.txt", "r")) == NULL)
 { 
  perror("error open ip_mask.txt");
  return 1;
 }
 fscanf(fp, "%s/n%s/n%s", gw, netmask, eth);
 fclose(fp);
 
 
 if ((fp = fopen("/etc/rc.d/find_ip/empty_ip.txt", "r")) == NULL)
        {
                perror("error set_ip open empty_ip.txt");
                return 1;
        }

        while(!feof(fp))
        {
                fscanf(fp, "%s/n", ip);
                if(strcmp(ip, gw) != 0)
                {
                        printf("ip = %s/n", ip);
                        break;
                }
        }
        fclose(fp);


 
 if((fp = fopen("/etc/rc.d/find_ip/set_ip.txt", "w")) == NULL)
 { 
  perror("error open ip_mask.txt");
  return 1;
 }

 sprintf(cmd, "ifconfig  %s  up %s  netmask  %s", eth, ip, netmask);
 printf("cmd = %s/n", cmd);
 fprintf(fp, "set ip is %s for %s", ip, eth);
 fclose(fp); 
 
 system(cmd);

 sprintf(cmd, "route add default gw %s", gw);
 system(cmd);
}



/*  activat eth[0-9]  */
int  defult_net(char *eth)
{
 char buf[100];
 sprintf(buf, "ifdown  %s ", eth);
 system(buf);
 sprintf(buf, "ifup %s ", eth);
 system(buf);
 return 0;
}



 


 /*setip*/

 


#include "include.h"

int  activat_net(unsigned char *ip, unsigned char *mask, char *eth)
{
 char buf[100];
 sprintf(buf, "ifconfig %s up %d.%d.%d.%d netmask %d.%d.%d.%d"
 , eth, ip[0], ip[1], ip[2], ip[3], mask[0], mask[1], mask[2], mask[3]);
 printf("cmd = %s/n", buf);
}

/*
int main()
{
 unsigned char ip[4] = {192, 168, 1, 2};
 unsigned char mask[4] = {255, 255, 255, 128};
 char eth[6] = "eth0";
 set_ip(ip, mask, eth);
}



