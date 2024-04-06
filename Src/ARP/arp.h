#ifndef ARP_H
#define ARP_H

#include "../types.h"

void arp_init(void);
void arp_request(s8 *daddr);
void arp_reply(void);

#endif // ARP_H