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

#include "npcf-build.h"

ogs_sbi_request_t *af_npcf_policyauthorization_build_create(
        af_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;

    OpenAPI_app_session_context_t AppSessionContext;
    OpenAPI_app_session_context_req_data_t AscReqData;

    OpenAPI_events_subsc_req_data_t evSubsc;
    OpenAPI_list_t *EventList = NULL;
    OpenAPI_af_event_subscription_t *Event = NULL;

    OpenAPI_snssai_t sNssai;

    OpenAPI_list_t *MediaComponentList = NULL;
    OpenAPI_map_t *MediaComponentMap = NULL;
    OpenAPI_media_component_t *MediaComponent = NULL;

    OpenAPI_list_t *SubComponentList = NULL;
    OpenAPI_map_t *SubComponentMap = NULL;
    OpenAPI_media_sub_component_t *SubComponent = NULL;

    OpenAPI_list_t *fDescList = NULL;
    OpenAPI_list_t *codecList = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL, *node3 = NULL;

    int i = 0, j = 0;

    af_npcf_policyauthorization_param_t *af_param;

    ogs_assert(sess);
    ogs_assert(sess->af_app_session_id);

    af_param = data;
    ogs_assert(af_param);
    ogs_assert(af_param->med_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_APP_SESSIONS;

    message.AppSessionContext = &AppSessionContext;

    memset(&AppSessionContext, 0, sizeof(AppSessionContext));
    AppSessionContext.asc_req_data = &AscReqData;

    memset(&AscReqData, 0, sizeof(AscReqData));

    server = ogs_list_first(&ogs_sbi_self()->server_list);
    ogs_expect_or_return_val(server, NULL);

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_APP_SESSIONS;
    header.resource.component[1] = (char *)sess->af_app_session_id;
    AscReqData.notif_uri = ogs_sbi_server_uri(server, &header);
    ogs_expect_or_return_val(AscReqData.notif_uri, NULL);

    AscReqData.supp_feat =
        ogs_uint64_to_string(sess->policyauthorization_features);
    ogs_assert(AscReqData.supp_feat);

    AscReqData.ue_ipv4 = sess->ipv4addr;
    AscReqData.ue_ipv6 = sess->ipv6addr;

    AscReqData.dnn = sess->dnn;

    memset(&evSubsc, 0, sizeof(evSubsc));

    EventList = OpenAPI_list_create();
    ogs_assert(EventList);

    Event = ogs_calloc(1, sizeof(*Event));
    ogs_expect_or_return_val(Event, NULL);
    Event->event = OpenAPI_af_event_CHARGING_CORRELATION;
    OpenAPI_list_add(EventList, Event);

    Event = ogs_calloc(1, sizeof(*Event));
    ogs_expect_or_return_val(Event, NULL);
    Event->event = OpenAPI_af_event_ANI_REPORT;
    Event->notif_method = OpenAPI_af_notif_method_ONE_TIME;
    OpenAPI_list_add(EventList, Event);

    evSubsc.events = EventList;
    AscReqData.ev_subsc = &evSubsc;

    memset(&sNssai, 0, sizeof(sNssai));
    if (sess->s_nssai.sst) {
        sNssai.sst = sess->s_nssai.sst;
        sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
        AscReqData.slice_info = &sNssai;
    }

    AscReqData.spon_status = OpenAPI_sponsoring_status_SPONSOR_DISABLED;

    AscReqData.supi = sess->supi;
    AscReqData.gpsi = sess->gpsi;

    AscReqData.af_app_id = (char *)"IMS Services";
    AscReqData.res_prio = OpenAPI_reserv_priority_PRIO_1;

    MediaComponentList = OpenAPI_list_create();
    ogs_assert(MediaComponentList);

    MediaComponent = ogs_calloc(1, sizeof(*MediaComponent));
    ogs_expect_or_return_val(MediaComponent, NULL);

    MediaComponent->med_comp_n = (i++);
    MediaComponent->f_status = OpenAPI_flow_status_ENABLED;
    MediaComponent->med_type = af_param->med_type;
    if (af_param->qos_type == 1) {
        MediaComponent->mar_bw_dl = ogs_sbi_bitrate_to_string(
                                        96000, OGS_SBI_BITRATE_BPS);
        MediaComponent->mar_bw_ul = ogs_sbi_bitrate_to_string(
                                        96000, OGS_SBI_BITRATE_BPS);
        MediaComponent->rr_bw = ogs_sbi_bitrate_to_string(
                                        2400, OGS_SBI_BITRATE_BPS);
        MediaComponent->rs_bw = ogs_sbi_bitrate_to_string(
                                        2400, OGS_SBI_BITRATE_BPS);
    } else if (af_param->qos_type == 2) {
        MediaComponent->mar_bw_dl = ogs_sbi_bitrate_to_string(
                                        96000, OGS_SBI_BITRATE_BPS);
        MediaComponent->mar_bw_ul = ogs_sbi_bitrate_to_string(
                                        96000, OGS_SBI_BITRATE_BPS);
        MediaComponent->rr_bw = ogs_sbi_bitrate_to_string(
                                        88000, OGS_SBI_BITRATE_BPS);
        MediaComponent->rs_bw = ogs_sbi_bitrate_to_string(
                                        88000, OGS_SBI_BITRATE_BPS);
    }

    /* Codec */
    codecList = OpenAPI_list_create();
    ogs_assert(codecList);
    OpenAPI_list_add(codecList,
        ogs_strdup("downlink\noffer\n"
            "m=audio 49000 RTP/AVP 116 99 97 105 100\r\nb=AS:41\r\n"
            "b=RS:512\r\nb=RR:1537\r\na=maxptime:240\r\n"
            "a=des:qos mandatory local sendrecv\r\na=curr:qos local none\r\n"
            "a=des:qos option"));
    OpenAPI_list_add(codecList,
        ogs_strdup("uplink\nanswer\nm=audio 50020 RTP/AVP 99 105\r\n"
            "b=AS:41\r\nb=RS:600\r\nb=RR:2000\r\na=rtpmap:99 AMR-WB/16000/1\r\n"
            "a=fmtp:99 mode-change-capability=2;max-red=0\r\n"
            "a=rtpmap:105 telephone-event/16"));
    ogs_assert(codecList->count);
    MediaComponent->codecs = codecList;

    MediaComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", MediaComponent->med_comp_n), MediaComponent);
    ogs_assert(MediaComponentMap);
    ogs_assert(MediaComponentMap->key);

    OpenAPI_list_add(MediaComponentList, MediaComponentMap);

    /* Sub Component */
    SubComponentList = OpenAPI_list_create();
    ogs_assert(SubComponentList);

    /* Sub Component #1 */
    SubComponent = ogs_calloc(1, sizeof(*SubComponent));
    ogs_expect_or_return_val(SubComponent, NULL);

    SubComponent->f_num = (j++);
    SubComponent->flow_usage = OpenAPI_flow_usage_NO_INFO;

    SubComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", SubComponent->f_num), SubComponent);
    ogs_assert(SubComponentMap);
    ogs_assert(SubComponentMap->key);

    OpenAPI_list_add(SubComponentList, SubComponentMap);

    /* Flow Description */
    fDescList = OpenAPI_list_create();
    ogs_assert(fDescList);
    if (af_param->flow_type == 0) {
        /* Nothing */
    } else if (af_param->flow_type == 1) {
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit out 17 from 172.20.166.84 to 10.45.0.2 20001"));
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit in 17 from 10.45.0.2 to 172.20.166.84 20360"));
    } else if (af_param->flow_type == 2) {
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit out 17 from 172.20.166.84 to 10.45.0.2 30001"));
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit in 17 from 10.45.0.2 to 172.20.166.84 30360"));
    } else if (af_param->flow_type == 99) {
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit out icmp from any to any"));
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit in icmp from any to any"));
    } else {
        ogs_assert_if_reached();
    }
    if (fDescList->count)
        SubComponent->f_descs = fDescList;
    else
        OpenAPI_list_free(fDescList);

    /* Sub Component #2 */
    SubComponent = ogs_calloc(1, sizeof(*SubComponent));
    ogs_expect_or_return_val(SubComponent, NULL);

    SubComponent->f_num = (j++);
    SubComponent->flow_usage = OpenAPI_flow_usage_RTCP;

    SubComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", SubComponent->f_num), SubComponent);
    ogs_assert(SubComponentMap);
    ogs_assert(SubComponentMap->key);

    OpenAPI_list_add(SubComponentList, SubComponentMap);

    /* Flow Description */
    fDescList = OpenAPI_list_create();
    ogs_assert(fDescList);
    if (af_param->flow_type == 0) {
        /* Nothing */
    } else if (af_param->flow_type == 1 || af_param->flow_type == 99) {
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit out 17 from 172.20.166.84 to 10.45.0.2 20002"));
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit in 17 from 10.45.0.2 to 172.20.166.84 20361"));
    } else if (af_param->flow_type == 2) {
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit out 17 from 172.20.166.84 to 10.45.0.2 30002"));
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit in 17 from 10.45.0.2 to 172.20.166.84 30361"));
    } else {
        ogs_assert_if_reached();
    }

    if (fDescList->count)
        SubComponent->f_descs = fDescList;
    else
        OpenAPI_list_free(fDescList);

    ogs_assert(SubComponentList->count);
    MediaComponent->med_sub_comps = SubComponentList;

    ogs_assert(MediaComponentList->count);
    AscReqData.med_components = MediaComponentList;

    request = ogs_sbi_build_request(&message);
    ogs_expect_or_return_val(request, NULL);

    ogs_free(AscReqData.notif_uri);

    ogs_free(AscReqData.supp_feat);

    EventList = evSubsc.events;
    OpenAPI_list_for_each(EventList, node) {
        Event = node->data;
        if (Event)
            ogs_free(Event);
    }
    OpenAPI_list_free(EventList);

    if (sNssai.sd)
        ogs_free(sNssai.sd);

    OpenAPI_list_for_each(MediaComponentList, node) {
        MediaComponentMap = node->data;
        if (MediaComponentMap) {
            MediaComponent = MediaComponentMap->value;
            if (MediaComponent) {

                if (MediaComponent->mar_bw_dl)
                    ogs_free(MediaComponent->mar_bw_dl);
                if (MediaComponent->mar_bw_ul)
                    ogs_free(MediaComponent->mar_bw_ul);
                if (MediaComponent->mir_bw_dl)
                    ogs_free(MediaComponent->mir_bw_dl);
                if (MediaComponent->mir_bw_ul)
                    ogs_free(MediaComponent->mir_bw_ul);
                if (MediaComponent->rr_bw)
                    ogs_free(MediaComponent->rr_bw);
                if (MediaComponent->rs_bw)
                    ogs_free(MediaComponent->rs_bw);

                codecList = MediaComponent->codecs;
                OpenAPI_list_for_each(codecList, node2) {
                    if (node2->data) ogs_free(node2->data);
                }
                OpenAPI_list_free(codecList);

                SubComponentList = MediaComponent->med_sub_comps;
                OpenAPI_list_for_each(SubComponentList, node2) {
                    SubComponentMap = node2->data;
                    if (SubComponentMap) {
                        SubComponent = SubComponentMap->value;
                        if (SubComponent) {

                            fDescList = SubComponent->f_descs;
                            OpenAPI_list_for_each(fDescList, node3) {
                                if (node3->data) ogs_free(node3->data);
                            }
                            OpenAPI_list_free(fDescList);

                            ogs_free(SubComponent);
                        }
                        if (SubComponentMap->key)
                            ogs_free(SubComponentMap->key);
                        ogs_free(SubComponentMap);
                    }
                }
                OpenAPI_list_free(SubComponentList);

                ogs_free(MediaComponent);
            }
            if (MediaComponentMap->key)
                ogs_free(MediaComponentMap->key);
            ogs_free(MediaComponentMap);
        }
    }
    OpenAPI_list_free(MediaComponentList);

    return request;
}

ogs_sbi_request_t *af_npcf_policyauthorization_build_update(
        af_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_app_session_context_update_data_patch_t
        AppSessionContextUpdateDataPatch;
    OpenAPI_app_session_context_update_data_t AscUpdateData;

    OpenAPI_list_t *MediaComponentList = NULL;
    OpenAPI_map_t *MediaComponentMap = NULL;
    OpenAPI_media_component_rm_t *MediaComponent = NULL;

    OpenAPI_list_t *SubComponentList = NULL;
    OpenAPI_map_t *SubComponentMap = NULL;
    OpenAPI_media_sub_component_t *SubComponent = NULL;

    OpenAPI_list_t *fDescList = NULL;
    OpenAPI_list_t *codecList = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL, *node3 = NULL;

    int i = 0, j = 0;

    af_npcf_policyauthorization_param_t *af_param;

    ogs_assert(sess);
    ogs_assert(sess->pcf_app_session_id);

    af_param = data;
    ogs_assert(af_param);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PATCH;
    message.h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_APP_SESSIONS;
    message.h.resource.component[1] = sess->pcf_app_session_id;

    message.AppSessionContextUpdateDataPatch =
        &AppSessionContextUpdateDataPatch;

    memset(&AppSessionContextUpdateDataPatch, 0,
            sizeof(AppSessionContextUpdateDataPatch));
    AppSessionContextUpdateDataPatch.asc_req_data = &AscUpdateData;

    memset(&AscUpdateData, 0, sizeof(AscUpdateData));

    AscUpdateData.spon_status = OpenAPI_sponsoring_status_SPONSOR_DISABLED;

    AscUpdateData.af_app_id = (char *)"IMS Services";
    AscUpdateData.res_prio = OpenAPI_reserv_priority_PRIO_1;

    MediaComponentList = OpenAPI_list_create();
    ogs_assert(MediaComponentList);

    MediaComponent = ogs_calloc(1, sizeof(*MediaComponent));
    ogs_expect_or_return_val(MediaComponent, NULL);

    MediaComponent->med_comp_n = (i++);
    MediaComponent->f_status = OpenAPI_flow_status_ENABLED;
    MediaComponent->med_type = OpenAPI_media_type_AUDIO;
    if (af_param->qos_type == 1) {
        MediaComponent->mar_bw_dl = ogs_sbi_bitrate_to_string(
                                        96000, OGS_SBI_BITRATE_BPS);
        MediaComponent->mar_bw_ul = ogs_sbi_bitrate_to_string(
                                        96000, OGS_SBI_BITRATE_BPS);
        MediaComponent->rr_bw = ogs_sbi_bitrate_to_string(
                                        2400, OGS_SBI_BITRATE_BPS);
        MediaComponent->rs_bw = ogs_sbi_bitrate_to_string(
                                        2400, OGS_SBI_BITRATE_BPS);
    } else if (af_param->qos_type == 2) {
        MediaComponent->mar_bw_dl = ogs_sbi_bitrate_to_string(
                                        96000, OGS_SBI_BITRATE_BPS);
        MediaComponent->mar_bw_ul = ogs_sbi_bitrate_to_string(
                                        96000, OGS_SBI_BITRATE_BPS);
        MediaComponent->mir_bw_dl = ogs_sbi_bitrate_to_string(
                                        88000, OGS_SBI_BITRATE_BPS);
        MediaComponent->mir_bw_ul = ogs_sbi_bitrate_to_string(
                                        88000, OGS_SBI_BITRATE_BPS);
    }

    /* Codec */
    codecList = OpenAPI_list_create();
    ogs_assert(codecList);
    OpenAPI_list_add(codecList,
        ogs_strdup("downlink\noffer\n"
            "m=audio 49000 RTP/AVP 116 99 97 105 100\r\nb=AS:41\r\n"
            "b=RS:512\r\nb=RR:1537\r\na=maxptime:240\r\n"
            "a=des:qos mandatory local sendrecv\r\na=curr:qos local none\r\n"
            "a=des:qos option"));
    OpenAPI_list_add(codecList,
        ogs_strdup("uplink\nanswer\nm=audio 50020 RTP/AVP 99 105\r\n"
            "b=AS:41\r\nb=RS:600\r\nb=RR:2000\r\na=rtpmap:99 AMR-WB/16000/1\r\n"
            "a=fmtp:99 mode-change-capability=2;max-red=0\r\n"
            "a=rtpmap:105 telephone-event/16"));
    ogs_assert(codecList->count);
    MediaComponent->codecs = codecList;

    MediaComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", MediaComponent->med_comp_n), MediaComponent);
    ogs_assert(MediaComponentMap);
    ogs_assert(MediaComponentMap->key);

    OpenAPI_list_add(MediaComponentList, MediaComponentMap);

    /* Sub Component */
    SubComponentList = OpenAPI_list_create();
    ogs_assert(SubComponentList);

    /* Sub Component #1 */
    SubComponent = ogs_calloc(1, sizeof(*SubComponent));
    ogs_expect_or_return_val(SubComponent, NULL);

    SubComponent->f_num = (j++);
    SubComponent->flow_usage = OpenAPI_flow_usage_NO_INFO;

    SubComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", SubComponent->f_num), SubComponent);
    ogs_assert(SubComponentMap);
    ogs_assert(SubComponentMap->key);

    OpenAPI_list_add(SubComponentList, SubComponentMap);

    /* Flow Description */
    fDescList = OpenAPI_list_create();
    ogs_assert(fDescList);
    if (af_param->flow_type == 0) {
        /* Nothing */
    } else if (af_param->flow_type == 1) {
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit out 17 from 172.20.166.84 to 10.45.0.2 20001"));
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit in 17 from 10.45.0.2 to 172.20.166.84 20360"));
    } else if (af_param->flow_type == 99) {
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit out icmp from any to any"));
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit in icmp from any to any"));
    } else {
        ogs_assert_if_reached();
    }
    if (fDescList->count)
        SubComponent->f_descs = fDescList;
    else
        OpenAPI_list_free(fDescList);

    /* Sub Component #2 */
    SubComponent = ogs_calloc(1, sizeof(*SubComponent));
    ogs_expect_or_return_val(SubComponent, NULL);

    SubComponent->f_num = (j++);
    SubComponent->flow_usage = OpenAPI_flow_usage_RTCP;

    SubComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", SubComponent->f_num), SubComponent);
    ogs_assert(SubComponentMap);
    ogs_assert(SubComponentMap->key);

    OpenAPI_list_add(SubComponentList, SubComponentMap);

    /* Flow Description */
    fDescList = OpenAPI_list_create();
    ogs_assert(fDescList);
    if (af_param->flow_type) {
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit out 17 from 172.20.166.84 to 10.45.0.2 20002"));
        OpenAPI_list_add(fDescList,
            ogs_strdup("permit in 17 from 10.45.0.2 to 172.20.166.84 20361"));
    }
    if (fDescList->count)
        SubComponent->f_descs = fDescList;
    else
        OpenAPI_list_free(fDescList);

    ogs_assert(SubComponentList->count);
    MediaComponent->med_sub_comps = SubComponentList;

    ogs_assert(MediaComponentList->count);
    AscUpdateData.med_components = MediaComponentList;

    request = ogs_sbi_build_request(&message);
    ogs_expect_or_return_val(request, NULL);

    OpenAPI_list_for_each(MediaComponentList, node) {
        MediaComponentMap = node->data;
        if (MediaComponentMap) {
            MediaComponent = MediaComponentMap->value;
            if (MediaComponent) {

                if (MediaComponent->mar_bw_dl)
                    ogs_free(MediaComponent->mar_bw_dl);
                if (MediaComponent->mar_bw_ul)
                    ogs_free(MediaComponent->mar_bw_ul);
                if (MediaComponent->mir_bw_dl)
                    ogs_free(MediaComponent->mir_bw_dl);
                if (MediaComponent->mir_bw_ul)
                    ogs_free(MediaComponent->mir_bw_ul);
                if (MediaComponent->rr_bw)
                    ogs_free(MediaComponent->rr_bw);
                if (MediaComponent->rs_bw)
                    ogs_free(MediaComponent->rs_bw);

                codecList = MediaComponent->codecs;
                OpenAPI_list_for_each(codecList, node2) {
                    if (node2->data) ogs_free(node2->data);
                }
                OpenAPI_list_free(codecList);

                SubComponentList = MediaComponent->med_sub_comps;
                OpenAPI_list_for_each(SubComponentList, node2) {
                    SubComponentMap = node2->data;
                    if (SubComponentMap) {
                        SubComponent = SubComponentMap->value;
                        if (SubComponent) {

                            fDescList = SubComponent->f_descs;
                            OpenAPI_list_for_each(fDescList, node3) {
                                if (node3->data) ogs_free(node3->data);
                            }
                            OpenAPI_list_free(fDescList);

                            ogs_free(SubComponent);
                        }
                        if (SubComponentMap->key)
                            ogs_free(SubComponentMap->key);
                        ogs_free(SubComponentMap);
                    }
                }
                OpenAPI_list_free(SubComponentList);

                ogs_free(MediaComponent);
            }
            if (MediaComponentMap->key)
                ogs_free(MediaComponentMap->key);
            ogs_free(MediaComponentMap);
        }
    }
    OpenAPI_list_free(MediaComponentList);

    return request;
}

ogs_sbi_request_t *af_npcf_policyauthorization_build_delete(
        af_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    ogs_assert(sess->pcf_app_session_id);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_APP_SESSIONS;
    message.h.resource.component[1] = sess->pcf_app_session_id;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_DELETE;

    request = ogs_sbi_build_request(&message);
    ogs_expect_or_return_val(request, NULL);

    return request;
}

ogs_sbi_request_t *af_npcf_policyauthorization_build_create_video(
        af_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;

    OpenAPI_app_session_context_t AppSessionContext;
    OpenAPI_app_session_context_req_data_t AscReqData;

    OpenAPI_events_subsc_req_data_t evSubsc;
    OpenAPI_list_t *EventList = NULL;
    OpenAPI_af_event_subscription_t *Event = NULL;

    OpenAPI_snssai_t sNssai;

    OpenAPI_list_t *MediaComponentList = NULL;
    OpenAPI_map_t *MediaComponentMap = NULL;
    OpenAPI_media_component_t *MediaComponent = NULL;

    OpenAPI_list_t *SubComponentList = NULL;
    OpenAPI_map_t *SubComponentMap = NULL;
    OpenAPI_media_sub_component_t *SubComponent = NULL;

    OpenAPI_list_t *fDescList = NULL;
    OpenAPI_list_t *codecList = NULL;

    OpenAPI_lnode_t *node = NULL, *node2 = NULL, *node3 = NULL;

    int i = 0, j = 0;

    af_npcf_policyauthorization_param_t *af_param;

    ogs_assert(sess);
    ogs_assert(sess->af_app_session_id);

    af_param = data;
    ogs_assert(af_param);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_APP_SESSIONS;

    message.AppSessionContext = &AppSessionContext;

    memset(&AppSessionContext, 0, sizeof(AppSessionContext));
    AppSessionContext.asc_req_data = &AscReqData;

    memset(&AscReqData, 0, sizeof(AscReqData));

    server = ogs_list_first(&ogs_sbi_self()->server_list);
    ogs_expect_or_return_val(server, NULL);

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_APP_SESSIONS;
    header.resource.component[1] = (char *)sess->af_app_session_id;
    AscReqData.notif_uri = ogs_sbi_server_uri(server, &header);
    ogs_expect_or_return_val(AscReqData.notif_uri, NULL);

    AscReqData.supp_feat =
        ogs_uint64_to_string(sess->policyauthorization_features);
    ogs_assert(AscReqData.supp_feat);

    AscReqData.ue_ipv4 = sess->ipv4addr;
    AscReqData.ue_ipv6 = sess->ipv6addr;

    AscReqData.dnn = sess->dnn;

    memset(&evSubsc, 0, sizeof(evSubsc));

    EventList = OpenAPI_list_create();
    ogs_assert(EventList);

    Event = ogs_calloc(1, sizeof(*Event));
    ogs_expect_or_return_val(Event, NULL);
    Event->event = OpenAPI_af_event_CHARGING_CORRELATION;
    OpenAPI_list_add(EventList, Event);

    Event = ogs_calloc(1, sizeof(*Event));
    ogs_expect_or_return_val(Event, NULL);
    Event->event = OpenAPI_af_event_ANI_REPORT;
    Event->notif_method = OpenAPI_af_notif_method_ONE_TIME;
    OpenAPI_list_add(EventList, Event);

    evSubsc.events = EventList;
    AscReqData.ev_subsc = &evSubsc;

    memset(&sNssai, 0, sizeof(sNssai));
    if (sess->s_nssai.sst) {
        sNssai.sst = sess->s_nssai.sst;
        sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
        AscReqData.slice_info = &sNssai;
    }

    AscReqData.spon_status = OpenAPI_sponsoring_status_SPONSOR_DISABLED;

    AscReqData.supi = sess->supi;
    AscReqData.gpsi = sess->gpsi;

    AscReqData.af_app_id = (char *)"IMS Services";
    AscReqData.res_prio = OpenAPI_reserv_priority_PRIO_1;

    MediaComponentList = OpenAPI_list_create();
    ogs_assert(MediaComponentList);

    MediaComponent = ogs_calloc(1, sizeof(*MediaComponent));
    ogs_expect_or_return_val(MediaComponent, NULL);

    MediaComponent->med_comp_n = (i++);
    MediaComponent->f_status = OpenAPI_flow_status_ENABLED;
    MediaComponent->med_type = OpenAPI_media_type_AUDIO;
    MediaComponent->mar_bw_dl = ogs_sbi_bitrate_to_string(
                                    41000, OGS_SBI_BITRATE_BPS);
    MediaComponent->mar_bw_ul = ogs_sbi_bitrate_to_string(
                                    41000, OGS_SBI_BITRATE_BPS);
    MediaComponent->rr_bw = ogs_sbi_bitrate_to_string(
                                    2000, OGS_SBI_BITRATE_BPS);
    MediaComponent->rs_bw = ogs_sbi_bitrate_to_string(
                                    600, OGS_SBI_BITRATE_BPS);

    /* Codec */
    codecList = OpenAPI_list_create();
    ogs_assert(codecList);
    OpenAPI_list_add(codecList,
        ogs_strdup("downlink\noffer\nm=audio 50026 RTP/AVP 104 102 96 97\r\n" \
            "b=AS:41\r\n" \
            "b=RS:600\r\n" \
            "b=RR:2000\r\n" \
            "a=rtpmap:104 AMR-WB/16000/1\r\n" \
            "a=fmtp:104 mode-change-capability=2;max-red=0\r\n" \
            "a=rtpmap:102 AMR/8000/1\r\n" \
            "a=fmtp:102 mode-change-capability=2;max-red=0\r\n" \
            "a=rtpmap:96 telephone-event/16000\r\n" \
            "a=fmtp:96 0-15\r\n" \
            "a=rtpmap:97 telephone-event/8000\r\n" \
            "a=fmtp:97 0-15\r\n" \
            "a=curr:qos local none\r\n" \
            "a=curr:qos remote none\r\n" \
            "a=des:qos mandatory local sendrecv\r\n" \
            "a=des:qos optional remote sendrecv\r\n" \
            "a=sendrecv\r\n" \
            "a=maxptime:240\r\n" \
            "a=ptime:20\r\n"));
    OpenAPI_list_add(codecList,
        ogs_strdup("uplink\nanswer\nm=audio 50022 RTP/AVP 104 96\r\n" \
            "b=AS:41\r\n" \
            "b=RS:600\r\n" \
            "b=RR:2000\r\n" \
            "a=rtpmap:104 AMR-WB/16000/1\r\n" \
            "a=fmtp:104 mode-change-capability=2;max-red=0\r\n" \
            "a=rtpmap:96 telephone-event/16000\r\n" \
            "a=fmtp:96 0-15\r\n" \
            "a=curr:qos local none\r\n" \
            "a=curr:qos remote none\r\n" \
            "a=des:qos mandatory local sendrecv\r\n" \
            "a=des:qos mandatory remote sendrecv\r\n" \
            "a=conf:qos remote sendrecv\r\n" \
            "a=sendrecv\r\n" \
            "a=maxptime:240\r\n" \
            "a=ptime:20\r\n"));
    ogs_assert(codecList->count);
    MediaComponent->codecs = codecList;

    MediaComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", MediaComponent->med_comp_n), MediaComponent);
    ogs_assert(MediaComponentMap);
    ogs_assert(MediaComponentMap->key);

    OpenAPI_list_add(MediaComponentList, MediaComponentMap);

    /* Sub Component */
    SubComponentList = OpenAPI_list_create();
    ogs_assert(SubComponentList);

    j = 0;

    /* Sub Component #1 */
    SubComponent = ogs_calloc(1, sizeof(*SubComponent));
    ogs_expect_or_return_val(SubComponent, NULL);

    SubComponent->f_num = (j++);
    SubComponent->flow_usage = OpenAPI_flow_usage_NO_INFO;

    SubComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", SubComponent->f_num), SubComponent);
    ogs_assert(SubComponentMap);
    ogs_assert(SubComponentMap->key);

    OpenAPI_list_add(SubComponentList, SubComponentMap);

    /* Flow Description */
    fDescList = OpenAPI_list_create();
    ogs_assert(fDescList);
    OpenAPI_list_add(fDescList,
        ogs_strdup("permit out 17 from 10.45.0.5 50026 to 10.45.0.3 50022"));
    OpenAPI_list_add(fDescList,
        ogs_strdup("permit in 17 from 10.45.0.3 50022 to 10.45.0.5 50026"));
    if (fDescList->count)
        SubComponent->f_descs = fDescList;
    else
        OpenAPI_list_free(fDescList);

    /* Sub Component #2 */
    SubComponent = ogs_calloc(1, sizeof(*SubComponent));
    ogs_expect_or_return_val(SubComponent, NULL);

    SubComponent->f_num = (j++);
    SubComponent->flow_usage = OpenAPI_flow_usage_NO_INFO;

    SubComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", SubComponent->f_num), SubComponent);
    ogs_assert(SubComponentMap);
    ogs_assert(SubComponentMap->key);

    OpenAPI_list_add(SubComponentList, SubComponentMap);

    /* Flow Description */
    fDescList = OpenAPI_list_create();
    ogs_assert(fDescList);
    OpenAPI_list_add(fDescList,
        ogs_strdup("permit out 17 from 10.45.0.5 50027 to 10.45.0.3 50023"));
    OpenAPI_list_add(fDescList,
        ogs_strdup("permit in 17 from 10.45.0.3 50023 to 10.45.0.5 50027"));
    if (fDescList->count)
        SubComponent->f_descs = fDescList;
    else
        OpenAPI_list_free(fDescList);

    ogs_assert(SubComponentList->count);
    MediaComponent->med_sub_comps = SubComponentList;

    MediaComponent = ogs_calloc(1, sizeof(*MediaComponent));
    ogs_expect_or_return_val(MediaComponent, NULL);

    MediaComponent->med_comp_n = (i++);
    MediaComponent->f_status = OpenAPI_flow_status_ENABLED;
    MediaComponent->med_type = OpenAPI_media_type_VIDEO;
    MediaComponent->mar_bw_dl = ogs_sbi_bitrate_to_string(
                                    401000, OGS_SBI_BITRATE_BPS);
    MediaComponent->mar_bw_ul = ogs_sbi_bitrate_to_string(
                                    401000, OGS_SBI_BITRATE_BPS);
    MediaComponent->rr_bw = ogs_sbi_bitrate_to_string(
                                    2000, OGS_SBI_BITRATE_BPS);
    MediaComponent->rs_bw = ogs_sbi_bitrate_to_string(
                                    600, OGS_SBI_BITRATE_BPS);

    /* Codec */
    codecList = OpenAPI_list_create();
    ogs_assert(codecList);
    OpenAPI_list_add(codecList,
        ogs_strdup("downlink\noffer\nm=video 60010 RTP/AVP 114 113\r\n" \
            "b=AS:401\r\n" \
            "b=RS:600\r\n" \
            "b=RR:2000\r\n" \
            "a=rtpmap:114 H264/90000\r\n" \
            "a=fmtp:114 profile-level-id=42C00C;packetization-mode=1;sar-understood=16;sar-supported=1;sprop-parameter-sets=Z0LADNoPCmgG0KE1,aM4G4g==\r\n" \
            "a=rtpmap:113 H264/90000\r\n" \
            "a=fmtp:113 profile-level-id=42C00C;packetization-mode=0;sar-understood=16;sar-supported=1;sprop-parameter-sets=Z0LADNoPCmgG0KE1,aM4G4g==\r\n" \
            "a=curr:qos local none\r\n" \
            "a=curr:qos remote none\r\n" \
            "a=des:qos mandatory local sendrecv\r\n" \
            "a=des:qos optional remote sendrecv\r\n" \
            "a=rtcp-fb:* nack\r\n" \
            "a=rtcp-fb:* nack pli\r\n" \
            "a=rtcp-fb:* ccm fir\r\n" \
            "a=rtcp-fb:* ccm tmmbr\r\n" \
            "a=sendrecv\r\n" \
            "a=tcap:1 RTP/AVPF\r\n" \
            "a=pcfg:1 t=1\r\n" \
            "a=extmap:2 urn:3gpp:video-orientation\r\n"));
    OpenAPI_list_add(codecList,
        ogs_strdup("uplink\nanswer\nm=video 60010 RTP/AVPF 114\r\n" \
            "b=AS:401\r\n" \
            "b=RS:600\r\n" \
            "b=RR:2000\r\n" \
            "a=rtpmap:114 H264/90000\r\n" \
            "a=fmtp:114 profile-level-id=42C00C;packetization-mode=1;sar-understood=16;sar-supported=1;sprop-parameter-sets=Z0LADNoPCmgG0KE1,aM4G4g==\r\n" \
            "a=curr:qos local none\r\n" \
            "a=curr:qos remote none\r\n" \
            "a=des:qos mandatory local sendrecv\r\n" \
            "a=des:qos mandatory remote sendrecv\r\n" \
            "a=conf:qos remote sendrecv\r\n" \
            "a=rtcp-fb:* nack\r\n" \
            "a=rtcp-fb:* nack pli\r\n" \
            "a=rtcp-fb:* ccm fir\r\n" \
            "a=rtcp-fb:* ccm tmmbr\r\n" \
            "a=sendrecv\r\n" \
            "a=extmap:2 urn:3gpp:video-orientation\r\n" \
            "a=acfg:1 t=1\r\n"));
    ogs_assert(codecList->count);
    MediaComponent->codecs = codecList;

    MediaComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", MediaComponent->med_comp_n), MediaComponent);
    ogs_assert(MediaComponentMap);
    ogs_assert(MediaComponentMap->key);

    OpenAPI_list_add(MediaComponentList, MediaComponentMap);

    /* Sub Component */
    SubComponentList = OpenAPI_list_create();
    ogs_assert(SubComponentList);

    j = 0;

    /* Sub Component #1 */
    SubComponent = ogs_calloc(1, sizeof(*SubComponent));
    ogs_expect_or_return_val(SubComponent, NULL);

    SubComponent->f_num = (j++);
    SubComponent->flow_usage = OpenAPI_flow_usage_NO_INFO;

    SubComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", SubComponent->f_num), SubComponent);
    ogs_assert(SubComponentMap);
    ogs_assert(SubComponentMap->key);

    OpenAPI_list_add(SubComponentList, SubComponentMap);

    /* Flow Description */
    fDescList = OpenAPI_list_create();
    ogs_assert(fDescList);
    OpenAPI_list_add(fDescList,
        ogs_strdup("permit out 17 from 10.45.0.5 60010 to 10.45.0.3 60010"));
    OpenAPI_list_add(fDescList,
        ogs_strdup("permit in 17 from 10.45.0.3 60010 to 10.45.0.5 60010"));
    if (fDescList->count)
        SubComponent->f_descs = fDescList;
    else
        OpenAPI_list_free(fDescList);

    /* Sub Component #2 */
    SubComponent = ogs_calloc(1, sizeof(*SubComponent));
    ogs_expect_or_return_val(SubComponent, NULL);

    SubComponent->f_num = (j++);
    SubComponent->flow_usage = OpenAPI_flow_usage_NO_INFO;

    SubComponentMap = OpenAPI_map_create(
            ogs_msprintf("%d", SubComponent->f_num), SubComponent);
    ogs_assert(SubComponentMap);
    ogs_assert(SubComponentMap->key);

    OpenAPI_list_add(SubComponentList, SubComponentMap);

    /* Flow Description */
    fDescList = OpenAPI_list_create();
    ogs_assert(fDescList);
    OpenAPI_list_add(fDescList,
        ogs_strdup("permit out 17 from 10.45.0.5 60011 to 10.45.0.3 60011"));
    OpenAPI_list_add(fDescList,
        ogs_strdup("permit in 17 from 10.45.0.3 60011 to 10.45.0.5 60011"));
    if (fDescList->count)
        SubComponent->f_descs = fDescList;
    else
        OpenAPI_list_free(fDescList);

    ogs_assert(SubComponentList->count);
    MediaComponent->med_sub_comps = SubComponentList;

    ogs_assert(MediaComponentList->count);
    AscReqData.med_components = MediaComponentList;

    request = ogs_sbi_build_request(&message);
    ogs_expect_or_return_val(request, NULL);

    ogs_free(AscReqData.notif_uri);

    ogs_free(AscReqData.supp_feat);

    EventList = evSubsc.events;
    OpenAPI_list_for_each(EventList, node) {
        Event = node->data;
        if (Event)
            ogs_free(Event);
    }
    OpenAPI_list_free(EventList);

    if (sNssai.sd)
        ogs_free(sNssai.sd);

    OpenAPI_list_for_each(MediaComponentList, node) {
        MediaComponentMap = node->data;
        if (MediaComponentMap) {
            MediaComponent = MediaComponentMap->value;
            if (MediaComponent) {

                if (MediaComponent->mar_bw_dl)
                    ogs_free(MediaComponent->mar_bw_dl);
                if (MediaComponent->mar_bw_ul)
                    ogs_free(MediaComponent->mar_bw_ul);
                if (MediaComponent->mir_bw_dl)
                    ogs_free(MediaComponent->mir_bw_dl);
                if (MediaComponent->mir_bw_ul)
                    ogs_free(MediaComponent->mir_bw_ul);
                if (MediaComponent->rr_bw)
                    ogs_free(MediaComponent->rr_bw);
                if (MediaComponent->rs_bw)
                    ogs_free(MediaComponent->rs_bw);

                codecList = MediaComponent->codecs;
                OpenAPI_list_for_each(codecList, node2) {
                    if (node2->data) ogs_free(node2->data);
                }
                OpenAPI_list_free(codecList);

                SubComponentList = MediaComponent->med_sub_comps;
                OpenAPI_list_for_each(SubComponentList, node2) {
                    SubComponentMap = node2->data;
                    if (SubComponentMap) {
                        SubComponent = SubComponentMap->value;
                        if (SubComponent) {

                            fDescList = SubComponent->f_descs;
                            OpenAPI_list_for_each(fDescList, node3) {
                                if (node3->data) ogs_free(node3->data);
                            }
                            OpenAPI_list_free(fDescList);

                            ogs_free(SubComponent);
                        }
                        if (SubComponentMap->key)
                            ogs_free(SubComponentMap->key);
                        ogs_free(SubComponentMap);
                    }
                }
                OpenAPI_list_free(SubComponentList);

                ogs_free(MediaComponent);
            }
            if (MediaComponentMap->key)
                ogs_free(MediaComponentMap->key);
            ogs_free(MediaComponentMap);
        }
    }
    OpenAPI_list_free(MediaComponentList);

    return request;
}
