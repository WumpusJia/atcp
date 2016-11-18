#ifndef ATCP_NETDEVICE_H
#define ATCP_NETDEVICE_H

#define MAX_MAC_LEN 6

#include "common.h"
#include "skbuff.h"

struct netdevice
{
    //now only support ethernet and ipv4
    uint8_t mac[MAX_MAC_LEN];
    uint32_t ip;
};

static struct netdevice netdev;

int net_tx_action(struct sk_buff* skb,uint8_t * dst_mac,uint16_t type);
void netdev_init(char* ip,char *mac);


#endif
