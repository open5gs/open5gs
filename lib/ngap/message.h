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

#if !defined(OGS_NGAP_INSIDE) && !defined(OGS_NGAP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NGAP_MESSAGE_H
#define OGS_NGAP_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Octets */
#define OGS_NGAP_CLEAR_DATA(__dATA) \
    do { \
        ogs_assert((__dATA)); \
        if ((__dATA)->buf) { \
            FREEMEM((__dATA)->buf); \
            (__dATA)->buf = NULL; \
            (__dATA)->size = 0; \
        } \
    } while(0)
#define OGS_NGAP_STORE_DATA(__dST, __sRC) \
    do { \
        ogs_assert((__sRC)); \
        ogs_assert((__sRC)->buf); \
        ogs_assert((__dST)); \
        OGS_NGAP_CLEAR_DATA(__dST); \
        (__dST)->size = (__sRC)->size; \
        (__dST)->buf = CALLOC((__dST)->size, sizeof(uint8_t)); \
        memcpy((__dST)->buf, (__sRC)->buf, (__dST)->size); \
    } while(0)

typedef struct NGAP_NGAP_PDU ogs_ngap_message_t;

int ogs_ngap_decode(ogs_ngap_message_t *message, ogs_pkbuf_t *pkbuf);
ogs_pkbuf_t *ogs_ngap_encode(ogs_ngap_message_t *message);
int ogs_ngap_free(ogs_ngap_message_t *message);

#ifdef __cplusplus
}
#endif

#endif

