#include "fib.h"


static struct fib_table * ip_fib_table;


uint32_t fib_hash(uint32_t addr)
{
    uint32_t res = 0;
    uint8_t *p = (uint8_t *)&addr;
    for(int i = 0;i < 4;++i)
    {
        res = (res*FIB_HASH_BASE+(*p))%FIB_HASH_MOD;
        p++;
    }
    return res;
}


// int fib_find(struct fib_table* tb,struct flowi* fl,struct fib_result * res)
// {
//     struct fib_zone * p;
//     for(p = tb->zone_list; p; p = p->next)
//     {
//         uint32_t now = fl->fl4_dst & p->mask;
//
//         struct fib_node* queue = p->buckets[fib_hash(now)];
//         for(queue;queue; queue = queue->next)
//         {
//             if(queue->key == now)
//             {
//                 //change res
//                 return 1;
//             }
//         }
//
//     }
//     return 0;
// }



int fib_find(struct flowi * fl,struct fib_result * res) //mock function
{
    uint32_t my = 192*256*256*256+168*256*256+1*256+2; //tmp here
    if(fl->fl4_dst == my)
    {
        res->type = RTN_LOCAL;
    }
    else
    {
        res->type = RTN_UNICAST;
    }
    return 1;
}



struct fib_table * fib_hash_init(int id)
{
    struct fib_table* tb;
    tb = malloc(sizeof(struct fib_table));

    tb->id = id;
    memset(tb->zone,0,sizeof(tb->zone));
    tb->zone_list = NULL;

}


void ip_fib_init()
{
    #define RT_TABLE_MAIN 0
    ip_fib_table = fib_hash_init(RT_TABLE_MAIN);
}
