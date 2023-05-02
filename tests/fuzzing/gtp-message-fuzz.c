/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include <stdio.h>
#include <stdint.h>

#include "fuzzing.h"
#include "ogs-gtp.h"

#define kMinInputLength 5
#define kMaxInputLength 1024

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) 
{ /* open5gs/tests/unit/gtp-message-test.c */

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 1;
    }
    initialize();

    int result;
    ogs_pkbuf_t *pkbuf;
    ogs_gtp2_create_session_request_t req;

    memset(&req, 0, sizeof(req));
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);

    ogs_pkbuf_put_data(pkbuf, Data, Size);

    ogs_tlv_parse_msg(&req, &ogs_gtp2_tlv_desc_create_session_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);

    ogs_pkbuf_free(pkbuf);

    terminate();
    return 0;
}
