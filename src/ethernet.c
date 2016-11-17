#include "common.h"
#include "ethernet.h"

void eth_print(struct eth_header* hdr)
{
    puts("Print Ethernet Header:");
    printf("DST: ");
    for(int i = 0;i < ETH_LEN;++i)
        printf("%02x ",hdr->daddr[i]);
    putchar('\n');

    printf("SRC: ");
    for(int i = 0;i < ETH_LEN;++i)
        printf("%02x ",hdr->saddr[i]);
    putchar('\n');

    printf("Type :  %04x\n",hdr->type);

}


struct eth_header* init_eth_header(char * buf)
{
    struct eth_header* hdr = (struct eth_header *)buf;
    hdr->type = ntohs(hdr->type);
    return hdr;
}
