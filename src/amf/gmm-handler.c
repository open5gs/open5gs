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

static int gmm_handle_nas_message_container(
        amf_ue_t *amf_ue, uint8_t message_type,
        ogs_nas_message_container_t *nas_message_container);

int gmm_handle_registration_request(amf_ue_t *amf_ue,
        ogs_nas_security_header_type_t h, NGAP_ProcedureCode_t ngap_code,
        ogs_nas_5gs_registration_request_t *registration_request)
{
    int served_tai_index = 0;

    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_registration_type_t *registration_type = NULL;
    ogs_nas_5gs_mobile_identity_t *mobile_identity = NULL;
    ogs_nas_5gs_mobile_identity_header_t *mobile_identity_header = NULL;
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci = NULL;
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

    /*
     * TS33.501
     * Ch 6.4.6. Protection of initial NAS message
     *
     * If non-cleartext IEs is received, Open5GS will send Registration reject.
     *
     * Step 1: The UE shall send the initial NAS message to the AMF.
     * If the UE has no NAS security context, the initial NAS message
     * shall only contain the cleartext IEs, i.e. subscription identifiers
     * (e.g. SUCI or GUTIs), UE security capabilities, ngKSI, indication
     * that the UE is moving from EPC, Additional GUTI, and IE containing
     * the TAU Request in the case idle mobility from LTE.
     *
     * If the UE has a NAS security context, the message sent shall contain
     * the information given above in cleartext and the complete initial
     * NAS message ciphered in a NAS container which is ciphered.
     * With a NAS security context, the sent message shall also be
     * integrity protected. In the case that the initial NAS message
     * was protected and the AMF has the same security context,
     * then steps 2 to 4 may be omitted In this case the AMF shall
     * use the complete initial NAS message that is in the NAS container
     * as the message to respond to.
     *
     * TS24.501
     * Ch 4.4.6 Protection of initial NAS signalling messages
     *
     * When the initial NAS message is a REGISTRATION REQUEST message,
     * the cleartext IEs are:
     *
     *   - Extended protocol discriminator;
     *   - Security header type;
     *   - Spare half octet;
     *   - Registration request message identity;
     *   - 5GS registration type;
     *   - ngKSI;
     *   - 5GS mobile identity;
     *   - UE security capability;
     *   - Additional GUTI;
     *   - UE status; and
     *   - EPS NAS message container.
     */

#define OGS_REGISTRATION_CLEARTEXT_PRESENT \
        (OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT| \
        OGS_NAS_5GS_REGISTRATION_REQUEST_UE_STATUS_PRESENT| \
        OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_PRESENT| \
        OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT)

    if (ngap_code == NGAP_ProcedureCode_id_InitialUEMessage &&
        registration_request->presencemask &
        ~OGS_REGISTRATION_CLEARTEXT_PRESENT) {
        ogs_error("Non cleartext IEs is included [0x%llx]",
                (long long)registration_request->presencemask);
        ogs_assert(OGS_OK ==
            nas_5gs_send_registration_reject(amf_ue,
                OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE));
        return OGS_ERROR;
    }

    if (!h.integrity_protected &&
        (registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT)) {
        ogs_error("NAS container present without Integrity-protected");
        ogs_assert(OGS_OK ==
            nas_5gs_send_registration_reject(amf_ue,
                OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE));
        return OGS_ERROR;
    }

    if (!mobile_identity->length || !mobile_identity->buffer) {
        ogs_error("No Mobile Identity");
        ogs_assert(OGS_OK ==
            nas_5gs_send_registration_reject(amf_ue,
                OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE));
        return OGS_ERROR;
    }

    mobile_identity_header =
            (ogs_nas_5gs_mobile_identity_header_t *)mobile_identity->buffer;

    switch (mobile_identity_header->type) {
    case OGS_NAS_5GS_MOBILE_IDENTITY_SUCI:
        mobile_identity_suci =
            (ogs_nas_5gs_mobile_identity_suci_t *)mobile_identity->buffer;
        if (mobile_identity_suci->protection_scheme_id !=
                OGS_NAS_5GS_NULL_SCHEME) {
            ogs_error("Not implemented ProtectionSchemeID(%d) in SUCI",
                mobile_identity_suci->protection_scheme_id);
            ogs_assert(OGS_OK ==
                nas_5gs_send_registration_reject(amf_ue,
                    OGS_5GMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED)
            );
            return OGS_ERROR;
        }
        amf_ue_set_suci(amf_ue, mobile_identity);
        ogs_info("[%s]    SUCI", amf_ue->suci);
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

        ogs_info("[%s]    5G-S_GUTI[AMF_ID:0x%x,M_TMSI:0x%x]",
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

    amf_ue->nas.ue.tsc = registration_type->tsc;
    amf_ue->nas.ue.ksi = registration_type->ksi;
    ogs_debug("    OLD TSC[UE:%d,AMF:%d] KSI[UE:%d,AMF:%d]",
            amf_ue->nas.ue.tsc, amf_ue->nas.amf.tsc,
            amf_ue->nas.ue.ksi, amf_ue->nas.amf.ksi);
    if (amf_ue->nas.ue.ksi < OGS_NAS_KSI_NO_KEY_IS_AVAILABLE) {
        amf_ue->nas.amf.tsc = amf_ue->nas.ue.tsc;
        amf_ue->nas.amf.ksi = amf_ue->nas.ue.ksi;
    }
    ogs_debug("    NEW TSC[UE:%d,AMF:%d] KSI[UE:%d,AMF:%d]",
            amf_ue->nas.ue.tsc, amf_ue->nas.amf.tsc,
            amf_ue->nas.ue.ksi, amf_ue->nas.amf.ksi);
    /*
     * REGISTRATION_REQUEST
     * SERVICE_REQUEST
     *   Clear N2 Transfer
     *   Clear Timer and Message
     */
    AMF_UE_CLEAR_PAGING_INFO(amf_ue);
    AMF_UE_CLEAR_N2_TRANSFER(amf_ue, pdu_session_resource_setup_request);
    AMF_UE_CLEAR_5GSM_MESSAGE(amf_ue);
    CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        ogs_kdf_kgnb_and_kn3iwf(
                amf_ue->kamf, amf_ue->ul_count.i32,
                amf_ue->nas.access_type, amf_ue->kgnb);
        ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->kgnb, amf_ue->nh);
        amf_ue->nhcc = 1;
    }

    /*
     * TS24.501
     * 5.3.3 Temporary identities
     *
     * The AMF shall assign a new 5G-GUTI for a particular UE:
     *
     * a) during a successful initial registration procedure;
     * b) during a successful registration procedure
     *    for mobility registration update; and
     * c) after a successful service request procedure invoked
     *    as a response to a paging request from the network and
     *    before the release of the N1 NAS signalling connection
     *    as specified in subclause 5.4.4.1.
     *
     * The AMF should assign a new 5G-GUTI for a particular UE
     * during a successful registration procedure
     * for periodic registration update.
     *
     * The AMF may assign a new 5G-GUTI at any time for a particular UE
     * by performing the generic UE configuration update procedure.
     */
    amf_ue_new_guti(amf_ue);

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->nr_tai.plmn_id), amf_ue->nr_tai.tac.v);
    ogs_debug("    OLD NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
            ogs_plmn_id_hexdump(&amf_ue->nr_cgi.plmn_id),
            (long long)amf_ue->nr_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.nr_tai.plmn_id),
            ran_ue->saved.nr_tai.tac.v);
    ogs_debug("    NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
            ogs_plmn_id_hexdump(&ran_ue->saved.nr_cgi.plmn_id),
            (long long)ran_ue->saved.nr_cgi.cell_id);

    /* Copy Stream-No/NR-TAI/NR-CGI from ran_ue */
    amf_ue->gnb_ostream_id = ran_ue->gnb_ostream_id;
    memcpy(&amf_ue->nr_tai, &ran_ue->saved.nr_tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->nr_cgi, &ran_ue->saved.nr_cgi, sizeof(ogs_nr_cgi_t));
    amf_ue->ue_location_timestamp = ogs_time_now();

    /* Check TAI */
    served_tai_index = amf_find_served_tai(&amf_ue->nr_tai);
    if (served_tai_index < 0) {
        /* Send Registration Reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->nr_tai.plmn_id), amf_ue->nr_tai.tac.v);
        ogs_assert(OGS_OK ==
            nas_5gs_send_registration_reject(amf_ue,
                OGS_5GMM_CAUSE_TRACKING_AREA_NOT_ALLOWED));
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    if (registration_request->presencemask &
            OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT) {
        memcpy(&amf_ue->ue_security_capability,
                &registration_request->ue_security_capability,
                registration_request->ue_security_capability.length +
                sizeof(registration_request->ue_security_capability.length));
    }

    if (amf_selected_int_algorithm(amf_ue) ==
            OGS_NAS_SECURITY_ALGORITHMS_NIA0) {
        ogs_error("[UE:0x%x:0x%x], NEA0 can be used in Encrypt[0x%x], "
            "but Integrity[0x%x] cannot be bypassed with NIA0",
            ue_security_capability->nr_ea, ue_security_capability->nr_ia,
            amf_selected_enc_algorithm(amf_ue), 
            amf_selected_int_algorithm(amf_ue));
        ogs_assert(OGS_OK ==
            nas_5gs_send_registration_reject(amf_ue,
                OGS_5GMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH));
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
                amf_ue, OGS_NAS_5GS_REGISTRATION_REQUEST,
                &registration_request->nas_message_container);
    }

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
            OGS_NAS_5GS_REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT) {
        memcpy(&amf_ue->ue_network_capability,
                &registration_request->s1_ue_network_capability,
                registration_request->s1_ue_network_capability.length +
                sizeof(registration_request->s1_ue_network_capability.length));
    }

    if (registration_request->presencemask &
        OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NSSAI_PRESENT) {

        amf_ue->requested_nssai.num_of_s_nssai =
            ogs_nas_parse_nssai(
                    amf_ue->requested_nssai.s_nssai,
                    &registration_request->requested_nssai);

        for (i = 0; i < amf_ue->requested_nssai.num_of_s_nssai; i++) {
            if (amf_find_s_nssai(
                    &amf_ue->nr_tai.plmn_id,
                    (ogs_s_nssai_t *)&amf_ue->requested_nssai.s_nssai[i]))
                break;
        }

        if (i == amf_ue->requested_nssai.num_of_s_nssai) {
            ogs_error("Cannot find Requested NSSAI [%d]",
                    amf_ue->requested_nssai.num_of_s_nssai);
            for (i = 0; i < amf_ue->requested_nssai.num_of_s_nssai; i++) {
                ogs_error("    PLMN_ID[MCC:%d MNC:%d]",
                        ogs_plmn_id_mcc(&amf_ue->nr_tai.plmn_id),
                        ogs_plmn_id_mnc(&amf_ue->nr_tai.plmn_id));
                ogs_error("    S_NSSAI[SST:%d SD:0x%x]",
                        amf_ue->requested_nssai.s_nssai[i].sst,
                        amf_ue->requested_nssai.s_nssai[i].sd.v);
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
                if (SESSION_CONTEXT_IN_SMF(sess))
                    amf_sbi_send_activating_session(
                            sess, AMF_UPDATE_SM_CONTEXT_REGISTRATION_REQUEST);
            }
        }
    }

    return OGS_OK;
}

int gmm_handle_service_request(amf_ue_t *amf_ue,
        ogs_nas_security_header_type_t h, NGAP_ProcedureCode_t ngap_code,
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

    /*
     * TS24.501
     * Ch 4.4.6 Protection of initial NAS signalling messages
     *
     * When the initial NAS message is a SERVICE REQUEST message,
     * the cleartext IEs are:
     *
     *   - Extended protocol discriminator;
     *   - Security header type;
     *   - Spare half octet;
     *   - ngKSI;
     *   - Service request message identity;
     *   - Service type; and
     *   - 5G-S-TMSI.
     */
#define OGS_SERVICE_CLEARTEXT_PRESENT \
        (OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT)

    if (ngap_code == NGAP_ProcedureCode_id_InitialUEMessage &&
        service_request->presencemask & ~OGS_SERVICE_CLEARTEXT_PRESENT) {
        ogs_error("Non cleartext IEs is included [0x%llx]",
                (long long)service_request->presencemask);
        ogs_assert(OGS_OK ==
            nas_5gs_send_service_reject(amf_ue,
                OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE));
        return OGS_ERROR;
    }

    if (!h.integrity_protected &&
        (service_request->presencemask &
        OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT)) {
        ogs_error("NAS container present without Integrity-protected");
        ogs_assert(OGS_OK ==
            nas_5gs_send_service_reject(amf_ue,
                OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE));
        return OGS_ERROR;
    }

    amf_ue->nas.message_type = OGS_NAS_5GS_SERVICE_REQUEST;

    amf_ue->nas.ue.tsc = ngksi->tsc;
    amf_ue->nas.ue.ksi = ngksi->value;
    ogs_debug("    OLD TSC[UE:%d,AMF:%d] KSI[UE:%d,AMF:%d]",
            amf_ue->nas.ue.tsc, amf_ue->nas.amf.tsc,
            amf_ue->nas.ue.ksi, amf_ue->nas.amf.ksi);
    if (amf_ue->nas.ue.ksi < OGS_NAS_KSI_NO_KEY_IS_AVAILABLE) {
        amf_ue->nas.amf.tsc = amf_ue->nas.ue.tsc;
        amf_ue->nas.amf.ksi = amf_ue->nas.ue.ksi;
    }
    ogs_debug("    NEW TSC[UE:%d,AMF:%d] KSI[UE:%d,AMF:%d]",
            amf_ue->nas.ue.tsc, amf_ue->nas.amf.tsc,
            amf_ue->nas.ue.ksi, amf_ue->nas.amf.ksi);

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
            ogs_plmn_id_hexdump(&amf_ue->nr_tai.plmn_id), amf_ue->nr_tai.tac.v);
    ogs_debug("    OLD NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
            ogs_plmn_id_hexdump(&amf_ue->nr_cgi.plmn_id),
            (long long)amf_ue->nr_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.nr_tai.plmn_id),
            ran_ue->saved.nr_tai.tac.v);
    ogs_debug("    NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
            ogs_plmn_id_hexdump(&ran_ue->saved.nr_cgi.plmn_id),
            (long long)ran_ue->saved.nr_cgi.cell_id);

    /* Copy Stream-No/NR-TAI/NR-CGI from ran_ue */
    amf_ue->gnb_ostream_id = ran_ue->gnb_ostream_id;
    memcpy(&amf_ue->nr_tai, &ran_ue->saved.nr_tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->nr_cgi, &ran_ue->saved.nr_cgi, sizeof(ogs_nr_cgi_t));
    amf_ue->ue_location_timestamp = ogs_time_now();

    /* Check TAI */
    served_tai_index = amf_find_served_tai(&amf_ue->nr_tai);
    if (served_tai_index < 0) {
        /* Send Registration Reject */
        ogs_warn("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->nr_tai.plmn_id), amf_ue->nr_tai.tac.v);
        ogs_assert(OGS_OK ==
            nas_5gs_send_registration_reject(amf_ue,
                OGS_5GMM_CAUSE_TRACKING_AREA_NOT_ALLOWED));
        return OGS_ERROR;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    ogs_info("[%s]    5G-S_GUTI[AMF_ID:0x%x,M_TMSI:0x%x]",
        AMF_UE_HAVE_SUCI(amf_ue) ? amf_ue->suci : "Unknown ID",
        ogs_amf_id_hexdump(&amf_ue->current.guti.amf_id),
        amf_ue->current.guti.m_tmsi);

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
                amf_ue, OGS_NAS_5GS_SERVICE_REQUEST,
                &service_request->nas_message_container);
    }

    xact_count = amf_sess_xact_count(amf_ue);

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
                if (SESSION_CONTEXT_IN_SMF(sess))
                    amf_sbi_send_activating_session(
                            sess, AMF_UPDATE_SM_CONTEXT_SERVICE_REQUEST);
            }
        }
    }

    if (amf_sess_xact_count(amf_ue) == xact_count)
        ogs_assert(OGS_OK ==
            nas_5gs_send_service_accept(amf_ue));

    return OGS_OK;
}

