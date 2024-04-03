#include "arp_hdr.h"

#include "../utils.h"

#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

void arp_hdr_fill(struct arp_hdr *arp, u16 htype, u16 ptype, 
    u8 hlen, u8 plen, u16 oper, u8 *sha, u32 spa, u8 *tha, u32 tpa)
{
    arp->htype = htons(htype);
    arp->ptype = htons(ptype);
    arp->hlen = hlen;
    arp->plen = plen;
    arp->oper = htons(oper);
    memcpy(arp->sha, sha, ETH_ALEN);
    arp->spa = spa;
    memcpy(arp->tha, tha, ETH_ALEN);
    arp->tpa = tpa;
}

void arp_hdr_dump(struct arp_hdr *arp)
{
    fprintf(stdout, "\nARP HEADER\n");
    fprintf(stdout, "HTYPE:%#06x\n", ntohs(arp->htype));
    fprintf(stdout, "PTYPE:%#06x\n", ntohs(arp->ptype));
    fprintf(stdout, "HLEN:%hhu\n", arp->hlen);
    fprintf(stdout, "PLEN:%hhu\n", arp->plen);
    fprintf(stdout, "OPER:%#06x\n", ntohs(arp->oper));
    fprintf(stdout, "Source MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", 
        arp->sha[0], 
        arp->sha[1], 
        arp->sha[2],
        arp->sha[3],
        arp->sha[4],
        arp->sha[5]);
    fprintf(stdout, "Source IP:%s\n", ntoa(arp->spa));
    fprintf(stdout, "Destination MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", 
        arp->tha[0], 
        arp->tha[1], 
        arp->tha[2],
        arp->tha[3],
        arp->tha[4],
        arp->tha[5]);
    fprintf(stdout, "Destination IP:%s\n", ntoa(arp->tpa));
}