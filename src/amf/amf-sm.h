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

#ifndef AMF_SM_H
#define AMF_SM_H

#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif

void amf_state_initial(ogs_fsm_t *s, amf_event_t *e);
void amf_state_final(ogs_fsm_t *s, amf_event_t *e);
void amf_state_operational(ogs_fsm_t *s, amf_event_t *e);

void ngap_state_initial(ogs_fsm_t *s, amf_event_t *e);
void ngap_state_final(ogs_fsm_t *s, amf_event_t *e);
void ngap_state_operational(ogs_fsm_t *s, amf_event_t *e);
void ngap_state_exception(ogs_fsm_t *s, amf_event_t *e);

void gmm_state_initial(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_final(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_de_registered(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_authentication(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_security_mode(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_initial_context_setup(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_registered(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_ue_context_will_remove(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_exception(ogs_fsm_t *s, amf_event_t *e);
void gmm_abort_authentication(amf_ue_t *amf_ue);

#define amf_sm_debug(__pe) \
    ogs_debug("%s(): %s", __func__, amf_event_get_name(__pe))

/* Keep the failure log's file, line and function at the call site. */
#define AMF_RESTORE_CONTEXT_ON_FAILURE(amf_ue, s) do {                   \
    CLEAR_AMF_UE_TIMER((amf_ue)->t3560);                                 \
    (amf_ue)->registration_session_release_after_authentication = false; \
    if ((amf_ue)->can_restore_context) {                                 \
        amf_ue_restore_memento((amf_ue), &((amf_ue)->memento));            \
        (amf_ue)->security_context_available = 1;                        \
        (amf_ue)->mac_failed = 0;                                        \
        /* Once authenticated, the request is the UE's own */            \
        if (!OGS_FSM_CHECK((s), gmm_state_security_mode))                \
            RESTORE_NG_CONTEXT(amf_ue);                                  \
        else                                                             \
            CLEAR_NG_CONTEXT(amf_ue);                                    \
        if (!OGS_FSM_CHECK((s), gmm_state_registered))                   \
            OGS_FSM_TRAN((s), &gmm_state_registered);                    \
        ogs_warn("[%s] Failure in transaction; restoring context and "  \
                  "transitioning to REGISTERED.", (amf_ue)->supi);      \
    } else {                                                           \
        OGS_FSM_TRAN((s), &gmm_state_exception);                         \
        ogs_warn("[%s] Failure in transaction; no context "             \
                  "restoration.", (amf_ue)->supi);                      \
    }                                                                  \
} while (0)

#ifdef __cplusplus
}
#endif

#endif /* AMF_SM_H */
