
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "intended_n32_purpose.h"

OpenAPI_intended_n32_purpose_t *OpenAPI_intended_n32_purpose_create(
    OpenAPI_n32_purpose_e usage_purpose,
    char *additional_info,
    char *cause
)
{
    OpenAPI_intended_n32_purpose_t *intended_n32_purpose_local_var = ogs_malloc(sizeof(OpenAPI_intended_n32_purpose_t));
    ogs_assert(intended_n32_purpose_local_var);

    intended_n32_purpose_local_var->usage_purpose = usage_purpose;
    intended_n32_purpose_local_var->additional_info = additional_info;
    intended_n32_purpose_local_var->cause = cause;

    return intended_n32_purpose_local_var;
}

void OpenAPI_intended_n32_purpose_free(OpenAPI_intended_n32_purpose_t *intended_n32_purpose)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == intended_n32_purpose) {
        return;
    }
    if (intended_n32_purpose->additional_info) {
        ogs_free(intended_n32_purpose->additional_info);
        intended_n32_purpose->additional_info = NULL;
    }
    if (intended_n32_purpose->cause) {
        ogs_free(intended_n32_purpose->cause);
        intended_n32_purpose->cause = NULL;
    }
    ogs_free(intended_n32_purpose);
}

cJSON *OpenAPI_intended_n32_purpose_convertToJSON(OpenAPI_intended_n32_purpose_t *intended_n32_purpose)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (intended_n32_purpose == NULL) {
        ogs_error("OpenAPI_intended_n32_purpose_convertToJSON() failed [IntendedN32Purpose]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (intended_n32_purpose->usage_purpose == OpenAPI_n32_purpose_NULL) {
        ogs_error("OpenAPI_intended_n32_purpose_convertToJSON() failed [usage_purpose]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "usagePurpose", OpenAPI_n32_purpose_ToString(intended_n32_purpose->usage_purpose)) == NULL) {
        ogs_error("OpenAPI_intended_n32_purpose_convertToJSON() failed [usage_purpose]");
        goto end;
    }

    if (intended_n32_purpose->additional_info) {
    if (cJSON_AddStringToObject(item, "additionalInfo", intended_n32_purpose->additional_info) == NULL) {
        ogs_error("OpenAPI_intended_n32_purpose_convertToJSON() failed [additional_info]");
        goto end;
    }
    }

    if (intended_n32_purpose->cause) {
    if (cJSON_AddStringToObject(item, "cause", intended_n32_purpose->cause) == NULL) {
        ogs_error("OpenAPI_intended_n32_purpose_convertToJSON() failed [cause]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_intended_n32_purpose_t *OpenAPI_intended_n32_purpose_parseFromJSON(cJSON *intended_n32_purposeJSON)
{
    OpenAPI_intended_n32_purpose_t *intended_n32_purpose_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *usage_purpose = NULL;
    OpenAPI_n32_purpose_e usage_purposeVariable = 0;
    cJSON *additional_info = NULL;
    cJSON *cause = NULL;
    usage_purpose = cJSON_GetObjectItemCaseSensitive(intended_n32_purposeJSON, "usagePurpose");
    if (!usage_purpose) {
        ogs_error("OpenAPI_intended_n32_purpose_parseFromJSON() failed [usage_purpose]");
        goto end;
    }
    if (!cJSON_IsString(usage_purpose)) {
        ogs_error("OpenAPI_intended_n32_purpose_parseFromJSON() failed [usage_purpose]");
        goto end;
    }
    usage_purposeVariable = OpenAPI_n32_purpose_FromString(usage_purpose->valuestring);

    additional_info = cJSON_GetObjectItemCaseSensitive(intended_n32_purposeJSON, "additionalInfo");
    if (additional_info) {
    if (!cJSON_IsString(additional_info) && !cJSON_IsNull(additional_info)) {
        ogs_error("OpenAPI_intended_n32_purpose_parseFromJSON() failed [additional_info]");
        goto end;
    }
    }

    cause = cJSON_GetObjectItemCaseSensitive(intended_n32_purposeJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause) && !cJSON_IsNull(cause)) {
        ogs_error("OpenAPI_intended_n32_purpose_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    intended_n32_purpose_local_var = OpenAPI_intended_n32_purpose_create (
        usage_purposeVariable,
        additional_info && !cJSON_IsNull(additional_info) ? ogs_strdup(additional_info->valuestring) : NULL,
        cause && !cJSON_IsNull(cause) ? ogs_strdup(cause->valuestring) : NULL
    );

    return intended_n32_purpose_local_var;
end:
    return NULL;
}

OpenAPI_intended_n32_purpose_t *OpenAPI_intended_n32_purpose_copy(OpenAPI_intended_n32_purpose_t *dst, OpenAPI_intended_n32_purpose_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_intended_n32_purpose_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_intended_n32_purpose_convertToJSON() failed");
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

    OpenAPI_intended_n32_purpose_free(dst);
    dst = OpenAPI_intended_n32_purpose_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

