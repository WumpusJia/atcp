#ifndef ATCP_ROUTE_H
#define ATCP_ROUTE_H


#include "util.h"
#include "dst.h"
#include "skbuff.h"
#include "inetpeer.h"

struct rtable
{
    struct dst_entry dst;

    struct inet_peer * peer;

    uint32_t rt_dst;
    uint32_t rt_src;
};


#define RTABLE_SIZE 20
static struct rtable rt_hash_table[RTABLE_SIZE];

int ip_route_input(struct sk_buff* skb,uint32_t dip,uint32_t sip);  //mock function




#endif
