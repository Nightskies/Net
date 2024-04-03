#include "ARP/arp.h"
#include "error.h"

#include <stdio.h>

int main(s32 argc, s8 *argv[])
{
	if (argc < 2) {
		error("Usage:arp <dst_ip>");
	}

	arp_init();
	arp_request(argv[1]);
	arp_reply();


	return 0;
}