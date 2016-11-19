#include "netdevice.h"
#include "tuntap.h"
#include "arp.h"
#include "ethernet.h"
#include "ip.h"


static struct netdevice _netdev_;

struct netdevice* netdev_get()
{
    return &_netdev_;
}

void netdev_init(char* ip,char *mac)
{
    struct netdevice* dev = &_netdev_;

    if(inet_pton(AF_INET,ip,&dev->ip) != 1)
    {
        puts("ERROR: parsing ip");
        exit(1);
    }

    dev->ip = ntohl(dev->ip);
    sscanf(mac,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &dev->mac[0],&dev->mac[1],&dev->mac[2],&dev->mac[3],&dev->mac[4],
        &dev->mac[5]);

}

int net_tx_action(struct sk_buff* skb,uint8_t * dst_mac,uint16_t type)
{
    skb_push(skb,ETH_HEADER_LEN);
    struct eth_header* ethhdr = (struct eth_header*) skb->data;

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
    skb->dev = netdev_get();
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


void net_rx_loop()
{
    while(1)
    {
        struct sk_buff * skb = alloc_skb(MTU);
        uint32_t len = tun_read(skb->data,MTU);
        skb_put(skb,len);
        net_rx_action(skb);
    }
}
