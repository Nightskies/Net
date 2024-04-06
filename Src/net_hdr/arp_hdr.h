#ifndef ARP_HDR_H
#define ARP_HDR_H 

#include "../types.h"

#define ETH_ALEN 6

struct arp_hdr
{	
    u16 htype;
    u16 ptype;
    u8 hlen;
    u8 plen;
    u16 oper;
    u8 sha[ETH_ALEN];
	u32 spa;
	u8 tha[ETH_ALEN];
	u32 tpa;
};

void arp_hdr_fill(struct arp_hdr *arp, u16 htype, u16 ptype, 
    u8 hlen, u8 plen, u16 oper, u8 *sha, u32 spa, u8 *tha, u32 tpa);

void arp_hdr_dump(struct arp_hdr *arp);

#endif // ARP_HDR_H