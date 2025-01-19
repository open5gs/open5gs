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

static OpenAPI_nf_service_t *build_nf_service(
        ogs_sbi_nf_service_t *nf_service);
static void free_nf_service(OpenAPI_nf_service_t *NFService);
static OpenAPI_smf_info_t *build_smf_info(ogs_sbi_nf_info_t *nf_info);
static OpenAPI_amf_info_t *build_amf_info(ogs_sbi_nf_info_t *nf_info);
static OpenAPI_scp_info_t *build_scp_info(ogs_sbi_nf_info_t *nf_info);
static OpenAPI_sepp_info_t *build_sepp_info(ogs_sbi_nf_info_t *nf_info);
static void free_smf_info(OpenAPI_smf_info_t *SmfInfo);
static void free_amf_info(OpenAPI_amf_info_t *AmfInfo);
static void free_scp_info(OpenAPI_scp_info_t *ScpInfo);
static void free_sepp_info(OpenAPI_sepp_info_t *SeppInfo);

ogs_sbi_request_t *ogs_nnrf_nfm_build_register(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_nf_profile_t *NFProfile = NULL;

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

    NFProfile = ogs_nnrf_nfm_build_nf_profile(
                    ogs_sbi_self()->nf_instance, NULL, NULL, true);
    if (!NFProfile) {
        ogs_error("No NFProfile");
        goto end;
    }

    /*
     * TS29510
     * 6.1.6.2 Structured data types
     * 6.1.6.2.2 Type: NFProfile
     * Table 6.1.6.2.2-1: Definition of type NFProfile
     *
     * NF Profile Changes Support Indicator.
     * See Annex B.
     *
     * This IE may be present in the NFRegister or
     * NFUpdate (NF Profile Complete Replacement)
     * request and shall be absent in the response.
     *
     * true: the NF Service Consumer supports receiving
     * NF Profile Changes in the response.
     *
     * false (default): the NF Service Consumer does not
     * support receiving NF Profile Changes in the response.
     */
    NFProfile->is_nf_profile_changes_support_ind = true;
    NFProfile->nf_profile_changes_support_ind = true;

    message.NFProfile = NFProfile;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:

    if (NFProfile)
        ogs_nnrf_nfm_free_nf_profile(NFProfile);

    return request;
}

