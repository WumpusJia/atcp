#include "route.h"


int ip_route_input(struct sk_buff* skb,uint32_t dip,uint32_t sip)
{
    struct rtable * rtp = &rt_hash_table[0];
    rtp->rt_dst = sip;
    rtp->rt_src = dip;

    skb->dst = (struct dst_entry *)rtp;

    return 1;
}
