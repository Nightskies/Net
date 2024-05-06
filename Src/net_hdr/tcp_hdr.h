#ifndef TCP_HDR_H
#define TCP_HDR_H

#include "../types.h"

struct tcp_hdr
{
    u16 src;
    u16 dst;
    u32 seq;
    u32 ackn;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	u8 reserved:4;	/* (unused) */
	u8 data_off:4;	/* data offset */
	u16 fin:1;
	u16 syn:1;
	u16 rst:1;
	u16 psh:1;
	u16 ack:1;
	u16 urg:1;
	u16 ece:1;
    u16 cwr:1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	u8 data_off:4;	/* data offset */
	u8 reserved:4;	/* (unused) */
	u16 ece:1;
    u16 cwr:1;
	u16 urg:1;
	u16 ack:1;
	u16 psh:1;
	u16 rst:1;
	u16 syn:1;
	u16 fin:1;
#endif
    u16 win_size;
    u16 check;
    u16 urgp;
};

void tcp_hdr_fill(struct tcp_hdr *tcp, u16 src, u16 dst, u32 seq, u32 ackn, u8 doff, u16 fin, u16 syn, u16 rst,
    u16 psh, u16 ack, u16 urg, u16 ece, u16 cwr, u16 wsize, u16 urgp);

void tcp_hdr_dump(struct tcp_hdr *tcp);

#endif // TCP_HDR_H