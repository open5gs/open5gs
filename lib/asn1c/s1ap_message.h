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

#ifndef _S1AP_MESSAGE_H__
#define _S1AP_MESSAGE_H__

#include "ogs-core.h"
#include "base/types.h"
#include "base/context.h"
#include "s1ap_asn1c.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Octets */
#define S1AP_CLEAR_DATA(__dATA) \
    do { \
        ogs_assert((__dATA)); \
        if ((__dATA)->buf) \
        { \
            ogs_free((__dATA)->buf); \
            (__dATA)->buf = NULL; \
            (__dATA)->size = 0; \
        } \
    } while(0)
#define S1AP_STORE_DATA(__dST, __sRC) \
    do { \
        ogs_assert((__sRC)); \
        ogs_assert((__sRC)->buf); \
        ogs_assert((__dST)); \
        S1AP_CLEAR_DATA(__dST); \
        (__dST)->size = (__sRC)->size; \
        (__dST)->buf = ogs_calloc((__dST)->size, sizeof(uint8_t)); \
        memcpy((__dST)->buf, (__sRC)->buf, (__dST)->size); \
    } while(0)

typedef struct S1AP_S1AP_PDU s1ap_message_t;

int s1ap_decode_pdu(s1ap_message_t *message, ogs_pkbuf_t *pkbuf);
int s1ap_encode_pdu(ogs_pkbuf_t **pkbuf, s1ap_message_t *message);
int s1ap_free_pdu(s1ap_message_t *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

