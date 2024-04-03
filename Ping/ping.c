
#include "../net_hdr/ip_hdr.h"
#include "../net_hdr/icmp_hdr.h"
#include "../net_if/net_if.h"

#include "../error.h"
#include "../timer.h"

#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

#define IPv4 4
#define IPv4_IHL 5
#define	IP_DF 0x4000	

#define ICMP_ECHO 8

#define MAX_REQUESTS 4

struct ping_stat
{
    struct timer timer;
    u32 transmitted_packets;
    u32 received_packets;
    f64 total_time;
};

static struct ping
{
    struct net_if nif;
    struct ping_stat stat;
} ping;

static void ping_print_total_stat()
{
    puts("\n--- ping stat ---");
    printf("%u packets transmitted, %u received, time %.2fms \n", ping.stat.transmitted_packets, 
        ping.stat.received_packets, ping.stat.total_time);
}

static void ping_print_packet_stat(u8 *packet, f64 packet_time)
{
    struct ip_hdr *ip = (struct ip_hdr *)packet;
    s8 ip_addr[INET_ADDRSTRLEN] = {};
    if (inet_ntop(AF_INET, &ip->saddr, ip_addr, INET_ADDRSTRLEN) == NULL) {
        sys_error();
    }

    u8 ttl = ip->ttl;

    printf("%u bytes from %s: ttl=%hhu time=%.2f ms\n", 48, ip_addr, ttl, packet_time);
}

static void interrupt_handler(s32 sig)
{
    timer_end(&ping.stat.timer);
    ping.stat.total_time = timer_get_elapsed_time(&ping.stat.timer);
    ping_print_total_stat();
    exit(EXIT_SUCCESS);
}

void ping_init()
{
    bzero(&ping, sizeof(struct ping));

    s32 sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sfd < 0){
	   sys_error();
    }

    const s32 val = 1;
    if (setsockopt(sfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(s32)) < 0) {
        sys_error();
    }

    net_if_init(&ping.nif, sfd, AF_INET);

    if (signal(SIGINT, interrupt_handler) == SIG_ERR) {
        sys_error();
    }

    srand(time(NULL));
}

void ping_test(s8* peer_host)
{
    struct addrinfo hint;
    struct addrinfo *peer_addr;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;

    if (getaddrinfo(peer_host, 0, &hint, &peer_addr) < 0) {
        gai_error();
    }

    u64 packet_len = sizeof(struct ip_hdr) + sizeof(struct icmp_hdr);
    u8 *send_packet = (u8 *)malloc(packet_len);
    if (send_packet == NULL) {
        sys_error();
    }
    u8 *recv_packet = (u8 *)malloc(packet_len);
    if (recv_packet == NULL) {
        sys_error();
    }

    bzero(send_packet, sizeof(struct ip_hdr) + sizeof(struct icmp_hdr));

    u32 saddr = net_if_get_pa(&ping.nif);
    u32 daddr = ((struct sockaddr_in*)(peer_addr->ai_addr))->sin_addr.s_addr;
    u16 id = rand() % 255;
    u16 seq = rand() % 255;

    struct ip_hdr *ip = (struct ip_hdr *)send_packet;
   
    ip_hdr_fill(ip, IPv4_IHL, IPv4, 0, packet_len, 
        0, IP_DF, 64, IPPROTO_ICMP, saddr, daddr);

    struct icmp_hdr *icmp = (struct icmp_hdr *)(send_packet + sizeof(struct ip_hdr));

    icmp_hdr_fill(icmp, ICMP_ECHO, 0, id, seq);

    timer_init(&ping.stat.timer);
    timer_start(&ping.stat.timer);

    sigset_t newmask, oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

    struct timer packet_timer;

    for (s32 i = 0; i < MAX_REQUESTS; ++i) {
        timer_delay(500);

        if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
            sys_error();
        }
    
        timer_init(&packet_timer);
        timer_start(&packet_timer);

        //TODO: set i+1 icmp seq for each new packet send
        ssize_t sbytes = sendto(ping.nif.fd, send_packet, packet_len, 0, peer_addr->ai_addr, peer_addr->ai_addrlen);
	    if (sbytes < 0) {
            sys_error();
    	}

        bzero(recv_packet, sizeof(struct ip_hdr) + sizeof(struct icmp_hdr));

        ping.stat.transmitted_packets++;

        //TODO: make timer for recv that process is not blocked forever
        ssize_t rbytes = recvfrom(ping.nif.fd, recv_packet, packet_len, 0, NULL, NULL);
        if (rbytes < 0) {
            sys_error();
        }

        timer_end(&packet_timer);

        f64 packet_time = timer_get_elapsed_time(&packet_timer);

        ping.stat.received_packets++;

        ping_print_packet_stat(recv_packet, packet_time);

        if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
            sys_error();
        }
    }

    if (raise(SIGINT) != 0) {
        sys_error();
    }
}