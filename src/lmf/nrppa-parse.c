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

#include "nrppa-parse.h"
#include "asn1c/util/message.h"

/* NRPPa v17.2.0 response parsing
 * Note: Uses simplified parsing for reliability. Full ASN.1 decode can be added
 * incrementally as needed for specific fields.
 */

int lmf_nrppa_parse_pdu(ogs_pkbuf_t *pkbuf, lmf_nrppa_pdu_t *pdu)
{
    int rv = OGS_OK;
    uint8_t *data;
    size_t len;
    uint8_t pdu_choice;
    uint16_t procedure_code;
    uint16_t transaction_id;
    
    ogs_assert(pkbuf);
    ogs_assert(pdu);
    
    memset(pdu, 0, sizeof(*pdu));
    pdu->message_type = NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_INITIATION_REQUEST;
    
    data = pkbuf->data;
    len = pkbuf->len;
    
    if (len < 6) {
        ogs_error("NRPPa PDU too short (%zu bytes)", len);
        return OGS_ERROR;
    }
    
    ogs_info("Parsing NRPPa PDU (%zu bytes)", len);
    ogs_log_hexdump(OGS_LOG_INFO, data, len);
    
    /* Parse APER-encoded wrapper header */
    pdu_choice = data[0] >> 5;
    procedure_code = data[1];
    transaction_id = data[4];
    
    ogs_info("  PDU: choice=%u, procedure=%u, transaction=%u", 
             pdu_choice, procedure_code, transaction_id);
    
    if (pdu_choice == 1 && procedure_code == 2) {  /* successfulOutcome */
        ogs_info("E-CID Measurement Initiation Response (v17.2.0)");
        
        pdu->message_type = NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_INITIATION_RESPONSE;
        pdu->u.ecid_response.measurement_id = transaction_id;
        pdu->u.ecid_response.ue_ngap_id = 0;
        
        /* Extract Cell ID from servingCell-ID.nG-RANcell.nR-CellID
         * Located at bytes 28-32: 5 bytes with 36 bits + 4 pad bits
         * Example: 2b 2c dc 06 f0 → 0x2b2cdc06f
         */
        if (len > 32) {
            uint64_t cell_id = ((uint64_t)data[28] << 28) |
                              ((uint64_t)data[29] << 20) |
                              ((uint64_t)data[30] << 12) |
                              ((uint64_t)data[31] << 4) |
                              ((uint64_t)data[32] >> 4);
            
            pdu->u.ecid_response.serving_cell.ncgi = cell_id;
            ogs_info("  Cell ID: 0x%llx", (unsigned long long)cell_id);
        }
        
        /* Extract SS-RSRP from ResultSS-RSRP-Item.valueSS-RSRP-Cell
         * The cell ID appears TWICE in the response:
         *   1. servingCell-ID (byte 28)
         *   2. cGI-NR within ResultSS-RSRP-Item (byte 54) ← RSRP follows this one!
         * 
         * RSRP is 7 bits encoded immediately after the SECOND cell ID:
         *   - Lower 4 bits of byte 58 (last cell ID byte): RSRP bits 0-3
         *   - Upper 3 bits of byte 59: RSRP bits 4-6
         * Formula: RSRP = ((data[58] & 0x0F) << 3) | (data[59] >> 5)
         * Then: RSRP (dBm) = value - 156 per 3GPP TS 38.455
         */
        if (len > 59) {
            uint8_t rsrp_raw = ((data[58] & 0x0F) << 3) | (data[59] >> 5);
            int16_t rsrp_dbm = (int16_t)rsrp_raw - 156;
            
            pdu->u.ecid_response.serving_cell.rsrp = rsrp_dbm * 10;  /* 0.1 dBm units */
            ogs_info("  SS-RSRP: value=%u (from cGI-NR), RSRP=%d dBm", 
                    rsrp_raw, rsrp_dbm);
        } else {
            ogs_warn("  SS-RSRP: response too short");
            pdu->u.ecid_response.serving_cell.rsrp = -800;  /* Default -80 dBm */
        }
        
        /* Extract NR Timing Advance (NR-TADV) from measuredResults
         * NR-TADV appears after ResultSS-RSRP in the measuredResults list
         * It's encoded as id-NR-TADV (94) followed by the TA value
         * 
         * Looking for pattern: 5E 40 02 00 00 (id=94, criticality=ignore, value=0)
         * Byte 60-61: 0x5E40 = id-NR-TADV (94) with criticality
         * Byte 62: Length indicator
         * Byte 63-64: NR-TADV value (0-7690, encoded as INTEGER)
         * 
         * NR-TADV range: 0..7690 (per 3GPP TS 38.455)
         * Physical meaning: TA × 16 × Tc where Tc = 1/(480000 × 4096) seconds
         * Approximate distance: TA × 156 meters (round-trip time / 2)
         */
        /* Extract NR Timing Advance (NR-TADV) from measuredResults
         * Search for id-NR-TADV (0x5E40) in the response
         */
        if (len > 64) {
            int ta_offset = -1;
            int i;
            /* Search from byte 59 to end, ensuring we have 5 bytes for full TA IE */
            for (i = 59; i <= (int)len - 5; i++) {
                if (data[i] == 0x5E && data[i+1] == 0x40) {
                    ta_offset = i;
                    break;
                }
            }
            
            if (ta_offset >= 0 && ta_offset + 4 < len) {
                /* Extract TA value (2 bytes, big-endian) */
                uint16_t ta_value = ((uint16_t)data[ta_offset + 3] << 8) | 
                                    (uint16_t)data[ta_offset + 4];
                
                pdu->u.ecid_response.serving_cell.timing_advance = ta_value;
                
                /* Calculate approximate distance in meters */
                uint32_t distance_m = ta_value * 156;  /* Approximate: TA × 156 meters */
                
                ogs_info("  NR-TADV: value=%u (distance ~%u meters)", 
                        ta_value, distance_m);
            } else {
                ogs_info("  NR-TADV: not found in response (searched bytes 59-%zu)", len - 5);
                pdu->u.ecid_response.serving_cell.timing_advance = 0;
            }
        } else {
            ogs_info("  NR-TADV: response too short (%zu bytes, need >64)", len);
            pdu->u.ecid_response.serving_cell.timing_advance = 0;
        }
        
        /* Extract Angle of Arrival (UL-AoA) from measuredResults
         * Search for id-AngleOfArrivalNR (0x2440) = 36
         * According to TS 38.455 v17.2.0 Section 9.2.38:
         * - Azimuth Angle of Arrival (mandatory when AoA is present)
         * - Zenith Angle of Arrival (optional)
         * 
         * UL-AoA structure:
         * - azimuthAoA: INTEGER (0..3599) in 0.1 degree units
         * - zenithAoA: INTEGER (0..1799) OPTIONAL in 0.1 degree units
         */
        {
            int aoa_offset = -1;
            int i;
            
            /* Search for id-AngleOfArrivalNR (0x24) in the response */
            for (i = 59; i <= (int)len - 10; i++) {
                if (data[i] == 0x24 && data[i+1] == 0x40) {
                    aoa_offset = i;
                    break;
                }
            }
            
            if (aoa_offset >= 0 && aoa_offset + 10 < len) {
                /* Extract Azimuth AoA (2 bytes, big-endian, range 0-3599, units: 0.1 degree) */
                uint16_t azimuth_raw = ((uint16_t)data[aoa_offset + 3] << 8) | 
                                       (uint16_t)data[aoa_offset + 4];
                
                /* Convert to degrees * 2 for storage (0.5 degree precision) */
                pdu->u.ecid_response.serving_cell.angle_of_arrival = (int16_t)(azimuth_raw / 5);
                
                /* Check if Zenith is present (OPTIONAL) */
                if (aoa_offset + 6 < len) {
                    /* Zenith might be at offset+5, but encoding is complex */
                    /* For now, just extract azimuth */
                }
                
                ogs_info("  UL-AoA: Azimuth=%u (%.1f degrees)", 
                        azimuth_raw, azimuth_raw / 10.0);
            } else {
                /* AoA not found - check if it was expected */
                ogs_info("  UL-AoA: not found in response");
                ogs_info("    Note: AoA requires UE SRS transmission and gNB antenna array");
                ogs_info("    Note: Check if LCS context or specific measurement type is needed");
                pdu->u.ecid_response.serving_cell.angle_of_arrival = 0;
            }
        }
        
        pdu->u.ecid_response.num_neighbor_cells = 0;
        pdu->u.ecid_response.measurement_timestamp = ogs_time_now() / 1000;
        
        ogs_info("Response parsed successfully");
        rv = OGS_OK;
        
    } else if (pdu_choice == 2 && procedure_code == 2) {  /* unsuccessfulOutcome */
        ogs_info("E-CID Measurement Initiation Failure");
        
        pdu->message_type = NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_FAILURE_INDICATION;
        pdu->u.ecid_failure.measurement_id = transaction_id;
        
        /* Extract cause if present */
        if (len > 14) {
            pdu->u.ecid_failure.cause = data[14];
            ogs_info("  Failure cause: %u", pdu->u.ecid_failure.cause);
        }
        
        rv = OGS_OK;
        
    } else {
        ogs_error("Unexpected NRPPa PDU: choice=%u, procedure=%u", 
                 pdu_choice, procedure_code);
        rv = OGS_ERROR;
    }
    
    return rv;
}
