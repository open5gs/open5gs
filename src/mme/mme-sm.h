/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef MME_SM_H
#define MME_SM_H

#include "mme-event.h"

#ifdef __cplusplus
extern "C" {
#endif

void mme_state_initial(ogs_fsm_t *s, mme_event_t *e);
void mme_state_final(ogs_fsm_t *s, mme_event_t *e);
void mme_state_operational(ogs_fsm_t *s, mme_event_t *e);
void mme_state_exception(ogs_fsm_t *s, mme_event_t *e);

void s1ap_state_initial(ogs_fsm_t *s, mme_event_t *e);
void s1ap_state_final(ogs_fsm_t *s, mme_event_t *e);
void s1ap_state_operational(ogs_fsm_t *s, mme_event_t *e);
void s1ap_state_exception(ogs_fsm_t *s, mme_event_t *e);

void sbcap_state_initial(ogs_fsm_t *s, mme_event_t *e);
void sbcap_state_final(ogs_fsm_t *s, mme_event_t *e);
void sbcap_state_operational(ogs_fsm_t *s, mme_event_t *e);
void sbcap_state_exception(ogs_fsm_t *s, mme_event_t *e);

void emm_state_initial(ogs_fsm_t *s, mme_event_t *e);
void emm_state_final(ogs_fsm_t *s, mme_event_t *e);
void emm_state_de_registered(ogs_fsm_t *s, mme_event_t *e);
void emm_state_authentication(ogs_fsm_t *s, mme_event_t *e);
void emm_state_security_mode(ogs_fsm_t *s, mme_event_t *e);
void emm_state_initial_context_setup(ogs_fsm_t *s, mme_event_t *e);
void emm_state_registered(ogs_fsm_t *s, mme_event_t *e);
void emm_state_exception(ogs_fsm_t *s, mme_event_t *e);

void esm_state_initial(ogs_fsm_t *s, mme_event_t *e);
void esm_state_final(ogs_fsm_t *s, mme_event_t *e);
void esm_state_inactive(ogs_fsm_t *s, mme_event_t *e);
void esm_state_active(ogs_fsm_t *s, mme_event_t *e);
void esm_state_pdn_will_disconnect(ogs_fsm_t *s, mme_event_t *e);
void esm_state_pdn_did_disconnect(ogs_fsm_t *s, mme_event_t *e);
void esm_state_bearer_deactivated(ogs_fsm_t *s, mme_event_t *e);
void esm_state_exception(ogs_fsm_t *s, mme_event_t *e);

void sgsap_state_initial(ogs_fsm_t *s, mme_event_t *e);
void sgsap_state_final(ogs_fsm_t *s, mme_event_t *e);
void sgsap_state_will_connect(ogs_fsm_t *s, mme_event_t *e);
void sgsap_state_connected(ogs_fsm_t *s, mme_event_t *e);
void sgsap_state_exception(ogs_fsm_t *s, mme_event_t *e);

#define mme_sm_debug(__pe) \
    ogs_debug("%s(): %s\n", __func__, mme_event_get_name(__pe))

#ifdef __cplusplus
}
#endif

#endif /* MME_SM_H */
