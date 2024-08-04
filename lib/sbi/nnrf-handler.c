/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

static void handle_nf_service(
        ogs_sbi_nf_service_t *nf_service, OpenAPI_nf_service_t *NFService);
static void handle_smf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_smf_info_t *SmfInfo);
static void handle_scp_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_scp_info_t *ScpInfo);
static void handle_sepp_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_sepp_info_t *SeppInfo);
static void handle_amf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_amf_info_t *AmfInfo);

void ogs_nnrf_nfm_handle_nf_register(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_nf_profile_t *NFProfile = NULL;
    OpenAPI_lnode_t *node;

    ogs_assert(recvmsg);
    ogs_assert(nf_instance);

    NFProfile = recvmsg->NFProfile;
    if (!NFProfile) {
        ogs_error("No NFProfile");
        return;
    }

    /* TIME : Update heartbeat from NRF */
    if (NFProfile->is_heart_beat_timer == true)
        nf_instance->time.heartbeat_interval = NFProfile->heart_beat_timer;

    if (NFProfile->plmn_list) {
        nf_instance->num_of_plmn_id = 0;
        OpenAPI_list_for_each(NFProfile->plmn_list, node) {
            OpenAPI_plmn_id_t *PlmnId = node->data;
            if (PlmnId) {
                if (ogs_local_conf()->num_of_serving_plmn_id >=
                        OGS_ARRAY_SIZE(ogs_local_conf()->serving_plmn_id)) {
                    ogs_error("OVERFLOW NFProfile->plmn_list [%d:%d:%d]",
                            ogs_local_conf()->num_of_serving_plmn_id,
                            OGS_MAX_NUM_OF_PLMN,
                            (int)OGS_ARRAY_SIZE(
                                ogs_local_conf()->serving_plmn_id));
                    break;
                }
                ogs_sbi_parse_plmn_id(
                    &ogs_local_conf()->serving_plmn_id[
                        ogs_local_conf()->num_of_serving_plmn_id], PlmnId);
                ogs_local_conf()->num_of_serving_plmn_id++;
            }
        }
    }
}

