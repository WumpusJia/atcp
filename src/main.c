
#include "common.h"
#include "util.h"
#include "ethernet.h"
#include "arp.h"
#include "netdevice.h"
#include "dev.h"




void main_init()
{

    netdev_init("eth0");

    net_init("eth0");

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


void test_icmp_request()
{
    char s[] = "10.0.0.3";
    uint32_t ip;
    inet_pton(AF_INET,s,&ip);
    ip = ntohl(ip);
    icmp_echo_request(netdev_get(),ip);
}




int main()
{

    main_init();

    //test_icmp_request();
    //test_arp_request();

    net_rx_loop();

}
