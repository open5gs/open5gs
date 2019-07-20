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

#include "s1ap-path.h"
#include "s1ap-build.h"
#include "esm-build.h"
#include "emm-build.h"
#include "nas-path.h"
#include "mme-event.h"
#include "mme-timer.h"
#include "mme-sm.h"

int nas_send_to_enb(mme_ue_t *mme_ue, ogs_pkbuf_t *pkbuf)
{
    enb_ue_t *enb_ue = NULL;

    ogs_assert(mme_ue);
    enb_ue = mme_ue->enb_ue;
    ogs_assert(enb_ue);

    return s1ap_send_to_enb_ue(enb_ue, pkbuf);
}

int nas_send_emm_to_esm(
        mme_ue_t *mme_ue, nas_esm_message_container_t *esm_message_container)
{
    ogs_pkbuf_t *esmbuf = NULL;

    ogs_assert(mme_ue);
    ogs_assert(esm_message_container);
    ogs_assert(esm_message_container->length);

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    esmbuf = ogs_pkbuf_alloc(NULL, NAS_HEADROOM+esm_message_container->length);
    ogs_pkbuf_reserve(esmbuf, NAS_HEADROOM);
    ogs_pkbuf_put_data(esmbuf,
            esm_message_container->buffer, esm_message_container->length);

    ogs_assert(s1ap_send_to_esm(mme_ue, esmbuf) == OGS_OK);

    return OGS_OK;
}

int nas_send_to_downlink_nas_transport(mme_ue_t *mme_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(pkbuf);
    ogs_assert(mme_ue);
    enb_ue = mme_ue->enb_ue;
    if (!enb_ue) {
        ogs_warn("S1 context has already been removed");
        ogs_pkbuf_free(pkbuf);

    } else {
        rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, pkbuf);
        ogs_assert(rv == OGS_OK && s1apbuf);

        rv = nas_send_to_enb(mme_ue, s1apbuf);
        ogs_assert(rv == OGS_OK);
    }

    return OGS_OK;
}

int nas_send_attach_accept(mme_ue_t *mme_ue)
{
    int rv;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL, *emmbuf = NULL;

    ogs_assert(mme_ue);
    sess = mme_sess_first(mme_ue);
    ogs_assert(sess);
    ogs_assert(mme_sess_next(sess) == NULL);
    bearer = mme_default_bearer_in_sess(sess);
    ogs_assert(bearer);
    ogs_assert(mme_bearer_next(bearer) == NULL);

    if (mme_ue->t3450.pkbuf) {
        s1apbuf = mme_ue->t3450.pkbuf;

    } else {
        rv = esm_build_activate_default_bearer_context_request(&esmbuf, sess);
        ogs_assert(rv == OGS_OK && esmbuf);

        rv = emm_build_attach_accept(&emmbuf, mme_ue, esmbuf);
        ogs_assert(rv == OGS_OK && emmbuf);

        rv = s1ap_build_initial_context_setup_request(&s1apbuf, mme_ue, emmbuf);
        ogs_assert(rv == OGS_OK && s1apbuf);
    }

    mme_ue->t3450.pkbuf = ogs_pkbuf_copy(s1apbuf);
    ogs_timer_start(mme_ue->t3450.timer, 
            mme_timer_cfg(MME_TIMER_T3450)->duration);

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_attach_reject(mme_ue_t *mme_ue,
    nas_emm_cause_t emm_cause, nas_esm_cause_t esm_cause)
{
    int rv;
    mme_sess_t *sess = NULL;
    ogs_pkbuf_t *esmbuf = NULL, *emmbuf = NULL;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Attach reject");
    ogs_debug("    IMSI[%s] Cause[%d]", mme_ue->imsi_bcd, emm_cause);

    sess = mme_sess_first(mme_ue);
    if (sess) {
        rv = esm_build_pdn_connectivity_reject(&esmbuf, sess, esm_cause);
        ogs_assert(rv == OGS_OK && esmbuf);
    }

    rv = emm_build_attach_reject(&emmbuf, emm_cause, esmbuf);
    ogs_assert(rv == OGS_OK && emmbuf);
    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return rv;
}

int nas_send_identity_request(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Identity request");

    if (mme_ue->t3470.pkbuf) {
        emmbuf = mme_ue->t3470.pkbuf;

    } else {
        rv = emm_build_identity_request(&emmbuf, mme_ue);
        ogs_assert(rv == OGS_OK && emmbuf);
    }

    mme_ue->t3470.pkbuf = ogs_pkbuf_copy(emmbuf);
    ogs_timer_start(mme_ue->t3470.timer, 
            mme_timer_cfg(MME_TIMER_T3470)->duration);

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return rv;
}

int nas_send_authentication_request(
        mme_ue_t *mme_ue, e_utran_vector_t *e_utran_vector)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Authentication request");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    if (mme_ue->t3460.pkbuf) {
        emmbuf = mme_ue->t3460.pkbuf;

    } else {
        ogs_assert(e_utran_vector);
        rv = emm_build_authentication_request(&emmbuf, e_utran_vector);
        ogs_assert(rv == OGS_OK && emmbuf);
    }

    mme_ue->t3460.pkbuf = ogs_pkbuf_copy(emmbuf);
    ogs_timer_start(mme_ue->t3460.timer, 
            mme_timer_cfg(MME_TIMER_T3460)->duration);

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return rv;
}

