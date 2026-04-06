
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_suggestion_information_1.h"

OpenAPI_reporting_suggestion_information_1_t *OpenAPI_reporting_suggestion_information_1_create(
    OpenAPI_reporting_urgency_e reporting_urgency,
    bool is_reporting_time_info,
    int reporting_time_info
)
{
    OpenAPI_reporting_suggestion_information_1_t *reporting_suggestion_information_1_local_var = ogs_malloc(sizeof(OpenAPI_reporting_suggestion_information_1_t));
    ogs_assert(reporting_suggestion_information_1_local_var);

    reporting_suggestion_information_1_local_var->reporting_urgency = reporting_urgency;
    reporting_suggestion_information_1_local_var->is_reporting_time_info = is_reporting_time_info;
    reporting_suggestion_information_1_local_var->reporting_time_info = reporting_time_info;

    return reporting_suggestion_information_1_local_var;
}

void OpenAPI_reporting_suggestion_information_1_free(OpenAPI_reporting_suggestion_information_1_t *reporting_suggestion_information_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reporting_suggestion_information_1) {
        return;
    }
    ogs_free(reporting_suggestion_information_1);
}

cJSON *OpenAPI_reporting_suggestion_information_1_convertToJSON(OpenAPI_reporting_suggestion_information_1_t *reporting_suggestion_information_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reporting_suggestion_information_1 == NULL) {
        ogs_error("OpenAPI_reporting_suggestion_information_1_convertToJSON() failed [ReportingSuggestionInformation_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (reporting_suggestion_information_1->reporting_urgency == OpenAPI_reporting_urgency_NULL) {
        ogs_error("OpenAPI_reporting_suggestion_information_1_convertToJSON() failed [reporting_urgency]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "reportingUrgency", OpenAPI_reporting_urgency_ToString(reporting_suggestion_information_1->reporting_urgency)) == NULL) {
        ogs_error("OpenAPI_reporting_suggestion_information_1_convertToJSON() failed [reporting_urgency]");
        goto end;
    }

    if (reporting_suggestion_information_1->is_reporting_time_info) {
    if (cJSON_AddNumberToObject(item, "reportingTimeInfo", reporting_suggestion_information_1->reporting_time_info) == NULL) {
        ogs_error("OpenAPI_reporting_suggestion_information_1_convertToJSON() failed [reporting_time_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_reporting_suggestion_information_1_t *OpenAPI_reporting_suggestion_information_1_parseFromJSON(cJSON *reporting_suggestion_information_1JSON)
{
    OpenAPI_reporting_suggestion_information_1_t *reporting_suggestion_information_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *reporting_urgency = NULL;
    OpenAPI_reporting_urgency_e reporting_urgencyVariable = 0;
    cJSON *reporting_time_info = NULL;
    reporting_urgency = cJSON_GetObjectItemCaseSensitive(reporting_suggestion_information_1JSON, "reportingUrgency");
    if (!reporting_urgency) {
        ogs_error("OpenAPI_reporting_suggestion_information_1_parseFromJSON() failed [reporting_urgency]");
        goto end;
    }
    if (!cJSON_IsString(reporting_urgency)) {
        ogs_error("OpenAPI_reporting_suggestion_information_1_parseFromJSON() failed [reporting_urgency]");
        goto end;
    }
    reporting_urgencyVariable = OpenAPI_reporting_urgency_FromString(reporting_urgency->valuestring);

    reporting_time_info = cJSON_GetObjectItemCaseSensitive(reporting_suggestion_information_1JSON, "reportingTimeInfo");
    if (reporting_time_info) {
    if (!cJSON_IsNumber(reporting_time_info)) {
        ogs_error("OpenAPI_reporting_suggestion_information_1_parseFromJSON() failed [reporting_time_info]");
        goto end;
    }
    }

    reporting_suggestion_information_1_local_var = OpenAPI_reporting_suggestion_information_1_create (
        reporting_urgencyVariable,
        reporting_time_info ? true : false,
        reporting_time_info ? reporting_time_info->valuedouble : 0
    );

    return reporting_suggestion_information_1_local_var;
end:
    return NULL;
}

OpenAPI_reporting_suggestion_information_1_t *OpenAPI_reporting_suggestion_information_1_copy(OpenAPI_reporting_suggestion_information_1_t *dst, OpenAPI_reporting_suggestion_information_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reporting_suggestion_information_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reporting_suggestion_information_1_convertToJSON() failed");
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

    OpenAPI_reporting_suggestion_information_1_free(dst);
    dst = OpenAPI_reporting_suggestion_information_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

