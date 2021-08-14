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
#include "nnrf-handler.h"

static int server_cb(ogs_sbi_request_t *request, void *data)
{
    amf_event_t *e = NULL;
    int rv;

    ogs_assert(request);
    ogs_assert(data);

    e = amf_event_new(AMF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.request = request;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        amf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_cb(ogs_sbi_response_t *response, void *data)
{
    amf_event_t *e = NULL;
    int rv;

    ogs_assert(response);

    e = amf_event_new(AMF_EVT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        amf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int amf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    if (ogs_sbi_server_start_all(server_cb) != OGS_OK)
        return OGS_ERROR;

    /*
     * The connection between NF and NRF is a little special.
     *
     * NF and NRF share nf_instance. I get the NRF EndPoint(client) information
     * the configuration file via lib/sbi/context.c.
     *
     * ogs_sbi_self()->nf_instance_id means NF's InstanceId.
     */
    ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance) {
        ogs_sbi_nf_service_t *service = NULL;
        ogs_sbi_client_t *client = NULL;

        /* Build NF instance information. It will be transmitted to NRF. */
        ogs_sbi_nf_instance_build_default(nf_instance, amf_self()->nf_type);
        ogs_sbi_nf_instance_add_allowed_nf_type(
                nf_instance, OpenAPI_nf_type_SMF);

        /* Build NF service information. It will be transmitted to NRF. */
        service = ogs_sbi_nf_service_build_default(nf_instance,
                (char*)OGS_SBI_SERVICE_NAME_NAMF_COMM);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V1,
                (char*)OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_SMF);

        /* Client callback is only used when NF sends to NRF */
        client = nf_instance->client;
        ogs_assert(client);
        client->cb = client_cb;

        /* NFRegister is sent and the response is received
         * by the above client callback. */
        amf_nf_fsm_init(nf_instance);
    }

    return OGS_OK;
}

void amf_sbi_close(void)
{
    ogs_sbi_server_stop_all();
}

bool amf_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = amf_nnrf_nfm_build_register(nf_instance);
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_client_send_request(
            client, client->cb, request, nf_instance);
}

bool amf_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    return ogs_sbi_send(nf_instance, client_cb, xact);
}

bool amf_ue_sbi_discover_and_send(
        OpenAPI_nf_type_e target_nf_type, amf_ue_t *amf_ue, void *data,
        ogs_sbi_request_t *(*build)(amf_ue_t *amf_ue, void *data))
{
    ogs_sbi_xact_t *xact = NULL;

    ogs_assert(target_nf_type);
    ogs_assert(amf_ue);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(target_nf_type, &amf_ue->sbi,
            (ogs_sbi_build_f)build, amf_ue, data,
            amf_timer_sbi_client_wait_expire);
    if (!xact) {
        ogs_error("amf_ue_sbi_discover_and_send() failed");
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT));
        return false;
    }

    if (ogs_sbi_discover_and_send(xact,
            (ogs_fsm_handler_t)amf_nf_state_registered, client_cb) != true) {
        ogs_error("amf_ue_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(OGS_OK ==
            nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT));
        return false;
    }

    return true;
}

bool amf_sess_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        amf_sess_t *sess, int state, void *data,
        ogs_sbi_request_t *(*build)(amf_sess_t *sess, void *data))
{
    ogs_sbi_xact_t *xact = NULL;

    ogs_assert(target_nf_type);
    ogs_assert(sess);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(target_nf_type, &sess->sbi,
            (ogs_sbi_build_f)build, sess, data,
            amf_timer_sbi_client_wait_expire);
    if (!xact) {
        ogs_error("amf_sess_sbi_discover_and_send() failed");
        ogs_assert(OGS_OK == nas_5gs_send_back_gsm_message(sess,
            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED, AMF_NAS_BACKOFF_TIME));
        return false;
    }

    xact->state = state;

    if (ogs_sbi_discover_and_send(xact,
            (ogs_fsm_handler_t)amf_nf_state_registered, client_cb) != true) {
        ogs_error("amf_sess_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(OGS_OK == nas_5gs_send_back_gsm_message(sess,
            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED, AMF_NAS_BACKOFF_TIME));
        return false;
    }

    return true;
}
static int client_discover_cb(ogs_sbi_response_t *response, void *data)
{
    int rv;

    ogs_sbi_message_t message;
    amf_sess_t *sess = data;

    ogs_assert(response);
    ogs_assert(sess);
    ogs_assert(sess->sbi.type == OGS_SBI_OBJ_SESS_TYPE);

    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        ogs_error("cannot parse HTTP response");
        ogs_assert(OGS_OK == nas_5gs_send_back_gsm_message(sess,
            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED, AMF_NAS_BACKOFF_TIME));

        goto cleanup;
    }

    if (message.res_status != OGS_SBI_HTTP_STATUS_OK) {
        ogs_error("NF-Discover failed [%d]", message.res_status);
        ogs_assert(OGS_OK == nas_5gs_send_back_gsm_message(sess,
            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED, AMF_NAS_BACKOFF_TIME));

        goto cleanup;
    }

    if (!message.SearchResult) {
        ogs_error("No SearchResult");
        ogs_assert(OGS_OK == nas_5gs_send_back_gsm_message(sess,
            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED, AMF_NAS_BACKOFF_TIME));

        goto cleanup;
    }

    amf_nnrf_handle_nf_discover_search_result(
            &sess->sbi, message.SearchResult);

    if (!OGS_SBI_NF_INSTANCE(&sess->sbi, OpenAPI_nf_type_SMF)) {
        ogs_error("Cannot discover [%s]",
                    OpenAPI_nf_type_ToString(OpenAPI_nf_type_SMF));
        ogs_assert(OGS_OK ==
            nas_5gs_send_back_gsm_message(sess,
                OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                AMF_NAS_BACKOFF_TIME));

        goto cleanup;
    }

    amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
            sess, AMF_CREATE_SM_CONTEXT_NO_STATE, NULL,
            amf_nsmf_pdusession_build_create_sm_context);

