/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-gtp.h"

int ogs_gtpu_header_len(ogs_pkbuf_t *pkbuf)
{
    ogs_gtp_header_t *gtp_h = NULL;
    uint8_t *ext_h = NULL;
    uint16_t len = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);

    gtp_h = (ogs_gtp_header_t *)pkbuf->data;

    len = OGS_GTPV1U_HEADER_LEN;
    if (pkbuf->len < len) return -1;

    if (gtp_h->flags & OGS_GTPU_FLAGS_E) {

        len += OGS_GTPV1U_EXTENSION_HEADER_LEN;
        if (pkbuf->len < len) return -1;

        /*
         * TS29.281
         * 5.2.1 General format of the GTP-U Extension Header
         *
         * If no such Header follows,
         * then the value of the Next Extension Header Type shall be 0. */
        while (*(ext_h = (((uint8_t *)gtp_h) + len - 1))) {
        /*
         * The length of the Extension header shall be defined
         * in a variable length of 4 octets, i.e. m+1 = n*4 octets,
         * where n is a positive integer.
         */
            len += (*(++ext_h)) * 4;
            if (pkbuf->len < len) return -1;
        }

    } else if (gtp_h->flags & (OGS_GTPU_FLAGS_S|OGS_GTPU_FLAGS_PN)) {
        /*
         * If and only if one or more of these three flags are set,
         * the fields Sequence Number, N-PDU and Extension Header
         * shall be present. The sender shall set all the bits of
         * the unused fields to zero. The receiver shall not evaluate
         * the unused fields.
         * For example, if only the E flag is set to 1, then
         * the N-PDU Number and Sequence Number fields shall also be present,
         * but will not have meaningful values and shall not be evaluated.
         */
        len += 4;
    }

    if (pkbuf->len < len) return -1;

    return len;
}


uint16_t ogs_in_cksum(uint16_t *addr, int len)
{
    int nleft = len;
    uint32_t sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;

    // Adding 16 bits sequentially in sum
    while (nleft > 1) {
        sum += *w;
        nleft -= 2;
        w++;
    }

    // If an odd byte is left
    if (nleft == 1) {
        *(uint8_t *) (&answer) = *(uint8_t *) w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}