void ogs_nnrf_nfm_handle_nf_profile(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_profile_t *NFProfile)
{
    int rv;
    OpenAPI_lnode_t *node;

    ogs_assert(nf_instance);
    ogs_assert(NFProfile);
    ogs_assert(NFProfile->nf_instance_id);
    ogs_assert(NFProfile->nf_type);
    ogs_assert(NFProfile->nf_status);

    ogs_sbi_nf_instance_clear(nf_instance);

    nf_instance->nf_type = NFProfile->nf_type;
    nf_instance->nf_status = NFProfile->nf_status;
    if (NFProfile->is_heart_beat_timer == true)
        nf_instance->time.heartbeat_interval = NFProfile->heart_beat_timer;

    if (NFProfile->fqdn)
        nf_instance->fqdn = ogs_strdup(NFProfile->fqdn);

    if (NFProfile->is_priority == true)
        nf_instance->priority = NFProfile->priority;
    if (NFProfile->is_capacity == true)
        nf_instance->capacity = NFProfile->capacity;
    if (NFProfile->is_load == true)
        nf_instance->load = NFProfile->load;

    nf_instance->num_of_plmn_id = 0;
    OpenAPI_list_for_each(NFProfile->plmn_list, node) {
        OpenAPI_plmn_id_t *PlmnId = node->data;
        if (PlmnId) {
            if (nf_instance->num_of_plmn_id >=
                    OGS_ARRAY_SIZE(nf_instance->plmn_id)) {
                ogs_error("OVERFLOW NFProfile->plmn_list [%d:%d:%d]",
                        nf_instance->num_of_plmn_id, OGS_MAX_NUM_OF_PLMN,
                        (int)OGS_ARRAY_SIZE(nf_instance->plmn_id));
                break;
            }
            ogs_sbi_parse_plmn_id(
                &nf_instance->plmn_id[nf_instance->num_of_plmn_id], PlmnId);
            nf_instance->num_of_plmn_id++;
        }
    }

    OpenAPI_list_for_each(NFProfile->ipv4_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) {
            ogs_error("No IPv4 Address");
            continue;
        }

        if (nf_instance->num_of_ipv4 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(
                    &addr, AF_UNSPEC,
                    node->data,
                    ogs_sbi_default_client_port(OpenAPI_uri_scheme_NULL), 0);
            if (rv != OGS_OK) {
                ogs_error("ogs_getaddrinfo[%s] failed", (char *)node->data);
                continue;
            }

            nf_instance->ipv4[nf_instance->num_of_ipv4] = addr;
            nf_instance->num_of_ipv4++;
        }
    }
    OpenAPI_list_for_each(NFProfile->ipv6_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) {
            ogs_error("No IPv6 Address");
            continue;
        }

        if (nf_instance->num_of_ipv6 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(
                    &addr, AF_UNSPEC,
                    node->data,
                    ogs_sbi_default_client_port(OpenAPI_uri_scheme_NULL), 0);
            if (rv != OGS_OK) {
                ogs_error("ogs_getaddrinfo[%s] failed", (char *)node->data);
                continue;
            }

            nf_instance->ipv6[nf_instance->num_of_ipv6] = addr;
            nf_instance->num_of_ipv6++;
        }
    }

    OpenAPI_list_for_each(NFProfile->allowed_nf_types, node) {
        OpenAPI_nf_type_e AllowedNfType = (uintptr_t)node->data;

        if (!AllowedNfType) {
            ogs_error("AllowedNfType");
            continue;
        }

        if (nf_instance->num_of_allowed_nf_type <
                OGS_SBI_MAX_NUM_OF_NF_TYPE) {
            nf_instance->allowed_nf_type[
                nf_instance->num_of_allowed_nf_type] = AllowedNfType;
            nf_instance->num_of_allowed_nf_type++;
        }
    }

    OpenAPI_list_for_each(NFProfile->nf_services, node) {
        ogs_sbi_nf_service_t *nf_service = NULL;
        OpenAPI_nf_service_t *NFService = node->data;

        if (!NFService) {
            ogs_error("No NFService");
            continue;
        }

        if (!NFService->service_instance_id) {
            ogs_error("No NFService.service_instance_id");
            continue;
        }

        if (!NFService->service_name) {
            ogs_error("No NFService.service_name");
            continue;
        }

        if (!NFService->scheme) {
            ogs_error("No NFService.scheme");
            continue;
        }

        nf_service = ogs_sbi_nf_service_find_by_id(
                        nf_instance, NFService->service_instance_id);
        if (!nf_service) {
            nf_service = ogs_sbi_nf_service_add(
                            nf_instance,
                            NFService->service_instance_id,
                            NFService->service_name, NFService->scheme);
            ogs_assert(nf_service);
        }

        ogs_sbi_nf_service_clear(nf_service);

        handle_nf_service(nf_service, NFService);
    }

    OpenAPI_list_for_each(NFProfile->nf_service_list, node) {
        ogs_sbi_nf_service_t *nf_service = NULL;
        OpenAPI_map_t *NFServiceMap = NULL;
        OpenAPI_nf_service_t *NFService = node->data;

        NFServiceMap = node->data;
        if (NFServiceMap) {
            NFService = NFServiceMap->value;
            if (!NFService) {
                ogs_error("No NFService");
                continue;
            }

            if (!NFService->service_instance_id) {
                ogs_error("No NFService.service_instance_id");
                continue;
            }

            if (!NFService->service_name) {
                ogs_error("No NFService.service_name");
                continue;
            }

            if (!NFService->scheme) {
                ogs_error("No NFService.scheme");
                continue;
            }

            nf_service = ogs_sbi_nf_service_find_by_id(
                            nf_instance, NFService->service_instance_id);
            if (!nf_service) {
                nf_service = ogs_sbi_nf_service_add(
                                nf_instance,
                                NFService->service_instance_id,
                                NFService->service_name, NFService->scheme);
                ogs_assert(nf_service);
            }

            ogs_sbi_nf_service_clear(nf_service);

            handle_nf_service(nf_service, NFService);
        }
    }

    ogs_sbi_nf_info_remove_all(&nf_instance->nf_info_list);

    if (NFProfile->smf_info)
        handle_smf_info(nf_instance, NFProfile->smf_info);

    OpenAPI_list_for_each(NFProfile->smf_info_list, node) {
        OpenAPI_map_t *SmfInfoMap = node->data;
        if (SmfInfoMap && SmfInfoMap->value)
            handle_smf_info(nf_instance, SmfInfoMap->value);
    }
    if (NFProfile->amf_info)
        handle_amf_info(nf_instance, NFProfile->amf_info);

    OpenAPI_list_for_each(NFProfile->amf_info_list, node) {
        OpenAPI_map_t *AmfInfoMap = node->data;
        if (AmfInfoMap && AmfInfoMap->value)
            handle_amf_info(nf_instance, AmfInfoMap->value);
    }
    if (NFProfile->scp_info)
        handle_scp_info(nf_instance, NFProfile->scp_info);
    if (NFProfile->sepp_info)
        handle_sepp_info(nf_instance, NFProfile->sepp_info);
}

