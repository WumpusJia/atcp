#ifndef ATCP_NETDEVICE_H
#define ATCP_NETDEVICE_H

#define MAX_MAC_LEN 6

#define MTU 1500

#include "skbuff.h"
#include "common.h"


struct netdevice
{
    //now only support ethernet and ipv4
    uint8_t mac[MAX_MAC_LEN];
    uint32_t ip;

    uint32_t ifindex;
};

struct netdevice* netdev_get();
void netdev_init(char * s);


#endif
