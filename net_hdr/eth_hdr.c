#include "eth_hdr.h"

#include "../utils.h"

#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

void eth_hdr_fill(struct eth_hdr *eth, u8 *hda, u8 *hsa, u16 type)
{
    memcpy(eth->hda, hda, ETH_ALEN);
    memcpy(eth->hsa, hsa, ETH_ALEN);
    eth->type = htons(type);
}

void eth_hdr_dump(struct eth_hdr *eth)
{
    fprintf(stdout, "ETHERNET HEADER\n");
    fprintf(stdout, "Source MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", 
        eth->hsa[0], 
        eth->hsa[1], 
        eth->hsa[2],
        eth->hsa[3],
        eth->hsa[4],
        eth->hsa[5]);
    fprintf(stdout, "Destination MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", 
        eth->hda[0], 
        eth->hda[1], 
        eth->hda[2],
        eth->hda[3],
        eth->hda[4],
        eth->hda[5]);
    fprintf(stdout, "Ether type:%#06x\n", ntohs(eth->type));
}