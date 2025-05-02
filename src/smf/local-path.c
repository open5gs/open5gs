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

#include "local-path.h"

void smf_trigger_session_release(smf_sess_t *sess,
    ogs_sbi_stream_t *stream, int pfcp_trigger)
{
    int rv;
    smf_event_t *e;
    ogs_pool_id_t sid;

    /* Ensure session is valid */
    ogs_assert(sess);

    /* Create a session release event */
    e = smf_event_new(SMF_EVT_SESSION_RELEASE);
    ogs_assert(e);

    /* Assign session ID */
    e->sess_id = sess->id;

    /* Store the PFCP trigger in the SBI state field */
    e->h.sbi.state = pfcp_trigger;

    /* Attach SBI stream ID if provided */
    if (stream) {
        sid = ogs_sbi_id_from_stream(stream);
        ogs_assert(sid >= OGS_MIN_POOL_ID &&
                   sid <= OGS_MAX_POOL_ID);
        e->h.sbi.data = OGS_UINT_TO_POINTER(sid);
    }

    /* Push event to the application queue */
    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push failed:%d", rv);
        ogs_event_free(e);
    }
}
