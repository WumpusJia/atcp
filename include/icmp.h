#ifndef ATCP_ICMP_H
#define ATCP_ICMP_H

#include "common.h"
#include "skbuff.h"

#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY 0


#define ICMP_ECHO_LEN 64

struct icmp_header
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    union
    {
        struct
        {
            uint16_t id;
            uint16_t seq;
            uint8_t data[];
        }echo;
    }un;
}__attribute__((packed));




struct icmp_header * get_icmp_header(struct sk_buff * skb);
void init_icmp_header(struct icmp_header* hdr);
void reset_icmp_header(struct icmp_header * hdr);
void icmp_solve(struct sk_buff * skb);
int icmp_echo_reply(struct sk_buff * skb);
int icmp_echo_request(struct netdevice* dev,uint32_t dstip);
#endif
