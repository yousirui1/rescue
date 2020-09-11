#ifndef __ARP_H__
#define __ARP_H__
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <errno.h>

#define EPT_IP   0x0800    /* type: IP */
#define EPT_ARP   0x0806    /* type: ARP */
#define EPT_RARP 0x8035    /* type: RARP */
#define ARP_HARDWARE 0x0001    /* Dummy type for 802.3 frames */
#define ARP_REQUEST 0x0001    /* ARP request */
#define ARP_REPLY 0x0002    /* ARP reply */

#pragma pack(2)
typedef struct ehhdr 
{
	unsigned char eh_dst[6];   /* destination ethernet addrress */
	unsigned char eh_src[6];   /* source ethernet addresss */
	unsigned short eh_type;   /* ethernet pachet type */
}EHHDR, *PEHHDR;

typedef struct _arphdr
{
//arp首部
	unsigned short arp_hrd;    /* format of hardware address */
	unsigned short arp_pro;    /* format of protocol address */
	unsigned char arp_hln;    /* length of hardware address */
	unsigned char arp_pln;    /* length of protocol address */
	unsigned short arp_op;     /* ARP/RARP operation */

	unsigned char arp_sha[6];    /* sender hardware address */
	unsigned int arp_spa;    /* sender protocol address */
	unsigned char arp_tha[6];    /* target hardware address */
	unsigned int arp_tpa;    /* target protocol address */
}ARPHDR, *PARPHDR;

typedef struct arpPacket
{
	EHHDR ehhdr;
	ARPHDR arphdr;
} ARPPACKET, *PARPPACKET;
#pragma pack()

#endif
