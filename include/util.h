#ifndef ATCP_UTIL_H
#define ATCP_UTIL_H

#include "common.h"

#define CMD_MAX_LEN 256
int my_cmd_run(char* format, ...);
void hex_print(char *s,int len);

uint16_t ip_header_checksum(void *s,int len);

#endif
