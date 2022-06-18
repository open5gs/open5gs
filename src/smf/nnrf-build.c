/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "nnrf-build.h"

ogs_sbi_request_t *smf_nnrf_nfm_build_register(void)
{
    int i, j;
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_info_t *nf_info = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_nf_profile_t *NFProfile = NULL;

    OpenAPI_list_t *SmfInfoList = NULL;
    OpenAPI_map_t *SmfInfoMap = NULL;
    OpenAPI_smf_info_t *SmfInfo = NULL;
    int SmfInfoMapKey;

    OpenAPI_list_t *sNssaiSmfInfoList = NULL;
    OpenAPI_snssai_smf_info_item_t *sNssaiSmfInfoItem = NULL;
    OpenAPI_snssai_t *sNssai = NULL;
    OpenAPI_list_t *DnnSmfInfoList = NULL;
    OpenAPI_dnn_smf_info_item_t *DnnSmfInfoItem = NULL;

    OpenAPI_list_t *TaiList = NULL;
    OpenAPI_tai_t *TaiItem = NULL;
    OpenAPI_list_t *TaiRangeList = NULL;
    OpenAPI_tai_range_t *TaiRangeItem = NULL;
    OpenAPI_list_t *TacRangeList = NULL;
    OpenAPI_tac_range_t *TacRangeItem = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL, *node3 = NULL;

    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_assert(nf_instance->id);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = nf_instance->id;

    message.http.content_encoding = (char*)ogs_sbi_self()->content_encoding;

    SmfInfoList = OpenAPI_list_create();
    ogs_assert(SmfInfoList);

    SmfInfoMapKey = 0;

    ogs_list_for_each(&nf_instance->nf_info_list, nf_info) {
        if (nf_info->nf_type != OpenAPI_nf_type_SMF) {
            ogs_fatal("Not implemented NF-type[%s]",
                    OpenAPI_nf_type_ToString(nf_info->nf_type));
            ogs_assert_if_reached();
        }

        if (nf_info->smf.num_of_slice == 0) {
            ogs_fatal("CHECK CONFIGURATION: No S-NSSAI");
            ogs_assert_if_reached();
        }

        SmfInfo = ogs_calloc(1, sizeof(*SmfInfo));
        ogs_expect_or_return_val(SmfInfo, NULL);

        sNssaiSmfInfoList = OpenAPI_list_create();
        ogs_assert(sNssaiSmfInfoList);

        for (i = 0; i < nf_info->smf.num_of_slice; i++) {
            DnnSmfInfoList = OpenAPI_list_create();
            ogs_assert(DnnSmfInfoList);

            for (j = 0; j < nf_info->smf.slice[i].num_of_dnn; j++) {
                DnnSmfInfoItem = ogs_calloc(1, sizeof(*DnnSmfInfoItem));
                ogs_expect_or_return_val(DnnSmfInfoItem, NULL);
                DnnSmfInfoItem->dnn = nf_info->smf.slice[i].dnn[j];

                OpenAPI_list_add(DnnSmfInfoList, DnnSmfInfoItem);
            }

            if (!DnnSmfInfoList->count) {
                OpenAPI_list_free(DnnSmfInfoList);

                ogs_error("CHECK CONFIGURATION: No DNN");
                ogs_expect_or_return_val(0, NULL);
            }

            sNssaiSmfInfoItem = ogs_calloc(1, sizeof(*sNssaiSmfInfoItem));
            ogs_expect_or_return_val(sNssaiSmfInfoItem, NULL);

            sNssaiSmfInfoItem->dnn_smf_info_list = DnnSmfInfoList;

            sNssaiSmfInfoItem->s_nssai = sNssai =
                ogs_calloc(1, sizeof(*sNssai));
            ogs_expect_or_return_val(sNssai, NULL);
            sNssai->sst = nf_info->smf.slice[i].s_nssai.sst;
            sNssai->sd =
                ogs_s_nssai_sd_to_string(nf_info->smf.slice[i].s_nssai.sd);

            OpenAPI_list_add(sNssaiSmfInfoList, sNssaiSmfInfoItem);
        }

        if (sNssaiSmfInfoList->count)
            SmfInfo->s_nssai_smf_info_list = sNssaiSmfInfoList;
        else
            OpenAPI_list_free(sNssaiSmfInfoList);

        TaiList = OpenAPI_list_create();
        ogs_assert(TaiList);

        for (i = 0; i < nf_info->smf.num_of_nr_tai; i++) {
            TaiItem = ogs_calloc(1, sizeof(*TaiItem));
            ogs_expect_or_return_val(TaiItem, NULL);
            TaiItem->plmn_id = ogs_sbi_build_plmn_id(
                    &nf_info->smf.nr_tai[i].plmn_id);
            ogs_expect_or_return_val(TaiItem->plmn_id, NULL);
            TaiItem->tac = ogs_uint24_to_0string(nf_info->smf.nr_tai[i].tac);
            ogs_expect_or_return_val(TaiItem->tac, NULL);

            OpenAPI_list_add(TaiList, TaiItem);
        }

        if (TaiList->count)
            SmfInfo->tai_list = TaiList;
        else
            OpenAPI_list_free(TaiList);

        TaiRangeList = OpenAPI_list_create();
        ogs_assert(TaiRangeList);

        for (i = 0; i < nf_info->smf.num_of_nr_tai_range; i++) {
            TacRangeList = OpenAPI_list_create();
            ogs_assert(TacRangeList);

            for (j = 0;
                    j < nf_info->smf.nr_tai_range[i].num_of_tac_range; j++) {
                TacRangeItem = ogs_calloc(1, sizeof(*TacRangeItem));
                ogs_expect_or_return_val(TacRangeItem, NULL);

                TacRangeItem->start = ogs_uint24_to_0string(
                        nf_info->smf.nr_tai_range[i].start[j]);
                ogs_expect_or_return_val(TacRangeItem->start, NULL);
                TacRangeItem->end =
                    ogs_uint24_to_0string(nf_info->smf.nr_tai_range[i].end[j]);
                ogs_expect_or_return_val(TacRangeItem->end, NULL);

                OpenAPI_list_add(TacRangeList, TacRangeItem);
            }

            if (!TacRangeList->count) {
                OpenAPI_list_free(TacRangeList);

                ogs_error("CHECK CONFIGURATION: No Start/End in TacRange");
                ogs_expect_or_return_val(0, NULL);
            }

            TaiRangeItem = ogs_calloc(1, sizeof(*TaiRangeItem));
            ogs_expect_or_return_val(TaiRangeItem, NULL);

            TaiRangeItem->plmn_id = ogs_sbi_build_plmn_id(
                    &nf_info->smf.nr_tai_range[i].plmn_id);
            ogs_expect_or_return_val(TaiRangeItem->plmn_id, NULL);

            TaiRangeItem->tac_range_list = TacRangeList;

            OpenAPI_list_add(TaiRangeList, TaiRangeItem);
        }

        if (TaiRangeList->count)
            SmfInfo->tai_range_list = TaiRangeList;
        else
            OpenAPI_list_free(TaiRangeList);

        SmfInfoMap = OpenAPI_map_create(
                ogs_msprintf("%d", ++SmfInfoMapKey), SmfInfo);
        ogs_assert(SmfInfoMap);

        OpenAPI_list_add(SmfInfoList, SmfInfoMap);
    }

    NFProfile = ogs_nnrf_nfm_build_nf_profile();
    ogs_expect_or_return_val(NFProfile, NULL);

    if (SmfInfoList->count == 1) {
        NFProfile->smf_info = SmfInfo;
    } else if (SmfInfoList->count > 1) {
        NFProfile->smf_info_list = SmfInfoList;
    }

    message.NFProfile = NFProfile;

    request = ogs_sbi_build_request(&message);

    ogs_sbi_nnrf_free_nf_profile(NFProfile);

    OpenAPI_list_for_each(SmfInfoList, node) {
        SmfInfoMap = node->data;
        if (SmfInfoMap) {
            SmfInfo = SmfInfoMap->value;
            if (SmfInfo) {
                sNssaiSmfInfoList = SmfInfo->s_nssai_smf_info_list;
                OpenAPI_list_for_each(sNssaiSmfInfoList, node2) {
                    sNssaiSmfInfoItem = node2->data;
                    ogs_assert(sNssaiSmfInfoItem);

                    DnnSmfInfoList = sNssaiSmfInfoItem->dnn_smf_info_list;
                    OpenAPI_list_for_each(DnnSmfInfoList, node3) {
                        DnnSmfInfoItem = node3->data;
                        ogs_assert(DnnSmfInfoItem);
                        ogs_free(DnnSmfInfoItem);
                    }
                    OpenAPI_list_free(DnnSmfInfoList);

                    sNssai = sNssaiSmfInfoItem->s_nssai;
                    if (sNssai) {
                        if (sNssai->sd)
                            ogs_free(sNssai->sd);
                        ogs_free(sNssai);
                    }

                    ogs_free(sNssaiSmfInfoItem);
                }
                OpenAPI_list_free(sNssaiSmfInfoList);

                TaiList = SmfInfo->tai_list;
                OpenAPI_list_for_each(TaiList, node2) {
                    TaiItem = node2->data;
                    ogs_assert(TaiItem);
                    if (TaiItem->plmn_id)
                        ogs_sbi_free_plmn_id(TaiItem->plmn_id);
                    if (TaiItem->tac)
                        ogs_free(TaiItem->tac);
                    ogs_free(TaiItem);
                }
                OpenAPI_list_free(TaiList);

                TaiRangeList = SmfInfo->tai_range_list;
                OpenAPI_list_for_each(TaiRangeList, node2) {
                    TaiRangeItem = node2->data;
                    ogs_assert(TaiRangeItem);

                    if (TaiRangeItem->plmn_id)
                        ogs_sbi_free_plmn_id(TaiRangeItem->plmn_id);

                    TacRangeList = TaiRangeItem->tac_range_list;
                    OpenAPI_list_for_each(TacRangeList, node3) {
                        TacRangeItem = node3->data;
                        ogs_assert(TacRangeItem);
                        if (TacRangeItem->start)
                            ogs_free(TacRangeItem->start);
                        if (TacRangeItem->end)
                            ogs_free(TacRangeItem->end);

                        ogs_free(TacRangeItem);
                    }
                    OpenAPI_list_free(TacRangeList);

                    ogs_free(TaiRangeItem);
                }
                OpenAPI_list_free(TaiRangeList);

                ogs_free(SmfInfo);
            }
            if (SmfInfoMap->key)
                ogs_free(SmfInfoMap->key);
            ogs_free(SmfInfoMap);
        }
    }
    OpenAPI_list_free(SmfInfoList);

    return request;
}
