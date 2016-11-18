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
    uint16_t htype;
    uint16_t ptype;
    uint8_t hsize;
    uint8_t psize;

    uint16_t op;

    uint8_t srcmac[ETH_MAC_LEN];
    uint32_t srcip;
    //unsigned char src_prot_addr[4];
    uint8_t dstmac[ETH_MAC_LEN];
    //unsigned char dst_prot_addr[4];
    uint32_t dstip;
}__attribute__((packed));



#define ARP_CACHE_SIZE 20

struct arp_cache_node
{
    uint32_t isused;
    uint16_t htype;
    uint32_t ip;
    uint8_t mac[ETH_MAC_LEN];
};

void arp_reply();

void arp_solve(struct eth_header* hdr);
void arp_cache_init();
int update_arp_cache(struct arp_header * hdr);
#endif
