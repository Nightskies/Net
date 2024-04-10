#include "address_ll.h"

#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <strings.h>

void address_ll_init(struct address *addr, s32 ifindex)
{
    bzero(addr, sizeof(struct address));

    struct sockaddr_ll *all = (struct sockaddr_ll *)&addr->saddr;
    all->sll_family = AF_PACKET;
	all->sll_ifindex = ifindex;
	all->sll_halen = ETH_ALEN;

    addr->socklen = sizeof(struct sockaddr_ll);
}