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

#ifndef AF_EVENT_H
#define AF_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct af_sess_s af_sess_t;

typedef enum {
    AF_EVENT_BASE = OGS_MAX_NUM_OF_PROTO_EVENT,

    AF_EVENT_SBI_LOCAL,

    MAX_NUM_OF_AF_EVENT,

} af_event_e;

typedef struct af_event_s {
    ogs_event_t h;
    int local_id;

    ogs_pkbuf_t *pkbuf;

    struct {
        ogs_sbi_service_type_e service_type;
        void *data;
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data);
    } local;

    af_sess_t *sess;
} af_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(af_event_t));

af_event_t *af_event_new(int id);

const char *af_event_get_name(af_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* AF_EVENT_H */
