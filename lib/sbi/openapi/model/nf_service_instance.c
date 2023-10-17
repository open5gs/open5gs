
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_service_instance.h"

OpenAPI_nf_service_instance_t *OpenAPI_nf_service_instance_create(
    char *service_instance_id,
    char *nf_instance_id,
    char *nf_service_set_id
)
{
    OpenAPI_nf_service_instance_t *nf_service_instance_local_var = ogs_malloc(sizeof(OpenAPI_nf_service_instance_t));
    ogs_assert(nf_service_instance_local_var);

    nf_service_instance_local_var->service_instance_id = service_instance_id;
    nf_service_instance_local_var->nf_instance_id = nf_instance_id;
    nf_service_instance_local_var->nf_service_set_id = nf_service_set_id;

    return nf_service_instance_local_var;
}

void OpenAPI_nf_service_instance_free(OpenAPI_nf_service_instance_t *nf_service_instance)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nf_service_instance) {
        return;
    }
    if (nf_service_instance->service_instance_id) {
        ogs_free(nf_service_instance->service_instance_id);
        nf_service_instance->service_instance_id = NULL;
    }
    if (nf_service_instance->nf_instance_id) {
        ogs_free(nf_service_instance->nf_instance_id);
        nf_service_instance->nf_instance_id = NULL;
    }
    if (nf_service_instance->nf_service_set_id) {
        ogs_free(nf_service_instance->nf_service_set_id);
        nf_service_instance->nf_service_set_id = NULL;
    }
    ogs_free(nf_service_instance);
}

cJSON *OpenAPI_nf_service_instance_convertToJSON(OpenAPI_nf_service_instance_t *nf_service_instance)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nf_service_instance == NULL) {
        ogs_error("OpenAPI_nf_service_instance_convertToJSON() failed [NfServiceInstance]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nf_service_instance->service_instance_id) {
    if (cJSON_AddStringToObject(item, "serviceInstanceId", nf_service_instance->service_instance_id) == NULL) {
        ogs_error("OpenAPI_nf_service_instance_convertToJSON() failed [service_instance_id]");
        goto end;
    }
    }

    if (nf_service_instance->nf_instance_id) {
    if (cJSON_AddStringToObject(item, "nfInstanceId", nf_service_instance->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_nf_service_instance_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    if (nf_service_instance->nf_service_set_id) {
    if (cJSON_AddStringToObject(item, "nfServiceSetId", nf_service_instance->nf_service_set_id) == NULL) {
        ogs_error("OpenAPI_nf_service_instance_convertToJSON() failed [nf_service_set_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nf_service_instance_t *OpenAPI_nf_service_instance_parseFromJSON(cJSON *nf_service_instanceJSON)
{
    OpenAPI_nf_service_instance_t *nf_service_instance_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *service_instance_id = NULL;
    cJSON *nf_instance_id = NULL;
    cJSON *nf_service_set_id = NULL;
    service_instance_id = cJSON_GetObjectItemCaseSensitive(nf_service_instanceJSON, "serviceInstanceId");
    if (service_instance_id) {
    if (!cJSON_IsString(service_instance_id) && !cJSON_IsNull(service_instance_id)) {
        ogs_error("OpenAPI_nf_service_instance_parseFromJSON() failed [service_instance_id]");
        goto end;
    }
    }

    nf_instance_id = cJSON_GetObjectItemCaseSensitive(nf_service_instanceJSON, "nfInstanceId");
    if (nf_instance_id) {
    if (!cJSON_IsString(nf_instance_id) && !cJSON_IsNull(nf_instance_id)) {
        ogs_error("OpenAPI_nf_service_instance_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    nf_service_set_id = cJSON_GetObjectItemCaseSensitive(nf_service_instanceJSON, "nfServiceSetId");
    if (nf_service_set_id) {
    if (!cJSON_IsString(nf_service_set_id) && !cJSON_IsNull(nf_service_set_id)) {
        ogs_error("OpenAPI_nf_service_instance_parseFromJSON() failed [nf_service_set_id]");
        goto end;
    }
    }

    nf_service_instance_local_var = OpenAPI_nf_service_instance_create (
        service_instance_id && !cJSON_IsNull(service_instance_id) ? ogs_strdup(service_instance_id->valuestring) : NULL,
        nf_instance_id && !cJSON_IsNull(nf_instance_id) ? ogs_strdup(nf_instance_id->valuestring) : NULL,
        nf_service_set_id && !cJSON_IsNull(nf_service_set_id) ? ogs_strdup(nf_service_set_id->valuestring) : NULL
    );

    return nf_service_instance_local_var;
end:
    return NULL;
}

OpenAPI_nf_service_instance_t *OpenAPI_nf_service_instance_copy(OpenAPI_nf_service_instance_t *dst, OpenAPI_nf_service_instance_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_service_instance_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_service_instance_convertToJSON() failed");
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

    OpenAPI_nf_service_instance_free(dst);
    dst = OpenAPI_nf_service_instance_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

