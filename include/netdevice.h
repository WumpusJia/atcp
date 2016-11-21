#ifndef ATCP_NETDEVICE_H
#define ATCP_NETDEVICE_H

#define MAX_MAC_LEN 6

#define MTU 1500

#include "skbuff.h"
#include "common.h"


struct netdevice
{
    //now only support ethernet and ipv4
    uint8_t mac[MAX_MAC_LEN];
    uint32_t ip;
};

struct netdevice* netdev_get();
int net_tx_action(struct sk_buff* skb,uint8_t * dst_mac,uint16_t type);
void net_rx_action(struct sk_buff* skb);
void netdev_init(char* ip,char *mac);

void net_rx_loop();

#endif
