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

#if 0
#include "ngap-path.h"
#include "ngap-build.h"
#include "gsm-build.h"
#endif
#include "nas-path.h"

void nas_5gs_send_to_gsm(smf_sess_t *sess, ogs_pkbuf_t *pkbuf)
{
    int rv;
    smf_event_t *e = NULL;

    ogs_assert(sess);
    ogs_assert(pkbuf);

    e = smf_event_new(SMF_EVT_5GSM_MESSAGE);
    ogs_assert(e);
    e->sess = sess;
    e->pkbuf = pkbuf;
    rv = ogs_queue_push(smf_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        smf_event_free(e);
    }
}

#if 0
void nas_5gs_send_detach_accept(amf_ue_t *amf_ue)
{
    ran_ue_t *ran_ue = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);
    ran_ue = amf_ue->ran_ue;
    ogs_assert(ran_ue);

    /* reply with detach accept */
    if (amf_ue->nas_5gs.detach.switch_off == 0) {
        int rv;
        gmmbuf = gmm_build_detach_accept(amf_ue);
        ogs_expect_or_return(gmmbuf);

        rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
        ogs_expect_or_return(rv == OGS_OK);
    }

    ngap_send_ue_context_release_command(ran_ue,
            NGAP_Cause_PR_nas, NGAP_CauseNas_detach,
            NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
}


void nas_5gs_send_pdn_connectivity_reject(
    amf_sess_t *sess, ogs_nas_gsm_cause_t gsm_cause)
{
    int rv;
    amf_ue_t *amf_ue;
    ogs_pkbuf_t *gsmbuf = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_registered)) {
        gsmbuf = esm_build_pdn_connectivity_reject(sess, gsm_cause);
        ogs_expect_or_return(gsmbuf);

        rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gsmbuf);
        ogs_expect(rv == OGS_OK);
    } else {
        /* During the UE-registration process, we'll send Registration-Reject 
         * with pyggybacking PDN-connectivity-Reject */
        nas_5gs_send_registration_reject(amf_ue,
            EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED, gsm_cause);
    }
}

void nas_5gs_send_esm_information_request(amf_bearer_t *bearer)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_pkbuf_t *gsmbuf = NULL;

    ogs_assert(bearer);
    amf_ue = bearer->amf_ue;
    ogs_assert(amf_ue);

    if (bearer->t3589.pkbuf) {
        gsmbuf = bearer->t3589.pkbuf;
        ogs_expect_or_return(gsmbuf);
    } else {
        gsmbuf = esm_build_information_request(bearer);
        ogs_expect_or_return(gsmbuf);
    }

    bearer->t3589.pkbuf = ogs_pkbuf_copy(gsmbuf);
    ogs_timer_start(bearer->t3589.timer,
            amf_timer_cfg(AMF_TIMER_T3589)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gsmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_activate_default_bearer_context_request(amf_bearer_t *bearer)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;
    ogs_pkbuf_t *gsmbuf = NULL;
    amf_sess_t *sess = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    amf_ue = bearer->amf_ue;
    ogs_assert(amf_ue);

    gsmbuf = esm_build_activate_default_bearer_context_request(sess);
    ogs_expect_or_return(gsmbuf);

    ngapbuf = ngap_build_e_rab_setup_request(bearer, gsmbuf);
    ogs_expect_or_return(ngapbuf);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_activate_dedicated_bearer_context_request(
        amf_bearer_t *bearer)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;
    ogs_pkbuf_t *gsmbuf = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(bearer);
    amf_ue = bearer->amf_ue;
    ogs_assert(amf_ue);

    gsmbuf = esm_build_activate_dedicated_bearer_context_request(bearer);
    ogs_expect_or_return(gsmbuf);

    ngapbuf = ngap_build_e_rab_setup_request(bearer, gsmbuf);
    ogs_expect_or_return(ngapbuf);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_activate_all_dedicated_bearers(amf_bearer_t *default_bearer)
{
    ogs_assert(default_bearer);

    amf_bearer_t *dedicated_bearer = amf_bearer_next(default_bearer);
    while (dedicated_bearer) {
        nas_5gs_send_activate_dedicated_bearer_context_request(
                dedicated_bearer);
        dedicated_bearer = amf_bearer_next(dedicated_bearer);
    }
}

void nas_5gs_send_modify_bearer_context_request(
        amf_bearer_t *bearer, int qos_presence, int tft_presence)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;
    ogs_pkbuf_t *gsmbuf = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(bearer);
    amf_ue = bearer->amf_ue;
    ogs_assert(amf_ue);

    gsmbuf = esm_build_modify_bearer_context_request(
            bearer, qos_presence, tft_presence);
    ogs_expect_or_return(gsmbuf);

    if (qos_presence == 1) {
        ngapbuf = ngap_build_e_rab_modify_request(bearer, gsmbuf);
        ogs_expect_or_return(ngapbuf);

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);
    } else {
        rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gsmbuf);
        ogs_expect(rv == OGS_OK);
    }
}

