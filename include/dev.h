#ifndef ATCP_DEV_H
#define ATCP_DEV_H

#include "skbuff.h"
#include "common.h"

void net_init(char *s);
void net_free();
int net_tx_action(struct sk_buff* skb,uint8_t * dst_mac,uint16_t type);
void netif_receive_skb(struct sk_buff* skb);
void net_rx_loop();
//void consume_rx_pkt_loop();

#endif
