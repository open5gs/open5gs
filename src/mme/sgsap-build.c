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
    return NULL;
}
ogs_pkbuf_t *sgsap_build_service_request(mme_ue_t *mme_ue)
{
    return NULL;
}
ogs_pkbuf_t *sgsap_build_mo_csfb_indication(mme_ue_t *mme_ue)
{
    return NULL;
}
