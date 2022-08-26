/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef AF_LOCAL_H
#define AF_LOCAL_H

#include "sbi-path.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    AF_LOCAL_BASE = 0,

    AF_LOCAL_DISCOVER_AND_SEND,
    AF_LOCAL_SEND_TO_PCF,

    MAX_NUM_OF_AF_LOCAL,

} af_local_e;

const char *af_local_get_name(af_local_e id);

void af_local_discover_and_send(
        ogs_sbi_service_type_e service_type,
        af_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data));

void af_local_send_to_pcf(
        af_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data));

#ifdef __cplusplus
}
#endif

#endif /* AF_LOCAL_H */