cleanup:
    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);

    return OGS_ERROR;
}

bool amf_sess_sbi_discover_by_nsi(
        OpenAPI_nf_type_e target_nf_type, amf_sess_t *sess)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(target_nf_type);
    ogs_assert(sess);
    client = sess->nssf.nrf.client;
    ogs_assert(client);

    request = amf_nnrf_disc_build_discover(
            sess->nssf.nrf.id, target_nf_type, amf_self()->nf_type);
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_client_send_request(
            client, client_discover_cb, request, sess);
}

void amf_sbi_send_activating_session(amf_sess_t *sess, int state)
{
    amf_nsmf_pdusession_sm_context_param_t param;

    ogs_assert(sess);

    memset(&param, 0, sizeof(param));
    param.upCnxState = OpenAPI_up_cnx_state_ACTIVATING;

    amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
            sess, state, &param,
            amf_nsmf_pdusession_build_update_sm_context);
}

void amf_sbi_send_deactivate_session(
        amf_sess_t *sess, int state, int group, int cause)
{
    amf_nsmf_pdusession_sm_context_param_t param;

    ogs_assert(sess);

    memset(&param, 0, sizeof(param));
    param.upCnxState = OpenAPI_up_cnx_state_DEACTIVATED;
    param.ngApCause.group = group;
    param.ngApCause.value = cause;
    param.ue_location = true;
    param.ue_timezone = true;

    amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
            sess, state, &param, amf_nsmf_pdusession_build_update_sm_context);
}

void amf_sbi_send_deactivate_all_sessions(
        amf_ue_t *amf_ue, int state, int group, int cause)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        if (SESSION_CONTEXT_IN_SMF(sess))
            amf_sbi_send_deactivate_session(sess, state, group, cause);
    }
}

void amf_sbi_send_deactivate_all_ue_in_gnb(amf_gnb_t *gnb, int state)
{
    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL, *ran_ue_next;

    ogs_list_for_each_safe(&gnb->ran_ue_list, ran_ue_next, ran_ue) {
        int old_xact_count = 0, new_xact_count = 0;

        amf_ue = ran_ue->amf_ue;

        if (amf_ue) {
            old_xact_count = amf_sess_xact_count(amf_ue);

            amf_sbi_send_deactivate_all_sessions(
                amf_ue, state, NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_failure_in_radio_interface_procedure);

            new_xact_count = amf_sess_xact_count(amf_ue);

            if (old_xact_count == new_xact_count) {
                ran_ue_remove(ran_ue);
                amf_ue_deassociate(amf_ue);
            }
        } else {
            ogs_warn("amf_sbi_send_deactivate_all_ue_in_gnb()");
            ogs_warn("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld] State[%d]",
                ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id,
                state);

            if (state == AMF_REMOVE_S1_CONTEXT_BY_LO_CONNREFUSED ||
                state == AMF_REMOVE_S1_CONTEXT_BY_RESET_ALL) {
                ran_ue_remove(ran_ue);
            } else {
                /* At this point, it does not support other action */
                ogs_fatal("Invalid state [%d]", state);
                ogs_assert_if_reached();
            }
        }
    }
}

void amf_sbi_send_release_session(amf_sess_t *sess, int state)
{
    ogs_assert(sess);

    amf_sess_sbi_discover_and_send(
            OpenAPI_nf_type_SMF, sess, state, NULL,
            amf_nsmf_pdusession_build_release_sm_context);

    /* Prevent to invoke SMF for this session */
    CLEAR_SM_CONTEXT_REF(sess);
}

void amf_sbi_send_release_all_sessions(amf_ue_t *amf_ue, int state)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        if (SESSION_CONTEXT_IN_SMF(sess))
            amf_sbi_send_release_session(sess, state);
    }
}

static int client_notify_cb(ogs_sbi_response_t *response, void *data)
{
    int rv;

    ogs_sbi_message_t message;

    ogs_assert(response);

    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        ogs_error("cannot parse HTTP response");
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    if (message.res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT)
        ogs_error("N1-N2-Message Transfer Failure Notification failed [%d]",
                message.res_status);

    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);
    return OGS_OK;
}

bool amf_sbi_send_n1_n2_failure_notify(
        amf_sess_t *sess, OpenAPI_n1_n2_message_transfer_cause_e cause)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(cause);
    ogs_assert(sess);
    client = sess->paging.client;
    ogs_assert(client);

    request = amf_nsmf_callback_build_n1_n2_failure_notify(sess, cause);
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_client_send_request(client, client_notify_cb, request, NULL);
}
