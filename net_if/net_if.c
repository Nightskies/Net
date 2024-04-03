#include "net_if.h"

#include "../error.h"

#include <string.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

void net_if_init(struct net_if *interface, s32 fd, s32 family)
{
    memset(interface, 0, sizeof(struct net_if));

    struct ifaddrs *ifa, *ifa_tmp;

    if (getifaddrs(&ifa) < 0) {
        sys_error();
    }
    
    ifa_tmp = ifa;
    while (ifa_tmp) {
        if (ifa_tmp->ifa_addr && ifa_tmp->ifa_addr->sa_family == family) {
            if (!(ifa_tmp->ifa_flags & IFF_LOOPBACK)) {
                strncpy(interface->ifr.ifr_name, ifa_tmp->ifa_name, IF_NAMESIZE);
                break;
            }
        }
        ifa_tmp = ifa_tmp->ifa_next;
    }
    freeifaddrs(ifa);

    interface->fd = fd;
}

s32 net_if_get_index(struct net_if *interface)
{
    if (ioctl(interface->fd, SIOCGIFINDEX, &interface->ifr) < 0) {
        sys_error();
    }

    return interface->ifr.ifr_ifindex;
}

u8* net_if_get_ha(struct net_if *interface)
{
    if (ioctl(interface->fd, SIOCGIFHWADDR, &interface->ifr) < 0) {
        sys_error();
    }

    return interface->ifr.ifr_hwaddr.sa_data;
}

u32 net_if_get_pa(struct net_if *interface)
{
    if (ioctl(interface->fd, SIOCGIFADDR, &interface->ifr) < 0) {
        sys_error();
    }

    return ((struct sockaddr_in *)&(interface->ifr.ifr_addr))->sin_addr.s_addr;
}