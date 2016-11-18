#ifndef ATCP_ETHERNET_H
#define ATCP_ETHERNET_H

#include "common.h"


#define ETH_LEN 6

struct eth_header
{
    uint8_t daddr[ETH_LEN];
    uint8_t saddr[ETH_LEN];
    uint16_t type;
    uint8_t payload[];

}__attribute__((packed));;

void eth_print(struct eth_header* hdr);
struct eth_header* init_eth_header(char * buf);

#endif
