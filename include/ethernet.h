#ifndef ATCP_ETHERNET_H
#define ATCP_ETHERNET_H

#include "common.h"
#include "skbuff.h"


#define ETH_HEADER_LEN 14
#define ETH_MAC_LEN 6


struct eth_header
{
    uint8_t daddr[ETH_MAC_LEN];
    uint8_t saddr[ETH_MAC_LEN];
    uint16_t type;
    uint8_t payload[];

}__attribute__((packed));;

void eth_print(struct eth_header* hdr);
struct eth_header* init_eth_header(struct sk_buff * skb);


static uint8_t BROADCAST_MAC[] = {0xff,0xff,0xff,0xff,0xff,0xff};

#endif
