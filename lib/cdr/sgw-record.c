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

/*
 * SGWRecord encoder per 3GPP TS 32.298 (checked against v18.2.0 ASN.1).
 * Emitted as GPRSRecord CHOICE alternative sGWRecord [78].
 * Same back-to-front emission as pgw-record.c.
 */

#include "ogs-cdr.h"
#include "ber.h"

#include <string.h>

/* GPRSRecord CHOICE */
#define TAG_SGW_RECORD                  78

/* SGWRecord SET member tags */
#define TAG_RECORD_TYPE                 0
#define TAG_SERVED_IMSI                 3
#define TAG_SGW_ADDRESS                 4
#define TAG_CHARGING_ID                 5
#define TAG_SERVING_NODE_ADDRESS        6
#define TAG_ACCESS_POINT_NAME_NI        7
#define TAG_PDP_PDN_TYPE                8
#define TAG_SERVED_PDP_PDN_ADDRESS      9
#define TAG_DYNAMIC_ADDRESS_FLAG        11
#define TAG_LIST_OF_TRAFFIC_VOLUMES     12
#define TAG_RECORD_OPENING_TIME         13
#define TAG_DURATION                    14
#define TAG_CAUSE_FOR_REC_CLOSING       15
#define TAG_RECORD_SEQUENCE_NUMBER      17
#define TAG_NODE_ID                     18
#define TAG_LOCAL_SEQUENCE_NUMBER       20
#define TAG_APN_SELECTION_MODE          21
#define TAG_SERVED_MSISDN               22
#define TAG_CHARGING_CHARACTERISTICS    23
#define TAG_SERVING_NODE_PLMN_ID        27
#define TAG_SERVED_IMEI                 29
#define TAG_RAT_TYPE                    30
#define TAG_MS_TIME_ZONE                31
#define TAG_USER_LOCATION_INFO          32
#define TAG_SERVING_NODE_TYPE           35
#define TAG_PGW_ADDRESS_USED            36
#define TAG_START_TIME                  38
#define TAG_STOP_TIME                   39

/* ChangeOfCharCondition SEQUENCE member tags */
#define TAG_COCC_VOLUME_UPLINK          3
#define TAG_COCC_VOLUME_DOWNLINK        4
#define TAG_COCC_CHANGE_CONDITION       5
#define TAG_COCC_CHANGE_TIME            6

/* IPBinaryAddress CHOICE */
#define TAG_IP_BIN_V4_ADDRESS           0
/* PDPAddress CHOICE */
#define TAG_PDP_IP_ADDRESS              0

#define RECORD_TYPE_SGW_RECORD          84
/* ServingNodeType ENUMERATED */
#define SERVING_NODE_TYPE_MME           5
/* APNSelectionMode ENUMERATED */
#define APN_SELECTION_MS_OR_NET_VERIFIED 0

static void ctx_timestamp(ogs_ber_t *ber,
        uint32_t tag, time_t t, int tz_offset_min)
{
    uint8_t ts[9];
    ogs_cdr_make_timestamp(ts, t, tz_offset_min);
    ogs_ber_ctx_octets(ber, tag, ts, sizeof(ts));
}

/* [tag] GSNAddress: explicit wrapper over CHOICE { [0] IPv4 octets } */
static void ctx_gsn_address(ogs_ber_t *ber, uint32_t tag, const uint8_t v4[4])
{
    size_t mark = ogs_ber_mark(ber);
    ogs_ber_ctx_octets(ber, TAG_IP_BIN_V4_ADDRESS, v4, 4);
    ogs_ber_wrap(ber, OGS_BER_CLASS_CONTEXT, tag, mark);
}

static void ctx_enumerated(ogs_ber_t *ber, uint32_t tag, uint8_t value)
{
    ogs_ber_ctx_octets(ber, tag, &value, 1);
}

/*
 * listOfTrafficVolumes: single ChangeOfCharCondition container.
 * changeCondition is always recordClosure(2): volume/time limits close
 * the whole record (causeForRecClosing), not just the container.
 */
static void encode_traffic_volume_container(ogs_ber_t *ber,
        const ogs_cdr_sgw_record_t *rec)
{
    size_t seq_mark, list_mark = ogs_ber_mark(ber);

    seq_mark = ogs_ber_mark(ber);

    ctx_timestamp(ber, TAG_COCC_CHANGE_TIME,
            rec->time_of_report, rec->tz_offset_min);
    ctx_enumerated(ber, TAG_COCC_CHANGE_CONDITION, 2 /* recordClosure */);
    ogs_ber_ctx_uint(ber, TAG_COCC_VOLUME_DOWNLINK, rec->vol_downlink);
    ogs_ber_ctx_uint(ber, TAG_COCC_VOLUME_UPLINK, rec->vol_uplink);

    ogs_ber_prepend_header(ber, OGS_BER_CLASS_UNIVERSAL, 1, 16,
            seq_mark - ogs_ber_mark(ber));
    ogs_ber_wrap(ber, OGS_BER_CLASS_CONTEXT,
            TAG_LIST_OF_TRAFFIC_VOLUMES, list_mark);
}

