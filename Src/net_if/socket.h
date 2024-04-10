#ifndef SOCKET_H
#define SOCKET_H

#include "../types.h"
#include "socket_type.h"
#include "socket_family.h"
#include "socket_proto.h"
#include "socket_opt.h"

struct sock
{
    s32 family;
    s32 fd;
};

void socket_init(struct sock *sock, enum sock_family family, enum sock_type type, enum sock_proto proto);

s32 socket_family_map(enum sock_family family);

s32 socket_type_map(enum sock_type type);

s32 socket_proto_map(enum sock_proto proto);

void socket_opt(struct sock *sock, enum sock_opt opt);

void socket_deinit(struct sock *sock);

#endif // SOCKET_H