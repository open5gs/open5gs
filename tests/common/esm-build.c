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

ogs_pkbuf_t *testesm_build_pdn_connectivity_request(
        test_sess_t *sess, bool integrity_protected)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_pdn_connectivity_request_t *pdn_connectivity_request =
        &message.esm.pdn_connectivity_request;
    ogs_nas_request_type_t *request_type =
        &pdn_connectivity_request->request_type;

    ogs_nas_access_point_name_t *access_point_name =
        &pdn_connectivity_request->access_point_name;
    ogs_nas_esm_information_transfer_flag_t *esm_information_transfer_flag =
        &pdn_connectivity_request->esm_information_transfer_flag;
    ogs_nas_protocol_configuration_options_t *protocol_configuration_options =
        &pdn_connectivity_request->protocol_configuration_options;
#if 0
    uint8_t ue_pco[29] =
            "\x80\x80\x21\x10\x01\x01\x00\x10\x81\x06\x00\x00\x00\x00"
            "\x83\x06\x00\x00\x00\x00\x00\x03\x00\x00\x0a\x00\x00\x0d\x00";
    uint8_t ue_pco[7] = "\x80\x00\x0a\x00\x00\x0d\x00";
#else
    uint8_t ue_pco[35] =
        "\x80\x80\x21\x10\x01\x00\x00\x10\x81\x06\x00\x00\x00\x00"
        "\x83\x06\x00\x00\x00\x00\x00\x0c\x00\x00\x0d\x00\x00\x02\x00\x00"
        "\x0a\x00\x00\x10\x00";
#endif

    test_ue_t *test_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));
    if (integrity_protected) {
        message.h.security_header_type =
            OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED;
        message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    }

    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST;

    request_type->type = OGS_NAS_EPS_PDN_TYPE_IPV4V6;
    request_type->value = sess->pdn_connectivity_param.request_type;
    ogs_assert(request_type->value);

    if (sess->pdn_connectivity_param.apn) {
        pdn_connectivity_request->presencemask |=
            OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT;
        ogs_cpystrn(access_point_name->apn, sess->apn,
                ogs_min(strlen(sess->apn), OGS_MAX_APN_LEN) + 1);
        access_point_name->length = strlen(sess->apn);
    }

    if (sess->pdn_connectivity_param.eit ||
        sess->pdn_connectivity_param.eit_no_required) {
        pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT;
        esm_information_transfer_flag->security_protected_required =
            sess->pdn_connectivity_param.eit;
    }

    if (sess->pdn_connectivity_param.pco) {
        pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
        protocol_configuration_options->length = sizeof(ue_pco);
        memcpy(protocol_configuration_options->buffer, ue_pco, sizeof(ue_pco));
    }

    if (integrity_protected)
        return test_nas_eps_security_encode(test_ue, &message);
    else
        return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *testesm_build_pdn_disconnect_request(test_sess_t *sess)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_pdn_disconnect_request_t *pdn_disconnect_request =
        &message.esm.pdn_disconnect_request;
    ogs_nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity =
        &pdn_disconnect_request->linked_eps_bearer_identity;

    test_ue_t *test_ue = NULL;
    test_bearer_t *bearer = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    bearer = ogs_list_first(&sess->bearer_list);
    ogs_assert(bearer);

    memset(&message, 0, sizeof(message));

    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = OGS_NAS_EPS_PDN_DISCONNECT_REQUEST;

    linked_eps_bearer_identity->eps_bearer_identity = bearer->ebi;

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testesm_build_esm_information_response(test_sess_t *sess)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_esm_information_response_t *esm_information_response =
        &message.esm.esm_information_response;

    ogs_nas_access_point_name_t *access_point_name =
        &esm_information_response->access_point_name;
    ogs_nas_protocol_configuration_options_t *protocol_configuration_options =
        &esm_information_response->protocol_configuration_options;

