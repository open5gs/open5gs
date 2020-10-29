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
#include "nas-path.h"

int nas_5gs_send_to_gnb(amf_ue_t *amf_ue, ogs_pkbuf_t *pkbuf)
{
    ran_ue_t *ran_ue = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    return ngap_send_to_ran_ue(ran_ue, pkbuf);
}

int nas_5gs_send_to_downlink_nas_transport(amf_ue_t *amf_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(pkbuf);
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
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
            return OGS_ERROR;
        }
    }

    return OGS_OK;
}

void nas_5gs_send_registration_accept(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    gmmbuf = gmm_build_registration_accept(amf_ue);
    ogs_expect_or_return(gmmbuf);

    ngapbuf = ngap_build_initial_context_setup_request(amf_ue, gmmbuf);
    ogs_expect_or_return(ngapbuf);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect_or_return(rv == OGS_OK);
}

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
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_service_accept(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(amf_ue);

    gmmbuf = gmm_build_service_accept(amf_ue);
    ogs_expect_or_return(gmmbuf);

    switch (amf_ue->nas.ngapProcedureCode) {
    case NGAP_ProcedureCode_id_InitialUEMessage:
        ngapbuf = ngap_build_initial_context_setup_request(amf_ue, gmmbuf);
        ogs_expect_or_return(ngapbuf);

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect_or_return(rv == OGS_OK);
        break;

    case NGAP_ProcedureCode_id_UplinkNASTransport:
        rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
        ogs_expect_or_return(rv == OGS_OK);
        break;

    default:
        ogs_error("Invalid NGAP ProcedureCode [%d]",
                (int)amf_ue->nas.ngapProcedureCode);
        return;
    }
}

