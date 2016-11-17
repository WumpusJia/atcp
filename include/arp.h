#ifndef ATCP_ARP_H
#define ATCP_ARP_H

#include "ethernet.h"


#define HARD_TYPE_ETH 0x0001
#define PROT_TYPE_IPV4 0x0800

#define OP_ARP_REQUEST 0x0001
#define OP_ARP_REPLY 0x0002
#define OP_RARP_REQUEST 0x0003
#define OP_RARP_REPLY 0x0004

struct arp_header
{
    uint16_t hard_type;
    uint16_t prot_type;
    unsigned char hard_size;
    unsigned char prot_size;

    uint16_t op;

    unsigned char payload[];

};


struct arp_ipv4
{
    unsigned char src_hard_addr[6];
    unsigned char src_prot_addr[4];
    unsigned char dst_hard_addr[6];
    unsigned char dst_prot_addr[4];
};


void arp_solve(struct eth_header* hdr);

#endif
