#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
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
