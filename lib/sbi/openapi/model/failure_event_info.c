
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "failure_event_info.h"

OpenAPI_failure_event_info_t *OpenAPI_failure_event_info_create(
    OpenAPI_nwdaf_event_e event,
    OpenAPI_nwdaf_failure_code_e failure_code
)
{
    OpenAPI_failure_event_info_t *failure_event_info_local_var = ogs_malloc(sizeof(OpenAPI_failure_event_info_t));
    ogs_assert(failure_event_info_local_var);

    failure_event_info_local_var->event = event;
    failure_event_info_local_var->failure_code = failure_code;

    return failure_event_info_local_var;
}

void OpenAPI_failure_event_info_free(OpenAPI_failure_event_info_t *failure_event_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == failure_event_info) {
        return;
    }
    ogs_free(failure_event_info);
}

cJSON *OpenAPI_failure_event_info_convertToJSON(OpenAPI_failure_event_info_t *failure_event_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (failure_event_info == NULL) {
        ogs_error("OpenAPI_failure_event_info_convertToJSON() failed [FailureEventInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (failure_event_info->event == OpenAPI_nwdaf_event_NULL) {
        ogs_error("OpenAPI_failure_event_info_convertToJSON() failed [event]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "event", OpenAPI_nwdaf_event_ToString(failure_event_info->event)) == NULL) {
        ogs_error("OpenAPI_failure_event_info_convertToJSON() failed [event]");
        goto end;
    }

    if (failure_event_info->failure_code == OpenAPI_nwdaf_failure_code_NULL) {
        ogs_error("OpenAPI_failure_event_info_convertToJSON() failed [failure_code]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "failureCode", OpenAPI_nwdaf_failure_code_ToString(failure_event_info->failure_code)) == NULL) {
        ogs_error("OpenAPI_failure_event_info_convertToJSON() failed [failure_code]");
        goto end;
    }

end:
    return item;
}

OpenAPI_failure_event_info_t *OpenAPI_failure_event_info_parseFromJSON(cJSON *failure_event_infoJSON)
{
    OpenAPI_failure_event_info_t *failure_event_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event = NULL;
    OpenAPI_nwdaf_event_e eventVariable = 0;
    cJSON *failure_code = NULL;
    OpenAPI_nwdaf_failure_code_e failure_codeVariable = 0;
    event = cJSON_GetObjectItemCaseSensitive(failure_event_infoJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_failure_event_info_parseFromJSON() failed [event]");
        goto end;
    }
    if (!cJSON_IsString(event)) {
        ogs_error("OpenAPI_failure_event_info_parseFromJSON() failed [event]");
        goto end;
    }
    eventVariable = OpenAPI_nwdaf_event_FromString(event->valuestring);

    failure_code = cJSON_GetObjectItemCaseSensitive(failure_event_infoJSON, "failureCode");
    if (!failure_code) {
        ogs_error("OpenAPI_failure_event_info_parseFromJSON() failed [failure_code]");
        goto end;
    }
    if (!cJSON_IsString(failure_code)) {
        ogs_error("OpenAPI_failure_event_info_parseFromJSON() failed [failure_code]");
        goto end;
    }
    failure_codeVariable = OpenAPI_nwdaf_failure_code_FromString(failure_code->valuestring);

    failure_event_info_local_var = OpenAPI_failure_event_info_create (
        eventVariable,
        failure_codeVariable
    );

    return failure_event_info_local_var;
end:
    return NULL;
}

OpenAPI_failure_event_info_t *OpenAPI_failure_event_info_copy(OpenAPI_failure_event_info_t *dst, OpenAPI_failure_event_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_failure_event_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_failure_event_info_convertToJSON() failed");
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

    OpenAPI_failure_event_info_free(dst);
    dst = OpenAPI_failure_event_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

