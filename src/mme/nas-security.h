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

#ifndef NAS_SECURITY_H
#define NAS_SECURITY_H

#include "ogs-crypt.h"

#include "mme-context.h"
#include "snow-3g.h"
#include "zuc.h"

#define NAS_SECURITY_BEARER 0
#define NAS_SECURITY_DOWNLINK_DIRECTION 1
#define NAS_SECURITY_UPLINK_DIRECTION 0

#define NAS_SECURITY_MAC_SIZE 4

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _nas_security_header_type_t {
    union {
        struct {
        ED5(uint8_t integrity_protected:1;,
            uint8_t ciphered:1;,
            uint8_t new_security_context:1;,
            uint8_t service_request:1;,
            uint8_t reserved:4;)
        };
        uint8_t type;
    };
} __attribute__ ((packed)) nas_security_header_type_t;

int nas_security_encode(
        ogs_pkbuf_t **pkbuf, mme_ue_t *mme_ue, nas_message_t *message);
int nas_security_decode(mme_ue_t *mme_ue, 
        nas_security_header_type_t security_header_type, ogs_pkbuf_t *pkbuf);

void nas_mac_calculate(uint8_t algorithm_identity,
        uint8_t *knas_int, uint32_t count, uint8_t bearer, 
        uint8_t direction, ogs_pkbuf_t *pkbuf, uint8_t *mac);

void nas_encrypt(uint8_t algorithm_identity,
        uint8_t *knas_enc, uint32_t count, uint8_t bearer, 
        uint8_t direction, ogs_pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif

#endif /* NAS_SECURITY_H */