static void handle_nf_service(
        ogs_sbi_nf_service_t *nf_service, OpenAPI_nf_service_t *NFService)
{
    int rv;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(nf_service);
    ogs_assert(NFService);

    OpenAPI_list_for_each(NFService->versions, node) {
        OpenAPI_nf_service_version_t *NFServiceVersion = node->data;

        if (!NFServiceVersion) {
            ogs_error("No NFServiceVersion");
            continue;
        }

        ogs_sbi_nf_service_add_version(nf_service,
                    NFServiceVersion->api_version_in_uri,
                    NFServiceVersion->api_full_version,
                    NFServiceVersion->expiry);
    }

    if (NFService->fqdn)
        nf_service->fqdn = ogs_strdup(NFService->fqdn);

    OpenAPI_list_for_each(NFService->ip_end_points, node) {
        OpenAPI_ip_end_point_t *IpEndPoint = node->data;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
        int port = 0;

        if (!IpEndPoint) {
            ogs_error("No IpEndPoint");
            continue;
        }

        if (nf_service->num_of_addr < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {
            if (!IpEndPoint->is_port)
                port = ogs_sbi_default_client_port(nf_service->scheme);
            else
                port = IpEndPoint->port;

            if (IpEndPoint->ipv4_address) {
                rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                        IpEndPoint->ipv4_address, port, 0);
                if (rv != OGS_OK) {
                    ogs_error("ogs_getaddrinfo[%s] failed",
                                IpEndPoint->ipv4_address);
                    continue;
                }
            }
            if (IpEndPoint->ipv6_address) {
                rv = ogs_getaddrinfo(&addr6, AF_UNSPEC,
                        IpEndPoint->ipv6_address, port, 0);
                if (rv != OGS_OK) {
                    ogs_error("ogs_getaddrinfo[%s] failed",
                                IpEndPoint->ipv6_address);
                    continue;
                }
            }

            if (addr || addr6) {
                nf_service->addr[nf_service->num_of_addr].
                    is_port = IpEndPoint->is_port;
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

    OpenAPI_list_for_each(NFService->allowed_nf_types, node) {
        OpenAPI_nf_type_e AllowedNfType = (uintptr_t)node->data;

        if (!AllowedNfType) {
            ogs_error("AllowedNfType");
            continue;
        }

        if (nf_service->num_of_allowed_nf_type <
                OGS_SBI_MAX_NUM_OF_NF_TYPE) {
            nf_service->allowed_nf_type[
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

static void handle_smf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_smf_info_t *SmfInfo)
{
    ogs_sbi_nf_info_t *nf_info = NULL;

    OpenAPI_list_t *sNssaiSmfInfoList = NULL;
    OpenAPI_snssai_smf_info_item_t *sNssaiSmfInfoItem = NULL;
    OpenAPI_ext_snssai_t *sNssai = NULL;
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

            if (nf_info->smf.num_of_nr_tai_range >= OGS_MAX_NUM_OF_TAI) {
                ogs_error("OVERFLOW TaiRangeItem [%d:%d]",
                        nf_info->smf.num_of_nr_tai_range, OGS_MAX_NUM_OF_TAI);
                break;
            }

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

static void handle_scp_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_scp_info_t *ScpInfo)
{
    ogs_sbi_nf_info_t *nf_info = NULL;

    OpenAPI_list_t *PortList = NULL;
    OpenAPI_map_t *PortMap = NULL;

    OpenAPI_map_t *DomainInfoMap = NULL;
    OpenAPI_scp_domain_info_t *DomainInfo = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    ogs_sbi_scp_info_t scp_info;

    ogs_assert(nf_instance);
    ogs_assert(ScpInfo);

    memset(&scp_info, 0, sizeof(scp_info));

    PortList = ScpInfo->scp_ports;
    OpenAPI_list_for_each(PortList, node) {
        PortMap = node->data;
        if (PortMap) {
            if (PortMap->key) {
                double *port = PortMap->value;
                if (strcmp(PortMap->key, "http") == 0) {
                    if (port) {
                        scp_info.http.presence = true;
                        scp_info.http.port = *port;
                    } else {
                        ogs_error("No Port Value");
                    }
                } else if (strcmp(PortMap->key, "https") == 0) {
                    if (port) {
                        scp_info.https.presence = true;
                        scp_info.https.port = *port;
                    } else {
                        ogs_error("No Port Value");
                    }
                } else {
                    ogs_error("Unknown Port Key = %s", PortMap->key);
                }
            }
        }
    }

    OpenAPI_list_for_each(ScpInfo->scp_domain_info_list, node) {
        DomainInfoMap = node->data;
        if (DomainInfoMap && DomainInfoMap->key && DomainInfoMap->value) {
            scp_info.domain[scp_info.num_of_domain].name =
                ogs_strdup(DomainInfoMap->key);
            DomainInfo = DomainInfoMap->value;
            if (DomainInfo->scp_fqdn)
                scp_info.domain[scp_info.num_of_domain].fqdn =
                    ogs_strdup(DomainInfo->scp_fqdn);
            OpenAPI_list_for_each(DomainInfo->scp_ports, node2) {
                PortMap = node2->data;
                if (PortMap) {
                    if (PortMap->key) {
                        double *port = PortMap->value;
                        if (strcmp(PortMap->key, "http") == 0) {
                            if (port) {
                                scp_info.domain[scp_info.num_of_domain].
                                    http.presence = true;
                                scp_info.domain[scp_info.num_of_domain].
                                    http.port = *port;
                            } else {
                                ogs_error("No Port Value");
                            }
                        } else if (strcmp(PortMap->key, "https") == 0) {
                            if (port) {
                                scp_info.domain[scp_info.num_of_domain].
                                    https.presence = true;
                                scp_info.domain[scp_info.num_of_domain].
                                    https.port = *port;
                            } else {
                                ogs_error("No Port Value");
                            }
                        } else {
                            ogs_error("Unknown Port Key = %s", PortMap->key);
                        }
                    }
                }
            }
            scp_info.num_of_domain++;
        }
    }

    if (scp_info.http.presence || scp_info.https.presence) {
        nf_info = ogs_sbi_nf_info_add(
                &nf_instance->nf_info_list, OpenAPI_nf_type_SCP);
        ogs_assert(nf_info);

        memcpy(&nf_info->scp, &scp_info, sizeof(scp_info));
    }
}

static void handle_sepp_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_sepp_info_t *SeppInfo)
{
    ogs_sbi_nf_info_t *nf_info = NULL;

    OpenAPI_list_t *PortList = NULL;
    OpenAPI_map_t *PortMap = NULL;

    OpenAPI_lnode_t *node = NULL;

    ogs_port_t http, https;

    ogs_assert(nf_instance);
    ogs_assert(SeppInfo);

    http.port = 0;
    https.port = 0;
    http.presence = false;
    https.presence = false;

    PortList = SeppInfo->sepp_ports;
    OpenAPI_list_for_each(PortList, node) {
        PortMap = node->data;
        if (PortMap) {
            if (PortMap->key) {
                double *port = PortMap->value;
                if (strcmp(PortMap->key, "http") == 0) {
                    if (port) {
                        http.presence = true;
                        http.port = *port;
                    } else {
                        ogs_error("No Port Value");
                    }
                } else if (strcmp(PortMap->key, "https") == 0) {
                    if (port) {
                        https.presence = true;
                        https.port = *port;
                    } else {
                        ogs_error("No Port Value");
                    }
                } else {
                    ogs_error("Unknown Port Key = %s", PortMap->key);
                }
            }
        }
    }

    if (http.presence || https.presence) {
        nf_info = ogs_sbi_nf_info_add(
                &nf_instance->nf_info_list, OpenAPI_nf_type_SEPP);
        ogs_assert(nf_info);

        nf_info->sepp.http.presence = http.presence;
        nf_info->sepp.http.port = http.port;

        nf_info->sepp.https.presence = https.presence;
        nf_info->sepp.https.port = https.port;
    }
}

static void handle_amf_info(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_amf_info_t *AmfInfo)
{
    ogs_sbi_nf_info_t *nf_info = NULL;
    OpenAPI_list_t *GuamiList = NULL;
    OpenAPI_guami_t *GuamiAmfInfoItem = NULL;
    OpenAPI_list_t *TaiList = NULL;
    OpenAPI_tai_t *TaiItem = NULL;
    OpenAPI_list_t *TaiRangeList = NULL;
    OpenAPI_tai_range_t *TaiRangeItem = NULL;
    OpenAPI_list_t *TacRangeList = NULL;
    OpenAPI_tac_range_t *TacRangeItem = NULL;
    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    ogs_assert(nf_instance);
    ogs_assert(AmfInfo);

    nf_info = ogs_sbi_nf_info_add(
            &nf_instance->nf_info_list, OpenAPI_nf_type_AMF);
    ogs_assert(nf_info);

    nf_info->amf.amf_set_id = ogs_uint64_from_string(AmfInfo->amf_set_id);
    nf_info->amf.amf_region_id = ogs_uint64_from_string(AmfInfo->amf_region_id);
    GuamiList = AmfInfo->guami_list;

    OpenAPI_list_for_each(GuamiList, node) {
        GuamiAmfInfoItem = node->data;
        if (GuamiAmfInfoItem) {
            ogs_assert(nf_info->amf.num_of_guami < OGS_MAX_NUM_OF_SERVED_GUAMI);

            if (GuamiAmfInfoItem->amf_id && GuamiAmfInfoItem->plmn_id &&
                    GuamiAmfInfoItem->plmn_id->mnc &&
                    GuamiAmfInfoItem->plmn_id->mcc) {

                ogs_sbi_parse_guami(
                        &nf_info->amf.guami[nf_info->amf.num_of_guami],
                        GuamiAmfInfoItem);
                nf_info->amf.num_of_guami++;
            }
        }
    }

    TaiList = AmfInfo->tai_list;
    OpenAPI_list_for_each(TaiList, node) {
        TaiItem = node->data;
        if (TaiItem && TaiItem->plmn_id && TaiItem->tac) {
            ogs_5gs_tai_t *nr_tai = NULL;

            if (nf_info->amf.num_of_nr_tai >= OGS_MAX_NUM_OF_TAI) {
                ogs_error("OVERFLOW TaiItem [%d:%d]",
                        nf_info->amf.num_of_nr_tai, OGS_MAX_NUM_OF_TAI);
                break;
            }

            nr_tai = &nf_info->amf.nr_tai[nf_info->amf.num_of_nr_tai];
            ogs_assert(nr_tai);
            ogs_sbi_parse_plmn_id(&nr_tai->plmn_id, TaiItem->plmn_id);
            nr_tai->tac = ogs_uint24_from_string(TaiItem->tac);
            nf_info->amf.num_of_nr_tai++;
        }
    }

    TaiRangeList = AmfInfo->tai_range_list;
    OpenAPI_list_for_each(TaiRangeList, node) {
        TaiRangeItem = node->data;
        if (TaiRangeItem && TaiRangeItem->plmn_id &&
                TaiRangeItem->tac_range_list) {
            ogs_assert(nf_info->amf.num_of_nr_tai_range <
                    OGS_MAX_NUM_OF_TAI);

            if (nf_info->amf.num_of_nr_tai_range >= OGS_MAX_NUM_OF_TAI) {
                ogs_error("OVERFLOW TaiRangeItem [%d:%d]",
                        nf_info->amf.num_of_nr_tai_range, OGS_MAX_NUM_OF_TAI);
                break;
            }

            ogs_sbi_parse_plmn_id(
                &nf_info->amf.nr_tai_range
                    [nf_info->amf.num_of_nr_tai_range].plmn_id,
                TaiRangeItem->plmn_id);

            TacRangeList = TaiRangeItem->tac_range_list;
            OpenAPI_list_for_each(TacRangeList, node2) {
                TacRangeItem = node2->data;
                if (TacRangeItem &&
                        TacRangeItem->start && TacRangeItem->end) {
                    int tac_index = nf_info->amf.nr_tai_range
                        [nf_info->amf.num_of_nr_tai_range].num_of_tac_range;
                    ogs_assert(tac_index < OGS_MAX_NUM_OF_TAI);

                    nf_info->amf.nr_tai_range
                        [nf_info->amf.num_of_nr_tai_range].start[tac_index] =
                                ogs_uint24_from_string(TacRangeItem->start);
                    nf_info->amf.nr_tai_range
                        [nf_info->amf.num_of_nr_tai_range].end[tac_index] =
                                ogs_uint24_from_string(TacRangeItem->end);

                    nf_info->amf.nr_tai_range
                        [nf_info->amf.num_of_nr_tai_range].num_of_tac_range++;
                }
            }
            nf_info->amf.num_of_nr_tai_range++;
        }
    }
}

static void handle_validity_time(
        ogs_sbi_subscription_data_t *subscription_data,
        char *validity_time, const char *action)
{
    ogs_time_t time, validity, patch;
    char *validity_time_string = NULL;

    ogs_assert(subscription_data);
    ogs_assert(action);

    /*
     * If there is a validity_time, then the NRF is updating
     * the validity_time by sending HTTP_STATUS to 200.
     * Therefore, change subscription_data->valdity_duration
     * according to this value.
     *
     * If validity_time is NULL, NRF sent an HTTP_STATUS of 204 (No content).
     * In this case, use the existing subscription_data->validity_duration.
     */
    if (validity_time) {
        if (ogs_sbi_time_from_string(&time, validity_time) == false) {
            ogs_error("[%s] Subscription %s until %s [parser error]",
                    subscription_data->id, action, validity_time);
            return;
        }

        validity = time - ogs_time_now();
        if (validity < 0) {
            ogs_error("[%s] Subscription %s until %s [validity:%d.%06d]",
                    subscription_data->id, action, validity_time,
                    (int)ogs_time_sec(validity), (int)ogs_time_usec(validity));
            return;
        }

        /*
         * Store subscription_data->validity_duration to derive NRF validity.
         * It will be used in ogs_nnrf_nfm_build_status_update().
         *
         * So, you should not remove the following lines.
         */
        subscription_data->validity_duration =
            /* Normalize seconds */
            ogs_time_from_sec(ogs_time_to_sec(validity));
    }

    if (!subscription_data->t_validity) {
        subscription_data->t_validity =
            ogs_timer_add(ogs_app()->timer_mgr,
                ogs_timer_subscription_validity, subscription_data);
        ogs_assert(subscription_data->t_validity);
    }
    ogs_timer_start(subscription_data->t_validity,
            subscription_data->validity_duration);

    /*
     * PATCH request will be sent before VALIDITY is expired.
     */
#define PATCH_TIME_FROM_VALIDITY(x) ((x) / 2)
    patch = PATCH_TIME_FROM_VALIDITY(subscription_data->validity_duration);

    if (!subscription_data->t_patch) {
        subscription_data->t_patch =
            ogs_timer_add(ogs_app()->timer_mgr,
                ogs_timer_subscription_patch, subscription_data);
        ogs_assert(subscription_data->t_patch);
    }
    ogs_timer_start(subscription_data->t_patch, patch);

    if (validity_time) {
        validity_time_string = ogs_strdup(validity_time);
        ogs_assert(validity_time_string);
    } else {
        validity_time_string = ogs_sbi_localtime_string(
                ogs_time_now() + subscription_data->validity_duration);
        ogs_assert(validity_time_string);
    }

    ogs_info("[%s] Subscription %s until %s "
            "[duration:%lld,validity:%d.%06d,patch:%d.%06d]",
            subscription_data->id, action, validity_time_string,
            (long long)subscription_data->validity_duration,
            (int)ogs_time_sec(subscription_data->validity_duration),
            (int)ogs_time_usec(subscription_data->validity_duration),
            (int)ogs_time_sec(patch), (int)ogs_time_usec(patch));

    ogs_free(validity_time_string);
}

void ogs_nnrf_nfm_handle_nf_status_subscribe(
        ogs_sbi_subscription_data_t *subscription_data,
        ogs_sbi_message_t *recvmsg)
{
    OpenAPI_subscription_data_t *SubscriptionData = NULL;

    int rv;
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;

    bool rc;
    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(recvmsg);
    ogs_assert(subscription_data);

    SubscriptionData = recvmsg->SubscriptionData;
    if (!SubscriptionData) {
        ogs_error("No SubscriptionData");
        return;
    }

    if (!recvmsg->http.location) {
        ogs_error("No http.location");
        return;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("Cannot parse http.location [%s]",
            recvmsg->http.location);
        return;
    }

    if (!message.h.resource.component[1]) {
        ogs_error("No Subscription ID [%s]", recvmsg->http.location);
        ogs_sbi_header_free(&header);
        return;
    }

    rc = ogs_sbi_getaddr_from_uri(
            &scheme, &fqdn, &fqdn_port, &addr, &addr6, header.uri);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("Invalid URI [%s]", header.uri);
        ogs_sbi_header_free(&header);
        return;
    }

    client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
    if (!client) {
        ogs_debug("%s: ogs_sbi_client_add()", OGS_FUNC);
        client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_error("%s: ogs_sbi_client_add() failed", OGS_FUNC);

            ogs_sbi_header_free(&header);
            ogs_free(fqdn);
            ogs_freeaddrinfo(addr);
            ogs_freeaddrinfo(addr6);

            return;
        }
    }
    OGS_SBI_SETUP_CLIENT(subscription_data, client);

    ogs_free(fqdn);
    ogs_freeaddrinfo(addr);
    ogs_freeaddrinfo(addr6);

    ogs_sbi_subscription_data_set_resource_uri(
            subscription_data, header.uri);
    ogs_sbi_subscription_data_set_id(
            subscription_data, message.h.resource.component[1]);

    ogs_sbi_header_free(&header);

    /* SBI Features */
    if (SubscriptionData->nrf_supported_features) {
        subscription_data->nrf_supported_features =
            ogs_uint64_from_string(SubscriptionData->nrf_supported_features);
    } else {
        subscription_data->nrf_supported_features = 0;
    }

    /* Subscription Validity Time */
    if (SubscriptionData->validity_time)
        handle_validity_time(
                subscription_data, SubscriptionData->validity_time, "created");
}

void ogs_nnrf_nfm_handle_nf_status_update(
        ogs_sbi_subscription_data_t *subscription_data,
        ogs_sbi_message_t *recvmsg)
{
    OpenAPI_subscription_data_t *SubscriptionData = NULL;
    char *validity_time = NULL;
    const char *action = NULL;

    ogs_assert(recvmsg);
    ogs_assert(subscription_data);

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
        SubscriptionData = recvmsg->SubscriptionData;
        if (!SubscriptionData) {
            ogs_error("No SubscriptionData");
            return;
        }
        if (!SubscriptionData->validity_time) {
            ogs_error("No validityTime");
            return;
        }

        validity_time = SubscriptionData->validity_time;
        action = "updated(200 OK)";
    } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT) {
        /* No valdityTime. Re-use current subscription_data->valdity_duration */
        action = "updated(204 No Content)";
    } else {
        ogs_fatal("[%s] HTTP response error [%d]",
                subscription_data->id ?  subscription_data->id : "Unknown",
                recvmsg->res_status);
        ogs_assert_if_reached();
    }

    /* Update Subscription Validity Time */
    handle_validity_time(subscription_data, validity_time, action);
}

bool ogs_nnrf_nfm_handle_nf_status_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;

    ogs_sbi_response_t *response = NULL;
    OpenAPI_notification_data_t *NotificationData = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_header_t header;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    NotificationData = recvmsg->NotificationData;
    if (!NotificationData) {
        ogs_error("No NotificationData");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No NotificationData", NULL, NULL));
        return false;
    }

    if (!NotificationData->nf_instance_uri) {
        ogs_error("No nfInstanceUri");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No nfInstanceUri", NULL, NULL));
        return false;
    }

    memset(&header, 0, sizeof(header));
    header.uri = NotificationData->nf_instance_uri;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("Cannot parse nfInstanceUri [%s]", header.uri);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Cannot parse nfInstanceUri", header.uri, NULL));
        return false;
    }

    if (!message.h.resource.component[1]) {
        ogs_error("No nfInstanceId [%s]", header.uri);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Cannot parse nfInstanceUri", header.uri, NULL));
        ogs_sbi_header_free(&header);
        return false;
    }

    if (NF_INSTANCE_ID_IS_SELF(message.h.resource.component[1])) {
        ogs_warn("[%s] The notification is not allowed",
                message.h.resource.component[1]);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "The notification is not allowed",
                message.h.resource.component[1], NULL));
        ogs_sbi_header_free(&header);
        return false;
    }

    if (NotificationData->event ==
            OpenAPI_notification_event_type_NF_REGISTERED) {

        OpenAPI_nf_profile_t *NFProfile = NULL;

        NFProfile = NotificationData->nf_profile;
        if (!NFProfile) {
            ogs_error("No NFProfile");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile", NULL, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_instance_id) {
            ogs_error("No NFProfile.NFInstanceId");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFInstanceId", NULL, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_type) {
            ogs_error("No NFProfile.NFType");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFType", NULL, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_status) {
            ogs_error("No NFProfile.NFStatus");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFStatus", NULL, NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        nf_instance = ogs_sbi_nf_instance_find(message.h.resource.component[1]);
        if (!nf_instance) {
            nf_instance = ogs_sbi_nf_instance_add();
            ogs_assert(nf_instance);

            ogs_sbi_nf_instance_set_id(
                    nf_instance, message.h.resource.component[1]);
            ogs_sbi_nf_fsm_init(nf_instance);

            ogs_info("[%s] (NRF-notify) NF registered", nf_instance->id);
        } else {
            ogs_warn("[%s] (NRF-notify) NF has already been added [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            if (!OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_registered)) {
                ogs_error("[%s] (NRF-notify) NF invalid state [type:%s]",
                        nf_instance->id,
                        OpenAPI_nf_type_ToString(nf_instance->nf_type));
            }
        }

        ogs_nnrf_nfm_handle_nf_profile(nf_instance, NFProfile);

        ogs_info("[%s] (NRF-notify) NF Profile updated [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));

        ogs_sbi_client_associate(nf_instance);

        switch (nf_instance->nf_type) {
        case OpenAPI_nf_type_SEPP:
            ogs_sbi_self()->sepp_instance = nf_instance;
            break;
        default:
            break;
        }

    } else if (NotificationData->event ==
            OpenAPI_notification_event_type_NF_DEREGISTERED) {
        nf_instance = ogs_sbi_nf_instance_find(message.h.resource.component[1]);
        if (nf_instance) {
            ogs_info("[%s] (NRF-notify) NF_DEREGISTERED event [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            ogs_sbi_nf_fsm_fini(nf_instance);
            ogs_sbi_nf_instance_remove(nf_instance);
        } else {
            ogs_warn("[%s] (NRF-notify) Not found",
                    message.h.resource.component[1]);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_NOT_FOUND,
                    recvmsg, "Not found", message.h.resource.component[1],
                    NULL));
            ogs_sbi_header_free(&header);
            return false;
        }
    } else {
        char *eventstr = OpenAPI_notification_event_type_ToString(
                            NotificationData->event);
        ogs_error("Not supported event [%d:%s]",
                NotificationData->event, eventstr ? eventstr : "Unknown");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Not supported event",
                eventstr ? eventstr : "Unknown", NULL));
        ogs_sbi_header_free(&header);
        return false;
    }

    response = ogs_sbi_build_response(recvmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_sbi_header_free(&header);
    return true;
}

