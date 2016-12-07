#include "arp.h"
#include "common.h"
#include "neighbour.h"
#include "route.h"





#define ARP_HASH_BASE 57
#define ARP_HASH_MOD 233
#define ARP_HASH_MAX_SIZE 100

uint32_t arp_hash(void *key)
{
    uint32_t res = 0;
    uint8_t *p = key;
    for(int i = 0;i < 4;++i)
    {
        res = (res*ARP_HASH_BASE+(*p))%ARP_HASH_MOD;
        p++;
    }
    return res;
}


int arp_resolve_output(struct sk_buff * skb)
{
    skb->protocol = ETH_P_IP;
    return neigh_event_send(skb->dst->neighbour,skb);

}

static struct neigh_ops arp_generic_ops = {
    output: arp_resolve_output,
    request : arp_request,
};

int arp_constructor(struct neighbour* n)
{
    n->ops = &arp_generic_ops;
}




static struct neigh_table arp_tbl = {
    key_len : 4,
    hash: arp_hash,
    constructor : arp_constructor,
    hash_size : ARP_HASH_MOD,
    hash_max_size : ARP_HASH_MAX_SIZE,

};




void arp_init()
{
    neigh_table_init(&arp_tbl);
}




int arp_bind_neighbour(struct dst_entry * dst)
{

    struct neighbour* n = dst->neighbour;

    if(n == NULL)
    {

        uint32_t dstip = ((struct rtable*)dst)->rt_dst;
        n = neigh_lookup(&arp_tbl,&dstip);
        if(n == NULL) return 0;
        dst->neighbour = n;
    }
    return 1;

}









/////////////////////////////////////
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


void arp_rcv(struct sk_buff* skb)
{
    puts("SOLVE : ARP");

    struct arp_header * hdr = get_arp_header(skb);
    init_arp_header(hdr);




    //Suppose hard_type = 1 (ethernet)
    assert(hdr->htype == HARD_TYPE_ETH);

    //Suppose prot_type = 0x8000 (ipv4);
    assert(hdr->ptype == PROT_TYPE_IPV4);


    neigh_event_rcv(&arp_tbl,hdr->srcmac,&hdr->srcip);

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

static uint8_t BROADCAST_MAC[] = {0xff,0xff,0xff,0xff,0xff,0xff};

int arp_request(void *ip)
{
    uint32_t reqip = * ((uint32_t *)ip);

    struct  sk_buff* skb = alloc_skb(ETH_HEADER_LEN+ARP_HEADER_LEN);
    skb_reserve(skb,ETH_HEADER_LEN);
    skb_put(skb,ARP_HEADER_LEN);

    skb->dev = netdev_get();


    struct arp_header* hdr = (struct arp_header *)skb->data;

    hdr->htype = HARD_TYPE_ETH;
    hdr->ptype = PROT_TYPE_IPV4;


    hdr->hsize = ETH_MAC_LEN;
    hdr->psize = 4; //todo: repalce with ipv4 const variable

    hdr->op = OP_ARP_REQUEST;

    hdr->srcip = skb->dev->ip;
    memcpy(hdr->srcmac,skb->dev->mac,sizeof(uint8_t)*ETH_MAC_LEN);

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
        return 0;
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



neigh_resolve_output() ->  neigh_event_sent()

*/
