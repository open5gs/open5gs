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

#include "sms.h"

ogs_pkbuf_t *smsf_sms_encode_cp_ack(bool ti_flag, int ti_o)
{
#define CP_ACK_FIXED_LENGTH 2
    ogs_pkbuf_t *pkbuf = NULL;
    smsf_sms_cp_hdr_t cp_data_header;

    memset(&cp_data_header, 0, sizeof(smsf_sms_cp_hdr_t));

    cp_data_header.flags.pd = SMSF_PROTOCOL_DISCRIMINATOR_SMS;
    cp_data_header.flags.tio = ti_o;
    cp_data_header.flags.tif = ti_flag;
    cp_data_header.sm_service_message_type = SMSF_SERVICE_MESSAGE_TYPE_CP_ACK;

    pkbuf = ogs_pkbuf_alloc(NULL, CP_ACK_FIXED_LENGTH);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }

    ogs_pkbuf_put_u8(pkbuf,cp_data_header.flags.octet);
    ogs_pkbuf_put_u8(pkbuf,cp_data_header.sm_service_message_type);

    return pkbuf;
}

ogs_pkbuf_t *smsf_sms_encode_cp_data(bool ti_flag, int ti_o, 
        const ogs_pkbuf_t *rpdu)
{
#define CP_HEADER_FIXED_LENGTH 3
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_assert(rpdu);
    smsf_sms_cp_data_t cp_data;

    memset(&cp_data, 0, sizeof(smsf_sms_cp_data_t));

    cp_data.header.flags.pd = SMSF_PROTOCOL_DISCRIMINATOR_SMS;
    cp_data.header.flags.tio = ti_o;
    cp_data.header.flags.tif = ti_flag;
    cp_data.header.sm_service_message_type = SMSF_SERVICE_MESSAGE_TYPE_CP_DATA;
    cp_data.cp_user_data_length = rpdu->len;

    pkbuf = ogs_pkbuf_alloc(NULL, CP_HEADER_FIXED_LENGTH + rpdu->len);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }

    ogs_pkbuf_put_u8(pkbuf, cp_data.header.flags.octet);
    ogs_pkbuf_put_u8(pkbuf, cp_data.header.sm_service_message_type);
    ogs_pkbuf_put_u8(pkbuf, cp_data.cp_user_data_length);
    ogs_pkbuf_put_data(pkbuf, rpdu->data, rpdu->len);

    return pkbuf;
}

ogs_pkbuf_t *smsf_sms_encode_n2ms_rp_data(const smsf_sms_rpdata_t *rpdu,
        const smsf_sms_tpdu_deliver_t *tpdu)
{
#define TPDU_FIXED_LENGTH 13
#define RPDU_FIXED_LENGTH 6
    ogs_pkbuf_t *pkbuf = NULL;
    int tpdu_oa_real_length;
    int tpdu_ud_real_length;
    int tpdu_length;
    int rpdu_length;

    ogs_assert(rpdu);
    ogs_assert(tpdu);

    tpdu_ud_real_length = smsf_sms_get_user_data_byte_length(tpdu->tpDCS,
            tpdu->tpUDL);
    tpdu_oa_real_length = ((tpdu->tp_originator_address.addr_length + 1) /2);
    tpdu_length = TPDU_FIXED_LENGTH + tpdu_ud_real_length + tpdu_oa_real_length;
    rpdu_length = RPDU_FIXED_LENGTH + rpdu->rp_originator_address.length +
            tpdu_length;

    pkbuf = ogs_pkbuf_alloc(NULL, rpdu_length);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }

    ogs_pkbuf_put_u8(pkbuf, rpdu->rpdu_message_type.octet);
    ogs_pkbuf_put_u8(pkbuf, rpdu->rp_message_reference);
    ogs_pkbuf_put_u8(pkbuf, rpdu->rp_originator_address.length);
    ogs_pkbuf_put_u8(pkbuf, rpdu->rp_originator_address.header.octet);
    ogs_pkbuf_put_data(pkbuf, rpdu->rp_originator_address.rp_address,
            rpdu->rp_originator_address.length - 1);
    ogs_pkbuf_put_u8(pkbuf, 0); // RP-DA Address Length

    ogs_pkbuf_put_u8(pkbuf, tpdu_length);

    ogs_pkbuf_put_u8(pkbuf, tpdu->header.octet);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tp_originator_address.addr_length);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tp_originator_address.header.octet);
    ogs_pkbuf_put_data(pkbuf, tpdu->tp_originator_address.tp_address,
            tpdu_oa_real_length);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpPID);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpDCS.octet);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpSCTS.year);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpSCTS.month);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpSCTS.day);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpSCTS.hour);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpSCTS.minute);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpSCTS.second);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpSCTS.timezone);
    ogs_pkbuf_put_u8(pkbuf, tpdu->tpUDL);
    ogs_pkbuf_put_data(pkbuf, &tpdu->tpUD, tpdu_ud_real_length);

    return pkbuf;
}

