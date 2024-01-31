/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SMSF_SMS_H
#define SMSF_SMS_H

#include "ogs-core.h"
#include "ogs-proto.h"
#include "context.h"
#include "namf-build.h"
#include "sbi-path.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct smsf_rpdu_address_s {
        uint8_t length;
        union {
        struct {
        ED3(uint8_t ext:1;,
            uint8_t ton:3;,
            uint8_t npi:4;)
        };
        uint8_t octet;
        } header;
        uint8_t rp_address[11];
} __attribute__ ((packed)) smsf_rpdu_address_t;

typedef struct smsf_sms_tp_address_s {
        uint8_t addr_length;
        union {
        struct {
        ED3(uint8_t ext:1;,
            uint8_t ton:3;,
            uint8_t npi:4;)
        };
        uint8_t octet;
        } header;
        uint8_t tp_address[10];
} __attribute__ ((packed)) smsf_sms_tp_address_t;

typedef struct smsf_sms_rpdu_message_type_s {
        union {
        ED2(uint8_t reserved:5;,
            uint8_t value:3;)
        uint8_t octet;
        };
} __attribute__ ((packed)) smsf_sms_rpdu_message_type_t;

typedef struct smsf_sms_rpdata_s {
        smsf_sms_rpdu_message_type_t rpdu_message_type;
        uint8_t rp_message_reference;
        smsf_rpdu_address_t rp_originator_address;
        smsf_rpdu_address_t rp_destination_address;
        uint8_t rp_user_data_length;
} __attribute__ ((packed)) smsf_sms_rpdata_t;

typedef struct smsf_sms_cp_hdr_s {
        union {
        struct {
        ED3(uint8_t tif:1;,
            uint8_t tio:3;,
            uint8_t pd:4;)
        };
        uint8_t octet;
        } flags;
        uint8_t sm_service_message_type;
} __attribute__ ((packed)) smsf_sms_cp_hdr_t;

typedef struct smsf_sms_cp_data_s {
        smsf_sms_cp_hdr_t header;
        uint8_t cp_user_data_length;
} __attribute__ ((packed)) smsf_sms_cp_data_t;

typedef struct smsf_sms_cp_error_s {
        smsf_sms_cp_hdr_t header;
        uint8_t cp_cause;
} __attribute__ ((packed)) smsf_sms_cp_error_t;

typedef struct smsf_sms_tpdu_hdr_s {
        union {
        ED2(uint8_t spare:6;,
            uint8_t tpMTI:2;)
        uint8_t octet;
        };
} __attribute__ ((packed)) smsf_sms_tpdu_hdr_t;

#define OGS_SCTS_TIME_TO_BCD(x) OGS_TIME_TO_BCD(x)
typedef struct smsf_sms_tpscts_s {
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t timezone;
} __attribute__ ((packed)) smsf_sms_tpscts_t;

typedef struct smsf_sms_tpdcs_s {
        union {
        ED5(uint8_t tpCG:2;,
            uint8_t tpText:1;,
            uint8_t tpMC1:1;,
            uint8_t tpCharSet:2;,
            uint8_t tpMC2:2;)
        uint8_t octet;
        };
} __attribute__ ((packed)) smsf_sms_tpdcs_t;

typedef struct smsf_sms_tpdu_submit_s {
        union {
        struct {
        ED6(uint8_t tpRP:1;,
            uint8_t tpUDHI:1;,
            uint8_t tpSRR:1;,
            uint8_t tpVPF:2;,
            uint8_t tpRD:1;,
            uint8_t tpMTI:2;)
        };
        uint8_t octet;
        } header;
        uint8_t tpMR;
        smsf_sms_tp_address_t tp_destination_address;
        uint8_t tpPID;
        smsf_sms_tpdcs_t tpDCS;
        uint8_t tpUDL;
        uint8_t tpUD[140];
} __attribute__ ((packed)) smsf_sms_tpdu_submit_t;

typedef struct smsf_sms_tpdu_deliver_s {
        union {
        struct {
        ED6(uint8_t tpRP:1;,
            uint8_t tpUDHI:1;,
            uint8_t tpSRI:1;,
            uint8_t tpReserved:2;,
            uint8_t tpMMS:1;,
            uint8_t tpMTI:2;)
        };
        uint8_t octet;
        } header;
        smsf_sms_tp_address_t tp_originator_address;
        uint8_t tpPID;
        smsf_sms_tpdcs_t tpDCS;
        smsf_sms_tpscts_t tpSCTS;
        uint8_t tpUDL;
        uint8_t tpUD[140];
} __attribute__ ((packed)) smsf_sms_tpdu_deliver_t;

/* ADD ms2net kinda prefixes here */
#define SMSF_SERVICE_MESSAGE_TYPE_CP_DATA            1
#define SMSF_SERVICE_MESSAGE_TYPE_CP_ACK             4
#define SMSF_SERVICE_MESSAGE_TYPE_CP_ERROR           16

#define SMSF_RP_MESSAGE_TYPE_MS2N_DATA               0
#define SMSF_RP_MESSAGE_TYPE_MS2N_ACK                2
#define SMSF_RP_MESSAGE_TYPE_MS2N_ERROR              4
#define SMSF_RP_MESSAGE_TYPE_MS2N_SMMA               6

#define SMSF_RP_MESSAGE_TYPE_N2MS_DATA               1
#define SMSF_RP_MESSAGE_TYPE_N2MS_ACK                3
#define SMSF_RP_MESSAGE_TYPE_N2MS_ERROR              5

#define SMSF_TPDU_MTI_SMS_DELIVER                    0
#define SMSF_TPDU_MTI_SMS_SUBMIT                     1
#define SMSF_TPDU_MTI_SMS_COMMAND                    2

#define SMSF_PROTOCOL_DISCRIMINATOR_SMS             0x9

ogs_pkbuf_t *smsf_sms_encode_cp_ack(bool ti_flag, int ti_o);

ogs_pkbuf_t *smsf_sms_encode_cp_data(bool ti_flag, int ti_o, 
        const ogs_pkbuf_t *rpdu);

ogs_pkbuf_t *smsf_sms_encode_n2ms_rp_data(const smsf_sms_rpdata_t *rpdu,
        const smsf_sms_tpdu_deliver_t *tpdu);

ogs_pkbuf_t *smsf_sms_encode_n2ms_rp_ack(int rp_message_reference);

ogs_pkbuf_t *smsf_sms_encode_n2ms_rp_error(int rp_message_reference,
        int rp_cause);

int smsf_sms_get_user_data_byte_length(smsf_sms_tpdcs_t data_coding_scheme,
    int user_data_length);

void smsf_sms_set_sc_timestamp(smsf_sms_tpscts_t *timestamp);

void smsf_copy_submit_to_deliver(smsf_sms_tpdu_deliver_t *tpduDeliver,
        const smsf_sms_tpdu_submit_t *tpdu_submit,
        const smsf_ue_t *mt_smsf_ue, const smsf_ue_t *smsf_ue);

void smsf_copy_rp_address(smsf_rpdu_address_t *destination,
        const smsf_rpdu_address_t *source);

ogs_pkbuf_t *smsf_send_to_internal_smsc(smsf_ue_t *smsf_ue,
        ogs_sbi_stream_t *stream, ogs_pkbuf_t *sms_payload_buf);
void smsf_send_rpdu(smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream,
        ogs_pkbuf_t *rpdubuf);

#ifdef __cplusplus
}
#endif

#endif /* SMSF_SMS_H */
