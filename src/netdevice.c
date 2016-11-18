#include "common.h"
#include "ethernet.h"
#include "tuntap.h"
#include "netdevice.h"

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
    struct eth_header* ethhdr = (struct eth_header*) skb->data;

    struct netdevice* dev = skb->dev;

    memcpy(ethhdr->daddr,dst_mac,sizeof(uint8_t)*ETH_MAC_LEN);
    memcpy(ethhdr->saddr,dev->mac,sizeof(uint8_t)*ETH_MAC_LEN);

    ethhdr->type = htons(type);

    int res = tun_write((char*)skb->data,skb->len);
    free_skb(skb);

    return res;
}
