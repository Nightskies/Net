#include "../net_hdr/dns_hdr.h"

#include "../error.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define DNS_QUERY    0
#define DNS_RESPONSE 1

#define DNS_STANDARD_QUERY      0
#define DNS_REVERSE_QUERY       1
#define DNS_SERVER_STATUS_QUERY 2

#define DNS_NON_RECURSIVE 0
#define DNS_RECURSIVE     1

#define DNS_PACKET_LEN 1300 // so far 1300

#define DNS_PORT "53"
#define ROUTER_IP "192.168.0.1"

void dns_init(void)
{}

void dns_resolve(s8 *host_name, s8 *req_type)
{
    u8 type;
    if (strcmp(req_type, "a") == 0) {
        type = 1;
    } else if (strcmp(req_type, "mx") == 0) {
        type = 15;
    } else if (strcmp(req_type, "txt") == 0) {
        type = 16;
    } else if (strcmp(req_type, "aaaa") == 0) {
        type = 28;
    } else if (strcmp(req_type, "any") == 0) {
        type = 255;
    } else {
        error("Unknown type. Use a, aaaa, txt, mx, or any");
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    struct addrinfo *peer_address;
    if (getaddrinfo(ROUTER_IP, DNS_PORT, &hints, &peer_address)) {
        gai_error();
    }

    u8 *send_packet = malloc(DNS_PACKET_LEN);
    if (send_packet == NULL) {
        sys_error();
    }

    u8 *recv_packet = malloc(DNS_PACKET_LEN);
    if (recv_packet == NULL) {
        sys_error();
    }

    s32 socket_peer = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);
    if (socket_peer < 0) {
        sys_error();
    } 

    dns_hdr_fill(send_packet, 45999, DNS_QUERY, DNS_STANDARD_QUERY, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0);
    dns_qst_fill(send_packet + sizeof(struct dns_hdr), host_name, type, 0x01);

    ssize_t sbytes = sendto(socket_peer, send_packet, DNS_PACKET_LEN, 0, peer_address->ai_addr, peer_address->ai_addrlen);
    if (sbytes < 0) {
        sys_error();
    }

    printf("Sent %ld bytes\nDNS packet:\n", sbytes);
    dns_dump(send_packet);
  
    ssize_t rbytes = recvfrom(socket_peer, recv_packet, DNS_PACKET_LEN, 0, 0, 0);
    if (rbytes < 0) {
        sys_error();
    }

    printf("Received %ld bytes\nDNS packet:\n", rbytes);
    dns_dump(recv_packet);
    printf("\n");
}