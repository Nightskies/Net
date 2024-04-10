#include "address_in.h"

#include "socket.h"

#include "../error.h"

#include <netdb.h>
#include <strings.h>
#include <string.h>

void address_in_init(struct address *addr, enum sock_family family, enum sock_type type, s8 *host, s8 *service)
{
    bzero(addr, sizeof(struct address));

    struct addrinfo hints;
    bzero(&hints, sizeof(hints));
    hints.ai_family = (family != SOCKET_AF_UNDEFINE ? socket_family_map(family) : 0);
    hints.ai_socktype = (type != SOCKET_UNDEFINE ? socket_type_map(type) : 0);
    struct addrinfo *peer_address;
    if (getaddrinfo(host, service, &hints, &peer_address)) {
        gai_error();
    }

    memcpy(&addr->saddr, peer_address->ai_addr, sizeof(struct sockaddr));

    freeaddrinfo(peer_address);

    addr->socklen = sizeof(struct sockaddr_in);

    /*
    TODO: Use own DNS for resolve host and service

    struct sockaddr_in *addr = (struct sockaddr_in *)&ain->addr;
    addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
    addr->sin_addr.s_addr = htonl(ip);

    */
}