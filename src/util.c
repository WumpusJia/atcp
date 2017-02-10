#include "common.h"
#include "util.h"


int my_cmd_run(char* format, ...)
{
    char buf[CMD_MAX_LEN];
    va_list ap;
    va_start(ap,format);
    vsnprintf(buf,CMD_MAX_LEN,format,ap);
    va_end(ap);

    printf("RUN: %s\n",buf);
    return system(buf);
}

void hex_print(char *s,int len)
{
    puts("Hex Print:");
    for(int i = 0;i < len;++i)
    {
        printf("%02x%c",(unsigned char)s[i]," \n"[(i && (i % 8 == 0)) || i == len-1]);
    }
}


void ipv4_print(uint32_t addr)
{
    int tmp[4];
    for(int i = 0;i < 4;++i)
    {
        tmp[i] = addr % 256;
        addr /= 256;
    }
    for(int i = 3;i >= 0;--i)
        printf("%d%c",tmp[i]," \n"[i == 0]);
}



uint16_t checksum(void *s,int len)
{

    uint32_t sum = 0;
    uint16_t *p = s;
    //puts("CHECK: ");
    for(int i = 0;i < len;++i)
    {
    //    printf("%04x\n",*p);
        sum += ntohs(*p);
        p++;
    }

    while(sum >> 16)
    {
        sum = (sum >> 16) + (sum & 0xffff);
    }
    return ~sum;
}
