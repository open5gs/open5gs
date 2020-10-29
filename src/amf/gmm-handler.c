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

#include "nas-security.h"

#include "ngap-path.h"
#include "nas-path.h"
#include "sbi-path.h"

#include "gmm-handler.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gmm_log_domain

static int gmm_handle_nas_message_container(amf_ue_t *amf_ue,
        ogs_nas_message_container_t *nas_message_container);

int gmm_handle_registration_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_registration_request_t *registration_request)
{
    int served_tai_index = 0;

    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_registration_type_t *registration_type = NULL;
    ogs_nas_5gs_mobile_identity_t *mobile_identity = NULL;
    ogs_nas_5gs_mobile_identity_header_t *mobile_identity_header = NULL;
    ogs_nas_5gs_mobile_identity_guti_t *mobile_identity_guti = NULL;
    ogs_nas_ue_security_capability_t *ue_security_capability = NULL;
    ogs_nas_5gs_guti_t nas_guti;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_assert(registration_request);
    registration_type = &registration_request->registration_type;
    ogs_assert(registration_type);
    mobile_identity = &registration_request->mobile_identity;
    ogs_assert(mobile_identity);
    ue_security_capability = &registration_request->ue_security_capability;
    ogs_assert(ue_security_capability);

    ogs_debug("Registration request");

    if (!mobile_identity->length || !mobile_identity->buffer) {
        ogs_error("No Mobile Identity");
        nas_5gs_send_registration_reject(amf_ue,
            OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE);
        return OGS_ERROR;
    }

    mobile_identity_header =
            (ogs_nas_5gs_mobile_identity_header_t *)mobile_identity->buffer;

    switch (mobile_identity_header->type) {
    case OGS_NAS_5GS_MOBILE_IDENTITY_SUCI:
        amf_ue_set_suci(amf_ue, mobile_identity);
        ogs_debug("[%s]    SUCI", amf_ue->suci);
        break;
    case OGS_NAS_5GS_MOBILE_IDENTITY_GUTI:
        mobile_identity_guti =
            (ogs_nas_5gs_mobile_identity_guti_t *)mobile_identity->buffer;
        if (!mobile_identity_guti) {
            ogs_error("No mobile identity");
            return OGS_ERROR;
        }

        ogs_nas_5gs_mobile_identity_guti_to_nas_guti(
            mobile_identity_guti, &nas_guti);

        ogs_debug("[%s]    5G-S_GUTI[AMF_ID:0x%x,M_TMSI:0x%x]",
            AMF_UE_HAVE_SUCI(amf_ue) ? amf_ue->suci : "Unknown ID",
            ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
        break;
    default:
        ogs_error("Unknown SUCI type [%d]", mobile_identity_header->type);
        break;
    }


    /* Set 5GS Registration Type */
    memcpy(&amf_ue->nas.registration, registration_type,
            sizeof(ogs_nas_5gs_registration_type_t));
    amf_ue->nas.message_type = OGS_NAS_5GS_REGISTRATION_REQUEST;
    ogs_debug("    OGS_NAS_5GS TYPE[%d] TSC[%d] KSI[%d] REGISTRATION[0x%x]",
            amf_ue->nas.message_type,
            amf_ue->nas.tsc, amf_ue->nas.ksi, amf_ue->nas.data);
    /*
     * REGISTRATION_REQUEST
     * SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        ogs_kdf_kgnb_and_kn3iwf(
                amf_ue->kamf, amf_ue->ul_count.i32,
                amf_ue->nas.access_type, amf_ue->kgnb);
        ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->kgnb, amf_ue->nh);
        amf_ue->nhcc = 1;
    }

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac.v);
    ogs_debug("    OLD NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
            ogs_plmn_id_hexdump(&amf_ue->nr_cgi.plmn_id),
            (long long)amf_ue->nr_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.tai.plmn_id),
            ran_ue->saved.tai.tac.v);
    ogs_debug("    NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
            ogs_plmn_id_hexdump(&ran_ue->saved.nr_cgi.plmn_id),
            (long long)ran_ue->saved.nr_cgi.cell_id);

    /* Copy TAI and ECGI from ran_ue */
    memcpy(&amf_ue->tai, &ran_ue->saved.tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->nr_cgi, &ran_ue->saved.nr_cgi, sizeof(ogs_nr_cgi_t));
    amf_ue->ue_location_timestamp = ogs_time_now();

    /* Check TAI */
    served_tai_index = amf_find_served_tai(&amf_ue->tai);
    if (served_tai_index < 0) {
        /* Send Registration Reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac.v);
        nas_5gs_send_registration_reject(amf_ue,
            OGS_5GMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    if (registration_request->presencemask &
            OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT) {
        ogs_nas_5gmm_capability_t *gmm_capability =
            &registration_request->gmm_capability;

        amf_ue->gmm_capability.lte_positioning_protocol_capability
            = gmm_capability->lte_positioning_protocol_capability;
        amf_ue->gmm_capability.ho_attach = gmm_capability->ho_attach;
        amf_ue->gmm_capability.s1_mode = gmm_capability->s1_mode;
            
        ogs_debug("    5GMM Capability:[LPP:%d, HO_ATTACH:%d, S1_MODE:%d]",
            amf_ue->gmm_capability.lte_positioning_protocol_capability,
            amf_ue->gmm_capability.ho_attach,
            amf_ue->gmm_capability.s1_mode);
    }

    if (registration_request->presencemask &
            OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT) {
        memcpy(&amf_ue->ue_security_capability,
                &registration_request->ue_security_capability,
                registration_request->ue_security_capability.length +
                sizeof(registration_request->ue_security_capability.length));
    }

    if (registration_request->presencemask &
            OGS_NAS_5GS_REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&amf_ue->ue_network_capability,
                &registration_request->s1_ue_network_capability,
                registration_request->s1_ue_network_capability.length +
                sizeof(registration_request->s1_ue_network_capability.length));
    }

    if (amf_selected_int_algorithm(amf_ue) ==
            OGS_NAS_SECURITY_ALGORITHMS_NIA0) {
        ogs_error("[UE:0x%x:0x%x], NEA0 can be used in Encrypt[0x%x], "
            "but Integrity[0x%x] cannot be bypassed with NIA0",
            ue_security_capability->nea, ue_security_capability->nia,
            amf_selected_enc_algorithm(amf_ue), 
            amf_selected_int_algorithm(amf_ue));
        nas_5gs_send_registration_reject(amf_ue,
            OGS_5GMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int gmm_handle_registration_update(amf_ue_t *amf_ue,
        ogs_nas_5gs_registration_request_t *registration_request)
{
    amf_sess_t *sess = NULL;
    uint16_t psimask;
    int i = 0;

    ogs_nas_5gs_tracking_area_identity_t *last_visited_registered_tai = NULL;
    ogs_nas_uplink_data_status_t *uplink_data_status = NULL;
    ogs_nas_pdu_session_status_t *pdu_session_status = NULL;

    ogs_assert(amf_ue);
    ogs_assert(registration_request);

    last_visited_registered_tai =
        &registration_request->last_visited_registered_tai;
    ogs_assert(last_visited_registered_tai);
    uplink_data_status = &registration_request->uplink_data_status;
    ogs_assert(uplink_data_status);
    pdu_session_status = &registration_request->pdu_session_status;
    ogs_assert(pdu_session_status);

    if (registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT) {

        return gmm_handle_nas_message_container(
                amf_ue, &registration_request->nas_message_container);
    }

    if (registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NSSAI_PRESENT) {

        amf_ue->num_of_requested_nssai = ogs_nas_parse_nssai(
            amf_ue->requested_nssai, &registration_request->requested_nssai);

        for (i = 0; i < amf_ue->num_of_requested_nssai; i++) {
            if (amf_find_s_nssai(
                    &amf_ue->tai.plmn_id, &amf_ue->requested_nssai[i]))
                break;
        }

        if (i == amf_ue->num_of_requested_nssai) {
            ogs_error("CHECK CONFIGURATION: Cannot find Requested NSSAI");
            for (i = 0; i < amf_ue->num_of_requested_nssai; i++) {
                ogs_error("    PLMN_ID[MCC:%d MNC:%d]",
                        ogs_plmn_id_mcc(&amf_ue->tai.plmn_id),
                        ogs_plmn_id_mnc(&amf_ue->tai.plmn_id));
                ogs_error("    S_NSSAI[SST:%d SD:0x%x]",
                        amf_ue->requested_nssai[i].sst,
                        amf_ue->requested_nssai[i].sd.v);
            }
            return OGS_ERROR;
        }
    }

    if (registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {

        ogs_nas_to_plmn_id(&amf_ue->last_visited_plmn_id,
            &last_visited_registered_tai->nas_plmn_id);
    }

    if (registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_UE_USAGE_SETTING_PRESENT) {
        memcpy(&amf_ue->ue_usage_setting,
                &registration_request->ue_usage_setting,
                registration_request->ue_usage_setting.length +
                sizeof(registration_request->ue_usage_setting.length));
    }

    if ((registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT)
            == 0) {
        amf_ue->nas.present.allowed_pdu_session_status = 0;
    } else {
        amf_ue->nas.present.allowed_pdu_session_status = 1;
        ogs_error("Not implemented for Allowed PDU Session Status IE");
    }

    if ((registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT) == 0) {
        amf_ue->nas.present.uplink_data_status = 0;
    } else {
        amf_ue->nas.present.uplink_data_status = 1;

        psimask = 0;
        psimask |= uplink_data_status->psi << 8;
        psimask |= uplink_data_status->psi >> 8;

        ogs_list_for_each(&amf_ue->sess_list, sess) {
            if (psimask & (1 << sess->psi)) {
#if REMOVED
    /*
     * TS23.502
     * 4.2.3.2 UE Triggered Service Request
     *
     * Step 4. The Nsmf_PDUSession_UpdateSMContext Request is invoked:
     *
     *  - If the UE identifies List Of PDU Sessions To Be Activated
     *    in the Service Request message;
     *          if (sess->smfUpCnxState == OpenAPI_up_cnx_state_DEACTIVATED)
     */
#endif
                if (SESSION_CONTEXT_IN_SMF(sess))
                    amf_sbi_send_activating_session(sess);
            }
        }
    }

    if ((registration_request->presencemask &
            OGS_NAS_5GS_REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT) == 0) {
        amf_ue->nas.present.pdu_session_status = 0;
    } else {
        amf_ue->nas.present.pdu_session_status = 1;

        psimask = 0;
        psimask |= pdu_session_status->psi << 8;
        psimask |= pdu_session_status->psi >> 8;

        ogs_list_for_each(&amf_ue->sess_list, sess) {
            if ((psimask & (1 << sess->psi)) == 0) {
                if (SESSION_CONTEXT_IN_SMF(sess))
                    amf_sbi_send_release_session(
                            sess, AMF_RELEASE_SM_CONTEXT_REGISTRATION_ACCEPT);
            }
        }
    }

    return OGS_OK;
}

