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
#include "ngap-build.h"
#include "gmm-build.h"
#if 0
#include "gsm-build.h"
#endif
#include "nas-path.h"

int nas_5gs_send_to_gnb(amf_ue_t *amf_ue, ogs_pkbuf_t *pkbuf)
{
    ran_ue_t *ran_ue = NULL;

    ogs_assert(amf_ue);
    ran_ue = amf_ue->ran_ue;
    ogs_assert(ran_ue);

    return ngap_send_to_ran_ue(ran_ue, pkbuf);
}

#if 0
int nas_5gs_send_gmm_to_esm(amf_ue_t *amf_ue,
        ogs_nas_esm_message_container_t *esm_message_container)
{
    int rv;
    ogs_pkbuf_t *gsmbuf = NULL;

    ogs_assert(amf_ue);
    ogs_assert(esm_message_container);
    ogs_assert(esm_message_container->length);

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    gsmbuf = ogs_pkbuf_alloc(NULL,
            OGS_NAS_HEADROOM+esm_message_container->length);
    ogs_pkbuf_reserve(gsmbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(gsmbuf,
            esm_message_container->buffer, esm_message_container->length);

    rv = ngap_send_to_esm(amf_ue, gsmbuf);
    if (rv != OGS_OK) {
        ogs_error("ngap_send_to_esm() failed");
    }

    return rv;
}
#endif

int nas_5gs_send_to_downlink_nas_transport(amf_ue_t *amf_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(pkbuf);
    ogs_assert(amf_ue);
    ran_ue = amf_ue->ran_ue;
    if (!ran_ue) {
        ogs_warn("NG context has already been removed");
        ogs_pkbuf_free(pkbuf);

    } else {
        ngapbuf = ngap_build_downlink_nas_transport(ran_ue, pkbuf);
        if (!ngapbuf) {
            ogs_error("ngap_build_downlink_nas_transport() failed");
            return OGS_ERROR;
        }

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        if (rv != OGS_OK) {
            ogs_error("nas_5gs_send_to_gnb() failed");
            return OGS_ERROR;
        }
    }

    return OGS_OK;
}

void nas_5gs_send_nas_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    ogs_assert(amf_ue);

    switch(amf_ue->nas.type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_authentication)) {
            nas_5gs_send_authentication_reject(amf_ue);
        } else {
            nas_5gs_send_registration_reject(amf_ue, gmm_cause);
        }
        break;
    default: 
        ogs_fatal("Unknown type : %d", amf_ue->nas.type);
        ogs_assert_if_reached();
        break;
    }
}

void nas_5gs_send_nas_reject_from_sbi(amf_ue_t *amf_ue, int status)
{
    ogs_nas_5gmm_cause_t gmm_cause;

    ogs_assert(amf_ue);

    switch(status) {
    case OGS_SBI_HTTP_STATUS_NOT_FOUND:
        gmm_cause = OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED;
        break;
    default:
        gmm_cause = OGS_5GMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED;
    }

    nas_5gs_send_nas_reject(amf_ue, gmm_cause);
}

#if 0
void nas_5gs_send_registration_accept(amf_ue_t *amf_ue)
{
    int rv;
    amf_sess_t *sess = NULL;
    amf_bearer_t *bearer = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;
    ogs_pkbuf_t *gsmbuf = NULL, *gmmbuf = NULL;

    ogs_assert(amf_ue);
    sess = amf_sess_first(amf_ue);
    ogs_assert(sess);
    ogs_assert(amf_sess_next(sess) == NULL);
    bearer = amf_default_bearer_in_sess(sess);
    ogs_assert(bearer);
    ogs_assert(amf_bearer_next(bearer) == NULL);

    gsmbuf = esm_build_activate_default_bearer_context_request(sess);
    ogs_expect_or_return(gsmbuf);

    gmmbuf = gmm_build_registration_accept(amf_ue, gsmbuf);
    ogs_expect_or_return(gmmbuf);

    ngapbuf = ngap_build_initial_context_setup_request(amf_ue, gmmbuf);
    ogs_expect_or_return(ngapbuf);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect_or_return(rv == OGS_OK);
}
#endif

void nas_5gs_send_registration_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_warn("[%s] Registration reject [%d]", amf_ue->suci, gmm_cause);

    gmmbuf = gmm_build_registration_reject(gmm_cause);
    ogs_expect_or_return(gmmbuf);
    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect_or_return(rv == OGS_OK);
}

void nas_5gs_send_identity_request(amf_ue_t *amf_ue)
{
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("Identity request");

    if (amf_ue->t3570.pkbuf) {
        gmmbuf = amf_ue->t3570.pkbuf;
        ogs_expect_or_return(gmmbuf);
    } else {
        gmmbuf = gmm_build_identity_request(amf_ue);
        ogs_expect_or_return(gmmbuf);
    }

    amf_ue->t3570.pkbuf = ogs_pkbuf_copy(gmmbuf);
    ogs_timer_start(amf_ue->t3570.timer,
            amf_timer_cfg(AMF_TIMER_T3570)->duration);

    nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
}

void nas_5gs_send_authentication_request(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] Authentication request", amf_ue->suci);

    if (amf_ue->t3560.pkbuf) {
        gmmbuf = amf_ue->t3560.pkbuf;
        ogs_expect_or_return(gmmbuf);
    } else {
        gmmbuf = gmm_build_authentication_request(amf_ue);
        ogs_expect_or_return(gmmbuf);
    }

    amf_ue->t3560.pkbuf = ogs_pkbuf_copy(gmmbuf);
    ogs_timer_start(amf_ue->t3560.timer,
            amf_timer_cfg(AMF_TIMER_T3560)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_authentication_reject(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] Authentication reject", amf_ue->suci);

    gmmbuf = gmm_build_authentication_reject();
    ogs_expect_or_return(gmmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_security_mode_command(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] Security mode command", amf_ue->supi);

    if (amf_ue->t3560.pkbuf) {
        gmmbuf = amf_ue->t3560.pkbuf;
        ogs_expect_or_return(gmmbuf);
    } else {
        gmmbuf = gmm_build_security_mode_command(amf_ue);
        ogs_expect_or_return(gmmbuf);
    }

    amf_ue->t3560.pkbuf = ogs_pkbuf_copy(gmmbuf);
    ogs_timer_start(amf_ue->t3560.timer,
            amf_timer_cfg(AMF_TIMER_T3560)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
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
