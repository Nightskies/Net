#include "ARP/arp.h"
#include "Ping/ping.h"

#include "error.h"

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

static s8 *prog_name;

const s8* short_opts = "h";

const struct option long_opts[] = {
    {"help", no_argument, 0, 'h'},
    {"arp", required_argument, 0, 1},
    {"ping", required_argument, 0, 2},
};

void help_menu(void)
{
    printf("Usage: %s [type] [args]\n\n", prog_name);
  
    puts("program arguments:");
    puts("  -h, -help             Show this help message and exit.");

    puts("program types:");
    puts("  -arp <dst ip>           Send simple arp");
    puts("  -ping <remote host>     Send simple ping");
}

bool check_root(void)
{
    if (geteuid() == 0) {
        return true;
    } else {
        return false;
    }
}

s32 main(s32 argc, s8 *argv[])
{
	prog_name = argv[0];

	s32 rez, option_index = 0;
	while ((rez = getopt_long_only(argc, argv, short_opts, long_opts, &option_index)) != -1) {
		switch (rez) {
			case 'h':
				help_menu();
			 	return 0;
		  	case 1:
				if (!check_root()){
	   				error("ERROR: Only (sudo) run");
    			}
			 	arp_init();
				arp_request(optarg);
				arp_reply();
				break;
		  	case 2:
				if (!check_root()){
	   				error("ERROR: Only (sudo) run");
    			}
				ping_init();
				ping_test(optarg);
		  	case '?':
		  	default:
				help_menu();
			 	return 0;
	   	}
    }

	return 0;
}