ogs_pkbuf_t *smsf_sms_encode_n2ms_rp_ack(int rp_message_reference)
{
#define RP_ACK_FIXED_LENGTH 4
    ogs_pkbuf_t *pkbuf = NULL;

    pkbuf = ogs_pkbuf_alloc(NULL, RP_ACK_FIXED_LENGTH);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }

    ogs_pkbuf_put_u8(pkbuf, SMSF_RP_MESSAGE_TYPE_N2MS_ACK);
    ogs_pkbuf_put_u8(pkbuf, rp_message_reference);
    ogs_pkbuf_put_u8(pkbuf, 65); // Element ID 0x41
    ogs_pkbuf_put_u8(pkbuf, 0); // Length: 0

    return pkbuf;
}

ogs_pkbuf_t *smsf_sms_encode_n2ms_rp_error(int rp_message_reference,
        int rp_cause)
{
#define RP_ERROR_FIXED_LENGTH 6
    ogs_pkbuf_t *pkbuf = NULL;

    pkbuf = ogs_pkbuf_alloc(NULL, RP_ERROR_FIXED_LENGTH);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }

    ogs_pkbuf_put_u8(pkbuf, SMSF_RP_MESSAGE_TYPE_N2MS_ERROR);
    ogs_pkbuf_put_u8(pkbuf, rp_message_reference);
    ogs_pkbuf_put_u8(pkbuf, 1); // RP-Cause Length: 1
    ogs_pkbuf_put_u8(pkbuf, rp_cause);
    ogs_pkbuf_put_u8(pkbuf, 65); // Element ID 0x41
    ogs_pkbuf_put_u8(pkbuf, 0); // Length: 0

    return pkbuf;
}

int smsf_sms_get_user_data_byte_length(smsf_sms_tpdcs_t data_coding_scheme,
    int user_data_length)
{
    int user_data_bytes;

    if (data_coding_scheme.octet == 0) {
        user_data_bytes = ((user_data_length + 1) * 7/8);
    } else {
       user_data_bytes = user_data_length;
    }

    return user_data_bytes;
}

void smsf_sms_set_sc_timestamp(smsf_sms_tpscts_t *sc_timestamp)
{
    struct timeval tv;
    struct tm local;
    int local_time_zone;

    ogs_gettimeofday(&tv);
    ogs_localtime(tv.tv_sec, &local);

    if (local.tm_gmtoff >= 0) {
        local_time_zone = OGS_SCTS_TIME_TO_BCD(local.tm_gmtoff / 900);
    } else {
        local_time_zone = OGS_SCTS_TIME_TO_BCD((-local.tm_gmtoff) / 900);
        local_time_zone |= 0x08;
    }

    sc_timestamp->year = OGS_SCTS_TIME_TO_BCD(local.tm_year % 100);
    sc_timestamp->month = OGS_SCTS_TIME_TO_BCD(local.tm_mon+1);
    sc_timestamp->day = OGS_SCTS_TIME_TO_BCD(local.tm_mday);
    sc_timestamp->hour = OGS_SCTS_TIME_TO_BCD(local.tm_hour);
    sc_timestamp->minute = OGS_SCTS_TIME_TO_BCD(local.tm_min);
    sc_timestamp->second = OGS_SCTS_TIME_TO_BCD(local.tm_sec);
    sc_timestamp->timezone = local_time_zone;
}

