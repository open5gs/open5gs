/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

void ogs_sbi_nnrf_handle_nf_register(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_nf_profile_t *NFProfile = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(recvmsg);
    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    NFProfile = recvmsg->NFProfile;
    if (!NFProfile) {
        ogs_error("No NFProfile");
        return;
    }

    /* TIME : Update heartbeat from NRF */
    if (NFProfile->is_heart_beat_timer == true)
        nf_instance->time.heartbeat_interval = NFProfile->heart_beat_timer;
}

void ogs_sbi_nnrf_handle_nf_profile(
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

    OpenAPI_list_for_each(NFProfile->ipv4_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) {
            ogs_error("No IPv4 Address");
            continue;
        }

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

        if (!node->data) {
            ogs_error("No IPv6 Address");
            continue;
        }

        if (nf_instance->num_of_ipv6 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                    node->data, ogs_sbi_self()->sbi_port, 0);
            if (rv != OGS_OK) continue;

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
            if (!IpEndPoint->is_port) {
                if (nf_service->scheme == OpenAPI_uri_scheme_http)
                    port = OGS_SBI_HTTP_PORT;
                else if (nf_service->scheme == OpenAPI_uri_scheme_https)
                    port = OGS_SBI_HTTPS_PORT;
                else {
                    ogs_error("Invalid scheme [%d]", nf_service->scheme);
                    continue;
                }
            } else {
                port = IpEndPoint->port;
            }

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

void ogs_nnrf_handle_nf_status_subscribe(
        ogs_sbi_subscription_data_t *subscription_data,
        ogs_sbi_message_t *recvmsg)
{
    OpenAPI_subscription_data_t *SubscriptionData = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(recvmsg);
    ogs_assert(subscription_data);
    client = subscription_data->client;
    ogs_assert(client);

    SubscriptionData = recvmsg->SubscriptionData;
    if (!SubscriptionData) {
        ogs_error("No SubscriptionData");
        return;
    }

    if (!SubscriptionData->subscription_id) {
        ogs_error("No SubscriptionId");
        return;
    }
    ogs_sbi_subscription_data_set_id(
        subscription_data, SubscriptionData->subscription_id);

    /* SBI Features */
    if (SubscriptionData->nrf_supported_features) {
        subscription_data->nrf_supported_features =
            ogs_uint64_from_string(SubscriptionData->nrf_supported_features);
    } else {
        subscription_data->nrf_supported_features = 0;
    }

    if (SubscriptionData->validity_time) {
#define VALIDITY_MINIMUM (10LL * OGS_USEC_PER_SEC) /* 10 seconds */
        ogs_time_t time, duration;
        if (ogs_sbi_time_from_string(
                &time, SubscriptionData->validity_time) == true) {
            duration = time - ogs_time_now();
            if (duration < VALIDITY_MINIMUM) {
                duration = VALIDITY_MINIMUM;
                ogs_warn("[%s] Forced to %lld seconds", subscription_data->id,
                        (long long)ogs_time_sec(VALIDITY_MINIMUM));
            }
            subscription_data->t_validity = ogs_timer_add(ogs_app()->timer_mgr,
                ogs_timer_subscription_validity, subscription_data);
            ogs_assert(subscription_data->t_validity);
            ogs_timer_start(subscription_data->t_validity, duration);
        } else {
            ogs_error("Cannot parse validitiyTime [%s]",
                    SubscriptionData->validity_time);
        }
    }
}

bool ogs_nnrf_handle_nf_status_notify(
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
                recvmsg, "No NotificationData", NULL));
        return false;
    }

    if (!NotificationData->nf_instance_uri) {
        ogs_error("No nfInstanceUri");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No nfInstanceUri", NULL));
        return false;
    }

    memset(&header, 0, sizeof(header));
    header.uri = NotificationData->nf_instance_uri;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("Cannot parse nfInstanceUri [%s]", header.uri);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Cannot parse nfInstanceUri", header.uri));
        return false;
    }

    if (!message.h.resource.component[1]) {
        ogs_error("No nfInstanceId [%s]", header.uri);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Cannot parse nfInstanceUri", header.uri));
        ogs_sbi_header_free(&header);
        return false;
    }

    if (NF_INSTANCE_ID_IS_SELF(message.h.resource.component[1])) {
        ogs_warn("[%s] The notification is not allowed",
                message.h.resource.component[1]);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "The notification is not allowed",
                message.h.resource.component[1]));
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
                    recvmsg, "No NFProfile", NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_instance_id) {
            ogs_error("No NFProfile.NFInstanceId");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFInstanceId", NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_type) {
            ogs_error("No NFProfile.NFType");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFType", NULL));
            ogs_sbi_header_free(&header);
            return false;
        }

        if (!NFProfile->nf_status) {
            ogs_error("No NFProfile.NFStatus");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No NFProfile.NFStatus", NULL));
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
            ogs_sbi_nf_fsm_tran(nf_instance, ogs_sbi_nf_state_registered);

            ogs_warn("[%s] (NRF-notify) NF has already been added",
                    message.h.resource.component[1]);
        }

        ogs_sbi_nnrf_handle_nf_profile(nf_instance, NFProfile);

        ogs_info("[%s] (NRF-notify) NF Profile updated", nf_instance->id);

        ogs_sbi_client_associate(nf_instance);

    } else if (NotificationData->event ==
            OpenAPI_notification_event_type_NF_DEREGISTERED) {
        nf_instance = ogs_sbi_nf_instance_find(message.h.resource.component[1]);
        if (nf_instance) {
            if (OGS_OBJECT_IS_REF(nf_instance)) {
                /* There are references to other contexts. */
                ogs_warn("[%s:%d] NF was referenced in other contexts",
                        nf_instance->id, nf_instance->reference_count);
                ogs_sbi_nf_fsm_tran(
                        nf_instance, ogs_sbi_nf_state_de_registered);
            } else {
                ogs_info("[%s] NF removed", nf_instance->id);
                ogs_sbi_nf_fsm_fini((nf_instance));
                ogs_sbi_nf_instance_remove(nf_instance);
            }
        } else {
            ogs_warn("[%s] (NRF-notify) Not found",
                    message.h.resource.component[1]);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_NOT_FOUND,
                    recvmsg, "Not found", message.h.resource.component[1]));
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
                eventstr ? eventstr : "Unknown"));
        ogs_sbi_header_free(&header);
        return false;
    }

    response = ogs_sbi_build_response(recvmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_sbi_header_free(&header);
    return true;
}

void ogs_nnrf_handle_nf_discover_search_result(
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

            ogs_info("[%s] (NF-discover) NF registered", nf_instance->id);
        } else {
            ogs_sbi_nf_fsm_tran(nf_instance, ogs_sbi_nf_state_registered);

            ogs_warn("[%s] (NF-discover) NF has already been added",
                    NFProfile->nf_instance_id);
        }

        if (NF_INSTANCE_ID_IS_OTHERS(nf_instance->id)) {
            ogs_sbi_nnrf_handle_nf_profile(nf_instance, NFProfile);

            ogs_sbi_client_associate(nf_instance);

            /* TIME : Update validity from NRF */
            if (SearchResult->is_validity_period &&
                SearchResult->validity_period) {
                nf_instance->time.validity_duration =
                        SearchResult->validity_period;

                ogs_assert(nf_instance->t_validity);
                ogs_timer_start(nf_instance->t_validity,
                    ogs_time_from_sec(nf_instance->time.validity_duration));

            } else
                ogs_warn("[%s] NF Instance validity-time should not 0",
                        nf_instance->id);

            ogs_info("[%s] (NF-discover) NF Profile updated", nf_instance->id);
        }
    }
}