int ogs_cdr_encode_sgw_record(
        const ogs_cdr_sgw_record_t *rec, uint8_t *buf, size_t bufsize)
{
    ogs_ber_t ber;
    size_t mark, set_mark;

    if (!rec || !buf)
        return -1;

    ogs_ber_init(&ber, buf, bufsize);
    set_mark = ogs_ber_mark(&ber);

    if (rec->has_stop_time)
        ctx_timestamp(&ber, TAG_STOP_TIME,
                rec->stop_time, rec->tz_offset_min);
    ctx_timestamp(&ber, TAG_START_TIME, rec->start_time, rec->tz_offset_min);

    if (rec->has_pgw_addr)
        ctx_gsn_address(&ber, TAG_PGW_ADDRESS_USED, rec->pgw_addr);

    /* servingNodeType: SEQUENCE OF ENUMERATED, single mME element */
    mark = ogs_ber_mark(&ber);
    {
        uint8_t e = SERVING_NODE_TYPE_MME;
        ogs_ber_prepend(&ber, &e, 1);
        ogs_ber_prepend_header(&ber, OGS_BER_CLASS_UNIVERSAL, 0, 10, 1);
    }
    ogs_ber_wrap(&ber, OGS_BER_CLASS_CONTEXT, TAG_SERVING_NODE_TYPE, mark);

    if (rec->uli_len > 0)
        ogs_ber_ctx_octets(&ber, TAG_USER_LOCATION_INFO,
                rec->uli, (size_t)rec->uli_len);
    if (rec->has_ms_timezone)
        ogs_ber_ctx_octets(&ber, TAG_MS_TIME_ZONE, rec->ms_timezone, 2);
    ogs_ber_ctx_uint(&ber, TAG_RAT_TYPE, rec->rat_type);
    if (rec->imeisv_len > 0)
        ogs_ber_ctx_octets(&ber, TAG_SERVED_IMEI,
                rec->imeisv, (size_t)rec->imeisv_len);
    if (rec->has_serving_plmn)
        ogs_ber_ctx_octets(&ber, TAG_SERVING_NODE_PLMN_ID,
                rec->serving_plmn, 3);
    ogs_ber_ctx_octets(&ber, TAG_CHARGING_CHARACTERISTICS,
            rec->charging_char, 2);
    if (rec->msisdn_len > 0) {
        uint8_t msisdn[1 + sizeof(rec->msisdn)];
        msisdn[0] = 0x91;
        memcpy(msisdn + 1, rec->msisdn, (size_t)rec->msisdn_len);
        ogs_ber_ctx_octets(&ber, TAG_SERVED_MSISDN,
                msisdn, 1 + (size_t)rec->msisdn_len);
    }
    ctx_enumerated(&ber, TAG_APN_SELECTION_MODE,
            APN_SELECTION_MS_OR_NET_VERIFIED);
    ogs_ber_ctx_uint(&ber, TAG_LOCAL_SEQUENCE_NUMBER,
            rec->local_sequence_number);
    if (rec->node_id[0])
        ogs_ber_ctx_octets(&ber, TAG_NODE_ID,
                (const uint8_t *)rec->node_id, strlen(rec->node_id));
    if (rec->record_sequence_number > 0)
        ogs_ber_ctx_uint(&ber, TAG_RECORD_SEQUENCE_NUMBER,
                rec->record_sequence_number);
    ogs_ber_ctx_uint(&ber, TAG_CAUSE_FOR_REC_CLOSING, (uint64_t)rec->cause);
    ogs_ber_ctx_uint(&ber, TAG_DURATION, rec->duration);
    ctx_timestamp(&ber, TAG_RECORD_OPENING_TIME,
            rec->opening_time, rec->tz_offset_min);

    encode_traffic_volume_container(&ber, rec);

    if (rec->dynamic_address)
        ogs_ber_ctx_bool(&ber, TAG_DYNAMIC_ADDRESS_FLAG, 1);
    if (rec->has_pdn_addr) {
        size_t pdn_mark = ogs_ber_mark(&ber);
        size_t ip_mark = ogs_ber_mark(&ber);
        ogs_ber_ctx_octets(&ber, TAG_IP_BIN_V4_ADDRESS, rec->pdn_addr, 4);
        ogs_ber_wrap(&ber, OGS_BER_CLASS_CONTEXT,
                TAG_PDP_IP_ADDRESS, ip_mark);
        ogs_ber_wrap(&ber, OGS_BER_CLASS_CONTEXT,
                TAG_SERVED_PDP_PDN_ADDRESS, pdn_mark);
    }
    {
        uint8_t pdp_type[2] = { 0xf1, rec->pdn_type };
        ogs_ber_ctx_octets(&ber, TAG_PDP_PDN_TYPE, pdp_type, 2);
    }
    if (rec->apn_ni[0])
        ogs_ber_ctx_octets(&ber, TAG_ACCESS_POINT_NAME_NI,
                (const uint8_t *)rec->apn_ni, strlen(rec->apn_ni));

    /* servingNodeAddress: SEQUENCE OF GSNAddress, single MME element */
    mark = ogs_ber_mark(&ber);
    ogs_ber_ctx_octets(&ber, TAG_IP_BIN_V4_ADDRESS, rec->mme_addr, 4);
    ogs_ber_wrap(&ber, OGS_BER_CLASS_CONTEXT,
            TAG_SERVING_NODE_ADDRESS, mark);

    ogs_ber_ctx_uint(&ber, TAG_CHARGING_ID, rec->charging_id);
    ctx_gsn_address(&ber, TAG_SGW_ADDRESS, rec->sgw_addr);
    if (rec->imsi_len > 0)
        ogs_ber_ctx_octets(&ber, TAG_SERVED_IMSI,
                rec->imsi, (size_t)rec->imsi_len);
    ogs_ber_ctx_uint(&ber, TAG_RECORD_TYPE, RECORD_TYPE_SGW_RECORD);

    ogs_ber_wrap(&ber, OGS_BER_CLASS_CONTEXT, TAG_SGW_RECORD, set_mark);

    return ogs_ber_finish(&ber);
}
