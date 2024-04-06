#ifndef IP_HDR_H
#define IP_HDR_H

#include "../types.h"

struct ip_hdr
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    u32 ihl:4;
    u32 version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    u32 version:4;
    u32 ihl:4;
#endif
    u8 tos;
    u16 tot_len;
    u16 id;
    u16 frag_off;
    u8 ttl;
    u8 proto;
    u16 check;
    u32 saddr;
    u32 daddr;
};

void ip_hdr_fill(struct ip_hdr *ip, u8 ihl, u8 vers, u8 tos, u16 tot_len, u16 id,
    u16 frag_off, u8 ttl, u8 proto, u32 saddr, u32 daddr);

void ip_hdr_dump(struct ip_hdr *ip);

#endif // IP_HDR_H