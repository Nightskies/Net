#include "udp_hdr.h"

#include "../utils.h"

#include <stdio.h>
#include <strings.h>

void udp_hdr_fill(struct udp_hdr *udp, u16 src, u16 dst, u16 len)
{
    bzero(udp, sizeof(struct udp_hdr));

    udp->src = src;
    udp->dst = dst;
    udp->len = len;
    udp->check = checksum16((u16*)udp, sizeof(struct udp_hdr));
}

void udp_hdr_dump(struct udp_hdr *udp)
{
    fprintf(stdout, "\nUDP HEADER\n");
    fprintf(stdout, "SRC PORT:%hu\n", htons(udp->src));
    fprintf(stdout, "DST PORT:%hu\n", htons(udp->dst));
    fprintf(stdout, "UDP LENGTH:%hu\n", htons(udp->len));
}