void smsf_copy_submit_to_deliver(smsf_sms_tpdu_deliver_t *tpdu_deliver,
        const smsf_sms_tpdu_submit_t *tpdu_submit,
        const smsf_ue_t *mt_smsf_ue, const smsf_ue_t *smsf_ue)
{
    ogs_assert(tpdu_deliver);
    ogs_assert(tpdu_submit);
    ogs_assert(mt_smsf_ue);
    ogs_assert(smsf_ue);

    int tpdurealbytes;

    tpdu_deliver->header.tpUDHI = tpdu_submit->header.tpUDHI;
    tpdu_deliver->header.tpMMS = 1;

    /* Populate the Sender's MSISDN */
    char *oa_msisdn;
    if (strncmp(smsf_ue->gpsi, OGS_ID_GPSI_TYPE_MSISDN,
            strlen(OGS_ID_GPSI_TYPE_MSISDN)) == 0) {
        oa_msisdn = ogs_id_get_value(smsf_ue->gpsi);
        ogs_assert(oa_msisdn);
    } else {
        ogs_error("SMS-MO without MSISDN");
    }

    char *oa_msisdn_bcd;
    oa_msisdn_bcd = ogs_calloc(1, OGS_MAX_MSISDN_BCD_LEN+1);
    int oa_msisdn_bcd_len;
    ogs_bcd_to_buffer(oa_msisdn, oa_msisdn_bcd,
            &oa_msisdn_bcd_len);
    tpdu_deliver->tp_originator_address.addr_length =
            strlen(oa_msisdn);
    tpdu_deliver->tp_originator_address.header.ext = 1;
    tpdu_deliver->tp_originator_address.header.ton = 1;
    tpdu_deliver->tp_originator_address.header.npi = 1;
    memcpy(&tpdu_deliver->tp_originator_address.tp_address,
            oa_msisdn_bcd, oa_msisdn_bcd_len);

    if (oa_msisdn)
        ogs_free(oa_msisdn);
    if (oa_msisdn_bcd)
        ogs_free(oa_msisdn_bcd);

    tpdu_deliver->tpPID = tpdu_submit->tpPID;
    tpdu_deliver->tpDCS.octet = tpdu_submit->tpDCS.octet;
    smsf_sms_set_sc_timestamp(&tpdu_deliver->tpSCTS);
    tpdu_deliver->tpUDL = tpdu_submit->tpUDL;

    tpdurealbytes = smsf_sms_get_user_data_byte_length(
            tpdu_submit->tpDCS, tpdu_submit->tpUDL);

    memcpy(&tpdu_deliver->tpUD, &tpdu_submit->tpUD, tpdurealbytes);
}

void smsf_copy_rp_address(smsf_rpdu_address_t *destination,
        const smsf_rpdu_address_t *source)
{
    ogs_assert(source);
    ogs_assert(destination);

    destination->length = source->length;
    destination->header.octet = source->header.octet;
    memcpy(&destination->rp_address, &source->rp_address, source->length);
}

