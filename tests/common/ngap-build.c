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

#include "test-ngap.h"

int testngap_build_setup_req(ogs_pkbuf_t **pkbuf, uint32_t gnb_id)
{
    int i, j;
    ogs_plmn_id_t *plmn_id = NULL;
    ogs_uint24_t uint24;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_NGSetupRequest_t *NGSetupRequest = NULL;

    NGAP_NGSetupRequestIEs_t *ie = NULL;
    NGAP_GlobalRANNodeID_t *GlobalRANNodeID = NULL;
    NGAP_GlobalGNB_ID_t *globalGNB_ID = NULL;
    NGAP_SupportedTAList_t *SupportedTAList = NULL;
    NGAP_SupportedTAItem_t *SupportedTAItem = NULL;
    NGAP_BroadcastPLMNItem_t *BroadcastPLMNItem = NULL;
    NGAP_SliceSupportItem_t *SliceSupportItem = NULL;
    NGAP_PLMNIdentity_t *pLMNIdentity = NULL;
    NGAP_PagingDRX_t *PagingDRX = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = 
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_NGSetup;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_NGSetupRequest;

    NGSetupRequest = &initiatingMessage->value.choice.NGSetupRequest;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GlobalRANNodeID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

    GlobalRANNodeID = &ie->value.choice.GlobalRANNodeID;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SupportedTAList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_SupportedTAList;

    SupportedTAList = &ie->value.choice.SupportedTAList;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);
    
    ie->id = NGAP_ProtocolIE_ID_id_DefaultPagingDRX;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_PagingDRX;

    PagingDRX = &ie->value.choice.PagingDRX;

    globalGNB_ID = CALLOC(1, sizeof(NGAP_GlobalGNB_ID_t));

    plmn_id = &test_self()->plmn_support[0].plmn_id;
    ogs_asn_buffer_to_OCTET_STRING(
            plmn_id, OGS_PLMN_ID_LEN, &globalGNB_ID->pLMNIdentity);

    ogs_ngap_uint32_to_GNB_ID(gnb_id, &globalGNB_ID->gNB_ID);

    GlobalRANNodeID->present = NGAP_GlobalRANNodeID_PR_globalGNB_ID;
    GlobalRANNodeID->choice.globalGNB_ID = globalGNB_ID;

    SupportedTAItem = CALLOC(1, sizeof(NGAP_SupportedTAItem_t));
    if (test_self()->served_tai[0].list2.num)
        ogs_asn_uint24_to_OCTET_STRING(
            test_self()->served_tai[0].list2.tai[0].tac, &SupportedTAItem->tAC);
    else if (test_self()->served_tai[0].list0.tai[0].num)
        ogs_asn_uint24_to_OCTET_STRING(
            test_self()->served_tai[0].list0.tai[0].tac[0],
                &SupportedTAItem->tAC);
    else
        ogs_assert_if_reached();

    for (i = 0; i < test_self()->num_of_plmn_support; i++) {
        plmn_id = &test_self()->plmn_support[i].plmn_id;

        BroadcastPLMNItem = CALLOC(1, sizeof(NGAP_BroadcastPLMNItem_t));

        ogs_asn_buffer_to_OCTET_STRING(
                plmn_id, OGS_PLMN_ID_LEN, &BroadcastPLMNItem->pLMNIdentity);

        for (j = 0; j < test_self()->plmn_support[i].num_of_s_nssai; j++) {
            ogs_s_nssai_t *s_nssai = &test_self()->plmn_support[i].s_nssai[j];

            SliceSupportItem = CALLOC(1, sizeof(NGAP_SliceSupportItem_t));
            ogs_asn_uint8_to_OCTET_STRING(s_nssai->sst,
                    &SliceSupportItem->s_NSSAI.sST);
            if (s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                SliceSupportItem->s_NSSAI.sD = CALLOC(1, sizeof(ogs_uint24_t));
                ogs_asn_uint24_to_OCTET_STRING(
                        s_nssai->sd, SliceSupportItem->s_NSSAI.sD);
            }

            ASN_SEQUENCE_ADD(&BroadcastPLMNItem->tAISliceSupportList.list,
                            SliceSupportItem);
        }

        ASN_SEQUENCE_ADD(&SupportedTAItem->broadcastPLMNList.list,
                BroadcastPLMNItem);
    }

    ASN_SEQUENCE_ADD(&SupportedTAList->list, SupportedTAItem);

    *PagingDRX = NGAP_PagingDRX_v64;

    *pkbuf = ogs_ngap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_ngap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

