
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "routing_info_sm_response.h"

OpenAPI_routing_info_sm_response_t *OpenAPI_routing_info_sm_response_create(
    char *supi,
    OpenAPI_smsf_registration_t *smsf3_gpp,
    OpenAPI_smsf_registration_t *smsf_non3_gpp,
    OpenAPI_ip_sm_gw_info_t *ip_sm_gw,
    OpenAPI_sms_router_info_t *sms_router
)
{
    OpenAPI_routing_info_sm_response_t *routing_info_sm_response_local_var = ogs_malloc(sizeof(OpenAPI_routing_info_sm_response_t));
    ogs_assert(routing_info_sm_response_local_var);

    routing_info_sm_response_local_var->supi = supi;
    routing_info_sm_response_local_var->smsf3_gpp = smsf3_gpp;
    routing_info_sm_response_local_var->smsf_non3_gpp = smsf_non3_gpp;
    routing_info_sm_response_local_var->ip_sm_gw = ip_sm_gw;
    routing_info_sm_response_local_var->sms_router = sms_router;

    return routing_info_sm_response_local_var;
}

void OpenAPI_routing_info_sm_response_free(OpenAPI_routing_info_sm_response_t *routing_info_sm_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == routing_info_sm_response) {
        return;
    }
    if (routing_info_sm_response->supi) {
        ogs_free(routing_info_sm_response->supi);
        routing_info_sm_response->supi = NULL;
    }
    if (routing_info_sm_response->smsf3_gpp) {
        OpenAPI_smsf_registration_free(routing_info_sm_response->smsf3_gpp);
        routing_info_sm_response->smsf3_gpp = NULL;
    }
    if (routing_info_sm_response->smsf_non3_gpp) {
        OpenAPI_smsf_registration_free(routing_info_sm_response->smsf_non3_gpp);
        routing_info_sm_response->smsf_non3_gpp = NULL;
    }
    if (routing_info_sm_response->ip_sm_gw) {
        OpenAPI_ip_sm_gw_info_free(routing_info_sm_response->ip_sm_gw);
        routing_info_sm_response->ip_sm_gw = NULL;
    }
    if (routing_info_sm_response->sms_router) {
        OpenAPI_sms_router_info_free(routing_info_sm_response->sms_router);
        routing_info_sm_response->sms_router = NULL;
    }
    ogs_free(routing_info_sm_response);
}

