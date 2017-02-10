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

    skb->alloc_mode = SKBUFF_ALLOC_DEFAULT;
}

struct sk_buff * alloc_p_skb(uint8_t *buf,uint32_t len)
{
    struct sk_buff *skb = malloc(sizeof(struct sk_buff));

    skb->dev = NULL;
    skb->dst = NULL;
    skb->list = NULL;
    skb->prev = NULL;
    skb->next = NULL;
    skb->protocol = 0;

    skb->len = 0;

    skb->data = buf;
    skb->head = skb->data;
    skb->tail = skb->data;
    skb->end = skb->data+ len;

    skb->alloc_mode = SKBUFF_ALLOC_PARTIAL;
}


void free_skb(struct sk_buff* skb)
{
    if(skb->alloc_mode == SKBUFF_ALLOC_DEFAULT)
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

///////////////////////////////////
void skb_queue_init(struct sk_buff_head * list)
{
    list->prev = (struct sk_buff *)list;
    list->next = (struct sk_buff *)list;
    list->len = 0;
    pthread_mutex_init(&list->lock,NULL);
}



void skb_queue_push_front(struct sk_buff_head * list,struct sk_buff * skb)
{
    pthread_mutex_lock(&list->lock);
    skb->list = list;

    struct sk_buff * tprev = (struct sk_buff *)list;
    struct sk_buff* tnext = list->next;

    skb->prev = tprev;
    skb->next = tnext;

    tprev->next = skb;
    tnext->prev = skb;

    list->len++;
    pthread_mutex_unlock(&list->lock);
}

void skb_queue_push_back(struct sk_buff_head * list,struct sk_buff * skb)
{
    pthread_mutex_lock(&list->lock);
    skb->list = list;

    struct sk_buff * tnext = (struct sk_buff *)list;
    struct sk_buff* tprev = list->prev;

    skb->prev = tprev;
    skb->next = tnext;

    tprev->next = skb;
    tnext->prev = skb;

    list->len++;
    pthread_mutex_unlock(&list->lock);
}


struct sk_buff * skb_queue_pop_front(struct sk_buff_head *list)
{
    pthread_mutex_lock(&list->lock);
    struct sk_buff * tprev = (struct sk_buff*) list;
    struct sk_buff * res = list->next;
    if(tprev == res)
    {
        pthread_mutex_unlock(&list->lock);
        return NULL;
    }

    struct sk_buff * tnext = res->next;
    tprev->next = tnext;
    tnext->prev = tprev;
    list->len--;

    res->next = NULL;
    res->prev = NULL;
    res->list = NULL;
    pthread_mutex_unlock(&list->lock);
    return res;
}


struct sk_buff * skb_queue_pop_back(struct sk_buff_head *list)
{
    pthread_mutex_lock(&list->lock);
    struct sk_buff * tnext = (struct sk_buff*) list;
    struct sk_buff * res = list->prev;
    if(tnext == res)
    {
        pthread_mutex_unlock(&list->lock);
        return NULL;
    }
    struct sk_buff * tprev = res->prev;
    tprev->next = tnext;
    tnext->prev = tprev;
    list->len--;

    res->next = NULL;
    res->prev = NULL;
    res->list = NULL;
    pthread_mutex_unlock(&list->lock);
    return res;
}
