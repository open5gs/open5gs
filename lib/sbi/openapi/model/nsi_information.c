
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsi_information.h"

OpenAPI_nsi_information_t *OpenAPI_nsi_information_create(
    char *nrf_id,
    char *nsi_id,
    char *nrf_nf_mgt_uri,
    char *nrf_access_token_uri
    )
{
    OpenAPI_nsi_information_t *nsi_information_local_var = OpenAPI_malloc(sizeof(OpenAPI_nsi_information_t));
    if (!nsi_information_local_var) {
        return NULL;
    }
    nsi_information_local_var->nrf_id = nrf_id;
    nsi_information_local_var->nsi_id = nsi_id;
    nsi_information_local_var->nrf_nf_mgt_uri = nrf_nf_mgt_uri;
    nsi_information_local_var->nrf_access_token_uri = nrf_access_token_uri;

    return nsi_information_local_var;
}

void OpenAPI_nsi_information_free(OpenAPI_nsi_information_t *nsi_information)
{
    if (NULL == nsi_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nsi_information->nrf_id);
    ogs_free(nsi_information->nsi_id);
    ogs_free(nsi_information->nrf_nf_mgt_uri);
    ogs_free(nsi_information->nrf_access_token_uri);
    ogs_free(nsi_information);
}

cJSON *OpenAPI_nsi_information_convertToJSON(OpenAPI_nsi_information_t *nsi_information)
{
    cJSON *item = NULL;

    if (nsi_information == NULL) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [NsiInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nsi_information->nrf_id) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nrfId", nsi_information->nrf_id) == NULL) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_id]");
        goto end;
    }

    if (nsi_information->nsi_id) {
        if (cJSON_AddStringToObject(item, "nsiId", nsi_information->nsi_id) == NULL) {
            ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nsi_id]");
            goto end;
        }
    }

    if (nsi_information->nrf_nf_mgt_uri) {
        if (cJSON_AddStringToObject(item, "nrfNfMgtUri", nsi_information->nrf_nf_mgt_uri) == NULL) {
            ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_nf_mgt_uri]");
            goto end;
        }
    }

    if (nsi_information->nrf_access_token_uri) {
        if (cJSON_AddStringToObject(item, "nrfAccessTokenUri", nsi_information->nrf_access_token_uri) == NULL) {
            ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_access_token_uri]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_nsi_information_t *OpenAPI_nsi_information_parseFromJSON(cJSON *nsi_informationJSON)
{
    OpenAPI_nsi_information_t *nsi_information_local_var = NULL;
    cJSON *nrf_id = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nrfId");
    if (!nrf_id) {
        ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_id]");
        goto end;
    }


    if (!cJSON_IsString(nrf_id)) {
        ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_id]");
        goto end;
    }

    cJSON *nsi_id = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nsiId");

    if (nsi_id) {
        if (!cJSON_IsString(nsi_id)) {
            ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nsi_id]");
            goto end;
        }
    }

    cJSON *nrf_nf_mgt_uri = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nrfNfMgtUri");

    if (nrf_nf_mgt_uri) {
        if (!cJSON_IsString(nrf_nf_mgt_uri)) {
            ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_nf_mgt_uri]");
            goto end;
        }
    }

    cJSON *nrf_access_token_uri = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nrfAccessTokenUri");

    if (nrf_access_token_uri) {
        if (!cJSON_IsString(nrf_access_token_uri)) {
            ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_access_token_uri]");
            goto end;
        }
    }

    nsi_information_local_var = OpenAPI_nsi_information_create (
        ogs_strdup(nrf_id->valuestring),
        nsi_id ? ogs_strdup(nsi_id->valuestring) : NULL,
        nrf_nf_mgt_uri ? ogs_strdup(nrf_nf_mgt_uri->valuestring) : NULL,
        nrf_access_token_uri ? ogs_strdup(nrf_access_token_uri->valuestring) : NULL
        );

    return nsi_information_local_var;
end:
    return NULL;
}

OpenAPI_nsi_information_t *OpenAPI_nsi_information_copy(OpenAPI_nsi_information_t *dst, OpenAPI_nsi_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsi_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed");
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

    OpenAPI_nsi_information_free(dst);
    dst = OpenAPI_nsi_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