int gmm_handle_service_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_service_request_t *service_request)
{
    int served_tai_index = 0;

    ran_ue_t *ran_ue = NULL;
    ogs_nas_key_set_identifier_t *ngksi = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ngksi = &service_request->ngksi;
    ogs_assert(ngksi);

    ogs_debug("Service request");

    amf_ue->nas.message_type = OGS_NAS_5GS_SERVICE_REQUEST;
    amf_ue->nas.tsc = ngksi->tsc;
    amf_ue->nas.ksi = ngksi->value;
    ogs_debug("    OGS_NAS_5GS TYPE[%d] TSC[%d] KSI[%d] SERVICE[0x%x]",
            amf_ue->nas.message_type,
            amf_ue->nas.tsc, amf_ue->nas.ksi, amf_ue->nas.data);

    /*
     * REGISTRATION_REQUEST
     * SERVICE_REQUEST
     *   Clear Timer and Message
     */
    CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        ogs_kdf_kgnb_and_kn3iwf(
                amf_ue->kamf, amf_ue->ul_count.i32,
                amf_ue->nas.access_type, amf_ue->kgnb);
        ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->kgnb, amf_ue->nh);
        amf_ue->nhcc = 1;
    }

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac.v);
    ogs_debug("    OLD NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
            ogs_plmn_id_hexdump(&amf_ue->nr_cgi.plmn_id),
            (long long)amf_ue->nr_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.tai.plmn_id),
            ran_ue->saved.tai.tac.v);
    ogs_debug("    NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
            ogs_plmn_id_hexdump(&ran_ue->saved.nr_cgi.plmn_id),
            (long long)ran_ue->saved.nr_cgi.cell_id);

    /* Copy TAI and ECGI from ran_ue */
    memcpy(&amf_ue->tai, &ran_ue->saved.tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->nr_cgi, &ran_ue->saved.nr_cgi, sizeof(ogs_nr_cgi_t));
    amf_ue->ue_location_timestamp = ogs_time_now();

    /* Check TAI */
    served_tai_index = amf_find_served_tai(&amf_ue->tai);
    if (served_tai_index < 0) {
        /* Send Registration Reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id), amf_ue->tai.tac.v);
        nas_5gs_send_registration_reject(amf_ue,
            OGS_5GMM_CAUSE_TRACKING_AREA_NOT_ALLOWED);
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    ogs_debug("[%s]    5G-S_GUTI[AMF_ID:0x%x,M_TMSI:0x%x]",
        AMF_UE_HAVE_SUCI(amf_ue) ? amf_ue->suci : "Unknown ID",
        ogs_amf_id_hexdump(&amf_ue->guti.amf_id), amf_ue->guti.m_tmsi);

    return OGS_OK;
}

int gmm_handle_service_update(amf_ue_t *amf_ue,
        ogs_nas_5gs_service_request_t *service_request)
{
    amf_sess_t *sess = NULL;
    uint16_t psimask = 0;
    int xact_count = 0;

    ogs_nas_uplink_data_status_t *uplink_data_status = NULL;
    ogs_nas_pdu_session_status_t *pdu_session_status = NULL;
    ogs_nas_allowed_pdu_session_status_t *allowed_pdu_session_status = NULL;

    ogs_assert(amf_ue);

    uplink_data_status = &service_request->uplink_data_status;
    ogs_assert(uplink_data_status);
    pdu_session_status = &service_request->pdu_session_status;
    ogs_assert(pdu_session_status);
    allowed_pdu_session_status = &service_request->allowed_pdu_session_status;
    ogs_assert(allowed_pdu_session_status);

    if (service_request->presencemask &
        OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT) {

        return gmm_handle_nas_message_container(
                amf_ue, &service_request->nas_message_container);
    }

    xact_count = amf_sess_xact_count(amf_ue);

    if ((service_request->presencemask &
        OGS_NAS_5GS_SERVICE_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT) == 0) {
        amf_ue->nas.present.allowed_pdu_session_status = 0;
    } else {
        amf_ue->nas.present.allowed_pdu_session_status = 1;
        ogs_error("Not implemented for Allowed PDU Session Status IE");
    }

    if ((service_request->presencemask &
            OGS_NAS_5GS_SERVICE_REQUEST_UPLINK_DATA_STATUS_PRESENT) == 0) {
        amf_ue->nas.present.uplink_data_status = 0;
    } else {
        amf_ue->nas.present.uplink_data_status = 1;

        psimask = 0;
        psimask |= uplink_data_status->psi << 8;
        psimask |= uplink_data_status->psi >> 8;

        ogs_list_for_each(&amf_ue->sess_list, sess) {
            if (psimask & (1 << sess->psi)) {
#if REMOVED
    /*
     * TS23.502
     * 4.2.3.2 UE Triggered Service Request
     *
     * Step 4. The Nsmf_PDUSession_UpdateSMContext Request is invoked:
     *
     *  - If the UE identifies List Of PDU Sessions To Be Activated
     *    in the Service Request message;
     *          if (sess->smfUpCnxState == OpenAPI_up_cnx_state_DEACTIVATED)
     */
#endif
                if (SESSION_CONTEXT_IN_SMF(sess))
                    amf_sbi_send_activating_session(sess);
            }
        }
    }

    /*
     * TS24.501
     * 5.6.1.5 Service request procedure not accepted by the network
     *
     * If the AMF needs to initiate PDU session status synchronisation
     * or a PDU session status IE was included in the SERVICE REQUEST message,
     * the AMF shall include a PDU session status IE in the SERVICE REJECT
     * message to indicate which PDU sessions associated with the access type
     * the SERVICE REJECT message is sent over are active in the AMF.
     * If the PDU session status IE is included in the SERVICE REJECT message
     * and if the message is integrity protected, then the UE shall perform
     * a local release of all those PDU sessions which are active
     * on the UE side associated with the access type the SERVICE REJECT
     * message is sent over, but are indicated by the AMF as being inactive.
     */
    if ((service_request->presencemask &
            OGS_NAS_5GS_SERVICE_REQUEST_PDU_SESSION_STATUS_PRESENT) == 0) {
        amf_ue->nas.present.pdu_session_status = 0;
    } else {
        amf_ue->nas.present.pdu_session_status = 1;

        psimask = 0;
        psimask |= pdu_session_status->psi << 8;
        psimask |= pdu_session_status->psi >> 8;

        ogs_list_for_each(&amf_ue->sess_list, sess) {
            if ((psimask & (1 << sess->psi)) == 0) {
                if (SESSION_CONTEXT_IN_SMF(sess))
                    amf_sbi_send_release_session(
                            sess, AMF_RELEASE_SM_CONTEXT_SERVICE_ACCEPT);
            }
        }
    }

    if (amf_sess_xact_count(amf_ue) == xact_count)
        nas_5gs_send_service_accept(amf_ue);

    return OGS_OK;
}

