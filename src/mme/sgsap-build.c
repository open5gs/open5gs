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

#include "mme-context.h"

#include "sgsap-types.h"
#include "sgsap-build.h"
#include "sgsap-conv.h"

ogs_pkbuf_t *sgsap_build_location_update_request(mme_ue_t *mme_ue)
{
    mme_csmap_t *csmap = NULL;
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    char mme_name[SGSAP_IE_MME_NAME_LEN+1];
    int mme_name_len = 0;
    served_gummei_t *served_gummei = &mme_self()->served_gummei[0];
    char eps_update_type;
    ogs_nas_lai_t lai;

    ogs_assert(mme_ue);
    csmap = mme_ue->csmap;
    ogs_assert(csmap);
    vlr = csmap->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    mme_name_len = mme_name_build(mme_name,
            served_gummei->mme_code[0],
            served_gummei->mme_gid[0],
            &served_gummei->plmn_id[0]);
    ogs_tlv_add(root, SGSAP_IE_MME_NAME_TYPE, mme_name_len, 0, mme_name);
    eps_update_type = SGSAP_EPS_UPDATE_IMSI_ATTACH;
    ogs_tlv_add(root, SGSAP_IE_EPS_UPDATE_TYPE, SGSAP_IE_EPS_UPDATE_LEN, 0,
            &eps_update_type);
    memcpy(&lai, &csmap->lai, sizeof(ogs_nas_lai_t));
    lai.lac = htobe16(lai.lac);
    ogs_tlv_add(root, SGSAP_IE_LAI_TYPE, SGSAP_IE_LAI_LEN, 0, &lai);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_LOCATION_UPDATE_REQUEST);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}
