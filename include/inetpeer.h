#ifndef ATCP_INETPEER_H
#define ATCP_INETPEER_H

#include "util.h"
#include "route.h"

/*
Need datastructure : AVL. To support quick find/edit operator


*/


struct inet_peer
{
    uint16_t ip_id_count;
};



void rt_bind_peer(struct rtable* rt);
uint16_t inet_getid(struct inet_peer * p);


#endif
