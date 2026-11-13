#include "sbi-path.h"

int ausf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_sbi_nf_fsm_init(nf_instance);

    /* Build NF instance information. */
    ogs_sbi_nf_instance_build_default(nf_instance);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_AUSF);

    /* Build NF service information. */
    if (ogs_sbi_nf_service_is_available(OGS_SBI_SERVICE_NAME_NAANF_AUTH)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NAANF_AUTH);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AUSF);
    }

    /* Initialize NRF NF Instance */
    // nf_instance = ogs_sbi_self()->nrf_instance;
    // if (nf_instance)
    //     ogs_sbi_nf_fsm_init(nf_instance);

    /* Setup Subscription-Data */
    // ogs_sbi_subscription_spec_add(OpenAPI_nf_type_SEPP, NULL);
    // ogs_sbi_subscription_spec_add(
    //         OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NUDM_UEAU);

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void aanf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool aaunf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    ogs_assert(nf_instance);
    ogs_assert(xact);
    return ogs_sbi_send_request_to_nf_instance(nf_instance, xact);
}

int ausf_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(ausf_ue_t *ausf_ue, void *data),
        ausf_ue_t *ausf_ue, ogs_sbi_stream_t *stream, void *data)
{
    ogs_sbi_xact_t *xact = NULL;
    int r;

    ogs_assert(service_type);
    ogs_assert(ausf_ue);
    ogs_assert(stream);
    ogs_assert(build);

    ogs_assert(ausf_ue->id >= OGS_MIN_POOL_ID &&
            ausf_ue->id <= OGS_MAX_POOL_ID);

    xact = ogs_sbi_xact_add(
            ausf_ue->id, &ausf_ue->sbi, service_type, discovery_option,
            (ogs_sbi_build_f)build, ausf_ue, data);
    if (!xact) {
        ogs_error("ausf_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", ausf_ue->suci, NULL));
        return OGS_ERROR;
    }

    xact->assoc_stream_id = ogs_sbi_id_from_stream(stream);
    ogs_assert(xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
            xact->assoc_stream_id <= OGS_MAX_POOL_ID);

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("ausf_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", ausf_ue->suci, NULL));
        return r;
    }
    
    return OGS_OK;
}
