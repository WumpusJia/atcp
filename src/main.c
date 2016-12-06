
#include "common.h"
#include "util.h"
#include "ethernet.h"
#include "arp.h"
#include "netdevice.h"
#include "dev.h"
#include "route.h"



void main_init()
{

    netdev_init("eth0");

    net_init("eth0");

    ip_rt_init();
    arp_init();
}



// void test_arp_request()
// {
//     char s[] = "192.168.1.80";
//     uint32_t ip;
//     inet_pton(AF_INET,s,&ip);
//     ip = ntohl(ip);
//     arp_request(netdev_get(),ip);
//
// }

//
void test_icmp_request()
{
    char s[] = "192.168.1.80";
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
