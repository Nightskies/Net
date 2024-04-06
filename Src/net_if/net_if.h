#ifndef NET_IF_H
#define NET_IF_H

#include "../types.h"

#include <net/if.h> 

struct net_if
{
   struct ifreq ifr;
   s32 fd;
}; 

void net_if_init(struct net_if *interface, s32 fd, s32 family);

s32 net_if_get_index(struct net_if *interface);

// get hardware address(MAC)
u8* net_if_get_ha(struct net_if *interface);

// get protocol address(IP)
u32 net_if_get_pa(struct net_if *interface);

#endif // NET_IF_H