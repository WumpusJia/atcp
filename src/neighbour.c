#include "neighbour.h"
#include "ip.h"
#include "arp.h"
#include "dev.h"


static struct neightbour * neigh_alloc(struct neigh_table * tbl,const void* key)
{
    struct neighbour * n;
    n = malloc(sizeof(struct neighbour));
    n->tbl = tbl;

    n->nud_state = NUD_NONE;
    n->req_cnt = 0;
    memset(n->ip,0,sizeof(n->ip));
    memset(n->mac,0,sizeof(n->mac));
    n->next = NULL;
    n->prev = NULL;
    n->ops = NULL;

    skb_queue_init(&n->queue);

    tbl->constructor(n);

    memcpy(n->ip,key,tbl->key_len);

    pthread_rwlock_init(&n->lock,NULL);

    return n;
}


static struct neighbour ** neigh_hash_alloc(unsigned int nums)
{
    unsigned long size = nums * sizeof(struct neighbour *);
    struct neighbour ** res = malloc(size);
    memset(res,NULL,size);
    return res;
}


void neigh_table_init(struct neigh_table * tbl)
{
    tbl->now_size = 0;
    tbl->hash_buckets = neigh_hash_alloc(tbl->hash_size);

    pthread_rwlock_init(&tbl->lock,NULL);
}



struct neighbour * neigh_find(struct neigh_table* tbl,const void* key)
{
    uint32_t val = tbl->hash(key);
    struct neighbour* p = NULL;

    pthread_rwlock_rdlock(&tbl->lock);
    for(p = tbl->hash_buckets[val];p != NULL;p = p->next)
    {
        if(!memcmp(p->ip,key,tbl->key_len))
        {
            break;
        }
    }
    pthread_rwlock_unlock(&tbl->lock);

    return p;
}

/*
need to check if exist
*/
int  neigh_insert(struct neigh_table* tbl,struct neighbour * n)
{
    //check if maxsize

    uint32_t val = tbl->hash(n->ip);

    pthread_rwlock_wrlock(&tbl->lock);
    n->next = tbl->hash_buckets[val];
    tbl->hash_buckets[val] = n;
    pthread_rwlock_unlock(&tbl->lock);

    return 1;
}



struct neighbour* neigh_lookup(struct neigh_table * tbl,const void* key)
{
    struct neighbour* n = neigh_find(tbl,key);
    if(n)
        return n;
    else
    {
        n = neigh_alloc(tbl,key);
        if(neigh_insert(tbl,n))
            return n;
    }
}


int neigh_update(struct neighbour* neigh,uint8_t * mac,uint8_t new)
{

    pthread_rwlock_wrlock(&neigh->lock);
    if(neigh->nud_state == NUD_NONE && new == NUD_REACHABLE)
        neigh->req_cnt = 0;
    neigh->nud_state = new;
    if(mac)
        memcpy(neigh->mac,mac,ETH_MAC_LEN);

    pthread_rwlock_unlock(&neigh->lock);



    return 1;
}

void neigh_free(struct neigh_table * tbl,struct neighbour * n)
{
    //todo

}


/////////////////////////////////////////////



void neigh_queue_push_front(struct neighbour* neigh,struct sk_buff * skb)
{
    pthread_rwlock_wrlock(&neigh->lock);
    skb_queue_push_front(&neigh->queue,skb);
    pthread_rwlock_unlock(&neigh->lock);
}
void neigh_queue_push_back(struct neighbour* neigh,struct sk_buff * skb)
{
    pthread_rwlock_wrlock(&neigh->lock);
    skb_queue_push_back(&neigh->queue,skb);
    pthread_rwlock_unlock(&neigh->lock);
}
struct sk_buff * neigh_queue_pop_front(struct neighbour* neigh)
{
    struct sk_buff * res = NULL;
    pthread_rwlock_wrlock(&neigh->lock);
    res = skb_queue_pop_front(&neigh->queue);
    pthread_rwlock_unlock(&neigh->lock);
    return res;
}
struct sk_buff * neigh_queue_pop_back(struct neighbour* neigh)
{
    struct sk_buff * res = NULL;
    pthread_rwlock_wrlock(&neigh->lock);
    res = skb_queue_pop_back(&neigh->queue);
    pthread_rwlock_unlock(&neigh->lock);
    return res;
}

///////////////////////////////////////////
void neigh_queue_free(struct neighbour * neigh)
{
    struct sk_buff * now = NULL;
    while( (now = neigh_queue_pop_front(neigh)) != NULL)
    {
        free_skb(now);
    }
}

void neigh_queue_send_all(struct neighbour * neigh)
{
    struct sk_buff * now = NULL;
    while( (now = neigh_queue_pop_front(neigh)) != NULL)
    {
        net_tx_action(now,neigh->mac,now->protocol);
    }
}


int neigh_event_rcv(struct neigh_table * tbl,uint8_t * mac,void* ip)
{

    struct neighbour * n = neigh_lookup(tbl,ip);
    if(n)
    {

        pthread_rwlock_rdlock(&n->lock);
        if(n->nud_state == NUD_NONE)
        {
            pthread_rwlock_unlock(&n->lock);
            neigh_update(n,mac,NUD_REACHABLE);
            neigh_queue_send_all(n);
        }

    }
    return n;

}


int neigh_event_send(struct neighbour * neigh,struct sk_buff * skb)
{

    pthread_rwlock_rdlock(&neigh->lock);
    uint8_t status = neigh->nud_state;
    pthread_rwlock_unlock(&neigh->lock);

    if(status == NUD_REACHABLE)
    {
        pthread_rwlock_rdlock(&neigh->lock);
        uint8_t buf[ETH_MAC_LEN];
        memcpy(buf,neigh->mac,ETH_MAC_LEN);
        pthread_rwlock_unlock(&neigh->lock);
        return net_tx_action(skb,buf,skb->protocol);
    }
    else if(status == NUD_NONE)
    {
        neigh_queue_push_back(neigh,skb);
        pthread_rwlock_rdlock(&neigh->lock);
        uint8_t buf[IP_MAX_LEN];
        memcpy(buf,neigh->ip,IP_MAX_LEN);
        uint8_t try_cnt = neigh->req_cnt;
        pthread_rwlock_unlock(&neigh->lock);

        if(try_cnt >= MAX_REQUEST_TIMES)
        {
            neigh_update(neigh,NULL,NUD_FAILED);
            return 0;
        }

        return neigh->ops->request(buf);
    }
    else if(status == NUD_FAILED)
    {
        return 0; //should free skb on higher layer
    }

}
