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
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    char mme_name[SGSAP_IE_MME_NAME_LEN+1];
    int mme_name_len = 0;
    served_gummei_t *served_gummei = &mme_self()->served_gummei[0];
    char eps_update_type;
    nas_lai_t lai;

    ogs_assert(mme_ue);
    vlr = mme_ue->vlr;
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
    memcpy(&lai, &vlr->lai, sizeof(nas_lai_t));
    lai.lac = htons(lai.lac);
    ogs_tlv_add(root, SGSAP_IE_LAI_TYPE, SGSAP_IE_LAI_LEN, 0, &lai);

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_LOCATION_UPDATE_REQUEST);
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}
ogs_pkbuf_t *sgsap_build_tmsi_reallocation_complete(mme_ue_t *mme_ue)
{
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(mme_ue);
    vlr = mme_ue->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_TMSI_REALLOCATION_COMPLETE);
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_detach_indication(mme_ue_t *mme_ue)
{
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    char mme_name[SGSAP_IE_MME_NAME_LEN+1];
    int mme_name_len = 0;
    served_gummei_t *served_gummei = &mme_self()->served_gummei[0];
    uint8_t type = SGSAP_EPS_DETACH_INDICATION;
    uint8_t indication = SGSAP_EPS_DETACH_UE_INITIATED;

    ogs_assert(mme_ue);
    vlr = mme_ue->vlr;
    ogs_assert(vlr);

    switch (mme_ue->nas_eps.detach.detach_type) {
    /* 0 0 1 : EPS detach */
    case NAS_DETACH_TYPE_FROM_UE_EPS_DETACH: 
        type = SGSAP_EPS_DETACH_INDICATION;
        indication = SGSAP_EPS_DETACH_UE_INITIATED;
        break;
    /* 0 1 0 : IMSI detach */
    case NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH: 
        type = SGSAP_IMSI_DETACH_INDICATION;
        indication = SGSAP_IMSI_DETACH_EXPLICIT_UE_INITIATED;
        break;
    case 6: /* 1 1 0 : reserved */
    case 7: /* 1 1 1 : reserved */
        ogs_warn("Unknown Detach type[%d]", mme_ue->nas_eps.detach.detach_type);
        break;
    /* 0 1 1 : combined EPS/IMSI detach */
    case NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH: 
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

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put_u8(pkbuf, type);
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}

ogs_pkbuf_t *sgsap_build_service_request(mme_ue_t *mme_ue)
{
    return NULL;
}

ogs_pkbuf_t *sgsap_build_mo_csfb_indication(mme_ue_t *mme_ue)
{
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(mme_ue);
    vlr = mme_ue->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            &mme_ue->nas_mobile_identity_imsi);

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_MO_CSFB_INDICIATION);
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

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

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put_u8(pkbuf, SGSAP_RESET_ACK);
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN-1);

    ogs_pkbuf_trim(pkbuf, 1+ogs_tlv_render(root,
            pkbuf->data+1, MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1));

    ogs_tlv_free_all(root);

    return pkbuf;
}
