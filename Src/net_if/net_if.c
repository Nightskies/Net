#include "socket.h"
#include "../error.h"

#include <string.h>
#include <strings.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
//#include <netinet/in.h>

#include <net/if.h>
#include <stdlib.h>

struct net_if
{
    struct ifreq ifr;
}; 

static void net_if_find(struct net_if *nif, enum sock_family family)
{
    s32 sfamily = socket_family_map(family);

    struct ifaddrs *ifa, *ifa_tmp;
    if (getifaddrs(&ifa) < 0) {
        sys_error();
    }
    
    ifa_tmp = ifa;
    while (ifa_tmp) {
        if (ifa_tmp->ifa_addr && ifa_tmp->ifa_addr->sa_family == sfamily) {
            if (!(ifa_tmp->ifa_flags & IFF_LOOPBACK)) {
                strncpy(nif->ifr.ifr_name, ifa_tmp->ifa_name, IF_NAMESIZE);
                break;
            }
        }
        ifa_tmp = ifa_tmp->ifa_next;
    }
    freeifaddrs(ifa);
}

struct net_if *net_if_create(enum sock_family family)
{
    struct net_if *nif = (struct net_if *)malloc(sizeof(struct net_if));
    if (nif == NULL) {
        sys_error();
    }

    bzero(nif, sizeof(struct net_if));

    net_if_find(nif, family);

    return nif;
}

s32 net_if_get_index(struct net_if *nif, s32 fd)
{
    if (ioctl(fd, SIOCGIFINDEX, &nif->ifr) < 0) {
        sys_error();
    }

    return nif->ifr.ifr_ifindex;
}

u8* net_if_get_ha(struct net_if *nif, s32 fd)
{
    if (ioctl(fd, SIOCGIFHWADDR, &nif->ifr) < 0) {
        sys_error();
    }

    return nif->ifr.ifr_hwaddr.sa_data;
}

u32 net_if_get_pa(struct net_if *nif, s32 fd)
{
    if (ioctl(fd, SIOCGIFADDR, &nif->ifr) < 0) {
        sys_error();
    }

    return ((struct sockaddr_in *)&(nif->ifr.ifr_addr))->sin_addr.s_addr;
}