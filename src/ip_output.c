#include "skbuff.h"
#include "util.h"
#include "ip.h"
#include "route.h"
#include "neighbour.h"
#include "arp.h"



int ip_forward(struct sk_buff * skb)
{

}
int ip_output(struct sk_buff * skb)
{

    struct rtable* rth = (struct rtable*) skb->dst;
    skb_push(skb,IP_HEADER_LEN);

    struct ip_header * hdr = get_ip_header(skb);
    hdr->version = 4;
    hdr->ihl = 5;
    hdr->tos = 0;
    hdr->totlen = skb->len;

    hdr->id = ip_select_id(skb->dst);

    hdr->frag = 0;
    hdr->ttl = 64;
    hdr->protocol = skb->protocol;

    hdr->srcip = rth->rt_src;
    hdr->dstip = rth->rt_dst;

    hdr->checksum = 0;
    reset_ip_header(hdr);


    hdr->checksum = checksum(hdr,hdr->ihl*2);
    hdr->checksum = htons(hdr->checksum);


    struct neighbour * n = skb->dst->neighbour; //temporary here.



    return n->ops->output(skb);
}

int ip_send(struct sk_buff * skb)
{

    struct sock* sk = &skb->sk;


    if(skb->dst == NULL)
    {
        
        struct flowi fl = {
            .iif = 0,
            .oif = sk->dev_if,
            .un = {
                .ip4= {
                        .dstip = sk->dip,
                        .srcip = sk->sip,
                    },

                },};

        if(!ip_route_output((struct rtable**)&skb->dst,&fl))
        {
            puts("ERROR: route output!");
            return 0;
        }
    }
    return skb->dst->output(skb);
}
