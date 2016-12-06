#ifndef ATCP_FLOW_H
#define ATCP_FLOW_H

#include "common.h"


enum rt_scope_t
{
     RT_SCOPE_UNIVERSE=0,
/* User defined values  */
     RT_SCOPE_SITE=200,
     RT_SCOPE_LINK=253,
     RT_SCOPE_HOST=254,
     RT_SCOPE_NOWHERE=255
};

enum
{
     RTN_UNSPEC,
     RTN_UNICAST,            /* Gateway or direct route      */
     RTN_LOCAL,              /* Accept locally               */
     RTN_BROADCAST,          /* Accept locally as broadcast,
                                send as broadcast */
     RTN_ANYCAST,            /* Accept locally as broadcast,
                                but send as unicast */
     RTN_MULTICAST,          /* Multicast route              */
     RTN_BLACKHOLE,          /* Drop                         */
     RTN_UNREACHABLE,        /* Destination is unreachable   */
     RTN_PROHIBIT,           /* Administratively prohibited  */
     RTN_THROW,              /* Not in this table            */
     RTN_NAT,                /* Translate this address       */
     RTN_XRESOLVE,           /* Use external resolver        */
     __RTN_MAX
};

struct flowi
{
    uint32_t oif;
    uint32_t iif;
    union {
        struct {
            uint32_t dstip;
            uint32_t srcip;
            uint8_t scope;
        }ip4;
    }un;
};


#define fl4_dst un.ip4.dstip
#define fl4_src un.ip4.srcip
#define fl4_scope un.ip4.scope

#endif
