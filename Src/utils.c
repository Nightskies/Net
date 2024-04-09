#include "utils.h"
#include "error.h"

#include <arpa/inet.h>

#define HTTP_PORT "80"

u32 aton(s8 *ip)
{
    struct in_addr addr;

    if (inet_aton(ip, &addr) < 0) {
        sys_error();
    }
    return addr.s_addr;
}

s8 *ntoa(u32 addr)
{
    struct in_addr in;
    in.s_addr = addr;

    return inet_ntoa(in);
}

u16 checksum16(u16 *data, s32 len)
{
    u16 ret = 0;
	u32 sum = 0;
	u16 odd_byte;
	
	while (len > 1) {
		sum += *data++;
		len -= 2;
	}
	
	if (len == 1) {
		*(u8*)(&odd_byte) = * (u8*)data;
		sum += odd_byte;
	}
	
	sum =  (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	ret =  ~sum;
	
	return ret; 
}

void parse_url(s8 *url, s8 **hostname, s8 **port, s8** path) 
{
    s8 *p;
    p = strstr(url, "://");

    s8 *protocol = 0;
    if (p) {
        protocol = url;
        *p = 0;
        p += 3;
    } else {
        p = url;
    }

    if (protocol) {
        if (strcmp(protocol, "http")) {
			error("Unknown protocol. Only 'http' is supported.\n");
        }
    }

    *hostname = p;
    while (*p && *p != ':' && *p != '/' && *p != '#') {
		++p;
	}

    *port = HTTP_PORT;
    if (*p == ':') {
        *p++ = 0;
        *port = p;
    }
    while (*p && *p != '/' && *p != '#') {
		++p;
	}

    *path = p;
    if (*p == '/') {
        *path = p + 1;
    }
    *p = 0;

    ++p;
    while (*p && *p != '#') { 
		++p;
	}

    if (*p == '#') {
		*p = 0;
	}
}