int gmm_handle_deregistration_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_deregistration_request_from_ue_t *deregistration_request)
{
    ogs_nas_de_registration_type_t *de_registration_type = NULL;

    ogs_assert(amf_ue);
    ogs_assert(deregistration_request);

    de_registration_type = &deregistration_request->de_registration_type;

    ogs_debug("[%s] Deregistration request", amf_ue->supi);

    /* Set 5GS Attach Type */
    memcpy(&amf_ue->nas.de_registration,
            de_registration_type, sizeof(ogs_nas_de_registration_type_t));
    amf_ue->nas.message_type = OGS_NAS_5GS_DEREGISTRATION_REQUEST;
    amf_ue->nas.tsc = de_registration_type->tsc;
    amf_ue->nas.ksi = de_registration_type->ksi;
    ogs_debug("[%s]    OGS_NAS_5GS TYPE[%d] TSC[%d] KSI[%d] "
            "DEREGISTRATION[0x%x]",
            amf_ue->suci, amf_ue->nas.message_type,
            amf_ue->nas.tsc, amf_ue->nas.ksi, amf_ue->nas.data);

    if (deregistration_request->de_registration_type.switch_off)
        ogs_debug("    Switch-Off");

    amf_sbi_send_release_all_sessions(
            amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);

    if (ogs_list_count(&amf_ue->sess_list) == 0)
        nas_5gs_send_de_registration_accept(amf_ue);

    return OGS_OK;
}

