#ifndef ATCP_DST_H
#define ATCP_DST_H


#include "netdevice.h"

struct dst_entry
{
    struct netdevice *dev;

    int (*input) (struct sk_buff *);
    int (*output) (struct sk_buff *);

    struct neighbour * neighbour;
};



#endif
