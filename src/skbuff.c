#include "skbuff.h"


struct sk_buff * alloc_skb(uint32_t datasize)
{
    struct sk_buff *skb = malloc(sizeof(struct sk_buff));

    skb->dev = NULL;
    skb->dst = NULL;
    skb->protocol = 0;

    skb->len = 0;

    skb->data = malloc(datasize);
    skb->head = skb->data;
    skb->tail = skb->data;
    skb->end = skb->data+ datasize;
}


void free_skb(struct sk_buff* skb)
{
    free(skb->head);
    free(skb);
}


static  uint8_t* __skb_put(struct sk_buff* skb,uint32_t len)
{
    uint8_t *tmp = skb->tail;
    skb->tail += len;
    skb->len += len;
    return tmp;
}

static  uint8_t * __skb_push(struct sk_buff * skb,uint32_t len)
{
    skb->data -= len;
    skb->len += len;
    return skb->data;
}

static  uint8_t* __skb_pull(struct sk_buff *skb,uint32_t len)
{
    skb->len -= len;
    return skb->data += len;
}

 uint8_t* skb_put(struct sk_buff* skb,uint32_t len)
{
    return __skb_put(skb,len);
}

 uint8_t * skb_push(struct sk_buff * skb,uint32_t len)
{
    return __skb_push(skb,len);
}
 uint8_t* skb_pull(struct sk_buff *skb,uint32_t len)
{
    return __skb_pull(skb,len);
}
 void skb_reserve(struct sk_buff* skb,uint32_t len)
{
    skb->data += len;
    skb->tail += len;
}
