#include "common.h"
#include "util.h"
#include "ethernet.h"
#include "arp.h"
#include "tuntap.h"







/*
What u need do first:
sudo ip tuntap add mode tun dev tun0
sudo ip addr add 10.0.0.0/24 dev tun0  # give it an ip
sudo ip link set dev tun0 up  # bring the if up
sudo ip route get 10.0.0.2  # check that packets to 10.0.0.x are going through tun0

ping 10.0.0.2  # leave this running in another shell to be able to see the effect of the next example



sudo ip tuntap add mode tap dev tap0
sudo ip link set dev tap0 up  # bring the if up
sudo ip route add dev tap0 10.0.0.0/24
*/


void frame_solve(struct eth_header* hdr)
{
    switch(hdr->type)
    {
        case ETH_P_ARP:
            arp_solve(hdr);
            break;
        default:
            puts("Unknown Frame");
            break;

    }
}


int main()
{
    int tun_fd;
    char dev[] = "tap0";
    tun_init(dev);

    char buf[1600];

    arp_cache_init();


    while(1)
    {
        int len = tun_read(buf,sizeof(buf));
        struct eth_header * hdr = init_eth_header(buf);
        eth_print(hdr);
        frame_solve(hdr);
        //hex_print(buf,len);
    }

}
