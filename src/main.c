#include "common.h"
#include "util.h"
#include "ethernet.h"
#include "arp.h"
#include "tuntap.h"
#include "netdevice.h"




void main_init()
{

    tun_init();

    netdev_init("10.0.0.2","52:27:1e:b1:f6:60");
    arp_cache_init();
}



void test_arp_request()
{
    char s[] = "10.0.0.3";
    uint32_t ip;
    inet_pton(AF_INET,s,&ip);
    ip = ntohl(ip);
    arp_request(netdev_get(),ip);
}


int main()
{
    main_init();

    test_arp_request();

    net_rx_loop();

}
