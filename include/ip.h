#ifndef ATCP_IP_H
#define ATCP_IP_H


#include "common.h"
#include "myendian.h"

#define IP_TCP 0x04
#define IP_ICMP 0x01

struct ip_header
{
    #if defined ATCP_LITTLE_ENDIAN
        uint8_t ihl:4;
        uint8_t version:4;
    #elif defined ATCP_LITTLE_ENDIAN
        uint8_t version:4;
        uint8_t ihl:4;
    #endif

        uint8_t tos;
        uint16_t totlen;
        uint16_t id;
        uint16_t frag;
        uint8_t ttl;
        uint8_t protocol;
        uint16_t checksum;
        uint32_t srcip;
        uint32_t dstip;
}__attribute__((packed));

static struct ip_header * get_ip_header(struct sk_buff* skb);

static void init_ip_header(struct ip_header* hdr);
static void reset_ip_header(struct ip_header * hdr);
void ip_solve(struct sk_buff* skb);

#endif
