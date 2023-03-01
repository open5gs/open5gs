
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_dereg_info.h"

OpenAPI_amf_dereg_info_t *OpenAPI_amf_dereg_info_create(
    OpenAPI_deregistration_reason_e dereg_reason
)
{
    OpenAPI_amf_dereg_info_t *amf_dereg_info_local_var = ogs_malloc(sizeof(OpenAPI_amf_dereg_info_t));
    ogs_assert(amf_dereg_info_local_var);

    amf_dereg_info_local_var->dereg_reason = dereg_reason;

    return amf_dereg_info_local_var;
}

void OpenAPI_amf_dereg_info_free(OpenAPI_amf_dereg_info_t *amf_dereg_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_dereg_info) {
        return;
    }
    ogs_free(amf_dereg_info);
}

cJSON *OpenAPI_amf_dereg_info_convertToJSON(OpenAPI_amf_dereg_info_t *amf_dereg_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_dereg_info == NULL) {
        ogs_error("OpenAPI_amf_dereg_info_convertToJSON() failed [AmfDeregInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (amf_dereg_info->dereg_reason == OpenAPI_deregistration_reason_NULL) {
        ogs_error("OpenAPI_amf_dereg_info_convertToJSON() failed [dereg_reason]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "deregReason", OpenAPI_deregistration_reason_ToString(amf_dereg_info->dereg_reason)) == NULL) {
        ogs_error("OpenAPI_amf_dereg_info_convertToJSON() failed [dereg_reason]");
        goto end;
    }

end:
    return item;
}

OpenAPI_amf_dereg_info_t *OpenAPI_amf_dereg_info_parseFromJSON(cJSON *amf_dereg_infoJSON)
{
    OpenAPI_amf_dereg_info_t *amf_dereg_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dereg_reason = NULL;
    OpenAPI_deregistration_reason_e dereg_reasonVariable = 0;
    dereg_reason = cJSON_GetObjectItemCaseSensitive(amf_dereg_infoJSON, "deregReason");
    if (!dereg_reason) {
        ogs_error("OpenAPI_amf_dereg_info_parseFromJSON() failed [dereg_reason]");
        goto end;
    }
    if (!cJSON_IsString(dereg_reason)) {
        ogs_error("OpenAPI_amf_dereg_info_parseFromJSON() failed [dereg_reason]");
        goto end;
    }
    dereg_reasonVariable = OpenAPI_deregistration_reason_FromString(dereg_reason->valuestring);

    amf_dereg_info_local_var = OpenAPI_amf_dereg_info_create (
        dereg_reasonVariable
    );

    return amf_dereg_info_local_var;
end:
    return NULL;
}

OpenAPI_amf_dereg_info_t *OpenAPI_amf_dereg_info_copy(OpenAPI_amf_dereg_info_t *dst, OpenAPI_amf_dereg_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_dereg_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_dereg_info_convertToJSON() failed");
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

    OpenAPI_amf_dereg_info_free(dst);
    dst = OpenAPI_amf_dereg_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

