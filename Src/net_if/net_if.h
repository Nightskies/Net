#ifndef NET_IF_H
#define NET_IF_H

#include "../types.h"
#include "socket_type.h"
#include "socket_family.h"
#include "socket_proto.h"

struct net_if;

struct net_if *net_if_create(enum sock_family family);

s32 net_if_get_index(struct net_if *nif, s32 fd);

// get hardware address(MAC)
u8* net_if_get_ha(struct net_if *nif, s32 fd);

// get protocol address(IP)
u32 net_if_get_pa(struct net_if *nif, s32 fd);

#endif // NET_IF_H