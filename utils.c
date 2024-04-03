#include "utils.h"
#include "error.h"

#include <arpa/inet.h>

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