#ifndef DNS_HDR_H
#define DNS_HDR_H

#include "../types.h"

struct dns_hdr
{
    u16 id;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    u32 rd:1;
	u32 tc:1;
	u32 aa:1;
	u32 opcode:4;
	u32 qr:1;

	u32 rcode:4;
	u32 z:3;
	u32 ra:1;
#else
    u32 qr:1;
	u32 opcode:4;
	u32 aa:1;
	u32 tc:1;
	u32 rd:1;
	u32 ra:1;
	u32 z:3;
	u32 rcode:4;
#endif
    u16 qdcount;
    u16 ancount;
    u16 nscount;
    u16 arcount;
};

struct dns_qst_flags
{
    u16 qtype;
    u16 qclass;
};

struct __attribute__ ((__packed__)) dns_ans_flags
{
    u16 type;
    u16 class;
    u32 ttl;
    u16 rdlength;
};


void dns_hdr_fill(u8 *packet, u16 id, u8 qr, u8 opcode, u8 aa, u8 tc, u8 rd, u8 ra, u8 z,
    u8 rcode, u16 qdcount, u16 ancount, u16 nscount, u16 arcount);

void dns_qst_fill(u8 *packet, u8 *name, u16 qtype, u16 qclass);

void dns_dump(u8 *packet);

#endif // DNS_HDR_H