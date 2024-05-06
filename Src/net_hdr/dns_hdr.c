#include "dns_hdr.h"

#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>

static void dns_name_to_dns_format(s8 *src, u8 *dst) 
{
	strcat(src, ".");
    while (*src) {
        s8 *p = strpbrk(src, ".");
        *dst++ = p - src;
        while(src != p) { 
            *dst++ = *src++;
        }
        ++src;
	}
	*dst = '\0';
}

static const u8* dns_print_name(const u8 *msg, const u8 *p)
{
    if ((*p & 0xC0) == 0xC0) {
        const s32 k = ((*p & 0x3F) << 8) + p[1];
        p += 2;
        printf(" (pointer %d) ", k);
        dns_print_name(msg, msg+k);
        return p;

    } else {
        const s32 len = *p++;

        printf("%.*s", len, p);
        p += len;
        if (*p) {
            printf(".");
            return dns_print_name(msg, p);
        } else {
            return p + 1;
        }
    }
}

void dns_hdr_fill(u8 *packet, u16 id, u8 qr, u8 opcode, u8 aa, u8 tc, u8 rd, u8 ra, u8 z,
    u8 rcode, u16 qdcount, u16 ancount, u16 nscount, u16 arcount)
{
    struct dns_hdr *hdr = (struct dns_hdr *)packet;

    bzero(hdr, sizeof(struct dns_hdr));

    hdr->id = htons(id);
    hdr->qr = qr;
    hdr->opcode = opcode;
    hdr->aa = aa;
    hdr->tc = tc;
    hdr->rd = rd;
    hdr->ra = ra;
    hdr->z = z;
    hdr->rcode = rcode;
    hdr->qdcount = htons(qdcount);
    hdr->ancount = htons(ancount);
    hdr->nscount = htons(nscount);
    hdr->arcount = htons(arcount);
}

void dns_qst_fill(u8 *packet, u8 *name, u16 qtype, u16 qclass)
{
    dns_name_to_dns_format(name, packet);
    struct dns_qst_flags *qst = (struct dns_qst_flags *)(packet + strlen(packet) + 1);
    bzero(qst, sizeof(struct dns_qst_flags));
    
    qst->qtype = htons(qtype);
    qst->qclass = htons(qclass);
}

void dns_dump(u8 *packet) 
{
    size_t packet_offset = 0;

    struct dns_hdr *hdr = (struct dns_hdr *)packet;
    printf("ID = %hu\n", ntohs(hdr->id));
    packet_offset += sizeof(struct dns_hdr);

    u32 qr = hdr->qr;
    printf("QR = %u %s\n", hdr->qr, hdr->qr ? "response" : "query");

    u32 opcode = hdr->opcode;
    printf("OPCODE = %u ", opcode);

    switch (opcode) {
        case 0: 
            puts("standard"); 
            break;
        case 1: 
            puts("reverse"); 
            break;
        case 2: 
            puts("status"); 
            break;
        default: 
            puts("?"); 
            break;
    }

    u32 aa = hdr->aa;
    u32 tc = hdr->tc;
    u32 rd = hdr->rd;
    printf("AA = %u %s\n", aa, aa ? "authoritative" : "");
    printf("TC = %u %s\n", tc, tc ? "message truncated" : "");
    printf("RD = %u %s\n", rd, rd ? "recursion desired" : "");

    if (qr) {
        u32 ra = hdr->ra;
        printf("RA = %u %s\n", ra, ra ? "recursion available" : "");

        u32 rcode = hdr->rcode;
        printf("RCODE = %u\n", rcode);

        if (rcode != 0) {
            switch (rcode) {
                case 0: 
                    puts("success"); 
                    break;
                case 1: 
                    puts("format error"); 
                    break;
                case 2: 
                    puts("server failure"); 
                    break;
                case 3: 
                    puts("name error"); 
                    break;
                case 4:
                    puts("not implemented"); 
                    break;
                case 5: 
                    puts("refused"); 
                    break;
                default: 
                    puts("?"); 
                    break;
            }
        }
    }

    const u16 qdcount = ntohs(hdr->qdcount);
    const u16 ancount = ntohs(hdr->ancount);
    const u16 nscount = ntohs(hdr->nscount);
    const u16 arcount = ntohs(hdr->arcount);

    printf("QDCOUNT = %hu\n", qdcount);
    printf("ANCOUNT = %hu\n", ancount);
    printf("NSCOUNT = %hu\n", nscount);
    printf("ARCOUNT = %hu\n", arcount);
    
    struct dns_qst_flags *qst = NULL;

    if (qdcount) {
        for (s32 i = 0; i < qdcount; ++i) {
            printf("Query %2d\n", i + 1);
            printf("  name: ");

            const u8 *name = packet + sizeof(struct dns_hdr);
            const u8 *qflags = dns_print_name(packet, name);
            qst = (struct dns_qst_flags *)qflags;
            packet_offset = (qflags - packet) + sizeof(struct dns_qst_flags);
            putc('\n', stdout);
            printf("  type: %d\n", ntohs(qst->qtype));
            printf(" class: %d\n", ntohs(qst->qclass));
        }
    }

    struct dns_ans_flags *ans = NULL;
    if (ancount || nscount || arcount) {
        for (s32 i = 0; i < ancount + nscount + arcount; ++i) {
            printf("Answer %2d\n", i + 1);
            printf("  name: ");

            u8 *name = packet + packet_offset;
            const u8 *aflags = dns_print_name(packet, name);
            printf("\n");

            ans = (struct dns_ans_flags *)aflags;
            packet_offset = (aflags - packet) + sizeof(struct dns_ans_flags);

            u16 type = ntohs(ans->type);
            printf("  type: %hu\n", type);
            printf(" class: %hu\n", ntohs(ans->class));
            printf("   ttl: %hu\n", ntohl(ans->ttl));
            u16 rdlen = ntohs(ans->rdlength);
            printf(" rdlen: %hu\n", rdlen);
            packet_offset += rdlen;

            u8 *data = (u8 *)ans + sizeof(struct dns_ans_flags);
            if (rdlen == 4 && type == 1) {
                printf("Address ");
                printf("%u.%u.%u.%u\n", data[0], data[1], data[2], data[3]);

            } else if (rdlen == 16 && type == 28) {
                printf("Address ");
                for (s32 j = 0; j < rdlen; j += 2) {
                    printf("%02x%02x", data[j], data[j + 1]);
                    if (j + 2 < rdlen) { 
                        putc(':', stdout);
                    }
                }
                printf("\n");   
            } else if (type == 15 && rdlen > 3) {
                const s32 preference = (data[0] << 8) + data[1];
                printf("  pref: %d\n", preference);
                printf("MX: ");
                dns_print_name(packet, data + 2); 
                printf("\n");
            } else if (type == 16) {
                printf("TXT: '%.*s'\n", rdlen - 1, data);
            } else if (type == 5) {
                printf("CNAME: ");
                dns_print_name(packet, data); 
                printf("\n");
            }
        }
    }
    printf("\n");
}