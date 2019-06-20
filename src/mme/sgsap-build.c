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

#include "sgsap-build.h"
#include "sgsap-conv.h"

#define SGSAP_IE_IMSI_TYPE                  1
#define SGSAP_IE_IMSI_LEN                   MAX_IMSI_LEN
#define SGSAP_IE_VLR_NAME_TYPE              2
#define SGSAP_IE_VLR_NAME_LEN               256
#define SGSAP_IE_LAI_TYPE                   4
#define SGSAP_IE_LAI_LEN                    5
#define SGSAP_IE_MME_NAME_TYPE              9
#define SGSAP_IE_MME_NAME_LEN               55
#define SGSAP_IE_EPS_UPDATE_TYPE            10
#define SGSAP_IE_EPS_UPDATE_LEN             1
#define SGSAP_IE_MOBILE_IDENTITY_TYPE       14
#define SGSAP_IE_MOBILE_IDENTITY_LEN        5
#define SGSAP_IE_SERVICE_INDICATOR_TYPE     32
#define SGSAP_IE_SERVICE_INDICATOR_LEN      1
#define SGSAP_IE_UE_EMM_MODE_TYPE           37
#define SGSAP_IE_UE_EMM_MODE_LEN            1

#define SGSAP_EPS_UPDATE_IMSI_ATTACH        1
#define SGSAP_EPS_UPDATE_NORMAL             2

ogs_pkbuf_t *sgsap_build_location_update_request(mme_ue_t *mme_ue)
{
    mme_vlr_t *vlr = NULL;
    ogs_tlv_t *root = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    char mme_name[SGSAP_IE_MME_NAME_LEN+1];
    served_gummei_t *served_gummei = &mme_self()->served_gummei[0];
    char eps_update_type;
    uint32_t len;
    const char *p;

    ogs_assert(mme_ue);
    vlr = mme_ue->vlr;
    ogs_assert(vlr);

    root = ogs_tlv_add(NULL, SGSAP_IE_IMSI_TYPE, SGSAP_IE_IMSI_LEN, 0,
            mme_ue->imsi);

    sgsap_to_mme_name(mme_name, SGSAP_IE_MME_NAME_LEN+1,
            served_gummei->mme_code[0],
            served_gummei->mme_gid[0],
            &served_gummei->plmn_id[0]);;
    ogs_tlv_add(root, SGSAP_IE_MME_NAME_TYPE, strlen(mme_name), 0,
            (uint8_t *)mme_name);
    eps_update_type = SGSAP_EPS_UPDATE_IMSI_ATTACH;
    ogs_tlv_add(root, SGSAP_IE_EPS_UPDATE_TYPE, SGSAP_IE_EPS_UPDATE_LEN, 0,
            (uint8_t *)&eps_update_type);
    ogs_tlv_add(root, SGSAP_IE_LAI_TYPE, SGSAP_IE_LAI_LEN, 0,
            (uint8_t *)&vlr->lai);

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN);
    len = ogs_tlv_render(root,
            pkbuf->data+1, MAX_SDU_LEN-1, OGS_TLV_MODE_T1_L1);
    p = pkbuf->data;
    ogs_pkbuf_trim(pkbuf, len+1);
    printf("len = %d\n", len);
    {
        int i;
        for (i = 0; i < len; i++) {
            const char *p = pkbuf->data;
            printf("%x\n", p[i]);
        }
    }

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
