#include "arp.h"
#include "common.h"


void arp_solve(struct eth_header* hdr)
{
    puts("SOLVE : ARP");

    struct arp_header * arphdr;
    arphdr = (struct arp_header *) hdr->payload;

    arphdr->htype = ntohs(arphdr->htype);
    arphdr->ptype = ntohs(arphdr->ptype);
    arphdr->op = ntohs(arphdr->op);

    arphdr->spaddr = ntohl(arphdr->spaddr);
    arphdr->dpaddr = ntohl(arphdr->dpaddr);

    //Suppose hard_type = 1 (ethernet)
    assert(arphdr->htype == HARD_TYPE_ETH);

    //Suppose prot_type = 0x8000 (ipv4);
    assert(arphdr->ptype == PROT_TYPE_IPV4);


    uint32_t num = arphdr->spaddr;
    printf("SRC %u\n",num);
    //printf("SRC IP: %u,%u,%u,%u\n",(num/256/256/256)%256,(num/256/256)%256,(num/256)%256,num%256);

}
