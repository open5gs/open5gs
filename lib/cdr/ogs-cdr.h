/*
 * Copyright (C) 2026 by DNL
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

#ifndef OGS_CDR_H
#define OGS_CDR_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Offline CDR generation per 3GPP TS 32.298 (PGWRecord, BER) and
 * TS 32.297 (CDR file format). Deliberately free of ogs-core
 * dependencies so it can be unit-tested standalone.
 */

/* CauseForRecClosing (TS 32.298 GenericChargingDataTypes) */
#define OGS_CDR_CAUSE_NORMAL_RELEASE        0
#define OGS_CDR_CAUSE_ABNORMAL_RELEASE      4
#define OGS_CDR_CAUSE_VOLUME_LIMIT          16
#define OGS_CDR_CAUSE_TIME_LIMIT            17

/* ServiceConditionChange bit numbers (TS 32.298 GPRSChargingDataTypes) */
#define OGS_CDR_SCC_RECORD_CLOSURE          24
#define OGS_CDR_SCC_TIME_LIMIT              25
#define OGS_CDR_SCC_VOLUME_LIMIT            26

/* pdpPDNType (TS 29.060 7.7.27: org=IETF, number) */
#define OGS_CDR_PDN_TYPE_IPV4               0x21
#define OGS_CDR_PDN_TYPE_IPV6               0x57
#define OGS_CDR_PDN_TYPE_IPV4V6             0x8D

/* RATType (TS 29.061) */
#define OGS_CDR_RAT_TYPE_EUTRAN             6

#define OGS_CDR_MAX_APN_NI_LEN              63
#define OGS_CDR_MAX_NODE_ID_LEN             20
#define OGS_CDR_MAX_ULI_LEN                 64

/* Encoded PGWRecord upper bound (all fields, single container) */
#define OGS_CDR_MAX_RECORD_LEN              512

typedef struct ogs_cdr_pgw_record_s {
    /* subscriber / session identity */
    uint8_t     imsi[8];            /* TBCD as on wire */
    int         imsi_len;
    uint8_t     msisdn[9];          /* TBCD, without NOA octet */
    int         msisdn_len;
    uint8_t     imeisv[8];          /* TBCD */
    int         imeisv_len;
    char        apn_ni[OGS_CDR_MAX_APN_NI_LEN + 1];
    uint32_t    charging_id;
    uint8_t     pgw_addr[4];        /* IPv4 only for now */
    uint8_t     sgw_addr[4];
    uint8_t     pdn_type;           /* OGS_CDR_PDN_TYPE_* */
    int         has_pdn_addr;
    uint8_t     pdn_addr[4];
    int         dynamic_address;    /* bool */
    uint8_t     charging_char[2];
    int         has_serving_plmn;
    uint8_t     serving_plmn[3];    /* TBCD MCC/MNC */
    int         has_pgw_plmn;
    uint8_t     pgw_plmn[3];
    uint8_t     rat_type;
    int         has_ms_timezone;
    uint8_t     ms_timezone[2];     /* as in TS 29.060 */
    uint8_t     uli[OGS_CDR_MAX_ULI_LEN];
    int         uli_len;
    char        node_id[OGS_CDR_MAX_NODE_ID_LEN + 1];

    /* record metadata */
    time_t      opening_time;
    time_t      start_time;
    int         has_stop_time;
    time_t      stop_time;
    int         tz_offset_min;      /* local offset from UTC, minutes */
    uint32_t    duration;           /* seconds */
    int         cause;              /* OGS_CDR_CAUSE_* */
    uint32_t    record_sequence_number; /* 0 = omit (single full record) */
    uint32_t    local_sequence_number;

    /* single service-data container */
    uint32_t    rating_group;
    uint64_t    vol_uplink;
    uint64_t    vol_downlink;
    int         has_usage_times;
    time_t      time_of_first_usage;
    time_t      time_of_last_usage;
    time_t      time_of_report;
    int         service_condition;  /* OGS_CDR_SCC_* bit number */
} ogs_cdr_pgw_record_t;

