#ifndef ATCP_UTIL_H
#define ATCP_UTIL_H

#include "common.h"

#define CMD_MAX_LEN 256
int my_cmd_run(char* format, ...);
void ipv4_print(uint32_t addr);
uint16_t checksum(void *s,int len);

#endif
