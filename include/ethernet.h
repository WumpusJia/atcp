#ifndef ATCP_ETHERNET_H
#define ATCP_ETHERNET_H

#include "common.h"


#define ETH_LEN 6

struct eth_header
{
    unsigned char daddr[ETH_LEN];
    unsigned char saddr[ETH_LEN];
    uint16_t type;
    unsigned char payload[];

};

void eth_print(struct eth_header* hdr);
struct eth_header* init_eth_header(char * buf);

#endif
