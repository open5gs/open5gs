/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "sgsap-types.h"
#include "sgsap-build.h"
#include "sgsap-path.h"
#include "sgsap-handler.h"

#include "mme-sm.h"
#include "mme-context.h"
#include "mme-path.h"
#include "nas-path.h"
#include "s1ap-path.h"

void sgsap_handle_location_update_accept(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf)
{
    ogs_tlv_t *root = NULL, *iter = NULL;
    mme_ue_t *mme_ue = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi = NULL;
    int nas_mobile_identity_imsi_len = 0;
    ogs_nas_lai_t *lai = NULL;
    ogs_nas_mobile_identity_tmsi_t *nas_mobile_identity_tmsi = NULL;

    ogs_assert(vlr);
    ogs_assert(pkbuf);

    ogs_debug("[SGSAP] LOCATION-UPDATE-ACCEPT");

    ogs_pkbuf_pull(pkbuf, 1);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, OGS_TLV_MODE_T1_L1);
    if (!root)
        goto error;

    iter = root;
    while (iter) {
        switch (iter->type) {
        case SGSAP_IE_IMSI_TYPE:
            nas_mobile_identity_imsi = iter->value;
            nas_mobile_identity_imsi_len = iter->length;
            break;
        case SGSAP_IE_LAI_TYPE:
            lai = iter->value;
            break;
        case SGSAP_IE_MOBILE_IDENTITY_TYPE:
            nas_mobile_identity_tmsi = iter->value;
            break;
        default:
            ogs_warn("Invalid Type [%d]", iter->type);
            break;
        }
        iter = iter->next;
    }

    ogs_tlv_free_all(root);

    if (!nas_mobile_identity_imsi || !lai)
        goto error;
    if (nas_mobile_identity_imsi_len != SGSAP_IE_IMSI_LEN)
        goto error;

    if (nas_mobile_identity_imsi->type == OGS_NAS_MOBILE_IDENTITY_IMSI) {
        ogs_nas_eps_imsi_to_bcd(nas_mobile_identity_imsi,
                nas_mobile_identity_imsi_len, imsi_bcd);
        mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
    } else
        goto error;

    if (!mme_ue)
        goto error;

    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
    if (lai) {
        ogs_debug("    LAI[PLMN_ID:%06x,LAC:%d]",
                    ogs_plmn_id_hexdump(&lai->nas_plmn_id), lai->lac);
    }

    if (nas_mobile_identity_tmsi) {
        if (nas_mobile_identity_tmsi->type == OGS_NAS_MOBILE_IDENTITY_TMSI) {
            mme_ue->p_tmsi = be32toh(nas_mobile_identity_tmsi->tmsi);
        } else {
            ogs_error("Not supported Identity type[%d]",
                    nas_mobile_identity_tmsi->type);
            goto error;
        }
        ogs_debug("    P-TMSI[0x%08x]", mme_ue->p_tmsi);
    }

    nas_eps_send_attach_accept(mme_ue);

    return;

error:
    nas_eps_send_attach_reject(mme_ue,
            EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED,
            ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
    mme_send_delete_session_or_mme_ue_context_release(mme_ue);
}

