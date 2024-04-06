#ifndef ICMP_HDR_H
#define ICMP_HDR_H

#include "../types.h"

#define ICMP_DATA_LEN 48

struct icmp_hdr
{
    u8 type;
    u8 code;
    u16 checksum;
    u16 id;
    u16	seq;
    u8 data[ICMP_DATA_LEN];
};

void icmp_hdr_fill(struct icmp_hdr *icmp, u8 type, u8 code, u16 id, u16 seq);

void icmp_hdr_dump(struct icmp_hdr *icmp);

#endif // ICMP_HDR_H