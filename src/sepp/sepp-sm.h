/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SEPP_SM_H
#define SEPP_SM_H

#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif

void sepp_state_initial(ogs_fsm_t *s, sepp_event_t *e);
void sepp_state_final(ogs_fsm_t *s, sepp_event_t *e);
void sepp_state_operational(ogs_fsm_t *s, sepp_event_t *e);
void sepp_state_exception(ogs_fsm_t *s, sepp_event_t *e);

void sepp_handshake_state_initial(ogs_fsm_t *s, sepp_event_t *e);
void sepp_handshake_state_final(ogs_fsm_t *s, sepp_event_t *e);
void sepp_handshake_state_will_establish(ogs_fsm_t *s, sepp_event_t *e);
void sepp_handshake_state_established(ogs_fsm_t *s, sepp_event_t *e);
void sepp_handshake_state_terminated(ogs_fsm_t *s, sepp_event_t *e);
void sepp_handshake_state_exception(ogs_fsm_t *s, sepp_event_t *e);

void sepp_handshake_fsm_init(sepp_node_t *sepp_node, bool try_to_establish);
void sepp_handshake_fsm_fini(sepp_node_t *sepp_node);

#define sepp_sm_debug(__pe) \
    ogs_debug("%s(): %s", __func__, sepp_event_get_name(__pe))

#ifdef __cplusplus
}
#endif

#endif /* SEPP_SM_H */
