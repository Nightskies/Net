#include "ip_hdr.h"

#include <stdio.h>

void ip_hdr_fill(struct ip_hdr *ip, u8 ihl, u8 vers, u8 tos, u16 tot_len, u16 id,
    u16 frag_off, u8 ttl, u8 proto, u32 saddr, u32 daddr)
{
    ip->version = vers; 
    ip->ihl = ihl;
    ip->tos = tos;
    ip->tot_len = htons(tot_len);
    ip->id = htons(id);
    ip->ttl = ttl;
    ip->frag_off = htons(frag_off); 
    ip->proto = proto;
    ip->saddr = saddr;
    ip->daddr = daddr;
    ip->check = 0;
}

void ip_hdr_dump(struct ip_hdr *ip)
{
    fprintf(stdout, "\nIP HEADER\n");
    fprintf(stdout, "Version:%u\n", ip->version);
    fprintf(stdout, "IHL:%u\n", ip->ihl);
    fprintf(stdout, "TOS:%hhu\n", ip->tos);
    fprintf(stdout, "Total length:%hu\n", ntohs(ip->tot_len));
    fprintf(stdout, "ID:%hu\n", ntohs(ip->id));
    fprintf(stdout, "Fragmentation offset:%hu\n", ntohs(ip->frag_off));
    fprintf(stdout, "TTL:%hhu\n", ip->ttl);
    fprintf(stdout, "Protocol:%hhu\n", ip->proto);
    fprintf(stdout, "Source IP:%s\n", ntoa(ip->saddr));
    fprintf(stdout, "Destination IP:%s\n", ntoa(ip->daddr));
}