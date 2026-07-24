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

#ifndef SGWC_CDR_CONTEXT_H
#define SGWC_CDR_CONTEXT_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Offline SGW-CDR generation (TS 32.251/32.298/32.297).
 * Same closure model as the SMF PGW-CDR: normalRelease,
 * abnormalRelease, volumeLimit, timeLimit.
 */

void sgwc_cdr_init(void);
void sgwc_cdr_final(void);

/* session fully established (Create Session Response accepted) */
void sgwc_cdr_sess_start(sgwc_sess_t *sess);

/* volume delta from any Sxa usage report of this session */
void sgwc_cdr_sess_usage(sgwc_sess_t *sess,
        uint64_t ul_octets, uint64_t dl_octets);

/* session ending; normal=false means abnormalRelease */
void sgwc_cdr_sess_stop(sgwc_sess_t *sess, bool normal);

#ifdef __cplusplus
}
#endif

#endif /* SGWC_CDR_CONTEXT_H */