int gmm_handle_authentication_response(amf_ue_t *amf_ue,
        ogs_nas_5gs_authentication_response_t *authentication_response)
{
    ogs_nas_authentication_response_parameter_t
        *authentication_response_parameter = NULL;
    uint8_t hxres_star[OGS_MAX_RES_LEN];

    ogs_assert(amf_ue);
    ogs_assert(authentication_response);

    authentication_response_parameter = &authentication_response->
                authentication_response_parameter;

    ogs_debug("[%s] Authentication response", amf_ue->suci);

    CLEAR_AMF_UE_TIMER(amf_ue->t3560);

    if (authentication_response_parameter->length != OGS_MAX_RES_LEN) {
        ogs_error("[%s] Invalid length [%d]",
                amf_ue->suci, authentication_response_parameter->length);
        return OGS_ERROR;
    }

    ogs_kdf_hxres_star(
            amf_ue->rand, authentication_response_parameter->res, hxres_star);

    if (memcmp(hxres_star, amf_ue->hxres_star, OGS_MAX_RES_LEN) != 0) {
        ogs_error("[%s] MAC failure", amf_ue->suci);
        ogs_log_hexdump(OGS_LOG_ERROR,
                authentication_response_parameter->res,
                authentication_response_parameter->length);
        ogs_log_hexdump(OGS_LOG_ERROR, hxres_star, OGS_MAX_RES_LEN);
        ogs_log_hexdump(OGS_LOG_ERROR,
                amf_ue->hxres_star, OGS_MAX_RES_LEN);
        return OGS_ERROR;
    }

    memcpy(amf_ue->xres_star, authentication_response_parameter->res,
            authentication_response_parameter->length);

    amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue, NULL,
            amf_nausf_auth_build_authenticate_confirmation);

    return OGS_OK;
}

