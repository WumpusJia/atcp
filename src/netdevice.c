#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include "netdevice.h"




static struct netdevice _netdev_;

struct netdevice* netdev_get()
{
    return &_netdev_;
}

void netdev_init(char * s)
{
    struct netdevice* dev = &_netdev_;
    struct ifreq buf;
    int fd = socket(PF_INET,SOCK_STREAM,0);
    memset(&buf,0,sizeof(buf));

    strcpy(buf.ifr_name, s);
    ioctl(fd, SIOCGIFHWADDR, &buf);

    memcpy(dev->mac,buf.ifr_hwaddr.sa_data,6*sizeof(uint8_t));

    ioctl(fd,SIOCGIFADDR,&buf);

    dev->ip = ntohl( ((struct sockaddr_in*)&(buf.ifr_addr))->sin_addr.s_addr);

    dev->ifindex = 1;

    close(fd);


}
