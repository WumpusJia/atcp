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

    arphdr->srcip = ntohl(arphdr->srcip);
    arphdr->dstip = ntohl(arphdr->dstip);

    //Suppose hard_type = 1 (ethernet)
    assert(arphdr->htype == HARD_TYPE_ETH);

    //Suppose prot_type = 0x8000 (ipv4);
    assert(arphdr->ptype == PROT_TYPE_IPV4);


    if(!update_arp_cache(arphdr))
    {
        puts("ARP Cache is Full");
    }

    switch(arphdr->op)
    {
        case OP_ARP_REQUEST:
            arp_reply();
    }


    //uint32_t num = arphdr->spaddr;
    //printf("SRC %u\n",num);
    //printf("SRC IP: %u,%u,%u,%u\n",(num/256/256/256)%256,(num/256/256)%256,(num/256)%256,num%256);

}

void arp_reply()
{
//todo

}


static struct arp_cache_node arp_cache[ARP_CACHE_SIZE];


void arp_cache_init()
{
    memset(arp_cache,0,sizeof(arp_cache));
}


int update_arp_cache(struct arp_header * hdr)
{
    for(int i = 0;i < ARP_CACHE_SIZE;++i)
    {
        struct arp_cache_node* now = &arp_cache[i];
        if(now->isused)
        {
            if(now->htype == hdr->htype && now->ip == hdr->srcip)
            {
                memcpy(now->mac,hdr->srcmac,ETH_LEN);
                return 1;
            }
        }
    }

    for(int i = 0;i < ARP_CACHE_SIZE;++i)
    {
        struct arp_cache_node* now = &arp_cache[i];
        if(!now->isused)
        {
            now->isused = 1;
            now->htype = hdr->htype;
            now->ip = hdr->srcip;
            memcpy(now->mac,hdr->srcmac,sizeof(hdr->srcmac));
            return 1;
        }
    }
    return 0;
}