int gmm_handle_identity_response(amf_ue_t *amf_ue,
        ogs_nas_5gs_identity_response_t *identity_response)
{
    ran_ue_t *ran_ue = NULL;

    ogs_nas_5gs_mobile_identity_t *mobile_identity = NULL;
    ogs_nas_5gs_mobile_identity_header_t *mobile_identity_header = NULL;

    ogs_assert(identity_response);

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    mobile_identity = &identity_response->mobile_identity;

    if (!mobile_identity->length || !mobile_identity->buffer) {
        ogs_error("No Mobile Identity");
        return OGS_ERROR;
    }

    mobile_identity_header =
            (ogs_nas_5gs_mobile_identity_header_t *)mobile_identity->buffer;

    if (mobile_identity_header->type == OGS_NAS_5GS_MOBILE_IDENTITY_SUCI) {
        amf_ue_set_suci(amf_ue, mobile_identity);
        ogs_debug("[%s]    SUCI", amf_ue->suci);
    } else {
        ogs_error("Not supported Identity type[%d]",
                mobile_identity_header->type);
    }

    return OGS_OK;
}

int gmm_handle_security_mode_complete(amf_ue_t *amf_ue,
    ogs_nas_5gs_security_mode_complete_t *security_mode_complete)
{
    ogs_nas_5gs_mobile_identity_t *imeisv = NULL;
    ogs_nas_mobile_identity_imeisv_t *mobile_identity_imeisv = NULL;

    ogs_assert(amf_ue);
    ogs_assert(security_mode_complete);

    if (security_mode_complete->presencemask &
        OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT) {

        imeisv = &security_mode_complete->imeisv;
        ogs_assert(imeisv);
        mobile_identity_imeisv =
            (ogs_nas_mobile_identity_imeisv_t *)imeisv->buffer;
        ogs_assert(mobile_identity_imeisv);

        switch (mobile_identity_imeisv->type) {
        case OGS_NAS_5GS_MOBILE_IDENTITY_IMEISV:
            memcpy(&amf_ue->nas_mobile_identity_imeisv,
                mobile_identity_imeisv, imeisv->length);
            ogs_nas_imeisv_to_bcd(mobile_identity_imeisv, imeisv->length,
                    amf_ue->imeisv_bcd);
            ogs_nas_imeisv_bcd_to_buffer(amf_ue->imeisv_bcd,
                    amf_ue->masked_imeisv, &amf_ue->masked_imeisv_len);
            amf_ue->masked_imeisv[5] = 0xff;
            amf_ue->masked_imeisv[6] = 0xff;
            if (amf_ue->pei)
                ogs_free(amf_ue->pei);
            amf_ue->pei = ogs_msprintf("imeisv-%s", amf_ue->imeisv_bcd);
            break;
        default:
            ogs_warn("[%s] Invalid IMEISV Type [%d]",
                    amf_ue->supi, mobile_identity_imeisv->type);
            break;

        }
    }

    if (security_mode_complete->presencemask &
        OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_PRESENT) {

        return gmm_handle_nas_message_container(
                amf_ue, &security_mode_complete->nas_message_container);
    }

    return OGS_OK;
}