void ogs_nnrf_disc_handle_nf_discover_search_result(
        OpenAPI_search_result_t *SearchResult)
{
    OpenAPI_lnode_t *node = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(SearchResult);

    OpenAPI_list_for_each(SearchResult->nf_instances, node) {
        OpenAPI_nf_profile_t *NFProfile = NULL;

        if (!node->data) continue;

        NFProfile = node->data;

        if (!NFProfile) {
            ogs_error("No NFProfile");
            continue;
        }

        if (!NFProfile->nf_instance_id) {
            ogs_error("No NFProfile.NFInstanceId");
            continue;
        }

        if (!NFProfile->nf_type) {
            ogs_error("No NFProfile.NFType");
            continue;
        }

        if (!NFProfile->nf_status) {
            ogs_error("No NFProfile.NFStatus");
            continue;
        }

        nf_instance = ogs_sbi_nf_instance_find(NFProfile->nf_instance_id);
        if (!nf_instance) {
            nf_instance = ogs_sbi_nf_instance_add();
            ogs_assert(nf_instance);

            ogs_sbi_nf_instance_set_id(nf_instance, NFProfile->nf_instance_id);
            ogs_sbi_nf_fsm_init(nf_instance);

            ogs_info("[%s] (NRF-discover) NF registered [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
        } else {
            ogs_warn("[%s] (NRF-discover) NF has already been added [type:%s]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            if (!OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_registered)) {
                ogs_error("[%s] (NRF-notify) NF invalid state [type:%s]",
                        nf_instance->id,
                        OpenAPI_nf_type_ToString(nf_instance->nf_type));
            }
        }

        if (NF_INSTANCE_ID_IS_OTHERS(nf_instance->id)) {
            ogs_nnrf_nfm_handle_nf_profile(nf_instance, NFProfile);

            ogs_sbi_client_associate(nf_instance);

            switch (nf_instance->nf_type) {
            case OpenAPI_nf_type_SEPP:
                ogs_sbi_self()->sepp_instance = nf_instance;
                break;
            default:
                break;
            }

            /* TIME : Update validity from NRF */
            if (SearchResult->is_validity_period &&
                SearchResult->validity_period) {
                nf_instance->time.validity_duration =
                        SearchResult->validity_period;

                ogs_assert(nf_instance->t_validity);
                ogs_timer_start(nf_instance->t_validity,
                    ogs_time_from_sec(nf_instance->time.validity_duration));

            } else
                ogs_warn("[%s] NF Instance validity-time should not 0 "
                        "[type:%s]",
                    nf_instance->id,
                    nf_instance->nf_type ?
                        OpenAPI_nf_type_ToString(nf_instance->nf_type) :
                        "NULL");

            ogs_info("[%s] (NF-discover) NF Profile updated "
                    "[type:%s validity:%ds]",
                    nf_instance->id,
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->time.validity_duration);
        }
    }
}
