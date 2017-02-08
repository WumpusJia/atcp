/*
 * Copyright (C) 2011-2016 Universita` di Pisa
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _NET_NETMAP_MY_H_
#define _NET_NETMAP_MY_H_

#include "net/netmap_user.h"

#define NIC_RING 0
#define HOST_RING 1

static int
my_inject(struct nm_desc *d, const void *buf, size_t size,u_int c)
{
		/* compute current ring to use */
  	struct netmap_ring *ring;
  	uint32_t i, idx;
  	uint32_t ri = d->cur_tx_ring + c;

  	if (ri > d->last_tx_ring)
  		ri = d->first_tx_ring;
  	ring = NETMAP_TXRING(d->nifp, ri);
  	if (nm_ring_empty(ring)) {
  	   return 0;
  	}
  	i = ring->cur;
  	idx = ring->slot[i].buf_idx;
  	ring->slot[i].len = size;
  	nm_pkt_copy(buf, NETMAP_BUF(ring, idx), size);
  	d->cur_tx_ring = ri;
  	ring->head = ring->cur = nm_ring_next(ring, i);
  	return size;

	  return 0; /* fail */
}

static int
nic_inject(struct nm_desc *d, const void *buf, size_t size)
{
    return my_inject(d,buf,size,NIC_RING);
}

static int
host_inject(struct nm_desc *d, const void *buf, size_t size)
{
    return my_inject(d,buf,size,HOST_RING);
}



static u_char *
my_nextpkt(struct nm_desc *d, struct nm_pkthdr *hdr,int * ringid)
{
	int ri = d->cur_rx_ring;

	do {
		/* compute current ring to use */
		struct netmap_ring *ring = NETMAP_RXRING(d->nifp, ri);
		if (!nm_ring_empty(ring)) {

			u_int i = ring->cur;
			u_int idx = ring->slot[i].buf_idx;
			u_char *buf = (u_char *)NETMAP_BUF(ring, idx);

			// __builtin_prefetch(buf);
			*ringid = ri;
			hdr->ts = ring->ts;
			hdr->len = hdr->caplen = ring->slot[i].len;
			ring->cur = nm_ring_next(ring, i);
			/* we could postpone advancing head if we want
			 * to hold the buffer. This can be supported in
			 * the future.
			 */
			ring->head = ring->cur;
			d->cur_rx_ring = ri;
			return buf;
		}
		ri++;
		if (ri > d->last_rx_ring)
			ri = d->first_rx_ring;
	} while (ri != d->cur_rx_ring);
	return NULL; /* nothing found */
}


#endif
