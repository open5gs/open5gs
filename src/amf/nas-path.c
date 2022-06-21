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
    ogs_expect_or_return_val(ran_ue, OGS_ERROR);

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

        return OGS_ERROR;
    } else {
        ngapbuf = ngap_build_downlink_nas_transport(
                ran_ue, pkbuf, false, false);
        ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);

        return rv;
    }
}

int nas_5gs_send_registration_accept(amf_ue_t *amf_ue)
{
    int rv;
    bool transfer_needed = false;

    ran_ue_t *ran_ue = NULL;

    ogs_pkbuf_t *ngapbuf = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_expect_or_return_val(ran_ue, OGS_ERROR);

    ogs_debug("[%s] Registration accept", amf_ue->supi);

    if (amf_ue->next.m_tmsi) {
        if (amf_ue->t3550.pkbuf) {
            gmmbuf = amf_ue->t3550.pkbuf;
            ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
        } else {
            gmmbuf = gmm_build_registration_accept(amf_ue);
            ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
        }

        amf_ue->t3550.pkbuf = ogs_pkbuf_copy(gmmbuf);
        ogs_expect_or_return_val(amf_ue->t3550.pkbuf, OGS_ERROR);
        ogs_timer_start(amf_ue->t3550.timer,
                amf_timer_cfg(AMF_TIMER_T3550)->duration);
    } else {
        gmmbuf = gmm_build_registration_accept(amf_ue);
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    }

    /*
     * Previously, AMF would sends PDUSessionResourceSetupRequest
     * when the following conditions were met:
     * - gNB didn't send UE Context Request IE of InitialUEMessage
     * - AMF should send SMF generated TRANSFER message(PDU_RES_SETUP_REQ)
     *   to the gNB
     *
     * However, in issues #771, the gNB did not accept
     * PDUSessionResourceSetupRequest. Perhaps the gNB engineer thought
     * that if gNB needs to send data traffic to the UE, AMF should send
     * an InitialContextSetupRequest regardless of UE Context Request IE.
     * This is because gNB requires the kgNB security context
     * for data connection.
     *
     * So, in this case, Open5GS-AMF decided to send
     * an InitialContexSetupRequest regardless of
     * whether it received UE Context Request IE of InitialUEMessage.
     */
    transfer_needed = PDU_RES_SETUP_REQ_TRANSFER_NEEDED(amf_ue);

    if (ran_ue->initial_context_setup_request_sent == false &&
        (ran_ue->ue_context_requested == true || transfer_needed == true)) {
        ngapbuf = ngap_ue_build_initial_context_setup_request(amf_ue, gmmbuf);
        ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

        ran_ue->initial_context_setup_request_sent = true;
    } else {
        if (transfer_needed == true) {
            ngapbuf = ngap_ue_build_pdu_session_resource_setup_request(
                    amf_ue, gmmbuf);
            ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

            rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
            ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);
        } else {
            ngapbuf = ngap_build_downlink_nas_transport(
                    ran_ue, gmmbuf, true, true);
            ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

            rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
            ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);
        }
    }

    return OGS_OK;
}

int nas_5gs_send_registration_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_warn("[%s] Registration reject [%d]", amf_ue->suci, gmm_cause);

    gmmbuf = gmm_build_registration_reject(gmm_cause);
    ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_service_accept(amf_ue_t *amf_ue)
{
    int rv;
    bool transfer_needed = false;
    ran_ue_t *ran_ue = NULL;

    ogs_pkbuf_t *gmmbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_expect_or_return_val(ran_ue, OGS_ERROR);

    ogs_debug("[%s] Service accept", amf_ue->supi);

    gmmbuf = gmm_build_service_accept(amf_ue);
    ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

    /*
     * Previously, AMF would sends PDUSessionResourceSetupRequest
     * when the following conditions were met:
     * - gNB didn't send UE Context Request IE of InitialUEMessage
     * - AMF should send SMF generated TRANSFER message(PDU_RES_SETUP_REQ)
     *   to the gNB
     *
     * However, in issues #771, the gNB did not accept
     * PDUSessionResourceSetupRequest. Perhaps the gNB engineer thought
     * that if gNB needs to send data traffic to the UE, AMF should send
     * an InitialContextSetupRequest regardless of UE Context Request IE.
     * This is because gNB requires the kgNB security context
     * for data connection.
     *
     * So, in this case, Open5GS-AMF decided to send
     * an InitialContexSetupRequest regardless of
     * whether it received UE Context Request IE of InitialUEMessage.
     */
    transfer_needed = PDU_RES_SETUP_REQ_TRANSFER_NEEDED(amf_ue);

    if (ran_ue->initial_context_setup_request_sent == false &&
        (ran_ue->ue_context_requested == true || transfer_needed == true)) {
        ngapbuf = ngap_ue_build_initial_context_setup_request(amf_ue, gmmbuf);
        ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

        ran_ue->initial_context_setup_request_sent = true;
    } else {
        if (transfer_needed == true) {
            ngapbuf = ngap_ue_build_pdu_session_resource_setup_request(
                    amf_ue, gmmbuf);
            ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

            rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
            ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);
        } else {
            rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
            ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);
        }
    }

    return OGS_OK;
}