#if 0
    uint8_t ue_pco[29] =
            "\x80\x80\x21\x10\x01\x01\x00\x10\x81\x06\x00\x00\x00\x00"
            "\x83\x06\x00\x00\x00\x00\x00\x03\x00\x00\x0a\x00\x00\x0d\x00";
    uint8_t ue_pco[94] =
        "\x80\xc2\x23"
        "\x23\x01\x01\x00\x23\x10\xec\xa3\x90\x00\x3e\xdb\xf9\x17\xbe\xcf"
        "\xa8\x14\x8a\xcd\xde\x56\x55\x4d\x54\x53\x5f\x43\x48\x41\x50\x5f"
        "\x53\x52\x56\x52\xc2\x23\x15\x02\x01\x00\x15\x10\xb6\xfa\xad\xc5"
        "\x6a\x43\x6b\x2f\x0f\x9f\x82\x35\x6e\x07\xd9\xd9\x80\x21\x1c\x01"
        "\x00\x00\x1c\x81\x06\x00\x00\x00\x00\x82\x06\x00\x00\x00\x00\x83"
        "\x06\x00\x00\x00\x00\x84\x06\x00\x00\x00\x00";
    uint8_t ue_pco[35] =
        "\x80\x80\x21\x10\x01\x00\x00\x10\x81\x06\x00\x00\x00\x00"
        "\x83\x06\x00\x00\x00\x00\x00\x0c\x00\x00\x0d\x00\x00\x02\x00\x00"
        "\x0a\x00\x00\x10\x00";
#endif
    uint8_t ue_pco[47] =
        "\x80\x80\x21\x0a\x01\x0a"
        "\x00\x0a\x81\x06\x00\x00\x00\x00\x80\x21\x0a\x01\x0b\x00\x0a\x83"
        "\x06\x00\x00\x00\x00\xc0\x23\x11\x01\x0c\x00\x11\x03\x72\x69\x6d"
        "\x08\x70\x61\x73\x73\x77\x6f\x72\x64";

    test_ue_t *test_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = OGS_NAS_EPS_ESM_INFORMATION_RESPONSE;

    esm_information_response->presencemask |= OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT;

    ogs_assert(sess->apn);
    ogs_cpystrn(access_point_name->apn, sess->apn,
            ogs_min(strlen(sess->apn), OGS_MAX_APN_LEN) + 1);
    access_point_name->length = strlen(sess->apn);

    if (sess->esm_information_param.pco) {
        esm_information_response->presencemask |= OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
        protocol_configuration_options->length = sizeof(ue_pco);
        memcpy(protocol_configuration_options->buffer, ue_pco, sizeof(ue_pco));
    }

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testesm_build_activate_default_eps_bearer_context_accept(
        test_bearer_t *bearer, bool integrity_protected_and_ciphered)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_activate_default_eps_bearer_context_accept_t
        *activate_default_eps_bearer_context_accept =
            &message.esm.activate_default_eps_bearer_context_accept;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    if (integrity_protected_and_ciphered) {
        message.h.security_header_type =
            OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
    }

    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.message_type =
        OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT;

    if (integrity_protected_and_ciphered)
        return test_nas_eps_security_encode(test_ue, &message);
    else
        return ogs_nas_eps_plain_encode(&message);
}

