
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_up_context.h"

OpenAPI_lcs_up_context_t *OpenAPI_lcs_up_context_create(
    OpenAPI_up_connection_status_e up_connection_status,
    char *serving_lmf_identification
)
{
    OpenAPI_lcs_up_context_t *lcs_up_context_local_var = ogs_malloc(sizeof(OpenAPI_lcs_up_context_t));
    ogs_assert(lcs_up_context_local_var);

    lcs_up_context_local_var->up_connection_status = up_connection_status;
    lcs_up_context_local_var->serving_lmf_identification = serving_lmf_identification;

    return lcs_up_context_local_var;
}

void OpenAPI_lcs_up_context_free(OpenAPI_lcs_up_context_t *lcs_up_context)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_up_context) {
        return;
    }
    if (lcs_up_context->serving_lmf_identification) {
        ogs_free(lcs_up_context->serving_lmf_identification);
        lcs_up_context->serving_lmf_identification = NULL;
    }
    ogs_free(lcs_up_context);
}

cJSON *OpenAPI_lcs_up_context_convertToJSON(OpenAPI_lcs_up_context_t *lcs_up_context)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_up_context == NULL) {
        ogs_error("OpenAPI_lcs_up_context_convertToJSON() failed [LcsUpContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lcs_up_context->up_connection_status != OpenAPI_up_connection_status_NULL) {
    if (cJSON_AddStringToObject(item, "upConnectionStatus", OpenAPI_up_connection_status_ToString(lcs_up_context->up_connection_status)) == NULL) {
        ogs_error("OpenAPI_lcs_up_context_convertToJSON() failed [up_connection_status]");
        goto end;
    }
    }

    if (lcs_up_context->serving_lmf_identification) {
    if (cJSON_AddStringToObject(item, "servingLMFIdentification", lcs_up_context->serving_lmf_identification) == NULL) {
        ogs_error("OpenAPI_lcs_up_context_convertToJSON() failed [serving_lmf_identification]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lcs_up_context_t *OpenAPI_lcs_up_context_parseFromJSON(cJSON *lcs_up_contextJSON)
{
    OpenAPI_lcs_up_context_t *lcs_up_context_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *up_connection_status = NULL;
    OpenAPI_up_connection_status_e up_connection_statusVariable = 0;
    cJSON *serving_lmf_identification = NULL;
    up_connection_status = cJSON_GetObjectItemCaseSensitive(lcs_up_contextJSON, "upConnectionStatus");
    if (up_connection_status) {
    if (!cJSON_IsString(up_connection_status)) {
        ogs_error("OpenAPI_lcs_up_context_parseFromJSON() failed [up_connection_status]");
        goto end;
    }
    up_connection_statusVariable = OpenAPI_up_connection_status_FromString(up_connection_status->valuestring);
    }

    serving_lmf_identification = cJSON_GetObjectItemCaseSensitive(lcs_up_contextJSON, "servingLMFIdentification");
    if (serving_lmf_identification) {
    if (!cJSON_IsString(serving_lmf_identification) && !cJSON_IsNull(serving_lmf_identification)) {
        ogs_error("OpenAPI_lcs_up_context_parseFromJSON() failed [serving_lmf_identification]");
        goto end;
    }
    }

    lcs_up_context_local_var = OpenAPI_lcs_up_context_create (
        up_connection_status ? up_connection_statusVariable : 0,
        serving_lmf_identification && !cJSON_IsNull(serving_lmf_identification) ? ogs_strdup(serving_lmf_identification->valuestring) : NULL
    );

    return lcs_up_context_local_var;
end:
    return NULL;
}

OpenAPI_lcs_up_context_t *OpenAPI_lcs_up_context_copy(OpenAPI_lcs_up_context_t *dst, OpenAPI_lcs_up_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_up_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_up_context_convertToJSON() failed");
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

    OpenAPI_lcs_up_context_free(dst);
    dst = OpenAPI_lcs_up_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

