#ifndef ATCP_SKBUFF_H
#define ATCP_SKBUFF_H

#include "netdevice.h"
#include "common.h"

struct sk_buff
{
    struct netdevice *dev;
    uint16_t protocol;

    uint32_t len;
    uint8_t *head;
    uint8_t *data;
    uint8_t *tail;
    uint8_t *end;

};

struct sk_buff * alloc_skb(uint32_t datasize);
void skb_free(struct sk_buff* skb);



static inline uint8_t* __skb_put(struct sk_buff* skb,uint32_t len)
{
    uint8_t *tmp = skb->tail;
    skb->tail += len;
    skb->len += len;
    return tmp;
}

static inline uint8_t * __skb_push(struct sk_buff * skb,uint32_t len)
{
    skb->data -= len;
    skb->len += len;
    return skb->data;
}

static inline uint8_t* __skb_pull(struct sk_buff *skb,uint32_t len)
{
    skb->len -= len;
    return skb->data += len;
}

inline uint8_t* skb_put(struct sk_buff* skb,uint32_t len)
{
    return __skb_put(skb,len);
}

inline uint8_t * skb_push(struct sk_buff * skb,uint32_t len)
{
    return __skb_push(skb,len);
}
inline uint8_t* skb_pull(struct sk_buff *skb,uint32_t len)
{
    return __skb_pull(skb,len);
}
inline void skb_reverse(struct sk_buff* skb,uint32_t len)
{
    skb->data += len;
    skb->tail += len;
}


#endif