int nas_send_security_mode_command(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Security mode command");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    if (mme_ue->t3460.pkbuf) {
        emmbuf = mme_ue->t3460.pkbuf;

    } else {
        rv = emm_build_security_mode_command(&emmbuf, mme_ue);
        ogs_assert(rv == OGS_OK && emmbuf);
    }

    mme_ue->t3460.pkbuf = ogs_pkbuf_copy(emmbuf);
    ogs_timer_start(mme_ue->t3460.timer, 
            mme_timer_cfg(MME_TIMER_T3460)->duration);

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return rv;
}

int nas_send_authentication_reject(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Authentication reject");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    rv = emm_build_authentication_reject(&emmbuf);
    ogs_assert(rv == OGS_OK && emmbuf);

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_detach_accept(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);
    enb_ue = mme_ue->enb_ue;
    ogs_assert(enb_ue);

    /* reply with detach accept */
    if (mme_ue->nas_eps.detach.switch_off == 0) {
        rv = emm_build_detach_accept(&emmbuf, mme_ue);
        ogs_assert(rv == OGS_OK && emmbuf);

        rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
        ogs_assert(rv == OGS_OK);
    }

    CLEAR_ENB_UE_TIMER(enb_ue->t_ue_context_release);
    rv = s1ap_send_ue_context_release_command(enb_ue,
            S1AP_Cause_PR_nas, S1AP_CauseNas_detach,
            S1AP_UE_CTX_REL_S1_NORMAL_RELEASE, 0);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}


int nas_send_pdn_connectivity_reject(
    mme_sess_t *sess, nas_esm_cause_t esm_cause)
{
    int rv;
    mme_ue_t *mme_ue;
    ogs_pkbuf_t *esmbuf = NULL;

    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_registered)) {
        rv = esm_build_pdn_connectivity_reject(&esmbuf, sess, esm_cause);
        ogs_assert(rv == OGS_OK && esmbuf);

        rv = nas_send_to_downlink_nas_transport(mme_ue, esmbuf);
        ogs_assert(rv == OGS_OK);
    } else {
        /* During the UE-attach process, we'll send Attach-Reject 
         * with pyggybacking PDN-connectivity-Reject */
        rv = nas_send_attach_reject(mme_ue,
            EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED, esm_cause);
        ogs_assert(rv == OGS_OK);
    }

    return OGS_OK;
}

