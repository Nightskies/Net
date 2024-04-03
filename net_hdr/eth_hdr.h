#ifndef ETH_HDR_H
#define ETH_HDR_H

#include "../types.h"

#define ETH_ALEN 6

struct eth_hdr
{
    u8 hda[ETH_ALEN];
	u8 hsa[ETH_ALEN];
	u16	type;
};

void eth_hdr_fill(struct eth_hdr *eth, u8 *hda, u8 *hsa, u16 type);

void eth_hdr_dump(struct eth_hdr *eth);

#endif // ETH_HDR_H