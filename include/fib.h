#ifndef ATCP_FIB_H
#define ATCP_FIB_H

#include "common.h"
#include "flow.h"

#define FIB_HASH_MOD 233
#define FIB_HASH_BASE 57

struct fib_result
{
    uint8_t prefixlen;
    uint8_t type;
    uint8_t scope;
};


struct fib_node
{
    struct fib_node * next;
    uint32_t key;


};

struct fib_zone
{
    struct fib_zone* next;
    struct fib_node* buckets[FIB_HASH_MOD];

    uint8_t prefixlen;
    uint32_t mask;

};


struct fib_table
{
    uint8_t id;

//    void (*tb_select_default) ();

    struct fib_zone * zone[33];
    struct fib_zone * zone_list;

};


int fib_find(struct flowi * fl,struct fib_result * res);

#endif
