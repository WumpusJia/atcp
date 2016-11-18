#include "netdevice.h"
#include "tuntap.h"
#include "arp.h"
#include "ethernet.h"

void netdev_init(char* ip,char *mac)
{
    struct netdevice* dev = &netdev;

    if(inet_pton(AF_INET,ip,&dev->ip) != 1)
    {
        puts("ERROR: parsing ip");
        exit(1);
    }

    dev->ip = ntohl(dev->ip);
    sscanf(mac,"%hhu:%hhu:%hhu:%hhu:%hhu:%hhu",
        &dev->mac[0],&dev->mac[1],&dev->mac[2],&dev->mac[3],&dev->mac[4],
        &dev->mac[5]);

}

int net_tx_action(struct sk_buff* skb,uint8_t * dst_mac,uint16_t type)
{
    skb_push(skb,ETH_HEADER_LEN);
    struct eth_header* ethhdr = NULL;//(struct eth_header*) skb->data;

    struct netdevice* dev = skb->dev;

    memcpy(ethhdr->daddr,dst_mac,sizeof(uint8_t)*ETH_MAC_LEN);
    memcpy(ethhdr->saddr,dev->mac,sizeof(uint8_t)*ETH_MAC_LEN);

    ethhdr->type = htons(type);

    int res = tun_write((char*)skb->data,skb->len);
    free_skb(skb);

    return res;
}


void net_rx_action(struct sk_buff* skb)
{
    struct eth_header * hdr = init_eth_header(skb);
    skb_pull(skb,ETH_HEADER_LEN);

    switch (hdr->type)
    {
        case ETH_P_ARP:
            arp_solve(skb);
            break;
        default:
            puts("Unknown packet");
            break;
    }
    free_skb(skb);
}


void net_rx_loop()
{
    while(1)
    {
        struct sk_buff * skb = alloc_skb(MTU);
        tun_read(skb->data,MTU);
        net_rx_action(skb);
    }
}