void sgsap_handle_location_update_reject(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf)
{
    ogs_tlv_t *root = NULL, *iter = NULL;
    mme_ue_t *mme_ue = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi = NULL;
    int nas_mobile_identity_imsi_len = 0;
    ogs_nas_emm_cause_t emm_cause = 0;
    ogs_nas_lai_t *lai = NULL;

    ogs_assert(vlr);
    ogs_assert(pkbuf);

    ogs_warn("[SGSAP] LOCATION-UPDATE-REJECT");

    ogs_pkbuf_pull(pkbuf, 1);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, OGS_TLV_MODE_T1_L1);
    if (!root)
        goto error;

    iter = root;
    while (iter) {
        switch (iter->type) {
        case SGSAP_IE_IMSI_TYPE:
            nas_mobile_identity_imsi = iter->value;
            nas_mobile_identity_imsi_len = iter->length;
            break;
        case SGSAP_IE_LAI_TYPE:
            lai = iter->value;
            break;
        case SGSAP_IE_REJECT_CAUSE_TYPE:
            emm_cause = *((uint8_t*)(iter->value));
            break;
        default:
            ogs_warn("Invalid Type [%d]", iter->type);
            break;
        }
        iter = iter->next;
    }

    ogs_tlv_free_all(root);

    if (!nas_mobile_identity_imsi || !emm_cause)
        goto error;
    if (nas_mobile_identity_imsi_len != SGSAP_IE_IMSI_LEN)
        goto error;

    if (nas_mobile_identity_imsi->type == OGS_NAS_MOBILE_IDENTITY_IMSI) {
        ogs_nas_eps_imsi_to_bcd(nas_mobile_identity_imsi,
                nas_mobile_identity_imsi_len, imsi_bcd);
        mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
    } else
        goto error;

    ogs_expect_or_return(mme_ue);

    ogs_debug("    IMSI[%s] CAUSE[%d]", mme_ue->imsi_bcd, emm_cause);
    if (lai) {
        ogs_debug("    LAI[PLMN_ID:%06x,LAC:%d]",
                    ogs_plmn_id_hexdump(&lai->nas_plmn_id), lai->lac);
    }

    nas_eps_send_attach_reject(mme_ue,
            emm_cause, ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
    mme_send_delete_session_or_mme_ue_context_release(mme_ue);

    return;
    
error:
    ogs_error("Error processing SGsAP LU REJECT");
    return;
}

void sgsap_handle_detach_ack(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf)
{
    ogs_tlv_t *root = NULL, *iter = NULL;
    mme_ue_t *mme_ue = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    uint8_t type = 0;
    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi = NULL;
    int nas_mobile_identity_imsi_len = 0;

    ogs_assert(vlr);
    ogs_assert(pkbuf);

    type = *(unsigned char *)(pkbuf->data);
    if (type == SGSAP_EPS_DETACH_ACK)
        ogs_debug("[SGSAP] EPS-DETACH-ACK");
    else if (type == SGSAP_IMSI_DETACH_ACK)
        ogs_debug("[SGSAP] IMSI-DETACH-ACK");
    else
        ogs_expect_or_return(0);

    ogs_pkbuf_pull(pkbuf, 1);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, OGS_TLV_MODE_T1_L1);
    ogs_expect_or_return(root);

    iter = root;
    while (iter) {
        switch (iter->type) {
        case SGSAP_IE_IMSI_TYPE:
            nas_mobile_identity_imsi = iter->value;
            nas_mobile_identity_imsi_len = iter->length;
            break;
        default:
            ogs_warn("Invalid Type [%d]", iter->type);
            break;
        }
        iter = iter->next;
    }

    ogs_tlv_free_all(root);

    ogs_expect_or_return(nas_mobile_identity_imsi);
    ogs_expect_or_return(nas_mobile_identity_imsi_len == SGSAP_IE_IMSI_LEN);

    if (nas_mobile_identity_imsi->type == OGS_NAS_MOBILE_IDENTITY_IMSI) {
        ogs_nas_eps_imsi_to_bcd(nas_mobile_identity_imsi,
                nas_mobile_identity_imsi_len, imsi_bcd);
        mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
    } else
        ogs_expect_or_return(0);

    ogs_expect_or_return(mme_ue);

    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    mme_send_delete_session_or_detach(mme_ue);
}

