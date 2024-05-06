#ifndef UDP_HDR_H
#define UDP_HDR_H

#include "../types.h"

struct udp_hdr
{
    u16 src;
    u16 dst;
    u16 len;
    u16 check;
};

void udp_hdr_fill(struct udp_hdr *udp, u16 src, u16 dst, u16 len);

void udp_hdr_dump(struct udp_hdr *udp);

#endif // UDP_HDR_H