int gmm_handle_ul_nas_transport(amf_ue_t *amf_ue,
        ogs_nas_5gs_ul_nas_transport_t *ul_nas_transport)
{
    amf_sess_t *sess = NULL;
    amf_nsmf_pdu_session_update_sm_context_param_t param;

    ogs_nas_payload_container_type_t *payload_container_type = NULL;
    ogs_nas_payload_container_t *payload_container = NULL;
    ogs_nas_pdu_session_identity_2_t *pdu_session_id = NULL;
    ogs_nas_s_nssai_t *nas_s_nssai = NULL;
    ogs_s_nssai_t *selected_s_nssai = NULL;
    ogs_nas_dnn_t *dnn = NULL;
    ogs_nas_5gsm_header_t *gsm_header = NULL;

    ogs_assert(amf_ue);
    ogs_assert(ul_nas_transport);

    payload_container_type = &ul_nas_transport->payload_container_type;
    ogs_assert(payload_container_type);
    payload_container = &ul_nas_transport->payload_container;
    ogs_assert(payload_container);

    if (!payload_container_type->value) {
        ogs_error("[%s] No Payload container type", amf_ue->supi);
        nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION);
        return OGS_ERROR;
    }

    if (!payload_container->length) {
        ogs_error("[%s] No Payload container length", amf_ue->supi);
        nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION);
        return OGS_ERROR;
    }

    if (!payload_container->buffer) {
        ogs_error("[%s] No Payload container buffer", amf_ue->supi);
        nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION);
        return OGS_ERROR;
    }

    if ((ul_nas_transport->presencemask &
        OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT) == 0) {
        ogs_error("[%s] No PDU session ID", amf_ue->supi);
        nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION);
        return OGS_ERROR;
    }

    pdu_session_id = &ul_nas_transport->pdu_session_id;
    if (*pdu_session_id == OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
        ogs_error("[%s] PDU session identity is unassigned",
                amf_ue->supi);
        nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION);
        return OGS_ERROR;
    }

    switch (payload_container_type->value) {
    case OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION:
        nas_s_nssai = &ul_nas_transport->s_nssai;
        ogs_assert(nas_s_nssai);
        dnn = &ul_nas_transport->dnn;
        ogs_assert(dnn);

        gsm_header = (ogs_nas_5gsm_header_t *)payload_container->buffer;
        ogs_assert(gsm_header);

        if (gsm_header->message_type ==
                OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST) {
            sess = amf_sess_find_by_psi(amf_ue, *pdu_session_id);
            if (!sess) {
                sess = amf_sess_add(amf_ue, *pdu_session_id);
                ogs_assert(sess);
            }
        } else {
            sess = amf_sess_find_by_psi(amf_ue, *pdu_session_id);
            if (!sess) {
                ogs_error("[%s] No Session Context [%d]",
                    amf_ue->supi, gsm_header->message_type);
                nas_5gs_send_gmm_status(amf_ue,
                    OGS_5GMM_CAUSE_INSUFFICIENT_USER_PLANE_RESOURCES_FOR_THE_PDU_SESSION);
                return OGS_ERROR;
            }
        }

        if (sess->payload_container)
            ogs_pkbuf_free(sess->payload_container);

        sess->payload_container_type = payload_container_type->value;
        sess->payload_container =
            ogs_pkbuf_alloc(NULL, payload_container->length);
        ogs_assert(sess->payload_container);
        ogs_pkbuf_put_data(sess->payload_container,
                payload_container->buffer, payload_container->length);

        if (gsm_header->message_type ==
                OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST) {

            if (ul_nas_transport->presencemask &
                    OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_PRESENT) {
                ogs_s_nssai_t s_nssai;
                if (ogs_nas_parse_s_nssai(&s_nssai, nas_s_nssai) != 0) {
                    selected_s_nssai =
                        amf_find_s_nssai(&amf_ue->tai.plmn_id, &s_nssai);
                }
            }

            if (!selected_s_nssai) {
                if (amf_ue->num_of_requested_nssai) {
                    selected_s_nssai = &amf_ue->requested_nssai[0];
                }
            }

            if (!selected_s_nssai) {
                ogs_error("No S_NSSAI : Set default S_NSSAI using AMF config");
                selected_s_nssai = &amf_self()->plmn_support[0].s_nssai[0];
                ogs_assert(selected_s_nssai);
            }

            memcpy(&sess->s_nssai, selected_s_nssai, sizeof(ogs_s_nssai_t));

            if (ul_nas_transport->presencemask &
                    OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_PRESENT) {
                if (sess->dnn)
                    ogs_free(sess->dnn);
                sess->dnn = ogs_strdup(dnn->value);
            }

            amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                    sess, AMF_UPDATE_SM_CONTEXT_NO_STATE, NULL,
                    amf_nsmf_pdu_session_build_create_sm_context);

        } else {

            if (!SESSION_CONTEXT_IN_SMF(sess)) {
                ogs_error("[%s:%d] Session Context is not in SMF [%d]",
                    amf_ue->supi, sess->psi, gsm_header->message_type);
                nas_5gs_send_back_5gsm_message(sess,
                    OGS_5GSM_CAUSE_PDU_SESSION_DOES_NOT_EXIST);
                return OGS_ERROR;
            }

            memset(&param, 0, sizeof(param));
            param.n1smbuf = sess->payload_container;

            if (gsm_header->message_type ==
                    OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE) {
                param.ue_location = true;
                param.ue_timezone = true;
            }

            amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                    sess, AMF_UPDATE_SM_CONTEXT_N1_RELEASED, &param,
                    amf_nsmf_pdu_session_build_update_sm_context);

            if (gsm_header->message_type ==
                    OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE) {
                /* Prevent to invoke SMF for this session */
                CLEAR_SM_CONTEXT_REF(sess);
            }
        }
        break;

    default:
        ogs_error("[%s] Unknown Payload container type [%d]",
            amf_ue->supi, payload_container_type->value);
        nas_5gs_send_gmm_status(amf_ue,
                OGS_5GMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int gmm_handle_nas_message_container(amf_ue_t *amf_ue,
        ogs_nas_message_container_t *nas_message_container)
{
    int rv = OGS_ERROR;

    ogs_pkbuf_t *nasbuf = NULL;
    ogs_nas_5gs_message_t nas_message;

    ogs_assert(amf_ue);
    ogs_assert(nas_message_container);

    if (!nas_message_container->buffer || !nas_message_container->length) {
        ogs_error("No NAS message container [%p:%d]",
            nas_message_container->buffer,nas_message_container->length);
        return OGS_ERROR;
    }

    nasbuf = ogs_pkbuf_alloc(NULL, nas_message_container->length);
    ogs_assert(nasbuf);
    ogs_pkbuf_put_data(nasbuf,
            nas_message_container->buffer, nas_message_container->length);

    if (ogs_nas_5gmm_decode(&nas_message, nasbuf) != OGS_OK) {
        ogs_error("ogs_nas_5gmm_decode() failed");
        ogs_pkbuf_free(nasbuf);
        return OGS_ERROR;
    }

    switch (nas_message.gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_debug("Registration request in NAS message container");
            rv = gmm_handle_registration_update(
                    amf_ue, &nas_message.gmm.registration_request);
            break;
        case OGS_NAS_5GS_SERVICE_REQUEST:
            ogs_debug("Service request in NAS message container");
            rv = gmm_handle_service_update(
                    amf_ue, &nas_message.gmm.service_request);
            break;
        default:
            ogs_error("Unknown message [%d]", nas_message.gmm.h.message_type);
            rv = OGS_ERROR;
    }

    ogs_pkbuf_free(nasbuf);
    return rv;
}
