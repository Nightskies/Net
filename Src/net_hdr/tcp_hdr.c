#include "tcp_hdr.h"

#include "../utils.h"

#include <stdio.h>
#include <strings.h>

void tcp_hdr_fill(struct tcp_hdr *tcp, u16 src, u16 dst, u32 seq, u32 ackn, u8 doff, u16 fin, u16 syn, u16 rst,
    u16 psh, u16 ack, u16 urg, u16 ece, u16 cwr, u16 wsize, u16 urgp)
{
    bzero(tcp, sizeof(struct tcp_hdr));

    tcp->src = src;
    tcp->dst = dst;
    tcp->seq = seq;
    tcp->ackn = ackn;
    tcp->data_off = doff;
    tcp->fin = fin;
    tcp->syn = syn;
    tcp->rst = rst;
    tcp->psh = psh;
    tcp->ack = ack;
    tcp->urg = urg;
    tcp->ece = ece;
    tcp->cwr = cwr;
    tcp->win_size = wsize;
    tcp->check = checksum16((u16*)tcp, sizeof(struct tcp_hdr));
    tcp->urgp = urgp;
}

void tcp_hdr_dump(struct tcp_hdr *tcp)
{
    fprintf(stdout, "\nTCP HEADER\n");
    fprintf(stdout, "SRC PORT:%hu\n", htons(tcp->src));
    fprintf(stdout, "DST PORT:%hu\n", htons(tcp->dst));
    fprintf(stdout, "ACK NUMBER:%u\n",htonl(tcp->ackn));
    fprintf(stdout, "DATA OFFSET:%u\n", tcp->data_off);
    fprintf(stdout, "CWR:%s\n", tcp->cwr ? "true" : "false");
    fprintf(stdout, "ECE:%s\n", tcp->ece ? "true" : "false");
    fprintf(stdout, "URG:%s\n", tcp->urg ? "true" : "false");
    fprintf(stdout, "ACK:%s\n", tcp->ack ? "true" : "false");
    fprintf(stdout, "PSH:%s\n", tcp->psh ? "true" : "false");
    fprintf(stdout, "RST:%s\n", tcp->rst ? "true" : "false");
    fprintf(stdout, "SYN:%s\n", tcp->syn ? "true" : "false");
    fprintf(stdout, "FIN:%s\n", tcp->fin ? "true" : "false");
    fprintf(stdout, "WINDOW SIZE:%hu\n", htons(tcp->win_size));
    fprintf(stdout, "URGP:%hu\n", htons(tcp->urgp));
}
