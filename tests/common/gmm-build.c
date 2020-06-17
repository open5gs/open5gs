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

ogs_pkbuf_t *testgmm_build_registration_request(test_ue_t *test_ue,
        ogs_nas_5gs_mobile_identity_t *mobile_identity)
{
    ogs_nas_5gs_message_t message;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_5gs_registration_request_t *registration_request =
            &message.gmm.registration_request;
    ogs_nas_5gs_registration_type_t *registration_type =
            &registration_request->registration_type;
    ogs_nas_5gmm_capability_t *gmm_capability =
            &registration_request->gmm_capability;
    ogs_nas_ue_security_capability_t *ue_security_capability =
            &registration_request->ue_security_capability;

    ogs_assert(test_ue);
    ogs_assert(mobile_identity);

    memset(&message, 0, sizeof(message));
    message.gmm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_REGISTRATION_REQUEST;

    registration_type->data = test_ue->nas.data;

    registration_request->mobile_identity.length = mobile_identity->length;
    registration_request->mobile_identity.buffer = mobile_identity->buffer;

    registration_request->presencemask |=
            OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT;
    gmm_capability->length = 1;
    gmm_capability->lte_positioning_protocol_capability = 1;
    gmm_capability->ho_attach = 1; 
    gmm_capability->s1_mode = 1; 

    registration_request->presencemask |=
            OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT;
    ue_security_capability->length = 8;
    ue_security_capability->nea = 0xff;
    ue_security_capability->nia = 0xff;
    ue_security_capability->eps_ea = 0xff;
    ue_security_capability->eps_ia = 0xff;

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *testgmm_build_authentication_response(test_ue_t *test_ue)
{
    ogs_nas_5gs_message_t message;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_5gs_authentication_response_t *authentication_response =
            &message.gmm.authentication_response;
    ogs_nas_authentication_response_parameter_t *authentication_response_parameter = &authentication_response->authentication_response_parameter;

    uint8_t ik[OGS_KEY_LEN];
    uint8_t ck[OGS_KEY_LEN];
    uint8_t ak[OGS_AK_LEN];
    uint8_t res[OGS_MAX_RES_LEN];
    uint8_t res_star[OGS_MAX_RES_LEN];
    uint8_t kausf[OGS_SHA256_DIGEST_SIZE];
    uint8_t kseaf[OGS_SHA256_DIGEST_SIZE];
    char *serving_network_name;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.gmm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_AUTHENTICATION_RESPONSE;

    authentication_response->presencemask |=
        OGS_NAS_5GS_AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_PRESENT;

    milenage_f2345(test_ue->opc, test_ue->k, test_ue->rand,
            res, ck, ik, ak, NULL);
    serving_network_name =
        ogs_serving_network_name_from_plmn_id(&test_self()->tai.plmn_id);
    ogs_kdf_xres_star(
            ck, ik,
            serving_network_name, test_ue->rand, res, 8,
            authentication_response_parameter->res);

    authentication_response_parameter->length = 16;

    memcpy(res_star, authentication_response_parameter->res,
            authentication_response_parameter->length);
    ogs_kdf_kausf(ck, ik, serving_network_name, test_ue->autn, kausf);
    ogs_kdf_kseaf(serving_network_name, kausf, kseaf);
    ogs_kdf_kamf(test_ue->supi, test_ue->abba, test_ue->abba_len,
                kseaf, test_ue->kamf);

    ogs_free(serving_network_name);

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *testgmm_build_security_mode_complete(
        test_ue_t *test_ue, ogs_pkbuf_t *nasbuf)
{
    ogs_nas_5gs_message_t message;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_5gs_security_mode_complete_t *security_mode_complete =
            &message.gmm.security_mode_complete;
    ogs_nas_message_container_t *nas_message_container =
            &security_mode_complete->nas_message_container;
    ogs_nas_5gs_mobile_identity_t *imeisv =
            &security_mode_complete->imeisv;

    ogs_nas_mobile_identity_imeisv_t mobile_identity_imeisv;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_SECURITY_MODE_COMPLETE;

    security_mode_complete->presencemask |=
        OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT;
    memset(&mobile_identity_imeisv, 0, sizeof(mobile_identity_imeisv));
    imeisv->length = sizeof(mobile_identity_imeisv);
    imeisv->buffer = &mobile_identity_imeisv;

    mobile_identity_imeisv.type = OGS_NAS_5GS_MOBILE_IDENTITY_IMEISV;
    mobile_identity_imeisv.odd_even = OGS_NAS_MOBILE_IDENTITY_EVEN;
    mobile_identity_imeisv.digit1 = 1;
    mobile_identity_imeisv.digit2 = 1;
    mobile_identity_imeisv.digit3 = 1;

    if (nasbuf) {
        security_mode_complete->presencemask |=
            OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_PRESENT;
        nas_message_container->length = nasbuf->len;
        nas_message_container->buffer = nasbuf->data;
    }

    return test_nas_5gs_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testgmm_build_registration_complete(test_ue_t *test_ue)
{
    ogs_nas_5gs_message_t message;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_5gs_registration_complete_t *registration_complete =
            &message.gmm.registration_complete;

    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_REGISTRATION_COMPLETE;

    return test_nas_5gs_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testgmm_build_configuration_update_complete(test_ue_t *test_ue)
{
    ogs_nas_5gs_message_t message;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_5gs_configuration_update_complete_t *configuration_update_complete =
            &message.gmm.configuration_update_complete;
    ogs_nas_control_plane_service_type_t *control_plane_service_type = NULL;

    ogs_assert(test_ue);

    control_plane_service_type = &configuration_update_complete->control_plane_service_type;

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_CONFIGURATION_UPDATE_COMPLETE;

    control_plane_service_type->value =
        OGS_NAS_5GS_CONTROL_PLANE_SERVICE_TYPE_MO;

    return test_nas_5gs_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testgmm_build_ul_nas_transport(test_sess_t *test_sess,
        uint8_t payload_type, ogs_pkbuf_t *payload)
{
    test_ue_t *test_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_ul_nas_transport_t *ul_nas_transport =
            &message.gmm.ul_nas_transport;

    ogs_nas_payload_container_type_t *payload_container_type = NULL;
    ogs_nas_payload_container_t *payload_container = NULL;
    ogs_nas_pdu_session_identity_2_t *pdu_session_id = NULL;
    ogs_nas_request_type_t *request_type = NULL;
    ogs_nas_s_nssai_t *s_nssai = NULL;

    ogs_assert(test_sess);
    test_ue = test_sess->test_ue;
    ogs_assert(test_ue);
    ogs_assert(payload_type);
    ogs_assert(payload);

    payload_container_type = &ul_nas_transport->payload_container_type;
    payload_container = &ul_nas_transport->payload_container;
    pdu_session_id = &ul_nas_transport->pdu_session_id;
    request_type = &ul_nas_transport->request_type;
    s_nssai = &ul_nas_transport->s_nssai;

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_UL_NAS_TRANSPORT;

    payload_container_type->value = payload_type;

    payload_container->length = payload->len;
    payload_container->buffer = payload->data;

    ul_nas_transport->presencemask |=
            OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT;
    *pdu_session_id = test_sess->psi;

    ul_nas_transport->presencemask |=
            OGS_NAS_5GS_UL_NAS_TRANSPORT_REQUEST_TYPE_PRESENT;
    request_type->value = OGS_NAS_5GS_REQUEST_TYPE_INITIAL;

    ul_nas_transport->presencemask |=
            OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_PRESENT;
    s_nssai->length = 4;
    s_nssai->sst = test_self()->plmn_support[0].s_nssai[0].sst;
    s_nssai->sd = ogs_htobe24(test_self()->plmn_support[0].s_nssai[0].sd);

    ul_nas_transport->presencemask |=
            OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_PRESENT;
    ul_nas_transport->dnn.length = strlen(test_sess->dnn);
    ogs_cpystrn(ul_nas_transport->dnn.value, test_sess->dnn,
            ogs_min(ul_nas_transport->dnn.length, OGS_MAX_DNN_LEN) + 1);

    pkbuf = test_nas_5gs_security_encode(test_ue, &message);
    ogs_pkbuf_free(payload);

    return pkbuf;
}
