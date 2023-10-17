
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_service_version.h"

OpenAPI_nf_service_version_t *OpenAPI_nf_service_version_create(
    char *api_version_in_uri,
    char *api_full_version,
    char *expiry
)
{
    OpenAPI_nf_service_version_t *nf_service_version_local_var = ogs_malloc(sizeof(OpenAPI_nf_service_version_t));
    ogs_assert(nf_service_version_local_var);

    nf_service_version_local_var->api_version_in_uri = api_version_in_uri;
    nf_service_version_local_var->api_full_version = api_full_version;
    nf_service_version_local_var->expiry = expiry;

    return nf_service_version_local_var;
}

void OpenAPI_nf_service_version_free(OpenAPI_nf_service_version_t *nf_service_version)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nf_service_version) {
        return;
    }
    if (nf_service_version->api_version_in_uri) {
        ogs_free(nf_service_version->api_version_in_uri);
        nf_service_version->api_version_in_uri = NULL;
    }
    if (nf_service_version->api_full_version) {
        ogs_free(nf_service_version->api_full_version);
        nf_service_version->api_full_version = NULL;
    }
    if (nf_service_version->expiry) {
        ogs_free(nf_service_version->expiry);
        nf_service_version->expiry = NULL;
    }
    ogs_free(nf_service_version);
}

cJSON *OpenAPI_nf_service_version_convertToJSON(OpenAPI_nf_service_version_t *nf_service_version)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nf_service_version == NULL) {
        ogs_error("OpenAPI_nf_service_version_convertToJSON() failed [NFServiceVersion]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_service_version->api_version_in_uri) {
        ogs_error("OpenAPI_nf_service_version_convertToJSON() failed [api_version_in_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "apiVersionInUri", nf_service_version->api_version_in_uri) == NULL) {
        ogs_error("OpenAPI_nf_service_version_convertToJSON() failed [api_version_in_uri]");
        goto end;
    }

    if (!nf_service_version->api_full_version) {
        ogs_error("OpenAPI_nf_service_version_convertToJSON() failed [api_full_version]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "apiFullVersion", nf_service_version->api_full_version) == NULL) {
        ogs_error("OpenAPI_nf_service_version_convertToJSON() failed [api_full_version]");
        goto end;
    }

    if (nf_service_version->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", nf_service_version->expiry) == NULL) {
        ogs_error("OpenAPI_nf_service_version_convertToJSON() failed [expiry]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nf_service_version_t *OpenAPI_nf_service_version_parseFromJSON(cJSON *nf_service_versionJSON)
{
    OpenAPI_nf_service_version_t *nf_service_version_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *api_version_in_uri = NULL;
    cJSON *api_full_version = NULL;
    cJSON *expiry = NULL;
    api_version_in_uri = cJSON_GetObjectItemCaseSensitive(nf_service_versionJSON, "apiVersionInUri");
    if (!api_version_in_uri) {
        ogs_error("OpenAPI_nf_service_version_parseFromJSON() failed [api_version_in_uri]");
        goto end;
    }
    if (!cJSON_IsString(api_version_in_uri)) {
        ogs_error("OpenAPI_nf_service_version_parseFromJSON() failed [api_version_in_uri]");
        goto end;
    }

    api_full_version = cJSON_GetObjectItemCaseSensitive(nf_service_versionJSON, "apiFullVersion");
    if (!api_full_version) {
        ogs_error("OpenAPI_nf_service_version_parseFromJSON() failed [api_full_version]");
        goto end;
    }
    if (!cJSON_IsString(api_full_version)) {
        ogs_error("OpenAPI_nf_service_version_parseFromJSON() failed [api_full_version]");
        goto end;
    }

    expiry = cJSON_GetObjectItemCaseSensitive(nf_service_versionJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_nf_service_version_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    nf_service_version_local_var = OpenAPI_nf_service_version_create (
        ogs_strdup(api_version_in_uri->valuestring),
        ogs_strdup(api_full_version->valuestring),
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL
    );

    return nf_service_version_local_var;
end:
    return NULL;
}

OpenAPI_nf_service_version_t *OpenAPI_nf_service_version_copy(OpenAPI_nf_service_version_t *dst, OpenAPI_nf_service_version_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_service_version_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_service_version_convertToJSON() failed");
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

    OpenAPI_nf_service_version_free(dst);
    dst = OpenAPI_nf_service_version_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

