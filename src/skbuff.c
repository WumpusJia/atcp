#include "skbuff.h"


struct sk_buff * alloc_skb(uint32_t datasize)
{
    struct sk_buff *skb = malloc(sizeof(struct sk_buff));

    skb->dev = NULL;
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
