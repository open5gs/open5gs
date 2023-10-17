
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nnwdaf_nf_status.h"

OpenAPI_nnwdaf_nf_status_t *OpenAPI_nnwdaf_nf_status_create(
    bool is_status_registered,
    int status_registered,
    bool is_status_unregistered,
    int status_unregistered,
    bool is_status_undiscoverable,
    int status_undiscoverable
)
{
    OpenAPI_nnwdaf_nf_status_t *nnwdaf_nf_status_local_var = ogs_malloc(sizeof(OpenAPI_nnwdaf_nf_status_t));
    ogs_assert(nnwdaf_nf_status_local_var);

    nnwdaf_nf_status_local_var->is_status_registered = is_status_registered;
    nnwdaf_nf_status_local_var->status_registered = status_registered;
    nnwdaf_nf_status_local_var->is_status_unregistered = is_status_unregistered;
    nnwdaf_nf_status_local_var->status_unregistered = status_unregistered;
    nnwdaf_nf_status_local_var->is_status_undiscoverable = is_status_undiscoverable;
    nnwdaf_nf_status_local_var->status_undiscoverable = status_undiscoverable;

    return nnwdaf_nf_status_local_var;
}

void OpenAPI_nnwdaf_nf_status_free(OpenAPI_nnwdaf_nf_status_t *nnwdaf_nf_status)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nnwdaf_nf_status) {
        return;
    }
    ogs_free(nnwdaf_nf_status);
}

cJSON *OpenAPI_nnwdaf_nf_status_convertToJSON(OpenAPI_nnwdaf_nf_status_t *nnwdaf_nf_status)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nnwdaf_nf_status == NULL) {
        ogs_error("OpenAPI_nnwdaf_nf_status_convertToJSON() failed [NnwdafNfStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nnwdaf_nf_status->is_status_registered) {
    if (cJSON_AddNumberToObject(item, "statusRegistered", nnwdaf_nf_status->status_registered) == NULL) {
        ogs_error("OpenAPI_nnwdaf_nf_status_convertToJSON() failed [status_registered]");
        goto end;
    }
    }

    if (nnwdaf_nf_status->is_status_unregistered) {
    if (cJSON_AddNumberToObject(item, "statusUnregistered", nnwdaf_nf_status->status_unregistered) == NULL) {
        ogs_error("OpenAPI_nnwdaf_nf_status_convertToJSON() failed [status_unregistered]");
        goto end;
    }
    }

    if (nnwdaf_nf_status->is_status_undiscoverable) {
    if (cJSON_AddNumberToObject(item, "statusUndiscoverable", nnwdaf_nf_status->status_undiscoverable) == NULL) {
        ogs_error("OpenAPI_nnwdaf_nf_status_convertToJSON() failed [status_undiscoverable]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nnwdaf_nf_status_t *OpenAPI_nnwdaf_nf_status_parseFromJSON(cJSON *nnwdaf_nf_statusJSON)
{
    OpenAPI_nnwdaf_nf_status_t *nnwdaf_nf_status_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *status_registered = NULL;
    cJSON *status_unregistered = NULL;
    cJSON *status_undiscoverable = NULL;
    status_registered = cJSON_GetObjectItemCaseSensitive(nnwdaf_nf_statusJSON, "statusRegistered");
    if (status_registered) {
    if (!cJSON_IsNumber(status_registered)) {
        ogs_error("OpenAPI_nnwdaf_nf_status_parseFromJSON() failed [status_registered]");
        goto end;
    }
    }

    status_unregistered = cJSON_GetObjectItemCaseSensitive(nnwdaf_nf_statusJSON, "statusUnregistered");
    if (status_unregistered) {
    if (!cJSON_IsNumber(status_unregistered)) {
        ogs_error("OpenAPI_nnwdaf_nf_status_parseFromJSON() failed [status_unregistered]");
        goto end;
    }
    }

    status_undiscoverable = cJSON_GetObjectItemCaseSensitive(nnwdaf_nf_statusJSON, "statusUndiscoverable");
    if (status_undiscoverable) {
    if (!cJSON_IsNumber(status_undiscoverable)) {
        ogs_error("OpenAPI_nnwdaf_nf_status_parseFromJSON() failed [status_undiscoverable]");
        goto end;
    }
    }

    nnwdaf_nf_status_local_var = OpenAPI_nnwdaf_nf_status_create (
        status_registered ? true : false,
        status_registered ? status_registered->valuedouble : 0,
        status_unregistered ? true : false,
        status_unregistered ? status_unregistered->valuedouble : 0,
        status_undiscoverable ? true : false,
        status_undiscoverable ? status_undiscoverable->valuedouble : 0
    );

    return nnwdaf_nf_status_local_var;
end:
    return NULL;
}

OpenAPI_nnwdaf_nf_status_t *OpenAPI_nnwdaf_nf_status_copy(OpenAPI_nnwdaf_nf_status_t *dst, OpenAPI_nnwdaf_nf_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nnwdaf_nf_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nnwdaf_nf_status_convertToJSON() failed");
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

    OpenAPI_nnwdaf_nf_status_free(dst);
    dst = OpenAPI_nnwdaf_nf_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

