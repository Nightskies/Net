#include "error.h"

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

void error(const s8* msg)
{
    fprintf(stderr, "In %s:%d line fatal error: %s\n", __FILE__, __LINE__, msg);

    exit(EXIT_FAILURE);
}

void sys_error(void)
{
    error(strerror(errno));
}

void gai_error(void)
{
    error(gai_strerror(errno));
}