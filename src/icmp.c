#include "icmp.h"
#include "util.h"
#include "ip.h"
#include "ethernet.h"
#include "route.h"

struct icmp_header * get_icmp_header(struct sk_buff * skb)
{
    return (struct icmp_header * )skb->data;
}
void init_icmp_header(struct icmp_header* hdr)
{
    switch (hdr->type)
    {
        case ICMP_ECHO_REQUEST:
        case ICMP_ECHO_REPLY:
            hdr->un.echo.id = ntohs(hdr->un.echo.id);
            hdr->un.echo.seq = ntohs(hdr->un.echo.seq);
            break;
        default:
            break;
    }
}
void reset_icmp_header(struct icmp_header * hdr)
{
    switch (hdr->type)
    {
        case ICMP_ECHO_REQUEST:
        case ICMP_ECHO_REPLY:
            hdr->un.echo.id = htons(hdr->un.echo.id);
            hdr->un.echo.seq = htons(hdr->un.echo.seq);
            break;
        default:
            break;
    }
}

void icmp_solve(struct sk_buff * skb)
{
    puts("SOLVE icmp");
    struct icmp_header * hdr = get_icmp_header(skb);



    if(checksum(hdr,skb->len/2))
    {
        puts("ICMP Checksum failed!");
        return;
    }

    init_icmp_header(hdr);

    switch(hdr->type)
    {
        case ICMP_ECHO_REQUEST:
            icmp_echo_reply(skb);
            break;
        case ICMP_ECHO_REPLY:
            puts("Received echo reply");
            break;
        default:
            puts("Unknown ICMP TYPE");
            break;
    }
}



int icmp_echo_reply(struct sk_buff * skb)
{




    puts("Send ICMP reply");
    struct icmp_header * hdr = get_icmp_header(skb);

    hdr->type = ICMP_ECHO_REPLY;
    hdr->code = 0;
    hdr->checksum = 0;

    reset_icmp_header(hdr);
    hdr->checksum = checksum(hdr,skb->len/2);
    hdr->checksum = htons(hdr->checksum);

    skb->protocol = IP_ICMP;

    struct rtable* rth = (struct rtable* )skb->dst;

    skb->sk.sip = rth->rt_dst;
    skb->sk.dip = rth->rt_src;

    struct netdevice * dev = skb->dev;
    skb->sk.dev_if = dev->ifindex;

    skb->dst = NULL;

    

    return ip_send(skb);

}


static uint16_t _icmp_sent_seq_;

int icmp_echo_request(struct netdevice* dev,uint32_t dstip)
{
    puts("Send ICMP ECHO REQUEST");
    struct sk_buff * skb = alloc_skb(ETH_HEADER_LEN+IP_HEADER_LEN+ICMP_ECHO_LEN);

    skb_reserve(skb,ETH_HEADER_LEN+IP_HEADER_LEN);
    skb_put(skb,ICMP_ECHO_LEN);

    skb->dev = dev;

    skb->sk.dip = dstip;
    skb->sk.sip = dev->ip;
    skb->sk.dev_if = dev->ifindex;


    struct icmp_header * hdr = get_icmp_header(skb);

    memset(hdr,0,sizeof(ICMP_ECHO_LEN));

    hdr->type = ICMP_ECHO_REQUEST;
    hdr->code = 0;
    hdr->checksum = 0;

    reset_icmp_header(hdr);
    hdr->checksum = checksum(hdr,skb->len/2);
    hdr->checksum = htons(hdr->checksum);

    skb->protocol = IP_ICMP;

    return ip_send(skb);

}
