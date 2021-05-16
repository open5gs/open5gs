/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "ngap-path.h"

void ngap_send_to_n2sm(smf_sess_t *sess,
        ogs_sbi_stream_t *stream, int type, ogs_pkbuf_t *pkbuf)
{
    int rv;
    smf_event_t *e = NULL;

    ogs_assert(sess);
    ogs_assert(pkbuf);

    e = smf_event_new(SMF_EVT_NGAP_MESSAGE);
    ogs_assert(e);
    e->sess = sess;
    e->sbi.data = stream;
    e->pkbuf = pkbuf;
    e->ngap.type = type;
    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        smf_event_free(e);
    }
}