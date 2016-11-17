#ifndef ATCP_ETHERNET_H
#define ATCP_ETHERNET_H

struct eth_header
{
    unsigned char daddr[6];
    unsigned char saddr[6];
    unsigned short type;
    char * payload;

};

void eth_print(struct eth_header* hdr);
struct eth_header* init_eth_header(char * buf);

#endif
