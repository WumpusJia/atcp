#include "common.h"
#include "util.h"
#include "ethernet.h"


int tun_alloc(char* dev)
{
    struct ifreq ifr;
    int fd,err;

    if( (fd = open("/dev/net/tun",O_RDWR)) < 0)
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



struct T
{
    short val;
};

int main()
{
    int tun_fd;
    char dev[] = "tap0";
    tun_fd = tun_alloc(dev);

    char buf[1600];

    while(1)
    {
        int len = read(tun_fd,buf,sizeof(buf));
        struct eth_header * hdr = init_eth_header(buf);
        eth_print(hdr);
        //hex_print(buf,len);
    }

}