/*
 * Encode one PGWRecord as GPRSRecord CHOICE alternative pGWRecord [79].
 * Returns encoded length, or -1 on error / buffer too small.
 */
int ogs_cdr_encode_pgw_record(
        const ogs_cdr_pgw_record_t *rec, uint8_t *buf, size_t bufsize);

typedef struct ogs_cdr_sgw_record_s {
    /* subscriber / session identity */
    uint8_t     imsi[8];            /* TBCD as on wire */
    int         imsi_len;
    uint8_t     msisdn[9];          /* TBCD, without NOA octet */
    int         msisdn_len;
    uint8_t     imeisv[8];          /* TBCD */
    int         imeisv_len;
    char        apn_ni[OGS_CDR_MAX_APN_NI_LEN + 1];
    uint32_t    charging_id;
    uint8_t     sgw_addr[4];        /* IPv4 only for now */
    uint8_t     mme_addr[4];        /* servingNodeAddress */
    int         has_pgw_addr;
    uint8_t     pgw_addr[4];        /* p-GWAddressUsed */
    uint8_t     pdn_type;           /* OGS_CDR_PDN_TYPE_* */
    int         has_pdn_addr;
    uint8_t     pdn_addr[4];
    int         dynamic_address;    /* bool */
    uint8_t     charging_char[2];
    int         has_serving_plmn;
    uint8_t     serving_plmn[3];    /* TBCD MCC/MNC */
    uint8_t     rat_type;
    int         has_ms_timezone;
    uint8_t     ms_timezone[2];
    uint8_t     uli[OGS_CDR_MAX_ULI_LEN];
    int         uli_len;
    char        node_id[OGS_CDR_MAX_NODE_ID_LEN + 1];

    /* record metadata (same closure model as PGWRecord) */
    time_t      opening_time;
    time_t      start_time;
    int         has_stop_time;
    time_t      stop_time;
    int         tz_offset_min;
    uint32_t    duration;
    int         cause;              /* OGS_CDR_CAUSE_* */
    uint32_t    record_sequence_number; /* 0 = omit */
    uint32_t    local_sequence_number;

    /* single ChangeOfCharCondition traffic-volume container */
    uint64_t    vol_uplink;
    uint64_t    vol_downlink;
    time_t      time_of_report;     /* container changeTime */
} ogs_cdr_sgw_record_t;

/*
 * Encode one SGWRecord as GPRSRecord CHOICE alternative sGWRecord [78].
 * Returns encoded length, or -1 on error / buffer too small.
 */
int ogs_cdr_encode_sgw_record(
        const ogs_cdr_sgw_record_t *rec, uint8_t *buf, size_t bufsize);

/* TS 32.297 file closure trigger reasons */
#define OGS_CDR_FILE_CLOSE_NORMAL           0
#define OGS_CDR_FILE_CLOSE_SIZE_LIMIT       1
#define OGS_CDR_FILE_CLOSE_TIME_LIMIT       2
#define OGS_CDR_FILE_CLOSE_MANUAL           4

typedef struct ogs_cdr_file_s ogs_cdr_file_t;

/*
 * Opens "<final_path>.tmp" and writes a placeholder file header.
 * node_ipv4 goes into the header "Node IP address" field.
 */
ogs_cdr_file_t *ogs_cdr_file_open(const char *final_path,
        uint32_t file_sequence_number, const uint8_t node_ipv4[4],
        time_t now, int tz_offset_min);

/* Appends one encoded CDR with its TS 32.297 CDR header. */
int ogs_cdr_file_append(ogs_cdr_file_t *file,
        const uint8_t *cdr, size_t cdr_len, time_t now);

unsigned int ogs_cdr_file_count(const ogs_cdr_file_t *file);
size_t ogs_cdr_file_size(const ogs_cdr_file_t *file);

/*
 * Finalizes the header, fsyncs and renames "<path>.tmp" to "<path>".
 * Frees the handle. Empty files (0 CDRs) are deleted instead.
 * Returns 0 on success.
 */
int ogs_cdr_file_close(ogs_cdr_file_t *file, uint8_t closure_reason);

#ifdef __cplusplus
}
#endif

#endif /* OGS_CDR_H */
