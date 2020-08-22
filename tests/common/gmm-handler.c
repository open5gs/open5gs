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

#include "test-common.h"

void testgmm_handle_registration_accept(test_ue_t *test_ue,
        ogs_nas_5gs_registration_accept_t *registration_accept)
{
    ogs_nas_pdu_session_status_t *pdu_session_status = NULL;
    ogs_nas_pdu_session_reactivation_result_t *pdu_session_reactivation_result;

    ogs_assert(test_ue);
    ogs_assert(registration_accept);

    pdu_session_status = &registration_accept->pdu_session_status;
    pdu_session_reactivation_result =
        &registration_accept->pdu_session_reactivation_result;

    if (registration_accept->presencemask &
            OGS_NAS_5GS_REGISTRATION_ACCEPT_5G_GUTI_PRESENT) {
        ogs_nas_5gs_mobile_identity_t *mobile_identity = NULL;
        ogs_nas_5gs_mobile_identity_guti_t *mobile_identity_guti = NULL;

        mobile_identity = &registration_accept->guti;
        mobile_identity_guti = mobile_identity->buffer;

        memcpy(&test_ue->nas_5gs_guti.nas_plmn_id,
                &mobile_identity_guti->nas_plmn_id, OGS_PLMN_ID_LEN);
        memcpy(&test_ue->nas_5gs_guti.amf_id,
                &mobile_identity_guti->amf_id, sizeof(ogs_amf_id_t));
        test_ue->nas_5gs_guti.m_tmsi = be32toh(mobile_identity_guti->m_tmsi);
    }

    test_ue->pdu_session_status = 0;
    test_ue->pdu_session_reactivation_result = 0;
    if (registration_accept->presencemask &
        OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT)
        test_ue->pdu_session_status = pdu_session_status->psi;
    if (registration_accept->presencemask &
        OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT)
        test_ue->pdu_session_reactivation_result =
            pdu_session_reactivation_result->psi;
}

void testgmm_handle_service_accept(test_ue_t *test_ue,
        ogs_nas_5gs_service_accept_t *service_accept)
{
    ogs_nas_pdu_session_status_t *pdu_session_status = NULL;
    ogs_nas_pdu_session_reactivation_result_t *pdu_session_reactivation_result;

    ogs_assert(test_ue);
    ogs_assert(service_accept);

    pdu_session_status = &service_accept->pdu_session_status;
    pdu_session_reactivation_result =
        &service_accept->pdu_session_reactivation_result;

    test_ue->pdu_session_status = 0;
    test_ue->pdu_session_reactivation_result = 0;
    if (service_accept->presencemask &
        OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT)
        test_ue->pdu_session_status = pdu_session_status->psi;
    if (service_accept->presencemask &
        OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT)
        test_ue->pdu_session_reactivation_result =
            pdu_session_reactivation_result->psi;
}

void testgmm_handle_service_reject(test_ue_t *test_ue,
        ogs_nas_5gs_service_reject_t *service_reject)
{
    ogs_nas_pdu_session_status_t *pdu_session_status = NULL;

    ogs_assert(test_ue);
    ogs_assert(service_reject);

    pdu_session_status = &service_reject->pdu_session_status;

    test_ue->pdu_session_status = 0;
    if (service_reject->presencemask &
        OGS_NAS_5GS_SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT)
        test_ue->pdu_session_status = pdu_session_status->psi;
}

void testgmm_handle_identity_request(test_ue_t *test_ue,
        ogs_nas_5gs_identity_request_t *identity_request)
{
    ogs_assert(test_ue);
    ogs_assert(identity_request);

    switch (identity_request->identity_type.value) {
    case OGS_NAS_5GS_MOBILE_IDENTITY_SUCI:
        break;
    default:
        ogs_error("Not implemented [%d]",
                identity_request->identity_type.value);
        break;
    }
}

void testgmm_handle_authentication_request(test_ue_t *test_ue,
        ogs_nas_5gs_authentication_request_t *authentication_request)
{
    ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand =
        NULL;
    ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn =
        NULL;
    ogs_nas_key_set_identifier_t *ngksi = NULL;

    ogs_assert(test_ue);
    ogs_assert(authentication_request);

    authentication_parameter_rand = &authentication_request->
        authentication_parameter_rand;
    authentication_parameter_autn = &authentication_request->
        authentication_parameter_autn;
    ngksi = &authentication_request->ngksi;

    test_ue->nas.ksi = ngksi->value;

    memcpy(test_ue->rand, authentication_parameter_rand->rand, OGS_RAND_LEN);
    memcpy(test_ue->autn, authentication_parameter_autn->autn, OGS_AUTN_LEN);
}

void testgmm_handle_security_mode_command(test_ue_t *test_ue,
        ogs_nas_5gs_security_mode_command_t *security_mode_command)
{
    ogs_nas_security_algorithms_t *selected_nas_security_algorithms = NULL;
    ogs_nas_key_set_identifier_t *ngksi = NULL;

    ogs_assert(test_ue);
    ogs_assert(security_mode_command);

    selected_nas_security_algorithms =
        &security_mode_command->selected_nas_security_algorithms;
    ngksi = &security_mode_command->ngksi;

    test_ue->selected_enc_algorithm =
        selected_nas_security_algorithms->type_of_ciphering_algorithm;
    test_ue->selected_int_algorithm =
        selected_nas_security_algorithms->
            type_of_integrity_protection_algorithm;

    ogs_kdf_nas_5gs(OGS_KDF_NAS_INT_ALG, test_ue->selected_int_algorithm,
        test_ue->kamf, test_ue->knas_int);
    ogs_kdf_nas_5gs(OGS_KDF_NAS_ENC_ALG, test_ue->selected_enc_algorithm,
        test_ue->kamf, test_ue->knas_enc);

    test_ue->nas.ksi = ngksi->value;

    test_ue->security_context_available = 1;
}

void testgmm_handle_dl_nas_transport(test_ue_t *test_ue,
        ogs_nas_5gs_dl_nas_transport_t *dl_nas_transport)
{
    test_sess_t *sess = NULL;
    ogs_assert(test_ue);
    ogs_assert(dl_nas_transport);

    sess = test_sess_find_by_psi(test_ue, dl_nas_transport->pdu_session_id);
    ogs_assert(sess);

    testgmm_send_to_gsm(sess, &dl_nas_transport->payload_container);
}
