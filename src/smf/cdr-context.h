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

#ifndef SMF_CDR_CONTEXT_H
#define SMF_CDR_CONTEXT_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Offline PGW-CDR generation (TS 32.251/32.298/32.297).
 * One service-data container per record, default rating group.
 * Closure causes: normalRelease, abnormalRelease, volumeLimit, timeLimit.
 */

void smf_cdr_init(void);
void smf_cdr_final(void);

/* EPC session established: open the first CDR of the session */
void smf_cdr_sess_start(smf_sess_t *sess);

/*
 * Volume delta from any PFCP usage report of this session.
 * first/last packet times are ogs_time_t (0 when absent).
 * Closes a partial record when volume_limit is reached or a
 * time-limit query is pending.
 */
void smf_cdr_sess_usage(smf_sess_t *sess, uint64_t ul_octets,
        uint64_t dl_octets, ogs_time_t time_of_first_packet,
        ogs_time_t time_of_last_packet);

/* Session is ending: write the final record.
 * normal=false means abnormalRelease (restart/association loss). */
void smf_cdr_sess_stop(smf_sess_t *sess, bool normal);

#ifdef __cplusplus
}
#endif

#endif /* SMF_CDR_CONTEXT_H */
