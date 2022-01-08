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

void testngap_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_ngap_message_t message;

    NGAP_NGAP_PDU_t *pdu = NULL;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;

    ogs_assert(test_ue);
    ogs_assert(pkbuf);

    rv = ogs_ngap_decode(&message, pkbuf);
    ogs_assert(rv == OGS_OK);

    pdu = &message;
    ogs_assert(pdu);

    switch (pdu->present) {
    case NGAP_NGAP_PDU_PR_initiatingMessage:
        initiatingMessage = pdu->choice.initiatingMessage;
        ogs_assert(initiatingMessage);

        if (test_ue)
            test_ue->ngap_procedure_code = initiatingMessage->procedureCode;

        switch (initiatingMessage->procedureCode) {
        case NGAP_ProcedureCode_id_DownlinkNASTransport:
            testngap_handle_downlink_nas_transport(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_InitialContextSetup:
            testngap_handle_initial_context_setup_request(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_UEContextRelease:
            testngap_handle_ue_release_context_command(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_PDUSessionResourceSetup:
            testngap_handle_pdu_session_resource_setup_request(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_PDUSessionResourceModify:
            testngap_handle_pdu_session_resource_modify_request(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_PDUSessionResourceRelease:
            testngap_handle_pdu_session_resource_release_command(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_DownlinkRANConfigurationTransfer:
            break;
        case NGAP_ProcedureCode_id_HandoverResourceAllocation:
            testngap_handle_handover_request(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_DownlinkRANStatusTransfer:
            testngap_handle_downlink_ran_status_transfer(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_ErrorIndication:
        case NGAP_ProcedureCode_id_Paging:
            /* Nothing */
            break;
        default:
            ogs_error("Not implemented(choice:%d, proc:%d)",
                    pdu->present, (int)initiatingMessage->procedureCode);
            break;
        }
        break;
    case NGAP_NGAP_PDU_PR_successfulOutcome :
        successfulOutcome = pdu->choice.successfulOutcome;
        ogs_assert(successfulOutcome);

        if (test_ue)
            test_ue->ngap_procedure_code = successfulOutcome->procedureCode;

        switch (successfulOutcome->procedureCode) {
        case NGAP_ProcedureCode_id_NGSetup:
            testngap_handle_ng_setup_response(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_PathSwitchRequest:
            break;
        case NGAP_ProcedureCode_id_HandoverPreparation:
            testngap_handle_handover_command(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_HandoverCancel:
            testngap_handle_handover_cancel_ack(test_ue, pdu);
            break;
        case NGAP_ProcedureCode_id_NGReset:
            break;
        default:
            ogs_error("Not implemented(choice:%d, proc:%d)",
                    pdu->present, (int)successfulOutcome->procedureCode);
            break;
        }
        break;
    case NGAP_NGAP_PDU_PR_unsuccessfulOutcome :
        unsuccessfulOutcome = pdu->choice.unsuccessfulOutcome;
        ogs_assert(unsuccessfulOutcome);

        if (test_ue)
            test_ue->ngap_procedure_code = unsuccessfulOutcome->procedureCode;

        switch (unsuccessfulOutcome->procedureCode) {
        case NGAP_ProcedureCode_id_NGSetup:
            break;
        case NGAP_ProcedureCode_id_HandoverPreparation:
            testngap_handle_handover_preparation_failure(test_ue, pdu);
            break;
        default:
            ogs_error("Not implemented(choice:%d, proc:%d)",
                    pdu->present, (int)unsuccessfulOutcome->procedureCode);
            break;
        }
        break;
    default:
        ogs_error("Not implemented(choice:%d)", pdu->present);
        break;
    }

    ogs_ngap_free(&message);
    ogs_pkbuf_free(pkbuf);
}

void testngap_send_to_nas(test_ue_t *test_ue, NGAP_NAS_PDU_t *nasPdu)
{
    ogs_nas_5gs_security_header_t *sh = NULL;
    ogs_nas_security_header_type_t security_header_type;

    ogs_nas_5gmm_header_t *h = NULL;
    ogs_pkbuf_t *nasbuf = NULL;

    ogs_assert(test_ue);
    ogs_assert(nasPdu);

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    nasbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+nasPdu->size);
    ogs_assert(nasbuf);
    ogs_pkbuf_reserve(nasbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(nasbuf, nasPdu->buf, nasPdu->size);

    sh = (ogs_nas_5gs_security_header_t *)nasbuf->data;
    ogs_assert(sh);

    memset(&security_header_type, 0, sizeof(ogs_nas_security_header_type_t));
    switch(sh->security_header_type) {
    case OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED:
        security_header_type.integrity_protected = 1;
        ogs_pkbuf_pull(nasbuf, 7);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED:
        security_header_type.integrity_protected = 1;
        security_header_type.ciphered = 1;
        ogs_pkbuf_pull(nasbuf, 7);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT:
        security_header_type.integrity_protected = 1;
        security_header_type.new_security_context = 1;
        ogs_pkbuf_pull(nasbuf, 7);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT:
        security_header_type.integrity_protected = 1;
        security_header_type.ciphered = 1;
        security_header_type.new_security_context = 1;
        ogs_pkbuf_pull(nasbuf, 7);
        break;
    default:
        ogs_error("Not implemented(security header type:0x%x)",
                sh->security_header_type);
        ogs_assert_if_reached();
    }

    h = (ogs_nas_5gmm_header_t *)nasbuf->data;
    ogs_assert(h);

    if (sh->security_header_type ==
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT &&
        h->message_type == OGS_NAS_5GS_SECURITY_MODE_COMMAND) {
        ogs_nas_5gs_message_t message;
        int rv;

        rv = ogs_nas_5gmm_decode(&message, nasbuf);
        ogs_assert(rv == OGS_OK);

        testgmm_handle_security_mode_command(test_ue,
                &message.gmm.security_mode_command);
    }

    if (test_nas_5gs_security_decode(test_ue,
            security_header_type, nasbuf) != OGS_OK) {
        ogs_error("nas_eps_security_decode failed()");
        ogs_assert_if_reached();
    }

    if (h->extended_protocol_discriminator ==
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM) {
        testgmm_recv(test_ue, nasbuf);
    } else {
        ogs_error("Unknown NAS Protocol discriminator 0x%02x",
                  h->extended_protocol_discriminator);
        ogs_assert_if_reached();
    }
}

