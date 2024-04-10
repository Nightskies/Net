#include "socket.h"

#include "../error.h"

#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <strings.h>

#define DEFAULT 0

static u32 socket_familys[] = {
   DEFAULT,
   AF_PACKET,
   AF_INET,
   AF_INET6
};

static u32 socket_types[] = {
   DEFAULT,
   SOCK_RAW,
   SOCK_DGRAM,
   SOCK_STREAM
};

static u32 socket_protocols[] = {
   DEFAULT,
   ETH_P_ARP,
   IPPROTO_ICMP,
   IPPROTO_UDP,
   IPPROTO_TCP,
};

s32 socket_family_map(enum sock_family family)
{
   return socket_familys[family];
}

s32 socket_type_map(enum sock_type type)
{
   return socket_types[type];
}

s32 socket_proto_map(enum sock_proto proto)
{
   s32 sproto = socket_protocols[proto];

   if (proto == SOCKET_P_ARP) {
      sproto = htons(sproto);
   }

   return sproto;
}

void socket_init(struct sock *sock, enum sock_family family, enum sock_type type, enum sock_proto proto)
{
   s32 sfamily = socket_family_map(family);
   s32 stype = socket_type_map(type);
   s32 sproto = socket_proto_map(proto);

   const s32 fd = socket(sfamily, stype, sproto);
   if (fd < 0) {
      sys_error();
   }

   bzero(sock, sizeof(struct sock));
   
   sock->family = sfamily;
   sock->fd = fd;
}

void socket_opt(struct sock *sock, enum sock_opt opt)
{
   const s32 val = 1;

   switch (opt)
   {
   case SOCKET_IPHDR:
      if (setsockopt(sock->fd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(s32)) < 0) {
         sys_error();
      }
      break;
   default:
      break;
   }
}

void socket_deinit(struct sock *sock)
{
   if (close(sock->fd) < 0) {
      sys_error();
   }
}