OpenAPI_nf_profile_t *ogs_nnrf_nfm_build_nf_profile(
        ogs_sbi_nf_instance_t *nf_instance,
        const char *service_name,
        ogs_sbi_discovery_option_t *discovery_option,
        bool service_map)
{
    ogs_sbi_nf_service_t *nf_service = NULL;
    ogs_sbi_nf_info_t *nf_info = NULL;

    OpenAPI_nf_profile_t *NFProfile = NULL;
    OpenAPI_list_t *Ipv4AddrList = NULL;
    OpenAPI_list_t *Ipv6AddrList = NULL;
    OpenAPI_list_t *AllowedNfTypeList = NULL;
    OpenAPI_list_t *NFServiceList = NULL;
    OpenAPI_map_t *NFServiceMap = NULL;
    OpenAPI_list_t *InfoList = NULL;
    OpenAPI_map_t *InfoMap = NULL;
    int InfoMapKey;

    OpenAPI_lnode_t *node = NULL;

    OpenAPI_smf_info_t *SmfInfo = NULL;
    OpenAPI_amf_info_t *AmfInfo = NULL;

    int i = 0;
    char *ipstr = NULL;

    ogs_assert(nf_instance);
    ogs_assert(nf_instance->id);

    NFProfile = ogs_calloc(1, sizeof(*NFProfile));
    if (!NFProfile) {
        ogs_error("No NFProfile");
        return NULL;
    }

    NFProfile->nf_instance_id = nf_instance->id;
    NFProfile->nf_type = nf_instance->nf_type;
    NFProfile->nf_status = nf_instance->nf_status;

    ogs_trace("[%s] ogs_nnrf_nfm_build_nf_profile()", nf_instance->id);

    ogs_trace("NF-Type[%s] NF-Status[%s] IPv4[%d] IPv6[%d]",
                OpenAPI_nf_type_ToString(nf_instance->nf_type),
                OpenAPI_nf_status_ToString(nf_instance->nf_status),
                nf_instance->num_of_ipv4, nf_instance->num_of_ipv6);

    if (nf_instance->time.heartbeat_interval) {
        NFProfile->is_heart_beat_timer = true;
        NFProfile->heart_beat_timer = nf_instance->time.heartbeat_interval;
    }

    if (nf_instance->num_of_plmn_id) {
        OpenAPI_list_t *PlmnIdList = NULL;
        OpenAPI_plmn_id_t *PlmnId = NULL;
        int i;

        PlmnIdList = OpenAPI_list_create();
        ogs_assert(PlmnIdList);

        for (i = 0; i < nf_instance->num_of_plmn_id; i++) {
            PlmnId = ogs_sbi_build_plmn_id(&nf_instance->plmn_id[i]);
            ogs_assert(PlmnId);
            OpenAPI_list_add(PlmnIdList, PlmnId);
        }

        if (PlmnIdList->count)
            NFProfile->plmn_list = PlmnIdList;
        else
            OpenAPI_list_free(PlmnIdList);
    }

    if (nf_instance->fqdn)
        NFProfile->fqdn = ogs_strdup(nf_instance->fqdn);

    NFProfile->is_priority = true;
    NFProfile->priority = nf_instance->priority;
    NFProfile->is_capacity = true;
    NFProfile->capacity = nf_instance->capacity;
    NFProfile->is_load = true;
    NFProfile->load = nf_instance->load;

    Ipv4AddrList = OpenAPI_list_create();
    if (!Ipv4AddrList) {
        ogs_error("No Ipv4AddrList");
        ogs_nnrf_nfm_free_nf_profile(NFProfile);
        return NULL;
    }

    for (i = 0; i < nf_instance->num_of_ipv4; i++) {
        if (nf_instance->ipv4[i]) {
            ogs_trace("IPv4 [family:%d, addr:%x, port:%d]",
                    nf_instance->ipv4[i]->ogs_sa_family,
                    htobe32(nf_instance->ipv4[i]->sin.sin_addr.s_addr),
                    nf_instance->ipv4[i]->ogs_sin_port);
            ogs_assert(nf_instance->ipv4[i]->ogs_sa_family == AF_INET);
            ipstr = ogs_ipstrdup(nf_instance->ipv4[i]);
            if (!ipstr) {
                ogs_error("No ipstr");
                ogs_nnrf_nfm_free_nf_profile(NFProfile);
                OpenAPI_list_free(Ipv4AddrList);
                return NULL;
            }
            OpenAPI_list_add(Ipv4AddrList, ipstr);
        }
    }

    if (Ipv4AddrList->count)
        NFProfile->ipv4_addresses = Ipv4AddrList;
    else
        OpenAPI_list_free(Ipv4AddrList);

    Ipv6AddrList = OpenAPI_list_create();
    if (!Ipv6AddrList) {
        ogs_error("No IPv6AddrList");
        ogs_nnrf_nfm_free_nf_profile(NFProfile);
        return NULL;
    }

    for (i = 0; i < nf_instance->num_of_ipv6; i++) {
        if (nf_instance->ipv6[i]) {
            ogs_trace("IPv6 [family:%d, port:%d]",
                    nf_instance->ipv6[i]->ogs_sa_family,
                    nf_instance->ipv6[i]->ogs_sin_port);
            ogs_assert(nf_instance->ipv6[i]->ogs_sa_family == AF_INET6);
            ipstr = ogs_ipstrdup(nf_instance->ipv6[i]);
            if (!ipstr) {
                ogs_error("No ipstr");
                ogs_nnrf_nfm_free_nf_profile(NFProfile);
                OpenAPI_list_free(Ipv6AddrList);
                return NULL;
            }
            OpenAPI_list_add(Ipv6AddrList, ipstr);
        }
    }

    if (Ipv6AddrList->count)
        NFProfile->ipv6_addresses = Ipv6AddrList;
    else
        OpenAPI_list_free(Ipv6AddrList);

    AllowedNfTypeList = OpenAPI_list_create();
    if (!AllowedNfTypeList) {
        ogs_error("No AllowedNfTypeList");
        ogs_nnrf_nfm_free_nf_profile(NFProfile);
        return NULL;
    }

    for (i = 0; i < nf_instance->num_of_allowed_nf_type; i++) {
        OpenAPI_list_add(AllowedNfTypeList,
                (void *)(uintptr_t)nf_instance->allowed_nf_type[i]);
    }

    if (AllowedNfTypeList->count)
        NFProfile->allowed_nf_types = AllowedNfTypeList;
    else
        OpenAPI_list_free(AllowedNfTypeList);

    NFServiceList = OpenAPI_list_create();
    if (!NFServiceList) {
        ogs_error("No nf_service_list");
        ogs_nnrf_nfm_free_nf_profile(NFProfile);
        return NULL;
    }

    ogs_list_for_each(&nf_instance->nf_service_list, nf_service) {
        OpenAPI_nf_service_t *NFService = NULL;

        if (service_name && nf_service->name &&
            strcmp(service_name, nf_service->name) != 0)
            continue;

        if (discovery_option && discovery_option->num_of_service_names) {
            for (i = 0; i < discovery_option->num_of_service_names; i++) {
                if (nf_service->name &&
                    discovery_option->service_names[i] &&
                    strcmp(nf_service->name,
                        discovery_option->service_names[i]) == 0) {
                    break;
                }
            }

            if (i == discovery_option->num_of_service_names)
                continue;
        }

        NFService = build_nf_service(nf_service);
        if (!NFService) {
            ogs_error("No NFService");
            ogs_nnrf_nfm_free_nf_profile(NFProfile);
            return NULL;
        }

        if (service_map == true) {
            NFServiceMap = OpenAPI_map_create(nf_service->id, NFService);
            if (!NFServiceMap) {
                ogs_error("No NFServiceMap");
                ogs_nnrf_nfm_free_nf_profile(NFProfile);
                return NULL;
            }

            OpenAPI_list_add(NFServiceList, NFServiceMap);
        } else {
            OpenAPI_list_add(NFServiceList, NFService);
        }
    }

    if (NFServiceList->count) {
        if (service_map == true) {
            NFProfile->nf_service_list = NFServiceList;
        } else {
            NFProfile->nf_services = NFServiceList;
        }
    }
    else
        OpenAPI_list_free(NFServiceList);

    InfoList = OpenAPI_list_create();
    ogs_assert(InfoList);

    InfoMapKey = 0;

    ogs_list_for_each(&nf_instance->nf_info_list, nf_info) {
        if (nf_info->nf_type == OpenAPI_nf_type_SMF) {

            if (nf_info->smf.num_of_slice == 0) {
                ogs_fatal("CHECK CONFIGURATION: No S-NSSAI");
                ogs_assert_if_reached();
            }

            SmfInfo = build_smf_info(nf_info);
            ogs_assert(SmfInfo);

            InfoMap = OpenAPI_map_create(
                    ogs_msprintf("%d", ++InfoMapKey), SmfInfo);
            ogs_assert(InfoMap);

            OpenAPI_list_add(InfoList, InfoMap);

        } else if (nf_info->nf_type == OpenAPI_nf_type_AMF) {
            AmfInfo = build_amf_info(nf_info);
            ogs_assert(AmfInfo);

            InfoMap = OpenAPI_map_create(
                    ogs_msprintf("%d", ++InfoMapKey), AmfInfo);
            ogs_assert(InfoMap);

            OpenAPI_list_add(InfoList, InfoMap);

        } else if (nf_info->nf_type == OpenAPI_nf_type_SCP) {

            /* SCP info will be skipped here first and dealt with below. */

        } else if (nf_info->nf_type == OpenAPI_nf_type_SEPP) {

            /* SEPP info will be skipped here first and dealt with below. */

        } else {
            ogs_fatal("Not implemented NF-type[%s]",
                    OpenAPI_nf_type_ToString(nf_info->nf_type));
            ogs_assert_if_reached();
        }
    }
    if (InfoList->count == 1) {
        if (nf_instance->nf_type == OpenAPI_nf_type_SMF) {
            NFProfile->smf_info = SmfInfo;
        } else if (nf_instance->nf_type == OpenAPI_nf_type_AMF) {
            NFProfile->amf_info = AmfInfo;
        } else {
            ogs_fatal("Not implemented NF-type[%s]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            ogs_assert_if_reached();
        }
        OpenAPI_list_for_each(InfoList, node) {
            InfoMap = node->data;
            if (InfoMap) {
                if (InfoMap->key)
                    ogs_free(InfoMap->key);
                ogs_free(InfoMap);
            }
        }
        OpenAPI_list_free(InfoList);
    } else if (InfoList->count > 1) {
        if (nf_instance->nf_type == OpenAPI_nf_type_SMF) {
            NFProfile->smf_info_list = InfoList;
        } else if (nf_instance->nf_type == OpenAPI_nf_type_AMF) {
            NFProfile->amf_info_list = InfoList;
        } else {
            ogs_fatal("Not implemented NF-type[%s]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type));
            ogs_assert_if_reached();
        }
    } else
        OpenAPI_list_free(InfoList);

    /* There can only be one SCP info, not multiple. */
    nf_info = ogs_sbi_nf_info_find(
            &nf_instance->nf_info_list, OpenAPI_nf_type_SCP);
    if (nf_info) {
        NFProfile->scp_info = build_scp_info(nf_info);
        ogs_assert(NFProfile->scp_info);
    }

    /* There can only be one SEPP info, not multiple. */
    nf_info = ogs_sbi_nf_info_find(
            &nf_instance->nf_info_list, OpenAPI_nf_type_SEPP);
    if (nf_info) {
        NFProfile->sepp_info = build_sepp_info(nf_info);
        ogs_assert(NFProfile->sepp_info);
    }

    return NFProfile;
}

void ogs_nnrf_nfm_free_nf_profile(OpenAPI_nf_profile_t *NFProfile)
{
    OpenAPI_map_t *NFServiceMap = NULL;
    OpenAPI_nf_service_t *NFService = NULL;
    OpenAPI_map_t *SmfInfoMap = NULL;
    OpenAPI_smf_info_t *SmfInfo = NULL;
    OpenAPI_map_t *AmfInfoMap = NULL;
    OpenAPI_amf_info_t *AmfInfo = NULL;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(NFProfile);

    if (NFProfile->fqdn)
        ogs_free(NFProfile->fqdn);

    OpenAPI_list_for_each(NFProfile->ipv4_addresses, node)
        ogs_free(node->data);
    OpenAPI_list_free(NFProfile->ipv4_addresses);
    OpenAPI_list_for_each(NFProfile->ipv6_addresses, node)
        ogs_free(node->data);
    OpenAPI_list_free(NFProfile->ipv6_addresses);

    OpenAPI_list_for_each(NFProfile->plmn_list, node) {
        OpenAPI_plmn_id_t *PlmnId = node->data;
        if (PlmnId)
            ogs_sbi_free_plmn_id(PlmnId);
    }
    OpenAPI_list_free(NFProfile->plmn_list);

    OpenAPI_list_free(NFProfile->allowed_nf_types);

    OpenAPI_list_for_each(NFProfile->nf_services, node) {
        NFService = node->data;
        ogs_assert(NFService);
        free_nf_service(NFService);
    }
    OpenAPI_list_free(NFProfile->nf_services);

    OpenAPI_list_for_each(NFProfile->nf_service_list, node) {
        NFServiceMap = node->data;
        if (NFServiceMap) {
            NFService = NFServiceMap->value;
            ogs_assert(NFService);
            free_nf_service(NFService);
            ogs_free(NFServiceMap);
        }
    }
    OpenAPI_list_free(NFProfile->nf_service_list);

    OpenAPI_list_for_each(NFProfile->smf_info_list, node) {
        SmfInfoMap = node->data;
        if (SmfInfoMap) {
            SmfInfo = SmfInfoMap->value;
            if (SmfInfo)
                free_smf_info(SmfInfo);
            if (SmfInfoMap->key)
                ogs_free(SmfInfoMap->key);
            ogs_free(SmfInfoMap);
        }
    }
    OpenAPI_list_free(NFProfile->smf_info_list);

    if (NFProfile->smf_info)
        free_smf_info(NFProfile->smf_info);

    OpenAPI_list_for_each(NFProfile->amf_info_list, node) {
        AmfInfoMap = node->data;
        if (AmfInfoMap) {
            AmfInfo = AmfInfoMap->value;
            if (AmfInfo)
                free_amf_info(AmfInfo);
            if (AmfInfoMap->key)
                ogs_free(AmfInfoMap->key);
            ogs_free(AmfInfoMap);
        }
    }
    OpenAPI_list_free(NFProfile->amf_info_list);

    if (NFProfile->amf_info)
        free_amf_info(NFProfile->amf_info);

    if (NFProfile->scp_info)
        free_scp_info(NFProfile->scp_info);

    if (NFProfile->sepp_info)
        free_sepp_info(NFProfile->sepp_info);

    ogs_free(NFProfile);
}

static OpenAPI_nf_service_t *build_nf_service(
        ogs_sbi_nf_service_t *nf_service)
{
    int i;
    OpenAPI_nf_service_t *NFService = NULL;
    OpenAPI_list_t *VersionList = NULL;
    OpenAPI_list_t *IpEndPointList = NULL;
    OpenAPI_list_t *AllowedNfTypeList = NULL;

    ogs_assert(nf_service);
    ogs_assert(nf_service->id);
    ogs_assert(nf_service->name);

    NFService = ogs_calloc(1, sizeof(*NFService));
    if (!NFService) {
        ogs_error("No NFService");
        return NULL;
    }
    NFService->service_instance_id = ogs_strdup(nf_service->id);
    if (!NFService->service_instance_id) {
        ogs_error("No service_instance_id");
        free_nf_service(NFService);
        return NULL;
    }
    NFService->service_name = ogs_strdup(nf_service->name);
    if (!NFService->service_name) {
        ogs_error("No service_name");
        free_nf_service(NFService);
        return NULL;
    }

    VersionList = OpenAPI_list_create();
    if (!VersionList) {
        ogs_error("No VersionList");
        free_nf_service(NFService);
        return NULL;
    }

    for (i = 0; i < nf_service->num_of_version; i++) {
        OpenAPI_nf_service_version_t *NFServiceVersion = NULL;

        NFServiceVersion = ogs_calloc(1, sizeof(*NFServiceVersion));
        if (!NFServiceVersion) {
            ogs_error("No NFServiceVersion");
            free_nf_service(NFService);
            OpenAPI_list_free(VersionList);
            return NULL;
        }
        if (nf_service->version[i].in_uri) {
            NFServiceVersion->api_version_in_uri =
                ogs_strdup(nf_service->version[i].in_uri);
            if (!NFServiceVersion->api_version_in_uri) {
                ogs_error("No api_version_in_uri");
                if (NFServiceVersion)
                    ogs_free(NFServiceVersion);
                free_nf_service(NFService);
                OpenAPI_list_free(VersionList);
                return NULL;
            }
        }
        if (nf_service->version[i].full) {
            NFServiceVersion->api_full_version =
                ogs_strdup(nf_service->version[i].full);
            if (!NFServiceVersion->api_full_version) {
                ogs_error("No api_full_version");
                if (NFServiceVersion) {
                    if (NFServiceVersion->api_version_in_uri)
                        ogs_free(NFServiceVersion->api_version_in_uri);
                    ogs_free(NFServiceVersion);
                }
                free_nf_service(NFService);
                OpenAPI_list_free(VersionList);
                return NULL;
            }
        }
        if (nf_service->version[i].expiry) {
            NFServiceVersion->expiry =
                ogs_strdup(nf_service->version[i].expiry);
            if (!NFServiceVersion->expiry) {
                ogs_error("No expiry");
                if (NFServiceVersion) {
                    if (NFServiceVersion->api_version_in_uri)
                        ogs_free(NFServiceVersion->api_version_in_uri);
                    if (NFServiceVersion->api_full_version)
                        ogs_free(NFServiceVersion->api_full_version);
                    ogs_free(NFServiceVersion);
                }
                free_nf_service(NFService);
                OpenAPI_list_free(VersionList);
                return NULL;
            }
        }

        OpenAPI_list_add(VersionList, NFServiceVersion);
    }

    ogs_assert(VersionList->count);
    NFService->versions = VersionList;

    NFService->scheme = nf_service->scheme;
    NFService->nf_service_status = nf_service->status;

    if (nf_service->fqdn)
        NFService->fqdn = ogs_strdup(nf_service->fqdn);

    IpEndPointList = OpenAPI_list_create();
    if (!IpEndPointList) {
        ogs_error("No IpEndPointList");
        free_nf_service(NFService);
        return NULL;
    }

    for (i = 0; i < nf_service->num_of_addr; i++) {
        ogs_sockaddr_t *ipv4 = NULL;
        ogs_sockaddr_t *ipv6 = NULL;

        OpenAPI_ip_end_point_t *IpEndPoint = NULL;

        ipv4 = nf_service->addr[i].ipv4;
        ipv6 = nf_service->addr[i].ipv6;

        if (ipv4 || ipv6) {
            IpEndPoint = ogs_calloc(1, sizeof(*IpEndPoint));
            if (!IpEndPoint) {
                ogs_error("No IpEndPoint");
                free_nf_service(NFService);
                OpenAPI_list_free(IpEndPointList);
                return NULL;
            }
            if (ipv4) {
                IpEndPoint->ipv4_address = ogs_ipstrdup(ipv4);
                if (!IpEndPoint->ipv4_address) {
                    ogs_error("No ipv4_address");
                    if (IpEndPoint)
                        ogs_free(IpEndPoint);
                    free_nf_service(NFService);
                    OpenAPI_list_free(IpEndPointList);
                    return NULL;
                }
            }
            if (ipv6) {
                IpEndPoint->ipv6_address = ogs_ipstrdup(ipv6);
                if (!IpEndPoint->ipv6_address) {
                    ogs_error("No ipv6_address");
                    if (IpEndPoint) {
                        if (IpEndPoint->ipv4_address)
                            ogs_free(IpEndPoint->ipv4_address);
                        ogs_free(IpEndPoint);
                    }
                    free_nf_service(NFService);
                    OpenAPI_list_free(IpEndPointList);
                    return NULL;
                }
            }
            IpEndPoint->is_port = nf_service->addr[i].is_port;
            IpEndPoint->port = nf_service->addr[i].port;
            OpenAPI_list_add(IpEndPointList, IpEndPoint);
        }
    }

    if (IpEndPointList->count)
        NFService->ip_end_points = IpEndPointList;
    else
        OpenAPI_list_free(IpEndPointList);

    AllowedNfTypeList = OpenAPI_list_create();
    if (!AllowedNfTypeList) {
        ogs_error("No AllowedNfTypeList");
        free_nf_service(NFService);
        return NULL;
    }

    for (i = 0; i < nf_service->num_of_allowed_nf_type; i++) {
        OpenAPI_list_add(AllowedNfTypeList,
                (void *)(uintptr_t)nf_service->allowed_nf_type[i]);
    }

    if (AllowedNfTypeList->count)
        NFService->allowed_nf_types = AllowedNfTypeList;
    else
        OpenAPI_list_free(AllowedNfTypeList);

    NFService->is_priority = true;
    NFService->priority = nf_service->priority;
    NFService->is_capacity = true;
    NFService->capacity = nf_service->capacity;
    NFService->is_load = true;
    NFService->load = nf_service->load;

    return NFService;
}

static void free_nf_service(OpenAPI_nf_service_t *NFService)
{
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(NFService);

    ogs_free(NFService->service_instance_id);
    ogs_free(NFService->service_name);

    OpenAPI_list_for_each(NFService->versions, node) {
        OpenAPI_nf_service_version_t *NFServiceVersion = node->data;
        ogs_assert(NFServiceVersion);
        ogs_free(NFServiceVersion->api_version_in_uri);
        ogs_free(NFServiceVersion->api_full_version);
        if (NFServiceVersion->expiry)
            ogs_free(NFServiceVersion->expiry);
        ogs_free(NFServiceVersion);
    }
    OpenAPI_list_free(NFService->versions);

    OpenAPI_list_for_each(NFService->ip_end_points, node) {
        OpenAPI_ip_end_point_t *IpEndPoint = node->data;
        ogs_assert(IpEndPoint);
        if (IpEndPoint->ipv4_address)
            ogs_free(IpEndPoint->ipv4_address);
        if (IpEndPoint->ipv6_address)
            ogs_free(IpEndPoint->ipv6_address);
        ogs_free(IpEndPoint);
    }
    OpenAPI_list_free(NFService->ip_end_points);

    OpenAPI_list_free(NFService->allowed_nf_types);

    if (NFService->fqdn)
        ogs_free(NFService->fqdn);

    ogs_free(NFService);
}

static OpenAPI_smf_info_t *build_smf_info(ogs_sbi_nf_info_t *nf_info)
{
    int i, j;
    OpenAPI_smf_info_t *SmfInfo = NULL;

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

    ogs_assert(nf_info);

    SmfInfo = ogs_calloc(1, sizeof(*SmfInfo));
    if (!SmfInfo) {
        ogs_error("No SmfInfo");
        return NULL;
    }

    sNssaiSmfInfoList = OpenAPI_list_create();
    if (!sNssaiSmfInfoList) {
        ogs_error("No sNssaiSmfInfoList");
        free_smf_info(SmfInfo);
        return NULL;
    }

    for (i = 0; i < nf_info->smf.num_of_slice; i++) {
        DnnSmfInfoList = OpenAPI_list_create();
        if (!DnnSmfInfoList) {
            ogs_error("No DnnSmfInfoList");
            free_smf_info(SmfInfo);
            OpenAPI_list_free(sNssaiSmfInfoList);
            return NULL;
        }

        for (j = 0; j < nf_info->smf.slice[i].num_of_dnn; j++) {
            DnnSmfInfoItem = ogs_calloc(1, sizeof(*DnnSmfInfoItem));
            ogs_assert(DnnSmfInfoItem);
            DnnSmfInfoItem->dnn = nf_info->smf.slice[i].dnn[j];

            OpenAPI_list_add(DnnSmfInfoList, DnnSmfInfoItem);
        }

        if (!DnnSmfInfoList->count) {
            ogs_error("CHECK CONFIGURATION: No DNN");

            OpenAPI_list_free(DnnSmfInfoList);
            free_smf_info(SmfInfo);
            OpenAPI_list_free(sNssaiSmfInfoList);

            return NULL;
        }

        sNssaiSmfInfoItem = ogs_calloc(1, sizeof(*sNssaiSmfInfoItem));
        ogs_assert(sNssaiSmfInfoItem);

        sNssaiSmfInfoItem->dnn_smf_info_list = DnnSmfInfoList;

        sNssaiSmfInfoItem->s_nssai = sNssai = ogs_calloc(1, sizeof(*sNssai));
        ogs_assert(sNssai);
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
    if (!TaiList) {
        ogs_error("No TaiList");
        free_smf_info(SmfInfo);
        return NULL;
    }

    for (i = 0; i < nf_info->smf.num_of_nr_tai; i++) {
        TaiItem = ogs_calloc(1, sizeof(*TaiItem));
        if (!TaiItem) {
            ogs_error("No TaiItem");
            free_smf_info(SmfInfo);
            OpenAPI_list_free(TaiList);
            return NULL;
        }
        TaiItem->plmn_id = ogs_sbi_build_plmn_id(
                &nf_info->smf.nr_tai[i].plmn_id);
        if (!TaiItem->plmn_id) {
            ogs_error("No TaiItem->plmn_id");
            if (TaiItem)
                ogs_free(TaiItem);
            free_smf_info(SmfInfo);
            OpenAPI_list_free(TaiList);
            return NULL;
        }
        TaiItem->tac =
            ogs_uint24_to_0string(nf_info->smf.nr_tai[i].tac);
        if (!TaiItem->tac) {
            ogs_error("No TaiItem->tac");
            if (TaiItem) {
                if (TaiItem->plmn_id)
                    ogs_sbi_free_plmn_id(TaiItem->plmn_id);
                ogs_free(TaiItem);
            }
            free_smf_info(SmfInfo);
            OpenAPI_list_free(TaiList);
            return NULL;
        }

        OpenAPI_list_add(TaiList, TaiItem);
    }

    if (TaiList->count)
        SmfInfo->tai_list = TaiList;
    else
        OpenAPI_list_free(TaiList);

    TaiRangeList = OpenAPI_list_create();
    if (!TaiRangeList) {
        ogs_error("No TaiRangeList");
        free_smf_info(SmfInfo);
        return NULL;
    }

    for (i = 0; i < nf_info->smf.num_of_nr_tai_range; i++) {
        TacRangeList = OpenAPI_list_create();
        if (!TacRangeList) {
            ogs_error("No TacRangeList");
            free_smf_info(SmfInfo);
            OpenAPI_list_free(TaiRangeList);
            return NULL;
        }

        for (j = 0;
                j < nf_info->smf.nr_tai_range[i].num_of_tac_range;
                j++) {
            TacRangeItem = ogs_calloc(1, sizeof(*TacRangeItem));
            ogs_assert(TacRangeItem);

            TacRangeItem->start = ogs_uint24_to_0string(
                    nf_info->smf.nr_tai_range[i].start[j]);
            ogs_assert(TacRangeItem->start);
            TacRangeItem->end =
                ogs_uint24_to_0string(
                        nf_info->smf.nr_tai_range[i].end[j]);
            ogs_assert(TacRangeItem->end);

            OpenAPI_list_add(TacRangeList, TacRangeItem);
        }

        if (!TacRangeList->count) {
            ogs_error("CHECK CONFIGURATION: No Start/End in TacRange");

            OpenAPI_list_free(TacRangeList);
            free_smf_info(SmfInfo);
            OpenAPI_list_free(TaiRangeList);

            return NULL;
        }

        TaiRangeItem = ogs_calloc(1, sizeof(*TaiRangeItem));
        ogs_assert(TaiRangeItem);

        TaiRangeItem->plmn_id = ogs_sbi_build_plmn_id(
                &nf_info->smf.nr_tai_range[i].plmn_id);
        ogs_assert(TaiRangeItem->plmn_id);

        TaiRangeItem->tac_range_list = TacRangeList;

        OpenAPI_list_add(TaiRangeList, TaiRangeItem);
    }

    if (TaiRangeList->count)
        SmfInfo->tai_range_list = TaiRangeList;
    else
        OpenAPI_list_free(TaiRangeList);

    return SmfInfo;
}

static OpenAPI_amf_info_t *build_amf_info(ogs_sbi_nf_info_t *nf_info)
{
    int i, j;
    OpenAPI_amf_info_t *AmfInfo = NULL;

    OpenAPI_list_t *guamiAmfInfoList = NULL;
    OpenAPI_guami_t *guamiAmfInfoItem = NULL;

    OpenAPI_list_t *TaiList = NULL;
    OpenAPI_tai_t *TaiItem = NULL;
    OpenAPI_list_t *TaiRangeList = NULL;
    OpenAPI_tai_range_t *TaiRangeItem = NULL;
    OpenAPI_list_t *TacRangeList = NULL;
    OpenAPI_tac_range_t *TacRangeItem = NULL;

    ogs_assert(nf_info);

    AmfInfo = ogs_calloc(1, sizeof(*AmfInfo));
    if (!AmfInfo) {
        ogs_error("No AmfInfo");
        return NULL;
    }

    AmfInfo->amf_set_id = ogs_msprintf("%03x", nf_info->amf.amf_set_id);
    if (!AmfInfo->amf_set_id) {
        ogs_error("No amf_set_id");
        free_amf_info(AmfInfo);
        return NULL;
    }
    AmfInfo->amf_region_id = ogs_msprintf("%02x", nf_info->amf.amf_region_id);
    if (!AmfInfo->amf_region_id) {
        ogs_error("No amf_region_id");
        free_amf_info(AmfInfo);
        return NULL;
    }

    guamiAmfInfoList = OpenAPI_list_create();
    if (!guamiAmfInfoList) {
        ogs_error("No guamiAmfInfoList");
        free_amf_info(AmfInfo);
        return NULL;
    }

    for (i = 0; i < nf_info->amf.num_of_guami; i++) {

        guamiAmfInfoItem = ogs_calloc(1, sizeof(*guamiAmfInfoItem));
        if (!guamiAmfInfoItem) {
            ogs_error("guamiAmfInfoItem");
            free_amf_info(AmfInfo);
            OpenAPI_list_free(guamiAmfInfoList);
            return NULL;
        }

        guamiAmfInfoItem->plmn_id =
                ogs_sbi_build_plmn_id_nid(&nf_info->amf.guami[i].plmn_id);
        if (!guamiAmfInfoItem->plmn_id) {
            ogs_error("guamiAmfInfoItem->plmn_id");
            if (guamiAmfInfoItem)
                ogs_free(guamiAmfInfoItem);
            free_amf_info(AmfInfo);
            OpenAPI_list_free(guamiAmfInfoList);
            return NULL;
        }
        guamiAmfInfoItem->amf_id =
                ogs_amf_id_to_string(&nf_info->amf.guami[i].amf_id);
        if (!guamiAmfInfoItem->amf_id) {
            ogs_error("guamiAmfInfoItem->amf_id");
            if (guamiAmfInfoItem) {
                if (guamiAmfInfoItem->plmn_id)
                    ogs_free(guamiAmfInfoItem->plmn_id);
                ogs_free(guamiAmfInfoItem);
            }
            free_amf_info(AmfInfo);
            OpenAPI_list_free(guamiAmfInfoList);
            return NULL;
        }

        OpenAPI_list_add(guamiAmfInfoList, guamiAmfInfoItem);
    }

    if (guamiAmfInfoList->count)
        AmfInfo->guami_list = guamiAmfInfoList;
    else
        OpenAPI_list_free(guamiAmfInfoList);

    TaiList = OpenAPI_list_create();
    if (!TaiList) {
        ogs_error("No TaiList");
        free_amf_info(AmfInfo);
        return NULL;
    }

    for (i = 0; i < nf_info->amf.num_of_nr_tai; i++) {
        TaiItem = ogs_calloc(1, sizeof(*TaiItem));
        if (!TaiList) {
            ogs_error("No TaiItem");
            free_amf_info(AmfInfo);
            OpenAPI_list_free(TaiList);
            return NULL;
        }
        TaiItem->plmn_id = ogs_sbi_build_plmn_id(
                &nf_info->amf.nr_tai[i].plmn_id);
        if (!TaiItem->plmn_id) {
            ogs_error("No TaiItem->plmn_id");
            if (TaiItem)
                ogs_free(TaiItem);
            free_amf_info(AmfInfo);
            OpenAPI_list_free(TaiList);
            return NULL;
        }
        TaiItem->tac =
            ogs_uint24_to_0string(nf_info->amf.nr_tai[i].tac);
        if (!TaiItem->tac) {
            ogs_error("No TaiItem->tac");
            if (TaiItem) {
                if (TaiItem->plmn_id)
                    ogs_sbi_free_plmn_id(TaiItem->plmn_id);
                ogs_free(TaiItem);
            }
            free_amf_info(AmfInfo);
            OpenAPI_list_free(TaiList);
            return NULL;
        }

        OpenAPI_list_add(TaiList, TaiItem);
    }

    if (TaiList->count)
        AmfInfo->tai_list = TaiList;
    else
        OpenAPI_list_free(TaiList);

    TaiRangeList = OpenAPI_list_create();
    if (!TaiRangeList) {
        ogs_error("No TaiRangeList");
        free_amf_info(AmfInfo);
        return NULL;
    }

    for (i = 0; i < nf_info->amf.num_of_nr_tai_range; i++) {
        TacRangeList = OpenAPI_list_create();
        if (!TacRangeList) {
            ogs_error("No TacRangeList");
            free_amf_info(AmfInfo);
            OpenAPI_list_free(TaiRangeList);
            return NULL;
        }

        for (j = 0;
                j < nf_info->amf.nr_tai_range[i].num_of_tac_range;
                j++) {
            TacRangeItem = ogs_calloc(1, sizeof(*TacRangeItem));
            ogs_assert(TacRangeItem);

            TacRangeItem->start = ogs_uint24_to_0string(
                    nf_info->amf.nr_tai_range[i].start[j]);
            ogs_assert(TacRangeItem->start);
            TacRangeItem->end =
                ogs_uint24_to_0string(
                        nf_info->amf.nr_tai_range[i].end[j]);
            ogs_assert(TacRangeItem->end);

            OpenAPI_list_add(TacRangeList, TacRangeItem);
        }

        if (!TacRangeList->count) {
            ogs_error("CHECK CONFIGURATION: No Start/End in TacRange");

            OpenAPI_list_free(TacRangeList);
            free_amf_info(AmfInfo);
            OpenAPI_list_free(TaiRangeList);

            return NULL;
        }

        TaiRangeItem = ogs_calloc(1, sizeof(*TaiRangeItem));
        if (!TaiRangeItem) {
            ogs_error("No TaiRangeItem");
            free_amf_info(AmfInfo);
            OpenAPI_list_free(TaiRangeList);
            return NULL;
        }

        TaiRangeItem->plmn_id = ogs_sbi_build_plmn_id(
                &nf_info->amf.nr_tai_range[i].plmn_id);
        if (!TaiRangeItem->plmn_id) {
            ogs_error("No TaiRangeItem->plmn_id");
            ogs_free(TaiRangeItem);
            free_amf_info(AmfInfo);
            OpenAPI_list_free(TaiRangeList);
            return NULL;
        }

        TaiRangeItem->tac_range_list = TacRangeList;

        OpenAPI_list_add(TaiRangeList, TaiRangeItem);
    }

    if (TaiRangeList->count)
        AmfInfo->tai_range_list = TaiRangeList;
    else
        OpenAPI_list_free(TaiRangeList);

    return AmfInfo;
}

static OpenAPI_scp_info_t *build_scp_info(ogs_sbi_nf_info_t *nf_info)
{
    int i;
    OpenAPI_scp_info_t *ScpInfo = NULL;
    OpenAPI_list_t *PortList = NULL;
    OpenAPI_map_t *PortMap = NULL;

    OpenAPI_list_t *DomainInfoList = NULL;
    OpenAPI_map_t *DomainInfoMap = NULL;
    OpenAPI_scp_domain_info_t *DomainInfo = NULL;

    ogs_assert(nf_info);

    ScpInfo = ogs_calloc(1, sizeof(*ScpInfo));
    if (!ScpInfo) {
        ogs_error("No ScpInfo");
        return NULL;
    }

    PortList = OpenAPI_list_create();
    if (!PortList) {
        ogs_error("No PortList");
        free_scp_info(ScpInfo);
        return NULL;
    }

    if (nf_info->scp.http.presence) {
        PortMap = OpenAPI_map_create(
                    (char *)"http", ogs_alloc_double(nf_info->scp.http.port));
        if (!PortMap) {
            ogs_error("No PortMap");
            free_scp_info(ScpInfo);
            return NULL;
        }
        OpenAPI_list_add(PortList, PortMap);
    }
    if (nf_info->scp.https.presence) {
        PortMap = OpenAPI_map_create(
                    (char *)"https", ogs_alloc_double(nf_info->scp.https.port));
        if (!PortMap) {
            ogs_error("No PortMap");
            free_scp_info(ScpInfo);
            return NULL;
        }
        OpenAPI_list_add(PortList, PortMap);
    }

    if (PortList->count)
        ScpInfo->scp_ports = PortList;
    else
        OpenAPI_list_free(PortList);

    DomainInfoList = OpenAPI_list_create();
    if (!DomainInfoList) {
        ogs_error("No DomainInfoList");
        free_scp_info(ScpInfo);
        return NULL;
    }
    for (i = 0; i < nf_info->scp.num_of_domain; i++) {
        ogs_assert(nf_info->scp.domain[i].name);

        DomainInfo = ogs_calloc(1, sizeof(*DomainInfo));
        if (!DomainInfo) {
            ogs_error("No DomainInfo");
            free_scp_info(ScpInfo);
            return NULL;
        }

        DomainInfo->scp_fqdn = nf_info->scp.domain[i].fqdn;

        PortList = OpenAPI_list_create();
        if (!PortList) {
            ogs_error("No PortList");
            free_scp_info(ScpInfo);
            return NULL;
        }

        if (nf_info->scp.domain[i].http.presence) {
            PortMap = OpenAPI_map_create(
                        (char *)"http",
                        ogs_alloc_double(nf_info->scp.domain[i].http.port));
            if (!PortMap) {
                ogs_error("No PortMap");
                free_scp_info(ScpInfo);
                return NULL;
            }
            OpenAPI_list_add(PortList, PortMap);
        }
        if (nf_info->scp.domain[i].https.presence) {
            PortMap = OpenAPI_map_create(
                        (char *)"https",
                        ogs_alloc_double(nf_info->scp.domain[i].https.port));
            if (!PortMap) {
                ogs_error("No PortMap");
                free_scp_info(ScpInfo);
                return NULL;
            }
            OpenAPI_list_add(PortList, PortMap);
        }

        if (PortList->count)
            DomainInfo->scp_ports = PortList;
        else
            OpenAPI_list_free(PortList);

        DomainInfoMap = OpenAPI_map_create(
                    nf_info->scp.domain[i].name, DomainInfo);
        if (!DomainInfoMap) {
            ogs_error("No PortMap");
            free_scp_info(ScpInfo);
            return NULL;
        }

        OpenAPI_list_add(DomainInfoList, DomainInfoMap);
    }

    if (DomainInfoList->count)
        ScpInfo->scp_domain_info_list = DomainInfoList;
    else
        OpenAPI_list_free(DomainInfoList);

    return ScpInfo;
}

static OpenAPI_sepp_info_t *build_sepp_info(ogs_sbi_nf_info_t *nf_info)
{
    OpenAPI_sepp_info_t *SeppInfo = NULL;
    OpenAPI_list_t *PortList = NULL;
    OpenAPI_map_t *PortMap = NULL;

    ogs_assert(nf_info);

    SeppInfo = ogs_calloc(1, sizeof(*SeppInfo));
    if (!SeppInfo) {
        ogs_error("No SeppInfo");
        return NULL;
    }

    PortList = OpenAPI_list_create();
    if (!PortList) {
        ogs_error("No PortList");
        free_sepp_info(SeppInfo);
        return NULL;
    }

    if (nf_info->sepp.http.presence) {
        PortMap = OpenAPI_map_create(
                    (char *)"http", ogs_alloc_double(nf_info->sepp.http.port));
        if (!PortMap) {
            ogs_error("No PortMap");
            free_sepp_info(SeppInfo);
            return NULL;
        }
        OpenAPI_list_add(PortList, PortMap);
    }
    if (nf_info->sepp.https.presence) {
        PortMap = OpenAPI_map_create(
                    (char *)"https", ogs_alloc_double(nf_info->sepp.https.port));
        if (!PortMap) {
            ogs_error("No PortMap");
            free_sepp_info(SeppInfo);
            return NULL;
        }
        OpenAPI_list_add(PortList, PortMap);
    }

    if (PortList->count)
        SeppInfo->sepp_ports = PortList;
    else
        OpenAPI_list_free(PortList);

    return SeppInfo;
}

static void free_smf_info(OpenAPI_smf_info_t *SmfInfo)
{
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

    ogs_assert(SmfInfo);

    sNssaiSmfInfoList = SmfInfo->s_nssai_smf_info_list;
    OpenAPI_list_for_each(sNssaiSmfInfoList, node) {
        sNssaiSmfInfoItem = node->data;
        ogs_assert(sNssaiSmfInfoItem);

        DnnSmfInfoList = sNssaiSmfInfoItem->dnn_smf_info_list;
        OpenAPI_list_for_each(DnnSmfInfoList, node2) {
            DnnSmfInfoItem = node2->data;
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
    OpenAPI_list_for_each(TaiList, node) {
        TaiItem = node->data;
        ogs_assert(TaiItem);
        if (TaiItem->plmn_id)
            ogs_sbi_free_plmn_id(TaiItem->plmn_id);
        if (TaiItem->tac)
            ogs_free(TaiItem->tac);
        ogs_free(TaiItem);
    }
    OpenAPI_list_free(TaiList);

    TaiRangeList = SmfInfo->tai_range_list;
    OpenAPI_list_for_each(TaiRangeList, node) {
        TaiRangeItem = node->data;
        ogs_assert(TaiRangeItem);

        if (TaiRangeItem->plmn_id)
            ogs_sbi_free_plmn_id(TaiRangeItem->plmn_id);

        TacRangeList = TaiRangeItem->tac_range_list;
        OpenAPI_list_for_each(TacRangeList, node2) {
            TacRangeItem = node2->data;
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

static void free_amf_info(OpenAPI_amf_info_t *AmfInfo)
{
    OpenAPI_list_t *guamiAmfInfoList = NULL;
    OpenAPI_guami_t *guamiAmfInfoItem = NULL;

    OpenAPI_list_t *TaiList = NULL;
    OpenAPI_tai_t *TaiItem = NULL;
    OpenAPI_list_t *TaiRangeList = NULL;
    OpenAPI_tai_range_t *TaiRangeItem = NULL;
    OpenAPI_list_t *TacRangeList = NULL;
    OpenAPI_tac_range_t *TacRangeItem = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    ogs_assert(AmfInfo);

    if (AmfInfo->amf_set_id)
        ogs_free(AmfInfo->amf_set_id);
    if (AmfInfo->amf_region_id)
        ogs_free(AmfInfo->amf_region_id);

    guamiAmfInfoList = AmfInfo->guami_list;
    OpenAPI_list_for_each(guamiAmfInfoList, node) {
        guamiAmfInfoItem = node->data;
        if (guamiAmfInfoItem) {
            if (guamiAmfInfoItem->plmn_id) {
                if (guamiAmfInfoItem->plmn_id->mcc)
                    ogs_free(guamiAmfInfoItem->plmn_id->mcc);
                if (guamiAmfInfoItem->plmn_id->mnc)
                    ogs_free(guamiAmfInfoItem->plmn_id->mnc);
                ogs_free(guamiAmfInfoItem->plmn_id);
            }
            if (guamiAmfInfoItem->amf_id)
                ogs_free(guamiAmfInfoItem->amf_id);
        }
        ogs_free(guamiAmfInfoItem);
    }

    OpenAPI_list_free(guamiAmfInfoList);

    TaiList = AmfInfo->tai_list;
    OpenAPI_list_for_each(TaiList, node) {
        TaiItem = node->data;
        ogs_assert(TaiItem);
        if (TaiItem->plmn_id)
            ogs_sbi_free_plmn_id(TaiItem->plmn_id);
        if (TaiItem->tac)
            ogs_free(TaiItem->tac);
        ogs_free(TaiItem);
    }
    OpenAPI_list_free(TaiList);

    TaiRangeList = AmfInfo->tai_range_list;
    OpenAPI_list_for_each(TaiRangeList, node) {
        TaiRangeItem = node->data;
        ogs_assert(TaiRangeItem);

        if (TaiRangeItem->plmn_id)
            ogs_sbi_free_plmn_id(TaiRangeItem->plmn_id);

        TacRangeList = TaiRangeItem->tac_range_list;
        OpenAPI_list_for_each(TacRangeList, node2) {
            TacRangeItem = node2->data;
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

    ogs_free(AmfInfo);
}

static void free_scp_info(OpenAPI_scp_info_t *ScpInfo)
{
    OpenAPI_map_t *PortMap = NULL;
    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    OpenAPI_map_t *DomainInfoMap = NULL;
    OpenAPI_scp_domain_info_t *DomainInfo = NULL;

    ogs_assert(ScpInfo);

    OpenAPI_list_for_each(ScpInfo->scp_ports, node) {
        PortMap = node->data;
        if (PortMap) {
            ogs_free(PortMap->value);
            OpenAPI_map_free(PortMap);
        }
    }
    OpenAPI_list_free(ScpInfo->scp_ports);

    OpenAPI_list_for_each(ScpInfo->scp_domain_info_list, node) {
        DomainInfoMap = node->data;
        if (DomainInfoMap) {
            DomainInfo = DomainInfoMap->value;
            if (DomainInfo) {
                OpenAPI_list_for_each(DomainInfo->scp_ports, node2) {
                    PortMap = node2->data;
                    if (PortMap) {
                        ogs_free(PortMap->value);
                        OpenAPI_map_free(PortMap);
                    }
                }
                OpenAPI_list_free(DomainInfo->scp_ports);
                ogs_free(DomainInfo);
            }
            OpenAPI_map_free(DomainInfoMap);
        }
    }
    OpenAPI_list_free(ScpInfo->scp_domain_info_list);

    ogs_free(ScpInfo);
}

static void free_sepp_info(OpenAPI_sepp_info_t *SeppInfo)
{
    OpenAPI_map_t *PortMap = NULL;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(SeppInfo);

    OpenAPI_list_for_each(SeppInfo->sepp_ports, node) {
        PortMap = node->data;
        if (PortMap) {
            ogs_free(PortMap->value);
            OpenAPI_map_free(PortMap);
        }
    }
    OpenAPI_list_free(SeppInfo->sepp_ports);

    ogs_free(SeppInfo);
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_update(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_list_t *PatchItemList = NULL;
    OpenAPI_patch_item_t StatusItem;
    OpenAPI_patch_item_t LoadItem;

    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_assert(nf_instance->id);

    memset(&StatusItem, 0, sizeof(StatusItem));
    memset(&LoadItem, 0, sizeof(LoadItem));

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PATCH;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = nf_instance->id;

    message.http.content_type = (char *)OGS_SBI_CONTENT_PATCH_TYPE;

    PatchItemList = OpenAPI_list_create();
    if (!PatchItemList) {
        ogs_error("No PatchItemList");
        goto end;
    }

    StatusItem.op = OpenAPI_patch_operation_replace;
    StatusItem.path = (char *)OGS_SBI_PATCH_PATH_NF_STATUS;
    StatusItem.value = OpenAPI_any_type_create_string(
        OpenAPI_nf_status_ToString(OpenAPI_nf_status_REGISTERED));
    if (!StatusItem.value) {
        ogs_error("No status item.value");
        goto end;
    }

    OpenAPI_list_add(PatchItemList, &StatusItem);

    LoadItem.op = OpenAPI_patch_operation_replace;
    LoadItem.path = (char *)OGS_SBI_PATCH_PATH_LOAD;
    LoadItem.value = OpenAPI_any_type_create_number(nf_instance->load);
    if (!LoadItem.value) {
        ogs_error("No load item.value");
        goto end;
    }

    OpenAPI_list_add(PatchItemList, &LoadItem);

    message.PatchItemList = PatchItemList;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (LoadItem.value)
        OpenAPI_any_type_free(LoadItem.value);
    if (StatusItem.value)
        OpenAPI_any_type_free(StatusItem.value);
    if (PatchItemList)
        OpenAPI_list_free(PatchItemList);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_de_register(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_assert(nf_instance->id);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = nf_instance->id;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_status_subscribe(
        ogs_sbi_subscription_data_t *subscription_data)
{
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_subscription_data_t *SubscriptionData = NULL;
    OpenAPI_subscr_cond_t SubscrCond;

    ogs_assert(subscription_data);
    ogs_assert(subscription_data->req_nf_type);

    /* Issue #2630 : The format of subscrCond is invalid. Must be 'oneOf'. */
    ogs_assert(!subscription_data->subscr_cond.nf_type ||
            !subscription_data->subscr_cond.service_name);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS;

    SubscriptionData = ogs_calloc(1, sizeof(*SubscriptionData));
    if (!SubscriptionData) {
        ogs_error("No SubscriptionData");
        goto end;
    }

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
            (char *)OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY;
    SubscriptionData->nf_status_notification_uri =
                        ogs_sbi_server_uri(server, &header);
    if (!SubscriptionData->nf_status_notification_uri) {
        ogs_error("No nf_status_notification_uri");
        goto end;
    }

    SubscriptionData->req_nf_type = subscription_data->req_nf_type;
    SubscriptionData->req_nf_instance_id =
        subscription_data->req_nf_instance_id;

    OGS_SBI_FEATURES_SET(subscription_data->requester_features,
            OGS_SBI_NNRF_NFM_SERVICE_MAP);
    SubscriptionData->requester_features =
        ogs_uint64_to_string(subscription_data->requester_features);
    if (!SubscriptionData->requester_features) {
        ogs_error("No requester_features");
        goto end;
    }

    memset(&SubscrCond, 0, sizeof(SubscrCond));
    if (subscription_data->subscr_cond.nf_type) {
        SubscrCond.nf_type = subscription_data->subscr_cond.nf_type;
        SubscriptionData->subscr_cond = &SubscrCond;
    } else if (subscription_data->subscr_cond.service_name) {
        SubscrCond.service_name = subscription_data->subscr_cond.service_name;
        SubscriptionData->subscr_cond = &SubscrCond;
    } else {
        ogs_fatal("SubscrCond must be 'oneOf'.");
        ogs_assert_if_reached();
    }

    message.SubscriptionData = SubscriptionData;

    message.http.custom.callback =
        (char *)OGS_SBI_CALLBACK_NNRF_NFMANAGEMENT_NF_STATUS_NOTIFY;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (SubscriptionData) {
        if (SubscriptionData->nf_status_notification_uri)
            ogs_free(SubscriptionData->nf_status_notification_uri);
        if (SubscriptionData->requester_features)
            ogs_free(SubscriptionData->requester_features);
        ogs_free(SubscriptionData);
    }

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_status_update(
        ogs_sbi_subscription_data_t *subscription_data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_list_t *PatchItemList = NULL;
    OpenAPI_patch_item_t ValidityItem;
    char *validity_time = NULL;

    ogs_assert(subscription_data);
    ogs_assert(subscription_data->id);

    memset(&ValidityItem, 0, sizeof(ValidityItem));

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PATCH;
    message.h.uri = subscription_data->resource_uri;

    message.http.content_type = (char *)OGS_SBI_CONTENT_PATCH_TYPE;

    PatchItemList = OpenAPI_list_create();
    if (!PatchItemList) {
        ogs_error("No PatchItemList");
        goto end;
    }

    ogs_assert(subscription_data->validity_duration);
    validity_time = ogs_sbi_localtime_string(
            ogs_time_now() + subscription_data->validity_duration);
    ogs_assert(validity_time);

    ValidityItem.op = OpenAPI_patch_operation_replace;
    ValidityItem.path = (char *)OGS_SBI_PATCH_PATH_VALIDITY_TIME;
    ValidityItem.value = OpenAPI_any_type_create_string(validity_time);

    if (!ValidityItem.value) {
        ogs_error("No status item.value");
        goto end;
    }

    OpenAPI_list_add(PatchItemList, &ValidityItem);

    message.PatchItemList = PatchItemList;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (ValidityItem.value)
        OpenAPI_any_type_free(ValidityItem.value);
    if (validity_time)
        ogs_free(validity_time);

    if (PatchItemList)
        OpenAPI_list_free(PatchItemList);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_status_unsubscribe(
        ogs_sbi_subscription_data_t *subscription_data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(subscription_data);
    ogs_assert(subscription_data->id);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.uri = subscription_data->resource_uri;

    message.http.custom.callback =
        (char *)OGS_SBI_CALLBACK_NNRF_NFMANAGEMENT_NF_STATUS_NOTIFY;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_profile_retrieve(char *nf_instance_id)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance_id);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = nf_instance_id;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_disc_build_discover(
        OpenAPI_nf_type_e target_nf_type,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(target_nf_type);
    ogs_assert(requester_nf_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_DISC;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;

    message.param.target_nf_type = target_nf_type;
    message.param.requester_nf_type = requester_nf_type;

    message.param.discovery_option = discovery_option;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}
