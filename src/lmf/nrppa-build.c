/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "nrppa-build.h"

#include "asn1c/util/conv.h"
#include "asn1c/util/message.h"
#include "E-CIDMeasurementInitiationRequest.h"
#include "Criticality.h"
#include "ProcedureCode.h"
#include "NRPPATransactionID.h"
#include "ProtocolIE-Container.h"
#include "ProtocolIE-Field.h"
#include "ProtocolIE-ID.h"
#include "UE-Measurement-ID.h"
#include "ReportCharacteristics.h"
#include "MeasurementQuantities.h"
#include "MeasurementQuantities-Item.h"
#include "ProtocolIE-Single-Container.h"
#include "MeasurementQuantitiesValue.h"
#include "asn_constant.h"
#include "asn1c/util/conv.h"
#include "asn1c/util/message.h"
#include "ogs-core.h"
#include <asn_SEQUENCE_OF.h>

/* No longer needed - v17.2.0/v18 uses MeasurementQuantities instead */

ogs_pkbuf_t *lmf_nrppa_build_ecid_measurement_request(
        uint32_t measurement_id,
        uint64_t ue_ngap_id,
        uint8_t requested_measurements)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pkbuf_t *request_pkbuf = NULL;
    E_CIDMeasurementInitiationRequest_t request;
    E_CIDMeasurementInitiationRequest_IEs_t *ie = NULL;
    MeasurementQuantities_ItemIEs_t *quantity_ie = NULL;
    static uint16_t transaction_id_counter = 1;
    uint16_t transaction_id;
    int rv;
    uint8_t *p;

    if (!requested_measurements) {
        ogs_warn("Requested measurements bitmap is zero, forcing CELL_ID|SS_RSRP");
        requested_measurements =
            NRPPA_ECID_MEASUREMENT_TYPE_RSRP;
    }

    ogs_info("Building NRPPa ECID request (v17.2.0 ProtocolIE format): "
             "measurement_id=%u, requested=0x%02x",
             measurement_id, requested_measurements);
    
    /* Log requested measurement types */
    if (requested_measurements & NRPPA_ECID_MEASUREMENT_TYPE_RSRP) {
        ogs_debug("  Requesting: SS-RSRP");
    }
    if (requested_measurements & NRPPA_ECID_MEASUREMENT_TYPE_TA) {
        ogs_debug("  Requesting: NR Timing Advance");
    }
    if (requested_measurements & NRPPA_ECID_MEASUREMENT_TYPE_AOA) {
        ogs_info("  Requesting: Angle of Arrival (requires UE SRS and gNB antenna array)");
        ogs_info("    Note: AoA may not be returned if UE doesn't support SRS");
        ogs_info("    Note: AoA requires specific gNB antenna configuration (array)");
        ogs_info("    Note: Per TS 38.455 §9.2.38, Azimuth+Zenith always reported when AoA present");
    }

    transaction_id = transaction_id_counter++;
    if (transaction_id_counter == 0)
        transaction_id_counter = 1;

    memset(&request, 0, sizeof(request));

    /* IE 1: id-LMF-UE-Measurement-ID (MANDATORY) */
    ie = CALLOC(1, sizeof(*ie));
    if (!ie) {
        ogs_error("CALLOC failed for ProtocolIE-Field");
        goto err;
    }
    ie->id = ProtocolIE_ID_id_LMF_UE_Measurement_ID;
    ie->criticality = Criticality_reject;
    ie->value.present = E_CIDMeasurementInitiationRequest_IEs__value_PR_UE_Measurement_ID;
    ie->value.choice.UE_Measurement_ID = measurement_id;
    rv = ASN_SEQUENCE_ADD(&request.protocolIEs.list, ie);
    if (rv != 0) {
        ogs_error("ASN_SEQUENCE_ADD failed for UE-Measurement-ID, rv=%d", rv);
        ogs_free(ie);
        goto err;
    }
    ie = NULL;

    /* IE 2: id-ReportCharacteristics (MANDATORY) */
    ie = CALLOC(1, sizeof(*ie));
    if (!ie) {
        ogs_error("CALLOC failed for ProtocolIE-Field");
        goto err;
    }
    ie->id = ProtocolIE_ID_id_ReportCharacteristics;
    ie->criticality = Criticality_reject;
    ie->value.present = E_CIDMeasurementInitiationRequest_IEs__value_PR_ReportCharacteristics;
    ie->value.choice.ReportCharacteristics = ReportCharacteristics_onDemand;
    rv = ASN_SEQUENCE_ADD(&request.protocolIEs.list, ie);
    if (rv != 0) {
        ogs_error("ASN_SEQUENCE_ADD failed for ReportCharacteristics, rv=%d", rv);
        ogs_free(ie);
        goto err;
    }
    ie = NULL;

    /* IE 3: id-MeasurementQuantities (MANDATORY) */
    ie = CALLOC(1, sizeof(*ie));
    if (!ie) {
        ogs_error("CALLOC failed for ProtocolIE-Field");
        goto err;
    }
    ie->id = ProtocolIE_ID_id_MeasurementQuantities;
    ie->criticality = Criticality_reject;
    ie->value.present = E_CIDMeasurementInitiationRequest_IEs__value_PR_MeasurementQuantities;

    /* Always include Cell-ID (Ericsson requirement) */
    quantity_ie = CALLOC(1, sizeof(*quantity_ie));
    if (!quantity_ie) {
        ogs_error("CALLOC failed for MeasurementQuantities_ItemIEs");
        ogs_free(ie);
        goto err;
    }
    quantity_ie->id = ProtocolIE_ID_id_MeasurementQuantities_Item;
    quantity_ie->criticality = Criticality_reject;
    quantity_ie->value.present = MeasurementQuantities_ItemIEs__value_PR_MeasurementQuantities_Item;
    quantity_ie->value.choice.MeasurementQuantities_Item.measurementQuantitiesValue = MeasurementQuantitiesValue_cell_ID;
    rv = ASN_SEQUENCE_ADD(&ie->value.choice.MeasurementQuantities.list, quantity_ie);
    if (rv != 0) {
        ogs_error("ASN_SEQUENCE_ADD failed for cell_ID, rv=%d", rv);
        ogs_free(quantity_ie);
        ogs_free(ie);
        goto err;
    }
    quantity_ie = NULL;

    /* Add SS-RSRP if requested (Ericsson supports) */
    if (requested_measurements & NRPPA_ECID_MEASUREMENT_TYPE_RSRP) {
        quantity_ie = CALLOC(1, sizeof(*quantity_ie));
        if (!quantity_ie) {
            ogs_error("CALLOC failed for MeasurementQuantities_ItemIEs (SS-RSRP)");
            goto err;
        }
        quantity_ie->id = ProtocolIE_ID_id_MeasurementQuantities_Item;
        quantity_ie->criticality = Criticality_reject;
        quantity_ie->value.present = MeasurementQuantities_ItemIEs__value_PR_MeasurementQuantities_Item;
        quantity_ie->value.choice.MeasurementQuantities_Item.measurementQuantitiesValue = MeasurementQuantitiesValue_sS_RSRP;
        rv = ASN_SEQUENCE_ADD(&ie->value.choice.MeasurementQuantities.list, quantity_ie);
        if (rv != 0) {
            ogs_error("ASN_SEQUENCE_ADD failed for sS_RSRP, rv=%d", rv);
            ogs_free(quantity_ie);
            goto err;
        }
        quantity_ie = NULL;
    }

    /* Add NR Timing Advance if requested (Ericsson supports) */
    if (requested_measurements & NRPPA_ECID_MEASUREMENT_TYPE_TA) {
        quantity_ie = CALLOC(1, sizeof(*quantity_ie));
        if (!quantity_ie) {
            ogs_error("CALLOC failed for MeasurementQuantities_ItemIEs (TA)");
            goto err;
        }
        quantity_ie->id = ProtocolIE_ID_id_MeasurementQuantities_Item;
        quantity_ie->criticality = Criticality_reject;
        quantity_ie->value.present = MeasurementQuantities_ItemIEs__value_PR_MeasurementQuantities_Item;
        quantity_ie->value.choice.MeasurementQuantities_Item.measurementQuantitiesValue = MeasurementQuantitiesValue_timingAdvanceNR;
        rv = ASN_SEQUENCE_ADD(&ie->value.choice.MeasurementQuantities.list, quantity_ie);
        if (rv != 0) {
            ogs_error("ASN_SEQUENCE_ADD failed for timingAdvanceNR, rv=%d", rv);
            ogs_free(quantity_ie);
            goto err;
        }
        quantity_ie = NULL;
    }

    /* Add Angle of Arrival NR if requested */
    if (requested_measurements & NRPPA_ECID_MEASUREMENT_TYPE_AOA) {
        quantity_ie = CALLOC(1, sizeof(*quantity_ie));
        if (!quantity_ie) {
            ogs_error("CALLOC failed for MeasurementQuantities_ItemIEs (AoA-NR)");
            goto err;
        }
        quantity_ie->id = ProtocolIE_ID_id_MeasurementQuantities_Item;
        quantity_ie->criticality = Criticality_reject;
        quantity_ie->value.present = MeasurementQuantities_ItemIEs__value_PR_MeasurementQuantities_Item;
        quantity_ie->value.choice.MeasurementQuantities_Item.measurementQuantitiesValue = MeasurementQuantitiesValue_angleOfArrivalNR;
        rv = ASN_SEQUENCE_ADD(&ie->value.choice.MeasurementQuantities.list, quantity_ie);
        if (rv != 0) {
            ogs_error("ASN_SEQUENCE_ADD failed for angleOfArrivalNR, rv=%d", rv);
            ogs_free(quantity_ie);
            goto err;
        }
        quantity_ie = NULL;
        ogs_info("  Added angleOfArrivalNR to measurement quantities");
    }

    /* Add MeasurementQuantities IE to protocolIEs */
    rv = ASN_SEQUENCE_ADD(&request.protocolIEs.list, ie);
    if (rv != 0) {
        ogs_error("ASN_SEQUENCE_ADD failed for MeasurementQuantities, rv=%d", rv);
        ogs_free(ie);
        goto err;
    }
    ie = NULL;

    /* Verify structure */
    ogs_info("  protocolIEs.list.count = %d", request.protocolIEs.list.count);
    if (request.protocolIEs.list.count < 3) {
        ogs_error("Protocol IEs count is %d, expected at least 3", request.protocolIEs.list.count);
        goto err;
    }
    
    /* Encode E-CIDMeasurementInitiationRequest body */
    request_pkbuf = ogs_asn_encode(&asn_DEF_E_CIDMeasurementInitiationRequest, &request);
    if (!request_pkbuf) {
        ogs_error("ogs_asn_encode failed for request");
        goto err;
    }
    
    /* Manually prepend InitiatingMessage header (APER encoded) */
    pkbuf = ogs_pkbuf_alloc(NULL, request_pkbuf->len + 20);  /* Extra space for header */
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc failed");
        ogs_pkbuf_free(request_pkbuf);
        goto err;
    }
    
    p = (uint8_t *)pkbuf->data;
    
    /* APER encoding of InitiatingMessage wrapper 
     * Based on NGAP pattern and NRPPa v17.2.0 structure:
     * - procedureCode (INTEGER 0..255)
     * - criticality (ENUMERATED)  
     * - nrppatransactionID (INTEGER 0..32767)
     * - value (OPEN TYPE containing the request)
     */
    
    /* Field 1: procedureCode = 2 (id-e-CIDMeasurementInitiation) - constrained integer */
    *p++ = 0x00;  /* Extension bit + 7 bits of value */
    *p++ = 0x02;  /* Value = 2 */
    
    /* Field 2: criticality = 0 (reject) - enumerated */
    *p++ = 0x00;  /* Value = 0 (reject) */
    
    /* Field 3: nrppatransactionID = transaction_id - constrained integer (0..32767) */
    *p++ = 0x00;  /* High byte (transaction_id is small) */
    *p++ = transaction_id & 0xFF;  /* Low byte */
    
    /* Field 4: value - OPEN TYPE, length-prefixed */
    /* APER OPEN TYPE: put length then content */
    if (request_pkbuf->len < 128) {
        *p++ = request_pkbuf->len;  /* Short form length */
    } else {
        *p++ = 0x80 | ((request_pkbuf->len >> 8) & 0x7F);  /* Long form */
        *p++ = request_pkbuf->len & 0xFF;
    }
    
    /* Copy the encoded request */
    memcpy(p, request_pkbuf->data, request_pkbuf->len);
    pkbuf->len = (p - (uint8_t *)pkbuf->data) + request_pkbuf->len;
    
    ogs_info("NRPPa InitiatingMessage (E-CID Request) built, "
             "measurement_id=%u, transaction_id=%u",
             measurement_id, transaction_id);
    ogs_log_hexdump(OGS_LOG_INFO, pkbuf->data, pkbuf->len);

    /* Free structures */
    ogs_pkbuf_free(request_pkbuf);
    ogs_asn_free(&asn_DEF_E_CIDMeasurementInitiationRequest, &request);
    return pkbuf;

err:
    if (request_pkbuf)
        ogs_pkbuf_free(request_pkbuf);
    ogs_asn_free(&asn_DEF_E_CIDMeasurementInitiationRequest, &request);
    return NULL;
}
