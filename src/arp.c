#include "arp.h"
#include "common.h"

//http://backreference.org/2010/03/26/tuntap-interface-tutorial/  this article introduce TUN/TAP


static struct arp_header * get_arp_header(struct sk_buff * skb)
{
    return (struct arp_header *)skb->data;
}

static void init_arp_header(struct arp_header * hdr)
{

    hdr->htype = ntohs(hdr->htype);
    hdr->ptype = ntohs(hdr->ptype);
    hdr->op = ntohs(hdr->op);

    hdr->srcip = ntohl(hdr->srcip);
    hdr->dstip = ntohl(hdr->dstip);

}


static void reset_arp_header(struct arp_header* hdr)
{
    hdr->htype = htons(hdr->htype);
    hdr->ptype = htons(hdr->ptype);
    hdr->op = htons(hdr->op);

    hdr->srcip = htonl(hdr->srcip);
    hdr->dstip = htonl(hdr->dstip);
}


void arp_solve(struct sk_buff* skb)
{
    puts("SOLVE : ARP");

    struct arp_header * hdr = get_arp_header(skb);
    init_arp_header(hdr);


    //Suppose hard_type = 1 (ethernet)
    assert(hdr->htype == HARD_TYPE_ETH);

    //Suppose prot_type = 0x8000 (ipv4);
    assert(hdr->ptype == PROT_TYPE_IPV4);


    if(!update_arp_cache(hdr))
    {
        puts("ARP Cache is Full");
    }

    switch(hdr->op)
    {
        case OP_ARP_REQUEST:
            arp_reply(skb);
            break;
        case OP_ARP_REPLY:
            puts("RECEIVE ARP  REPLY");
            break;
        default:
            puts("Unknown ARP");
            break;

    }



}



int arp_request(struct netdevice* dev,uint32_t reqip)
{

    struct  sk_buff* skb = alloc_skb(ETH_HEADER_LEN+ARP_HEADER_LEN);
    skb_reserve(skb,ETH_HEADER_LEN);
    skb_put(skb,ARP_HEADER_LEN);

    skb->dev = dev;


    struct arp_header* hdr = (struct arp_header *)skb->data;

    hdr->htype = HARD_TYPE_ETH;
    hdr->ptype = PROT_TYPE_IPV4;


    hdr->hsize = ETH_MAC_LEN;
    hdr->psize = 4; //todo: repalce with ipv4 const variable

    hdr->op = OP_ARP_REQUEST;

    hdr->srcip = dev->ip;
    memcpy(hdr->srcmac,dev->mac,sizeof(uint8_t)*ETH_MAC_LEN);

    hdr->dstip = reqip;
    memcpy(hdr->dstmac,BROADCAST_MAC,sizeof(uint8_t)*ETH_MAC_LEN);


    reset_arp_header(hdr);

    return net_tx_action(skb,BROADCAST_MAC,ETH_P_ARP);

}

int arp_reply(struct sk_buff* skb)
{
    puts("ARP: REPLY");
    struct netdevice * dev = skb->dev;
    struct arp_header* hdr = get_arp_header(skb);

    if(dev->ip != hdr->dstip)
    {
        puts("Not come for me~");
        return;
    }


    hdr->dstip = hdr->srcip;
    memcpy(hdr->dstmac,hdr->srcmac,sizeof(uint8_t)*ETH_MAC_LEN);




    hdr->srcip = dev->ip;

    memcpy(hdr->srcmac,dev->mac,sizeof(uint8_t)*ETH_MAC_LEN);


    hdr->htype = HARD_TYPE_ETH;
    hdr->ptype = PROT_TYPE_IPV4;


    hdr->hsize = ETH_MAC_LEN;
    hdr->psize = 4; //todo: repalce with ipv4 const variable

    hdr->op = OP_ARP_REPLY;

    reset_arp_header(hdr);

    return net_tx_action(skb,hdr->dstmac,ETH_P_ARP);

}


/*
This implement is simple and slow.
Todo:
    Use data structure like: hash_table with linked list
    Support O(1) find and update

    There also need timeout mechanism , function like : double saving space when space is full


*/
static struct arp_cache_node arp_cache[ARP_CACHE_SIZE];


void arp_cache_init()
{
    memset(arp_cache,0,sizeof(arp_cache));
}


static int update_arp_cache(struct arp_header * hdr)
{
    for(int i = 0;i < ARP_CACHE_SIZE;++i)
    {
        struct arp_cache_node* now = &arp_cache[i];
        if(now->isused)
        {
            if(now->htype == hdr->htype && now->ip == hdr->srcip)
            {
                memcpy(now->mac,hdr->srcmac,ETH_MAC_LEN);
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


uint8_t * query_arp_cache(uint32_t ip)
{
    for(int i = 0;i < ARP_CACHE_SIZE;++i)
    {
        struct arp_cache_node* now = &arp_cache[i];
        if(now->isused)
        {
            if(now->ip == ip)
            {
                return now->ip;
            }
        }
    }
    return NULL;
}
