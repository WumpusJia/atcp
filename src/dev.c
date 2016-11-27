#define NETMAP_WITH_LIBS
#include "net/netmap_user.h"
#include "dev.h"
#include "arp.h"
#include "ethernet.h"
#include "ip.h"
#include "poll.h"
#include "netdevice.h"

static struct nm_desc *_nmd_;



void net_init(char *s)
{
    char tmp[20];
    sprintf(tmp,"netmap:%s",s);
    _nmd_ = nm_open(tmp, NULL, 0, 0);
    if(_nmd_ == NULL)
    {
        puts("Error: netmap init failed!");
        exit(1);
    }

}



void netif_receive_skb(struct sk_buff* skb)
{

    struct eth_header * hdr = init_eth_header(skb);
    skb_pull(skb,ETH_HEADER_LEN);



    switch (hdr->type)
    {
        case ETH_P_ARP:
            arp_solve(skb);
            break;
        case ETH_P_IP:
            ip_solve(skb);
            break;
        case ETH_P_IPV6:
            puts("Unsupport ipv6");
            break;
        default:
            puts("Unknown packet");
            break;
    }
}



static void process_pkt(char*buf,struct nm_pkthdr* p)
{
    uint32_t len = p->len;

    struct sk_buff* skb = alloc_skb(MTU);
    skb_put(skb,len);
    memcpy(skb->data,buf,len*sizeof(uint8_t));

    //do some init here
    skb->dev = netdev_get();

    netif_receive_skb(skb);

}


void net_rx_loop()
{
    struct pollfd fds;
    char *buf;
    struct nm_pkthdr h;

    fds.fd = NETMAP_FD(_nmd_);
    fds.events = POLLIN;
    while(1)
    {
         poll(&fds, 1, -1);
         while ( (buf = nm_nextpkt(_nmd_, &h)) )
             process_pkt(buf, &h);
     }
     nm_close(_nmd_);
}





int net_tx_action(struct sk_buff* skb,uint8_t * dst_mac,uint16_t type)
{


    skb_push(skb,ETH_HEADER_LEN);
    struct eth_header* ethhdr = (struct eth_header*) skb->data;

    struct netdevice* dev = skb->dev;
    memcpy(ethhdr->daddr,dst_mac,sizeof(uint8_t)*ETH_MAC_LEN);
    memcpy(ethhdr->saddr,dev->mac,sizeof(uint8_t)*ETH_MAC_LEN);

    ethhdr->type = htons(type);


    struct pollfd fds;
    fds.fd = NETMAP_FD(_nmd_);
    fds.events = POLLOUT;
    poll(&fds, 1, -1);

    int res = nm_inject(_nmd_,(char*)skb->data,skb->len);


    free_skb(skb);
    return res;
}