int gmm_handle_deregistration_request(amf_ue_t *amf_ue,
        ogs_nas_5gs_deregistration_request_from_ue_t *deregistration_request)
{
    ogs_nas_de_registration_type_t *de_registration_type = NULL;

    ogs_assert(amf_ue);
    ogs_assert(deregistration_request);

    de_registration_type = &deregistration_request->de_registration_type;

    /* Set 5GS De-registration Type */
    memcpy(&amf_ue->nas.de_registration,
            de_registration_type, sizeof(ogs_nas_de_registration_type_t));
    amf_ue->nas.message_type = OGS_NAS_5GS_DEREGISTRATION_REQUEST;

    amf_ue->nas.ue.tsc = de_registration_type->tsc;
    amf_ue->nas.ue.ksi = de_registration_type->ksi;
    ogs_debug("    OLD TSC[UE:%d,AMF:%d] KSI[UE:%d,AMF:%d]",
            amf_ue->nas.ue.tsc, amf_ue->nas.amf.tsc,
            amf_ue->nas.ue.ksi, amf_ue->nas.amf.ksi);
    if (amf_ue->nas.ue.ksi < OGS_NAS_KSI_NO_KEY_IS_AVAILABLE) {
        amf_ue->nas.amf.tsc = amf_ue->nas.ue.tsc;
        amf_ue->nas.amf.ksi = amf_ue->nas.ue.ksi;
    }
    ogs_debug("    NEW TSC[UE:%d,AMF:%d] KSI[UE:%d,AMF:%d]",
            amf_ue->nas.ue.tsc, amf_ue->nas.amf.tsc,
            amf_ue->nas.ue.ksi, amf_ue->nas.amf.ksi);

    if (deregistration_request->de_registration_type.switch_off)
        ogs_debug("    Switch-Off");

    ogs_info("[%s]    SUCI", amf_ue->suci);

    amf_sbi_send_release_all_sessions(
            amf_ue, AMF_RELEASE_SM_CONTEXT_NO_STATE);

    if (ogs_list_count(&amf_ue->sess_list) == 0)
        ogs_assert(OGS_OK ==
            nas_5gs_send_de_registration_accept(amf_ue));

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

    ogs_assert(true ==
        amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue, NULL,
                amf_nausf_auth_build_authenticate_confirmation));

    return OGS_OK;
}

