#include "icmp.h"
#include "util.h"
#include "ip.h"

struct icmp_header * get_icmp_header(struct sk_buff * skb)
{
    return (struct icmp_header * )skb->data;
}
void init_icmp_header(struct icmp_header* hdr)
{
    switch (hdr->type)
    {
        case ICMP_ECHO:
        case ICMP_ECHO_REPLY:
            hdr->un.echo.id = ntohs(hdr->un.echo.id);
            hdr->un.echo.seq = ntohs(hdr->un.echo.seq);
            break;
        default:
            break;
    }
}
void reset_icmp_header(struct icmp_header * hdr)
{
    switch (hdr->type)
    {
        case ICMP_ECHO:
        case ICMP_ECHO_REPLY:
            hdr->un.echo.id = htons(hdr->un.echo.id);
            hdr->un.echo.seq = htons(hdr->un.echo.seq);
            break;
        default:
            break;
    }
}


void icmp_solve(struct sk_buff * skb)
{
    puts("SOLVE icmp");
    struct icmp_header * hdr = get_icmp_header(skb);

    if(checksum(hdr,skb->len/2))
    {
        puts("ICMP Checksum failed!");
        return 0;
    }

    init_icmp_header(hdr);

    switch(hdr->type)
    {
        case ICMP_ECHO:
            icmp_echo_reply(skb);
            break;
        default:
            puts("Unknown ICMP TYPE");
            break;
    }
}

static uint16_t _icmp_sent_seq_;

int icmp_echo_reply(struct sk_buff * skb)
{
    puts("Send ICMP reply");
    struct icmp_header * hdr = get_icmp_header(skb);

    hdr->type = ICMP_ECHO_REPLY;
    hdr->code = 0;
    hdr->checksum = 0;

    hdr->un.echo.seq = _icmp_sent_seq_++;

    reset_icmp_header(hdr);
    hdr->checksum = checksum(hdr,skb->len/2);
    hdr->checksum = htons(hdr->checksum);

    skb->protocol = IP_ICMP;

    return ip_send(skb);

}