int nas_5gs_send_service_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] Service reject", amf_ue->supi);

    gmmbuf = gmm_build_service_reject(amf_ue, gmm_cause);
    ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_de_registration_accept(amf_ue_t *amf_ue)
{
    int rv;

    ran_ue_t *ran_ue = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_expect_or_return_val(ran_ue, OGS_ERROR);

    ogs_debug("[%s] De-registration accept", amf_ue->supi);

    if (amf_ue->nas.de_registration.switch_off == 0) {
        int rv;

        gmmbuf = gmm_build_de_registration_accept(amf_ue);
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

        rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
        ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);
    }

    rv = ngap_send_ran_ue_context_release_command(ran_ue,
            NGAP_Cause_PR_nas, NGAP_CauseNas_deregister,
            NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_de_registration_request(amf_ue_t *amf_ue)
{
    int rv;

    ran_ue_t *ran_ue = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_expect_or_return_val(ran_ue, OGS_ERROR);

    ogs_debug("[%s] De-registration request", amf_ue->supi);

    if (amf_ue->t3522.pkbuf) {
        gmmbuf = amf_ue->t3522.pkbuf;
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    } else {
        gmmbuf = gmm_build_de_registration_request(amf_ue);
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    }

    amf_ue->t3522.pkbuf = ogs_pkbuf_copy(gmmbuf);
    ogs_expect_or_return_val(amf_ue->t3522.pkbuf, OGS_ERROR);
    ogs_timer_start(amf_ue->t3522.timer,
            amf_timer_cfg(AMF_TIMER_T3522)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

    return rv;
}

int nas_5gs_send_identity_request(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("Identity request");

    if (amf_ue->t3570.pkbuf) {
        gmmbuf = amf_ue->t3570.pkbuf;
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    } else {
        gmmbuf = gmm_build_identity_request(amf_ue);
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    }

    amf_ue->t3570.pkbuf = ogs_pkbuf_copy(gmmbuf);
    ogs_expect_or_return_val(amf_ue->t3570.pkbuf, OGS_ERROR);
    ogs_timer_start(amf_ue->t3570.timer,
            amf_timer_cfg(AMF_TIMER_T3570)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_authentication_request(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] Authentication request", amf_ue->suci);

    if (amf_ue->t3560.pkbuf) {
        gmmbuf = amf_ue->t3560.pkbuf;
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    } else {
        gmmbuf = gmm_build_authentication_request(amf_ue);
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    }

    amf_ue->t3560.pkbuf = ogs_pkbuf_copy(gmmbuf);
    ogs_expect_or_return_val(amf_ue->t3560.pkbuf, OGS_ERROR);
    ogs_timer_start(amf_ue->t3560.timer,
            amf_timer_cfg(AMF_TIMER_T3560)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_authentication_reject(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_warn("[%s] Authentication reject", amf_ue->suci);

    gmmbuf = gmm_build_authentication_reject();
    ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_security_mode_command(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] Security mode command", amf_ue->supi);

    if (amf_ue->t3560.pkbuf) {
        gmmbuf = amf_ue->t3560.pkbuf;
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    } else {
        gmmbuf = gmm_build_security_mode_command(amf_ue);
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    }

    amf_ue->t3560.pkbuf = ogs_pkbuf_copy(gmmbuf);
    ogs_expect_or_return_val(amf_ue->t3560.pkbuf, OGS_ERROR);
    ogs_timer_start(amf_ue->t3560.timer,
            amf_timer_cfg(AMF_TIMER_T3560)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_configuration_update_command(
        amf_ue_t *amf_ue, gmm_configuration_update_command_param_t *param)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_info("[%s] Configuration update command", amf_ue->supi);

    if (amf_ue->t3555.pkbuf) {
        gmmbuf = amf_ue->t3555.pkbuf;
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

        amf_ue->t3555.pkbuf = ogs_pkbuf_copy(gmmbuf);
        ogs_expect_or_return_val(amf_ue->t3555.pkbuf, OGS_ERROR);
        ogs_timer_start(amf_ue->t3555.timer,
                amf_timer_cfg(AMF_TIMER_T3555)->duration);

    } else {
        ogs_expect_or_return_val(param, OGS_ERROR);
        gmmbuf = gmm_build_configuration_update_command(amf_ue, param);
        ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

        if (param->acknowledgement_requested) {
            amf_ue->t3555.pkbuf = ogs_pkbuf_copy(gmmbuf);
            ogs_expect_or_return_val(amf_ue->t3555.pkbuf, OGS_ERROR);
            ogs_timer_start(amf_ue->t3555.timer,
                    amf_timer_cfg(AMF_TIMER_T3555)->duration);
        }
    }

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_send_pdu_session_modification_command(amf_sess_t *sess,
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
    ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

    ngapbuf = ngap_build_pdu_session_resource_modify_request(
            sess, gmmbuf, n2smbuf);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}


int nas_send_pdu_session_release_command(amf_sess_t *sess,
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
    ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

    ngapbuf = ngap_build_pdu_session_resource_release_command(
            sess, gmmbuf, n2smbuf);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_gmm_status(amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(amf_ue);

    ogs_debug("[%s] 5GMM status", amf_ue->supi);

    gmmbuf = gmm_build_status(amf_ue, cause);
    ogs_expect_or_return_val(gmmbuf, OGS_ERROR);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_gmm_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;
    ogs_assert(amf_ue);

    switch(amf_ue->nas.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        rv = nas_5gs_send_registration_reject(amf_ue, gmm_cause);
        ogs_expect(rv == OGS_OK);
        break;
    case OGS_NAS_5GS_SERVICE_REQUEST:
        rv = nas_5gs_send_service_reject(amf_ue, gmm_cause);
        ogs_expect(rv == OGS_OK);
        break;
    default:
        ogs_error("Unknown message type [%d]", amf_ue->nas.message_type);
        rv = OGS_ERROR;
    }

    return rv;
}

static ogs_nas_5gmm_cause_t gmm_cause_from_sbi(int status)
{
    ogs_nas_5gmm_cause_t gmm_cause;

    switch(status) {
    case OGS_SBI_HTTP_STATUS_NOT_FOUND:
        gmm_cause = OGS_5GMM_CAUSE_PLMN_NOT_ALLOWED;
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

int nas_5gs_send_gmm_reject_from_sbi(amf_ue_t *amf_ue, int status)
{
    int rv;

    ogs_assert(amf_ue);
    rv = nas_5gs_send_gmm_reject(amf_ue, gmm_cause_from_sbi(status));
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_dl_nas_transport(amf_sess_t *sess,
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

    ogs_warn("[%s] DL NAS transport", amf_ue->suci);

    gmmbuf = gmm_build_dl_nas_transport(sess,
            payload_container_type, payload_container, cause, backoff_time);
    ogs_expect_or_return_val(gmmbuf, OGS_ERROR);
    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue, gmmbuf);
    ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

    return rv;
}

/*
 * TS24.501
 * 8.2.11 DL NAS transport
 * 8.2.11.4 5GMM cause
 *
 * The AMF shall include this IE when the Payload container IE
 * contains an uplink payload which was not forwarded and
 * the Payload container type IE is not set to "Multiple payloads".
 *
 * -0-
 * As such, this function 'nas_5gs_send_gsm_reject()' must be used
 * only when an N1 SM message has been forwarded to the SMF.
 */
int nas_5gs_send_gsm_reject(amf_sess_t *sess,
        uint8_t payload_container_type, ogs_pkbuf_t *payload_container)
{
    int rv;

    ogs_assert(sess);
    ogs_assert(payload_container_type);
    ogs_assert(payload_container);

    rv = nas_5gs_send_dl_nas_transport(
            sess, payload_container_type, payload_container, 0, 0);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_back_gsm_message(
        amf_sess_t *sess, ogs_nas_5gmm_cause_t cause, uint8_t backoff_time)
{
    int rv;
    ogs_pkbuf_t *pbuf = NULL;

    ogs_assert(sess);
    ogs_assert(sess->payload_container_type);
    ogs_assert(sess->payload_container);

    pbuf = ogs_pkbuf_copy(sess->payload_container);
    ogs_expect_or_return_val(pbuf, OGS_ERROR);

    rv = nas_5gs_send_dl_nas_transport(sess, sess->payload_container_type, pbuf,
            cause, backoff_time);
    ogs_expect(rv == OGS_OK);

    return rv;
}