cJSON *OpenAPI_routing_info_sm_response_convertToJSON(OpenAPI_routing_info_sm_response_t *routing_info_sm_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (routing_info_sm_response == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [RoutingInfoSmResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (routing_info_sm_response->supi) {
    if (cJSON_AddStringToObject(item, "supi", routing_info_sm_response->supi) == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (routing_info_sm_response->smsf3_gpp) {
    cJSON *smsf3_gpp_local_JSON = OpenAPI_smsf_registration_convertToJSON(routing_info_sm_response->smsf3_gpp);
    if (smsf3_gpp_local_JSON == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [smsf3_gpp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsf3Gpp", smsf3_gpp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [smsf3_gpp]");
        goto end;
    }
    }

    if (routing_info_sm_response->smsf_non3_gpp) {
    cJSON *smsf_non3_gpp_local_JSON = OpenAPI_smsf_registration_convertToJSON(routing_info_sm_response->smsf_non3_gpp);
    if (smsf_non3_gpp_local_JSON == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [smsf_non3_gpp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsfNon3Gpp", smsf_non3_gpp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [smsf_non3_gpp]");
        goto end;
    }
    }

    if (routing_info_sm_response->ip_sm_gw) {
    cJSON *ip_sm_gw_local_JSON = OpenAPI_ip_sm_gw_info_convertToJSON(routing_info_sm_response->ip_sm_gw);
    if (ip_sm_gw_local_JSON == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [ip_sm_gw]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipSmGw", ip_sm_gw_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [ip_sm_gw]");
        goto end;
    }
    }

    if (routing_info_sm_response->sms_router) {
    cJSON *sms_router_local_JSON = OpenAPI_sms_router_info_convertToJSON(routing_info_sm_response->sms_router);
    if (sms_router_local_JSON == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [sms_router]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsRouter", sms_router_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed [sms_router]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_routing_info_sm_response_t *OpenAPI_routing_info_sm_response_parseFromJSON(cJSON *routing_info_sm_responseJSON)
{
    OpenAPI_routing_info_sm_response_t *routing_info_sm_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *smsf3_gpp = NULL;
    OpenAPI_smsf_registration_t *smsf3_gpp_local_nonprim = NULL;
    cJSON *smsf_non3_gpp = NULL;
    OpenAPI_smsf_registration_t *smsf_non3_gpp_local_nonprim = NULL;
    cJSON *ip_sm_gw = NULL;
    OpenAPI_ip_sm_gw_info_t *ip_sm_gw_local_nonprim = NULL;
    cJSON *sms_router = NULL;
    OpenAPI_sms_router_info_t *sms_router_local_nonprim = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(routing_info_sm_responseJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_routing_info_sm_response_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    smsf3_gpp = cJSON_GetObjectItemCaseSensitive(routing_info_sm_responseJSON, "smsf3Gpp");
    if (smsf3_gpp) {
    smsf3_gpp_local_nonprim = OpenAPI_smsf_registration_parseFromJSON(smsf3_gpp);
    if (!smsf3_gpp_local_nonprim) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON failed [smsf3_gpp]");
        goto end;
    }
    }

    smsf_non3_gpp = cJSON_GetObjectItemCaseSensitive(routing_info_sm_responseJSON, "smsfNon3Gpp");
    if (smsf_non3_gpp) {
    smsf_non3_gpp_local_nonprim = OpenAPI_smsf_registration_parseFromJSON(smsf_non3_gpp);
    if (!smsf_non3_gpp_local_nonprim) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON failed [smsf_non3_gpp]");
        goto end;
    }
    }

    ip_sm_gw = cJSON_GetObjectItemCaseSensitive(routing_info_sm_responseJSON, "ipSmGw");
    if (ip_sm_gw) {
    ip_sm_gw_local_nonprim = OpenAPI_ip_sm_gw_info_parseFromJSON(ip_sm_gw);
    if (!ip_sm_gw_local_nonprim) {
        ogs_error("OpenAPI_ip_sm_gw_info_parseFromJSON failed [ip_sm_gw]");
        goto end;
    }
    }

    sms_router = cJSON_GetObjectItemCaseSensitive(routing_info_sm_responseJSON, "smsRouter");
    if (sms_router) {
    sms_router_local_nonprim = OpenAPI_sms_router_info_parseFromJSON(sms_router);
    if (!sms_router_local_nonprim) {
        ogs_error("OpenAPI_sms_router_info_parseFromJSON failed [sms_router]");
        goto end;
    }
    }

    routing_info_sm_response_local_var = OpenAPI_routing_info_sm_response_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        smsf3_gpp ? smsf3_gpp_local_nonprim : NULL,
        smsf_non3_gpp ? smsf_non3_gpp_local_nonprim : NULL,
        ip_sm_gw ? ip_sm_gw_local_nonprim : NULL,
        sms_router ? sms_router_local_nonprim : NULL
    );

    return routing_info_sm_response_local_var;
end:
    if (smsf3_gpp_local_nonprim) {
        OpenAPI_smsf_registration_free(smsf3_gpp_local_nonprim);
        smsf3_gpp_local_nonprim = NULL;
    }
    if (smsf_non3_gpp_local_nonprim) {
        OpenAPI_smsf_registration_free(smsf_non3_gpp_local_nonprim);
        smsf_non3_gpp_local_nonprim = NULL;
    }
    if (ip_sm_gw_local_nonprim) {
        OpenAPI_ip_sm_gw_info_free(ip_sm_gw_local_nonprim);
        ip_sm_gw_local_nonprim = NULL;
    }
    if (sms_router_local_nonprim) {
        OpenAPI_sms_router_info_free(sms_router_local_nonprim);
        sms_router_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_routing_info_sm_response_t *OpenAPI_routing_info_sm_response_copy(OpenAPI_routing_info_sm_response_t *dst, OpenAPI_routing_info_sm_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_routing_info_sm_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_routing_info_sm_response_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_routing_info_sm_response_free(dst);
    dst = OpenAPI_routing_info_sm_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

