/*
 * Copyright (c) 2015, EURECOM (www.eurecom.fr)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 */

/*******************************************************************************
 * This file had been created by asn1tostruct.py script v1.0.2
 * Please do not modify this file but regenerate it via script.
 * Created on: 2017-08-01 09:50:55.515948 by acetcom
 * from ['S1AP-PDU.asn']
 ******************************************************************************/
#define TRACE_MODULE _s1ap_ies_decoder
#include "core_debug.h"
#include "s1ap_ies_defs.h"

int s1ap_decode_s1ap_deactivatetraceies(
    S1ap_DeactivateTraceIEs_t *s1ap_DeactivateTraceIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_DeactivateTrace_t  s1ap_DeactivateTrace;
    S1ap_DeactivateTrace_t *s1ap_DeactivateTrace_p = &s1ap_DeactivateTrace;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_DeactivateTraceIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_DeactivateTraceIEs\n");

    memset(&s1ap_DeactivateTrace, 0, sizeof(S1ap_DeactivateTrace_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_DeactivateTrace, (void **)&s1ap_DeactivateTrace_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_DeactivateTrace failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DeactivateTrace, s1ap_DeactivateTrace_p);
        return -1;
    }

    for (i = 0; i < s1ap_DeactivateTrace_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_DeactivateTrace_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_DeactivateTraceIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DeactivateTrace, s1ap_DeactivateTrace_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_DeactivateTraceIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DeactivateTrace, s1ap_DeactivateTrace_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_UTRAN_Trace_ID:
            {
                S1ap_E_UTRAN_Trace_ID_t *s1apEUTRANTraceID_p = &s1ap_DeactivateTraceIEs->e_UTRAN_Trace_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_UTRAN_Trace_ID, (void **)&s1apEUTRANTraceID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANTraceID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DeactivateTrace, s1ap_DeactivateTrace_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_deactivatetraceies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DeactivateTrace, s1ap_DeactivateTrace_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DeactivateTrace, s1ap_DeactivateTrace_p);
    return 0;
}

int s1ap_decode_s1ap_enbdirectinformationtransferies(
    S1ap_ENBDirectInformationTransferIEs_t *s1ap_ENBDirectInformationTransferIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_ENBDirectInformationTransfer_t  s1ap_ENBDirectInformationTransfer;
    S1ap_ENBDirectInformationTransfer_t *s1ap_ENBDirectInformationTransfer_p = &s1ap_ENBDirectInformationTransfer;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ENBDirectInformationTransferIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ENBDirectInformationTransferIEs\n");

    memset(&s1ap_ENBDirectInformationTransfer, 0, sizeof(S1ap_ENBDirectInformationTransfer_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENBDirectInformationTransfer, (void **)&s1ap_ENBDirectInformationTransfer_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_ENBDirectInformationTransfer failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBDirectInformationTransfer, s1ap_ENBDirectInformationTransfer_p);
        return -1;
    }

    for (i = 0; i < s1ap_ENBDirectInformationTransfer_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_ENBDirectInformationTransfer_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Inter_SystemInformationTransferTypeEDT:
            {
                S1ap_Inter_SystemInformationTransferType_t *s1apInterSystemInformationTransferType_p = &s1ap_ENBDirectInformationTransferIEs->inter_SystemInformationTransferTypeEDT;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Inter_SystemInformationTransferType, (void **)&s1apInterSystemInformationTransferType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apInterSystemInformationTransferType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBDirectInformationTransfer, s1ap_ENBDirectInformationTransfer_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_enbdirectinformationtransferies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBDirectInformationTransfer, s1ap_ENBDirectInformationTransfer_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBDirectInformationTransfer, s1ap_ENBDirectInformationTransfer_p);
    return 0;
}

int s1ap_decode_s1ap_e_rabreleaseresponseies(
    S1ap_E_RABReleaseResponseIEs_t *s1ap_E_RABReleaseResponseIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_E_RABReleaseResponse_t  s1ap_E_RABReleaseResponse;
    S1ap_E_RABReleaseResponse_t *s1ap_E_RABReleaseResponse_p = &s1ap_E_RABReleaseResponse;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_E_RABReleaseResponseIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_E_RABReleaseResponseIEs\n");

    memset(&s1ap_E_RABReleaseResponse, 0, sizeof(S1ap_E_RABReleaseResponse_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_RABReleaseResponse, (void **)&s1ap_E_RABReleaseResponse_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_E_RABReleaseResponse failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseResponse, s1ap_E_RABReleaseResponse_p);
        return -1;
    }

    for (i = 0; i < s1ap_E_RABReleaseResponse_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_E_RABReleaseResponse_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_E_RABReleaseResponseIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseResponse, s1ap_E_RABReleaseResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_E_RABReleaseResponseIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseResponse, s1ap_E_RABReleaseResponse_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABReleaseListBearerRelComp:
            {
                S1ap_E_RABReleaseListBearerRelComp_t *s1apERABReleaseListBearerRelComp_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABReleaseListBearerRelComp, (void**)&s1apERABReleaseListBearerRelComp_p);
                if (s1ap_decode_s1ap_e_rabreleaselistbearerrelcomp(&s1ap_E_RABReleaseResponseIEs->e_RABReleaseListBearerRelComp, s1apERABReleaseListBearerRelComp_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABReleaseListBearerRelComp failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABReleaseListBearerRelComp, s1apERABReleaseListBearerRelComp_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseResponse, s1ap_E_RABReleaseResponse_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABReleaseListBearerRelComp, s1apERABReleaseListBearerRelComp_p);
                s1ap_E_RABReleaseResponseIEs->presenceMask |= S1AP_E_RABRELEASERESPONSEIES_E_RABRELEASELISTBEARERRELCOMP_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABFailedToReleaseList:
            {
                S1ap_E_RABList_t *s1apERABList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABList, (void**)&s1apERABList_p);
                if (s1ap_decode_s1ap_e_rablist(&s1ap_E_RABReleaseResponseIEs->e_RABFailedToReleaseList, s1apERABList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseResponse, s1ap_E_RABReleaseResponse_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                s1ap_E_RABReleaseResponseIEs->presenceMask |= S1AP_E_RABRELEASERESPONSEIES_E_RABFAILEDTORELEASELIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_E_RABReleaseResponseIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseResponse, s1ap_E_RABReleaseResponse_p);
                    return -1;
                }
                s1ap_E_RABReleaseResponseIEs->presenceMask |= S1AP_E_RABRELEASERESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_e_rabreleaseresponseies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseResponse, s1ap_E_RABReleaseResponse_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseResponse, s1ap_E_RABReleaseResponse_p);
    return 0;
}

int s1ap_decode_s1ap_uplinknonueassociatedlppatransport_ies(
    S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkNonUEAssociatedLPPaTransport_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UplinkNonUEAssociatedLPPaTransport_t  s1ap_UplinkNonUEAssociatedLPPaTransport;
    S1ap_UplinkNonUEAssociatedLPPaTransport_t *s1ap_UplinkNonUEAssociatedLPPaTransport_p = &s1ap_UplinkNonUEAssociatedLPPaTransport;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UplinkNonUEAssociatedLPPaTransport_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UplinkNonUEAssociatedLPPaTransport_IEs\n");

    memset(&s1ap_UplinkNonUEAssociatedLPPaTransport, 0, sizeof(S1ap_UplinkNonUEAssociatedLPPaTransport_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UplinkNonUEAssociatedLPPaTransport, (void **)&s1ap_UplinkNonUEAssociatedLPPaTransport_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UplinkNonUEAssociatedLPPaTransport failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNonUEAssociatedLPPaTransport, s1ap_UplinkNonUEAssociatedLPPaTransport_p);
        return -1;
    }

    for (i = 0; i < s1ap_UplinkNonUEAssociatedLPPaTransport_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UplinkNonUEAssociatedLPPaTransport_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Routing_ID:
            {
                S1ap_Routing_ID_t *s1apRoutingID_p = &s1ap_UplinkNonUEAssociatedLPPaTransport_IEs->routing_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Routing_ID, (void **)&s1apRoutingID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRoutingID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNonUEAssociatedLPPaTransport, s1ap_UplinkNonUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_LPPa_PDU:
            {
                S1ap_LPPa_PDU_t *s1apLPPaPDU_p = &s1ap_UplinkNonUEAssociatedLPPaTransport_IEs->lpPa_PDU;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LPPa_PDU, (void **)&s1apLPPaPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apLPPaPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNonUEAssociatedLPPaTransport, s1ap_UplinkNonUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinknonueassociatedlppatransport_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNonUEAssociatedLPPaTransport, s1ap_UplinkNonUEAssociatedLPPaTransport_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNonUEAssociatedLPPaTransport, s1ap_UplinkNonUEAssociatedLPPaTransport_p);
    return 0;
}

int s1ap_decode_s1ap_tracestarties(
    S1ap_TraceStartIEs_t *s1ap_TraceStartIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_TraceStart_t  s1ap_TraceStart;
    S1ap_TraceStart_t *s1ap_TraceStart_p = &s1ap_TraceStart;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_TraceStartIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_TraceStartIEs\n");

    memset(&s1ap_TraceStart, 0, sizeof(S1ap_TraceStart_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TraceStart, (void **)&s1ap_TraceStart_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_TraceStart failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceStart, s1ap_TraceStart_p);
        return -1;
    }

    for (i = 0; i < s1ap_TraceStart_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_TraceStart_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_TraceStartIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceStart, s1ap_TraceStart_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_TraceStartIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceStart, s1ap_TraceStart_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TraceActivation:
            {
                S1ap_TraceActivation_t *s1apTraceActivation_p = &s1ap_TraceStartIEs->traceActivation;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TraceActivation, (void **)&s1apTraceActivation_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTraceActivation failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceStart, s1ap_TraceStart_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_tracestarties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceStart, s1ap_TraceStart_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceStart, s1ap_TraceStart_p);
    return 0;
}

int s1ap_decode_s1ap_writereplacewarningresponseies(
    S1ap_WriteReplaceWarningResponseIEs_t *s1ap_WriteReplaceWarningResponseIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_WriteReplaceWarningResponse_t  s1ap_WriteReplaceWarningResponse;
    S1ap_WriteReplaceWarningResponse_t *s1ap_WriteReplaceWarningResponse_p = &s1ap_WriteReplaceWarningResponse;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_WriteReplaceWarningResponseIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_WriteReplaceWarningResponseIEs\n");

    memset(&s1ap_WriteReplaceWarningResponse, 0, sizeof(S1ap_WriteReplaceWarningResponse_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_WriteReplaceWarningResponse, (void **)&s1ap_WriteReplaceWarningResponse_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_WriteReplaceWarningResponse failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningResponse, s1ap_WriteReplaceWarningResponse_p);
        return -1;
    }

    for (i = 0; i < s1ap_WriteReplaceWarningResponse_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_WriteReplaceWarningResponse_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MessageIdentifier:
            {
                S1ap_MessageIdentifier_t *s1apMessageIdentifier_p = &s1ap_WriteReplaceWarningResponseIEs->messageIdentifier;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MessageIdentifier, (void **)&s1apMessageIdentifier_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMessageIdentifier failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningResponse, s1ap_WriteReplaceWarningResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_SerialNumber:
            {
                S1ap_SerialNumber_t *s1apSerialNumber_p = &s1ap_WriteReplaceWarningResponseIEs->serialNumber;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SerialNumber, (void **)&s1apSerialNumber_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSerialNumber failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningResponse, s1ap_WriteReplaceWarningResponse_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_BroadcastCompletedAreaList:
            {
                S1ap_BroadcastCompletedAreaList_t *s1apBroadcastCompletedAreaList_p = &s1ap_WriteReplaceWarningResponseIEs->broadcastCompletedAreaList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_BroadcastCompletedAreaList, (void **)&s1apBroadcastCompletedAreaList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apBroadcastCompletedAreaList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningResponse, s1ap_WriteReplaceWarningResponse_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningResponseIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGRESPONSEIES_BROADCASTCOMPLETEDAREALIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_WriteReplaceWarningResponseIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningResponse, s1ap_WriteReplaceWarningResponse_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningResponseIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_writereplacewarningresponseies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningResponse, s1ap_WriteReplaceWarningResponse_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningResponse, s1ap_WriteReplaceWarningResponse_p);
    return 0;
}

int s1ap_decode_s1ap_handovercancelies(
    S1ap_HandoverCancelIEs_t *s1ap_HandoverCancelIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverCancel_t  s1ap_HandoverCancel;
    S1ap_HandoverCancel_t *s1ap_HandoverCancel_p = &s1ap_HandoverCancel;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverCancelIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverCancelIEs\n");

    memset(&s1ap_HandoverCancel, 0, sizeof(S1ap_HandoverCancel_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverCancel, (void **)&s1ap_HandoverCancel_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverCancel failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancel, s1ap_HandoverCancel_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverCancel_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverCancel_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverCancelIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancel, s1ap_HandoverCancel_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_HandoverCancelIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancel, s1ap_HandoverCancel_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_HandoverCancelIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancel, s1ap_HandoverCancel_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handovercancelies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancel, s1ap_HandoverCancel_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancel, s1ap_HandoverCancel_p);
    return 0;
}

int s1ap_decode_s1ap_handoverpreparationfailureies(
    S1ap_HandoverPreparationFailureIEs_t *s1ap_HandoverPreparationFailureIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverPreparationFailure_t  s1ap_HandoverPreparationFailure;
    S1ap_HandoverPreparationFailure_t *s1ap_HandoverPreparationFailure_p = &s1ap_HandoverPreparationFailure;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverPreparationFailureIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverPreparationFailureIEs\n");

    memset(&s1ap_HandoverPreparationFailure, 0, sizeof(S1ap_HandoverPreparationFailure_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverPreparationFailure, (void **)&s1ap_HandoverPreparationFailure_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverPreparationFailure failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverPreparationFailure, s1ap_HandoverPreparationFailure_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverPreparationFailure_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverPreparationFailure_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverPreparationFailureIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverPreparationFailure, s1ap_HandoverPreparationFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_HandoverPreparationFailureIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverPreparationFailure, s1ap_HandoverPreparationFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_HandoverPreparationFailureIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverPreparationFailure, s1ap_HandoverPreparationFailure_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_HandoverPreparationFailureIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverPreparationFailure, s1ap_HandoverPreparationFailure_p);
                    return -1;
                }
                s1ap_HandoverPreparationFailureIEs->presenceMask |= S1AP_HANDOVERPREPARATIONFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handoverpreparationfailureies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverPreparationFailure, s1ap_HandoverPreparationFailure_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverPreparationFailure, s1ap_HandoverPreparationFailure_p);
    return 0;
}

int s1ap_decode_s1ap_uecontextreleaserequest_ies(
    S1ap_UEContextReleaseRequest_IEs_t *s1ap_UEContextReleaseRequest_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UEContextReleaseRequest_t  s1ap_UEContextReleaseRequest;
    S1ap_UEContextReleaseRequest_t *s1ap_UEContextReleaseRequest_p = &s1ap_UEContextReleaseRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UEContextReleaseRequest_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UEContextReleaseRequest_IEs\n");

    memset(&s1ap_UEContextReleaseRequest, 0, sizeof(S1ap_UEContextReleaseRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEContextReleaseRequest, (void **)&s1ap_UEContextReleaseRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UEContextReleaseRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseRequest, s1ap_UEContextReleaseRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_UEContextReleaseRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UEContextReleaseRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UEContextReleaseRequest_IEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseRequest, s1ap_UEContextReleaseRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UEContextReleaseRequest_IEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseRequest, s1ap_UEContextReleaseRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_UEContextReleaseRequest_IEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseRequest, s1ap_UEContextReleaseRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_GWContextReleaseIndication:
            {
                S1ap_GWContextReleaseIndication_t *s1apGWContextReleaseIndication_p = &s1ap_UEContextReleaseRequest_IEs->gwContextReleaseIndication;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_GWContextReleaseIndication, (void **)&s1apGWContextReleaseIndication_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apGWContextReleaseIndication failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseRequest, s1ap_UEContextReleaseRequest_p);
                    return -1;
                }
                s1ap_UEContextReleaseRequest_IEs->presenceMask |= S1AP_UECONTEXTRELEASEREQUEST_IES_GWCONTEXTRELEASEINDICATION_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uecontextreleaserequest_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseRequest, s1ap_UEContextReleaseRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseRequest, s1ap_UEContextReleaseRequest_p);
    return 0;
}

int s1ap_decode_s1ap_initialuemessage_ies(
    S1ap_InitialUEMessage_IEs_t *s1ap_InitialUEMessage_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_InitialUEMessage_t  s1ap_InitialUEMessage;
    S1ap_InitialUEMessage_t *s1ap_InitialUEMessage_p = &s1ap_InitialUEMessage;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_InitialUEMessage_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_InitialUEMessage_IEs\n");

    memset(&s1ap_InitialUEMessage, 0, sizeof(S1ap_InitialUEMessage_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_InitialUEMessage, (void **)&s1ap_InitialUEMessage_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_InitialUEMessage failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
        return -1;
    }

    for (i = 0; i < s1ap_InitialUEMessage_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_InitialUEMessage_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_InitialUEMessage_IEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_NAS_PDU:
            {
                S1ap_NAS_PDU_t *s1apNASPDU_p = &s1ap_InitialUEMessage_IEs->nas_pdu;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_NAS_PDU, (void **)&s1apNASPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apNASPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TAI:
            {
                S1ap_TAI_t *s1apTAI_p = &s1ap_InitialUEMessage_IEs->tai;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TAI, (void **)&s1apTAI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTAI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_EUTRAN_CGI:
            {
                S1ap_EUTRAN_CGI_t *s1apEUTRANCGI_p = &s1ap_InitialUEMessage_IEs->eutran_cgi;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_EUTRAN_CGI, (void **)&s1apEUTRANCGI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANCGI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_RRC_Establishment_Cause:
            {
                S1ap_RRC_Establishment_Cause_t *s1apRRCEstablishmentCause_p = &s1ap_InitialUEMessage_IEs->rrC_Establishment_Cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_RRC_Establishment_Cause, (void **)&s1apRRCEstablishmentCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRRCEstablishmentCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_S_TMSI:
            {
                S1ap_S_TMSI_t *s1apSTMSI_p = &s1ap_InitialUEMessage_IEs->s_tmsi;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_S_TMSI, (void **)&s1apSTMSI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSTMSI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
                s1ap_InitialUEMessage_IEs->presenceMask |= S1AP_INITIALUEMESSAGE_IES_S_TMSI_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSG_Id:
            {
                S1ap_CSG_Id_t *s1apCSGId_p = &s1ap_InitialUEMessage_IEs->csG_Id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSG_Id, (void **)&s1apCSGId_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGId failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
                s1ap_InitialUEMessage_IEs->presenceMask |= S1AP_INITIALUEMESSAGE_IES_CSG_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_GUMMEI_ID:
            {
                S1ap_GUMMEI_t *s1apGUMMEI_p = &s1ap_InitialUEMessage_IEs->gummei_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_GUMMEI, (void **)&s1apGUMMEI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apGUMMEI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
                s1ap_InitialUEMessage_IEs->presenceMask |= S1AP_INITIALUEMESSAGE_IES_GUMMEI_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CellAccessMode:
            {
                S1ap_CellAccessMode_t *s1apCellAccessMode_p = &s1ap_InitialUEMessage_IEs->cellAccessMode;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CellAccessMode, (void **)&s1apCellAccessMode_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCellAccessMode failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
                s1ap_InitialUEMessage_IEs->presenceMask |= S1AP_INITIALUEMESSAGE_IES_CELLACCESSMODE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_GW_TransportLayerAddress:
            {
                S1ap_TransportLayerAddress_t *s1apTransportLayerAddress_p = &s1ap_InitialUEMessage_IEs->gW_TransportLayerAddress;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TransportLayerAddress, (void **)&s1apTransportLayerAddress_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTransportLayerAddress failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
                s1ap_InitialUEMessage_IEs->presenceMask |= S1AP_INITIALUEMESSAGE_IES_GW_TRANSPORTLAYERADDRESS_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_RelayNode_Indicator:
            {
                S1ap_RelayNode_Indicator_t *s1apRelayNodeIndicator_p = &s1ap_InitialUEMessage_IEs->relayNode_Indicator;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_RelayNode_Indicator, (void **)&s1apRelayNodeIndicator_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRelayNodeIndicator failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                    return -1;
                }
                s1ap_InitialUEMessage_IEs->presenceMask |= S1AP_INITIALUEMESSAGE_IES_RELAYNODE_INDICATOR_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_initialuemessage_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialUEMessage, s1ap_InitialUEMessage_p);
    return 0;
}

int s1ap_decode_s1ap_killrequesties(
    S1ap_KillRequestIEs_t *s1ap_KillRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_KillRequest_t  s1ap_KillRequest;
    S1ap_KillRequest_t *s1ap_KillRequest_p = &s1ap_KillRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_KillRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_KillRequestIEs\n");

    memset(&s1ap_KillRequest, 0, sizeof(S1ap_KillRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_KillRequest, (void **)&s1ap_KillRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_KillRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillRequest, s1ap_KillRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_KillRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_KillRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MessageIdentifier:
            {
                S1ap_MessageIdentifier_t *s1apMessageIdentifier_p = &s1ap_KillRequestIEs->messageIdentifier;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MessageIdentifier, (void **)&s1apMessageIdentifier_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMessageIdentifier failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillRequest, s1ap_KillRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_SerialNumber:
            {
                S1ap_SerialNumber_t *s1apSerialNumber_p = &s1ap_KillRequestIEs->serialNumber;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SerialNumber, (void **)&s1apSerialNumber_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSerialNumber failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillRequest, s1ap_KillRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_WarningAreaList:
            {
                S1ap_WarningAreaList_t *s1apWarningAreaList_p = &s1ap_KillRequestIEs->warningAreaList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_WarningAreaList, (void **)&s1apWarningAreaList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apWarningAreaList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillRequest, s1ap_KillRequest_p);
                    return -1;
                }
                s1ap_KillRequestIEs->presenceMask |= S1AP_KILLREQUESTIES_WARNINGAREALIST_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_killrequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillRequest, s1ap_KillRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillRequest, s1ap_KillRequest_p);
    return 0;
}

int s1ap_decode_s1ap_writereplacewarningrequesties(
    S1ap_WriteReplaceWarningRequestIEs_t *s1ap_WriteReplaceWarningRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_WriteReplaceWarningRequest_t  s1ap_WriteReplaceWarningRequest;
    S1ap_WriteReplaceWarningRequest_t *s1ap_WriteReplaceWarningRequest_p = &s1ap_WriteReplaceWarningRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_WriteReplaceWarningRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_WriteReplaceWarningRequestIEs\n");

    memset(&s1ap_WriteReplaceWarningRequest, 0, sizeof(S1ap_WriteReplaceWarningRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_WriteReplaceWarningRequest, (void **)&s1ap_WriteReplaceWarningRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_WriteReplaceWarningRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_WriteReplaceWarningRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_WriteReplaceWarningRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MessageIdentifier:
            {
                S1ap_MessageIdentifier_t *s1apMessageIdentifier_p = &s1ap_WriteReplaceWarningRequestIEs->messageIdentifier;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MessageIdentifier, (void **)&s1apMessageIdentifier_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMessageIdentifier failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_SerialNumber:
            {
                S1ap_SerialNumber_t *s1apSerialNumber_p = &s1ap_WriteReplaceWarningRequestIEs->serialNumber;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SerialNumber, (void **)&s1apSerialNumber_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSerialNumber failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_WarningAreaList:
            {
                S1ap_WarningAreaList_t *s1apWarningAreaList_p = &s1ap_WriteReplaceWarningRequestIEs->warningAreaList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_WarningAreaList, (void **)&s1apWarningAreaList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apWarningAreaList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningRequestIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGAREALIST_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_RepetitionPeriod:
            {
                S1ap_RepetitionPeriod_t *s1apRepetitionPeriod_p = &s1ap_WriteReplaceWarningRequestIEs->repetitionPeriod;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_RepetitionPeriod, (void **)&s1apRepetitionPeriod_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRepetitionPeriod failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_ExtendedRepetitionPeriod:
            {
                S1ap_ExtendedRepetitionPeriod_t *s1apExtendedRepetitionPeriod_p = &s1ap_WriteReplaceWarningRequestIEs->extendedRepetitionPeriod;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ExtendedRepetitionPeriod, (void **)&s1apExtendedRepetitionPeriod_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apExtendedRepetitionPeriod failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningRequestIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGREQUESTIES_EXTENDEDREPETITIONPERIOD_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_NumberofBroadcastRequest:
            {
                S1ap_NumberofBroadcastRequest_t *s1apNumberofBroadcastRequest_p = &s1ap_WriteReplaceWarningRequestIEs->numberofBroadcastRequest;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_NumberofBroadcastRequest, (void **)&s1apNumberofBroadcastRequest_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apNumberofBroadcastRequest failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_WarningType:
            {
                S1ap_WarningType_t *s1apWarningType_p = &s1ap_WriteReplaceWarningRequestIEs->warningType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_WarningType, (void **)&s1apWarningType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apWarningType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningRequestIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGTYPE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_WarningSecurityInfo:
            {
                S1ap_WarningSecurityInfo_t *s1apWarningSecurityInfo_p = &s1ap_WriteReplaceWarningRequestIEs->warningSecurityInfo;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_WarningSecurityInfo, (void **)&s1apWarningSecurityInfo_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apWarningSecurityInfo failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningRequestIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGSECURITYINFO_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_DataCodingScheme:
            {
                S1ap_DataCodingScheme_t *s1apDataCodingScheme_p = &s1ap_WriteReplaceWarningRequestIEs->dataCodingScheme;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_DataCodingScheme, (void **)&s1apDataCodingScheme_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apDataCodingScheme failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningRequestIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGREQUESTIES_DATACODINGSCHEME_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_WarningMessageContents:
            {
                S1ap_WarningMessageContents_t *s1apWarningMessageContents_p = &s1ap_WriteReplaceWarningRequestIEs->warningMessageContents;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_WarningMessageContents, (void **)&s1apWarningMessageContents_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apWarningMessageContents failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningRequestIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGMESSAGECONTENTS_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_ConcurrentWarningMessageIndicator:
            {
                S1ap_ConcurrentWarningMessageIndicator_t *s1apConcurrentWarningMessageIndicator_p = &s1ap_WriteReplaceWarningRequestIEs->concurrentWarningMessageIndicator;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ConcurrentWarningMessageIndicator, (void **)&s1apConcurrentWarningMessageIndicator_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apConcurrentWarningMessageIndicator failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                    return -1;
                }
                s1ap_WriteReplaceWarningRequestIEs->presenceMask |= S1AP_WRITEREPLACEWARNINGREQUESTIES_CONCURRENTWARNINGMESSAGEINDICATOR_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_writereplacewarningrequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WriteReplaceWarningRequest, s1ap_WriteReplaceWarningRequest_p);
    return 0;
}

int s1ap_decode_s1ap_uecapabilityinfoindicationies(
    S1ap_UECapabilityInfoIndicationIEs_t *s1ap_UECapabilityInfoIndicationIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UECapabilityInfoIndication_t  s1ap_UECapabilityInfoIndication;
    S1ap_UECapabilityInfoIndication_t *s1ap_UECapabilityInfoIndication_p = &s1ap_UECapabilityInfoIndication;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UECapabilityInfoIndicationIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UECapabilityInfoIndicationIEs\n");

    memset(&s1ap_UECapabilityInfoIndication, 0, sizeof(S1ap_UECapabilityInfoIndication_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UECapabilityInfoIndication, (void **)&s1ap_UECapabilityInfoIndication_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UECapabilityInfoIndication failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UECapabilityInfoIndication, s1ap_UECapabilityInfoIndication_p);
        return -1;
    }

    for (i = 0; i < s1ap_UECapabilityInfoIndication_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UECapabilityInfoIndication_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UECapabilityInfoIndicationIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UECapabilityInfoIndication, s1ap_UECapabilityInfoIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UECapabilityInfoIndicationIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UECapabilityInfoIndication, s1ap_UECapabilityInfoIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_UERadioCapability:
            {
                S1ap_UERadioCapability_t *s1apUERadioCapability_p = &s1ap_UECapabilityInfoIndicationIEs->ueRadioCapability;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UERadioCapability, (void **)&s1apUERadioCapability_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUERadioCapability failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UECapabilityInfoIndication, s1ap_UECapabilityInfoIndication_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uecapabilityinfoindicationies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UECapabilityInfoIndication, s1ap_UECapabilityInfoIndication_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UECapabilityInfoIndication, s1ap_UECapabilityInfoIndication_p);
    return 0;
}

int s1ap_decode_s1ap_locationreportingfailureindicationies(
    S1ap_LocationReportingFailureIndicationIEs_t *s1ap_LocationReportingFailureIndicationIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_LocationReportingFailureIndication_t  s1ap_LocationReportingFailureIndication;
    S1ap_LocationReportingFailureIndication_t *s1ap_LocationReportingFailureIndication_p = &s1ap_LocationReportingFailureIndication;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_LocationReportingFailureIndicationIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_LocationReportingFailureIndicationIEs\n");

    memset(&s1ap_LocationReportingFailureIndication, 0, sizeof(S1ap_LocationReportingFailureIndication_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LocationReportingFailureIndication, (void **)&s1ap_LocationReportingFailureIndication_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_LocationReportingFailureIndication failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingFailureIndication, s1ap_LocationReportingFailureIndication_p);
        return -1;
    }

    for (i = 0; i < s1ap_LocationReportingFailureIndication_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_LocationReportingFailureIndication_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_LocationReportingFailureIndicationIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingFailureIndication, s1ap_LocationReportingFailureIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_LocationReportingFailureIndicationIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingFailureIndication, s1ap_LocationReportingFailureIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_LocationReportingFailureIndicationIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingFailureIndication, s1ap_LocationReportingFailureIndication_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_locationreportingfailureindicationies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingFailureIndication, s1ap_LocationReportingFailureIndication_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingFailureIndication, s1ap_LocationReportingFailureIndication_p);
    return 0;
}

int s1ap_decode_s1ap_initialcontextsetuprequesties(
    S1ap_InitialContextSetupRequestIEs_t *s1ap_InitialContextSetupRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_InitialContextSetupRequest_t  s1ap_InitialContextSetupRequest;
    S1ap_InitialContextSetupRequest_t *s1ap_InitialContextSetupRequest_p = &s1ap_InitialContextSetupRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_InitialContextSetupRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_InitialContextSetupRequestIEs\n");

    memset(&s1ap_InitialContextSetupRequest, 0, sizeof(S1ap_InitialContextSetupRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_InitialContextSetupRequest, (void **)&s1ap_InitialContextSetupRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_InitialContextSetupRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_InitialContextSetupRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_InitialContextSetupRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_InitialContextSetupRequestIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_InitialContextSetupRequestIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate:
            {
                S1ap_UEAggregateMaximumBitrate_t *s1apUEAggregateMaximumBitrate_p = &s1ap_InitialContextSetupRequestIEs->uEaggregateMaximumBitrate;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEAggregateMaximumBitrate, (void **)&s1apUEAggregateMaximumBitrate_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEAggregateMaximumBitrate failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABToBeSetupListCtxtSUReq:
            {
                S1ap_E_RABToBeSetupListCtxtSUReq_t *s1apERABToBeSetupListCtxtSUReq_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSetupListCtxtSUReq, (void**)&s1apERABToBeSetupListCtxtSUReq_p);
                if (s1ap_decode_s1ap_e_rabtobesetuplistctxtsureq(&s1ap_InitialContextSetupRequestIEs->e_RABToBeSetupListCtxtSUReq, s1apERABToBeSetupListCtxtSUReq_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABToBeSetupListCtxtSUReq failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupListCtxtSUReq, s1apERABToBeSetupListCtxtSUReq_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupListCtxtSUReq, s1apERABToBeSetupListCtxtSUReq_p);
            } break;
            case S1ap_ProtocolIE_ID_id_UESecurityCapabilities:
            {
                S1ap_UESecurityCapabilities_t *s1apUESecurityCapabilities_p = &s1ap_InitialContextSetupRequestIEs->ueSecurityCapabilities;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UESecurityCapabilities, (void **)&s1apUESecurityCapabilities_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUESecurityCapabilities failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_SecurityKey:
            {
                S1ap_SecurityKey_t *s1apSecurityKey_p = &s1ap_InitialContextSetupRequestIEs->securityKey;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SecurityKey, (void **)&s1apSecurityKey_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSecurityKey failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_TraceActivation:
            {
                S1ap_TraceActivation_t *s1apTraceActivation_p = &s1ap_InitialContextSetupRequestIEs->traceActivation;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TraceActivation, (void **)&s1apTraceActivation_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTraceActivation failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_TRACEACTIVATION_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_HandoverRestrictionList:
            {
                S1ap_HandoverRestrictionList_t *s1apHandoverRestrictionList_p = &s1ap_InitialContextSetupRequestIEs->handoverRestrictionList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverRestrictionList, (void **)&s1apHandoverRestrictionList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apHandoverRestrictionList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_HANDOVERRESTRICTIONLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_UERadioCapability:
            {
                S1ap_UERadioCapability_t *s1apUERadioCapability_p = &s1ap_InitialContextSetupRequestIEs->ueRadioCapability;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UERadioCapability, (void **)&s1apUERadioCapability_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUERadioCapability failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_UERADIOCAPABILITY_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SubscriberProfileIDforRFP:
            {
                S1ap_SubscriberProfileIDforRFP_t *s1apSubscriberProfileIDforRFP_p = &s1ap_InitialContextSetupRequestIEs->subscriberProfileIDforRFP;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SubscriberProfileIDforRFP, (void **)&s1apSubscriberProfileIDforRFP_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSubscriberProfileIDforRFP failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_SUBSCRIBERPROFILEIDFORRFP_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSFallbackIndicator:
            {
                S1ap_CSFallbackIndicator_t *s1apCSFallbackIndicator_p = &s1ap_InitialContextSetupRequestIEs->csFallbackIndicator;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSFallbackIndicator, (void **)&s1apCSFallbackIndicator_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSFallbackIndicator failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_CSFALLBACKINDICATOR_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SRVCCOperationPossible:
            {
                S1ap_SRVCCOperationPossible_t *s1apSRVCCOperationPossible_p = &s1ap_InitialContextSetupRequestIEs->srvccOperationPossible;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SRVCCOperationPossible, (void **)&s1apSRVCCOperationPossible_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSRVCCOperationPossible failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_SRVCCOPERATIONPOSSIBLE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSGMembershipStatus:
            {
                S1ap_CSGMembershipStatus_t *s1apCSGMembershipStatus_p = &s1ap_InitialContextSetupRequestIEs->csgMembershipStatus;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSGMembershipStatus, (void **)&s1apCSGMembershipStatus_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGMembershipStatus failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_RegisteredLAI:
            {
                S1ap_LAI_t *s1apLAI_p = &s1ap_InitialContextSetupRequestIEs->registeredLAI;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LAI, (void **)&s1apLAI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apLAI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_REGISTEREDLAI_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_GUMMEI_ID:
            {
                S1ap_GUMMEI_t *s1apGUMMEI_p = &s1ap_InitialContextSetupRequestIEs->gummei_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_GUMMEI, (void **)&s1apGUMMEI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apGUMMEI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_GUMMEI_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID_2:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_InitialContextSetupRequestIEs->mme_ue_s1ap_id_2;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_MME_UE_S1AP_ID_2_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_ManagementBasedMDTAllowed:
            {
                S1ap_ManagementBasedMDTAllowed_t *s1apManagementBasedMDTAllowed_p = &s1ap_InitialContextSetupRequestIEs->managementBasedMDTAllowed;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ManagementBasedMDTAllowed, (void **)&s1apManagementBasedMDTAllowed_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apManagementBasedMDTAllowed failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                    return -1;
                }
                s1ap_InitialContextSetupRequestIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPREQUESTIES_MANAGEMENTBASEDMDTALLOWED_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_initialcontextsetuprequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupRequest, s1ap_InitialContextSetupRequest_p);
    return 0;
}

int s1ap_decode_s1ap_locationreporties(
    S1ap_LocationReportIEs_t *s1ap_LocationReportIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_LocationReport_t  s1ap_LocationReport;
    S1ap_LocationReport_t *s1ap_LocationReport_p = &s1ap_LocationReport;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_LocationReportIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_LocationReportIEs\n");

    memset(&s1ap_LocationReport, 0, sizeof(S1ap_LocationReport_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LocationReport, (void **)&s1ap_LocationReport_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_LocationReport failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReport, s1ap_LocationReport_p);
        return -1;
    }

    for (i = 0; i < s1ap_LocationReport_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_LocationReport_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_LocationReportIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReport, s1ap_LocationReport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_LocationReportIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReport, s1ap_LocationReport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_EUTRAN_CGI:
            {
                S1ap_EUTRAN_CGI_t *s1apEUTRANCGI_p = &s1ap_LocationReportIEs->eutran_cgi;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_EUTRAN_CGI, (void **)&s1apEUTRANCGI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANCGI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReport, s1ap_LocationReport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TAI:
            {
                S1ap_TAI_t *s1apTAI_p = &s1ap_LocationReportIEs->tai;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TAI, (void **)&s1apTAI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTAI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReport, s1ap_LocationReport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_RequestType:
            {
                S1ap_RequestType_t *s1apRequestType_p = &s1ap_LocationReportIEs->requestType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_RequestType, (void **)&s1apRequestType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRequestType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReport, s1ap_LocationReport_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_locationreporties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReport, s1ap_LocationReport_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReport, s1ap_LocationReport_p);
    return 0;
}

int s1ap_decode_s1ap_uplinknastransport_ies(
    S1ap_UplinkNASTransport_IEs_t *s1ap_UplinkNASTransport_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UplinkNASTransport_t  s1ap_UplinkNASTransport;
    S1ap_UplinkNASTransport_t *s1ap_UplinkNASTransport_p = &s1ap_UplinkNASTransport;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UplinkNASTransport_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UplinkNASTransport_IEs\n");

    memset(&s1ap_UplinkNASTransport, 0, sizeof(S1ap_UplinkNASTransport_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UplinkNASTransport, (void **)&s1ap_UplinkNASTransport_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UplinkNASTransport failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
        return -1;
    }

    for (i = 0; i < s1ap_UplinkNASTransport_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UplinkNASTransport_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UplinkNASTransport_IEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UplinkNASTransport_IEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_NAS_PDU:
            {
                S1ap_NAS_PDU_t *s1apNASPDU_p = &s1ap_UplinkNASTransport_IEs->nas_pdu;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_NAS_PDU, (void **)&s1apNASPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apNASPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_EUTRAN_CGI:
            {
                S1ap_EUTRAN_CGI_t *s1apEUTRANCGI_p = &s1ap_UplinkNASTransport_IEs->eutran_cgi;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_EUTRAN_CGI, (void **)&s1apEUTRANCGI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANCGI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TAI:
            {
                S1ap_TAI_t *s1apTAI_p = &s1ap_UplinkNASTransport_IEs->tai;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TAI, (void **)&s1apTAI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTAI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_GW_TransportLayerAddress:
            {
                S1ap_TransportLayerAddress_t *s1apTransportLayerAddress_p = &s1ap_UplinkNASTransport_IEs->gW_TransportLayerAddress;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TransportLayerAddress, (void **)&s1apTransportLayerAddress_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTransportLayerAddress failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
                    return -1;
                }
                s1ap_UplinkNASTransport_IEs->presenceMask |= S1AP_UPLINKNASTRANSPORT_IES_GW_TRANSPORTLAYERADDRESS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinknastransport_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkNASTransport, s1ap_UplinkNASTransport_p);
    return 0;
}

int s1ap_decode_s1ap_uecontextmodificationresponseies(
    S1ap_UEContextModificationResponseIEs_t *s1ap_UEContextModificationResponseIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UEContextModificationResponse_t  s1ap_UEContextModificationResponse;
    S1ap_UEContextModificationResponse_t *s1ap_UEContextModificationResponse_p = &s1ap_UEContextModificationResponse;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UEContextModificationResponseIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UEContextModificationResponseIEs\n");

    memset(&s1ap_UEContextModificationResponse, 0, sizeof(S1ap_UEContextModificationResponse_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEContextModificationResponse, (void **)&s1ap_UEContextModificationResponse_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UEContextModificationResponse failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationResponse, s1ap_UEContextModificationResponse_p);
        return -1;
    }

    for (i = 0; i < s1ap_UEContextModificationResponse_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UEContextModificationResponse_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UEContextModificationResponseIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationResponse, s1ap_UEContextModificationResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UEContextModificationResponseIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationResponse, s1ap_UEContextModificationResponse_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_UEContextModificationResponseIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationResponse, s1ap_UEContextModificationResponse_p);
                    return -1;
                }
                s1ap_UEContextModificationResponseIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uecontextmodificationresponseies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationResponse, s1ap_UEContextModificationResponse_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationResponse, s1ap_UEContextModificationResponse_p);
    return 0;
}

int s1ap_decode_s1ap_uecontextmodificationrequesties(
    S1ap_UEContextModificationRequestIEs_t *s1ap_UEContextModificationRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UEContextModificationRequest_t  s1ap_UEContextModificationRequest;
    S1ap_UEContextModificationRequest_t *s1ap_UEContextModificationRequest_p = &s1ap_UEContextModificationRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UEContextModificationRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UEContextModificationRequestIEs\n");

    memset(&s1ap_UEContextModificationRequest, 0, sizeof(S1ap_UEContextModificationRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEContextModificationRequest, (void **)&s1ap_UEContextModificationRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UEContextModificationRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_UEContextModificationRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UEContextModificationRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UEContextModificationRequestIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UEContextModificationRequestIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SecurityKey:
            {
                S1ap_SecurityKey_t *s1apSecurityKey_p = &s1ap_UEContextModificationRequestIEs->securityKey;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SecurityKey, (void **)&s1apSecurityKey_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSecurityKey failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
                s1ap_UEContextModificationRequestIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONREQUESTIES_SECURITYKEY_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SubscriberProfileIDforRFP:
            {
                S1ap_SubscriberProfileIDforRFP_t *s1apSubscriberProfileIDforRFP_p = &s1ap_UEContextModificationRequestIEs->subscriberProfileIDforRFP;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SubscriberProfileIDforRFP, (void **)&s1apSubscriberProfileIDforRFP_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSubscriberProfileIDforRFP failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
                s1ap_UEContextModificationRequestIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONREQUESTIES_SUBSCRIBERPROFILEIDFORRFP_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate:
            {
                S1ap_UEAggregateMaximumBitrate_t *s1apUEAggregateMaximumBitrate_p = &s1ap_UEContextModificationRequestIEs->uEaggregateMaximumBitrate;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEAggregateMaximumBitrate, (void **)&s1apUEAggregateMaximumBitrate_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEAggregateMaximumBitrate failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
                s1ap_UEContextModificationRequestIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSFallbackIndicator:
            {
                S1ap_CSFallbackIndicator_t *s1apCSFallbackIndicator_p = &s1ap_UEContextModificationRequestIEs->csFallbackIndicator;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSFallbackIndicator, (void **)&s1apCSFallbackIndicator_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSFallbackIndicator failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
                s1ap_UEContextModificationRequestIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONREQUESTIES_CSFALLBACKINDICATOR_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_UESecurityCapabilities:
            {
                S1ap_UESecurityCapabilities_t *s1apUESecurityCapabilities_p = &s1ap_UEContextModificationRequestIEs->ueSecurityCapabilities;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UESecurityCapabilities, (void **)&s1apUESecurityCapabilities_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUESecurityCapabilities failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
                s1ap_UEContextModificationRequestIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONREQUESTIES_UESECURITYCAPABILITIES_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSGMembershipStatus:
            {
                S1ap_CSGMembershipStatus_t *s1apCSGMembershipStatus_p = &s1ap_UEContextModificationRequestIEs->csgMembershipStatus;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSGMembershipStatus, (void **)&s1apCSGMembershipStatus_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGMembershipStatus failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
                s1ap_UEContextModificationRequestIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_RegisteredLAI:
            {
                S1ap_LAI_t *s1apLAI_p = &s1ap_UEContextModificationRequestIEs->registeredLAI;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LAI, (void **)&s1apLAI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apLAI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                    return -1;
                }
                s1ap_UEContextModificationRequestIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONREQUESTIES_REGISTEREDLAI_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uecontextmodificationrequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationRequest, s1ap_UEContextModificationRequest_p);
    return 0;
}

int s1ap_decode_s1ap_locationreportingcontrolies(
    S1ap_LocationReportingControlIEs_t *s1ap_LocationReportingControlIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_LocationReportingControl_t  s1ap_LocationReportingControl;
    S1ap_LocationReportingControl_t *s1ap_LocationReportingControl_p = &s1ap_LocationReportingControl;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_LocationReportingControlIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_LocationReportingControlIEs\n");

    memset(&s1ap_LocationReportingControl, 0, sizeof(S1ap_LocationReportingControl_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LocationReportingControl, (void **)&s1ap_LocationReportingControl_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_LocationReportingControl failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingControl, s1ap_LocationReportingControl_p);
        return -1;
    }

    for (i = 0; i < s1ap_LocationReportingControl_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_LocationReportingControl_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_LocationReportingControlIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingControl, s1ap_LocationReportingControl_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_LocationReportingControlIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingControl, s1ap_LocationReportingControl_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_RequestType:
            {
                S1ap_RequestType_t *s1apRequestType_p = &s1ap_LocationReportingControlIEs->requestType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_RequestType, (void **)&s1apRequestType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRequestType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingControl, s1ap_LocationReportingControl_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_locationreportingcontrolies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingControl, s1ap_LocationReportingControl_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LocationReportingControl, s1ap_LocationReportingControl_p);
    return 0;
}

int s1ap_decode_s1ap_uplinks1cdma2000tunnelingies(
    S1ap_UplinkS1cdma2000tunnelingIEs_t *s1ap_UplinkS1cdma2000tunnelingIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UplinkS1cdma2000tunneling_t  s1ap_UplinkS1cdma2000tunneling;
    S1ap_UplinkS1cdma2000tunneling_t *s1ap_UplinkS1cdma2000tunneling_p = &s1ap_UplinkS1cdma2000tunneling;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UplinkS1cdma2000tunnelingIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UplinkS1cdma2000tunnelingIEs\n");

    memset(&s1ap_UplinkS1cdma2000tunneling, 0, sizeof(S1ap_UplinkS1cdma2000tunneling_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UplinkS1cdma2000tunneling, (void **)&s1ap_UplinkS1cdma2000tunneling_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UplinkS1cdma2000tunneling failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
        return -1;
    }

    for (i = 0; i < s1ap_UplinkS1cdma2000tunneling_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UplinkS1cdma2000tunneling_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UplinkS1cdma2000tunnelingIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UplinkS1cdma2000tunnelingIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_cdma2000RATType:
            {
                S1ap_Cdma2000RATType_t *s1apCdma2000RATType_p = &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000RATType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000RATType, (void **)&s1apCdma2000RATType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000RATType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_cdma2000SectorID:
            {
                S1ap_Cdma2000SectorID_t *s1apCdma2000SectorID_p = &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000SectorID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000SectorID, (void **)&s1apCdma2000SectorID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000SectorID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_cdma2000HORequiredIndication:
            {
                S1ap_Cdma2000HORequiredIndication_t *s1apCdma2000HORequiredIndication_p = &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000HORequiredIndication;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000HORequiredIndication, (void **)&s1apCdma2000HORequiredIndication_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000HORequiredIndication failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
                s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask |= S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000HOREQUIREDINDICATION_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_cdma2000OneXSRVCCInfo:
            {
                S1ap_Cdma2000OneXSRVCCInfo_t *s1apCdma2000OneXSRVCCInfo_p = &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000OneXSRVCCInfo;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000OneXSRVCCInfo, (void **)&s1apCdma2000OneXSRVCCInfo_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000OneXSRVCCInfo failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
                s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask |= S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000ONEXSRVCCINFO_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_cdma2000OneXRAND:
            {
                S1ap_Cdma2000OneXRAND_t *s1apCdma2000OneXRAND_p = &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000OneXRAND;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000OneXRAND, (void **)&s1apCdma2000OneXRAND_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000OneXRAND failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
                s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask |= S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000ONEXRAND_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_cdma2000PDU:
            {
                S1ap_Cdma2000PDU_t *s1apCdma2000PDU_p = &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000PDU;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000PDU, (void **)&s1apCdma2000PDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000PDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_EUTRANRoundTripDelayEstimationInfo:
            {
                S1ap_EUTRANRoundTripDelayEstimationInfo_t *s1apEUTRANRoundTripDelayEstimationInfo_p = &s1ap_UplinkS1cdma2000tunnelingIEs->eutranRoundTripDelayEstimationInfo;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_EUTRANRoundTripDelayEstimationInfo, (void **)&s1apEUTRANRoundTripDelayEstimationInfo_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANRoundTripDelayEstimationInfo failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                    return -1;
                }
                s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask |= S1AP_UPLINKS1CDMA2000TUNNELINGIES_EUTRANROUNDTRIPDELAYESTIMATIONINFO_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinks1cdma2000tunnelingies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkS1cdma2000tunneling, s1ap_UplinkS1cdma2000tunneling_p);
    return 0;
}

int s1ap_decode_s1ap_handoverrequesties(
    S1ap_HandoverRequestIEs_t *s1ap_HandoverRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverRequest_t  s1ap_HandoverRequest;
    S1ap_HandoverRequest_t *s1ap_HandoverRequest_p = &s1ap_HandoverRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverRequestIEs\n");

    memset(&s1ap_HandoverRequest, 0, sizeof(S1ap_HandoverRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverRequest, (void **)&s1ap_HandoverRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverRequestIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_HandoverType:
            {
                S1ap_HandoverType_t *s1apHandoverType_p = &s1ap_HandoverRequestIEs->handoverType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverType, (void **)&s1apHandoverType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apHandoverType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_HandoverRequestIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate:
            {
                S1ap_UEAggregateMaximumBitrate_t *s1apUEAggregateMaximumBitrate_p = &s1ap_HandoverRequestIEs->uEaggregateMaximumBitrate;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEAggregateMaximumBitrate, (void **)&s1apUEAggregateMaximumBitrate_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEAggregateMaximumBitrate failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABToBeSetupListHOReq:
            {
                S1ap_E_RABToBeSetupListHOReq_t *s1apERABToBeSetupListHOReq_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSetupListHOReq, (void**)&s1apERABToBeSetupListHOReq_p);
                if (s1ap_decode_s1ap_e_rabtobesetuplisthoreq(&s1ap_HandoverRequestIEs->e_RABToBeSetupListHOReq, s1apERABToBeSetupListHOReq_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABToBeSetupListHOReq failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupListHOReq, s1apERABToBeSetupListHOReq_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupListHOReq, s1apERABToBeSetupListHOReq_p);
            } break;
            case S1ap_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer:
            {
                S1ap_Source_ToTarget_TransparentContainer_t *s1apSourceToTargetTransparentContainer_p = &s1ap_HandoverRequestIEs->source_ToTarget_TransparentContainer;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Source_ToTarget_TransparentContainer, (void **)&s1apSourceToTargetTransparentContainer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSourceToTargetTransparentContainer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_UESecurityCapabilities:
            {
                S1ap_UESecurityCapabilities_t *s1apUESecurityCapabilities_p = &s1ap_HandoverRequestIEs->ueSecurityCapabilities;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UESecurityCapabilities, (void **)&s1apUESecurityCapabilities_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUESecurityCapabilities failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_HandoverRestrictionList:
            {
                S1ap_HandoverRestrictionList_t *s1apHandoverRestrictionList_p = &s1ap_HandoverRequestIEs->handoverRestrictionList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverRestrictionList, (void **)&s1apHandoverRestrictionList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apHandoverRestrictionList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_HANDOVERRESTRICTIONLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_TraceActivation:
            {
                S1ap_TraceActivation_t *s1apTraceActivation_p = &s1ap_HandoverRequestIEs->traceActivation;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TraceActivation, (void **)&s1apTraceActivation_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTraceActivation failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_TRACEACTIVATION_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_RequestType:
            {
                S1ap_RequestType_t *s1apRequestType_p = &s1ap_HandoverRequestIEs->requestType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_RequestType, (void **)&s1apRequestType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRequestType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_REQUESTTYPE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SRVCCOperationPossible:
            {
                S1ap_SRVCCOperationPossible_t *s1apSRVCCOperationPossible_p = &s1ap_HandoverRequestIEs->srvccOperationPossible;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SRVCCOperationPossible, (void **)&s1apSRVCCOperationPossible_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSRVCCOperationPossible failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_SRVCCOPERATIONPOSSIBLE_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_SecurityContext:
            {
                S1ap_SecurityContext_t *s1apSecurityContext_p = &s1ap_HandoverRequestIEs->securityContext;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SecurityContext, (void **)&s1apSecurityContext_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSecurityContext failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSG_Id:
            {
                S1ap_CSG_Id_t *s1apCSGId_p = &s1ap_HandoverRequestIEs->csG_Id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSG_Id, (void **)&s1apCSGId_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGId failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_CSG_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSGMembershipStatus:
            {
                S1ap_CSGMembershipStatus_t *s1apCSGMembershipStatus_p = &s1ap_HandoverRequestIEs->csgMembershipStatus;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSGMembershipStatus, (void **)&s1apCSGMembershipStatus_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGMembershipStatus failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_GUMMEI_ID:
            {
                S1ap_GUMMEI_t *s1apGUMMEI_p = &s1ap_HandoverRequestIEs->gummei_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_GUMMEI, (void **)&s1apGUMMEI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apGUMMEI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_GUMMEI_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID_2:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverRequestIEs->mme_ue_s1ap_id_2;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_MME_UE_S1AP_ID_2_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_ManagementBasedMDTAllowed:
            {
                S1ap_ManagementBasedMDTAllowed_t *s1apManagementBasedMDTAllowed_p = &s1ap_HandoverRequestIEs->managementBasedMDTAllowed;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ManagementBasedMDTAllowed, (void **)&s1apManagementBasedMDTAllowed_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apManagementBasedMDTAllowed failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                    return -1;
                }
                s1ap_HandoverRequestIEs->presenceMask |= S1AP_HANDOVERREQUESTIES_MANAGEMENTBASEDMDTALLOWED_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handoverrequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequest, s1ap_HandoverRequest_p);
    return 0;
}

int s1ap_decode_s1ap_handoverrequiredies(
    S1ap_HandoverRequiredIEs_t *s1ap_HandoverRequiredIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverRequired_t  s1ap_HandoverRequired;
    S1ap_HandoverRequired_t *s1ap_HandoverRequired_p = &s1ap_HandoverRequired;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverRequiredIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverRequiredIEs\n");

    memset(&s1ap_HandoverRequired, 0, sizeof(S1ap_HandoverRequired_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverRequired, (void **)&s1ap_HandoverRequired_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverRequired failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverRequired_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverRequired_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverRequiredIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_HandoverRequiredIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_HandoverType:
            {
                S1ap_HandoverType_t *s1apHandoverType_p = &s1ap_HandoverRequiredIEs->handoverType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverType, (void **)&s1apHandoverType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apHandoverType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_HandoverRequiredIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TargetID:
            {
                S1ap_TargetID_t *s1apTargetID_p = &s1ap_HandoverRequiredIEs->targetID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TargetID, (void **)&s1apTargetID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTargetID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_Direct_Forwarding_Path_Availability:
            {
                S1ap_Direct_Forwarding_Path_Availability_t *s1apDirectForwardingPathAvailability_p = &s1ap_HandoverRequiredIEs->direct_Forwarding_Path_Availability;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Direct_Forwarding_Path_Availability, (void **)&s1apDirectForwardingPathAvailability_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apDirectForwardingPathAvailability failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
                s1ap_HandoverRequiredIEs->presenceMask |= S1AP_HANDOVERREQUIREDIES_DIRECT_FORWARDING_PATH_AVAILABILITY_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SRVCCHOIndication:
            {
                S1ap_SRVCCHOIndication_t *s1apSRVCCHOIndication_p = &s1ap_HandoverRequiredIEs->srvcchoIndication;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SRVCCHOIndication, (void **)&s1apSRVCCHOIndication_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSRVCCHOIndication failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
                s1ap_HandoverRequiredIEs->presenceMask |= S1AP_HANDOVERREQUIREDIES_SRVCCHOINDICATION_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer:
            {
                S1ap_Source_ToTarget_TransparentContainer_t *s1apSourceToTargetTransparentContainer_p = &s1ap_HandoverRequiredIEs->source_ToTarget_TransparentContainer;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Source_ToTarget_TransparentContainer, (void **)&s1apSourceToTargetTransparentContainer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSourceToTargetTransparentContainer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer_Secondary:
            {
                S1ap_Source_ToTarget_TransparentContainer_t *s1apSourceToTargetTransparentContainer_p = &s1ap_HandoverRequiredIEs->source_ToTarget_TransparentContainer_Secondary;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Source_ToTarget_TransparentContainer, (void **)&s1apSourceToTargetTransparentContainer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSourceToTargetTransparentContainer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
                s1ap_HandoverRequiredIEs->presenceMask |= S1AP_HANDOVERREQUIREDIES_SOURCE_TOTARGET_TRANSPARENTCONTAINER_SECONDARY_PRESENT;
            } break;
            /* Conditional field */
            case S1ap_ProtocolIE_ID_id_MSClassmark2:
            {
                S1ap_MSClassmark2_t *s1apMSClassmark2_p = &s1ap_HandoverRequiredIEs->msClassmark2;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MSClassmark2, (void **)&s1apMSClassmark2_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMSClassmark2 failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
                s1ap_HandoverRequiredIEs->presenceMask |= S1AP_HANDOVERREQUIREDIES_MSCLASSMARK2_PRESENT;
            } break;
            /* Conditional field */
            case S1ap_ProtocolIE_ID_id_MSClassmark3:
            {
                S1ap_MSClassmark3_t *s1apMSClassmark3_p = &s1ap_HandoverRequiredIEs->msClassmark3;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MSClassmark3, (void **)&s1apMSClassmark3_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMSClassmark3 failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
                s1ap_HandoverRequiredIEs->presenceMask |= S1AP_HANDOVERREQUIREDIES_MSCLASSMARK3_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSG_Id:
            {
                S1ap_CSG_Id_t *s1apCSGId_p = &s1ap_HandoverRequiredIEs->csG_Id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSG_Id, (void **)&s1apCSGId_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGId failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
                s1ap_HandoverRequiredIEs->presenceMask |= S1AP_HANDOVERREQUIREDIES_CSG_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CellAccessMode:
            {
                S1ap_CellAccessMode_t *s1apCellAccessMode_p = &s1ap_HandoverRequiredIEs->cellAccessMode;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CellAccessMode, (void **)&s1apCellAccessMode_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCellAccessMode failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
                s1ap_HandoverRequiredIEs->presenceMask |= S1AP_HANDOVERREQUIREDIES_CELLACCESSMODE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_PS_ServiceNotAvailable:
            {
                S1ap_PS_ServiceNotAvailable_t *s1apPSServiceNotAvailable_p = &s1ap_HandoverRequiredIEs->pS_ServiceNotAvailable;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PS_ServiceNotAvailable, (void **)&s1apPSServiceNotAvailable_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apPSServiceNotAvailable failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                    return -1;
                }
                s1ap_HandoverRequiredIEs->presenceMask |= S1AP_HANDOVERREQUIREDIES_PS_SERVICENOTAVAILABLE_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handoverrequiredies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequired, s1ap_HandoverRequired_p);
    return 0;
}

int s1ap_decode_s1ap_mmeconfigurationupdateacknowledgeies(
    S1ap_MMEConfigurationUpdateAcknowledgeIEs_t *s1ap_MMEConfigurationUpdateAcknowledgeIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_MMEConfigurationUpdateAcknowledge_t  s1ap_MMEConfigurationUpdateAcknowledge;
    S1ap_MMEConfigurationUpdateAcknowledge_t *s1ap_MMEConfigurationUpdateAcknowledge_p = &s1ap_MMEConfigurationUpdateAcknowledge;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_MMEConfigurationUpdateAcknowledgeIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_MMEConfigurationUpdateAcknowledgeIEs\n");

    memset(&s1ap_MMEConfigurationUpdateAcknowledge, 0, sizeof(S1ap_MMEConfigurationUpdateAcknowledge_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMEConfigurationUpdateAcknowledge, (void **)&s1ap_MMEConfigurationUpdateAcknowledge_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_MMEConfigurationUpdateAcknowledge failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateAcknowledge, s1ap_MMEConfigurationUpdateAcknowledge_p);
        return -1;
    }

    for (i = 0; i < s1ap_MMEConfigurationUpdateAcknowledge_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_MMEConfigurationUpdateAcknowledge_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_MMEConfigurationUpdateAcknowledgeIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateAcknowledge, s1ap_MMEConfigurationUpdateAcknowledge_p);
                    return -1;
                }
                s1ap_MMEConfigurationUpdateAcknowledgeIEs->presenceMask |= S1AP_MMECONFIGURATIONUPDATEACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_mmeconfigurationupdateacknowledgeies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateAcknowledge, s1ap_MMEConfigurationUpdateAcknowledge_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateAcknowledge, s1ap_MMEConfigurationUpdateAcknowledge_p);
    return 0;
}

int s1ap_decode_s1ap_killresponseies(
    S1ap_KillResponseIEs_t *s1ap_KillResponseIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_KillResponse_t  s1ap_KillResponse;
    S1ap_KillResponse_t *s1ap_KillResponse_p = &s1ap_KillResponse;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_KillResponseIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_KillResponseIEs\n");

    memset(&s1ap_KillResponse, 0, sizeof(S1ap_KillResponse_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_KillResponse, (void **)&s1ap_KillResponse_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_KillResponse failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillResponse, s1ap_KillResponse_p);
        return -1;
    }

    for (i = 0; i < s1ap_KillResponse_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_KillResponse_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MessageIdentifier:
            {
                S1ap_MessageIdentifier_t *s1apMessageIdentifier_p = &s1ap_KillResponseIEs->messageIdentifier;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MessageIdentifier, (void **)&s1apMessageIdentifier_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMessageIdentifier failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillResponse, s1ap_KillResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_SerialNumber:
            {
                S1ap_SerialNumber_t *s1apSerialNumber_p = &s1ap_KillResponseIEs->serialNumber;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SerialNumber, (void **)&s1apSerialNumber_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSerialNumber failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillResponse, s1ap_KillResponse_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_BroadcastCancelledAreaList:
            {
                S1ap_BroadcastCancelledAreaList_t *s1apBroadcastCancelledAreaList_p = &s1ap_KillResponseIEs->broadcastCancelledAreaList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_BroadcastCancelledAreaList, (void **)&s1apBroadcastCancelledAreaList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apBroadcastCancelledAreaList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillResponse, s1ap_KillResponse_p);
                    return -1;
                }
                s1ap_KillResponseIEs->presenceMask |= S1AP_KILLRESPONSEIES_BROADCASTCANCELLEDAREALIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_KillResponseIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillResponse, s1ap_KillResponse_p);
                    return -1;
                }
                s1ap_KillResponseIEs->presenceMask |= S1AP_KILLRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_killresponseies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillResponse, s1ap_KillResponse_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_KillResponse, s1ap_KillResponse_p);
    return 0;
}

int s1ap_decode_s1ap_mmestatustransferies(
    S1ap_MMEStatusTransferIEs_t *s1ap_MMEStatusTransferIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_MMEStatusTransfer_t  s1ap_MMEStatusTransfer;
    S1ap_MMEStatusTransfer_t *s1ap_MMEStatusTransfer_p = &s1ap_MMEStatusTransfer;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_MMEStatusTransferIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_MMEStatusTransferIEs\n");

    memset(&s1ap_MMEStatusTransfer, 0, sizeof(S1ap_MMEStatusTransfer_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMEStatusTransfer, (void **)&s1ap_MMEStatusTransfer_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_MMEStatusTransfer failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEStatusTransfer, s1ap_MMEStatusTransfer_p);
        return -1;
    }

    for (i = 0; i < s1ap_MMEStatusTransfer_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_MMEStatusTransfer_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_MMEStatusTransferIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEStatusTransfer, s1ap_MMEStatusTransfer_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_MMEStatusTransferIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEStatusTransfer, s1ap_MMEStatusTransfer_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer:
            {
                S1ap_ENB_StatusTransfer_TransparentContainer_t *s1apENBStatusTransferTransparentContainer_p = &s1ap_MMEStatusTransferIEs->eNB_StatusTransfer_TransparentContainer;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_StatusTransfer_TransparentContainer, (void **)&s1apENBStatusTransferTransparentContainer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBStatusTransferTransparentContainer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEStatusTransfer, s1ap_MMEStatusTransfer_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_mmestatustransferies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEStatusTransfer, s1ap_MMEStatusTransfer_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEStatusTransfer, s1ap_MMEStatusTransfer_p);
    return 0;
}

int s1ap_decode_s1ap_mmeconfigurationupdateies(
    S1ap_MMEConfigurationUpdateIEs_t *s1ap_MMEConfigurationUpdateIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_MMEConfigurationUpdate_t  s1ap_MMEConfigurationUpdate;
    S1ap_MMEConfigurationUpdate_t *s1ap_MMEConfigurationUpdate_p = &s1ap_MMEConfigurationUpdate;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_MMEConfigurationUpdateIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_MMEConfigurationUpdateIEs\n");

    memset(&s1ap_MMEConfigurationUpdate, 0, sizeof(S1ap_MMEConfigurationUpdate_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMEConfigurationUpdate, (void **)&s1ap_MMEConfigurationUpdate_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_MMEConfigurationUpdate failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdate, s1ap_MMEConfigurationUpdate_p);
        return -1;
    }

    for (i = 0; i < s1ap_MMEConfigurationUpdate_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_MMEConfigurationUpdate_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_MMEname:
            {
                S1ap_MMEname_t *s1apMMEname_p = &s1ap_MMEConfigurationUpdateIEs->mmEname;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMEname, (void **)&s1apMMEname_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEname failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdate, s1ap_MMEConfigurationUpdate_p);
                    return -1;
                }
                s1ap_MMEConfigurationUpdateIEs->presenceMask |= S1AP_MMECONFIGURATIONUPDATEIES_MMENAME_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_ServedGUMMEIs:
            {
                S1ap_ServedGUMMEIs_t *s1apServedGUMMEIs_p = &s1ap_MMEConfigurationUpdateIEs->servedGUMMEIs;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ServedGUMMEIs, (void **)&s1apServedGUMMEIs_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apServedGUMMEIs failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdate, s1ap_MMEConfigurationUpdate_p);
                    return -1;
                }
                s1ap_MMEConfigurationUpdateIEs->presenceMask |= S1AP_MMECONFIGURATIONUPDATEIES_SERVEDGUMMEIS_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_RelativeMMECapacity:
            {
                S1ap_RelativeMMECapacity_t *s1apRelativeMMECapacity_p = &s1ap_MMEConfigurationUpdateIEs->relativeMMECapacity;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_RelativeMMECapacity, (void **)&s1apRelativeMMECapacity_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRelativeMMECapacity failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdate, s1ap_MMEConfigurationUpdate_p);
                    return -1;
                }
                s1ap_MMEConfigurationUpdateIEs->presenceMask |= S1AP_MMECONFIGURATIONUPDATEIES_RELATIVEMMECAPACITY_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_mmeconfigurationupdateies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdate, s1ap_MMEConfigurationUpdate_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdate, s1ap_MMEConfigurationUpdate_p);
    return 0;
}

int s1ap_decode_s1ap_e_rabmodifyrequesties(
    S1ap_E_RABModifyRequestIEs_t *s1ap_E_RABModifyRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_E_RABModifyRequest_t  s1ap_E_RABModifyRequest;
    S1ap_E_RABModifyRequest_t *s1ap_E_RABModifyRequest_p = &s1ap_E_RABModifyRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_E_RABModifyRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_E_RABModifyRequestIEs\n");

    memset(&s1ap_E_RABModifyRequest, 0, sizeof(S1ap_E_RABModifyRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_RABModifyRequest, (void **)&s1ap_E_RABModifyRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_E_RABModifyRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyRequest, s1ap_E_RABModifyRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_E_RABModifyRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_E_RABModifyRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_E_RABModifyRequestIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyRequest, s1ap_E_RABModifyRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_E_RABModifyRequestIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyRequest, s1ap_E_RABModifyRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate:
            {
                S1ap_UEAggregateMaximumBitrate_t *s1apUEAggregateMaximumBitrate_p = &s1ap_E_RABModifyRequestIEs->uEaggregateMaximumBitrate;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEAggregateMaximumBitrate, (void **)&s1apUEAggregateMaximumBitrate_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEAggregateMaximumBitrate failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyRequest, s1ap_E_RABModifyRequest_p);
                    return -1;
                }
                s1ap_E_RABModifyRequestIEs->presenceMask |= S1AP_E_RABMODIFYREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABToBeModifiedListBearerModReq:
            {
                S1ap_E_RABToBeModifiedListBearerModReq_t *s1apERABToBeModifiedListBearerModReq_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeModifiedListBearerModReq, (void**)&s1apERABToBeModifiedListBearerModReq_p);
                if (s1ap_decode_s1ap_e_rabtobemodifiedlistbearermodreq(&s1ap_E_RABModifyRequestIEs->e_RABToBeModifiedListBearerModReq, s1apERABToBeModifiedListBearerModReq_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABToBeModifiedListBearerModReq failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeModifiedListBearerModReq, s1apERABToBeModifiedListBearerModReq_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyRequest, s1ap_E_RABModifyRequest_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeModifiedListBearerModReq, s1apERABToBeModifiedListBearerModReq_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_e_rabmodifyrequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyRequest, s1ap_E_RABModifyRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyRequest, s1ap_E_RABModifyRequest_p);
    return 0;
}

int s1ap_decode_s1ap_enbconfigurationupdatefailureies(
    S1ap_ENBConfigurationUpdateFailureIEs_t *s1ap_ENBConfigurationUpdateFailureIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_ENBConfigurationUpdateFailure_t  s1ap_ENBConfigurationUpdateFailure;
    S1ap_ENBConfigurationUpdateFailure_t *s1ap_ENBConfigurationUpdateFailure_p = &s1ap_ENBConfigurationUpdateFailure;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ENBConfigurationUpdateFailureIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ENBConfigurationUpdateFailureIEs\n");

    memset(&s1ap_ENBConfigurationUpdateFailure, 0, sizeof(S1ap_ENBConfigurationUpdateFailure_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENBConfigurationUpdateFailure, (void **)&s1ap_ENBConfigurationUpdateFailure_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_ENBConfigurationUpdateFailure failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateFailure, s1ap_ENBConfigurationUpdateFailure_p);
        return -1;
    }

    for (i = 0; i < s1ap_ENBConfigurationUpdateFailure_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_ENBConfigurationUpdateFailure_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_ENBConfigurationUpdateFailureIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateFailure, s1ap_ENBConfigurationUpdateFailure_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_TimeToWait:
            {
                S1ap_TimeToWait_t *s1apTimeToWait_p = &s1ap_ENBConfigurationUpdateFailureIEs->timeToWait;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TimeToWait, (void **)&s1apTimeToWait_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTimeToWait failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateFailure, s1ap_ENBConfigurationUpdateFailure_p);
                    return -1;
                }
                s1ap_ENBConfigurationUpdateFailureIEs->presenceMask |= S1AP_ENBCONFIGURATIONUPDATEFAILUREIES_TIMETOWAIT_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_ENBConfigurationUpdateFailureIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateFailure, s1ap_ENBConfigurationUpdateFailure_p);
                    return -1;
                }
                s1ap_ENBConfigurationUpdateFailureIEs->presenceMask |= S1AP_ENBCONFIGURATIONUPDATEFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_enbconfigurationupdatefailureies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateFailure, s1ap_ENBConfigurationUpdateFailure_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateFailure, s1ap_ENBConfigurationUpdateFailure_p);
    return 0;
}

int s1ap_decode_s1ap_reseties(
    S1ap_ResetIEs_t *s1ap_ResetIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_Reset_t  s1ap_Reset;
    S1ap_Reset_t *s1ap_Reset_p = &s1ap_Reset;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ResetIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ResetIEs\n");

    memset(&s1ap_Reset, 0, sizeof(S1ap_Reset_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Reset, (void **)&s1ap_Reset_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_Reset failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Reset, s1ap_Reset_p);
        return -1;
    }

    for (i = 0; i < s1ap_Reset_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_Reset_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_ResetIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Reset, s1ap_Reset_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_ResetType:
            {
                S1ap_ResetType_t *s1apResetType_p = &s1ap_ResetIEs->resetType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ResetType, (void **)&s1apResetType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apResetType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Reset, s1ap_Reset_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_reseties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Reset, s1ap_Reset_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Reset, s1ap_Reset_p);
    return 0;
}

int s1ap_decode_s1ap_s1setupresponseies(
    S1ap_S1SetupResponseIEs_t *s1ap_S1SetupResponseIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_S1SetupResponse_t  s1ap_S1SetupResponse;
    S1ap_S1SetupResponse_t *s1ap_S1SetupResponse_p = &s1ap_S1SetupResponse;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_S1SetupResponseIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_S1SetupResponseIEs\n");

    memset(&s1ap_S1SetupResponse, 0, sizeof(S1ap_S1SetupResponse_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_S1SetupResponse, (void **)&s1ap_S1SetupResponse_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_S1SetupResponse failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupResponse, s1ap_S1SetupResponse_p);
        return -1;
    }

    for (i = 0; i < s1ap_S1SetupResponse_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_S1SetupResponse_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_MMEname:
            {
                S1ap_MMEname_t *s1apMMEname_p = &s1ap_S1SetupResponseIEs->mmEname;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMEname, (void **)&s1apMMEname_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEname failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupResponse, s1ap_S1SetupResponse_p);
                    return -1;
                }
                s1ap_S1SetupResponseIEs->presenceMask |= S1AP_S1SETUPRESPONSEIES_MMENAME_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_ServedGUMMEIs:
            {
                S1ap_ServedGUMMEIs_t *s1apServedGUMMEIs_p = &s1ap_S1SetupResponseIEs->servedGUMMEIs;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ServedGUMMEIs, (void **)&s1apServedGUMMEIs_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apServedGUMMEIs failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupResponse, s1ap_S1SetupResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_RelativeMMECapacity:
            {
                S1ap_RelativeMMECapacity_t *s1apRelativeMMECapacity_p = &s1ap_S1SetupResponseIEs->relativeMMECapacity;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_RelativeMMECapacity, (void **)&s1apRelativeMMECapacity_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRelativeMMECapacity failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupResponse, s1ap_S1SetupResponse_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_MMERelaySupportIndicator:
            {
                S1ap_MMERelaySupportIndicator_t *s1apMMERelaySupportIndicator_p = &s1ap_S1SetupResponseIEs->mmeRelaySupportIndicator;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMERelaySupportIndicator, (void **)&s1apMMERelaySupportIndicator_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMERelaySupportIndicator failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupResponse, s1ap_S1SetupResponse_p);
                    return -1;
                }
                s1ap_S1SetupResponseIEs->presenceMask |= S1AP_S1SETUPRESPONSEIES_MMERELAYSUPPORTINDICATOR_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_S1SetupResponseIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupResponse, s1ap_S1SetupResponse_p);
                    return -1;
                }
                s1ap_S1SetupResponseIEs->presenceMask |= S1AP_S1SETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_s1setupresponseies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupResponse, s1ap_S1SetupResponse_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupResponse, s1ap_S1SetupResponse_p);
    return 0;
}

int s1ap_decode_s1ap_pagingies(
    S1ap_PagingIEs_t *s1ap_PagingIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_Paging_t  s1ap_Paging;
    S1ap_Paging_t *s1ap_Paging_p = &s1ap_Paging;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_PagingIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_PagingIEs\n");

    memset(&s1ap_Paging, 0, sizeof(S1ap_Paging_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Paging, (void **)&s1ap_Paging_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_Paging failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
        return -1;
    }

    for (i = 0; i < s1ap_Paging_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_Paging_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_UEIdentityIndexValue:
            {
                S1ap_UEIdentityIndexValue_t *s1apUEIdentityIndexValue_p = &s1ap_PagingIEs->ueIdentityIndexValue;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEIdentityIndexValue, (void **)&s1apUEIdentityIndexValue_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEIdentityIndexValue failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_UEPagingID:
            {
                S1ap_UEPagingID_t *s1apUEPagingID_p = &s1ap_PagingIEs->uePagingID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEPagingID, (void **)&s1apUEPagingID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEPagingID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_pagingDRX:
            {
                S1ap_PagingDRX_t *s1apPagingDRX_p = &s1ap_PagingIEs->pagingDRX;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PagingDRX, (void **)&s1apPagingDRX_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apPagingDRX failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
                    return -1;
                }
                s1ap_PagingIEs->presenceMask |= S1AP_PAGINGIES_PAGINGDRX_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_CNDomain:
            {
                S1ap_CNDomain_t *s1apCNDomain_p = &s1ap_PagingIEs->cnDomain;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CNDomain, (void **)&s1apCNDomain_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCNDomain failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TAIList:
            {
                S1ap_TAIList_t *s1apTAIList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_TAIList, (void**)&s1apTAIList_p);
                if (s1ap_decode_s1ap_tailist(&s1ap_PagingIEs->taiList, s1apTAIList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apTAIList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_TAIList, s1apTAIList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_TAIList, s1apTAIList_p);
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSG_IdList:
            {
                S1ap_CSG_IdList_t *s1apCSGIdList_p = &s1ap_PagingIEs->csG_IdList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSG_IdList, (void **)&s1apCSGIdList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGIdList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
                    return -1;
                }
                s1ap_PagingIEs->presenceMask |= S1AP_PAGINGIES_CSG_IDLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_PagingPriority:
            {
                S1ap_PagingPriority_t *s1apPagingPriority_p = &s1ap_PagingIEs->pagingPriority;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PagingPriority, (void **)&s1apPagingPriority_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apPagingPriority failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
                    return -1;
                }
                s1ap_PagingIEs->presenceMask |= S1AP_PAGINGIES_PAGINGPRIORITY_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_pagingies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Paging, s1ap_Paging_p);
    return 0;
}

int s1ap_decode_s1ap_enbconfigurationupdateacknowledgeies(
    S1ap_ENBConfigurationUpdateAcknowledgeIEs_t *s1ap_ENBConfigurationUpdateAcknowledgeIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_ENBConfigurationUpdateAcknowledge_t  s1ap_ENBConfigurationUpdateAcknowledge;
    S1ap_ENBConfigurationUpdateAcknowledge_t *s1ap_ENBConfigurationUpdateAcknowledge_p = &s1ap_ENBConfigurationUpdateAcknowledge;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ENBConfigurationUpdateAcknowledgeIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ENBConfigurationUpdateAcknowledgeIEs\n");

    memset(&s1ap_ENBConfigurationUpdateAcknowledge, 0, sizeof(S1ap_ENBConfigurationUpdateAcknowledge_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENBConfigurationUpdateAcknowledge, (void **)&s1ap_ENBConfigurationUpdateAcknowledge_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_ENBConfigurationUpdateAcknowledge failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateAcknowledge, s1ap_ENBConfigurationUpdateAcknowledge_p);
        return -1;
    }

    for (i = 0; i < s1ap_ENBConfigurationUpdateAcknowledge_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_ENBConfigurationUpdateAcknowledge_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_ENBConfigurationUpdateAcknowledgeIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateAcknowledge, s1ap_ENBConfigurationUpdateAcknowledge_p);
                    return -1;
                }
                s1ap_ENBConfigurationUpdateAcknowledgeIEs->presenceMask |= S1AP_ENBCONFIGURATIONUPDATEACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_enbconfigurationupdateacknowledgeies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateAcknowledge, s1ap_ENBConfigurationUpdateAcknowledge_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdateAcknowledge, s1ap_ENBConfigurationUpdateAcknowledge_p);
    return 0;
}

int s1ap_decode_s1ap_e_rabreleasecommandies(
    S1ap_E_RABReleaseCommandIEs_t *s1ap_E_RABReleaseCommandIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_E_RABReleaseCommand_t  s1ap_E_RABReleaseCommand;
    S1ap_E_RABReleaseCommand_t *s1ap_E_RABReleaseCommand_p = &s1ap_E_RABReleaseCommand;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_E_RABReleaseCommandIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_E_RABReleaseCommandIEs\n");

    memset(&s1ap_E_RABReleaseCommand, 0, sizeof(S1ap_E_RABReleaseCommand_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_RABReleaseCommand, (void **)&s1ap_E_RABReleaseCommand_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_E_RABReleaseCommand failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseCommand, s1ap_E_RABReleaseCommand_p);
        return -1;
    }

    for (i = 0; i < s1ap_E_RABReleaseCommand_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_E_RABReleaseCommand_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_E_RABReleaseCommandIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseCommand, s1ap_E_RABReleaseCommand_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_E_RABReleaseCommandIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseCommand, s1ap_E_RABReleaseCommand_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate:
            {
                S1ap_UEAggregateMaximumBitrate_t *s1apUEAggregateMaximumBitrate_p = &s1ap_E_RABReleaseCommandIEs->uEaggregateMaximumBitrate;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEAggregateMaximumBitrate, (void **)&s1apUEAggregateMaximumBitrate_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEAggregateMaximumBitrate failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseCommand, s1ap_E_RABReleaseCommand_p);
                    return -1;
                }
                s1ap_E_RABReleaseCommandIEs->presenceMask |= S1AP_E_RABRELEASECOMMANDIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABToBeReleasedList:
            {
                S1ap_E_RABList_t *s1apERABList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABList, (void**)&s1apERABList_p);
                if (s1ap_decode_s1ap_e_rablist(&s1ap_E_RABReleaseCommandIEs->e_RABToBeReleasedList, s1apERABList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseCommand, s1ap_E_RABReleaseCommand_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_NAS_PDU:
            {
                S1ap_NAS_PDU_t *s1apNASPDU_p = &s1ap_E_RABReleaseCommandIEs->nas_pdu;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_NAS_PDU, (void **)&s1apNASPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apNASPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseCommand, s1ap_E_RABReleaseCommand_p);
                    return -1;
                }
                s1ap_E_RABReleaseCommandIEs->presenceMask |= S1AP_E_RABRELEASECOMMANDIES_NAS_PDU_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_e_rabreleasecommandies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseCommand, s1ap_E_RABReleaseCommand_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseCommand, s1ap_E_RABReleaseCommand_p);
    return 0;
}

int s1ap_decode_s1ap_pathswitchrequestfailureies(
    S1ap_PathSwitchRequestFailureIEs_t *s1ap_PathSwitchRequestFailureIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_PathSwitchRequestFailure_t  s1ap_PathSwitchRequestFailure;
    S1ap_PathSwitchRequestFailure_t *s1ap_PathSwitchRequestFailure_p = &s1ap_PathSwitchRequestFailure;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_PathSwitchRequestFailureIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_PathSwitchRequestFailureIEs\n");

    memset(&s1ap_PathSwitchRequestFailure, 0, sizeof(S1ap_PathSwitchRequestFailure_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PathSwitchRequestFailure, (void **)&s1ap_PathSwitchRequestFailure_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_PathSwitchRequestFailure failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestFailure, s1ap_PathSwitchRequestFailure_p);
        return -1;
    }

    for (i = 0; i < s1ap_PathSwitchRequestFailure_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_PathSwitchRequestFailure_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_PathSwitchRequestFailureIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestFailure, s1ap_PathSwitchRequestFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_PathSwitchRequestFailureIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestFailure, s1ap_PathSwitchRequestFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_PathSwitchRequestFailureIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestFailure, s1ap_PathSwitchRequestFailure_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_PathSwitchRequestFailureIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestFailure, s1ap_PathSwitchRequestFailure_p);
                    return -1;
                }
                s1ap_PathSwitchRequestFailureIEs->presenceMask |= S1AP_PATHSWITCHREQUESTFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_pathswitchrequestfailureies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestFailure, s1ap_PathSwitchRequestFailure_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestFailure, s1ap_PathSwitchRequestFailure_p);
    return 0;
}

int s1ap_decode_s1ap_e_rabmodifyresponseies(
    S1ap_E_RABModifyResponseIEs_t *s1ap_E_RABModifyResponseIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_E_RABModifyResponse_t  s1ap_E_RABModifyResponse;
    S1ap_E_RABModifyResponse_t *s1ap_E_RABModifyResponse_p = &s1ap_E_RABModifyResponse;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_E_RABModifyResponseIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_E_RABModifyResponseIEs\n");

    memset(&s1ap_E_RABModifyResponse, 0, sizeof(S1ap_E_RABModifyResponse_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_RABModifyResponse, (void **)&s1ap_E_RABModifyResponse_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_E_RABModifyResponse failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyResponse, s1ap_E_RABModifyResponse_p);
        return -1;
    }

    for (i = 0; i < s1ap_E_RABModifyResponse_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_E_RABModifyResponse_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_E_RABModifyResponseIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyResponse, s1ap_E_RABModifyResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_E_RABModifyResponseIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyResponse, s1ap_E_RABModifyResponse_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABModifyListBearerModRes:
            {
                S1ap_E_RABModifyListBearerModRes_t *s1apERABModifyListBearerModRes_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABModifyListBearerModRes, (void**)&s1apERABModifyListBearerModRes_p);
                if (s1ap_decode_s1ap_e_rabmodifylistbearermodres(&s1ap_E_RABModifyResponseIEs->e_RABModifyListBearerModRes, s1apERABModifyListBearerModRes_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABModifyListBearerModRes failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABModifyListBearerModRes, s1apERABModifyListBearerModRes_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyResponse, s1ap_E_RABModifyResponse_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABModifyListBearerModRes, s1apERABModifyListBearerModRes_p);
                s1ap_E_RABModifyResponseIEs->presenceMask |= S1AP_E_RABMODIFYRESPONSEIES_E_RABMODIFYLISTBEARERMODRES_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABFailedToModifyList:
            {
                S1ap_E_RABList_t *s1apERABList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABList, (void**)&s1apERABList_p);
                if (s1ap_decode_s1ap_e_rablist(&s1ap_E_RABModifyResponseIEs->e_RABFailedToModifyList, s1apERABList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyResponse, s1ap_E_RABModifyResponse_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                s1ap_E_RABModifyResponseIEs->presenceMask |= S1AP_E_RABMODIFYRESPONSEIES_E_RABFAILEDTOMODIFYLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_E_RABModifyResponseIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyResponse, s1ap_E_RABModifyResponse_p);
                    return -1;
                }
                s1ap_E_RABModifyResponseIEs->presenceMask |= S1AP_E_RABMODIFYRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_e_rabmodifyresponseies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyResponse, s1ap_E_RABModifyResponse_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABModifyResponse, s1ap_E_RABModifyResponse_p);
    return 0;
}

int s1ap_decode_s1ap_handovernotifyies(
    S1ap_HandoverNotifyIEs_t *s1ap_HandoverNotifyIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverNotify_t  s1ap_HandoverNotify;
    S1ap_HandoverNotify_t *s1ap_HandoverNotify_p = &s1ap_HandoverNotify;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverNotifyIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverNotifyIEs\n");

    memset(&s1ap_HandoverNotify, 0, sizeof(S1ap_HandoverNotify_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverNotify, (void **)&s1ap_HandoverNotify_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverNotify failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverNotify, s1ap_HandoverNotify_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverNotify_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverNotify_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverNotifyIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverNotify, s1ap_HandoverNotify_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_HandoverNotifyIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverNotify, s1ap_HandoverNotify_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_EUTRAN_CGI:
            {
                S1ap_EUTRAN_CGI_t *s1apEUTRANCGI_p = &s1ap_HandoverNotifyIEs->eutran_cgi;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_EUTRAN_CGI, (void **)&s1apEUTRANCGI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANCGI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverNotify, s1ap_HandoverNotify_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TAI:
            {
                S1ap_TAI_t *s1apTAI_p = &s1ap_HandoverNotifyIEs->tai;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TAI, (void **)&s1apTAI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTAI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverNotify, s1ap_HandoverNotify_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handovernotifyies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverNotify, s1ap_HandoverNotify_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverNotify, s1ap_HandoverNotify_p);
    return 0;
}

int s1ap_decode_s1ap_handovercancelacknowledgeies(
    S1ap_HandoverCancelAcknowledgeIEs_t *s1ap_HandoverCancelAcknowledgeIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverCancelAcknowledge_t  s1ap_HandoverCancelAcknowledge;
    S1ap_HandoverCancelAcknowledge_t *s1ap_HandoverCancelAcknowledge_p = &s1ap_HandoverCancelAcknowledge;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverCancelAcknowledgeIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverCancelAcknowledgeIEs\n");

    memset(&s1ap_HandoverCancelAcknowledge, 0, sizeof(S1ap_HandoverCancelAcknowledge_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverCancelAcknowledge, (void **)&s1ap_HandoverCancelAcknowledge_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverCancelAcknowledge failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancelAcknowledge, s1ap_HandoverCancelAcknowledge_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverCancelAcknowledge_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverCancelAcknowledge_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverCancelAcknowledgeIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancelAcknowledge, s1ap_HandoverCancelAcknowledge_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_HandoverCancelAcknowledgeIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancelAcknowledge, s1ap_HandoverCancelAcknowledge_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_HandoverCancelAcknowledgeIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancelAcknowledge, s1ap_HandoverCancelAcknowledge_p);
                    return -1;
                }
                s1ap_HandoverCancelAcknowledgeIEs->presenceMask |= S1AP_HANDOVERCANCELACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handovercancelacknowledgeies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancelAcknowledge, s1ap_HandoverCancelAcknowledge_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCancelAcknowledge, s1ap_HandoverCancelAcknowledge_p);
    return 0;
}

int s1ap_decode_s1ap_initialcontextsetupfailureies(
    S1ap_InitialContextSetupFailureIEs_t *s1ap_InitialContextSetupFailureIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_InitialContextSetupFailure_t  s1ap_InitialContextSetupFailure;
    S1ap_InitialContextSetupFailure_t *s1ap_InitialContextSetupFailure_p = &s1ap_InitialContextSetupFailure;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_InitialContextSetupFailureIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_InitialContextSetupFailureIEs\n");

    memset(&s1ap_InitialContextSetupFailure, 0, sizeof(S1ap_InitialContextSetupFailure_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_InitialContextSetupFailure, (void **)&s1ap_InitialContextSetupFailure_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_InitialContextSetupFailure failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupFailure, s1ap_InitialContextSetupFailure_p);
        return -1;
    }

    for (i = 0; i < s1ap_InitialContextSetupFailure_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_InitialContextSetupFailure_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_InitialContextSetupFailureIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupFailure, s1ap_InitialContextSetupFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_InitialContextSetupFailureIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupFailure, s1ap_InitialContextSetupFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_InitialContextSetupFailureIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupFailure, s1ap_InitialContextSetupFailure_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_InitialContextSetupFailureIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupFailure, s1ap_InitialContextSetupFailure_p);
                    return -1;
                }
                s1ap_InitialContextSetupFailureIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_initialcontextsetupfailureies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupFailure, s1ap_InitialContextSetupFailure_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupFailure, s1ap_InitialContextSetupFailure_p);
    return 0;
}

int s1ap_decode_s1ap_handovercommandies(
    S1ap_HandoverCommandIEs_t *s1ap_HandoverCommandIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverCommand_t  s1ap_HandoverCommand;
    S1ap_HandoverCommand_t *s1ap_HandoverCommand_p = &s1ap_HandoverCommand;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverCommandIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverCommandIEs\n");

    memset(&s1ap_HandoverCommand, 0, sizeof(S1ap_HandoverCommand_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverCommand, (void **)&s1ap_HandoverCommand_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverCommand failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverCommand_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverCommand_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverCommandIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_HandoverCommandIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_HandoverType:
            {
                S1ap_HandoverType_t *s1apHandoverType_p = &s1ap_HandoverCommandIEs->handoverType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverType, (void **)&s1apHandoverType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apHandoverType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABDataForwardingList:
            {
                S1ap_E_RABDataForwardingList_t *s1apERABDataForwardingList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABDataForwardingList, (void**)&s1apERABDataForwardingList_p);
                if (s1ap_decode_s1ap_e_rabdataforwardinglist(&s1ap_HandoverCommandIEs->e_RABDataForwardingList, s1apERABDataForwardingList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABDataForwardingList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABDataForwardingList, s1apERABDataForwardingList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABDataForwardingList, s1apERABDataForwardingList_p);
                s1ap_HandoverCommandIEs->presenceMask |= S1AP_HANDOVERCOMMANDIES_E_RABDATAFORWARDINGLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABtoReleaseListHOCmd:
            {
                S1ap_E_RABList_t *s1apERABList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABList, (void**)&s1apERABList_p);
                if (s1ap_decode_s1ap_e_rablist(&s1ap_HandoverCommandIEs->e_RABtoReleaseListHOCmd, s1apERABList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                s1ap_HandoverCommandIEs->presenceMask |= S1AP_HANDOVERCOMMANDIES_E_RABTORELEASELISTHOCMD_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_Target_ToSource_TransparentContainer:
            {
                S1ap_Target_ToSource_TransparentContainer_t *s1apTargetToSourceTransparentContainer_p = &s1ap_HandoverCommandIEs->target_ToSource_TransparentContainer;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Target_ToSource_TransparentContainer, (void **)&s1apTargetToSourceTransparentContainer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTargetToSourceTransparentContainer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_Target_ToSource_TransparentContainer_Secondary:
            {
                S1ap_Target_ToSource_TransparentContainer_t *s1apTargetToSourceTransparentContainer_p = &s1ap_HandoverCommandIEs->target_ToSource_TransparentContainer_Secondary;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Target_ToSource_TransparentContainer, (void **)&s1apTargetToSourceTransparentContainer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTargetToSourceTransparentContainer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                    return -1;
                }
                s1ap_HandoverCommandIEs->presenceMask |= S1AP_HANDOVERCOMMANDIES_TARGET_TOSOURCE_TRANSPARENTCONTAINER_SECONDARY_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_HandoverCommandIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                    return -1;
                }
                s1ap_HandoverCommandIEs->presenceMask |= S1AP_HANDOVERCOMMANDIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handovercommandies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverCommand, s1ap_HandoverCommand_p);
    return 0;
}

int s1ap_decode_s1ap_mmedirectinformationtransferies(
    S1ap_MMEDirectInformationTransferIEs_t *s1ap_MMEDirectInformationTransferIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_MMEDirectInformationTransfer_t  s1ap_MMEDirectInformationTransfer;
    S1ap_MMEDirectInformationTransfer_t *s1ap_MMEDirectInformationTransfer_p = &s1ap_MMEDirectInformationTransfer;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_MMEDirectInformationTransferIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_MMEDirectInformationTransferIEs\n");

    memset(&s1ap_MMEDirectInformationTransfer, 0, sizeof(S1ap_MMEDirectInformationTransfer_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMEDirectInformationTransfer, (void **)&s1ap_MMEDirectInformationTransfer_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_MMEDirectInformationTransfer failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEDirectInformationTransfer, s1ap_MMEDirectInformationTransfer_p);
        return -1;
    }

    for (i = 0; i < s1ap_MMEDirectInformationTransfer_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_MMEDirectInformationTransfer_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Inter_SystemInformationTransferTypeMDT:
            {
                S1ap_Inter_SystemInformationTransferType_t *s1apInterSystemInformationTransferType_p = &s1ap_MMEDirectInformationTransferIEs->inter_SystemInformationTransferTypeMDT;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Inter_SystemInformationTransferType, (void **)&s1apInterSystemInformationTransferType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apInterSystemInformationTransferType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEDirectInformationTransfer, s1ap_MMEDirectInformationTransfer_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_mmedirectinformationtransferies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEDirectInformationTransfer, s1ap_MMEDirectInformationTransfer_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEDirectInformationTransfer, s1ap_MMEDirectInformationTransfer_p);
    return 0;
}

int s1ap_decode_s1ap_errorindicationies(
    S1ap_ErrorIndicationIEs_t *s1ap_ErrorIndicationIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_ErrorIndication_t  s1ap_ErrorIndication;
    S1ap_ErrorIndication_t *s1ap_ErrorIndication_p = &s1ap_ErrorIndication;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ErrorIndicationIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ErrorIndicationIEs\n");

    memset(&s1ap_ErrorIndication, 0, sizeof(S1ap_ErrorIndication_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ErrorIndication, (void **)&s1ap_ErrorIndication_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_ErrorIndication failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ErrorIndication, s1ap_ErrorIndication_p);
        return -1;
    }

    for (i = 0; i < s1ap_ErrorIndication_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_ErrorIndication_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_ErrorIndicationIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ErrorIndication, s1ap_ErrorIndication_p);
                    return -1;
                }
                s1ap_ErrorIndicationIEs->presenceMask |= S1AP_ERRORINDICATIONIES_MME_UE_S1AP_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_ErrorIndicationIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ErrorIndication, s1ap_ErrorIndication_p);
                    return -1;
                }
                s1ap_ErrorIndicationIEs->presenceMask |= S1AP_ERRORINDICATIONIES_ENB_UE_S1AP_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_ErrorIndicationIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ErrorIndication, s1ap_ErrorIndication_p);
                    return -1;
                }
                s1ap_ErrorIndicationIEs->presenceMask |= S1AP_ERRORINDICATIONIES_CAUSE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_ErrorIndicationIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ErrorIndication, s1ap_ErrorIndication_p);
                    return -1;
                }
                s1ap_ErrorIndicationIEs->presenceMask |= S1AP_ERRORINDICATIONIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_errorindicationies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ErrorIndication, s1ap_ErrorIndication_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ErrorIndication, s1ap_ErrorIndication_p);
    return 0;
}

int s1ap_decode_s1ap_overloadstarties(
    S1ap_OverloadStartIEs_t *s1ap_OverloadStartIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_OverloadStart_t  s1ap_OverloadStart;
    S1ap_OverloadStart_t *s1ap_OverloadStart_p = &s1ap_OverloadStart;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_OverloadStartIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_OverloadStartIEs\n");

    memset(&s1ap_OverloadStart, 0, sizeof(S1ap_OverloadStart_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_OverloadStart, (void **)&s1ap_OverloadStart_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_OverloadStart failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStart, s1ap_OverloadStart_p);
        return -1;
    }

    for (i = 0; i < s1ap_OverloadStart_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_OverloadStart_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_OverloadResponse:
            {
                S1ap_OverloadResponse_t *s1apOverloadResponse_p = &s1ap_OverloadStartIEs->overloadResponse;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_OverloadResponse, (void **)&s1apOverloadResponse_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apOverloadResponse failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStart, s1ap_OverloadStart_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_GUMMEIList:
            {
                S1ap_GUMMEIList_t *s1apGUMMEIList_p = &s1ap_OverloadStartIEs->gummeiList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_GUMMEIList, (void **)&s1apGUMMEIList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apGUMMEIList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStart, s1ap_OverloadStart_p);
                    return -1;
                }
                s1ap_OverloadStartIEs->presenceMask |= S1AP_OVERLOADSTARTIES_GUMMEILIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_TrafficLoadReductionIndication:
            {
                S1ap_TrafficLoadReductionIndication_t *s1apTrafficLoadReductionIndication_p = &s1ap_OverloadStartIEs->trafficLoadReductionIndication;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TrafficLoadReductionIndication, (void **)&s1apTrafficLoadReductionIndication_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTrafficLoadReductionIndication failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStart, s1ap_OverloadStart_p);
                    return -1;
                }
                s1ap_OverloadStartIEs->presenceMask |= S1AP_OVERLOADSTARTIES_TRAFFICLOADREDUCTIONINDICATION_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_overloadstarties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStart, s1ap_OverloadStart_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStart, s1ap_OverloadStart_p);
    return 0;
}

int s1ap_decode_s1ap_downlinknonueassociatedlppatransport_ies(
    S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_DownlinkNonUEAssociatedLPPaTransport_t  s1ap_DownlinkNonUEAssociatedLPPaTransport;
    S1ap_DownlinkNonUEAssociatedLPPaTransport_t *s1ap_DownlinkNonUEAssociatedLPPaTransport_p = &s1ap_DownlinkNonUEAssociatedLPPaTransport;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs\n");

    memset(&s1ap_DownlinkNonUEAssociatedLPPaTransport, 0, sizeof(S1ap_DownlinkNonUEAssociatedLPPaTransport_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_DownlinkNonUEAssociatedLPPaTransport, (void **)&s1ap_DownlinkNonUEAssociatedLPPaTransport_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_DownlinkNonUEAssociatedLPPaTransport failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNonUEAssociatedLPPaTransport, s1ap_DownlinkNonUEAssociatedLPPaTransport_p);
        return -1;
    }

    for (i = 0; i < s1ap_DownlinkNonUEAssociatedLPPaTransport_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_DownlinkNonUEAssociatedLPPaTransport_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Routing_ID:
            {
                S1ap_Routing_ID_t *s1apRoutingID_p = &s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs->routing_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Routing_ID, (void **)&s1apRoutingID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRoutingID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNonUEAssociatedLPPaTransport, s1ap_DownlinkNonUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_LPPa_PDU:
            {
                S1ap_LPPa_PDU_t *s1apLPPaPDU_p = &s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs->lpPa_PDU;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LPPa_PDU, (void **)&s1apLPPaPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apLPPaPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNonUEAssociatedLPPaTransport, s1ap_DownlinkNonUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_downlinknonueassociatedlppatransport_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNonUEAssociatedLPPaTransport, s1ap_DownlinkNonUEAssociatedLPPaTransport_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNonUEAssociatedLPPaTransport, s1ap_DownlinkNonUEAssociatedLPPaTransport_p);
    return 0;
}

int s1ap_decode_s1ap_handoverrequestacknowledgeies(
    S1ap_HandoverRequestAcknowledgeIEs_t *s1ap_HandoverRequestAcknowledgeIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverRequestAcknowledge_t  s1ap_HandoverRequestAcknowledge;
    S1ap_HandoverRequestAcknowledge_t *s1ap_HandoverRequestAcknowledge_p = &s1ap_HandoverRequestAcknowledge;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverRequestAcknowledgeIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverRequestAcknowledgeIEs\n");

    memset(&s1ap_HandoverRequestAcknowledge, 0, sizeof(S1ap_HandoverRequestAcknowledge_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverRequestAcknowledge, (void **)&s1ap_HandoverRequestAcknowledge_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverRequestAcknowledge failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverRequestAcknowledge_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverRequestAcknowledge_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverRequestAcknowledgeIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_HandoverRequestAcknowledgeIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABAdmittedList:
            {
                S1ap_E_RABAdmittedList_t *s1apERABAdmittedList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABAdmittedList, (void**)&s1apERABAdmittedList_p);
                if (s1ap_decode_s1ap_e_rabadmittedlist(&s1ap_HandoverRequestAcknowledgeIEs->e_RABAdmittedList, s1apERABAdmittedList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABAdmittedList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABAdmittedList, s1apERABAdmittedList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABAdmittedList, s1apERABAdmittedList_p);
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABFailedToSetupListHOReqAck:
            {
                S1ap_E_RABFailedtoSetupListHOReqAck_t *s1apERABFailedtoSetupListHOReqAck_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABFailedtoSetupListHOReqAck, (void**)&s1apERABFailedtoSetupListHOReqAck_p);
                if (s1ap_decode_s1ap_e_rabfailedtosetuplisthoreqack(&s1ap_HandoverRequestAcknowledgeIEs->e_RABFailedToSetupListHOReqAck, s1apERABFailedtoSetupListHOReqAck_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABFailedtoSetupListHOReqAck failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABFailedtoSetupListHOReqAck, s1apERABFailedtoSetupListHOReqAck_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABFailedtoSetupListHOReqAck, s1apERABFailedtoSetupListHOReqAck_p);
                s1ap_HandoverRequestAcknowledgeIEs->presenceMask |= S1AP_HANDOVERREQUESTACKNOWLEDGEIES_E_RABFAILEDTOSETUPLISTHOREQACK_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_Target_ToSource_TransparentContainer:
            {
                S1ap_Target_ToSource_TransparentContainer_t *s1apTargetToSourceTransparentContainer_p = &s1ap_HandoverRequestAcknowledgeIEs->target_ToSource_TransparentContainer;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Target_ToSource_TransparentContainer, (void **)&s1apTargetToSourceTransparentContainer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTargetToSourceTransparentContainer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSG_Id:
            {
                S1ap_CSG_Id_t *s1apCSGId_p = &s1ap_HandoverRequestAcknowledgeIEs->csG_Id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSG_Id, (void **)&s1apCSGId_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGId failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
                    return -1;
                }
                s1ap_HandoverRequestAcknowledgeIEs->presenceMask |= S1AP_HANDOVERREQUESTACKNOWLEDGEIES_CSG_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_HandoverRequestAcknowledgeIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
                    return -1;
                }
                s1ap_HandoverRequestAcknowledgeIEs->presenceMask |= S1AP_HANDOVERREQUESTACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handoverrequestacknowledgeies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRequestAcknowledge, s1ap_HandoverRequestAcknowledge_p);
    return 0;
}

int s1ap_decode_s1ap_downlinkueassociatedlppatransport_ies(
    S1ap_DownlinkUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkUEAssociatedLPPaTransport_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_DownlinkUEAssociatedLPPaTransport_t  s1ap_DownlinkUEAssociatedLPPaTransport;
    S1ap_DownlinkUEAssociatedLPPaTransport_t *s1ap_DownlinkUEAssociatedLPPaTransport_p = &s1ap_DownlinkUEAssociatedLPPaTransport;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_DownlinkUEAssociatedLPPaTransport_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_DownlinkUEAssociatedLPPaTransport_IEs\n");

    memset(&s1ap_DownlinkUEAssociatedLPPaTransport, 0, sizeof(S1ap_DownlinkUEAssociatedLPPaTransport_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_DownlinkUEAssociatedLPPaTransport, (void **)&s1ap_DownlinkUEAssociatedLPPaTransport_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_DownlinkUEAssociatedLPPaTransport failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkUEAssociatedLPPaTransport, s1ap_DownlinkUEAssociatedLPPaTransport_p);
        return -1;
    }

    for (i = 0; i < s1ap_DownlinkUEAssociatedLPPaTransport_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_DownlinkUEAssociatedLPPaTransport_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkUEAssociatedLPPaTransport, s1ap_DownlinkUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkUEAssociatedLPPaTransport, s1ap_DownlinkUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Routing_ID:
            {
                S1ap_Routing_ID_t *s1apRoutingID_p = &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->routing_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Routing_ID, (void **)&s1apRoutingID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRoutingID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkUEAssociatedLPPaTransport, s1ap_DownlinkUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_LPPa_PDU:
            {
                S1ap_LPPa_PDU_t *s1apLPPaPDU_p = &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->lpPa_PDU;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LPPa_PDU, (void **)&s1apLPPaPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apLPPaPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkUEAssociatedLPPaTransport, s1ap_DownlinkUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_downlinkueassociatedlppatransport_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkUEAssociatedLPPaTransport, s1ap_DownlinkUEAssociatedLPPaTransport_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkUEAssociatedLPPaTransport, s1ap_DownlinkUEAssociatedLPPaTransport_p);
    return 0;
}

int s1ap_decode_s1ap_uecontextmodificationfailureies(
    S1ap_UEContextModificationFailureIEs_t *s1ap_UEContextModificationFailureIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UEContextModificationFailure_t  s1ap_UEContextModificationFailure;
    S1ap_UEContextModificationFailure_t *s1ap_UEContextModificationFailure_p = &s1ap_UEContextModificationFailure;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UEContextModificationFailureIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UEContextModificationFailureIEs\n");

    memset(&s1ap_UEContextModificationFailure, 0, sizeof(S1ap_UEContextModificationFailure_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEContextModificationFailure, (void **)&s1ap_UEContextModificationFailure_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UEContextModificationFailure failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationFailure, s1ap_UEContextModificationFailure_p);
        return -1;
    }

    for (i = 0; i < s1ap_UEContextModificationFailure_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UEContextModificationFailure_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UEContextModificationFailureIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationFailure, s1ap_UEContextModificationFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UEContextModificationFailureIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationFailure, s1ap_UEContextModificationFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_UEContextModificationFailureIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationFailure, s1ap_UEContextModificationFailure_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_UEContextModificationFailureIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationFailure, s1ap_UEContextModificationFailure_p);
                    return -1;
                }
                s1ap_UEContextModificationFailureIEs->presenceMask |= S1AP_UECONTEXTMODIFICATIONFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uecontextmodificationfailureies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationFailure, s1ap_UEContextModificationFailure_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextModificationFailure, s1ap_UEContextModificationFailure_p);
    return 0;
}

int s1ap_decode_s1ap_mmeconfigurationtransferies(
    S1ap_MMEConfigurationTransferIEs_t *s1ap_MMEConfigurationTransferIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_MMEConfigurationTransfer_t  s1ap_MMEConfigurationTransfer;
    S1ap_MMEConfigurationTransfer_t *s1ap_MMEConfigurationTransfer_p = &s1ap_MMEConfigurationTransfer;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_MMEConfigurationTransferIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_MMEConfigurationTransferIEs\n");

    memset(&s1ap_MMEConfigurationTransfer, 0, sizeof(S1ap_MMEConfigurationTransfer_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMEConfigurationTransfer, (void **)&s1ap_MMEConfigurationTransfer_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_MMEConfigurationTransfer failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationTransfer, s1ap_MMEConfigurationTransfer_p);
        return -1;
    }

    for (i = 0; i < s1ap_MMEConfigurationTransfer_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_MMEConfigurationTransfer_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SONConfigurationTransferMCT:
            {
                S1ap_SONConfigurationTransfer_t *s1apSONConfigurationTransfer_p = &s1ap_MMEConfigurationTransferIEs->sonConfigurationTransferMCT;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SONConfigurationTransfer, (void **)&s1apSONConfigurationTransfer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSONConfigurationTransfer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationTransfer, s1ap_MMEConfigurationTransfer_p);
                    return -1;
                }
                s1ap_MMEConfigurationTransferIEs->presenceMask |= S1AP_MMECONFIGURATIONTRANSFERIES_SONCONFIGURATIONTRANSFERMCT_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_mmeconfigurationtransferies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationTransfer, s1ap_MMEConfigurationTransfer_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationTransfer, s1ap_MMEConfigurationTransfer_p);
    return 0;
}

int s1ap_decode_s1ap_pathswitchrequestacknowledgeies(
    S1ap_PathSwitchRequestAcknowledgeIEs_t *s1ap_PathSwitchRequestAcknowledgeIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_PathSwitchRequestAcknowledge_t  s1ap_PathSwitchRequestAcknowledge;
    S1ap_PathSwitchRequestAcknowledge_t *s1ap_PathSwitchRequestAcknowledge_p = &s1ap_PathSwitchRequestAcknowledge;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_PathSwitchRequestAcknowledgeIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_PathSwitchRequestAcknowledgeIEs\n");

    memset(&s1ap_PathSwitchRequestAcknowledge, 0, sizeof(S1ap_PathSwitchRequestAcknowledge_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PathSwitchRequestAcknowledge, (void **)&s1ap_PathSwitchRequestAcknowledge_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_PathSwitchRequestAcknowledge failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
        return -1;
    }

    for (i = 0; i < s1ap_PathSwitchRequestAcknowledge_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_PathSwitchRequestAcknowledge_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_PathSwitchRequestAcknowledgeIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_PathSwitchRequestAcknowledgeIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate:
            {
                S1ap_UEAggregateMaximumBitrate_t *s1apUEAggregateMaximumBitrate_p = &s1ap_PathSwitchRequestAcknowledgeIEs->uEaggregateMaximumBitrate;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEAggregateMaximumBitrate, (void **)&s1apUEAggregateMaximumBitrate_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEAggregateMaximumBitrate failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                    return -1;
                }
                s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask |= S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABToBeSwitchedULList:
            {
                S1ap_E_RABToBeSwitchedULList_t *s1apERABToBeSwitchedULList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSwitchedULList, (void**)&s1apERABToBeSwitchedULList_p);
                if (s1ap_decode_s1ap_e_rabtobeswitchedullist(&s1ap_PathSwitchRequestAcknowledgeIEs->e_RABToBeSwitchedULList, s1apERABToBeSwitchedULList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABToBeSwitchedULList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSwitchedULList, s1apERABToBeSwitchedULList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSwitchedULList, s1apERABToBeSwitchedULList_p);
                s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask |= S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_E_RABTOBESWITCHEDULLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABToBeReleasedList:
            {
                S1ap_E_RABList_t *s1apERABList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABList, (void**)&s1apERABList_p);
                if (s1ap_decode_s1ap_e_rablist(&s1ap_PathSwitchRequestAcknowledgeIEs->e_RABToBeReleasedList, s1apERABList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask |= S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_E_RABTOBERELEASEDLIST_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_SecurityContext:
            {
                S1ap_SecurityContext_t *s1apSecurityContext_p = &s1ap_PathSwitchRequestAcknowledgeIEs->securityContext;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SecurityContext, (void **)&s1apSecurityContext_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSecurityContext failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_PathSwitchRequestAcknowledgeIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                    return -1;
                }
                s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask |= S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID_2:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_PathSwitchRequestAcknowledgeIEs->mme_ue_s1ap_id_2;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                    return -1;
                }
                s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask |= S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_MME_UE_S1AP_ID_2_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_pathswitchrequestacknowledgeies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequestAcknowledge, s1ap_PathSwitchRequestAcknowledge_p);
    return 0;
}

int s1ap_decode_s1ap_s1setupfailureies(
    S1ap_S1SetupFailureIEs_t *s1ap_S1SetupFailureIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_S1SetupFailure_t  s1ap_S1SetupFailure;
    S1ap_S1SetupFailure_t *s1ap_S1SetupFailure_p = &s1ap_S1SetupFailure;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_S1SetupFailureIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_S1SetupFailureIEs\n");

    memset(&s1ap_S1SetupFailure, 0, sizeof(S1ap_S1SetupFailure_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_S1SetupFailure, (void **)&s1ap_S1SetupFailure_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_S1SetupFailure failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupFailure, s1ap_S1SetupFailure_p);
        return -1;
    }

    for (i = 0; i < s1ap_S1SetupFailure_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_S1SetupFailure_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_S1SetupFailureIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupFailure, s1ap_S1SetupFailure_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_TimeToWait:
            {
                S1ap_TimeToWait_t *s1apTimeToWait_p = &s1ap_S1SetupFailureIEs->timeToWait;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TimeToWait, (void **)&s1apTimeToWait_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTimeToWait failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupFailure, s1ap_S1SetupFailure_p);
                    return -1;
                }
                s1ap_S1SetupFailureIEs->presenceMask |= S1AP_S1SETUPFAILUREIES_TIMETOWAIT_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_S1SetupFailureIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupFailure, s1ap_S1SetupFailure_p);
                    return -1;
                }
                s1ap_S1SetupFailureIEs->presenceMask |= S1AP_S1SETUPFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_s1setupfailureies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupFailure, s1ap_S1SetupFailure_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupFailure, s1ap_S1SetupFailure_p);
    return 0;
}

int s1ap_decode_s1ap_mmeconfigurationupdatefailureies(
    S1ap_MMEConfigurationUpdateFailureIEs_t *s1ap_MMEConfigurationUpdateFailureIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_MMEConfigurationUpdateFailure_t  s1ap_MMEConfigurationUpdateFailure;
    S1ap_MMEConfigurationUpdateFailure_t *s1ap_MMEConfigurationUpdateFailure_p = &s1ap_MMEConfigurationUpdateFailure;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_MMEConfigurationUpdateFailureIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_MMEConfigurationUpdateFailureIEs\n");

    memset(&s1ap_MMEConfigurationUpdateFailure, 0, sizeof(S1ap_MMEConfigurationUpdateFailure_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MMEConfigurationUpdateFailure, (void **)&s1ap_MMEConfigurationUpdateFailure_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_MMEConfigurationUpdateFailure failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateFailure, s1ap_MMEConfigurationUpdateFailure_p);
        return -1;
    }

    for (i = 0; i < s1ap_MMEConfigurationUpdateFailure_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_MMEConfigurationUpdateFailure_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_MMEConfigurationUpdateFailureIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateFailure, s1ap_MMEConfigurationUpdateFailure_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_TimeToWait:
            {
                S1ap_TimeToWait_t *s1apTimeToWait_p = &s1ap_MMEConfigurationUpdateFailureIEs->timeToWait;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TimeToWait, (void **)&s1apTimeToWait_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTimeToWait failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateFailure, s1ap_MMEConfigurationUpdateFailure_p);
                    return -1;
                }
                s1ap_MMEConfigurationUpdateFailureIEs->presenceMask |= S1AP_MMECONFIGURATIONUPDATEFAILUREIES_TIMETOWAIT_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_MMEConfigurationUpdateFailureIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateFailure, s1ap_MMEConfigurationUpdateFailure_p);
                    return -1;
                }
                s1ap_MMEConfigurationUpdateFailureIEs->presenceMask |= S1AP_MMECONFIGURATIONUPDATEFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_mmeconfigurationupdatefailureies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateFailure, s1ap_MMEConfigurationUpdateFailure_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEConfigurationUpdateFailure, s1ap_MMEConfigurationUpdateFailure_p);
    return 0;
}

int s1ap_decode_s1ap_initialcontextsetupresponseies(
    S1ap_InitialContextSetupResponseIEs_t *s1ap_InitialContextSetupResponseIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_InitialContextSetupResponse_t  s1ap_InitialContextSetupResponse;
    S1ap_InitialContextSetupResponse_t *s1ap_InitialContextSetupResponse_p = &s1ap_InitialContextSetupResponse;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_InitialContextSetupResponseIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_InitialContextSetupResponseIEs\n");

    memset(&s1ap_InitialContextSetupResponse, 0, sizeof(S1ap_InitialContextSetupResponse_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_InitialContextSetupResponse, (void **)&s1ap_InitialContextSetupResponse_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_InitialContextSetupResponse failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupResponse, s1ap_InitialContextSetupResponse_p);
        return -1;
    }

    for (i = 0; i < s1ap_InitialContextSetupResponse_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_InitialContextSetupResponse_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_InitialContextSetupResponseIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupResponse, s1ap_InitialContextSetupResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_InitialContextSetupResponseIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupResponse, s1ap_InitialContextSetupResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABSetupListCtxtSURes:
            {
                S1ap_E_RABSetupListCtxtSURes_t *s1apERABSetupListCtxtSURes_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABSetupListCtxtSURes, (void**)&s1apERABSetupListCtxtSURes_p);
                if (s1ap_decode_s1ap_e_rabsetuplistctxtsures(&s1ap_InitialContextSetupResponseIEs->e_RABSetupListCtxtSURes, s1apERABSetupListCtxtSURes_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABSetupListCtxtSURes failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABSetupListCtxtSURes, s1apERABSetupListCtxtSURes_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupResponse, s1ap_InitialContextSetupResponse_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABSetupListCtxtSURes, s1apERABSetupListCtxtSURes_p);
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABFailedToSetupListCtxtSURes:
            {
                S1ap_E_RABList_t *s1apERABList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABList, (void**)&s1apERABList_p);
                if (s1ap_decode_s1ap_e_rablist(&s1ap_InitialContextSetupResponseIEs->e_RABFailedToSetupListCtxtSURes, s1apERABList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupResponse, s1ap_InitialContextSetupResponse_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                s1ap_InitialContextSetupResponseIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPRESPONSEIES_E_RABFAILEDTOSETUPLISTCTXTSURES_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_InitialContextSetupResponseIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupResponse, s1ap_InitialContextSetupResponse_p);
                    return -1;
                }
                s1ap_InitialContextSetupResponseIEs->presenceMask |= S1AP_INITIALCONTEXTSETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_initialcontextsetupresponseies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupResponse, s1ap_InitialContextSetupResponse_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_InitialContextSetupResponse, s1ap_InitialContextSetupResponse_p);
    return 0;
}

int s1ap_decode_s1ap_downlinks1cdma2000tunnelingies(
    S1ap_DownlinkS1cdma2000tunnelingIEs_t *s1ap_DownlinkS1cdma2000tunnelingIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_DownlinkS1cdma2000tunneling_t  s1ap_DownlinkS1cdma2000tunneling;
    S1ap_DownlinkS1cdma2000tunneling_t *s1ap_DownlinkS1cdma2000tunneling_p = &s1ap_DownlinkS1cdma2000tunneling;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_DownlinkS1cdma2000tunnelingIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_DownlinkS1cdma2000tunnelingIEs\n");

    memset(&s1ap_DownlinkS1cdma2000tunneling, 0, sizeof(S1ap_DownlinkS1cdma2000tunneling_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_DownlinkS1cdma2000tunneling, (void **)&s1ap_DownlinkS1cdma2000tunneling_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_DownlinkS1cdma2000tunneling failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
        return -1;
    }

    for (i = 0; i < s1ap_DownlinkS1cdma2000tunneling_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_DownlinkS1cdma2000tunneling_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_DownlinkS1cdma2000tunnelingIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_DownlinkS1cdma2000tunnelingIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABDataForwardingList:
            {
                S1ap_E_RABDataForwardingList_t *s1apERABDataForwardingList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABDataForwardingList, (void**)&s1apERABDataForwardingList_p);
                if (s1ap_decode_s1ap_e_rabdataforwardinglist(&s1ap_DownlinkS1cdma2000tunnelingIEs->e_RABDataForwardingList, s1apERABDataForwardingList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABDataForwardingList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABDataForwardingList, s1apERABDataForwardingList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABDataForwardingList, s1apERABDataForwardingList_p);
                s1ap_DownlinkS1cdma2000tunnelingIEs->presenceMask |= S1AP_DOWNLINKS1CDMA2000TUNNELINGIES_E_RABDATAFORWARDINGLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_cdma2000HOStatus:
            {
                S1ap_Cdma2000HOStatus_t *s1apCdma2000HOStatus_p = &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000HOStatus;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000HOStatus, (void **)&s1apCdma2000HOStatus_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000HOStatus failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
                    return -1;
                }
                s1ap_DownlinkS1cdma2000tunnelingIEs->presenceMask |= S1AP_DOWNLINKS1CDMA2000TUNNELINGIES_CDMA2000HOSTATUS_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_cdma2000RATType:
            {
                S1ap_Cdma2000RATType_t *s1apCdma2000RATType_p = &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000RATType;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000RATType, (void **)&s1apCdma2000RATType_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000RATType failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_cdma2000PDU:
            {
                S1ap_Cdma2000PDU_t *s1apCdma2000PDU_p = &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000PDU;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cdma2000PDU, (void **)&s1apCdma2000PDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCdma2000PDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_downlinks1cdma2000tunnelingies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkS1cdma2000tunneling, s1ap_DownlinkS1cdma2000tunneling_p);
    return 0;
}

int s1ap_decode_s1ap_overloadstopies(
    S1ap_OverloadStopIEs_t *s1ap_OverloadStopIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_OverloadStop_t  s1ap_OverloadStop;
    S1ap_OverloadStop_t *s1ap_OverloadStop_p = &s1ap_OverloadStop;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_OverloadStopIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_OverloadStopIEs\n");

    memset(&s1ap_OverloadStop, 0, sizeof(S1ap_OverloadStop_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_OverloadStop, (void **)&s1ap_OverloadStop_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_OverloadStop failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStop, s1ap_OverloadStop_p);
        return -1;
    }

    for (i = 0; i < s1ap_OverloadStop_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_OverloadStop_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_GUMMEIList:
            {
                S1ap_GUMMEIList_t *s1apGUMMEIList_p = &s1ap_OverloadStopIEs->gummeiList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_GUMMEIList, (void **)&s1apGUMMEIList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apGUMMEIList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStop, s1ap_OverloadStop_p);
                    return -1;
                }
                s1ap_OverloadStopIEs->presenceMask |= S1AP_OVERLOADSTOPIES_GUMMEILIST_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_overloadstopies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStop, s1ap_OverloadStop_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadStop, s1ap_OverloadStop_p);
    return 0;
}

int s1ap_decode_s1ap_enbconfigurationupdateies(
    S1ap_ENBConfigurationUpdateIEs_t *s1ap_ENBConfigurationUpdateIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_ENBConfigurationUpdate_t  s1ap_ENBConfigurationUpdate;
    S1ap_ENBConfigurationUpdate_t *s1ap_ENBConfigurationUpdate_p = &s1ap_ENBConfigurationUpdate;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ENBConfigurationUpdateIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ENBConfigurationUpdateIEs\n");

    memset(&s1ap_ENBConfigurationUpdate, 0, sizeof(S1ap_ENBConfigurationUpdate_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENBConfigurationUpdate, (void **)&s1ap_ENBConfigurationUpdate_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_ENBConfigurationUpdate failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdate, s1ap_ENBConfigurationUpdate_p);
        return -1;
    }

    for (i = 0; i < s1ap_ENBConfigurationUpdate_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_ENBConfigurationUpdate_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_eNBname:
            {
                S1ap_ENBname_t *s1apENBname_p = &s1ap_ENBConfigurationUpdateIEs->eNBname;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENBname, (void **)&s1apENBname_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBname failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdate, s1ap_ENBConfigurationUpdate_p);
                    return -1;
                }
                s1ap_ENBConfigurationUpdateIEs->presenceMask |= S1AP_ENBCONFIGURATIONUPDATEIES_ENBNAME_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SupportedTAs:
            {
                S1ap_SupportedTAs_t *s1apSupportedTAs_p = &s1ap_ENBConfigurationUpdateIEs->supportedTAs;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SupportedTAs, (void **)&s1apSupportedTAs_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSupportedTAs failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdate, s1ap_ENBConfigurationUpdate_p);
                    return -1;
                }
                s1ap_ENBConfigurationUpdateIEs->presenceMask |= S1AP_ENBCONFIGURATIONUPDATEIES_SUPPORTEDTAS_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSG_IdList:
            {
                S1ap_CSG_IdList_t *s1apCSGIdList_p = &s1ap_ENBConfigurationUpdateIEs->csG_IdList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSG_IdList, (void **)&s1apCSGIdList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGIdList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdate, s1ap_ENBConfigurationUpdate_p);
                    return -1;
                }
                s1ap_ENBConfigurationUpdateIEs->presenceMask |= S1AP_ENBCONFIGURATIONUPDATEIES_CSG_IDLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_DefaultPagingDRX:
            {
                S1ap_PagingDRX_t *s1apPagingDRX_p = &s1ap_ENBConfigurationUpdateIEs->defaultPagingDRX;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PagingDRX, (void **)&s1apPagingDRX_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apPagingDRX failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdate, s1ap_ENBConfigurationUpdate_p);
                    return -1;
                }
                s1ap_ENBConfigurationUpdateIEs->presenceMask |= S1AP_ENBCONFIGURATIONUPDATEIES_DEFAULTPAGINGDRX_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_enbconfigurationupdateies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdate, s1ap_ENBConfigurationUpdate_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationUpdate, s1ap_ENBConfigurationUpdate_p);
    return 0;
}

int s1ap_decode_s1ap_tracefailureindicationies(
    S1ap_TraceFailureIndicationIEs_t *s1ap_TraceFailureIndicationIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_TraceFailureIndication_t  s1ap_TraceFailureIndication;
    S1ap_TraceFailureIndication_t *s1ap_TraceFailureIndication_p = &s1ap_TraceFailureIndication;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_TraceFailureIndicationIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_TraceFailureIndicationIEs\n");

    memset(&s1ap_TraceFailureIndication, 0, sizeof(S1ap_TraceFailureIndication_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TraceFailureIndication, (void **)&s1ap_TraceFailureIndication_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_TraceFailureIndication failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceFailureIndication, s1ap_TraceFailureIndication_p);
        return -1;
    }

    for (i = 0; i < s1ap_TraceFailureIndication_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_TraceFailureIndication_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_TraceFailureIndicationIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceFailureIndication, s1ap_TraceFailureIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_TraceFailureIndicationIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceFailureIndication, s1ap_TraceFailureIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_UTRAN_Trace_ID:
            {
                S1ap_E_UTRAN_Trace_ID_t *s1apEUTRANTraceID_p = &s1ap_TraceFailureIndicationIEs->e_UTRAN_Trace_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_UTRAN_Trace_ID, (void **)&s1apEUTRANTraceID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANTraceID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceFailureIndication, s1ap_TraceFailureIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_TraceFailureIndicationIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceFailureIndication, s1ap_TraceFailureIndication_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_tracefailureindicationies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceFailureIndication, s1ap_TraceFailureIndication_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceFailureIndication, s1ap_TraceFailureIndication_p);
    return 0;
}

int s1ap_decode_s1ap_enbconfigurationtransferies(
    S1ap_ENBConfigurationTransferIEs_t *s1ap_ENBConfigurationTransferIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_ENBConfigurationTransfer_t  s1ap_ENBConfigurationTransfer;
    S1ap_ENBConfigurationTransfer_t *s1ap_ENBConfigurationTransfer_p = &s1ap_ENBConfigurationTransfer;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ENBConfigurationTransferIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ENBConfigurationTransferIEs\n");

    memset(&s1ap_ENBConfigurationTransfer, 0, sizeof(S1ap_ENBConfigurationTransfer_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENBConfigurationTransfer, (void **)&s1ap_ENBConfigurationTransfer_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_ENBConfigurationTransfer failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationTransfer, s1ap_ENBConfigurationTransfer_p);
        return -1;
    }

    for (i = 0; i < s1ap_ENBConfigurationTransfer_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_ENBConfigurationTransfer_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SONConfigurationTransferECT:
            {
                S1ap_SONConfigurationTransfer_t *s1apSONConfigurationTransfer_p = &s1ap_ENBConfigurationTransferIEs->sonConfigurationTransferECT;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SONConfigurationTransfer, (void **)&s1apSONConfigurationTransfer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSONConfigurationTransfer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationTransfer, s1ap_ENBConfigurationTransfer_p);
                    return -1;
                }
                s1ap_ENBConfigurationTransferIEs->presenceMask |= S1AP_ENBCONFIGURATIONTRANSFERIES_SONCONFIGURATIONTRANSFERECT_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_enbconfigurationtransferies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationTransfer, s1ap_ENBConfigurationTransfer_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBConfigurationTransfer, s1ap_ENBConfigurationTransfer_p);
    return 0;
}

int s1ap_decode_s1ap_handoverfailureies(
    S1ap_HandoverFailureIEs_t *s1ap_HandoverFailureIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_HandoverFailure_t  s1ap_HandoverFailure;
    S1ap_HandoverFailure_t *s1ap_HandoverFailure_p = &s1ap_HandoverFailure;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_HandoverFailureIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_HandoverFailureIEs\n");

    memset(&s1ap_HandoverFailure, 0, sizeof(S1ap_HandoverFailure_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverFailure, (void **)&s1ap_HandoverFailure_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_HandoverFailure failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverFailure, s1ap_HandoverFailure_p);
        return -1;
    }

    for (i = 0; i < s1ap_HandoverFailure_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_HandoverFailure_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_HandoverFailureIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverFailure, s1ap_HandoverFailure_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_HandoverFailureIEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverFailure, s1ap_HandoverFailure_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_HandoverFailureIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverFailure, s1ap_HandoverFailure_p);
                    return -1;
                }
                s1ap_HandoverFailureIEs->presenceMask |= S1AP_HANDOVERFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_handoverfailureies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverFailure, s1ap_HandoverFailure_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverFailure, s1ap_HandoverFailure_p);
    return 0;
}

int s1ap_decode_s1ap_e_rabsetupresponseies(
    S1ap_E_RABSetupResponseIEs_t *s1ap_E_RABSetupResponseIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_E_RABSetupResponse_t  s1ap_E_RABSetupResponse;
    S1ap_E_RABSetupResponse_t *s1ap_E_RABSetupResponse_p = &s1ap_E_RABSetupResponse;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_E_RABSetupResponseIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_E_RABSetupResponseIEs\n");

    memset(&s1ap_E_RABSetupResponse, 0, sizeof(S1ap_E_RABSetupResponse_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_RABSetupResponse, (void **)&s1ap_E_RABSetupResponse_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_E_RABSetupResponse failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupResponse, s1ap_E_RABSetupResponse_p);
        return -1;
    }

    for (i = 0; i < s1ap_E_RABSetupResponse_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_E_RABSetupResponse_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_E_RABSetupResponseIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupResponse, s1ap_E_RABSetupResponse_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_E_RABSetupResponseIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupResponse, s1ap_E_RABSetupResponse_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABSetupListBearerSURes:
            {
                S1ap_E_RABSetupListBearerSURes_t *s1apERABSetupListBearerSURes_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABSetupListBearerSURes, (void**)&s1apERABSetupListBearerSURes_p);
                if (s1ap_decode_s1ap_e_rabsetuplistbearersures(&s1ap_E_RABSetupResponseIEs->e_RABSetupListBearerSURes, s1apERABSetupListBearerSURes_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABSetupListBearerSURes failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABSetupListBearerSURes, s1apERABSetupListBearerSURes_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupResponse, s1ap_E_RABSetupResponse_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABSetupListBearerSURes, s1apERABSetupListBearerSURes_p);
                s1ap_E_RABSetupResponseIEs->presenceMask |= S1AP_E_RABSETUPRESPONSEIES_E_RABSETUPLISTBEARERSURES_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_E_RABFailedToSetupListBearerSURes:
            {
                S1ap_E_RABList_t *s1apERABList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABList, (void**)&s1apERABList_p);
                if (s1ap_decode_s1ap_e_rablist(&s1ap_E_RABSetupResponseIEs->e_RABFailedToSetupListBearerSURes, s1apERABList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupResponse, s1ap_E_RABSetupResponse_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                s1ap_E_RABSetupResponseIEs->presenceMask |= S1AP_E_RABSETUPRESPONSEIES_E_RABFAILEDTOSETUPLISTBEARERSURES_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_E_RABSetupResponseIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupResponse, s1ap_E_RABSetupResponse_p);
                    return -1;
                }
                s1ap_E_RABSetupResponseIEs->presenceMask |= S1AP_E_RABSETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_e_rabsetupresponseies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupResponse, s1ap_E_RABSetupResponse_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupResponse, s1ap_E_RABSetupResponse_p);
    return 0;
}

int s1ap_decode_s1ap_uecontextreleasecommand_ies(
    S1ap_UEContextReleaseCommand_IEs_t *s1ap_UEContextReleaseCommand_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UEContextReleaseCommand_t  s1ap_UEContextReleaseCommand;
    S1ap_UEContextReleaseCommand_t *s1ap_UEContextReleaseCommand_p = &s1ap_UEContextReleaseCommand;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UEContextReleaseCommand_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UEContextReleaseCommand_IEs\n");

    memset(&s1ap_UEContextReleaseCommand, 0, sizeof(S1ap_UEContextReleaseCommand_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEContextReleaseCommand, (void **)&s1ap_UEContextReleaseCommand_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UEContextReleaseCommand failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseCommand, s1ap_UEContextReleaseCommand_p);
        return -1;
    }

    for (i = 0; i < s1ap_UEContextReleaseCommand_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UEContextReleaseCommand_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_UE_S1AP_IDs:
            {
                S1ap_UE_S1AP_IDs_t *s1apUES1APIDs_p = &s1ap_UEContextReleaseCommand_IEs->uE_S1AP_IDs;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UE_S1AP_IDs, (void **)&s1apUES1APIDs_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUES1APIDs failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseCommand, s1ap_UEContextReleaseCommand_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_UEContextReleaseCommand_IEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseCommand, s1ap_UEContextReleaseCommand_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uecontextreleasecommand_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseCommand, s1ap_UEContextReleaseCommand_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseCommand, s1ap_UEContextReleaseCommand_p);
    return 0;
}

int s1ap_decode_s1ap_pathswitchrequesties(
    S1ap_PathSwitchRequestIEs_t *s1ap_PathSwitchRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_PathSwitchRequest_t  s1ap_PathSwitchRequest;
    S1ap_PathSwitchRequest_t *s1ap_PathSwitchRequest_p = &s1ap_PathSwitchRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_PathSwitchRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_PathSwitchRequestIEs\n");

    memset(&s1ap_PathSwitchRequest, 0, sizeof(S1ap_PathSwitchRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PathSwitchRequest, (void **)&s1ap_PathSwitchRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_PathSwitchRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_PathSwitchRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_PathSwitchRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_PathSwitchRequestIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABToBeSwitchedDLList:
            {
                S1ap_E_RABToBeSwitchedDLList_t *s1apERABToBeSwitchedDLList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSwitchedDLList, (void**)&s1apERABToBeSwitchedDLList_p);
                if (s1ap_decode_s1ap_e_rabtobeswitcheddllist(&s1ap_PathSwitchRequestIEs->e_RABToBeSwitchedDLList, s1apERABToBeSwitchedDLList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABToBeSwitchedDLList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSwitchedDLList, s1apERABToBeSwitchedDLList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSwitchedDLList, s1apERABToBeSwitchedDLList_p);
            } break;
            case S1ap_ProtocolIE_ID_id_SourceMME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_PathSwitchRequestIEs->sourceMME_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_EUTRAN_CGI:
            {
                S1ap_EUTRAN_CGI_t *s1apEUTRANCGI_p = &s1ap_PathSwitchRequestIEs->eutran_cgi;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_EUTRAN_CGI, (void **)&s1apEUTRANCGI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANCGI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TAI:
            {
                S1ap_TAI_t *s1apTAI_p = &s1ap_PathSwitchRequestIEs->tai;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TAI, (void **)&s1apTAI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTAI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_UESecurityCapabilities:
            {
                S1ap_UESecurityCapabilities_t *s1apUESecurityCapabilities_p = &s1ap_PathSwitchRequestIEs->ueSecurityCapabilities;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UESecurityCapabilities, (void **)&s1apUESecurityCapabilities_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUESecurityCapabilities failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSG_Id:
            {
                S1ap_CSG_Id_t *s1apCSGId_p = &s1ap_PathSwitchRequestIEs->csG_Id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSG_Id, (void **)&s1apCSGId_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGId failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
                s1ap_PathSwitchRequestIEs->presenceMask |= S1AP_PATHSWITCHREQUESTIES_CSG_ID_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CellAccessMode:
            {
                S1ap_CellAccessMode_t *s1apCellAccessMode_p = &s1ap_PathSwitchRequestIEs->cellAccessMode;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CellAccessMode, (void **)&s1apCellAccessMode_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCellAccessMode failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
                s1ap_PathSwitchRequestIEs->presenceMask |= S1AP_PATHSWITCHREQUESTIES_CELLACCESSMODE_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SourceMME_GUMMEI:
            {
                S1ap_GUMMEI_t *s1apGUMMEI_p = &s1ap_PathSwitchRequestIEs->sourceMME_GUMMEI;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_GUMMEI, (void **)&s1apGUMMEI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apGUMMEI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                    return -1;
                }
                s1ap_PathSwitchRequestIEs->presenceMask |= S1AP_PATHSWITCHREQUESTIES_SOURCEMME_GUMMEI_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_pathswitchrequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PathSwitchRequest, s1ap_PathSwitchRequest_p);
    return 0;
}

int s1ap_decode_s1ap_downlinknastransport_ies(
    S1ap_DownlinkNASTransport_IEs_t *s1ap_DownlinkNASTransport_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_DownlinkNASTransport_t  s1ap_DownlinkNASTransport;
    S1ap_DownlinkNASTransport_t *s1ap_DownlinkNASTransport_p = &s1ap_DownlinkNASTransport;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_DownlinkNASTransport_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_DownlinkNASTransport_IEs\n");

    memset(&s1ap_DownlinkNASTransport, 0, sizeof(S1ap_DownlinkNASTransport_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_DownlinkNASTransport, (void **)&s1ap_DownlinkNASTransport_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_DownlinkNASTransport failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNASTransport, s1ap_DownlinkNASTransport_p);
        return -1;
    }

    for (i = 0; i < s1ap_DownlinkNASTransport_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_DownlinkNASTransport_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_DownlinkNASTransport_IEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNASTransport, s1ap_DownlinkNASTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_DownlinkNASTransport_IEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNASTransport, s1ap_DownlinkNASTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_NAS_PDU:
            {
                S1ap_NAS_PDU_t *s1apNASPDU_p = &s1ap_DownlinkNASTransport_IEs->nas_pdu;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_NAS_PDU, (void **)&s1apNASPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apNASPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNASTransport, s1ap_DownlinkNASTransport_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_HandoverRestrictionList:
            {
                S1ap_HandoverRestrictionList_t *s1apHandoverRestrictionList_p = &s1ap_DownlinkNASTransport_IEs->handoverRestrictionList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_HandoverRestrictionList, (void **)&s1apHandoverRestrictionList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apHandoverRestrictionList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNASTransport, s1ap_DownlinkNASTransport_p);
                    return -1;
                }
                s1ap_DownlinkNASTransport_IEs->presenceMask |= S1AP_DOWNLINKNASTRANSPORT_IES_HANDOVERRESTRICTIONLIST_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_SubscriberProfileIDforRFP:
            {
                S1ap_SubscriberProfileIDforRFP_t *s1apSubscriberProfileIDforRFP_p = &s1ap_DownlinkNASTransport_IEs->subscriberProfileIDforRFP;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SubscriberProfileIDforRFP, (void **)&s1apSubscriberProfileIDforRFP_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSubscriberProfileIDforRFP failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNASTransport, s1ap_DownlinkNASTransport_p);
                    return -1;
                }
                s1ap_DownlinkNASTransport_IEs->presenceMask |= S1AP_DOWNLINKNASTRANSPORT_IES_SUBSCRIBERPROFILEIDFORRFP_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_downlinknastransport_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNASTransport, s1ap_DownlinkNASTransport_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DownlinkNASTransport, s1ap_DownlinkNASTransport_p);
    return 0;
}

int s1ap_decode_s1ap_e_rabreleaseindicationies(
    S1ap_E_RABReleaseIndicationIEs_t *s1ap_E_RABReleaseIndicationIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_E_RABReleaseIndication_t  s1ap_E_RABReleaseIndication;
    S1ap_E_RABReleaseIndication_t *s1ap_E_RABReleaseIndication_p = &s1ap_E_RABReleaseIndication;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_E_RABReleaseIndicationIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_E_RABReleaseIndicationIEs\n");

    memset(&s1ap_E_RABReleaseIndication, 0, sizeof(S1ap_E_RABReleaseIndication_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_RABReleaseIndication, (void **)&s1ap_E_RABReleaseIndication_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_E_RABReleaseIndication failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseIndication, s1ap_E_RABReleaseIndication_p);
        return -1;
    }

    for (i = 0; i < s1ap_E_RABReleaseIndication_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_E_RABReleaseIndication_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_E_RABReleaseIndicationIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseIndication, s1ap_E_RABReleaseIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_E_RABReleaseIndicationIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseIndication, s1ap_E_RABReleaseIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABReleasedList:
            {
                S1ap_E_RABList_t *s1apERABList_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABList, (void**)&s1apERABList_p);
                if (s1ap_decode_s1ap_e_rablist(&s1ap_E_RABReleaseIndicationIEs->e_RABReleasedList, s1apERABList_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABList failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseIndication, s1ap_E_RABReleaseIndication_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABList, s1apERABList_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_e_rabreleaseindicationies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseIndication, s1ap_E_RABReleaseIndication_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABReleaseIndication, s1ap_E_RABReleaseIndication_p);
    return 0;
}

int s1ap_decode_s1ap_s1setuprequesties(
    S1ap_S1SetupRequestIEs_t *s1ap_S1SetupRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_S1SetupRequest_t  s1ap_S1SetupRequest;
    S1ap_S1SetupRequest_t *s1ap_S1SetupRequest_p = &s1ap_S1SetupRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_S1SetupRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_S1SetupRequestIEs\n");

    memset(&s1ap_S1SetupRequest, 0, sizeof(S1ap_S1SetupRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_S1SetupRequest, (void **)&s1ap_S1SetupRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_S1SetupRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupRequest, s1ap_S1SetupRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_S1SetupRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_S1SetupRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Global_ENB_ID:
            {
                S1ap_Global_ENB_ID_t *s1apGlobalENBID_p = &s1ap_S1SetupRequestIEs->global_ENB_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Global_ENB_ID, (void **)&s1apGlobalENBID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apGlobalENBID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupRequest, s1ap_S1SetupRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_eNBname:
            {
                S1ap_ENBname_t *s1apENBname_p = &s1ap_S1SetupRequestIEs->eNBname;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENBname, (void **)&s1apENBname_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBname failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupRequest, s1ap_S1SetupRequest_p);
                    return -1;
                }
                s1ap_S1SetupRequestIEs->presenceMask |= S1AP_S1SETUPREQUESTIES_ENBNAME_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_SupportedTAs:
            {
                S1ap_SupportedTAs_t *s1apSupportedTAs_p = &s1ap_S1SetupRequestIEs->supportedTAs;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_SupportedTAs, (void **)&s1apSupportedTAs_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apSupportedTAs failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupRequest, s1ap_S1SetupRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_DefaultPagingDRX:
            {
                S1ap_PagingDRX_t *s1apPagingDRX_p = &s1ap_S1SetupRequestIEs->defaultPagingDRX;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PagingDRX, (void **)&s1apPagingDRX_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apPagingDRX failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupRequest, s1ap_S1SetupRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CSG_IdList:
            {
                S1ap_CSG_IdList_t *s1apCSGIdList_p = &s1ap_S1SetupRequestIEs->csG_IdList;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CSG_IdList, (void **)&s1apCSGIdList_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCSGIdList failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupRequest, s1ap_S1SetupRequest_p);
                    return -1;
                }
                s1ap_S1SetupRequestIEs->presenceMask |= S1AP_S1SETUPREQUESTIES_CSG_IDLIST_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_s1setuprequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupRequest, s1ap_S1SetupRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S1SetupRequest, s1ap_S1SetupRequest_p);
    return 0;
}

int s1ap_decode_s1ap_nasnondeliveryindication_ies(
    S1ap_NASNonDeliveryIndication_IEs_t *s1ap_NASNonDeliveryIndication_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_NASNonDeliveryIndication_t  s1ap_NASNonDeliveryIndication;
    S1ap_NASNonDeliveryIndication_t *s1ap_NASNonDeliveryIndication_p = &s1ap_NASNonDeliveryIndication;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_NASNonDeliveryIndication_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_NASNonDeliveryIndication_IEs\n");

    memset(&s1ap_NASNonDeliveryIndication, 0, sizeof(S1ap_NASNonDeliveryIndication_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_NASNonDeliveryIndication, (void **)&s1ap_NASNonDeliveryIndication_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_NASNonDeliveryIndication failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NASNonDeliveryIndication, s1ap_NASNonDeliveryIndication_p);
        return -1;
    }

    for (i = 0; i < s1ap_NASNonDeliveryIndication_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_NASNonDeliveryIndication_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_NASNonDeliveryIndication_IEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NASNonDeliveryIndication, s1ap_NASNonDeliveryIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_NASNonDeliveryIndication_IEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NASNonDeliveryIndication, s1ap_NASNonDeliveryIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_NAS_PDU:
            {
                S1ap_NAS_PDU_t *s1apNASPDU_p = &s1ap_NASNonDeliveryIndication_IEs->nas_pdu;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_NAS_PDU, (void **)&s1apNASPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apNASPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NASNonDeliveryIndication, s1ap_NASNonDeliveryIndication_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Cause:
            {
                S1ap_Cause_t *s1apCause_p = &s1ap_NASNonDeliveryIndication_IEs->cause;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Cause, (void **)&s1apCause_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCause failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NASNonDeliveryIndication, s1ap_NASNonDeliveryIndication_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_nasnondeliveryindication_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NASNonDeliveryIndication, s1ap_NASNonDeliveryIndication_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NASNonDeliveryIndication, s1ap_NASNonDeliveryIndication_p);
    return 0;
}

int s1ap_decode_s1ap_celltraffictraceies(
    S1ap_CellTrafficTraceIEs_t *s1ap_CellTrafficTraceIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_CellTrafficTrace_t  s1ap_CellTrafficTrace;
    S1ap_CellTrafficTrace_t *s1ap_CellTrafficTrace_p = &s1ap_CellTrafficTrace;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_CellTrafficTraceIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_CellTrafficTraceIEs\n");

    memset(&s1ap_CellTrafficTrace, 0, sizeof(S1ap_CellTrafficTrace_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CellTrafficTrace, (void **)&s1ap_CellTrafficTrace_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_CellTrafficTrace failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
        return -1;
    }

    for (i = 0; i < s1ap_CellTrafficTrace_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_CellTrafficTrace_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_CellTrafficTraceIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_CellTrafficTraceIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_E_UTRAN_Trace_ID:
            {
                S1ap_E_UTRAN_Trace_ID_t *s1apEUTRANTraceID_p = &s1ap_CellTrafficTraceIEs->e_UTRAN_Trace_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_UTRAN_Trace_ID, (void **)&s1apEUTRANTraceID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANTraceID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_EUTRAN_CGI:
            {
                S1ap_EUTRAN_CGI_t *s1apEUTRANCGI_p = &s1ap_CellTrafficTraceIEs->eutran_cgi;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_EUTRAN_CGI, (void **)&s1apEUTRANCGI_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apEUTRANCGI failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_TraceCollectionEntityIPAddress:
            {
                S1ap_TransportLayerAddress_t *s1apTransportLayerAddress_p = &s1ap_CellTrafficTraceIEs->traceCollectionEntityIPAddress;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_TransportLayerAddress, (void **)&s1apTransportLayerAddress_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apTransportLayerAddress failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_PrivacyIndicator:
            {
                S1ap_PrivacyIndicator_t *s1apPrivacyIndicator_p = &s1ap_CellTrafficTraceIEs->privacyIndicator;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_PrivacyIndicator, (void **)&s1apPrivacyIndicator_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apPrivacyIndicator failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
                    return -1;
                }
                s1ap_CellTrafficTraceIEs->presenceMask |= S1AP_CELLTRAFFICTRACEIES_PRIVACYINDICATOR_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_celltraffictraceies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellTrafficTrace, s1ap_CellTrafficTrace_p);
    return 0;
}

int s1ap_decode_s1ap_e_rabsetuprequesties(
    S1ap_E_RABSetupRequestIEs_t *s1ap_E_RABSetupRequestIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_E_RABSetupRequest_t  s1ap_E_RABSetupRequest;
    S1ap_E_RABSetupRequest_t *s1ap_E_RABSetupRequest_p = &s1ap_E_RABSetupRequest;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_E_RABSetupRequestIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_E_RABSetupRequestIEs\n");

    memset(&s1ap_E_RABSetupRequest, 0, sizeof(S1ap_E_RABSetupRequest_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_E_RABSetupRequest, (void **)&s1ap_E_RABSetupRequest_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_E_RABSetupRequest failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupRequest, s1ap_E_RABSetupRequest_p);
        return -1;
    }

    for (i = 0; i < s1ap_E_RABSetupRequest_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_E_RABSetupRequest_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_E_RABSetupRequestIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupRequest, s1ap_E_RABSetupRequest_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_E_RABSetupRequestIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupRequest, s1ap_E_RABSetupRequest_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate:
            {
                S1ap_UEAggregateMaximumBitrate_t *s1apUEAggregateMaximumBitrate_p = &s1ap_E_RABSetupRequestIEs->uEaggregateMaximumBitrate;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEAggregateMaximumBitrate, (void **)&s1apUEAggregateMaximumBitrate_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apUEAggregateMaximumBitrate failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupRequest, s1ap_E_RABSetupRequest_p);
                    return -1;
                }
                s1ap_E_RABSetupRequestIEs->presenceMask |= S1AP_E_RABSETUPREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT;
            } break;
            case S1ap_ProtocolIE_ID_id_E_RABToBeSetupListBearerSUReq:
            {
                S1ap_E_RABToBeSetupListBearerSUReq_t *s1apERABToBeSetupListBearerSUReq_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSetupListBearerSUReq, (void**)&s1apERABToBeSetupListBearerSUReq_p);
                if (s1ap_decode_s1ap_e_rabtobesetuplistbearersureq(&s1ap_E_RABSetupRequestIEs->e_RABToBeSetupListBearerSUReq, s1apERABToBeSetupListBearerSUReq_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apERABToBeSetupListBearerSUReq failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupListBearerSUReq, s1apERABToBeSetupListBearerSUReq_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupRequest, s1ap_E_RABSetupRequest_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupListBearerSUReq, s1apERABToBeSetupListBearerSUReq_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_e_rabsetuprequesties", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupRequest, s1ap_E_RABSetupRequest_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupRequest, s1ap_E_RABSetupRequest_p);
    return 0;
}

int s1ap_decode_s1ap_enbstatustransferies(
    S1ap_ENBStatusTransferIEs_t *s1ap_ENBStatusTransferIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_ENBStatusTransfer_t  s1ap_ENBStatusTransfer;
    S1ap_ENBStatusTransfer_t *s1ap_ENBStatusTransfer_p = &s1ap_ENBStatusTransfer;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ENBStatusTransferIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ENBStatusTransferIEs\n");

    memset(&s1ap_ENBStatusTransfer, 0, sizeof(S1ap_ENBStatusTransfer_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENBStatusTransfer, (void **)&s1ap_ENBStatusTransfer_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_ENBStatusTransfer failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBStatusTransfer, s1ap_ENBStatusTransfer_p);
        return -1;
    }

    for (i = 0; i < s1ap_ENBStatusTransfer_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_ENBStatusTransfer_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_ENBStatusTransferIEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBStatusTransfer, s1ap_ENBStatusTransfer_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_ENBStatusTransferIEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBStatusTransfer, s1ap_ENBStatusTransfer_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer:
            {
                S1ap_ENB_StatusTransfer_TransparentContainer_t *s1apENBStatusTransferTransparentContainer_p = &s1ap_ENBStatusTransferIEs->eNB_StatusTransfer_TransparentContainer;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_StatusTransfer_TransparentContainer, (void **)&s1apENBStatusTransferTransparentContainer_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBStatusTransferTransparentContainer failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBStatusTransfer, s1ap_ENBStatusTransfer_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_enbstatustransferies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBStatusTransfer, s1ap_ENBStatusTransfer_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBStatusTransfer, s1ap_ENBStatusTransfer_p);
    return 0;
}

int s1ap_decode_s1ap_resetacknowledgeies(
    S1ap_ResetAcknowledgeIEs_t *s1ap_ResetAcknowledgeIEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_ResetAcknowledge_t  s1ap_ResetAcknowledge;
    S1ap_ResetAcknowledge_t *s1ap_ResetAcknowledge_p = &s1ap_ResetAcknowledge;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_ResetAcknowledgeIEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_ResetAcknowledgeIEs\n");

    memset(&s1ap_ResetAcknowledge, 0, sizeof(S1ap_ResetAcknowledge_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ResetAcknowledge, (void **)&s1ap_ResetAcknowledge_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_ResetAcknowledge failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ResetAcknowledge, s1ap_ResetAcknowledge_p);
        return -1;
    }

    for (i = 0; i < s1ap_ResetAcknowledge_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_ResetAcknowledge_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionListResAck:
            {
                S1ap_UE_associatedLogicalS1_ConnectionListResAck_t *s1apUEassociatedLogicalS1ConnectionListResAck_p = NULL;

                ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionListResAck, (void**)&s1apUEassociatedLogicalS1ConnectionListResAck_p);
                if (s1ap_decode_s1ap_ue_associatedlogicals1_connectionlistresack(&s1ap_ResetAcknowledgeIEs->uE_associatedLogicalS1_ConnectionListResAck, s1apUEassociatedLogicalS1ConnectionListResAck_p) < 0)
                {
                    d_error("Decoding of encapsulated IE s1apUEassociatedLogicalS1ConnectionListResAck failed");
                    ASN_STRUCT_FREE(asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionListResAck, s1apUEassociatedLogicalS1ConnectionListResAck_p);
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ResetAcknowledge, s1ap_ResetAcknowledge_p);
                    return -1;
                }
                ASN_STRUCT_FREE(asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionListResAck, s1apUEassociatedLogicalS1ConnectionListResAck_p);
                s1ap_ResetAcknowledgeIEs->presenceMask |= S1AP_RESETACKNOWLEDGEIES_UE_ASSOCIATEDLOGICALS1_CONNECTIONLISTRESACK_PRESENT;
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_ResetAcknowledgeIEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ResetAcknowledge, s1ap_ResetAcknowledge_p);
                    return -1;
                }
                s1ap_ResetAcknowledgeIEs->presenceMask |= S1AP_RESETACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_resetacknowledgeies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ResetAcknowledge, s1ap_ResetAcknowledge_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ResetAcknowledge, s1ap_ResetAcknowledge_p);
    return 0;
}

int s1ap_decode_s1ap_uecontextreleasecomplete_ies(
    S1ap_UEContextReleaseComplete_IEs_t *s1ap_UEContextReleaseComplete_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UEContextReleaseComplete_t  s1ap_UEContextReleaseComplete;
    S1ap_UEContextReleaseComplete_t *s1ap_UEContextReleaseComplete_p = &s1ap_UEContextReleaseComplete;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UEContextReleaseComplete_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UEContextReleaseComplete_IEs\n");

    memset(&s1ap_UEContextReleaseComplete, 0, sizeof(S1ap_UEContextReleaseComplete_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UEContextReleaseComplete, (void **)&s1ap_UEContextReleaseComplete_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UEContextReleaseComplete failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseComplete, s1ap_UEContextReleaseComplete_p);
        return -1;
    }

    for (i = 0; i < s1ap_UEContextReleaseComplete_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UEContextReleaseComplete_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UEContextReleaseComplete_IEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseComplete, s1ap_UEContextReleaseComplete_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UEContextReleaseComplete_IEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseComplete, s1ap_UEContextReleaseComplete_p);
                    return -1;
                }
            } break;
            /* Optional field */
            case S1ap_ProtocolIE_ID_id_CriticalityDiagnostics:
            {
                S1ap_CriticalityDiagnostics_t *s1apCriticalityDiagnostics_p = &s1ap_UEContextReleaseComplete_IEs->criticalityDiagnostics;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_CriticalityDiagnostics, (void **)&s1apCriticalityDiagnostics_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apCriticalityDiagnostics failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseComplete, s1ap_UEContextReleaseComplete_p);
                    return -1;
                }
                s1ap_UEContextReleaseComplete_IEs->presenceMask |= S1AP_UECONTEXTRELEASECOMPLETE_IES_CRITICALITYDIAGNOSTICS_PRESENT;
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uecontextreleasecomplete_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseComplete, s1ap_UEContextReleaseComplete_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEContextReleaseComplete, s1ap_UEContextReleaseComplete_p);
    return 0;
}

int s1ap_decode_s1ap_uplinkueassociatedlppatransport_ies(
    S1ap_UplinkUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkUEAssociatedLPPaTransport_IEs,
    ANY_t *any_p) {

    asn_dec_rval_t dec_ret = {0};
    S1ap_UplinkUEAssociatedLPPaTransport_t  s1ap_UplinkUEAssociatedLPPaTransport;
    S1ap_UplinkUEAssociatedLPPaTransport_t *s1ap_UplinkUEAssociatedLPPaTransport_p = &s1ap_UplinkUEAssociatedLPPaTransport;
    int i;
    d_assert(any_p, return -1, "Null param");
    d_assert(s1ap_UplinkUEAssociatedLPPaTransport_IEs, return -1, "Null param");

    d_trace(3, "[S1AP] Decode S1ap_UplinkUEAssociatedLPPaTransport_IEs\n");

    memset(&s1ap_UplinkUEAssociatedLPPaTransport, 0, sizeof(S1ap_UplinkUEAssociatedLPPaTransport_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1ap_UplinkUEAssociatedLPPaTransport, (void **)&s1ap_UplinkUEAssociatedLPPaTransport_p, any_p->buf, any_p->size, 0, 0);
    if (dec_ret.code != RC_OK)
    {
        d_error("Decoding of IE s1ap_UplinkUEAssociatedLPPaTransport failed");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkUEAssociatedLPPaTransport, s1ap_UplinkUEAssociatedLPPaTransport_p);
        return -1;
    }

    for (i = 0; i < s1ap_UplinkUEAssociatedLPPaTransport_p->protocolIEs.list.count; i++) {
        S1ap_IE_t *ie_p;
        ANY_t *st;

        ie_p = (S1ap_IE_t *)s1ap_UplinkUEAssociatedLPPaTransport_p->protocolIEs.list.array[i];
        d_assert(ie_p, return -1, "Null param");
        st = &ie_p->value;
        d_assert(st, return -1, "Null param");
        switch(ie_p->id) {
            case S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            {
                S1ap_MME_UE_S1AP_ID_t *s1apMMEUES1APID_p = &s1ap_UplinkUEAssociatedLPPaTransport_IEs->mme_ue_s1ap_id;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_MME_UE_S1AP_ID, (void **)&s1apMMEUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apMMEUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkUEAssociatedLPPaTransport, s1ap_UplinkUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            {
                S1ap_ENB_UE_S1AP_ID_t *s1apENBUES1APID_p = &s1ap_UplinkUEAssociatedLPPaTransport_IEs->eNB_UE_S1AP_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_ENB_UE_S1AP_ID, (void **)&s1apENBUES1APID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apENBUES1APID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkUEAssociatedLPPaTransport, s1ap_UplinkUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_Routing_ID:
            {
                S1ap_Routing_ID_t *s1apRoutingID_p = &s1ap_UplinkUEAssociatedLPPaTransport_IEs->routing_ID;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_Routing_ID, (void **)&s1apRoutingID_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apRoutingID failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkUEAssociatedLPPaTransport, s1ap_UplinkUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            case S1ap_ProtocolIE_ID_id_LPPa_PDU:
            {
                S1ap_LPPa_PDU_t *s1apLPPaPDU_p = &s1ap_UplinkUEAssociatedLPPaTransport_IEs->lpPa_PDU;

                dec_ret = aper_decode(NULL, &asn_DEF_S1ap_LPPa_PDU, (void **)&s1apLPPaPDU_p, st->buf, st->size, 0, 0);
                if (dec_ret.code != RC_OK)
                {
                    d_error("Decoding of IE s1apLPPaPDU failed");
                    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkUEAssociatedLPPaTransport, s1ap_UplinkUEAssociatedLPPaTransport_p);
                    return -1;
                }
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkUEAssociatedLPPaTransport, s1ap_UplinkUEAssociatedLPPaTransport_p);
                return -1;
        }
    }

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UplinkUEAssociatedLPPaTransport, s1ap_UplinkUEAssociatedLPPaTransport_p);
    return 0;
}

int s1ap_decode_s1ap_e_rabfailedtosetuplisthoreqack(
    S1ap_E_RABFailedtoSetupListHOReqAck_IEs_t *s1ap_E_RABFailedtoSetupListHOReqAckIEs,
    S1ap_E_RABFailedtoSetupListHOReqAck_t *s1ap_E_RABFailedtoSetupListHOReqAck) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABFailedtoSetupListHOReqAck, return -1, "Null param");
    d_assert(s1ap_E_RABFailedtoSetupListHOReqAckIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABFailedtoSetupListHOReqAck->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABFailedtoSetupListHOReqAck->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABFailedtoSetupItemHOReqAck:
            {
                S1ap_E_RABFailedToSetupItemHOReqAck_t *s1apERABFailedToSetupItemHOReqAck_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABFailedToSetupItemHOReqAck, (void**)&s1apERABFailedToSetupItemHOReqAck_p);
                if (tempDecoded < 0 || s1apERABFailedToSetupItemHOReqAck_p == NULL) {
                    d_error("Decoding of IE e_RABFailedtoSetupItemHOReqAck for message S1ap_E_RABFailedtoSetupListHOReqAck failed");
                    if (s1apERABFailedToSetupItemHOReqAck_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABFailedToSetupItemHOReqAck, s1apERABFailedToSetupItemHOReqAck_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABFailedtoSetupListHOReqAckIEs->s1ap_E_RABFailedtoSetupItemHOReqAck, s1apERABFailedToSetupItemHOReqAck_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabreleaselistbearerrelcomp(
    S1ap_E_RABReleaseListBearerRelComp_IEs_t *s1ap_E_RABReleaseListBearerRelCompIEs,
    S1ap_E_RABReleaseListBearerRelComp_t *s1ap_E_RABReleaseListBearerRelComp) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABReleaseListBearerRelComp, return -1, "Null param");
    d_assert(s1ap_E_RABReleaseListBearerRelCompIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABReleaseListBearerRelComp->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABReleaseListBearerRelComp->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABReleaseItemBearerRelComp:
            {
                S1ap_E_RABReleaseItemBearerRelComp_t *s1apERABReleaseItemBearerRelComp_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABReleaseItemBearerRelComp, (void**)&s1apERABReleaseItemBearerRelComp_p);
                if (tempDecoded < 0 || s1apERABReleaseItemBearerRelComp_p == NULL) {
                    d_error("Decoding of IE e_RABReleaseItemBearerRelComp for message S1ap_E_RABReleaseListBearerRelComp failed");
                    if (s1apERABReleaseItemBearerRelComp_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABReleaseItemBearerRelComp, s1apERABReleaseItemBearerRelComp_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp, s1apERABReleaseItemBearerRelComp_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabtobesetuplisthoreq(
    S1ap_E_RABToBeSetupListHOReq_IEs_t *s1ap_E_RABToBeSetupListHOReqIEs,
    S1ap_E_RABToBeSetupListHOReq_t *s1ap_E_RABToBeSetupListHOReq) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABToBeSetupListHOReq, return -1, "Null param");
    d_assert(s1ap_E_RABToBeSetupListHOReqIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABToBeSetupListHOReq->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABToBeSetupListHOReq->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABToBeSetupItemHOReq:
            {
                S1ap_E_RABToBeSetupItemHOReq_t *s1apERABToBeSetupItemHOReq_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSetupItemHOReq, (void**)&s1apERABToBeSetupItemHOReq_p);
                if (tempDecoded < 0 || s1apERABToBeSetupItemHOReq_p == NULL) {
                    d_error("Decoding of IE e_RABToBeSetupItemHOReq for message S1ap_E_RABToBeSetupListHOReq failed");
                    if (s1apERABToBeSetupItemHOReq_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupItemHOReq, s1apERABToBeSetupItemHOReq_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSetupListHOReqIEs->s1ap_E_RABToBeSetupItemHOReq, s1apERABToBeSetupItemHOReq_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabtobemodifiedlistbearermodreq(
    S1ap_E_RABToBeModifiedListBearerModReq_IEs_t *s1ap_E_RABToBeModifiedListBearerModReqIEs,
    S1ap_E_RABToBeModifiedListBearerModReq_t *s1ap_E_RABToBeModifiedListBearerModReq) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABToBeModifiedListBearerModReq, return -1, "Null param");
    d_assert(s1ap_E_RABToBeModifiedListBearerModReqIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABToBeModifiedListBearerModReq->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABToBeModifiedListBearerModReq->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABToBeModifiedItemBearerModReq:
            {
                S1ap_E_RABToBeModifiedItemBearerModReq_t *s1apERABToBeModifiedItemBearerModReq_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeModifiedItemBearerModReq, (void**)&s1apERABToBeModifiedItemBearerModReq_p);
                if (tempDecoded < 0 || s1apERABToBeModifiedItemBearerModReq_p == NULL) {
                    d_error("Decoding of IE e_RABToBeModifiedItemBearerModReq for message S1ap_E_RABToBeModifiedListBearerModReq failed");
                    if (s1apERABToBeModifiedItemBearerModReq_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeModifiedItemBearerModReq, s1apERABToBeModifiedItemBearerModReq_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq, s1apERABToBeModifiedItemBearerModReq_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabadmittedlist(
    S1ap_E_RABAdmittedList_IEs_t *s1ap_E_RABAdmittedListIEs,
    S1ap_E_RABAdmittedList_t *s1ap_E_RABAdmittedList) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABAdmittedList, return -1, "Null param");
    d_assert(s1ap_E_RABAdmittedListIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABAdmittedList->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABAdmittedList->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABAdmittedItem:
            {
                S1ap_E_RABAdmittedItem_t *s1apERABAdmittedItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABAdmittedItem, (void**)&s1apERABAdmittedItem_p);
                if (tempDecoded < 0 || s1apERABAdmittedItem_p == NULL) {
                    d_error("Decoding of IE e_RABAdmittedItem for message S1ap_E_RABAdmittedList failed");
                    if (s1apERABAdmittedItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABAdmittedItem, s1apERABAdmittedItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABAdmittedListIEs->s1ap_E_RABAdmittedItem, s1apERABAdmittedItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabtobesetuplistbearersureq(
    S1ap_E_RABToBeSetupListBearerSUReq_IEs_t *s1ap_E_RABToBeSetupListBearerSUReqIEs,
    S1ap_E_RABToBeSetupListBearerSUReq_t *s1ap_E_RABToBeSetupListBearerSUReq) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABToBeSetupListBearerSUReq, return -1, "Null param");
    d_assert(s1ap_E_RABToBeSetupListBearerSUReqIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABToBeSetupListBearerSUReq->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABToBeSetupListBearerSUReq->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABToBeSetupItemBearerSUReq:
            {
                S1ap_E_RABToBeSetupItemBearerSUReq_t *s1apERABToBeSetupItemBearerSUReq_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSetupItemBearerSUReq, (void**)&s1apERABToBeSetupItemBearerSUReq_p);
                if (tempDecoded < 0 || s1apERABToBeSetupItemBearerSUReq_p == NULL) {
                    d_error("Decoding of IE e_RABToBeSetupItemBearerSUReq for message S1ap_E_RABToBeSetupListBearerSUReq failed");
                    if (s1apERABToBeSetupItemBearerSUReq_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupItemBearerSUReq, s1apERABToBeSetupItemBearerSUReq_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq, s1apERABToBeSetupItemBearerSUReq_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabdataforwardinglist(
    S1ap_E_RABDataForwardingList_IEs_t *s1ap_E_RABDataForwardingListIEs,
    S1ap_E_RABDataForwardingList_t *s1ap_E_RABDataForwardingList) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABDataForwardingList, return -1, "Null param");
    d_assert(s1ap_E_RABDataForwardingListIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABDataForwardingList->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABDataForwardingList->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABDataForwardingItem:
            {
                S1ap_E_RABDataForwardingItem_t *s1apERABDataForwardingItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABDataForwardingItem, (void**)&s1apERABDataForwardingItem_p);
                if (tempDecoded < 0 || s1apERABDataForwardingItem_p == NULL) {
                    d_error("Decoding of IE e_RABDataForwardingItem for message S1ap_E_RABDataForwardingList failed");
                    if (s1apERABDataForwardingItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABDataForwardingItem, s1apERABDataForwardingItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABDataForwardingListIEs->s1ap_E_RABDataForwardingItem, s1apERABDataForwardingItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabtobeswitcheddllist(
    S1ap_E_RABToBeSwitchedDLList_IEs_t *s1ap_E_RABToBeSwitchedDLListIEs,
    S1ap_E_RABToBeSwitchedDLList_t *s1ap_E_RABToBeSwitchedDLList) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABToBeSwitchedDLList, return -1, "Null param");
    d_assert(s1ap_E_RABToBeSwitchedDLListIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABToBeSwitchedDLList->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABToBeSwitchedDLList->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABToBeSwitchedDLItem:
            {
                S1ap_E_RABToBeSwitchedDLItem_t *s1apERABToBeSwitchedDLItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSwitchedDLItem, (void**)&s1apERABToBeSwitchedDLItem_p);
                if (tempDecoded < 0 || s1apERABToBeSwitchedDLItem_p == NULL) {
                    d_error("Decoding of IE e_RABToBeSwitchedDLItem for message S1ap_E_RABToBeSwitchedDLList failed");
                    if (s1apERABToBeSwitchedDLItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSwitchedDLItem, s1apERABToBeSwitchedDLItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSwitchedDLListIEs->s1ap_E_RABToBeSwitchedDLItem, s1apERABToBeSwitchedDLItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabsetuplistctxtsures(
    S1ap_E_RABSetupListCtxtSURes_IEs_t *s1ap_E_RABSetupListCtxtSUResIEs,
    S1ap_E_RABSetupListCtxtSURes_t *s1ap_E_RABSetupListCtxtSURes) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABSetupListCtxtSURes, return -1, "Null param");
    d_assert(s1ap_E_RABSetupListCtxtSUResIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABSetupListCtxtSURes->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABSetupListCtxtSURes->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABSetupItemCtxtSURes:
            {
                S1ap_E_RABSetupItemCtxtSURes_t *s1apERABSetupItemCtxtSURes_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABSetupItemCtxtSURes, (void**)&s1apERABSetupItemCtxtSURes_p);
                if (tempDecoded < 0 || s1apERABSetupItemCtxtSURes_p == NULL) {
                    d_error("Decoding of IE e_RABSetupItemCtxtSURes for message S1ap_E_RABSetupListCtxtSURes failed");
                    if (s1apERABSetupItemCtxtSURes_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABSetupItemCtxtSURes, s1apERABSetupItemCtxtSURes_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes, s1apERABSetupItemCtxtSURes_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabtobeswitchedullist(
    S1ap_E_RABToBeSwitchedULList_IEs_t *s1ap_E_RABToBeSwitchedULListIEs,
    S1ap_E_RABToBeSwitchedULList_t *s1ap_E_RABToBeSwitchedULList) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABToBeSwitchedULList, return -1, "Null param");
    d_assert(s1ap_E_RABToBeSwitchedULListIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABToBeSwitchedULList->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABToBeSwitchedULList->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABToBeSwitchedULItem:
            {
                S1ap_E_RABToBeSwitchedULItem_t *s1apERABToBeSwitchedULItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSwitchedULItem, (void**)&s1apERABToBeSwitchedULItem_p);
                if (tempDecoded < 0 || s1apERABToBeSwitchedULItem_p == NULL) {
                    d_error("Decoding of IE e_RABToBeSwitchedULItem for message S1ap_E_RABToBeSwitchedULList failed");
                    if (s1apERABToBeSwitchedULItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSwitchedULItem, s1apERABToBeSwitchedULItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSwitchedULListIEs->s1ap_E_RABToBeSwitchedULItem, s1apERABToBeSwitchedULItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_bearers_subjecttostatustransfer_list(
    S1ap_Bearers_SubjectToStatusTransfer_List_IEs_t *s1ap_Bearers_SubjectToStatusTransfer_ListIEs,
    S1ap_Bearers_SubjectToStatusTransfer_List_t *s1ap_Bearers_SubjectToStatusTransfer_List) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_Bearers_SubjectToStatusTransfer_List, return -1, "Null param");
    d_assert(s1ap_Bearers_SubjectToStatusTransfer_ListIEs, return -1, "Null param");

    for (i = 0; i < s1ap_Bearers_SubjectToStatusTransfer_List->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_Bearers_SubjectToStatusTransfer_List->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_Bearers_SubjectToStatusTransfer_Item:
            {
                S1ap_Bearers_SubjectToStatusTransfer_Item_t *s1apBearersSubjectToStatusTransferItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_Bearers_SubjectToStatusTransfer_Item, (void**)&s1apBearersSubjectToStatusTransferItem_p);
                if (tempDecoded < 0 || s1apBearersSubjectToStatusTransferItem_p == NULL) {
                    d_error("Decoding of IE bearers_SubjectToStatusTransfer_Item for message S1ap_Bearers_SubjectToStatusTransfer_List failed");
                    if (s1apBearersSubjectToStatusTransferItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_Bearers_SubjectToStatusTransfer_Item, s1apBearersSubjectToStatusTransferItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item, s1apBearersSubjectToStatusTransferItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rablist(
    S1ap_E_RABList_IEs_t *s1ap_E_RABListIEs,
    S1ap_E_RABList_t *s1ap_E_RABList) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABList, return -1, "Null param");
    d_assert(s1ap_E_RABListIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABList->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABList->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABItem:
            {
                S1ap_E_RABItem_t *s1apERABItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABItem, (void**)&s1apERABItem_p);
                if (tempDecoded < 0 || s1apERABItem_p == NULL) {
                    d_error("Decoding of IE e_RABItem for message S1ap_E_RABList failed");
                    if (s1apERABItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABItem, s1apERABItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABListIEs->s1ap_E_RABItem, s1apERABItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_tailist(
    S1ap_TAIList_IEs_t *s1ap_TAIListIEs,
    S1ap_TAIList_t *s1ap_TAIList) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_TAIList, return -1, "Null param");
    d_assert(s1ap_TAIListIEs, return -1, "Null param");

    for (i = 0; i < s1ap_TAIList->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_TAIList->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_TAIItem:
            {
                S1ap_TAIItem_t *s1apTAIItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_TAIItem, (void**)&s1apTAIItem_p);
                if (tempDecoded < 0 || s1apTAIItem_p == NULL) {
                    d_error("Decoding of IE taiItem for message S1ap_TAIList failed");
                    if (s1apTAIItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_TAIItem, s1apTAIItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_TAIListIEs->s1ap_TAIItem, s1apTAIItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_ue_associatedlogicals1_connectionlistres(
    S1ap_UE_associatedLogicalS1_ConnectionListRes_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResIEs,
    S1ap_UE_associatedLogicalS1_ConnectionListRes_t *s1ap_UE_associatedLogicalS1_ConnectionListRes) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_UE_associatedLogicalS1_ConnectionListRes, return -1, "Null param");
    d_assert(s1ap_UE_associatedLogicalS1_ConnectionListResIEs, return -1, "Null param");

    for (i = 0; i < s1ap_UE_associatedLogicalS1_ConnectionListRes->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_UE_associatedLogicalS1_ConnectionListRes->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem:
            {
                S1ap_UE_associatedLogicalS1_ConnectionItem_t *s1apUEassociatedLogicalS1ConnectionItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem, (void**)&s1apUEassociatedLogicalS1ConnectionItem_p);
                if (tempDecoded < 0 || s1apUEassociatedLogicalS1ConnectionItem_p == NULL) {
                    d_error("Decoding of IE uE_associatedLogicalS1_ConnectionItem for message S1ap_UE_associatedLogicalS1_ConnectionListRes failed");
                    if (s1apUEassociatedLogicalS1ConnectionItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem, s1apUEassociatedLogicalS1ConnectionItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_UE_associatedLogicalS1_ConnectionListResIEs->s1ap_UE_associatedLogicalS1_ConnectionItemRes, s1apUEassociatedLogicalS1ConnectionItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabsetuplistbearersures(
    S1ap_E_RABSetupListBearerSURes_IEs_t *s1ap_E_RABSetupListBearerSUResIEs,
    S1ap_E_RABSetupListBearerSURes_t *s1ap_E_RABSetupListBearerSURes) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABSetupListBearerSURes, return -1, "Null param");
    d_assert(s1ap_E_RABSetupListBearerSUResIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABSetupListBearerSURes->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABSetupListBearerSURes->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABSetupItemBearerSURes:
            {
                S1ap_E_RABSetupItemBearerSURes_t *s1apERABSetupItemBearerSURes_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABSetupItemBearerSURes, (void**)&s1apERABSetupItemBearerSURes_p);
                if (tempDecoded < 0 || s1apERABSetupItemBearerSURes_p == NULL) {
                    d_error("Decoding of IE e_RABSetupItemBearerSURes for message S1ap_E_RABSetupListBearerSURes failed");
                    if (s1apERABSetupItemBearerSURes_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABSetupItemBearerSURes, s1apERABSetupItemBearerSURes_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes, s1apERABSetupItemBearerSURes_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabmodifylistbearermodres(
    S1ap_E_RABModifyListBearerModRes_IEs_t *s1ap_E_RABModifyListBearerModResIEs,
    S1ap_E_RABModifyListBearerModRes_t *s1ap_E_RABModifyListBearerModRes) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABModifyListBearerModRes, return -1, "Null param");
    d_assert(s1ap_E_RABModifyListBearerModResIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABModifyListBearerModRes->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABModifyListBearerModRes->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABModifyItemBearerModRes:
            {
                S1ap_E_RABModifyItemBearerModRes_t *s1apERABModifyItemBearerModRes_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABModifyItemBearerModRes, (void**)&s1apERABModifyItemBearerModRes_p);
                if (tempDecoded < 0 || s1apERABModifyItemBearerModRes_p == NULL) {
                    d_error("Decoding of IE e_RABModifyItemBearerModRes for message S1ap_E_RABModifyListBearerModRes failed");
                    if (s1apERABModifyItemBearerModRes_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABModifyItemBearerModRes, s1apERABModifyItemBearerModRes_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes, s1apERABModifyItemBearerModRes_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabtobesetuplistctxtsureq(
    S1ap_E_RABToBeSetupListCtxtSUReq_IEs_t *s1ap_E_RABToBeSetupListCtxtSUReqIEs,
    S1ap_E_RABToBeSetupListCtxtSUReq_t *s1ap_E_RABToBeSetupListCtxtSUReq) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABToBeSetupListCtxtSUReq, return -1, "Null param");
    d_assert(s1ap_E_RABToBeSetupListCtxtSUReqIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABToBeSetupListCtxtSUReq->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABToBeSetupListCtxtSUReq->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABToBeSetupItemCtxtSUReq:
            {
                S1ap_E_RABToBeSetupItemCtxtSUReq_t *s1apERABToBeSetupItemCtxtSUReq_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABToBeSetupItemCtxtSUReq, (void**)&s1apERABToBeSetupItemCtxtSUReq_p);
                if (tempDecoded < 0 || s1apERABToBeSetupItemCtxtSUReq_p == NULL) {
                    d_error("Decoding of IE e_RABToBeSetupItemCtxtSUReq for message S1ap_E_RABToBeSetupListCtxtSUReq failed");
                    if (s1apERABToBeSetupItemCtxtSUReq_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupItemCtxtSUReq, s1apERABToBeSetupItemCtxtSUReq_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq, s1apERABToBeSetupItemCtxtSUReq_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_e_rabinformationlist(
    S1ap_E_RABInformationList_IEs_t *s1ap_E_RABInformationListIEs,
    S1ap_E_RABInformationList_t *s1ap_E_RABInformationList) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_E_RABInformationList, return -1, "Null param");
    d_assert(s1ap_E_RABInformationListIEs, return -1, "Null param");

    for (i = 0; i < s1ap_E_RABInformationList->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_E_RABInformationList->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_E_RABInformationListItem:
            {
                S1ap_E_RABInformationListItem_t *s1apERABInformationListItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_E_RABInformationListItem, (void**)&s1apERABInformationListItem_p);
                if (tempDecoded < 0 || s1apERABInformationListItem_p == NULL) {
                    d_error("Decoding of IE e_RABInformationListItem for message S1ap_E_RABInformationList failed");
                    if (s1apERABInformationListItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABInformationListItem, s1apERABInformationListItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList, s1apERABInformationListItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

int s1ap_decode_s1ap_ue_associatedlogicals1_connectionlistresack(
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs,
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_t *s1ap_UE_associatedLogicalS1_ConnectionListResAck) {

    int i, decoded = 0;
    int tempDecoded = 0;

    d_assert(s1ap_UE_associatedLogicalS1_ConnectionListResAck, return -1, "Null param");
    d_assert(s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs, return -1, "Null param");

    for (i = 0; i < s1ap_UE_associatedLogicalS1_ConnectionListResAck->list.count; i++) {
        S1ap_IE_t *ie_p = (S1ap_IE_t *)s1ap_UE_associatedLogicalS1_ConnectionListResAck->list.array[i];
        switch (ie_p->id) {
            case S1ap_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem:
            {
                S1ap_UE_associatedLogicalS1_ConnectionItem_t *s1apUEassociatedLogicalS1ConnectionItem_p = NULL;
                tempDecoded = ANY_to_type_aper(&ie_p->value, &asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem, (void**)&s1apUEassociatedLogicalS1ConnectionItem_p);
                if (tempDecoded < 0 || s1apUEassociatedLogicalS1ConnectionItem_p == NULL) {
                    d_error("Decoding of IE uE_associatedLogicalS1_ConnectionItem for message S1ap_UE_associatedLogicalS1_ConnectionListResAck failed");
                    if (s1apUEassociatedLogicalS1ConnectionItem_p)
                        ASN_STRUCT_FREE(asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem, s1apUEassociatedLogicalS1ConnectionItem_p);
                    return -1;
                }
                decoded += tempDecoded;
                ASN_SEQUENCE_ADD(&s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs->s1ap_UE_associatedLogicalS1_ConnectionItemResAck, s1apUEassociatedLogicalS1ConnectionItem_p);
            } break;
            default:
                d_error("Unknown protocol IE id (%d) for message s1ap_uplinkueassociatedlppatransport_ies", (int)ie_p->id);
                return -1;
        }
    }
    return decoded;
}

