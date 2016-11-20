#include "skbuff.h"
#include "ip.h"
#include "util.h"
#include "route.h"



struct ip_header * get_ip_header(struct sk_buff* skb)
{
    return (struct ip_header *)skb->data;
}

void init_ip_header(struct ip_header* hdr)
{
    hdr->totlen = ntohs(hdr->totlen);
    hdr->id = ntohs(hdr->id);
    hdr->frag = ntohs(hdr->frag);
    hdr->srcip = ntohl(hdr->srcip);
    hdr->dstip = ntohl(hdr->dstip);

}

void reset_ip_header(struct ip_header * hdr)
{
    hdr->totlen = htons(hdr->totlen);
    hdr->id = htons(hdr->id);
    hdr->frag = htons(hdr->frag);
    hdr->srcip = htonl(hdr->srcip);
    hdr->dstip = htonl(hdr->dstip);
}


void ip_solve(struct sk_buff* skb)
{
    puts("SOLVE: IP");

    struct ip_header * hdr = get_ip_header(skb);

    if(ip_header_checksum(hdr,hdr->ihl*2) != 0)
    {
        puts("IP Header checksum error!");
        return;
    }

    init_ip_header(hdr);

    //Suppose version is ipv4
    assert(hdr->version == 0x04);

    if(hdr->ttl == 0)
    {
        puts("TTL = 0,TimeOut");
        //Todo: send icmp
        return;
    }


    //ip_rcv_finish

    if(skb->dst == NULL)
    {
        if(! ip_route_input(skb,hdr->dstip,hdr->srcip)) //route subsystem
        {
            puts("Route error : drop");
            return;
        }

    }

    switch(hdr->protocol)
    {
        case IP_TCP:
            puts("Solve TCP");
            break;
        case IP_ICMP:
            puts("Solve icmp");
            break;
        default:
            puts("Unknown IP datagram");
            break;

    }

}
