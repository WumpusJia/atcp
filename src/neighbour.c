#include "neighbour.h"
#include "ip.h"
#include "arp.h"
#include "netdevice.h"

//Find the dstmac of dstip and send it
//stupid implementation XD.
int neigh_output(struct sk_buff * skb)
{
    struct ip_header * hdr = get_ip_header(skb);
    struct netdevice * dev = skb->dev;
    uint32_t dstip = ntohl(hdr->dstip);

    uint8_t * dstmac = NULL;


    int max_query_cnt = 3;

     printf("Dst IP: %d,%d,%d,%d\n",(dstip/256/256/256)%256,(dstip/256/256)%256,(dstip/256)%256,dstip%256);


    while(max_query_cnt && (dstmac = query_arp_cache(dstip)) == NULL)  //Here need multithread
    {
        arp_request(dev,dstip);
        max_query_cnt--;
        //sleep(2);
    }

    if(dstmac == NULL)
    {
        puts("Error: unable to find mac of ip");
        return;
    }
    else
    {
        return net_tx_action(skb,dstmac,ETH_P_IP);
    }



}
