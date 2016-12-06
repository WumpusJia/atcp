#ifndef ATCP_ROUTE_H
#define ATCP_ROUTE_H


#include "util.h"
#include "dst.h"
#include "skbuff.h"
#include "inetpeer.h"
#include "flow.h"




struct rtable
{
    struct dst_entry dst;

    struct inet_peer * peer;

    uint32_t rt_dst;
    uint32_t rt_src;

    uint8_t rt_type;

    uint32_t rt_gateway;

    struct flowi fl;
    struct rtable* next;
};


int ip_rt_init();


int ip_route_input(struct sk_buff* skb,uint32_t dip,uint32_t sip);  //mock function
int ip_route_output(struct sk_buff* skb,uint32_t dip,uint32_t sip);



#endif
