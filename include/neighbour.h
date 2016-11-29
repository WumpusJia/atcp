#ifndef ATCP_NEIGHBOUR_H
#define ATCP_NEIGHBOUR_H

#include "skbuff.h"
#include "ethernet.h"
#include "defined.h"


#define NUD_INCOMPLETE  0x01
#define NUD_REACHABLE   0x02
#define NUD_STALE       0x04
#define NUD_DELAY       0x08
#define NUD_PROBE       0x10
#define NUD_FAILED      0x20
/* Dummy states */
#define NUD_NOARP       0x40
#define NUD_PERMANENT   0x80
#define NUD_NONE        0x00


#define IP_MAX_LEN 16

struct neighbour
{

    struct neigh_table *tbl;
    uint8_t nud_state;

    uint8_t ip[IP_MAX_LEN];
    uint8_t mac[ETH_MAC_LEN];

    //timer

    struct neighbour * next;
    struct neighbour * prev;

    //sk_buff queue
    struct neigh_ops *ops;
    pthread_rwlock_t lock;
};


struct neigh_ops
{
    int (*output) (struct sk_buff*);
};

struct neigh_table
{
    int key_len;
    uint32_t (*hash) (void *);
    int (*constructor) (struct neighbour* );

    struct neighbour ** hash_buckets;
    uint32_t hash_size;
    uint32_t hash_max_size;

    uint32_t now_size;
    pthread_rwlock_t lock;

};

void neigh_table_init(struct neigh_table * tbl);

int neigh_output(struct sk_buff * skb);


struct neighbour* neigh_lookup(struct neigh_table * tbl,const void* key);
int neigh_event_rcv(struct neigh_table * tbl,uint8_t * mac,void* ip);
int neigh_event_send(struct neighbour * neigh,struct sk_buff * skb);
#endif