void sgsap_handle_paging_request(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf)
{
    ogs_tlv_t *root = NULL, *iter = NULL;
    mme_ue_t *mme_ue = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi = NULL;
    int nas_mobile_identity_imsi_len = 0;
    ogs_nas_lai_t *lai = NULL;
    char vlr_name[SGSAP_IE_VLR_NAME_LEN] = { 0, };
    uint8_t service_indicator = 0;

    ogs_assert(vlr);
    ogs_assert(pkbuf);

    ogs_debug("[SGSAP] PAGING-REQUEST");

    ogs_pkbuf_pull(pkbuf, 1);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, OGS_TLV_MODE_T1_L1);
    ogs_expect_or_return(root);

    iter = root;
    while (iter) {
        switch (iter->type) {
        case SGSAP_IE_IMSI_TYPE:
            nas_mobile_identity_imsi = iter->value;
            nas_mobile_identity_imsi_len = iter->length;
            break;
        case SGSAP_IE_VLR_NAME_TYPE:
            ogs_fqdn_parse(vlr_name, iter->value, iter->length);
            break;
        case SGSAP_IE_LAI_TYPE:
            lai = iter->value;
            break;
        case SGSAP_IE_SERVICE_INDICATOR_TYPE:
            service_indicator = *((uint8_t*)(iter->value));
            break;
        default:
            ogs_warn("Invalid Type [%d]", iter->type);
            break;
        }
        iter = iter->next;
    }

    ogs_tlv_free_all(root);

    ogs_expect_or_return(nas_mobile_identity_imsi);
    ogs_expect_or_return(nas_mobile_identity_imsi_len == SGSAP_IE_IMSI_LEN);

    if (nas_mobile_identity_imsi->type == OGS_NAS_MOBILE_IDENTITY_IMSI) {

        ogs_nas_eps_imsi_to_bcd(nas_mobile_identity_imsi,
                nas_mobile_identity_imsi_len, imsi_bcd);
        mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
    } else 
        ogs_expect_or_return(0);

    if (mme_ue) {
        ogs_assert(service_indicator);
        mme_ue->service_indicator = service_indicator;

        ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
        ogs_debug("    VLR_NAME[%s]", vlr_name);
        ogs_debug("    SERVICE_INDICATOR[%d]", mme_ue->service_indicator);

        if (lai) {
            ogs_debug("    LAI[PLMN_ID:%06x,LAC:%d]",
                        ogs_plmn_id_hexdump(&lai->nas_plmn_id), lai->lac);
        }

        if (ECM_IDLE(mme_ue)) {
            if (CS_CALL_SERVICE_INDICATOR(mme_ue)) {
                /* UE will respond Extended Service Request in PS CNDomain*/
                s1ap_send_paging(mme_ue, S1AP_CNDomain_cs);

            } else if (SMS_SERVICE_INDICATOR(mme_ue)) {
                /* UE will respond Service Request in PS CNDomain*/
                s1ap_send_paging(mme_ue, S1AP_CNDomain_ps);
            } else
                goto paging_reject;

        } else {
            if (CS_CALL_SERVICE_INDICATOR(mme_ue)) {
                nas_eps_send_cs_service_notification(mme_ue);
            } else if (SMS_SERVICE_INDICATOR(mme_ue)) {
                sgsap_send_service_request(mme_ue, SGSAP_EMM_CONNECTED_MODE);
            } else
                goto paging_reject;
        }

        return;
    }

paging_reject:
    ogs_debug("[SGSAP] PAGING-REJECT");
    ogs_debug("    IMSI[%s]", imsi_bcd);

    sgsap_send_to_vlr_with_sid(
        vlr, 
        sgsap_build_paging_reject(
            nas_mobile_identity_imsi, nas_mobile_identity_imsi_len,
            SGSAP_SGS_CAUSE_IMSI_UNKNOWN),
        0);
}

void sgsap_handle_downlink_unitdata(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf)
{
    ogs_tlv_t *root = NULL, *iter = NULL;
    mme_ue_t *mme_ue = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi = NULL;
    int nas_mobile_identity_imsi_len = 0;
    uint8_t *nas_message_container_buffer = NULL;
    uint8_t nas_message_container_length = 0;

    ogs_assert(vlr);
    ogs_assert(pkbuf);

    ogs_debug("[SGSAP] DOWNLINK-UNITDATA");

    ogs_pkbuf_pull(pkbuf, 1);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, OGS_TLV_MODE_T1_L1);
    ogs_assert(root);

    iter = root;
    while (iter) {
        switch (iter->type) {
        case SGSAP_IE_IMSI_TYPE:
            nas_mobile_identity_imsi = iter->value;
            nas_mobile_identity_imsi_len = iter->length;
            break;
        case SGSAP_IE_NAS_MESSAGE_CONTAINER_TYPE:
            nas_message_container_buffer = iter->value;
            nas_message_container_length = iter->length;
            break;
        default:
            ogs_warn("Invalid Type [%d]", iter->type);
            break;
        }
        iter = iter->next;
    }

    ogs_tlv_free_all(root);

    ogs_assert(nas_mobile_identity_imsi);
    ogs_assert(nas_mobile_identity_imsi_len == SGSAP_IE_IMSI_LEN);
    ogs_assert(nas_message_container_buffer);
    ogs_assert(nas_message_container_length);

    if (nas_mobile_identity_imsi->type == OGS_NAS_MOBILE_IDENTITY_IMSI) {

        ogs_nas_eps_imsi_to_bcd(nas_mobile_identity_imsi,
                nas_mobile_identity_imsi_len, imsi_bcd);
        mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
    } else 
        ogs_assert_if_reached();

    ogs_expect_or_return(mme_ue);

    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
    ogs_log_hexdump(OGS_LOG_DEBUG,
            nas_message_container_buffer,
            nas_message_container_length);

    nas_eps_send_downlink_nas_transport(mme_ue,
            nas_message_container_buffer, nas_message_container_length);
}