ogs_pkbuf_t *sgsap_build_tmsi_reallocation_complete(mme_ue_t *mme_ue)
{
    mme_csmap_t *csmap = NULL;
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(mme_ue);
    csmap = mme_ue->csmap;
    ogs_assert(csmap);
    vlr = csmap->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_TMSI_REALLOCATION_COMPLETE);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_detach_indication(mme_ue_t *mme_ue)
{
    mme_csmap_t *csmap = NULL;
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    char mme_name[SGSAP_IE_MME_NAME_LEN+1];
    int mme_name_len = 0;
    served_gummei_t *served_gummei = &mme_self()->served_gummei[0];
    uint8_t type = SGSAP_EPS_DETACH_INDICATION;
    uint8_t indication = SGSAP_EPS_DETACH_UE_INITIATED;

    ogs_assert(mme_ue);
    csmap = mme_ue->csmap;
    ogs_assert(csmap);
    vlr = csmap->vlr;
    ogs_assert(vlr);

    switch (mme_ue->nas_eps.detach.value) {
    /* 0 0 1 : EPS detach */
    case OGS_NAS_DETACH_TYPE_FROM_UE_EPS_DETACH: 
        type = SGSAP_EPS_DETACH_INDICATION;
        indication = SGSAP_EPS_DETACH_UE_INITIATED;
        break;
    /* 0 1 0 : IMSI detach */
    case OGS_NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH: 
        type = SGSAP_IMSI_DETACH_INDICATION;
        indication = SGSAP_IMSI_DETACH_EXPLICIT_UE_INITIATED;
        break;
    case 6: /* 1 1 0 : reserved */
    case 7: /* 1 1 1 : reserved */
        ogs_warn("Unknown Detach type[%d]", mme_ue->nas_eps.detach.value);
        break;
    /* 0 1 1 : combined EPS/IMSI detach */
    case OGS_NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH: 
        type = SGSAP_IMSI_DETACH_INDICATION;
        indication = SGSAP_IMSI_DETACH_COMBINED_UE_INITIATED;
    default: /* all other values */
        break;
    }

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    mme_name_len = mme_name_build(mme_name,
            served_gummei->mme_code[0],
            served_gummei->mme_gid[0],
            &served_gummei->plmn_id[0]);
    ogs_tlv_add(root, SGSAP_IE_MME_NAME_TYPE, mme_name_len, 0, mme_name);
    if (type == SGSAP_EPS_DETACH_INDICATION) {
        ogs_debug("[SGSAP] EPS-DETACH-INDICATION");
        ogs_tlv_add(root, SGSAP_IE_EPS_DETACH_INDICATION_TYPE,
                SGSAP_IE_EPS_DETACH_INDICATION_LEN, 0, &indication);
    } else if (type == SGSAP_IMSI_DETACH_INDICATION) {
        ogs_debug("[SGSAP] IMSI-DETACH-INDICATION");
        ogs_tlv_add(root, SGSAP_IE_IMSI_DETACH_INDICATION_TYPE,
                SGSAP_IE_IMSI_DETACH_INDICATION_LEN, 0, &indication);
    } else
        ogs_assert_if_reached();

    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);
    ogs_debug("    INDICATION[%d]", indication);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, type);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_mo_csfb_indication(mme_ue_t *mme_ue)
{
    mme_csmap_t *csmap = NULL;
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(mme_ue);
    csmap = mme_ue->csmap;
    ogs_assert(csmap);
    vlr = csmap->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_MO_CSFB_INDICIATION);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_paging_reject(
    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi,
    int nas_mobile_identity_imsi_len, uint8_t sgs_cause)
{
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(nas_mobile_identity_imsi);
    ogs_assert(nas_mobile_identity_imsi_len == SGSAP_IE_IMSI_LEN);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            nas_mobile_identity_imsi);
    sgs_cause = SGSAP_SGS_CAUSE_IMSI_UNKNOWN;
    ogs_tlv_add(root, SGSAP_IE_SGS_CAUSE_TYPE,
            SGSAP_IE_SGS_CAUSE_LEN, 0, &sgs_cause);

    ogs_debug("    CAUSE[%d]", sgs_cause);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_PAGING_REJECT);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_service_request(mme_ue_t *mme_ue, uint8_t emm_mode)
{
    mme_csmap_t *csmap = NULL;
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(mme_ue);
    csmap = mme_ue->csmap;
    ogs_assert(csmap);
    vlr = csmap->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    ogs_tlv_add(root, SGSAP_IE_SERVICE_INDICATOR_TYPE,
            SGSAP_IE_SERVICE_INDICATOR_LEN, 0, &mme_ue->service_indicator);

    ogs_tlv_add(root, SGSAP_IE_UE_EMM_MODE_TYPE,
            SGSAP_IE_UE_EMM_MODE_LEN, 0, &emm_mode);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_SERVICE_REQUEST);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_reset_ack(mme_vlr_t *vlr)
{
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    char mme_name[SGSAP_IE_MME_NAME_LEN+1];
    int mme_name_len = 0;
    served_gummei_t *served_gummei = &mme_self()->served_gummei[0];

    ogs_assert(vlr);

    mme_name_len = mme_name_build(mme_name,
            served_gummei->mme_code[0],
            served_gummei->mme_gid[0],
            &served_gummei->plmn_id[0]);
    root = ogs_tlv_add(NULL, SGSAP_IE_MME_NAME_TYPE, mme_name_len, 0, mme_name);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_RESET_ACK);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_uplink_unidata(mme_ue_t *mme_ue,
        ogs_nas_eps_message_container_t *nas_message_container)
{
    mme_csmap_t *csmap = NULL;
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(nas_message_container);

    ogs_assert(mme_ue);
    csmap = mme_ue->csmap;
    ogs_assert(csmap);
    vlr = csmap->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    ogs_tlv_add(root, SGSAP_IE_NAS_MESSAGE_CONTAINER_TYPE,
            nas_message_container->length, 0, nas_message_container->buffer);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_UPLINK_UNITDATA);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_ue_unreachable(mme_ue_t *mme_ue, uint8_t sgs_cause)
{
    mme_csmap_t *csmap = NULL;
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(mme_ue);
    csmap = mme_ue->csmap;
    ogs_assert(csmap);
    vlr = csmap->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    ogs_tlv_add(root, SGSAP_IE_SGS_CAUSE_TYPE,
            SGSAP_IE_SGS_CAUSE_LEN, 0, &sgs_cause);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_UE_UNREACHABLE);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, OGS_MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}
