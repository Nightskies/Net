#ifndef ADDRESS_H
#define ADDRESS_H

#include "../types.h"
#include <sys/socket.h>

struct address
{
    struct sockaddr saddr;
    u64 socklen;
};

#endif // ADDRESS_H