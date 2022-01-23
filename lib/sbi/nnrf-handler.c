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

#include "ogs-sbi.h"
#include "ogs-app.h"

static void handle_smf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_smf_info_t *SmfInfo)
{
    ogs_sbi_nf_info_t *nf_info = NULL;

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

    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    ogs_assert(nf_instance);
    ogs_assert(SmfInfo);

    nf_info = ogs_sbi_nf_info_add(
            &nf_instance->nf_info_list, OpenAPI_nf_type_SMF);
    ogs_assert(nf_info);

    sNssaiSmfInfoList = SmfInfo->s_nssai_smf_info_list;
    OpenAPI_list_for_each(sNssaiSmfInfoList, node) {
        sNssaiSmfInfoItem = node->data;
        if (sNssaiSmfInfoItem) {
            ogs_assert(nf_info->smf.num_of_slice < OGS_MAX_NUM_OF_SLICE);

            DnnSmfInfoList = sNssaiSmfInfoItem->dnn_smf_info_list;
            OpenAPI_list_for_each(DnnSmfInfoList, node2) {
                DnnSmfInfoItem = node2->data;
                if (DnnSmfInfoItem && DnnSmfInfoItem->dnn) {
                    int dnn_index = nf_info->smf.slice
                        [nf_info->smf.num_of_slice].num_of_dnn;

                    ogs_assert(dnn_index < OGS_MAX_NUM_OF_DNN);
                    nf_info->smf.slice[nf_info->smf.num_of_slice].
                        dnn[dnn_index] = ogs_strdup(DnnSmfInfoItem->dnn);
                    ogs_assert(
                        nf_info->smf.slice[nf_info->smf.num_of_slice].
                            dnn[dnn_index]);
                    nf_info->smf.slice[nf_info->smf.num_of_slice].
                        num_of_dnn++;
                }
            }

            if (!nf_info->smf.slice[nf_info->smf.num_of_slice].num_of_dnn) {
                ogs_error("No DNN");
                continue;
            }

            sNssai = sNssaiSmfInfoItem->s_nssai;
            if (sNssai) {
                ogs_s_nssai_t *s_nssai = NULL;

                s_nssai = &nf_info->smf.
                    slice[nf_info->smf.num_of_slice].s_nssai;
                s_nssai->sst = sNssai->sst;
                s_nssai->sd = ogs_s_nssai_sd_from_string(sNssai->sd);
                nf_info->smf.num_of_slice++;
            }
        }
    }

    if (nf_info->smf.num_of_slice == 0) {
        ogs_error("No S-NSSAI(DNN) in smfInfo");
        ogs_sbi_nf_info_remove(&nf_instance->nf_info_list, nf_info);
        return;
    }

    TaiList = SmfInfo->tai_list;
    OpenAPI_list_for_each(TaiList, node) {
        TaiItem = node->data;
        if (TaiItem && TaiItem->plmn_id && TaiItem->tac) {
            ogs_5gs_tai_t *nr_tai = NULL;
            ogs_assert(nf_info->smf.num_of_nr_tai < OGS_MAX_NUM_OF_TAI);

            nr_tai = &nf_info->smf.nr_tai[nf_info->smf.num_of_nr_tai];
            ogs_assert(nr_tai);
            ogs_sbi_parse_plmn_id(&nr_tai->plmn_id, TaiItem->plmn_id);
            nr_tai->tac = ogs_uint24_from_string(TaiItem->tac);

            nf_info->smf.num_of_nr_tai++;
        }
    }

    TaiRangeList = SmfInfo->tai_range_list;
    OpenAPI_list_for_each(TaiRangeList, node) {
        TaiRangeItem = node->data;
        if (TaiRangeItem && TaiRangeItem->plmn_id &&
                TaiRangeItem->tac_range_list) {
            ogs_assert(nf_info->smf.num_of_nr_tai_range <
                    OGS_MAX_NUM_OF_TAI);

            ogs_sbi_parse_plmn_id(
                &nf_info->smf.nr_tai_range
                    [nf_info->smf.num_of_nr_tai_range].plmn_id,
                TaiRangeItem->plmn_id);

            TacRangeList = TaiRangeItem->tac_range_list;
            OpenAPI_list_for_each(TacRangeList, node2) {
                TacRangeItem = node2->data;
                if (TacRangeItem &&
                        TacRangeItem->start && TacRangeItem->end) {
                    int tac_index = nf_info->smf.nr_tai_range
                        [nf_info->smf.num_of_nr_tai_range].num_of_tac_range;
                    ogs_assert(tac_index < OGS_MAX_NUM_OF_TAI);

                    nf_info->smf.nr_tai_range
                        [nf_info->smf.num_of_nr_tai_range].
                            start[tac_index] =
                                ogs_uint24_from_string(TacRangeItem->start);
                    nf_info->smf.nr_tai_range
                        [nf_info->smf.num_of_nr_tai_range].
                            end[tac_index] =
                                ogs_uint24_from_string(TacRangeItem->end);

                    nf_info->smf.nr_tai_range
                        [nf_info->smf.num_of_nr_tai_range].
                            num_of_tac_range++;
                }
            }

            nf_info->smf.num_of_nr_tai_range++;
        }
    }
}