void sgsap_handle_reset_indication(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf)
{
    ogs_debug("[SGSAP] RESET-INDICATION");

    ogs_assert(vlr);
    ogs_assert(pkbuf);

    sgsap_send_reset_ack(vlr);
}

void sgsap_handle_release_request(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf)
{
    ogs_tlv_t *root = NULL, *iter = NULL;
    mme_ue_t *mme_ue = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi = NULL;
    int nas_mobile_identity_imsi_len = 0;

    ogs_assert(vlr);
    ogs_assert(pkbuf);

    ogs_debug("[SGSAP] RELEASE-REQUEST");

    ogs_pkbuf_pull(pkbuf, 1);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, OGS_TLV_MODE_T1_L1);
    ogs_expect_or_return(root);

    iter = root;
    while (iter) {
        switch (iter->type) {
        case SGSAP_IE_IMSI_TYPE:
            nas_mobile_identity_imsi = iter->value;
            nas_mobile_identity_imsi_len = iter->length;
            break;
        default:
            ogs_warn("Invalid Type [%d]", iter->type);
            break;
        }
        iter = iter->next;
    }

    ogs_tlv_free_all(root);

    ogs_expect_or_return(nas_mobile_identity_imsi);
    ogs_expect_or_return(nas_mobile_identity_imsi_len == SGSAP_IE_IMSI_LEN);

    if (nas_mobile_identity_imsi->type == OGS_NAS_MOBILE_IDENTITY_IMSI) {

        ogs_nas_eps_imsi_to_bcd(nas_mobile_identity_imsi,
                nas_mobile_identity_imsi_len, imsi_bcd);
        mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
    } else 
        ogs_expect_or_return(0);

    if (mme_ue)
        ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
    else
        ogs_warn("Unknown IMSI[%s]", imsi_bcd);

}

void sgsap_handle_mm_information_request(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf)
{
    ogs_tlv_t *root = NULL, *iter = NULL;
    mme_ue_t *mme_ue = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi = NULL;
    int nas_mobile_identity_imsi_len = 0;

    ogs_assert(vlr);
    ogs_assert(pkbuf);

    ogs_debug("[SGSAP] MM-INFORMATION-REQUEST(DISCARD by OPTION2)");

    ogs_pkbuf_pull(pkbuf, 1);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, OGS_TLV_MODE_T1_L1);
    ogs_expect_or_return(root);

    iter = root;
    while (iter) {
        switch (iter->type) {
        case SGSAP_IE_IMSI_TYPE:
            nas_mobile_identity_imsi = iter->value;
            nas_mobile_identity_imsi_len = iter->length;
            break;
        case SGSAP_IE_MM_INFORMATION_TYPE:
            /* TODO */
            break;
        default:
            ogs_warn("Invalid Type [%d]", iter->type);
            break;
        }
        iter = iter->next;
    }

    ogs_tlv_free_all(root);

    ogs_expect_or_return(nas_mobile_identity_imsi);
    ogs_expect_or_return(nas_mobile_identity_imsi_len == SGSAP_IE_IMSI_LEN);

    if (nas_mobile_identity_imsi->type == OGS_NAS_MOBILE_IDENTITY_IMSI) {

        ogs_nas_eps_imsi_to_bcd(nas_mobile_identity_imsi,
                nas_mobile_identity_imsi_len, imsi_bcd);
        mme_ue = mme_ue_find_by_imsi_bcd(imsi_bcd);
    } else 
        ogs_expect_or_return(0);

    if (mme_ue)
        ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
    else
        ogs_warn("Unknown IMSI[%s]", imsi_bcd);
}
