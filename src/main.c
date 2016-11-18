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



int main()
{
    main_init();

    net_rx_loop();

}
