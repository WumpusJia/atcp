#ifndef ATCP_DST_H
#define ATCP_DST_H


#include "netdevice.h"

struct dst_entry
{
    struct netdevice *dev;

    struct neighbour * neighbour;
};



#endif
