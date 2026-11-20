/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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

#ifndef CHF_SM_H
#define CHF_SM_H

#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif

void chf_state_initial(ogs_fsm_t *s, chf_event_t *e);
void chf_state_final(ogs_fsm_t *s, chf_event_t *e);
void chf_state_operational(ogs_fsm_t *s, chf_event_t *e);

#define chf_sm_debug(__cE) \
    ogs_debug("%s(): %s", __func__, chf_event_get_name(__cE))

#ifdef __cplusplus
}
#endif

#endif /* CHF_SM_H */
