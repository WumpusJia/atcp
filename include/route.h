#ifndef ATCP_ROUTE_H
#define ATCP_ROUTE_H


#include "util.h"
#include "dst.h"
#include "skbuff.h"
#include "inetpeer.h"
#include "flow.h"


struct inet_peer; //Declare to aviod warnings

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
uint16_t ip_select_id(struct dst_entry * dst);

int ip_route_input(struct sk_buff* skb,uint32_t dip,uint32_t sip);
int ip_route_output(struct rtable** res,struct flowi* fl);



#endif
