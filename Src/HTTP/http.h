#ifndef HTTP_H
#define HTTP_H

#include "../types.h"

void http_init(s8 *hostname, s8 *port);

void http_send_request(s8 *hostname, s8 *port, s8 *path);

bool http_recv();

void http_test(s8 *url);
/*
#define MAX_REQUEST_SIZE 2047

#include <sys/select.h>

struct client_info 
{
    socklen_t address_length;
    struct sockaddr_storage address;
    int socket;
    char request[MAX_REQUEST_SIZE + 1];
    int received;
    struct client_info *next;
};

struct client_info *get_client(int s);
void drop_client(struct client_info *client);
const char *get_client_address(struct client_info *ci);
fd_set wait_on_clients(int server);
void send_400(struct client_info *client);
void send_404(struct client_info *client);
void serve_resource(struct client_info *client, const char *path);
*/
#endif // HTTP_H