/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

int smsf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_sbi_nf_fsm_init(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_SCP);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_AMF);

    /* Build NF service information. It will be transmitted to NRF. */
    if (ogs_sbi_nf_service_is_available(
                OGS_SBI_SERVICE_NAME_NSMSF_SMS)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NSMSF_SMS);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V2, OGS_SBI_API_V2_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);
    }

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance)
        ogs_sbi_nf_fsm_init(nf_instance);

    /* Setup Subscription-Data */
    ogs_sbi_subscription_spec_add(OpenAPI_nf_type_SEPP, NULL);

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void smsf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool smsf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    ogs_assert(nf_instance);
    ogs_assert(xact);
    return ogs_sbi_send_request_to_nf_instance(nf_instance, xact);
}

static int smsf_sbi_discover_and_send(
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
            (ogs_sbi_build_f)build, context, data);
    if (!xact) {
        ogs_error("smsf_sbi_discover_and_send() failed");
        return OGS_ERROR;
    }

    xact->assoc_stream = stream;

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("smsf_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        return r;
    }

    return OGS_OK;
}

int smsf_ue_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(smsf_ue_t *smsf_ue, void *data),
        smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, void *data)
{
    int r;

    r = smsf_sbi_discover_and_send(
                &smsf_ue->sbi, service_type, discovery_option,
                (ogs_sbi_build_f)build, smsf_ue, stream, data);
    if (r != OGS_OK) {
        ogs_error("smsf_ue_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", smsf_ue->supi));
        return r;
    }

    return OGS_OK;
}

void smsf_namf_comm_send_n1_n2_message_transfer(
        smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream,
        smsf_n1_n2_message_transfer_param_t *param)
{
    ogs_sbi_xact_t *xact = NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;
    int r;

    ogs_assert(smsf_ue);

    ogs_assert(param);
    ogs_assert(param->n1smbuf);

    discovery_option = ogs_sbi_discovery_option_new();
    ogs_assert(discovery_option);
    ogs_sbi_discovery_option_set_target_nf_instance_id(
            discovery_option, smsf_ue->amf_instance_id);

    xact = ogs_sbi_xact_add(
            &smsf_ue->sbi, OGS_SBI_SERVICE_TYPE_NAMF_COMM, discovery_option,
            (ogs_sbi_build_f)smsf_namf_comm_build_n1_n2_message_transfer,
            smsf_ue, param);
    if (!xact) {
        ogs_error("smsf_namf_comm_send_n1_n2_message_transfer() failed");
        return;
    }

    xact->assoc_stream = stream;

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("smsf_namf_comm_send_n1_n2_message_transfer() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(r != OGS_ERROR);
    }
}
