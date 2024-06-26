#include "../net_if/net_if.h"
#include "../net_if/address_in.h"
#include "../net_if/socket.h"

#include "../error.h"

#include <poll.h>
#include <time.h>

static struct http
{
    struct net_if *nif;
    struct address addr;
    struct sock sock;
} http;

void http_init(s8 *hostname, s8 *port)
{
    socket_init(&http.sock, SOCKET_AF_INET, SOCKET_STREAM, SOCKET_P_TCP);
    address_in_init(&http.addr, SOCKET_AF_INET, SOCKET_STREAM, hostname, port);

    http.nif = net_if_create(SOCKET_AF_INET);

    if (connect(http.sock.fd, &http.addr.saddr, http.addr.socklen)) {
        sys_error();
    }

    printf("Connected.\n\n");
}

void http_send_request(s8 *hostname, s8 *port, s8 *path) 
{
    s8 buffer[2048];

    sprintf(buffer, "GET /%s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    ssize_t sbytes = send(http.sock.fd, buffer, strlen(buffer), 0);
    if (sbytes < 0) {
        sys_error();
    }

    printf("Sent Headers:\n%s", buffer);
}

bool http_recv()
{
    #define RESPONSE_SIZE 32768
    static s8 response[RESPONSE_SIZE + 1];
    static s8 *end = response + RESPONSE_SIZE;
    static s8 *p = response;
    static s8 *body = NULL;
    s8 *q;

    enum {length, chunked, connection};
    u32 encoding = 0;
    u32 remaining = 0;

    ssize_t rbytes = recv(http.sock.fd, p, end - p, 0);
    if (rbytes < 1) {
        if (encoding == connection && body) {
            printf("%.*s", (int)(end - body), body);
        }

        printf("\nConnection closed by peer.\n");
        return true;
    }        

    p += rbytes;
    *p = 0;

    if (body == NULL && (body = strstr(response, "\r\n\r\n"))) 
    {
        *body = 0;
        body += 4;

        printf("Received Headers:\n%s\n", response);

        q = strstr(response, "\nContent-Length: ");
        if (q) {
            encoding = length;
            q = strchr(q, ' ');
            q += 1;
            remaining = strtol(q, NULL, 10);

        } else {
            q = strstr(response, "\nTransfer-Encoding: chunked");
            if (q) {
                encoding = chunked;
                remaining = 0;
            } else {
                encoding = connection;
            }
        }
            
        printf("\nReceived Body:\n");
    }

    if (body) {
        if (encoding == length) {
            if (p - body >= remaining) {
                printf("%.*s", remaining, body);
                return true;
            }
        } else if (encoding == chunked) {
            do {
                if (remaining == 0) {
                    if ((q = strstr(body, "\r\n"))) {
                        remaining = strtol(body, 0, 16);
                        if (!remaining) { 
                            error("remainning 0");
                        }
                        body = q + 2;
                    } else {
                        return true;
                    }
                }
                if (remaining && p - body >= remaining) {
                    printf("%.*s", remaining, body);
                    body += remaining + 2;
                    remaining = 0;
                }
            } while (!remaining);
        }
    }

    return false;
}

void http_test(s8 *url)
{
    s8 *hostname, *port, *path;
    printf("URL: %s\n", url);

    parse_url(url, &hostname, &port, &path);

    printf("hostname: %s\n", hostname);
    printf("port: %s\n", port);
    printf("path: %s\n", path);

    http_init(hostname, port);
    http_send_request(hostname, port, path);

    struct pollfd pfd;

    pfd.fd = http.sock.fd;
    pfd.events = POLLOUT;

    for (;;) {
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 200000;

        if (poll(&pfd, 1, &timeout) > 0) {
            if (pfd.revents & POLLOUT) {            
                if (http_recv()) {
                    break;
                }
            }
        } else {
            sys_error();
        }
    }
}