#ifndef ATCP_TUNTAP_H
#define ATCP_TUNTAP_H

#include "common.h"

static int tunfd;



void tun_init();
int tun_read(char* buf,int len);
int tun_write(char* buf,int len);

#endif
