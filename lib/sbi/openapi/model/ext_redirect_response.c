
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ext_redirect_response.h"

OpenAPI_ext_redirect_response_t *OpenAPI_ext_redirect_response_create(
    char *cause,
    char *target_scp,
    char *target_sepp,
    char *sepp_fqdn_for_discovery
)
{
    OpenAPI_ext_redirect_response_t *ext_redirect_response_local_var = ogs_malloc(sizeof(OpenAPI_ext_redirect_response_t));
    ogs_assert(ext_redirect_response_local_var);

    ext_redirect_response_local_var->cause = cause;
    ext_redirect_response_local_var->target_scp = target_scp;
    ext_redirect_response_local_var->target_sepp = target_sepp;
    ext_redirect_response_local_var->sepp_fqdn_for_discovery = sepp_fqdn_for_discovery;

    return ext_redirect_response_local_var;
}

void OpenAPI_ext_redirect_response_free(OpenAPI_ext_redirect_response_t *ext_redirect_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ext_redirect_response) {
        return;
    }
    if (ext_redirect_response->cause) {
        ogs_free(ext_redirect_response->cause);
        ext_redirect_response->cause = NULL;
    }
    if (ext_redirect_response->target_scp) {
        ogs_free(ext_redirect_response->target_scp);
        ext_redirect_response->target_scp = NULL;
    }
    if (ext_redirect_response->target_sepp) {
        ogs_free(ext_redirect_response->target_sepp);
        ext_redirect_response->target_sepp = NULL;
    }
    if (ext_redirect_response->sepp_fqdn_for_discovery) {
        ogs_free(ext_redirect_response->sepp_fqdn_for_discovery);
        ext_redirect_response->sepp_fqdn_for_discovery = NULL;
    }
    ogs_free(ext_redirect_response);
}

cJSON *OpenAPI_ext_redirect_response_convertToJSON(OpenAPI_ext_redirect_response_t *ext_redirect_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ext_redirect_response == NULL) {
        ogs_error("OpenAPI_ext_redirect_response_convertToJSON() failed [ExtRedirectResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ext_redirect_response->cause) {
    if (cJSON_AddStringToObject(item, "cause", ext_redirect_response->cause) == NULL) {
        ogs_error("OpenAPI_ext_redirect_response_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (ext_redirect_response->target_scp) {
    if (cJSON_AddStringToObject(item, "targetScp", ext_redirect_response->target_scp) == NULL) {
        ogs_error("OpenAPI_ext_redirect_response_convertToJSON() failed [target_scp]");
        goto end;
    }
    }

    if (ext_redirect_response->target_sepp) {
    if (cJSON_AddStringToObject(item, "targetSepp", ext_redirect_response->target_sepp) == NULL) {
        ogs_error("OpenAPI_ext_redirect_response_convertToJSON() failed [target_sepp]");
        goto end;
    }
    }

    if (ext_redirect_response->sepp_fqdn_for_discovery) {
    if (cJSON_AddStringToObject(item, "seppFqdnForDiscovery", ext_redirect_response->sepp_fqdn_for_discovery) == NULL) {
        ogs_error("OpenAPI_ext_redirect_response_convertToJSON() failed [sepp_fqdn_for_discovery]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ext_redirect_response_t *OpenAPI_ext_redirect_response_parseFromJSON(cJSON *ext_redirect_responseJSON)
{
    OpenAPI_ext_redirect_response_t *ext_redirect_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cause = NULL;
    cJSON *target_scp = NULL;
    cJSON *target_sepp = NULL;
    cJSON *sepp_fqdn_for_discovery = NULL;
    cause = cJSON_GetObjectItemCaseSensitive(ext_redirect_responseJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause) && !cJSON_IsNull(cause)) {
        ogs_error("OpenAPI_ext_redirect_response_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    target_scp = cJSON_GetObjectItemCaseSensitive(ext_redirect_responseJSON, "targetScp");
    if (target_scp) {
    if (!cJSON_IsString(target_scp) && !cJSON_IsNull(target_scp)) {
        ogs_error("OpenAPI_ext_redirect_response_parseFromJSON() failed [target_scp]");
        goto end;
    }
    }

    target_sepp = cJSON_GetObjectItemCaseSensitive(ext_redirect_responseJSON, "targetSepp");
    if (target_sepp) {
    if (!cJSON_IsString(target_sepp) && !cJSON_IsNull(target_sepp)) {
        ogs_error("OpenAPI_ext_redirect_response_parseFromJSON() failed [target_sepp]");
        goto end;
    }
    }

    sepp_fqdn_for_discovery = cJSON_GetObjectItemCaseSensitive(ext_redirect_responseJSON, "seppFqdnForDiscovery");
    if (sepp_fqdn_for_discovery) {
    if (!cJSON_IsString(sepp_fqdn_for_discovery) && !cJSON_IsNull(sepp_fqdn_for_discovery)) {
        ogs_error("OpenAPI_ext_redirect_response_parseFromJSON() failed [sepp_fqdn_for_discovery]");
        goto end;
    }
    }

    ext_redirect_response_local_var = OpenAPI_ext_redirect_response_create (
        cause && !cJSON_IsNull(cause) ? ogs_strdup(cause->valuestring) : NULL,
        target_scp && !cJSON_IsNull(target_scp) ? ogs_strdup(target_scp->valuestring) : NULL,
        target_sepp && !cJSON_IsNull(target_sepp) ? ogs_strdup(target_sepp->valuestring) : NULL,
        sepp_fqdn_for_discovery && !cJSON_IsNull(sepp_fqdn_for_discovery) ? ogs_strdup(sepp_fqdn_for_discovery->valuestring) : NULL
    );

    return ext_redirect_response_local_var;
end:
    return NULL;
}

OpenAPI_ext_redirect_response_t *OpenAPI_ext_redirect_response_copy(OpenAPI_ext_redirect_response_t *dst, OpenAPI_ext_redirect_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ext_redirect_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ext_redirect_response_convertToJSON() failed");
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

    OpenAPI_ext_redirect_response_free(dst);
    dst = OpenAPI_ext_redirect_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