bool ogs_sbi_nnrf_handle_nf_profile(ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_profile_t *NFProfile,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    int rv;

    OpenAPI_lnode_t *node;
    ogs_sbi_nf_service_t *nf_service = NULL, *next_nf_service = NULL;
#if SBI_FQDN_WITH_ONE_OCTET_LENGTH
    char fqdn[OGS_MAX_FQDN_LEN+1];
#endif

    ogs_assert(nf_instance);
    ogs_assert(NFProfile);

    if (!NFProfile) {
        ogs_error("No NFProfile");
        if (stream)
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    message, "No NFProfile", NULL));
        return false;
    }

    if (!NFProfile->nf_instance_id) {
        ogs_error("No NFProfile.NFInstanceId");
        if (stream)
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    message, "NFProfile", "No NFInstanceId"));
        return false;
    }

    if (!NFProfile->nf_type) {
        ogs_error("No NFProfile.NFType");
        if (stream)
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    message, "NFProfile", "No NFType"));
        return false;
    }

    if (!NFProfile->nf_status) {
        ogs_error("No NFProfile.NFStatus");
        if (stream)
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    message, "NFProfile", "No NFStatus"));
        return false;
    }

    ogs_list_for_each_safe(&nf_instance->nf_service_list,
            next_nf_service, nf_service) {
        bool nf_service_should_not_be_deleted = false;

        ogs_assert(nf_service->id);

        OpenAPI_list_for_each(NFProfile->nf_services, node) {
            OpenAPI_nf_service_t *NFService = node->data;

            if (!NFService) continue;
            if (!NFService->service_instance_id) continue;
            if (!NFService->service_name) continue;

            if (strcmp(nf_service->id, NFService->service_instance_id) == 0) {
                nf_service_should_not_be_deleted = true;
                break;
            }
        }

        if (nf_service_should_not_be_deleted == false) {
            ogs_warn("NFService[%s:%s] removed",
                    nf_service->id, nf_service->name);
            OpenAPI_list_for_each(NFProfile->nf_services, node) {
                OpenAPI_nf_service_t *NFService = node->data;

                if (!NFService) continue;
                if (!NFService->service_instance_id) continue;
                if (!NFService->service_name) continue;

                ogs_warn("NFService[%s:%s] will be added",
                    NFService->service_instance_id, NFService->service_name);
            }
            ogs_sbi_nf_service_remove(nf_service);
        }
    }
    ogs_sbi_nf_instance_clear(nf_instance);

    nf_instance->nf_type = NFProfile->nf_type;
    nf_instance->nf_status = NFProfile->nf_status;
    if (NFProfile->is_heart_beat_timer == true)
        nf_instance->time.heartbeat_interval = NFProfile->heart_beat_timer;

    if (NFProfile->fqdn) {
#if SBI_FQDN_WITH_ONE_OCTET_LENGTH
        if (ogs_fqdn_parse(
                fqdn, NFProfile->fqdn,
                ogs_min(strlen(NFProfile->fqdn), OGS_MAX_FQDN_LEN)) > 0) {

            /* Nothing : succeeded to parse FQDN */
            nf_instance->fqdn = ogs_strdup(fqdn);
            ogs_assert(nf_instance);

        } else {
            ogs_error("ogs_fqdn_parse() failed[%s]", NFProfile->fqdn);
            return false;
        }
#else
        nf_instance->fqdn = ogs_strdup(NFProfile->fqdn);
#endif
    }

    if (NFProfile->is_priority == true)
        nf_instance->priority = NFProfile->priority;
    if (NFProfile->is_capacity == true)
        nf_instance->capacity = NFProfile->capacity;
    if (NFProfile->is_load == true)
        nf_instance->load = NFProfile->load;

    /* Only one time handles RegisterNFInstance operation */
    OpenAPI_list_for_each(NFProfile->ipv4_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) continue;

        if (nf_instance->num_of_ipv4 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                    node->data, ogs_sbi_self()->sbi_port, 0);
            if (rv != OGS_OK) continue;

            nf_instance->ipv4[nf_instance->num_of_ipv4] = addr;
            nf_instance->num_of_ipv4++;
        }
    }
    OpenAPI_list_for_each(NFProfile->ipv6_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) continue;

        if (nf_instance->num_of_ipv6 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                    node->data, ogs_sbi_self()->sbi_port, 0);
            if (rv != OGS_OK) continue;

            nf_instance->ipv6[nf_instance->num_of_ipv6] = addr;
            nf_instance->num_of_ipv6++;
        }
    }

    OpenAPI_list_for_each(NFProfile->nf_services, node) {
        OpenAPI_nf_service_t *NFService = node->data;
        OpenAPI_list_t *VersionList = NULL;
        OpenAPI_list_t *IpEndPointList = NULL;
        OpenAPI_list_t *AllowedNfTypeList = NULL;
        OpenAPI_lnode_t *node2 = NULL;

        if (!NFService) continue;
        if (!NFService->service_instance_id) continue;
        if (!NFService->service_name) continue;

        VersionList = NFService->versions;
        IpEndPointList = NFService->ip_end_points;
        AllowedNfTypeList = NFService->allowed_nf_types;

        nf_service = ogs_sbi_nf_service_find_by_id(nf_instance,
                NFService->service_instance_id);
        if (!nf_service) {
            nf_service = ogs_sbi_nf_service_add(nf_instance,
                NFService->service_instance_id,
                NFService->service_name, NFService->scheme);
            ogs_assert(nf_service);
        }

        ogs_sbi_nf_service_clear(nf_service);

        OpenAPI_list_for_each(VersionList, node2) {
            OpenAPI_nf_service_version_t *NFServiceVersion = node2->data;

            if (!NFServiceVersion) continue;

            ogs_sbi_nf_service_add_version(nf_service,
                        NFServiceVersion->api_version_in_uri,
                        NFServiceVersion->api_full_version,
                        NFServiceVersion->expiry);
        }

        if (NFService->fqdn) {
#if SBI_FQDN_WITH_ONE_OCTET_LENGTH
            if (ogs_fqdn_parse(
                    fqdn, NFService->fqdn,
                    ogs_min(strlen(NFService->fqdn), OGS_MAX_FQDN_LEN)) > 0) {

                /* Nothing : succeeded to parse FQDN */
                nf_service->fqdn = ogs_strdup(fqdn);
                ogs_assert(nf_service);

            } else {
                ogs_error("ogs_fqdn_parse() failed[%s]", NFService->fqdn);
                return false;
            }
#else
            nf_service->fqdn = ogs_strdup(NFService->fqdn);
            ogs_assert(nf_service);
#endif
        }

        OpenAPI_list_for_each(IpEndPointList, node2) {
            OpenAPI_ip_end_point_t *IpEndPoint = node2->data;
            ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
            int port = 0;

            if (!IpEndPoint) continue;

            if (nf_service->num_of_addr < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {
                if (!IpEndPoint->is_port) {
                    if (nf_service->scheme == OpenAPI_uri_scheme_http)
                        port = OGS_SBI_HTTP_PORT;
                    else if (nf_service->scheme == OpenAPI_uri_scheme_https)
                        port = OGS_SBI_HTTPS_PORT;
                    else
                        continue;
                } else {
                    port = IpEndPoint->port;
                }

                if (IpEndPoint->ipv4_address) {
                    rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                            IpEndPoint->ipv4_address, port, 0);
                    if (rv != OGS_OK) continue;
                }
                if (IpEndPoint->ipv6_address) {
                    rv = ogs_getaddrinfo(&addr6, AF_UNSPEC,
                            IpEndPoint->ipv6_address, port, 0);
                    if (rv != OGS_OK) continue;
                }

                if (addr || addr6) {
                    nf_service->addr[nf_service->num_of_addr].
                        port = port;
                    nf_service->addr[nf_service->num_of_addr].
                        ipv4 = addr;
                    nf_service->addr[nf_service->num_of_addr].
                        ipv6 = addr6;
                    nf_service->num_of_addr++;
                }
            }
        }

        OpenAPI_list_for_each(AllowedNfTypeList, node2) {
            OpenAPI_nf_type_e AllowedNfType = (uintptr_t)node2->data;

            if (!AllowedNfType) continue;

            if (nf_service->num_of_allowed_nf_type <
                    OGS_SBI_MAX_NUM_OF_NF_TYPE) {
                nf_service->allowed_nf_types[
                    nf_service->num_of_allowed_nf_type] = AllowedNfType;
                nf_service->num_of_allowed_nf_type++;
            }
        }

        if (NFService->is_priority == true)
            nf_service->priority = NFService->priority;
        if (NFService->is_capacity == true)
            nf_service->capacity = NFService->capacity;
        if (NFService->is_load == true)
            nf_service->load = NFService->load;
    }

    ogs_sbi_nf_info_remove_all(&nf_instance->nf_info_list);

    if (NFProfile->smf_info)
        handle_smf_info(nf_instance, NFProfile->smf_info);

    OpenAPI_list_for_each(NFProfile->smf_info_list, node) {
        OpenAPI_map_t *SmfInfoMap = node->data;
        if (SmfInfoMap && SmfInfoMap->value)
            handle_smf_info(nf_instance, SmfInfoMap->value);
    }

    return true;
}