int nas_send_esm_information_request(mme_bearer_t *bearer)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    ogs_pkbuf_t *esmbuf = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    if (bearer->t3489.pkbuf) {
        esmbuf = bearer->t3489.pkbuf;
    } else {
        rv = esm_build_information_request(&esmbuf, bearer);
        ogs_assert(rv == OGS_OK && esmbuf);
    }

    bearer->t3489.pkbuf = ogs_pkbuf_copy(esmbuf);
    ogs_timer_start(bearer->t3489.timer, 
            mme_timer_cfg(MME_TIMER_T3489)->duration);

    rv = nas_send_to_downlink_nas_transport(mme_ue, esmbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_activate_default_bearer_context_request(mme_bearer_t *bearer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL;
    mme_sess_t *sess = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    rv = esm_build_activate_default_bearer_context_request(&esmbuf, sess);
    ogs_assert(rv == OGS_OK && esmbuf);

    rv = s1ap_build_e_rab_setup_request(&s1apbuf, bearer, esmbuf);
    ogs_assert(rv == OGS_OK && s1apbuf);

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_activate_dedicated_bearer_context_request(
        mme_bearer_t *bearer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    rv = esm_build_activate_dedicated_bearer_context_request(&esmbuf, bearer);
    ogs_assert(rv == OGS_OK && esmbuf);

    rv = s1ap_build_e_rab_setup_request(&s1apbuf, bearer, esmbuf);
    ogs_assert(rv == OGS_OK && s1apbuf);

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_activate_all_dedicated_bearers(mme_bearer_t *default_bearer)
{
    int rv;

    ogs_assert(default_bearer);

    mme_bearer_t *dedicated_bearer = mme_bearer_next(default_bearer);
    while (dedicated_bearer) {
        rv = nas_send_activate_dedicated_bearer_context_request(
                dedicated_bearer);
        ogs_assert(rv == OGS_OK);

        dedicated_bearer = mme_bearer_next(dedicated_bearer);
    }

    return OGS_OK;
}

int nas_send_modify_bearer_context_request(
        mme_bearer_t *bearer, int qos_presence, int tft_presence)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    rv = esm_build_modify_bearer_context_request(
            &esmbuf, bearer, qos_presence, tft_presence);
    ogs_assert(rv == OGS_OK && esmbuf);

    if (qos_presence == 1) {
        rv = s1ap_build_e_rab_modify_request(&s1apbuf, bearer, esmbuf);
        ogs_assert(rv == OGS_OK && s1apbuf);

        rv = nas_send_to_enb(mme_ue, s1apbuf);
        ogs_assert(rv == OGS_OK);
    } else {
        rv = nas_send_to_downlink_nas_transport(mme_ue, esmbuf);
        ogs_assert(rv == OGS_OK);
    }

    return OGS_OK;
}

int nas_send_deactivate_bearer_context_request(mme_bearer_t *bearer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    rv = esm_build_deactivate_bearer_context_request(
            &esmbuf, bearer, ESM_CAUSE_REGULAR_DEACTIVATION);
    ogs_assert(rv == OGS_OK && esmbuf);

    rv = s1ap_build_e_rab_release_command(&s1apbuf, bearer, esmbuf,
            S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release);
    ogs_assert(rv == OGS_OK && s1apbuf);

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_tau_accept(
        mme_ue_t *mme_ue, S1AP_ProcedureCode_t procedureCode)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Tracking area update accept");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    rv = emm_build_tau_accept(&emmbuf, mme_ue);
    ogs_assert(rv == OGS_OK);

    if (procedureCode == S1AP_ProcedureCode_id_InitialContextSetup) {
        ogs_pkbuf_t *s1apbuf = NULL;
        rv = s1ap_build_initial_context_setup_request(&s1apbuf, mme_ue, emmbuf);
        ogs_assert(rv == OGS_OK && s1apbuf);

        rv = nas_send_to_enb(mme_ue, s1apbuf);
        ogs_assert(rv == OGS_OK);
    } else if (procedureCode == S1AP_ProcedureCode_id_downlinkNASTransport) {
        rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
        ogs_assert(rv == OGS_OK);
    } else
        ogs_assert_if_reached();

    return rv;
}

int nas_send_tau_reject(mme_ue_t *mme_ue, nas_emm_cause_t emm_cause)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);

    /* Build TAU reject */
    rv = emm_build_tau_reject(&emmbuf, emm_cause, mme_ue);
    ogs_assert(rv == OGS_OK);

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_service_reject(mme_ue_t *mme_ue, nas_emm_cause_t emm_cause)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);

    /* Build Service Reject */
    rv = emm_build_service_reject(&emmbuf, emm_cause, mme_ue);
    ogs_assert(rv == OGS_OK);

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_cs_service_notification(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] CS Service Notification");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    rv = emm_build_cs_service_notification(&emmbuf, mme_ue);
    ogs_assert(rv == OGS_OK && emmbuf);

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int nas_send_downlink_nas_transport(
        mme_ue_t *mme_ue, uint8_t *buffer, uint8_t length)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    ogs_assert(mme_ue);
    ogs_assert(buffer);
    ogs_assert(length);

    ogs_debug("[EMM] Downlink NAS transport");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    rv = emm_build_downlink_nas_transport(&emmbuf, mme_ue, buffer, length);
    ogs_assert(rv == OGS_OK && emmbuf);

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}
