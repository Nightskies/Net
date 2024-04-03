#ifndef DNS_H
#define DNS_H

#include "../types.h"

void dns_init(void);

void dns_resolve(s8 *host_name, s8 *req_type);

#endif // DNS_H