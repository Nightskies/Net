#ifndef ADDRESS_IN_H
#define ADDRESS_IN_H

#include "socket_type.h"
#include "socket_family.h"

#include "address.h"

void address_in_init(struct address *addr, enum sock_family family, enum sock_type type, s8 *host, s8 *service);

#endif // ADDRESS_IN_H