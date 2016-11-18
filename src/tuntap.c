#include "tuntap.h"
#include "util.h"

static int tun_alloc(char* dev)
{
    struct ifreq ifr;
    int fd,err;

    if( (fd = open("/dev/net/tap",O_RDWR)) < 0)
    {
        printf("ERROR: Could not open TUN/TAP dev\n");
        exit(1);
    }


    memset(&ifr,0,sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
         *        IFF_TAP   - TAP device
         *
         *        IFF_NO_PI - Do not provide packet information
         */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if( *dev )
    {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 )
    {
        printf("ERROR: Could not ioctl %s",strerror(errno));
        close(fd);
        return err;
    }
    strcpy(dev, ifr.ifr_name);
    return fd;
}

/*
$ mknod /dev/net/tap c 10 200

sudo ip tuntap add mode tap dev tap0
sudo ip link set dev tap0 up  # bring the if up
sudo ip route add dev tap0 10.0.0.0/24
sudo ip address add dev tap0 local 10.0.0.3/24
*/






void tun_init()
{
    char dev[] = "tap0";
    tunfd = tun_alloc(dev);
    //puts(dev);

}



int tun_read(char* buf,int len)
{
    return read(tunfd,buf,len);
}
int tun_write(char* buf,int len)
{
    return write(tunfd,buf,len);
}
