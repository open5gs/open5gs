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

#include "sbi-path.h"
#include "nas-path.h"
#include "ngap-path.h"
#include "nnrf-handler.h"

void amf_nnrf_handle_nf_register(
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

void amf_nnrf_handle_nf_status_subscribe(
        ogs_sbi_subscription_t *subscription, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_subscription_data_t *SubscriptionData = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(recvmsg);
    ogs_assert(subscription);
    client = subscription->client;
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
    ogs_sbi_subscription_set_id(
        subscription, SubscriptionData->subscription_id);

    if (SubscriptionData->validity_time) {
#define VALIDITY_MINIMUM (10LL * OGS_USEC_PER_SEC) /* 10 seconds */
        ogs_time_t time, duration;
        if (ogs_sbi_time_from_string(
                &time, SubscriptionData->validity_time) == true) {
            duration = time - ogs_time_now();
            if (duration < VALIDITY_MINIMUM) {
                duration = VALIDITY_MINIMUM;
                ogs_warn("[%s] Forced to %lld seconds", subscription->id,
                        (long long)ogs_time_sec(VALIDITY_MINIMUM));
            }
            subscription->t_validity = ogs_timer_add(ogs_app()->timer_mgr,
                amf_timer_subscription_validity, subscription);
            ogs_assert(subscription->t_validity);
            ogs_timer_start(subscription->t_validity, duration);
        } else {
            ogs_error("Cannot parse validitiyTime [%s]",
                    SubscriptionData->validity_time);
        }
    }
}

bool amf_nnrf_handle_nf_status_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;
    bool handled;

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

    if (NF_INSTANCE_IS_SELF(message.h.resource.component[1])) {
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

        nf_instance = ogs_sbi_nf_instance_find(message.h.resource.component[1]);
        if (!nf_instance) {
            nf_instance = ogs_sbi_nf_instance_add();
            ogs_assert(nf_instance);
            ogs_sbi_nf_instance_set_id(nf_instance,
                    message.h.resource.component[1]);

            amf_nf_fsm_init(nf_instance);

            ogs_info("[%s] (NRF-notify) NF registered", nf_instance->id);

        } else {
            nf_instance->reference_count++;

            OGS_FSM_TRAN(&nf_instance->sm, amf_nf_state_registered);
            ogs_fsm_dispatch(&nf_instance->sm, NULL);

            ogs_warn("[%s] (NRF-notify) NF has already been added",
                    message.h.resource.component[1]);

        }

        handled = ogs_sbi_nnrf_handle_nf_profile(
                    nf_instance, NFProfile, stream, recvmsg);
        if (!handled) {
            AMF_NF_INSTANCE_CLEAR("NRF-notify", nf_instance);
            ogs_sbi_header_free(&header);
            return false;
        }

        ogs_info("[%s] (NRF-notify) NF Profile updated", nf_instance->id);

        handled = ogs_sbi_client_associate(nf_instance);
        if (!handled) {
            ogs_error("[%s] Cannot associate NF EndPoint", nf_instance->id);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "Cannot find NF EndPoint", nf_instance->id));
            AMF_NF_INSTANCE_CLEAR("NRF-notify", nf_instance);
            ogs_sbi_header_free(&header);
            return false;
        }

    } else if (NotificationData->event ==
            OpenAPI_notification_event_type_NF_DEREGISTERED) {
        nf_instance = ogs_sbi_nf_instance_find(message.h.resource.component[1]);
        if (nf_instance) {
            AMF_NF_INSTANCE_CLEAR("NRF-notify", nf_instance);
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

void amf_nnrf_handle_nf_discover(
        ogs_sbi_xact_t *xact, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_object_t *sbi_object = NULL;
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    OpenAPI_search_result_t *SearchResult = NULL;

    ogs_assert(xact);
    sbi_object = xact->sbi_object;
    ogs_assert(sbi_object);
    ogs_assert(recvmsg);

    SearchResult = recvmsg->SearchResult;
    if (!SearchResult) {
        ogs_error("No SearchResult");
        return;
    }

    amf_nnrf_handle_nf_discover_search_result(sbi_object, SearchResult);

    ogs_assert(xact->target_nf_type);
    nf_instance = OGS_SBI_NF_INSTANCE(sbi_object, xact->target_nf_type);
    if (!nf_instance) {
        ogs_assert(sbi_object->type > OGS_SBI_OBJ_BASE &&
                    sbi_object->type < OGS_SBI_OBJ_TOP);
        switch(sbi_object->type) {
        case OGS_SBI_OBJ_UE_TYPE:
            amf_ue = (amf_ue_t *)sbi_object;
            ogs_assert(amf_ue);
            ogs_error("[%s] (NF discover) No [%s]", amf_ue->suci,
                    OpenAPI_nf_type_ToString(xact->target_nf_type));
            ogs_assert(OGS_OK ==
                nas_5gs_send_gmm_reject_from_sbi(amf_ue,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT));
            break;
        case OGS_SBI_OBJ_SESS_TYPE:
            sess = (amf_sess_t *)sbi_object;
            ogs_assert(sess);
            ogs_error("[%d:%d] (NF discover) No [%s]", sess->psi, sess->pti,
                    OpenAPI_nf_type_ToString(xact->target_nf_type));
            if (sess->payload_container_type) {
                ogs_assert(OGS_OK ==
                    nas_5gs_send_back_gsm_message(sess,
                        OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                        AMF_NAS_BACKOFF_TIME));
            } else {
                ogs_assert(OGS_OK ==
                    ngap_send_error_indication2(amf_ue,
                        NGAP_Cause_PR_transport,
                        NGAP_CauseTransport_transport_resource_unavailable));
            }
            break;
        default:
            ogs_fatal("(NF discover) Not implemented [%s:%d]",
                OpenAPI_nf_type_ToString(xact->target_nf_type),
                sbi_object->type);
            ogs_assert_if_reached();
        }
    } else {
        ogs_assert(true == amf_sbi_send(nf_instance, xact));
    }
}

void amf_nnrf_handle_nf_discover_search_result(
        ogs_sbi_object_t *sbi_object, OpenAPI_search_result_t *SearchResult)
{
    bool handled;

    OpenAPI_lnode_t *node = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(sbi_object);
    ogs_assert(SearchResult);

    OpenAPI_list_for_each(SearchResult->nf_instances, node) {
        OpenAPI_nf_profile_t *NFProfile = NULL;

        if (!node->data) continue;

        NFProfile = node->data;

        nf_instance = ogs_sbi_nf_instance_find(NFProfile->nf_instance_id);
        if (!nf_instance) {
            nf_instance = ogs_sbi_nf_instance_add();
            ogs_assert(nf_instance);
            ogs_sbi_nf_instance_set_id(nf_instance, NFProfile->nf_instance_id);

            amf_nf_fsm_init(nf_instance);

            ogs_info("[%s] (NF-discover) NF registered", nf_instance->id);
        } else {
            nf_instance->reference_count++;

            OGS_FSM_TRAN(&nf_instance->sm, amf_nf_state_registered);
            ogs_fsm_dispatch(&nf_instance->sm, NULL);

            ogs_warn("[%s] (NF-discover) NF has already been added",
                    NFProfile->nf_instance_id);
        }

        if (NF_INSTANCE_IS_OTHERS(nf_instance->id)) {
            amf_ue_t *amf_ue = NULL;
            amf_sess_t *sess = NULL;

            handled = ogs_sbi_nnrf_handle_nf_profile(
                        nf_instance, NFProfile, NULL, NULL);
            if (!handled) {
                ogs_error("ogs_sbi_nnrf_handle_nf_profile() failed [%s]",
                        nf_instance->id);
                AMF_NF_INSTANCE_CLEAR("NRF-discover", nf_instance);
                continue;
            }

            handled = ogs_sbi_client_associate(nf_instance);
            if (!handled) {
                ogs_error("[%s] Cannot assciate NF EndPoint", nf_instance->id);
                AMF_NF_INSTANCE_CLEAR("NRF-discover", nf_instance);
                continue;
            }

            switch(sbi_object->type) {
            case OGS_SBI_OBJ_UE_TYPE:
                amf_ue = (amf_ue_t *)sbi_object;
                ogs_assert(amf_ue);
                amf_ue_select_nf(amf_ue, nf_instance->nf_type);
                break;
            case OGS_SBI_OBJ_SESS_TYPE:
                sess = (amf_sess_t *)sbi_object;
                ogs_assert(sess);
                amf_sess_select_nf(sess, nf_instance->nf_type);
                break;
            default:
                ogs_fatal("(NF discover search result) Not implemented [%d]",
                            sbi_object->type);
                ogs_assert_if_reached();
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
                ogs_warn("[%s] NF Instance validity-time should not 0",
                        nf_instance->id);

            ogs_info("[%s] (NF-discover) NF Profile updated", nf_instance->id);
        }
    }
}
