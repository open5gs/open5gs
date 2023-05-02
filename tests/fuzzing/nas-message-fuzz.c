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
#include "ogs-nas-eps.h"

#define kMinInputLength 5
#define kMaxInputLength 1024

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) 
{ /* open5gs/tests/unit/nas-message-test.c */

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 1;
    }

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_NONE);
    }

    int result;
    ogs_pkbuf_t *pkbuf;
    ogs_nas_eps_message_t message;

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    if (pkbuf == NULL) {
        return 1;
    }

    ogs_pkbuf_put_data(pkbuf, Data, Size);

    result = ogs_nas_emm_decode(&message, pkbuf);

    ogs_pkbuf_free(pkbuf);

    return result;
}
