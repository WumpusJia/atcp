#ifndef ATCP_ENDIAN_H
#define ATCP_ENDIAN_H



#if __BYTE_ORDER == __LITTLE_ENDIAN
#define ATCP_LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#define ATCP_LITTLE_ENDIAN
#else
#error "Unsupport Endian"
#endif


#endif
