#include "inetpeer.h"

static struct inet_peer inet_peer_cache;
void rt_bind_peer(struct rtable* rt)
{
    rt -> peer = &inet_peer_cache;
}


uint16_t inet_getid(struct inet_peer * p)
{
    return p->ip_id_count++;
}
