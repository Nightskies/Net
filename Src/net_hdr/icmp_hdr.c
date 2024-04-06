#include "icmp_hdr.h"

#include "../utils.h"

#include <string.h>
#include <stdio.h>

static void icmp_fill_data(struct icmp_hdr *icmp)
{
    memset(icmp->data, rand() % 255, ICMP_DATA_LEN);
}

void icmp_hdr_fill(struct icmp_hdr *icmp, u8 type, u8 code, u16 id, u16 seq)
{
    icmp->type = type;
    icmp->code = code;
    icmp->seq = htons(seq);
    icmp->id = htons(id);
    icmp_fill_data(icmp);
    icmp->checksum = checksum16((u16*)icmp, sizeof(struct icmp_hdr));
}

void icmp_hdr_dump(struct icmp_hdr *icmp)
{
    fprintf(stdout, "\nICMP HEADER\n");
    fprintf(stdout, "Type:%hhu\n", icmp->type);
    fprintf(stdout, "Code:%hhu\n", icmp->code);
    fprintf(stdout, "ID:%hu\n", ntohs(icmp->id));
    fprintf(stdout, "Sequence:%hu\n", ntohs(icmp->seq));
    fprintf(stdout, "Data:%s\n", icmp->data);
}
