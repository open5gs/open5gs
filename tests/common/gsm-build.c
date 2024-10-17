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

ogs_pkbuf_t *testgsm_build_pdu_session_establishment_request(
        test_sess_t *test_sess)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_establishment_request_t
        *pdu_session_establishment_request =
            &message.gsm.pdu_session_establishment_request;
    ogs_nas_integrity_protection_maximum_data_rate_t
        *integrity_protection_maximum_data_rate = NULL;
    ogs_nas_pdu_session_type_t *pdu_session_type = NULL;
    ogs_nas_ssc_mode_t *ssc_mode = NULL;
    ogs_nas_extended_protocol_configuration_options_t
        *extended_protocol_configuration_options = NULL;

    /*
     * Challenge Handshake Authentication Protocol(0xc223)
     * Code : Challenge (1)
     *        Response (2)
     */
#if 0
    uint8_t ue_pco[94] =
        "\x80\xc2\x23\x16\x01\x00\x00\x16\x10\xa1\x53\x8b\x8b"
        "\xa1\x53\x8b\x8b\xa1\x53\x8b\x8b\xa1\x53\x8b\x8b\x2a\xc2\x23\x16"
        "\x02\x00\x00\x16\x10\x52\xfb\x1e\xd0\x6e\x58\xd6\x5c\xb6\x3f\x54"
        "\x45\x9c\x94\x84\xaa\x2a\x80\x21\x10\x01\x00\x00\x10\x81\x06\x00"
        "\x00\x00\x00\x83\x06\x00\x00\x00\x00\x00\x0d\x00\x00\x03\x00\x00"
        "\x0a\x00\x00\x05\x00\x00\x10\x00\x00\x11\x00\x00\x23\x00\x00\x24"
        "\x00";