int gmm_handle_identity_response(amf_ue_t *amf_ue,
        ogs_nas_5gs_identity_response_t *identity_response)
{
    ran_ue_t *ran_ue = NULL;

    ogs_nas_5gs_mobile_identity_t *mobile_identity = NULL;
    ogs_nas_5gs_mobile_identity_suci_t *mobile_identity_suci = NULL;
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
        mobile_identity_suci =
            (ogs_nas_5gs_mobile_identity_suci_t *)mobile_identity->buffer;
        if (mobile_identity_suci->protection_scheme_id !=
                OGS_NAS_5GS_NULL_SCHEME) {
            ogs_error("Not implemented ProtectionSchemeID(%d) in SUCI",
                mobile_identity_suci->protection_scheme_id);
            return OGS_ERROR;
        }
        amf_ue_set_suci(amf_ue, mobile_identity);
        ogs_info("[%s]    SUCI", amf_ue->suci);
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

    /*
     * TS33.501
     * Ch 6.4.6. Protection of initial NAS message
     *
     * UE should send NAS Container in Security mode complete message.
     * Otherwise, Open5GS will send Registration reject message.
     *
     * Step 4: The UE shall send the NAS Security Mode Complete message
     * to the network in response to a NAS Security Mode Command message.
     * The NAS Security Mode Complete message shall be ciphered and
     * integrity protected. Furthermore the NAS Security Mode Complete message
     * shall include the complete initial NAS message in a NAS Container
     * if either requested by the AMF or the UE sent the initial NAS message
     * unprotected. The AMF shall use the complete initial NAS message
     * that is in the NAS container as the message to respond to.
     */
    if ((security_mode_complete->presencemask &
        OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_PRESENT)
            == 0) {
        ogs_error("No NAS Message Container in Security mode complete message");
        return OGS_ERROR;
    }

    if (security_mode_complete->presencemask &
        OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT) {

        imeisv = &security_mode_complete->imeisv;
        ogs_assert(imeisv);
        mobile_identity_imeisv =
            (ogs_nas_mobile_identity_imeisv_t *)imeisv->buffer;
        ogs_assert(mobile_identity_imeisv);

        switch (mobile_identity_imeisv->type) {
        case OGS_NAS_5GS_MOBILE_IDENTITY_IMEISV:
            /* TS23.003 6.2.2 Composition of IMEISV
             *
             * The International Mobile station Equipment Identity and
             * Software Version Number (IMEISV) is composed.
             *
             * TAC(8 digits) - SNR(6 digits) - SVN(2 digits)
             * IMEISV(16 digits) ==> 8bytes
             */
            if (imeisv->length == sizeof(ogs_nas_mobile_identity_imeisv_t)) {
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
                ogs_assert(amf_ue->pei);
            } else {
                ogs_error("[%s] Unknown IMEISV Length [%d]",
                        amf_ue->supi, imeisv->length);
                ogs_log_hexdump(OGS_LOG_ERROR, imeisv->buffer, imeisv->length);
            }
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
                amf_ue, OGS_NAS_5GS_SECURITY_MODE_COMPLETE,
                &security_mode_complete->nas_message_container);
    }

    return OGS_OK;
}

