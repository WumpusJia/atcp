#ifndef ATCP_SKBUFF_H
#define ATCP_SKBUFF_H

#include "netdevice.h"
#include "common.h"
#include "dst.h"

struct sk_buff
{
    struct netdevice *dev;
    struct dst_entry * dst;

    uint16_t protocol;

    uint32_t len;
    uint8_t *head;
    uint8_t *data;
    uint8_t *tail;
    uint8_t *end;

};

struct sk_buff * alloc_skb(uint32_t datasize);
void free_skb(struct sk_buff* skb);



static  uint8_t* __skb_put(struct sk_buff* skb,uint32_t len);
static  uint8_t * __skb_push(struct sk_buff * skb,uint32_t len);

static  uint8_t* __skb_pull(struct sk_buff *skb,uint32_t len);
uint8_t* skb_put(struct sk_buff* skb,uint32_t len);

uint8_t * skb_push(struct sk_buff * skb,uint32_t len);
uint8_t* skb_pull(struct sk_buff *skb,uint32_t len);
void skb_reserve(struct sk_buff* skb,uint32_t len);


#endif
