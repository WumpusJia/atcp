#include "skbuff.h"
#include "util.h"
#include "ip.h"
#include "route.h"
#include "neighbour.h"
#include "arp.h"


int ip_send(struct sk_buff * skb) //skb->dst has been calculated
{

// ip_route_output() ----

    arp_bind_neighbour(skb->dst); //temporary here.  should be put into route subsystem



    //todo Support ip option

    struct rtable* rt = (struct rtable *)skb->dst;


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

    hdr->srcip = rt->rt_src;
    hdr->dstip = rt->rt_dst;

    hdr->checksum = 0;
    reset_ip_header(hdr);


    hdr->checksum = checksum(hdr,hdr->ihl*2);
    hdr->checksum = htons(hdr->checksum);


    struct neighbour * n = skb->dst->neighbour; //temporary here.

    

    return n->ops->output(skb);

}
