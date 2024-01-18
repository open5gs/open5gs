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

#include "sbi-path.h"

int pcf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    bool smpolicycontrol_enabled = false;
    bool policyauthorization_enabled = false;

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_sbi_nf_fsm_init(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_SCP);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_AMF);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_SMF);

    /* Build NF service information. It will be transmitted to NRF. */
    if (ogs_sbi_nf_service_is_available(
                OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);
    }

    if (ogs_sbi_nf_service_is_available(
                OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_SMF);

        smpolicycontrol_enabled = true;
    }

    if (ogs_sbi_nf_service_is_available(
                OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);

        policyauthorization_enabled = true;
    }

    if ((smpolicycontrol_enabled == true &&
            policyauthorization_enabled == false) ||
        (smpolicycontrol_enabled == false &&
            policyauthorization_enabled == true)) {
        ogs_fatal("CHECK CONFIGURATION:");
        ogs_fatal("   %s - %s",
            OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL,
            smpolicycontrol_enabled ? "enabled" : "disabled");
        ogs_fatal("   %s - %s",
            OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION,
            policyauthorization_enabled ? "enabled" : "disabled");
        ogs_fatal("Only one of %s and %s cannot be enabled.",
            OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL,
            OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION);
        ogs_fatal("They can be enabled or disabled together.");

        return OGS_ERROR;
    }

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance)
        ogs_sbi_nf_fsm_init(nf_instance);

    /* Setup Subscription-Data */
    ogs_sbi_subscription_spec_add(OpenAPI_nf_type_SEPP, NULL);
    ogs_sbi_subscription_spec_add(
            OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT);
    ogs_sbi_subscription_spec_add(
            OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NUDR_DR);

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void pcf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool pcf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    ogs_assert(nf_instance);
    ogs_assert(xact);
    return ogs_sbi_send_request_to_nf_instance(nf_instance, xact);
}

static int pcf_sbi_discover_and_send(
        ogs_sbi_object_t *sbi_object,
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_build_f build,
        void *context, ogs_sbi_stream_t *stream, void *data)
{
    ogs_sbi_xact_t *xact = NULL;
    int r;

    ogs_assert(service_type);
    ogs_assert(sbi_object);
    ogs_assert(stream);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(
            sbi_object, service_type, discovery_option,
            build, context, data);
    if (!xact) {
        ogs_error("ogs_sbi_xact_add() failed");
        return OGS_ERROR;
    }

    xact->assoc_stream = stream;

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("ogs_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        return r;
    }

    return OGS_OK;
}

int pcf_ue_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(pcf_ue_t *pcf_ue, void *data),
        pcf_ue_t *pcf_ue, ogs_sbi_stream_t *stream, void *data)
{
    int r;

    r = pcf_sbi_discover_and_send(
                &pcf_ue->sbi, service_type, discovery_option,
                (ogs_sbi_build_f)build, pcf_ue, stream, data);
    if (r != OGS_OK) {
        ogs_error("pcf_ue_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", pcf_ue->supi));
        return r;
    }

    return OGS_OK;
}

int pcf_sess_sbi_discover_only(
        pcf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_sbi_service_type_e service_type)
{
    ogs_sbi_xact_t *xact = NULL;

    ogs_assert(sess);
    ogs_assert(service_type);

    xact = ogs_sbi_xact_add(&sess->sbi, service_type, NULL, NULL, NULL, NULL);
    if (!xact) {
        ogs_error("ogs_sbi_xact_add() failed");
        return OGS_ERROR;
    }

    xact->assoc_stream = stream;

    return ogs_sbi_discover_only(xact);
}

int pcf_sess_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(pcf_sess_t *sess, void *data),
        pcf_sess_t *sess, ogs_sbi_stream_t *stream, void *data)
{
    int r;

    r = pcf_sbi_discover_and_send(
                &sess->sbi, service_type, discovery_option,
                (ogs_sbi_build_f)build, sess, stream, data);
    if (r != OGS_OK) {
        ogs_error("pcf_sess_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", NULL));
        return r;
    }

    return OGS_OK;
}

static int client_notify_cb(
        int status, ogs_sbi_response_t *response, void *data)
{
    int rv;
    ogs_sbi_message_t message;

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "client_notify_cb() failed [%d]", status);
        return OGS_ERROR;
    }

    ogs_assert(response);

    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        ogs_error("cannot parse HTTP response");
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    if (message.res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT)
        ogs_error("SmContextStatusNotification failed [%d]",
                message.res_status);

    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);
    return OGS_OK;
}

static int client_delete_notify_cb(
        int status, ogs_sbi_response_t *response, void *data)
{
    pcf_app_t *app_session = data;

    ogs_assert(app_session);
    client_notify_cb(status, response, data);

    pcf_app_remove(app_session);

    return OGS_OK;
}

bool pcf_sbi_send_am_policy_control_notify(pcf_ue_t *pcf_ue)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(pcf_ue);
    client = pcf_ue->namf.client;
    ogs_assert(client);

    request = pcf_namf_callback_build_am_policy_control(pcf_ue, NULL);
    if (!request) {
        ogs_error("pcf_namf_callback_build_am_policy_control() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool pcf_sbi_send_smpolicycontrol_update_notify(
        pcf_sess_t *sess, OpenAPI_sm_policy_decision_t *SmPolicyDecision)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->nsmf.client;
    ogs_assert(client);

    request = pcf_nsmf_callback_build_smpolicycontrol_update(
                sess, SmPolicyDecision);
    if (!request) {
        ogs_error("pcf_nsmf_callback_build_smpolicycontrol_update() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool pcf_sbi_send_smpolicycontrol_delete_notify(
        pcf_sess_t *sess, pcf_app_t *app_session,
        OpenAPI_sm_policy_decision_t *SmPolicyDecision)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->nsmf.client;
    ogs_assert(client);

    request = pcf_nsmf_callback_build_smpolicycontrol_update(
                sess, SmPolicyDecision);
    if (!request) {
        ogs_error("pcf_nsmf_callback_build_smpolicycontrol_update() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_delete_notify_cb, request, app_session);
    if (rc == false) {
        ogs_error("ogs_sbi_send_request_to_client() failed");
        pcf_app_remove(app_session);
    }

    ogs_sbi_request_free(request);

    return rc;
}

bool pcf_sbi_send_policyauthorization_terminate_notify(pcf_app_t *app)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(app);
    client = app->naf.client;
    ogs_assert(client);

    request = pcf_naf_callback_build_policyauthorization_terminate(app, NULL);
    if (!request) {
        ogs_error("pcf_naf_callback_build_policyauthorization_terminate() "
                "failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}
