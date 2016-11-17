#include "arp.h"
#include "common.h"


void arp_solve(struct eth_header* hdr)
{
    puts("SOLVE : ARP");

    struct arp_header * arphdr;
    arphdr = (struct arp_header *) hdr->payload;

    arphdr->hard_type = ntohs(arphdr->hard_type);
    arphdr->prot_type = ntohs(arphdr->prot_type);
    arphdr->op = ntohs(arphdr->op);

    //Suppose hard_type = 1 (ethernet)
    assert(arphdr->hard_type == HARD_TYPE_ETH);

    //Suppose prot_type = 0x8000 (ipv4);
    assert(arphdr->prot_type == PROT_TYPE_IPV4);

    struct arp_ipv4 * arpipv4;
    arpipv4 = (struct arp_ipv4*) arphdr->payload;

    printf("OP : %04x\n",arphdr->op);

}