#endif
    uint8_t ue_pco[35] =
        "\x80\x80\x21\x10\x01\x00\x00\x10\x81\x06\x00\x00\x00\x00"
        "\x83\x06\x00\x00\x00\x00\x00\x0c\x00\x00\x0d\x00\x00\x02\x00\x00"
        "\x0a\x00\x00\x10\x00";

    test_ue_t *test_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(test_sess);
    test_ue = test_sess;
    ogs_assert(test_ue);

    integrity_protection_maximum_data_rate =
        &pdu_session_establishment_request->
            integrity_protection_maximum_data_rate;
    pdu_session_type = &pdu_session_establishment_request->pdu_session_type;
    ssc_mode = &pdu_session_establishment_request->ssc_mode;
    extended_protocol_configuration_options =
        &pdu_session_establishment_request->
            extended_protocol_configuration_options;

    memset(&message, 0, sizeof(message));

    message.gsm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = test_sess->psi;
    message.gsm.h.procedure_transaction_identity = test_sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST;

    integrity_protection_maximum_data_rate->ul = 0xff;
    integrity_protection_maximum_data_rate->dl = 0xff;

    pdu_session_establishment_request->presencemask |=
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT;
    pdu_session_type->value = test_sess->pdu_session_type;

    if (test_sess->pdu_session_establishment_param.ssc_mode) {
        pdu_session_establishment_request->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT;
        ssc_mode->value = OGS_NAS_SSC_MODE_1;
    }

    if (test_sess->pdu_session_establishment_param.epco) {
        pdu_session_establishment_request->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
        extended_protocol_configuration_options->length = sizeof(ue_pco);
        extended_protocol_configuration_options->buffer = ue_pco;
    }

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *testgsm_build_pdu_session_modification_request(
        test_bearer_t *qos_flow, uint8_t gsm_cause,
        uint8_t qos_rule_code, uint8_t qos_flow_description_code)
{
    int num_of_param, rv;

    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_modification_request_t
        *pdu_session_modification_request =
            &message.gsm.pdu_session_modification_request;

    ogs_nas_qos_rule_t qos_rule[OGS_NAS_MAX_NUM_OF_QOS_RULE];
    ogs_nas_qos_flow_description_t
        qos_flow_description[OGS_NAS_MAX_NUM_OF_QOS_FLOW_DESCRIPTION];

    ogs_nas_qos_rules_t *requested_qos_rules =
        &pdu_session_modification_request->requested_qos_rules;
    ogs_nas_qos_flow_descriptions_t *requested_qos_flow_descriptions =
        &pdu_session_modification_request->requested_qos_flow_descriptions;

    test_ue_t *test_ue = NULL;
    test_sess_t *test_sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_ipsubnet_t ipsubnet;

    ogs_assert(qos_flow);
    test_sess = qos_flow->sess;
    ogs_assert(test_sess);
    test_ue = test_sess;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.gsm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = test_sess->psi;
    message.gsm.h.procedure_transaction_identity = test_sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST;

    if (gsm_cause) {
        pdu_session_modification_request->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT;
        pdu_session_modification_request->gsm_cause = gsm_cause;
    }
    if (qos_rule_code) {
        pdu_session_modification_request->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT;

        memset(qos_rule, 0, sizeof(qos_rule));
        qos_rule[0].identifier = qos_flow->qfi; /* Use QFI in Open5GS */
        qos_rule[0].code = qos_rule_code;

        if (qos_rule_code ==
                OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS) {
            qos_rule[0].DQR_bit = 0;
            qos_rule[0].num_of_packet_filter = 1;

            qos_rule[0].pf[0].direction = OGS_NAS_QOS_DIRECTION_UPLINK;
            qos_rule[0].pf[0].identifier = 0;

            qos_rule[0].pf[0].content.length = 18;
            qos_rule[0].pf[0].content.num_of_component = 1;
            qos_rule[0].pf[0].content.component[0].type =
                OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;

            rv = ogs_ipsubnet(&ipsubnet, "2001:db8:cafe::9", "120");
            ogs_assert(rv == OGS_OK);

            memcpy(qos_rule[0].pf[0].content.component[0].ipv6.addr,
                    ipsubnet.sub,
                    sizeof(qos_rule[0].pf[0].content.component[0].ipv6.addr));
            qos_rule[0].pf[0].content.component[0].ipv6.prefixlen = 120;

        } else if (qos_rule_code ==
                OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS) {
            qos_rule[0].DQR_bit = 0;
            qos_rule[0].num_of_packet_filter = 1;

            qos_rule[0].pf[0].direction = OGS_NAS_QOS_DIRECTION_DOWNLINK;
            qos_rule[0].pf[0].identifier = 0;

            qos_rule[0].pf[0].content.length = 9;
            qos_rule[0].pf[0].content.num_of_component = 1;
            qos_rule[0].pf[0].content.component[0].type =
                OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;

            rv = ogs_ipsubnet(&ipsubnet, "201.20.2.5", NULL);
            ogs_assert(rv == OGS_OK);

            qos_rule[0].pf[0].content.component[0].ipv4.addr = ipsubnet.sub[0];
            qos_rule[0].pf[0].content.component[0].ipv4.mask = ipsubnet.mask[0];

        } else if (qos_rule_code ==
                OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS) {
            qos_rule[0].DQR_bit = 0;
            qos_rule[0].num_of_packet_filter = 4;

            qos_rule[0].pf[0].identifier = 1;
            qos_rule[0].pf[1].identifier = 2;
            qos_rule[0].pf[2].identifier = 3;
            qos_rule[0].pf[3].identifier = 4;

        } else {
            ogs_fatal("Unknown qos_rule_code[%d]", qos_rule_code);
            ogs_assert_if_reached();
        }

        qos_rule[0].precedence = 0x0f; /* lowest precedence */
        qos_rule[0].flow.segregation = 0;
        qos_rule[0].flow.identifier = qos_flow->qfi;

        rv = ogs_nas_build_qos_rules(requested_qos_rules, qos_rule, 1);
        ogs_assert(rv == OGS_OK);
    }
    if (qos_flow_description_code) {
        pdu_session_modification_request->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT;

        memset(&qos_flow_description, 0, sizeof(qos_flow_description));
        qos_flow_description[0].identifier = qos_flow->qfi;
        qos_flow_description[0].code = qos_flow_description_code;

        num_of_param = 0;

        if (qos_flow_description_code !=
                OGS_NAS_DELETE_NEW_QOS_FLOW_DESCRIPTION) {
            qos_flow_description[0].E_bit = 1;

            qos_flow_description[0].param[num_of_param].identifier =
                OGS_NAX_QOS_FLOW_PARAMETER_ID_5QI;
            qos_flow_description[0].param[num_of_param].len =
                sizeof(qos_flow_description[0].param[num_of_param].qos_index);
            qos_flow_description[0].param[num_of_param].qos_index = 5;
            num_of_param++;

            qos_flow_description[0].param[num_of_param].identifier =
                OGS_NAX_QOS_FLOW_PARAMETER_ID_GFBR_UPLINK;
            qos_flow_description[0].param[num_of_param].len =
                sizeof(qos_flow_description[0].param[num_of_param].br);
            ogs_nas_bitrate_from_uint64(
                    &qos_flow_description[0].param[num_of_param].br,
                    32*1000);
            num_of_param++;
            qos_flow_description[0].param[num_of_param].identifier =
                OGS_NAX_QOS_FLOW_PARAMETER_ID_GFBR_DOWNLINK;
            qos_flow_description[0].param[num_of_param].len =
                sizeof(qos_flow_description[0].param[num_of_param].br);
            ogs_nas_bitrate_from_uint64(
                    &qos_flow_description[0].param[num_of_param].br,
                    48*1000);
            num_of_param++;
            qos_flow_description[0].param[num_of_param].identifier =
                OGS_NAX_QOS_FLOW_PARAMETER_ID_MFBR_UPLINK;
            qos_flow_description[0].param[num_of_param].len =
                sizeof(qos_flow_description[0].param[num_of_param].br);
            ogs_nas_bitrate_from_uint64(
                    &qos_flow_description[0].param[num_of_param].br,
                    16*1000);
            num_of_param++;
            qos_flow_description[0].param[num_of_param].identifier =
                OGS_NAX_QOS_FLOW_PARAMETER_ID_MFBR_DOWNLINK;
            qos_flow_description[0].param[num_of_param].len =
                sizeof(qos_flow_description[0].param[num_of_param].br);
            ogs_nas_bitrate_from_uint64(
                    &qos_flow_description[0].param[num_of_param].br,
                    64*1000);
            num_of_param++;
        }

        qos_flow_description[0].num_of_parameter = num_of_param;

        rv = ogs_nas_build_qos_flow_descriptions(
                requested_qos_flow_descriptions, qos_flow_description, 1);
    }

    pkbuf = ogs_nas_5gs_plain_encode(&message);

    ogs_free(requested_qos_rules->buffer);
    ogs_free(requested_qos_flow_descriptions->buffer);

    return pkbuf;
}

ogs_pkbuf_t *testgsm_build_pdu_session_modification_complete(
        test_sess_t *test_sess)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_modification_complete_t
        *pdu_session_modification_complete =
            &message.gsm.pdu_session_modification_complete;

    test_ue_t *test_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(test_sess);
    test_ue = test_sess;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.gsm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = test_sess->psi;
    message.gsm.h.procedure_transaction_identity = test_sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE;

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *testgsm_build_pdu_session_release_request(test_sess_t *test_sess)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_release_request_t *pdu_session_release_request =
            &message.gsm.pdu_session_release_request;

    test_ue_t *test_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(test_sess);
    test_ue = test_sess;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.gsm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = test_sess->psi;
    message.gsm.h.procedure_transaction_identity = test_sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST;

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *testgsm_build_pdu_session_release_complete(test_sess_t *test_sess)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_release_complete_t *pdu_session_release_complete =
            &message.gsm.pdu_session_release_complete;

    test_ue_t *test_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(test_sess);
    test_ue = test_sess;
    ogs_assert(test_ue);

    memset(&message, 0, sizeof(message));

    message.gsm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = test_sess->psi;
    message.gsm.h.procedure_transaction_identity = test_sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE;

    return ogs_nas_5gs_plain_encode(&message);
}