int gmm_handle_ul_nas_transport(amf_ue_t *amf_ue,
        ogs_nas_5gs_ul_nas_transport_t *ul_nas_transport)
{
    ogs_slice_data_t *selected_slice = NULL;
    amf_sess_t *sess = NULL;
    amf_nsmf_pdusession_sm_context_param_t param;

    ogs_nas_payload_container_type_t *payload_container_type = NULL;
    ogs_nas_payload_container_t *payload_container = NULL;
    ogs_nas_pdu_session_identity_2_t *pdu_session_id = NULL;
    ogs_nas_s_nssai_t *nas_s_nssai = NULL;
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
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION));
        return OGS_ERROR;
    }

    if (!payload_container->length) {
        ogs_error("[%s] No Payload container length", amf_ue->supi);
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION));
        return OGS_ERROR;
    }

    if (!payload_container->buffer) {
        ogs_error("[%s] No Payload container buffer", amf_ue->supi);
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION));
        return OGS_ERROR;
    }

    if ((ul_nas_transport->presencemask &
        OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT) == 0) {
        ogs_error("[%s] No PDU session ID", amf_ue->supi);
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION));
        return OGS_ERROR;
    }

    pdu_session_id = &ul_nas_transport->pdu_session_id;
    if (*pdu_session_id == OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
        ogs_error("[%s] PDU session identity is unassigned",
                amf_ue->supi);
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_status(
                amf_ue, OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION));
        return OGS_ERROR;
    }

    switch (payload_container_type->value) {
    case OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION:
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
                ogs_assert(OGS_OK ==
                    nas_5gs_send_gmm_status(amf_ue,
                    OGS_5GMM_CAUSE_INSUFFICIENT_USER_PLANE_RESOURCES_FOR_THE_PDU_SESSION));
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

            int i, j, k;

            nas_s_nssai = &ul_nas_transport->s_nssai;
            ogs_assert(nas_s_nssai);
            dnn = &ul_nas_transport->dnn;
            ogs_assert(dnn);


            for (i = 0; i < amf_ue->num_of_slice; i++) {
                if (ul_nas_transport->presencemask &
                        OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_PRESENT) {
                    ogs_nas_s_nssai_ie_t ie;
                    if (ogs_nas_parse_s_nssai(&ie, nas_s_nssai) != 0) {
                        if (ie.sst == amf_ue->slice[i].s_nssai.sst &&
                            ie.sd.v == amf_ue->slice[i].s_nssai.sd.v) {

                            /* PASS */

                        } else {
                            continue;
                        }
                    }
                }
                for (j = 0; j < amf_ue->allowed_nssai.num_of_s_nssai; j++) {
                    if (amf_ue->slice[i].s_nssai.sst ==
                            amf_ue->allowed_nssai.s_nssai[j].sst &&
                        amf_ue->slice[i].s_nssai.sd.v ==
                            amf_ue->allowed_nssai.s_nssai[j].sd.v) {

                        if (ul_nas_transport->presencemask &
                                OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_PRESENT) {

                            for (k = 0;
                                    k < amf_ue->slice[i].num_of_session; k++) {
                                if (!strcmp(dnn->value,
                                            amf_ue->slice[i].session[k].name)) {

                                    selected_slice = amf_ue->slice + i;
                                    ogs_assert(selected_slice);

                                    if (sess->dnn)
                                        ogs_free(sess->dnn);
                                    sess->dnn = ogs_strdup(dnn->value);
                                    ogs_assert(sess->dnn);

                                } else {
                                    continue;
                                }
                            }

                        } else {

                            selected_slice = amf_ue->slice + i;
                            ogs_assert(selected_slice);

                            if (selected_slice->num_of_session) {
                                if (sess->dnn)
                                    ogs_free(sess->dnn);
                                sess->dnn = ogs_strdup(
                                        selected_slice->session[0].name);
                                ogs_assert(sess->dnn);
                            }
                        }
                    }
                }
            }

            if (!selected_slice || !sess->dnn) {
                ogs_warn("[%s] DNN Not Supporetd OR "
                            "Not Subscribed in the Slice", amf_ue->supi);
                ogs_assert(OGS_OK ==
                    nas_5gs_send_gmm_status(amf_ue, OGS_5GMM_CAUSE_DNN_NOT_SUPPORTED_OR_NOT_SUBSCRIBED_IN_THE_SLICE));
                return OGS_ERROR;
            }

            /* Store S-NSSAI */
            sess->s_nssai.sst = selected_slice->s_nssai.sst;
            sess->s_nssai.sd.v = selected_slice->s_nssai.sd.v;

            ogs_info("UE SUPI[%s] DNN[%s] S_NSSAI[SST:%d SD:0x%x]",
                amf_ue->supi, sess->dnn, sess->s_nssai.sst, sess->s_nssai.sd.v);

            if (!SESSION_CONTEXT_IN_SMF(sess)) {
                ogs_sbi_nf_instance_t *nf_instance = NULL;

                nf_instance = OGS_SBI_NF_INSTANCE(
                                &sess->sbi, OpenAPI_nf_type_SMF);
                if (!nf_instance) {
                    amf_sess_select_smf(sess);
                    nf_instance = OGS_SBI_NF_INSTANCE(
                                    &sess->sbi, OpenAPI_nf_type_SMF);
                }

                if (nf_instance) {
                    ogs_assert(true ==
                        amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                            sess, AMF_CREATE_SM_CONTEXT_NO_STATE, NULL,
                            amf_nsmf_pdusession_build_create_sm_context));
                } else {
                    ogs_assert(true ==
                        amf_sess_sbi_discover_and_send(OpenAPI_nf_type_NSSF,
                            sess, 0, NULL, amf_nnssf_nsselection_build_get));
                }

            } else {

                memset(&param, 0, sizeof(param));
                param.release = 1;
                param.cause = OpenAPI_cause_REL_DUE_TO_DUPLICATE_SESSION_ID;

                ogs_assert(true ==
                    amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                        sess, AMF_UPDATE_SM_CONTEXT_DUPLICATED_PDU_SESSION_ID,
                        &param, amf_nsmf_pdusession_build_update_sm_context));
            }

        } else {

            if (!SESSION_CONTEXT_IN_SMF(sess)) {
                ogs_error("[%s:%d] Session Context is not in SMF [%d]",
                    amf_ue->supi, sess->psi, gsm_header->message_type);
                ogs_assert(OGS_OK ==
                    nas_5gs_send_back_gsm_message(sess,
                        OGS_5GMM_CAUSE_DNN_NOT_SUPPORTED_OR_NOT_SUBSCRIBED_IN_THE_SLICE, 0));
                return OGS_ERROR;
            }

            memset(&param, 0, sizeof(param));
            param.n1smbuf = sess->payload_container;

            if (gsm_header->message_type ==
                    OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE) {
                param.ue_location = true;
                param.ue_timezone = true;
            }

            ogs_assert(true ==
                amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                    sess, AMF_UPDATE_SM_CONTEXT_N1_RELEASED, &param,
                    amf_nsmf_pdusession_build_update_sm_context));

            switch (gsm_header->message_type) {
            case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
            case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT:
                if (PAGING_ONGOING(amf_ue) == true) {

                    gmm_configuration_update_command_param_t param;
                /*
                 * TS24.501
                 * 5.4.4 Generic UE configuration update procedure
                 * 5.4.4.1 General
                 *
                 * This procedure shall be initiated by the network to assign
                 * a new 5G-GUTI to the UE after a successful service request
                 * procedure invoked as a response to a paging request
                 * from the network and before the release
                 * of the N1 NAS signalling connection.
                 *
                 * If the service request procedure was triggered
                 * due to 5GSM downlink signalling pending, the procedure
                 * for assigning a new 5G-GUTI can be initiated by the network
                 * after the transport of the 5GSM downlink signalling.
                 */
                    amf_ue_new_guti(amf_ue);

                    memset(&param, 0, sizeof(param));
                    param.acknowledgement_requested = 1;
                    param.guti = 1;
                    ogs_assert(OGS_OK ==
                        nas_5gs_send_configuration_update_command(
                            amf_ue, &param));

                    AMF_UE_CLEAR_PAGING_INFO(amf_ue);
                }
                break;
            case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
                /* Prevent to invoke SMF for this session */
                CLEAR_SM_CONTEXT_REF(sess);
                break;
            default:
                break;
            }
        }
        break;

    default:
        ogs_error("[%s] Unknown Payload container type [%d]",
            amf_ue->supi, payload_container_type->value);
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_status(amf_ue,
                OGS_5GMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED));
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int gmm_handle_nas_message_container(
        amf_ue_t *amf_ue, uint8_t message_type,
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

    /*
     * 3GPP TS 24.501 version 16.6.0 Release 16
     * 4.4 NAS security
     * 4.4.6 Protection of initial NAS signalling messages
     *
     * 1) the UE needs to send non-cleartext IEs in a REGISTRATION REQUEST
     * or SERVICE REQUEST message, the UE includes the entire REGISTRATION
     * REQUEST or SERVICE REQUEST message (i.e. containing both cleartext IEs
     * and non-cleartext IEs) in the NAS message container IE and shall cipher
     * the value part of the NAS message container IE. The UE shall then send
     * a REGISTRATION REQUEST or SERVICE REQUEST message containing
     * the cleartext IEs and the NAS message container IE;
     */
    switch (message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
    case OGS_NAS_5GS_SERVICE_REQUEST:
        switch (amf_ue->selected_enc_algorithm) {
        case OGS_NAS_SECURITY_ALGORITHMS_128_NEA1:
        case OGS_NAS_SECURITY_ALGORITHMS_128_NEA2:
        case OGS_NAS_SECURITY_ALGORITHMS_128_NEA3:
            ogs_nas_encrypt(amf_ue->selected_enc_algorithm,
                amf_ue->knas_enc, amf_ue->ul_count.i32,
                amf_ue->nas.access_type,
                OGS_NAS_SECURITY_UPLINK_DIRECTION, nasbuf);
        default:
            break;
        }
    default:
        break;
    }

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
