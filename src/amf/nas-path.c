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
    int rv;
    ogs_assert(pkbuf);

    amf_ue = amf_ue_cycle(amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf-ue) context has already been removed");
        ogs_pkbuf_free(pkbuf);
        return OGS_NOTFOUND;
    }

    rv = ngap_send_to_ran_ue(amf_ue->ran_ue, pkbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_to_downlink_nas_transport(
        ran_ue_t *ran_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(ran_ue_cycle(ran_ue));
    ogs_assert(pkbuf);

    ngapbuf = ngap_build_downlink_nas_transport(ran_ue, pkbuf, false, false);
    if (!ngapbuf) {
        ogs_error("ngap_build_downlink_nas_transport() failed");
        return OGS_ERROR;
    }

    rv = ngap_send_to_ran_ue(ran_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_registration_accept(amf_ue_t *amf_ue)
{
    int rv;
    bool transfer_needed = false;

    ran_ue_t *ran_ue = NULL;

    ogs_pkbuf_t *ngapbuf = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (!ran_ue) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Registration accept", amf_ue->supi);

    if (amf_ue->next.m_tmsi) {
        if (amf_ue->t3550.pkbuf) {
            gmmbuf = amf_ue->t3550.pkbuf;
        } else {
            gmmbuf = gmm_build_registration_accept(amf_ue);
            if (!gmmbuf) {
                ogs_error("gmm_build_registration_accept() failed");
                return OGS_ERROR;
            }
        }

        amf_ue->t3550.pkbuf = ogs_pkbuf_copy(gmmbuf);
        if (!amf_ue->t3550.pkbuf) {
            ogs_error("ogs_pkbuf_copy(amf_ue->t3550.pkbuf) failed");
            ogs_pkbuf_free(gmmbuf);
            return OGS_ERROR;
        }
        ogs_timer_start(amf_ue->t3550.timer,
                amf_timer_cfg(AMF_TIMER_T3550)->duration);
    } else {
        gmmbuf = gmm_build_registration_accept(amf_ue);
        if (!gmmbuf) {
            ogs_error("gmm_build_registration_accept() failed");
            return OGS_ERROR;
        }
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
        if (!ngapbuf) {
            ogs_error("ngap_ue_build_initial_context_setup_request() failed");
            return OGS_ERROR;
        }

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);

        ran_ue->initial_context_setup_request_sent = true;
    } else {
        if (transfer_needed == true) {
            ngapbuf = ngap_ue_build_pdu_session_resource_setup_request(
                    amf_ue, gmmbuf);
            if (!ngapbuf) {
                ogs_error("ngap_ue_build_pdu_session_resource_setup_request()"
                        " failed");
                return OGS_ERROR;
            }

            rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
            ogs_expect(rv == OGS_OK);
        } else {
            ngapbuf = ngap_build_downlink_nas_transport(
                    ran_ue, gmmbuf, true, true);
            if (!ngapbuf) {
                ogs_error("ngap_build_downlink_nas_transport() failed");
                return OGS_ERROR;
            }

            rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
            ogs_expect(rv == OGS_OK);
        }
    }

    return rv;
}

int nas_5gs_send_registration_reject(
        ran_ue_t *ran_ue, amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(ran_ue_cycle(ran_ue));
    ogs_assert(amf_ue_cycle(amf_ue));

    switch (amf_ue->nas.registration.value) {
    case OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL:
        amf_metrics_inst_by_cause_add(gmm_cause,
                AMF_METR_CTR_RM_REG_INIT_FAIL, 1);
        break;
    case OGS_NAS_5GS_REGISTRATION_TYPE_MOBILITY_UPDATING:
        amf_metrics_inst_by_cause_add(gmm_cause,
                AMF_METR_CTR_RM_REG_MOB_FAIL, 1);
        break;
    case OGS_NAS_5GS_REGISTRATION_TYPE_PERIODIC_UPDATING:
        amf_metrics_inst_by_cause_add(gmm_cause,
                AMF_METR_CTR_RM_REG_PERIOD_FAIL, 1);
        break;
    case OGS_NAS_5GS_REGISTRATION_TYPE_EMERGENCY:
        amf_metrics_inst_by_cause_add(gmm_cause,
                AMF_METR_CTR_RM_REG_EMERG_FAIL, 1);
        break;
    default:
        ogs_error("Unknown reg_type[%d]",
                amf_ue->nas.registration.value);
    }

    ogs_warn("[%s] Registration reject [%d]", amf_ue->suci, gmm_cause);

    gmmbuf = gmm_build_registration_reject(amf_ue, gmm_cause);
    if (!gmmbuf) {
        ogs_error("gmm_build_registration_reject() failed");
        return OGS_ERROR;
    }

    rv = nas_5gs_send_to_downlink_nas_transport(ran_ue, gmmbuf);
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

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (!ran_ue) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Service accept", amf_ue->supi);

    gmmbuf = gmm_build_service_accept(amf_ue);
    if (!gmmbuf) {
        ogs_error("gmm_build_registration_reject() failed");
        return OGS_ERROR;
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
        if (!ngapbuf) {
            ogs_error("ngap_ue_build_initial_context_setup_request() failed");
            return OGS_ERROR;
        }

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);

        ran_ue->initial_context_setup_request_sent = true;
    } else {
        if (transfer_needed == true) {
            ngapbuf = ngap_ue_build_pdu_session_resource_setup_request(
                    amf_ue, gmmbuf);
            if (!ngapbuf) {
                ogs_error("ngap_ue_build_pdu_session_resource_setup_request()"
                        " failed");
                return OGS_ERROR;
            }

            rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
            ogs_expect(rv == OGS_OK);
        } else {
            rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
            ogs_expect(rv == OGS_OK);
        }
    }

    return rv;
}

int nas_5gs_send_service_reject(
        ran_ue_t *ran_ue, amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_assert(ran_ue_cycle(ran_ue));
    ogs_assert(amf_ue_cycle(amf_ue));

    ogs_debug("[%s] Service reject", amf_ue->supi);

    gmmbuf = gmm_build_service_reject(amf_ue, gmm_cause);
    if (!gmmbuf) {
        ogs_error("gmm_build_registration_reject() failed");
        return OGS_ERROR;
    }

    rv = nas_5gs_send_to_downlink_nas_transport(ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_de_registration_accept(amf_ue_t *amf_ue)
{
    int rv;

    ran_ue_t *ran_ue = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (!ran_ue) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] De-registration accept", amf_ue->supi);

    if (amf_ue->nas.de_registration.switch_off == 0) {
        int rv;

        gmmbuf = gmm_build_de_registration_accept(amf_ue);
        if (!gmmbuf) {
            ogs_error("gmm_build_registration_reject() failed");
            return OGS_ERROR;
        }

        rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
        if (rv != OGS_OK) {
            ogs_error("nas_5gs_send_to_downlink_nas_transport() failed");
            return rv;
        }
    }

    rv = ngap_send_ran_ue_context_release_command(ran_ue,
            NGAP_Cause_PR_nas, NGAP_CauseNas_deregister,
            NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_de_registration_request(
        amf_ue_t *amf_ue,
        OpenAPI_deregistration_reason_e dereg_reason,
        ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;

    ran_ue_t *ran_ue = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (!ran_ue) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] De-registration request", amf_ue->supi);

    if (amf_ue->t3522.pkbuf) {
        gmmbuf = amf_ue->t3522.pkbuf;
    } else {
        gmmbuf = gmm_build_de_registration_request(
                amf_ue, dereg_reason, gmm_cause);
        if (!gmmbuf) {
            ogs_error("gmm_build_de_registration_request() failed");
            return OGS_ERROR;
        }
    }

    amf_ue->t3522.pkbuf = ogs_pkbuf_copy(gmmbuf);
    if (!amf_ue->t3522.pkbuf) {
        ogs_error("ogs_pkbuf_copy(amf_ue->t3522.pkbuf) failed");
        ogs_pkbuf_free(gmmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(amf_ue->t3522.timer,
            amf_timer_cfg(AMF_TIMER_T3522)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_identity_request(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!ran_ue_cycle(amf_ue->ran_ue)) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("Identity request");

    if (amf_ue->t3570.pkbuf) {
        gmmbuf = amf_ue->t3570.pkbuf;
    } else {
        gmmbuf = gmm_build_identity_request(amf_ue);
        if (!gmmbuf) {
            ogs_error("gmm_build_identity_request() failed");
            return OGS_ERROR;
        }
    }

    amf_ue->t3570.pkbuf = ogs_pkbuf_copy(gmmbuf);
    if (!amf_ue->t3570.pkbuf) {
        ogs_error("ogs_pkbuf_copy(amf_ue->t3570.pkbuf) failed");
        ogs_pkbuf_free(gmmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(amf_ue->t3570.timer,
            amf_timer_cfg(AMF_TIMER_T3570)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_authentication_request(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!ran_ue_cycle(amf_ue->ran_ue)) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Authentication request", amf_ue->suci);

    if (amf_ue->t3560.pkbuf) {
        gmmbuf = amf_ue->t3560.pkbuf;
    } else {
        gmmbuf = gmm_build_authentication_request(amf_ue);
        if (!gmmbuf) {
            ogs_error("gmm_build_authentication_request() failed");
            return OGS_ERROR;
        }
    }

    amf_ue->t3560.pkbuf = ogs_pkbuf_copy(gmmbuf);
    if (!amf_ue->t3560.pkbuf) {
        ogs_error("ogs_pkbuf_copy(amf_ue->t3560.pkbuf) failed");
        ogs_pkbuf_free(gmmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(amf_ue->t3560.timer,
            amf_timer_cfg(AMF_TIMER_T3560)->duration);

    amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_AMF_AUTH_REQ);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_authentication_reject(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!ran_ue_cycle(amf_ue->ran_ue)) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_warn("[%s] Authentication reject", amf_ue->suci);

    gmmbuf = gmm_build_authentication_reject();
    if (!gmmbuf) {
        ogs_error("gmm_build_authentication_reject() failed");
        return OGS_ERROR;
    }

    amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_AMF_AUTH_REJECT);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_security_mode_command(amf_ue_t *amf_ue)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!ran_ue_cycle(amf_ue->ran_ue)) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Security mode command", amf_ue->supi);

    if (amf_ue->t3560.pkbuf) {
        gmmbuf = amf_ue->t3560.pkbuf;
    } else {
        gmmbuf = gmm_build_security_mode_command(amf_ue);
        if (!gmmbuf) {
            ogs_error("gmm_build_security_mode_command() failed");
            return OGS_ERROR;
        }
    }

    amf_ue->t3560.pkbuf = ogs_pkbuf_copy(gmmbuf);
    if (!amf_ue->t3560.pkbuf) {
        ogs_error("ogs_pkbuf_copy(amf_ue->t3560.pkbuf) failed");
        ogs_pkbuf_free(gmmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(amf_ue->t3560.timer,
            amf_timer_cfg(AMF_TIMER_T3560)->duration);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_configuration_update_command(
        amf_ue_t *amf_ue, gmm_configuration_update_command_param_t *param)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!ran_ue_cycle(amf_ue->ran_ue)) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_info("[%s] Configuration update command", amf_ue->supi);

    if (amf_ue->t3555.pkbuf) {
        gmmbuf = amf_ue->t3555.pkbuf;

        amf_ue->t3555.pkbuf = ogs_pkbuf_copy(gmmbuf);
        if (!amf_ue->t3555.pkbuf) {
            ogs_error("ogs_pkbuf_copy(amf_ue->t3555.pkbuf) failed");
            ogs_pkbuf_free(gmmbuf);
            return OGS_ERROR;
        }
        ogs_timer_start(amf_ue->t3555.timer,
                amf_timer_cfg(AMF_TIMER_T3555)->duration);

    } else {
        if (!param) {
            ogs_error("No param");
            return OGS_ERROR;
        }
        gmmbuf = gmm_build_configuration_update_command(amf_ue, param);
        if (!gmmbuf) {
            ogs_error("gmm_build_configuration_update_command() failed");
            return OGS_ERROR;
        }

        if (param->acknowledgement_requested) {
            amf_ue->t3555.pkbuf = ogs_pkbuf_copy(gmmbuf);
            if (!amf_ue->t3555.pkbuf) {
                ogs_error("ogs_pkbuf_copy(amf_ue->t3555.pkbuf) failed");
                ogs_pkbuf_free(gmmbuf);
                return OGS_ERROR;
            }
            ogs_timer_start(amf_ue->t3555.timer,
                    amf_timer_cfg(AMF_TIMER_T3555)->duration);
        }
    }

    amf_metrics_inst_global_inc(AMF_METR_GLOB_CTR_MM_CONF_UPDATE);

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_send_pdu_session_setup_request(amf_sess_t *sess,
        ogs_pkbuf_t *n1smbuf, ogs_pkbuf_t *n2smbuf)
{
    int rv;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_pkbuf_t *gmmbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(sess);
    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf-ue) context has already been removed");
        if (n1smbuf) ogs_pkbuf_free(n1smbuf);
        ogs_pkbuf_free(n2smbuf);
        return OGS_NOTFOUND;
    }
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (!ran_ue) {
        ogs_warn("NG context has already been removed");
        if (n1smbuf) ogs_pkbuf_free(n1smbuf);
        ogs_pkbuf_free(n2smbuf);
        return OGS_NOTFOUND;
    }

    if (n1smbuf) {
        gmmbuf = gmm_build_dl_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, n1smbuf, 0, 0);
        if (!gmmbuf) {
            ogs_error("gmm_build_dl_nas_transport() failed");
            return OGS_ERROR;
        }
    }

    if (ran_ue->ue_context_requested == true &&
        ran_ue->initial_context_setup_request_sent == false) {
        ngapbuf = ngap_sess_build_initial_context_setup_request(
                ran_ue, sess, gmmbuf, n2smbuf);
        if (!ngapbuf) {
            ogs_error("ngap_sess_build_initial_context_setup_request() failed");
            return OGS_ERROR;
        }

        rv = ngap_send_to_ran_ue(ran_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);

        ran_ue->initial_context_setup_request_sent = true;
    } else {
        ngapbuf = ngap_sess_build_pdu_session_resource_setup_request(
                ran_ue, sess, gmmbuf, n2smbuf);
        if (!ngapbuf) {
            ogs_error("ngap_sess_build_initial_context_setup_request() failed");
            return OGS_ERROR;
        }

        rv = ngap_send_to_ran_ue(ran_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);
    }

    return rv;
}

int nas_send_pdu_session_modification_command(amf_sess_t *sess,
        ogs_pkbuf_t *n1smbuf, ogs_pkbuf_t *n2smbuf)
{
    int rv;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_pkbuf_t *gmmbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(sess);
    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf-ue) context has already been removed");
        if (n1smbuf) ogs_pkbuf_free(n1smbuf);
        ogs_pkbuf_free(n2smbuf);
        return OGS_NOTFOUND;
    }
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (!ran_ue) {
        ogs_warn("[%s] NG context has already been removed", amf_ue->supi);
        if (n1smbuf) ogs_pkbuf_free(n1smbuf);
        ogs_pkbuf_free(n2smbuf);
        return OGS_NOTFOUND;
    }

    if (n1smbuf) {
        gmmbuf = gmm_build_dl_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, n1smbuf, 0, 0);
        if (!gmmbuf) {
            ogs_error("gmm_build_dl_nas_transport() failed");
            return OGS_ERROR;
        }
    }

    /*
     * Issues #1925
     *
     * We should not check an activated PSI mask to send DownlinkNASTransport.
     *
     * PDUSessionResourceModifyRequest needs to activate QoSFlow with
     * AddOrModifyQosFlow in the message.
     *
     * So, we should always use PDUSessionResourceModifyRequest instead of
     * send with DownlinkNASTransport.
     */
    ngapbuf = ngap_build_pdu_session_resource_modify_request(
            sess, gmmbuf, n2smbuf);
    if (!ngapbuf) {
        ogs_error("ngap_build_pdu_session_resource_modify_request() failed");
        return OGS_ERROR;
    }

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_send_pdu_session_release_command(amf_sess_t *sess,
        ogs_pkbuf_t *n1smbuf, ogs_pkbuf_t *n2smbuf)
{
    int rv;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_pkbuf_t *gmmbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(n2smbuf);
    ogs_assert(sess);
    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf-ue) context has already been removed");
        if (n1smbuf) ogs_pkbuf_free(n1smbuf);
        ogs_pkbuf_free(n2smbuf);
        return OGS_NOTFOUND;
    }
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (!ran_ue) {
        ogs_warn("NG context has already been removed");
        if (n1smbuf) ogs_pkbuf_free(n1smbuf);
        ogs_pkbuf_free(n2smbuf);
        return OGS_NOTFOUND;
    }

    if (n1smbuf) {
        gmmbuf = gmm_build_dl_nas_transport(sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, n1smbuf, 0, 0);
        if (!gmmbuf) {
            ogs_error("gmm_build_dl_nas_transport() failed");
            return OGS_ERROR;
        }
    }

    /*
     * Issues #1925
     *
     * We should CHECK an activated PSI mask to send DownlinkNASTransport.
     *
     * - RAN removed the PDU Session Resource in the following process..
     * 1. UEContextReleaseRequest
     * 2. UEContextReleaseCommand
     * 3. UEContextReleaseComplete.
     *
     * - If Service Request has no UpdateDataStatus while waking up UE,
     * 1. ServiceRequest
     * 2. InitialContextSetupRequest
     * 3. InitialContextSetupResponse
     *
     * - In this case, we should use the DownlinkNASTransport.
     *   instead of PDUSessionResourceReleaseCommand
     */
    if (ran_ue->psimask.activated & (1 << sess->psi)) {
        ngapbuf = ngap_build_pdu_session_resource_release_command(
                sess, gmmbuf, n2smbuf);
        if (!ngapbuf) {
            ogs_error(
                    "ngap_build_pdu_session_resource_release_command() failed");
            return OGS_ERROR;
        }

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);
    } else if (gmmbuf) {
        ogs_pkbuf_free(n2smbuf);
        ngapbuf = ngap_build_downlink_nas_transport(
                ran_ue, gmmbuf, false, false);
        if (!ngapbuf) {
            ogs_error("ngap_build_downlink_nas_transport() failed");
            return OGS_ERROR;
        }

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect(rv == OGS_OK);
    } else {
        /*
         * TS23.527
         * 5.3 Restoration Procedures upon Loss of GTP-U contexts
         * 5.3.2 Procedure for GTP-U Error Indication received from 5G-AN
         *
         * 1. gNB sent TP-U Error Indication to the UPF.
         * 2. UPF sent PFCP Report to SMF.
         * 3. SMF sent N1N2Transfer to AMF to release PDU Session
         * 4. In AMF, there is no Activated Session.
         *
         * Since AMF does not yet have an activated PDU Session,
         * it ignores it without requesting PDU release from gNB.
         */
        ogs_error("No session and N1 message");
        ogs_error("    ACTIVATED[0x%x] N1[%p], SUPI[%s] PSI[%d]",
                ran_ue->psimask.activated, gmmbuf, amf_ue->supi, sess->psi);

        ogs_pkbuf_free(n2smbuf);
        rv = OGS_OK;
    }

    return rv;
}

int nas_5gs_send_gmm_status(amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t cause)
{
    int rv;
    ogs_pkbuf_t *gmmbuf = NULL;

    if (!amf_ue_cycle(amf_ue)) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!ran_ue_cycle(amf_ue->ran_ue)) {
        ogs_error("NG context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] 5GMM status", amf_ue->supi);

    gmmbuf = gmm_build_status(amf_ue, cause);
    if (!gmmbuf) {
        ogs_error("gmm_build_status() failed");
        return OGS_ERROR;
    }

    rv = nas_5gs_send_to_downlink_nas_transport(amf_ue->ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_gmm_reject(
        ran_ue_t *ran_ue, amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    int rv;

    amf_ue = amf_ue_cycle(amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ran_ue = ran_ue_cycle(ran_ue);
    if (!ran_ue) {
        ogs_error("[%s] NG context has already been removed", amf_ue->supi);
        return OGS_NOTFOUND;
    }

    switch(amf_ue->nas.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        rv = nas_5gs_send_registration_reject(ran_ue, amf_ue, gmm_cause);
        ogs_expect(rv == OGS_OK);
        break;
    case OGS_NAS_5GS_SERVICE_REQUEST:
        rv = nas_5gs_send_service_reject(ran_ue, amf_ue, gmm_cause);
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

    rv = nas_5gs_send_gmm_reject(
            amf_ue->ran_ue, amf_ue, gmm_cause_from_sbi(status));
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_dl_nas_transport(ran_ue_t *ran_ue, amf_sess_t *sess,
        uint8_t payload_container_type, ogs_pkbuf_t *payload_container,
        ogs_nas_5gmm_cause_t cause, uint8_t backoff_time)
{
    int rv;

    ogs_pkbuf_t *gmmbuf = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(sess);
    sess = amf_sess_cycle(sess);
    if (!sess) {
        ogs_error("Session has already been removed");
        return OGS_NOTFOUND;
    }

    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ran_ue = ran_ue_cycle(ran_ue);
    if (!ran_ue) {
        ogs_error("[%s] NG context has already been removed", amf_ue->supi);
        return OGS_NOTFOUND;
    }

    ogs_assert(payload_container_type);
    ogs_assert(payload_container);

    ogs_warn("[%s] DL NAS transport", amf_ue->suci);

    gmmbuf = gmm_build_dl_nas_transport(sess,
            payload_container_type, payload_container, cause, backoff_time);
    if (!gmmbuf) {
        ogs_error("gmm_build_dl_nas_transport() failed");
        return OGS_ERROR;
    }
    rv = nas_5gs_send_to_downlink_nas_transport(ran_ue, gmmbuf);
    ogs_expect(rv == OGS_OK);

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
int nas_5gs_send_gsm_reject(ran_ue_t *ran_ue, amf_sess_t *sess,
        uint8_t payload_container_type, ogs_pkbuf_t *payload_container)
{
    int rv;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(sess);
    sess = amf_sess_cycle(sess);
    if (!sess) {
        ogs_error("Session has already been removed");
        return OGS_NOTFOUND;
    }

    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ran_ue = ran_ue_cycle(ran_ue);
    if (!ran_ue) {
        ogs_error("[%s] NG context has already been removed", amf_ue->supi);
        return OGS_NOTFOUND;
    }

    ogs_assert(payload_container_type);
    ogs_assert(payload_container);

    rv = nas_5gs_send_dl_nas_transport(
            ran_ue, sess, payload_container_type, payload_container, 0, 0);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_5gs_send_back_gsm_message(
        ran_ue_t *ran_ue, amf_sess_t *sess,
        ogs_nas_5gmm_cause_t cause, uint8_t backoff_time)
{
    int rv;
    ogs_pkbuf_t *pbuf = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(sess);
    sess = amf_sess_cycle(sess);
    if (!sess) {
        ogs_error("Session has already been removed");
        return OGS_NOTFOUND;
    }

    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ran_ue = ran_ue_cycle(ran_ue);
    if (!ran_ue) {
        ogs_error("[%s] NG context has already been removed", amf_ue->supi);
        return OGS_NOTFOUND;
    }

    ogs_assert(sess->payload_container_type);
    ogs_assert(sess->payload_container);

    pbuf = ogs_pkbuf_copy(sess->payload_container);
    if (!pbuf) {
        ogs_error("ogs_pkbuf_copy(pbuf) failed");
        return OGS_ERROR;
    }

    rv = nas_5gs_send_dl_nas_transport(
            ran_ue, sess, sess->payload_container_type, pbuf,
            cause, backoff_time);
    ogs_expect(rv == OGS_OK);

    return rv;
}
