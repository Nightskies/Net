#include "../net_hdr/eth_hdr.h"
#include "../net_hdr/arp_hdr.h"
#include "../net_if/net_if.h"
#include "../utils.h"
#include "../error.h"

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <string.h> //
#include <strings.h>
#include <stdlib.h>

#define ETHERNET  1
#define ETH_P_ARP 0x0806
#define IPv4      0x0800
#define IPv4_ALEN 4

#define MAX_ETH_FRAME_LEN sizeof(struct eth_hdr) + 1500

// arp types
#define REQUEST 1
#define REPLY   2

static struct arp // maybe call arp manager
{
    struct net_if nif;
    struct sockaddr_ll sll;
} arp;

void arp_init(void)
{
    bzero(&arp, sizeof(struct arp));

    s32 sfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sfd < 0) {
        sys_error();
    }

    net_if_init(&arp.nif, sfd, AF_PACKET);

    arp.sll.sll_family = AF_PACKET;
	arp.sll.sll_ifindex = net_if_get_index(&arp.nif);
	arp.sll.sll_halen = ETH_ALEN;
}

void arp_request(s8 *daddr)
{
    u8 *packet = (u8 *)malloc(MAX_ETH_FRAME_LEN);
    if (packet == NULL) {
        sys_error();
    }

    struct eth_hdr *eth_hdr = (struct eth_hdr *)packet;
    u8 src_mac[ETH_ALEN] = {0};

#define BROADCAST_ADDR "\xff\xff\xff\xff\xff\xff"
    u8 *hda = BROADCAST_ADDR;
    memcpy(src_mac, net_if_get_ha(&arp.nif), ETH_ALEN);
    u8 *hsa = src_mac;
    eth_hdr_fill(eth_hdr, hda, hsa, ETH_P_ARP);

    struct arp_hdr *arp_hdr = (struct arp_hdr *)(packet + sizeof(struct eth_hdr));

#define UNKNOWN_ADDR "\x00\x00\x00\x00\x00\x00"
    u8 *tha = UNKNOWN_ADDR;
    u8 *sha = src_mac;
    u32 spa = net_if_get_pa(&arp.nif);
    u32 tpa = aton(daddr);
    arp_hdr_fill(arp_hdr, ETHERNET, IPv4, ETH_ALEN, IPv4_ALEN, REQUEST, sha, spa, tha, tpa);

    u64 packet_len = sizeof(struct eth_hdr) + sizeof(struct arp_hdr);

    ssize_t sbytes = sendto(arp.nif.fd, &packet, packet_len, 0, (struct sockaddr *)&(arp.sll), sizeof(struct sockaddr_ll));
	if (sbytes < 0) {
		sys_error();
	}
}

void arp_reply(void)
{
    u8 *packet = (u8 *)malloc(MAX_ETH_FRAME_LEN);
    if (packet == NULL) {
        sys_error();
    }

    ssize_t rbytes = recvfrom(arp.nif.fd, &packet, MAX_ETH_FRAME_LEN, 0, NULL, 0);
    if (rbytes < 0) {
        sys_error();
    }

    struct eth_hdr *eth_hdr = (struct eth_hdr *)packet;
    struct arp_hdr *arp_hdr = (struct arp_hdr *)(packet + sizeof(struct eth_hdr));

    eth_hdr_dump(eth_hdr);
    arp_hdr_dump(arp_hdr);
}