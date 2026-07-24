/*
 * Copyright (C) 2026 by DNL
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

#ifndef OGS_CDR_BER_H
#define OGS_CDR_BER_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>

/* TS 32.298 TimeStamp (9 octets), implemented in pgw-record.c */
void ogs_cdr_make_timestamp(uint8_t out[9], time_t t, int tz_offset_min);

/*
 * Minimal BER emitter, definite length form only.
 * TLVs are written back-to-front: value first, then length, then tag,
 * so constructed lengths need no second pass.
 */

typedef struct ogs_ber_s {
    uint8_t *buf;       /* buffer start */
    size_t size;
    size_t pos;         /* write position; decrements towards 0 */
    int error;
} ogs_ber_t;

#define OGS_BER_CLASS_UNIVERSAL     0x00
#define OGS_BER_CLASS_CONTEXT       0x80
#define OGS_BER_CONSTRUCTED         0x20

void ogs_ber_init(ogs_ber_t *ber, uint8_t *buf, size_t size);
/* bytes used so far */
size_t ogs_ber_len(const ogs_ber_t *ber);
/* final result: moves encoding to buffer start, returns length or -1 */
int ogs_ber_finish(ogs_ber_t *ber);

void ogs_ber_prepend(ogs_ber_t *ber, const uint8_t *data, size_t len);
/* length octets for `len`, then tag with class|constructed and number
 * (high-tag-number form when number >= 31) */
void ogs_ber_prepend_header(ogs_ber_t *ber,
        uint8_t cls, int constructed, uint32_t tag_number, size_t len);

/* primitive context-tagged leafs; each records its own total length */
void ogs_ber_ctx_octets(ogs_ber_t *ber,
        uint32_t tag, const uint8_t *data, size_t len);
void ogs_ber_ctx_uint(ogs_ber_t *ber, uint32_t tag, uint64_t value);
void ogs_ber_ctx_bool(ogs_ber_t *ber, uint32_t tag, int value);
void ogs_ber_ctx_null(ogs_ber_t *ber, uint32_t tag);
/* BIT STRING with exactly one named bit set */
void ogs_ber_ctx_single_bit(ogs_ber_t *ber, uint32_t tag, unsigned int bit);

/*
 * Constructed wrapper: encode children first (they sit at ber->pos),
 * then call with the saved position to wrap everything written since.
 *   size_t mark = ogs_ber_mark(ber);
 *   ...children...
 *   ogs_ber_wrap(ber, cls, tag, mark);
 */
size_t ogs_ber_mark(const ogs_ber_t *ber);
void ogs_ber_wrap(ogs_ber_t *ber, uint8_t cls, uint32_t tag, size_t mark);

#endif /* OGS_CDR_BER_H */
