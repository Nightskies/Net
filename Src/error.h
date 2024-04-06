#ifndef ERROR_H
#define ERROR_H

#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>

#define error(msg)                                                               \
    fprintf(stderr, "In %s:%d line fatal error: %s\n", __FILE__, __LINE__, msg); \
    exit(EXIT_FAILURE);                                                          \

#define sys_error()        \
    error(strerror(errno)) \

#define gai_error()            \
    error(gai_strerror(errno)) \

#endif // ERROR_H