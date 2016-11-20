#ifndef ATCP_INETPEER_H
#define ATCP_INETPEER_H

#include "util.h"
#include "route.h"

struct inet_peer
{
    uint16_t ip_id_count;
};

static struct inet_peer inet_peer_cache;

void rt_bind_peer(struct rtable* rt);
uint16_t inet_getid(struct inet_peer * p);


#endif
