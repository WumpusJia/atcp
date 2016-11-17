#ifndef ATCP_NETDEVICE_H
#define ATCP_NETDEVICE_H

#include "ethernet.h"

struct netdevice
{
    unsigned char mac[ETH_LEN];
    uint32_t ip;
};

#endif