ogs_pkbuf_t *testesm_build_activate_dedicated_eps_bearer_context_accept(
        test_bearer_t *bearer)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_activate_dedicated_eps_bearer_context_accept_t
        *activate_dedicated_eps_bearer_context_accept =
            &message.esm.activate_dedicated_eps_bearer_context_accept;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.message_type =
        OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT;

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testesm_build_activate_dedicated_eps_bearer_context_reject(
        test_bearer_t *bearer, uint8_t esm_cause)
{
    ogs_nas_eps_message_t message;
    ogs_nas_eps_activate_dedicated_eps_bearer_context_reject_t
        *activate_dedicated_eps_bearer_context_reject =
            &message.esm.activate_dedicated_eps_bearer_context_reject;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.message_type =
        OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT;

    activate_dedicated_eps_bearer_context_reject->esm_cause = esm_cause;

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testesm_build_modify_eps_bearer_context_accept(
        test_bearer_t *bearer)
{
    ogs_nas_eps_message_t message;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT;

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testesm_build_deactivate_eps_bearer_context_accept(
        test_bearer_t *bearer)
{
    ogs_nas_eps_message_t message;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.message_type =
        OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT;

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testesm_build_bearer_resource_allocation_request(
        test_bearer_t *bearer)
{
    ogs_nas_eps_message_t message;

    ogs_nas_eps_bearer_resource_allocation_request_t
        *bearer_resource_allocation_request =
            &message.esm.bearer_resource_allocation_request;

    ogs_nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity =
        &bearer_resource_allocation_request->linked_eps_bearer_identity;
    ogs_nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate =
        &bearer_resource_allocation_request->traffic_flow_aggregate;
    ogs_nas_eps_quality_of_service_t *required_traffic_flow_qos =
        &bearer_resource_allocation_request->required_traffic_flow_qos;

    uint8_t tft[4] = "\x21\x20\x01\x00";

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST;

    linked_eps_bearer_identity->eps_bearer_identity = bearer->ebi;

    traffic_flow_aggregate->length = sizeof(tft);
    memcpy(traffic_flow_aggregate->buffer, tft, traffic_flow_aggregate->length);

    required_traffic_flow_qos->length = 5;
    required_traffic_flow_qos->qci = 1;

    return test_nas_eps_security_encode(test_ue, &message);
}

ogs_pkbuf_t *testesm_build_bearer_resource_modification_request(
    test_bearer_t *bearer, uint8_t tft_code, uint8_t qci, uint8_t esm_cause)
{
    int rv;
    ogs_nas_eps_message_t message;

    ogs_nas_eps_bearer_resource_modification_request_t *req =
            &message.esm.bearer_resource_modification_request;

    ogs_nas_linked_eps_bearer_identity_t
        *eps_bearer_identity_for_packet_filter =
            &req->eps_bearer_identity_for_packet_filter;
    ogs_nas_traffic_flow_aggregate_description_t *tad =
        &req->traffic_flow_aggregate;
    ogs_nas_eps_quality_of_service_t *qos = &req->required_traffic_flow_qos;

    ogs_gtp2_tft_t tft;
    ogs_tlv_octet_t octet;
    ogs_ipsubnet_t ipsubnet;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.h.security_header_type =
       OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type =
        OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST;

    eps_bearer_identity_for_packet_filter->eps_bearer_identity = bearer->ebi;

    memset(&tft, 0, sizeof tft);
    tft.code = tft_code;
    if (tft.code == OGS_GTP2_TFT_CODE_REPLACE_PACKET_FILTERS_IN_EXISTING) {
        tft.num_of_packet_filter = 1;
        tft.pf[0].direction = 1;
        tft.pf[0].identifier = 0;
        tft.pf[0].precedence = 0x0f;
        tft.pf[0].content.length = 9;
        tft.pf[0].content.component[0].type =
            OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "201.20.2.5", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[0].content.component[0].ipv4.addr = ipsubnet.sub[0];
        tft.pf[0].content.component[0].ipv4.mask = ipsubnet.mask[0];
        tft.pf[0].content.num_of_component = 1;
    } else if (tft.code ==
            OGS_GTP2_TFT_CODE_ADD_PACKET_FILTERS_TO_EXISTING_TFT) {
        tft.num_of_packet_filter = 1;
        tft.pf[0].direction = 2;
        tft.pf[0].identifier = 4;
        tft.pf[0].precedence = 0x0f;

        rv = ogs_ipsubnet(&ipsubnet, "2001:db8:cafe::9", "120");
        ogs_assert(rv == OGS_OK);
#if 1
        tft.pf[0].content.length = 18;
        tft.pf[0].content.component[0].type =
            OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;
        memcpy(tft.pf[0].content.component[0].ipv6.addr, ipsubnet.sub,
                sizeof(tft.pf[0].content.component[0].ipv6.addr));
        tft.pf[0].content.component[0].ipv6.prefixlen = 120;
#else
        tft.pf[0].content.length = 33;
        tft.pf[0].content.component[0].type =
            OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_TYPE;
        memcpy(tft.pf[0].content.component[0].ipv6_mask.addr, ipsubnet.sub,
                sizeof(tft.pf[0].content.component[0].ipv6_mask.addr));
        memcpy(tft.pf[0].content.component[0].ipv6_mask.mask, ipsubnet.mask,
                sizeof(tft.pf[0].content.component[0].ipv6_mask.mask));
#endif
        tft.pf[0].content.num_of_component = 1;

    } else if (tft.code == OGS_GTP2_TFT_CODE_CREATE_NEW_TFT) {
        tft.num_of_packet_filter = 4;

        tft.pf[0].direction = 1;
        tft.pf[0].identifier = 0;
        tft.pf[0].precedence = 0x01;
        tft.pf[0].content.length = 0x17;
        tft.pf[0].content.component[0].type =
            OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        tft.pf[0].content.component[0].proto = 0x11; /* UDP */
        tft.pf[0].content.component[1].type =
            OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.20.166.84", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[0].content.component[1].ipv4.addr = ipsubnet.sub[0];
        tft.pf[0].content.component[1].ipv4.mask = ipsubnet.mask[0];
        tft.pf[0].content.component[2].type =
            OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.18.128.20", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[0].content.component[2].ipv4.addr = ipsubnet.sub[0];
        tft.pf[0].content.component[2].ipv4.mask = ipsubnet.mask[0];
        tft.pf[0].content.component[3].type =
            OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
        tft.pf[0].content.component[3].port.low = 20001;
        tft.pf[0].content.num_of_component = 4;

        tft.pf[1].direction = 2;
        tft.pf[1].identifier = 1;
        tft.pf[1].precedence = 0x02;
        tft.pf[1].content.length = 0x17;
        tft.pf[1].content.component[0].type =
            OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        tft.pf[1].content.component[0].proto = 0x11; /* UDP */
        tft.pf[1].content.component[1].type =
            OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.20.166.84", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[1].content.component[1].ipv4.addr = ipsubnet.sub[0];
        tft.pf[1].content.component[1].ipv4.mask = ipsubnet.mask[0];
        tft.pf[1].content.component[2].type =
            OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.18.128.20", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[1].content.component[2].ipv4.addr = ipsubnet.sub[0];
        tft.pf[1].content.component[2].ipv4.mask = ipsubnet.mask[0];
        tft.pf[1].content.component[3].type =
            OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
        tft.pf[1].content.component[3].port.low = 20360;
        tft.pf[1].content.num_of_component = 4;

        tft.pf[2].direction = 1;
        tft.pf[2].identifier = 2;
        tft.pf[2].precedence = 0x03;
        tft.pf[2].content.length = 0x17;
        tft.pf[2].content.component[0].type =
            OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        tft.pf[2].content.component[0].proto = 0x11; /* UDP */
        tft.pf[2].content.component[1].type =
            OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.20.166.84", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[2].content.component[1].ipv4.addr = ipsubnet.sub[0];
        tft.pf[2].content.component[1].ipv4.mask = ipsubnet.mask[0];
        tft.pf[2].content.component[2].type =
            OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.18.128.20", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[2].content.component[2].ipv4.addr = ipsubnet.sub[0];
        tft.pf[2].content.component[2].ipv4.mask = ipsubnet.mask[0];
        tft.pf[2].content.component[3].type =
            OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
        tft.pf[2].content.component[3].port.low = 20002;
        tft.pf[2].content.num_of_component = 4;

        tft.pf[3].direction = 2;
        tft.pf[3].identifier = 3;
        tft.pf[3].precedence = 0x04;
        tft.pf[3].content.length = 0x17;
        tft.pf[3].content.component[0].type =
            OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
        tft.pf[3].content.component[0].proto = 0x11; /* UDP */
        tft.pf[3].content.component[1].type =
            OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.20.166.84", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[3].content.component[1].ipv4.addr = ipsubnet.sub[0];
        tft.pf[3].content.component[1].ipv4.mask = ipsubnet.mask[0];
        tft.pf[3].content.component[2].type =
            OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
        rv = ogs_ipsubnet(&ipsubnet, "172.18.128.20", NULL);
        ogs_assert(rv == OGS_OK);
        tft.pf[3].content.component[2].ipv4.addr = ipsubnet.sub[0];
        tft.pf[3].content.component[2].ipv4.mask = ipsubnet.mask[0];
        tft.pf[3].content.component[3].type =
            OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
        tft.pf[3].content.component[3].port.low = 20361;
        tft.pf[3].content.num_of_component = 4;
    } else if (tft.code ==
            OGS_GTP2_TFT_CODE_DELETE_PACKET_FILTERS_FROM_EXISTING) {
        tft.num_of_packet_filter = 4;
        tft.pf[0].identifier = 0;
        tft.pf[1].identifier = 1;
        tft.pf[2].identifier = 2;
        tft.pf[3].identifier = 3;
    }
    tad->length = ogs_gtp2_build_tft(&octet,
            &tft, tad->buffer, OGS_GTP2_MAX_TRAFFIC_FLOW_TEMPLATE);

    if (qci) {
        req->presencemask |=
            OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT;
        qos->length = 5;
        qos->qci = qci;
        qos->ul_mbr = 44;
        qos->dl_mbr = 55;
        qos->ul_gbr = 22;
        qos->dl_gbr = 33;
    }

    if (esm_cause) {
        req->presencemask |=
            OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_PRESENT;
        req->esm_cause = esm_cause;
    }

    return test_nas_eps_security_encode(test_ue, &message);
}
