
#include "common.h"
#include "util.h"
#include "ethernet.h"
#include "arp.h"
#include "netdevice.h"
#include "dev.h"
#include "route.h"
#include "icmp.h"

pthread_t rx_id;


void exit_signal_handler(int signo)
{
    if(signo == SIGINT || signo == SIGTERM)
    {
        pthread_cancel(rx_id);
        puts("Exiting rx thread...");
    }
}

void exit_signal_init()
{

    if (signal(SIGINT, exit_signal_handler) == SIG_ERR)
        printf("ERROR: Can't catch SIGINT\n");
    if (signal(SIGTERM, exit_signal_handler) == SIG_ERR)
        printf("ERROR: Can't catch SIGTERM\n");
}

void main_init()
{
    exit_signal_init();
    netdev_init("eth0");

    net_init("eth0");

    ip_rt_init();
    arp_init();
}

void main_free()
{
    puts("Cleaning...");
    net_free();

    ip_rt_free();
    arp_free();
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


void *rx_loop_wrapper(void* args)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    net_rx_loop();
}

// void *tx_loop_wrapper(void* args)
// {
//     int cnt = 0;
//     int BASE = 100000000;
//     while(1)
//     {
//         while(cnt < BASE)
//             cnt++;
//         cnt = 0;
//         puts("Send");
//     }
// }

// void *consume_rx_pkt_loop_wrapper(void* args)
// {
//     consume_rx_pkt_loop();
// }


int main()
{

    main_init();

    //test_icmp_request();
    //test_arp_request();

    //net_rx_loop();


    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    if(pthread_create(&rx_id,&attr,rx_loop_wrapper,NULL))
    {
        printf("ERROR: Create rx loop thread");
    }
    // pthread_t tx_id;
    // if(pthread_create(&tx_id,NULL,tx_loop_wrapper,NULL))
    // {
    //     printf("ERROR: Create tx loop thread");
    // }

    // int crx_thread = 1;
    // pthread_t crx_id[2];
    // for(int i = 0;i < crx_thread;++i)
    // {
    //     if(pthread_create(&crx_id[i],NULL,consume_rx_pkt_loop_wrapper,NULL))
    //     {
    //         printf("ERROR: Create comsume rx loop thread");
    //     }
    // }

    pthread_attr_destroy(&attr);
    if(pthread_join(rx_id,NULL))
    {
        printf("ERROR: return code from tx loop thread\n");
    }
    main_free();
    pthread_exit(NULL);
}