ogs_pkbuf_t *smsf_send_to_internal_smsc(smsf_ue_t *smsf_ue,
        ogs_sbi_stream_t *stream, ogs_pkbuf_t *sms_payload_buf)
{
    int templen = 0;

    ogs_assert(smsf_ue);
    ogs_assert(stream);
    ogs_assert(sms_payload_buf);

    smsf_sms_rpdu_message_type_t *rp_header;
    rp_header = (smsf_sms_rpdu_message_type_t *)sms_payload_buf->data;
    ogs_assert(rp_header);

    ogs_pkbuf_pull(sms_payload_buf, sizeof(smsf_sms_rpdu_message_type_t));

    switch(rp_header->value) {
    case SMSF_RP_MESSAGE_TYPE_MS2N_DATA:
        ogs_debug("[%s] RP-DATA (ms->n)", smsf_ue->supi);
        smsf_sms_rpdata_t rpdu;
        memset(&rpdu, 0, sizeof(smsf_sms_rpdata_t));

        rpdu.rpdu_message_type.value = rp_header->value;
        memcpy(&rpdu.rp_message_reference, sms_payload_buf->data, 1);
        ogs_pkbuf_pull(sms_payload_buf, sizeof(rpdu.rp_message_reference));
        memcpy(&templen, sms_payload_buf->data, 1);
        if (templen)
            ogs_error("OA Length Invalid");

        ogs_pkbuf_pull(sms_payload_buf, 1);
        memcpy(&templen, sms_payload_buf->data, 1);
        if (!templen)
            ogs_error("DA Length Invalid");

        memcpy(&rpdu.rp_destination_address,
                sms_payload_buf->data, templen + 1);
        ogs_pkbuf_pull(sms_payload_buf, templen + 1);

        memcpy(&rpdu.rp_user_data_length, sms_payload_buf->data, 1);
        ogs_pkbuf_pull(sms_payload_buf, 1);

        /* RP Decoding complete.  Capture the TPDU now. */
        smsf_sms_tpdu_hdr_t *tpdu_hdr;
        tpdu_hdr = (smsf_sms_tpdu_hdr_t *)sms_payload_buf->data;
        ogs_assert(tpdu_hdr);

        switch(tpdu_hdr->tpMTI) {
        case SMSF_TPDU_MTI_SMS_DELIVER:
            ogs_debug("[%s] SMS-DELIVER Report (ms->n)", smsf_ue->supi);
            break;

        case SMSF_TPDU_MTI_SMS_SUBMIT:
            ogs_debug("[%s] SMS-SUBMIT (ms->n)", smsf_ue->supi);

            smsf_sms_tpdu_submit_t tpdu_submit;
            memset(&tpdu_submit, 0, sizeof(smsf_sms_tpdu_submit_t));
            memcpy(&tpdu_submit, sms_payload_buf->data, 2);
            ogs_pkbuf_pull(sms_payload_buf, 2);
            memcpy(&templen, sms_payload_buf->data, 1);
            memcpy(&tpdu_submit.tp_destination_address,
                    sms_payload_buf->data, 2 + ((templen + 1) / 2));
            /* Copy Address Length, Address Type, and Address Itself */
            ogs_pkbuf_pull(sms_payload_buf, 2 + ((templen + 1) / 2));
            /* Copy PID, DCS, and UDL */
            memcpy(&tpdu_submit.tpPID, sms_payload_buf->data, 3);
            ogs_pkbuf_pull(sms_payload_buf, 3);

            int tpdurealbytes;
            tpdurealbytes = smsf_sms_get_user_data_byte_length(
                    tpdu_submit.tpDCS, tpdu_submit.tpUDL);

            memcpy(&tpdu_submit.tpUD, sms_payload_buf->data, tpdurealbytes);

            /* Begin looking for our TPDU destination smsf_ue */
            smsf_sms_tp_address_t tp_da;
            char *output_bcd;
            memset(&tp_da, 0, sizeof(smsf_sms_tp_address_t));
            tp_da = tpdu_submit.tp_destination_address;
            output_bcd = ogs_calloc(1, OGS_MAX_MSISDN_BCD_LEN+1);
            ogs_buffer_to_bcd(tp_da.tp_address, (tp_da.addr_length + 1) /2,
                    output_bcd);

            /* Look for the MT MSISDN */
            smsf_ue_t *mt_smsf_ue = NULL;
            char *mt_gpsi = ogs_msprintf("%s-%s", OGS_ID_GPSI_TYPE_MSISDN,
                    output_bcd);
            ogs_debug("[%s] Looking for [%s]", smsf_ue->supi, mt_gpsi);
            mt_smsf_ue = smsf_ue_find_by_gpsi(mt_gpsi);
            if (!mt_smsf_ue)
                ogs_error("[%s] No context with GPSI[%s]",
                        smsf_ue->supi, mt_gpsi);

            if (mt_gpsi)
                ogs_free(mt_gpsi);
            if (output_bcd)
                ogs_free(output_bcd);

            if (!smsf_ue->mo_sms_subscribed) {
                /* Could also 403 the send-sms */
                ogs_error("[%s] Not subscribed for MO-SMS",
                        smsf_ue->supi);

                ogs_debug("[%s] Sending RP-ERROR", smsf_ue->supi);
                ogs_pkbuf_t *rpdubuf;
                // RP-Cause: Requested facility not subscribed (50)
                rpdubuf = smsf_sms_encode_n2ms_rp_error(
                        rpdu.rp_message_reference, 50);
                return rpdubuf;
                break;
            }
            
            if (mt_smsf_ue) {
                ogs_debug("[%s] Sending CP-DATA", mt_smsf_ue->supi);

                if (mt_smsf_ue->mt_sms_subscribed) {
                    smsf_sms_tpdu_deliver_t tpduDeliver;
                    memset(&tpduDeliver, 0,
                            sizeof(smsf_sms_tpdu_deliver_t));
                    smsf_copy_submit_to_deliver(&tpduDeliver, &tpdu_submit,
                            mt_smsf_ue, smsf_ue);

                    smsf_sms_increment_tio(smsf_ue);
                    smsf_sms_increment_message_reference(smsf_ue);

                    smsf_sms_rpdata_t rpduDeliver;
                    memset(&rpduDeliver, 0, sizeof(smsf_sms_rpdata_t));
                    rpduDeliver.rpdu_message_type.value =
                            SMSF_RP_MESSAGE_TYPE_N2MS_DATA;
                    rpduDeliver.rp_message_reference =
                            mt_smsf_ue->mt_message_reference;
                    smsf_copy_rp_address(&rpduDeliver.rp_originator_address,
                            &rpdu.rp_destination_address);

                    ogs_pkbuf_t *rpdubuf;
                    rpdubuf = smsf_sms_encode_n2ms_rp_data(&rpduDeliver,
                            &tpduDeliver);
                    smsf_send_rpdu(mt_smsf_ue, stream, rpdubuf);
                } else {
                    ogs_error("[%s] Not subscribed for MT-SMS",
                            mt_smsf_ue->supi);
                }
            }

            ogs_debug("[%s] Sending RP-Ack", smsf_ue->supi);
            ogs_pkbuf_t *rpdubuf;
            rpdubuf = smsf_sms_encode_n2ms_rp_ack(rpdu.rp_message_reference);
            return rpdubuf;
            break;

        case SMSF_TPDU_MTI_SMS_COMMAND:
            ogs_debug("[%s] SMS-COMMAND (ms->n)", smsf_ue->supi);
            break;

        default:
            ogs_error("[%s] Undefined TPDU Message Type for ms->n [%d]",
                    smsf_ue->supi, tpdu_hdr->tpMTI);

            return NULL;
        }
        break;

    case SMSF_RP_MESSAGE_TYPE_MS2N_ACK:
        ogs_debug("[%s] RP-ACK (ms->n)", smsf_ue->supi);
        break;

    case SMSF_RP_MESSAGE_TYPE_MS2N_ERROR:
        ogs_debug("[%s] RP-ERROR (ms->n)", smsf_ue->supi);
        break;

    case SMSF_RP_MESSAGE_TYPE_MS2N_SMMA:
        ogs_debug("[%s] RP-SMMA (ms->n)", smsf_ue->supi);
        break;

    default:
        ogs_error("[%s] Undefined RPDU Message Type for ms->n [%d]",
                smsf_ue->supi, rp_header->value);
        return NULL;
    }

    return NULL;
}

void smsf_send_rpdu(smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream,
        ogs_pkbuf_t *rpdubuf)
{
    smsf_n1_n2_message_transfer_param_t param;

    ogs_assert(smsf_ue);
    ogs_assert(rpdubuf);

    memset(&param, 0, sizeof(param));

    param.n1smbuf = smsf_sms_encode_cp_data(false,
            smsf_ue->mt_tio, rpdubuf);
    ogs_assert(param.n1smbuf);
    smsf_namf_comm_send_n1_n2_message_transfer(smsf_ue,
            stream, &param);
}
