#include "route.h"
#include "fib.h"
#include "flow.h"
#include "neighbour.h"
#include "ip.h"
#include "netdevice.h"

#define RTABLE_HASH_MOD 233
#define RTABLE_HASH_BASE 57

struct rt_hash_bucket
{
    struct rtable* queue;
    pthread_rwlock_t lock;
};

static struct rt_hash_bucket rt_hash_table[RTABLE_HASH_MOD];

int ip_rt_init()
{
    for(int i = 0;i < RTABLE_HASH_MOD;++i)
    {
        rt_hash_table[i].queue = NULL;
        pthread_rwlock_init(&rt_hash_table[i].lock,NULL);
    }
}

uint32_t rt_hash(uint32_t daddr,uint32_t saddr)
{
    uint32_t res = 0;
    uint8_t *p = &daddr;
    for(int i = 0;i < 4;++i)
    {
        res = (res*RTABLE_HASH_BASE+(*p))%RTABLE_HASH_MOD;
        p++;
    }
    p = &saddr;
    for(int i = 0;i < 4;++i)
    {
        res = (res*RTABLE_HASH_BASE+(*p))%RTABLE_HASH_MOD;
        p++;
    }

    return res;
}

struct rtable* ip_rt_insert(uint32_t hash,struct rtable* now)
{
    struct rtable* rth;

    //lock
    for(rth = rt_hash_table[hash].queue;rth;rth = rth->next)
    {
        if(rth->fl.fl4_dst == now->fl.fl4_dst &&
            rth->fl.fl4_src == now->fl.fl4_src &&
            rth->fl.iif == now->fl.iif &&
            rth->fl.oif == now->fl.oif )
            {
                return rth;
            }
    }



    if(now->rt_type == RTN_UNICAST || now->fl.iif == 0)
    {

        arp_bind_neighbour((struct dst_entry *)now);

    }

    now->next = rt_hash_table[hash].queue;
    rt_hash_table[hash].queue = now;

    return now;
}

////////////////////////////////////////////




struct rtable * rt_alloc()
{
    struct rtable* rt = malloc(sizeof(struct rtable));
    rt->dst.input = ip_io_error;
    rt->dst.output = ip_io_error;
    rt->dst.neighbour = NULL;
    rt->dst.dev = NULL;

    //not init rtable
    rt->peer = NULL;
    rt->next = NULL;

    return rt;
}


int ip_route_input_slow(struct sk_buff* skb,uint32_t dip,uint32_t sip)
{

    struct flowi fl = {
        .un = { .ip4 = {
                    .dstip = dip,
                    .srcip = sip,
                    .scope = RT_SCOPE_UNIVERSE,
                }},
                .iif = skb->dev->ifindex };
    struct netdevice * dev = skb->dev;
    struct fib_result res;
    struct rtable* rth;

    uint32_t hash = rt_hash(dip,sip);

    //check multicast | boardcast | loopback ....

    //query route table
    if(!fib_find(&fl,&res))
    {
        puts("ERROR: route table!");
    }

    if(res.type == RTN_LOCAL)
    {

        rth = rt_alloc();

        rth->dst.input = ip_local_deliver;

        rth->fl.fl4_dst = dip;
        rth->fl.fl4_src = sip;
        rth->rt_src = sip;
        rth->rt_dst = dip;
        rth->rt_gateway = dip;

        rth->dst.dev = dev;

        rth->fl.iif = dev->ifindex;
        rth->fl.oif = 0;

        rth->rt_type = res.type;

        skb->dst = (struct dst_entry * )ip_rt_insert(hash,rth);

        return 1;
    }

}




int ip_route_input(struct sk_buff* skb,uint32_t dip,uint32_t sip)
{
    uint32_t hash = rt_hash(dip,sip);
    uint32_t iif = skb->dev->ifindex;
    struct rtable* rth;

    //lock
    for(rth = rt_hash_table[hash].queue;rth;rth = rth->next)
    {
        if(rth->fl.fl4_dst == dip &&
            rth->fl.fl4_src == sip &&
            rth->fl.iif == iif &&
            rth->fl.oif == 0 )
            {
                skb->dst = (struct dst_entry *)rth;
                return 1;
            }
    }
    //unlock

    return ip_route_input_slow(skb,dip,sip);
}


int ip_route_output_slow(struct rtable** tar,struct flowi* fl)
{
    fl->fl4_scope = RT_SCOPE_UNIVERSE;

    struct netdevice * dev_out = netdev_get();
    struct fib_result res;
    struct rtable * rth;

    if(!fib_find(&fl,&res))
    {
        puts("ERROR: route table!");
    }

    if(res.type == RTN_UNICAST)
    {

        rth = rt_alloc();

        rth->dst.output = ip_output;

        rth->fl.fl4_dst = fl->fl4_dst;
        rth->fl.fl4_src = fl->fl4_src;
        rth->rt_src = fl->fl4_src;
        rth->rt_dst = fl->fl4_dst;
        rth->rt_gateway = fl->fl4_dst;

        rth->dst.dev = dev_out;

        rth->fl.iif = 0;
        rth->fl.oif = fl->oif;

        rth->rt_type = res.type;

        uint32_t hash = rt_hash(fl->fl4_dst,fl->fl4_src);



        *tar = ip_rt_insert(hash,rth);
        
        return 1;
    }

}



int ip_route_output(struct rtable** tar,struct flowi* fl)
{
    uint32_t hash = rt_hash(fl->fl4_dst,fl->fl4_src);
    struct rtable* rth;
    //lock
    for(rth = rt_hash_table[hash].queue;rth;rth = rth->next)
    {
        if(rth->fl.fl4_dst == fl->fl4_dst &&
            rth->fl.fl4_src == fl->fl4_src &&
            rth->fl.iif == 0 &&
            rth->fl.oif == fl->oif )
            {
                *tar = rth;
                return 1;
            }
    }
    //unlock

    return ip_route_output_slow(tar,fl);
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