void nas_5gs_send_deactivate_bearer_context_request(amf_bearer_t *bearer)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;
    ogs_pkbuf_t *gsmbuf = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(bearer);
    amf_ue = bearer->amf_ue;
    ogs_assert(amf_ue);

    gsmbuf = esm_build_deactivate_bearer_context_request(
            bearer, ESM_CAUSE_REGULAR_DEACTIVATION);
    ogs_expect_or_return(gsmbuf);

    ngapbuf = ngap_build_e_rab_release_command(bearer, gsmbuf,
            NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release);
    ogs_expect_or_return(ngapbuf);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_bearer_resource_allocation_reject(
    amf_bearer_t *bearer, ogs_nas_gsm_cause_t gsm_cause)
{
    int rv;
    amf_ue_t *amf_ue;
    ogs_pkbuf_t *gsmbuf = NULL;

    ogs_assert(bearer);
    amf_ue = bearer->amf_ue;
    ogs_assert(bearer);

    gsmbuf = esm_build_bearer_resource_allocation_reject(bearer, gsm_cause);
    ogs_expect_or_return(gsmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gsmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_bearer_resource_modification_reject(
    amf_bearer_t *bearer, ogs_nas_gsm_cause_t gsm_cause)
{
    int rv;
    amf_ue_t *amf_ue;
    ogs_pkbuf_t *gsmbuf = NULL;

    ogs_assert(bearer);
    amf_ue = bearer->amf_ue;
    ogs_assert(bearer);

    gsmbuf = esm_build_bearer_resource_modification_reject(bearer, gsm_cause);
    ogs_expect_or_return(gsmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gsmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_tau_accept(
        amf_ue_t *amf_ue, NGAP_ProcedureCode_t procedureCode)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("Tracking area update accept");
    ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

    gmmbuf = gmm_build_tau_accept(amf_ue);
    ogs_expect_or_return(gmmbuf);

    if (procedureCode == NGAP_ProcedureCode_id_InitialContextSetup) {
        ogs_pkbuf_t *ngapbuf = NULL;
        ngapbuf = ngap_build_initial_context_setup_request(amf_ue, gmmbuf);
        ogs_expect_or_return(ngapbuf);

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);
    } else if (procedureCode == NGAP_ProcedureCode_id_downlinkNASTransport) {
        rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
        ogs_expect(rv == OGS_OK);
    } else
        ogs_assert_if_reached();
}

void nas_5gs_send_tau_reject(amf_ue_t *amf_ue, ogs_nas_gmm_cause_t gmm_cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    /* Build TAU reject */
    gmmbuf = gmm_build_tau_reject(gmm_cause, amf_ue);
    ogs_expect_or_return(gmmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_service_reject(amf_ue_t *amf_ue,
        ogs_nas_gmm_cause_t gmm_cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    /* Build Service Reject */
    gmmbuf = gmm_build_service_reject(gmm_cause, amf_ue);
    ogs_expect_or_return(gmmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_cs_service_notification(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("CS Service Notification");
    ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

    gmmbuf = gmm_build_cs_service_notification(amf_ue);
    ogs_expect_or_return(gmmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_downlink_nas_transport(
        amf_ue_t *amf_ue, uint8_t *buffer, uint8_t length)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);
    ogs_assert(buffer);
    ogs_assert(length);

    ogs_debug("Downlink NAS transport");
    ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

    gmmbuf = gmm_build_downlink_nas_transport(amf_ue, buffer, length);
    ogs_expect_or_return(gmmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}
#endif