void nas_5gs_send_service_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    gmmbuf = gmm_build_service_reject(amf_ue, gmm_cause);
    ogs_expect_or_return(gmmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_accept(amf_ue_t *amf_ue)
{
    ogs_assert(amf_ue);

    switch(amf_ue->nas.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        nas_5gs_send_registration_accept(amf_ue);
        break;
    case OGS_NAS_5GS_SERVICE_REQUEST:
        nas_5gs_send_service_accept(amf_ue);
        break;
    default:
        ogs_error("Unknown message type [%d]", amf_ue->nas.message_type);
    }
}

void nas_5gs_send_de_registration_accept(amf_ue_t *amf_ue)
{
    ran_ue_t *ran_ue = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    if (amf_ue->nas.de_registration.switch_off == 0) {
        int rv;

        gmmbuf = gmm_build_de_registration_accept(amf_ue);
        ogs_expect_or_return(gmmbuf);

        rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
        ogs_expect_or_return(rv == OGS_OK);
    }

    ngap_send_ran_ue_context_release_command(ran_ue,
            NGAP_Cause_PR_nas, NGAP_CauseNas_deregister,
            NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
}

void nas_5gs_send_identity_request(amf_ue_t *amf_ue)
{
    int rv;
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
    ogs_assert(amf_ue->t3570.pkbuf);
    ogs_timer_start(amf_ue->t3570.timer,
            amf_timer_cfg(AMF_TIMER_T3570)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
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
    ogs_assert(amf_ue->t3560.pkbuf);
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

    ogs_warn("[%s] Authentication reject", amf_ue->suci);

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
    ogs_assert(amf_ue->t3560.pkbuf);
    ogs_timer_start(amf_ue->t3560.timer,
            amf_timer_cfg(AMF_TIMER_T3560)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_configuration_update_command(
        amf_ue_t *amf_ue, gmm_configuration_update_command_param_t *param)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] Configuration update command", amf_ue->supi);

    if (amf_ue->t3555.pkbuf) {
        gmmbuf = amf_ue->t3555.pkbuf;
        ogs_expect_or_return(gmmbuf);

        amf_ue->t3555.pkbuf = ogs_pkbuf_copy(gmmbuf);
        ogs_assert(amf_ue->t3555.pkbuf);
        ogs_timer_start(amf_ue->t3555.timer,
                amf_timer_cfg(AMF_TIMER_T3555)->duration);

    } else {
        ogs_expect_or_return(param);
        gmmbuf = gmm_build_configuration_update_command(amf_ue, param);
        ogs_expect_or_return(gmmbuf);

        if (param->acknowledgement_requested) {
            amf_ue->t3555.pkbuf = ogs_pkbuf_copy(gmmbuf);
            ogs_assert(amf_ue->t3555.pkbuf);
            ogs_timer_start(amf_ue->t3555.timer,
                    amf_timer_cfg(AMF_TIMER_T3555)->duration);
        }
    }

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_send_pdu_session_release_command(amf_sess_t *sess,
        ogs_pkbuf_t *n1smbuf, ogs_pkbuf_t *n2smbuf)
{
    int rv;

    amf_ue_t *amf_ue = NULL;

    ogs_pkbuf_t *gmmbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ogs_assert(n1smbuf);
    ogs_assert(n2smbuf);

    gmmbuf = gmm_build_dl_nas_transport(sess,
            OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, n1smbuf, 0, 0);
    ogs_expect_or_return(gmmbuf);

    ngapbuf = ngap_build_pdu_session_resource_release_command(
            sess, gmmbuf, n2smbuf);
    ogs_expect_or_return(ngapbuf);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_gmm_status(amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] 5GMM status", amf_ue->supi);

    gmmbuf = gmm_build_status(amf_ue, cause);
    ogs_expect_or_return(gmmbuf);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);
}

void nas_5gs_send_gmm_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    ogs_assert(amf_ue);

    switch(amf_ue->nas.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        nas_5gs_send_registration_reject(amf_ue, gmm_cause);
        break;
    case OGS_NAS_5GS_SERVICE_REQUEST:
        nas_5gs_send_service_reject(amf_ue, gmm_cause);
        break;
    default:
        ogs_error("Unknown message type [%d]", amf_ue->nas.message_type);
    }
}

static ogs_nas_5gmm_cause_t gmm_cause_from_sbi(int status)
{
    ogs_nas_5gmm_cause_t gmm_cause;

    switch(status) {
    case OGS_SBI_HTTP_STATUS_NOT_FOUND:
        gmm_cause = OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED;
        break;
    case OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT:
        gmm_cause = OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED;
        break;
    case OGS_SBI_HTTP_STATUS_BAD_REQUEST:
        gmm_cause = OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE;
        break;
    case OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR:
        gmm_cause =
            OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK;
        break;
    default:
        gmm_cause = OGS_5GMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED;
    }

    return gmm_cause;
}

void nas_5gs_send_gmm_reject_from_sbi(amf_ue_t *amf_ue, int status)
{
    ogs_assert(amf_ue);
    nas_5gs_send_gmm_reject(amf_ue, gmm_cause_from_sbi(status));
}

void nas_5gs_send_gsm_reject(amf_sess_t *sess,
        uint8_t payload_container_type, ogs_pkbuf_t *payload_container,
        ogs_nas_5gmm_cause_t cause, uint8_t backoff_time)
{
    int rv;

    ogs_pkbuf_t *gmmbuf = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    ogs_assert(payload_container_type);
    ogs_assert(payload_container);

    ogs_warn("[%s] 5GSM reject", amf_ue->suci);

    gmmbuf = gmm_build_dl_nas_transport(sess,
            payload_container_type, payload_container, cause, backoff_time);
    ogs_expect_or_return(gmmbuf);
    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect_or_return(rv == OGS_OK);
}

void nas_5gs_send_gsm_reject_from_sbi(amf_sess_t *sess,
        uint8_t payload_container_type, ogs_pkbuf_t *payload_container,
        int status)
{
    ogs_assert(sess);
    ogs_assert(payload_container_type);
    ogs_assert(payload_container);

    nas_5gs_send_gsm_reject(sess, payload_container_type, payload_container,
            gmm_cause_from_sbi(status), AMF_NAS_BACKOFF_TIME);
}

void nas_5gs_send_back_5gsm_message(
        amf_sess_t *sess, ogs_nas_5gmm_cause_t cause)
{
    ogs_pkbuf_t *pbuf = NULL;

    ogs_assert(sess);
    ogs_assert(sess->payload_container_type);
    ogs_assert(sess->payload_container);

    pbuf = ogs_pkbuf_copy(sess->payload_container);
    ogs_assert(pbuf);

    nas_5gs_send_gsm_reject(sess, sess->payload_container_type, pbuf,
            cause, AMF_NAS_BACKOFF_TIME);
}

void nas_5gs_send_back_5gsm_message_from_sbi(amf_sess_t *sess, int status)
{
    ogs_assert(sess);
    nas_5gs_send_back_5gsm_message(sess, gmm_cause_from_sbi(status));
}
