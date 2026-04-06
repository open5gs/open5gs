
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "capability_report.h"

OpenAPI_capability_report_t *OpenAPI_capability_report_create(
    OpenAPI_notif_cap_e cap_report,
    OpenAPI_notif_cap_type_e cap_type
)
{
    OpenAPI_capability_report_t *capability_report_local_var = ogs_malloc(sizeof(OpenAPI_capability_report_t));
    ogs_assert(capability_report_local_var);

    capability_report_local_var->cap_report = cap_report;
    capability_report_local_var->cap_type = cap_type;

    return capability_report_local_var;
}

void OpenAPI_capability_report_free(OpenAPI_capability_report_t *capability_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == capability_report) {
        return;
    }
    ogs_free(capability_report);
}

cJSON *OpenAPI_capability_report_convertToJSON(OpenAPI_capability_report_t *capability_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (capability_report == NULL) {
        ogs_error("OpenAPI_capability_report_convertToJSON() failed [CapabilityReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (capability_report->cap_report == OpenAPI_notif_cap_NULL) {
        ogs_error("OpenAPI_capability_report_convertToJSON() failed [cap_report]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "capReport", OpenAPI_notif_cap_ToString(capability_report->cap_report)) == NULL) {
        ogs_error("OpenAPI_capability_report_convertToJSON() failed [cap_report]");
        goto end;
    }

    if (capability_report->cap_type == OpenAPI_notif_cap_type_NULL) {
        ogs_error("OpenAPI_capability_report_convertToJSON() failed [cap_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "capType", OpenAPI_notif_cap_type_ToString(capability_report->cap_type)) == NULL) {
        ogs_error("OpenAPI_capability_report_convertToJSON() failed [cap_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_capability_report_t *OpenAPI_capability_report_parseFromJSON(cJSON *capability_reportJSON)
{
    OpenAPI_capability_report_t *capability_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cap_report = NULL;
    OpenAPI_notif_cap_e cap_reportVariable = 0;
    cJSON *cap_type = NULL;
    OpenAPI_notif_cap_type_e cap_typeVariable = 0;
    cap_report = cJSON_GetObjectItemCaseSensitive(capability_reportJSON, "capReport");
    if (!cap_report) {
        ogs_error("OpenAPI_capability_report_parseFromJSON() failed [cap_report]");
        goto end;
    }
    if (!cJSON_IsString(cap_report)) {
        ogs_error("OpenAPI_capability_report_parseFromJSON() failed [cap_report]");
        goto end;
    }
    cap_reportVariable = OpenAPI_notif_cap_FromString(cap_report->valuestring);

    cap_type = cJSON_GetObjectItemCaseSensitive(capability_reportJSON, "capType");
    if (!cap_type) {
        ogs_error("OpenAPI_capability_report_parseFromJSON() failed [cap_type]");
        goto end;
    }
    if (!cJSON_IsString(cap_type)) {
        ogs_error("OpenAPI_capability_report_parseFromJSON() failed [cap_type]");
        goto end;
    }
    cap_typeVariable = OpenAPI_notif_cap_type_FromString(cap_type->valuestring);

    capability_report_local_var = OpenAPI_capability_report_create (
        cap_reportVariable,
        cap_typeVariable
    );

    return capability_report_local_var;
end:
    return NULL;
}

OpenAPI_capability_report_t *OpenAPI_capability_report_copy(OpenAPI_capability_report_t *dst, OpenAPI_capability_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_capability_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_capability_report_convertToJSON() failed");
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

    OpenAPI_capability_report_free(dst);
    dst = OpenAPI_capability_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

