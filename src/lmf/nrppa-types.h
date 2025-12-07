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

#ifndef LMF_NRPPA_TYPES_H
#define LMF_NRPPA_TYPES_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NRPPa Message Types (TS 38.455) */
#define NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_INITIATION_REQUEST     0
#define NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_INITIATION_RESPONSE    1
#define NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_FAILURE_INDICATION     2

/* ECID Measurement Types */
#define NRPPA_ECID_MEASUREMENT_TYPE_RSRP   0x01
#define NRPPA_ECID_MEASUREMENT_TYPE_RSRQ   0x02
#define NRPPA_ECID_MEASUREMENT_TYPE_TA     0x04  /* Timing Advance */
#define NRPPA_ECID_MEASUREMENT_TYPE_AOA    0x08  /* Angle of Arrival */
#define NRPPA_ECID_MEASUREMENT_TYPE_CSI    0x10  /* CSI quality */

/* Maximum number of neighbor cells in ECID measurement */
#define NRPPA_MAX_NEIGHBOR_CELLS    16

/* ECID Measurement Request */
typedef struct lmf_nrppa_ecid_request_s {
    uint32_t measurement_id;          /* Measurement ID */
    uint64_t ue_ngap_id;              /* UE NGAP ID */
    uint8_t requested_measurements;   /* Bitmap of requested measurements */
    uint32_t measurement_period;      /* Measurement period in ms */
} lmf_nrppa_ecid_request_t;

/* ECID Cell Measurement */
typedef struct lmf_nrppa_ecid_cell_measurement_s {
    uint64_t ncgi;                    /* NR Cell Global Identifier */
    int16_t rsrp;                     /* RSRP in 0.1 dBm units (-1400 to -440) */
    int16_t rsrq;                     /* RSRQ in 0.1 dB units (-300 to -196) */
    uint16_t timing_advance;          /* Timing advance in 16 Ts units */
    int16_t angle_of_arrival;         /* Angle of arrival in degrees * 2 (-180 to 180) */
} lmf_nrppa_ecid_cell_measurement_t;

/* ECID Measurement Response */
typedef struct lmf_nrppa_ecid_response_s {
    uint32_t measurement_id;          /* Measurement ID */
    uint64_t ue_ngap_id;              /* UE NGAP ID */
    
    /* Serving cell measurement */
    lmf_nrppa_ecid_cell_measurement_t serving_cell;
    
    /* Neighbor cell measurements */
    int num_neighbor_cells;
    lmf_nrppa_ecid_cell_measurement_t neighbor_cells[NRPPA_MAX_NEIGHBOR_CELLS];
    
    uint64_t measurement_timestamp;   /* Timestamp in milliseconds since epoch */
} lmf_nrppa_ecid_response_t;

/* ECID Measurement Failure */
typedef struct lmf_nrppa_ecid_failure_s {
    uint32_t measurement_id;          /* Measurement ID */
    uint64_t ue_ngap_id;              /* UE NGAP ID */
    uint8_t cause;                    /* Failure cause */
    char *cause_description;          /* Optional failure description */
} lmf_nrppa_ecid_failure_t;

/* NRPPa PDU (Generic container) */
typedef struct lmf_nrppa_pdu_s {
    uint8_t message_type;             /* NRPPa message type */
    
    union {
        lmf_nrppa_ecid_request_t ecid_request;
        lmf_nrppa_ecid_response_t ecid_response;
        lmf_nrppa_ecid_failure_t ecid_failure;
    } u;
} lmf_nrppa_pdu_t;

#ifdef __cplusplus
}
#endif

#endif /* LMF_NRPPA_TYPES_H */


