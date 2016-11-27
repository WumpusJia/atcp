#include "neighbour.h"
#include "ip.h"
#include "arp.h"
#include "dev.h"

//Find the dstmac of dstip and send it
//stupid implementation XD.
int neigh_output(struct sk_buff * skb)
{
    struct ip_header * hdr = get_ip_header(skb);
    struct netdevice * dev = skb->dev;
    uint32_t dstip = ntohl(hdr->dstip);

    uint8_t * dstmac = NULL;


    int max_query_cnt = 3;




    // while(max_query_cnt && (dstmac = query_arp_cache(dstip)) == NULL)  //Here need multithread
    // {
    //     arp_request(dev,dstip);
    //     max_query_cnt--;
    //     //sleep(2);
    // }
    //printf("Dst IP: %d,%d,%d,%d\n",(dstip/256/256/256)%256,(dstip/256/256)%256,(dstip/256)%256,dstip%256);

    /*
    Unsupport !

    */
    // dstmac = query_arp_cache(dstip);
    // if(dstmac == NULL)
    // {
    //     arp_request(dev,dstip);
    //
    //     while(1)
    //     {
    //
    //         struct sk_buff * skb = alloc_skb(MTU);
    //         uint32_t len = tun_read(skb->data,MTU);
    //         skb_put(skb,len);
    //         net_rx_action(skb);
    //
    //         if((dstmac = query_arp_cache(dstip)) != NULL)
    //             break;
    //     }
    // }

    if(dstmac == NULL)
    {
        puts("Error: unable to find mac of ip");
        return -1;
    }
    else
    {
         return net_tx_action(skb,dstmac,ETH_P_IP);
    }


}
