#include "route.h"

int ip_route_input(struct sk_buff* skb,uint32_t dip,uint32_t sip)
{
    struct rtable * rtp = &rt_hash_table[0];
    rtp->rt_dst = sip;
    rtp->rt_src = dip;

    skb->dst = (struct dst_entry *)rtp;

    return 1;
}


uint16_t ip_select_id(struct dst_entry * dst)
{
    struct rtable * rt = (struct rtable *) dst;
    if(rt)
    {
        if(rt->peer == NULL)
            rt_bind_peer(rt);

        if(rt->peer)
        {
            return inet_getid(rt->peer);
        }

    }
    else
    {
        puts("Id Select Error!");
        exit(1);
    }
}
