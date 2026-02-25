
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "periodic_event_info.h"

OpenAPI_periodic_event_info_t *OpenAPI_periodic_event_info_create(
    int reporting_amount,
    int reporting_interval
)
{
    OpenAPI_periodic_event_info_t *periodic_event_info_local_var = ogs_malloc(sizeof(OpenAPI_periodic_event_info_t));
    ogs_assert(periodic_event_info_local_var);

    periodic_event_info_local_var->reporting_amount = reporting_amount;
    periodic_event_info_local_var->reporting_interval = reporting_interval;

    return periodic_event_info_local_var;
}

void OpenAPI_periodic_event_info_free(OpenAPI_periodic_event_info_t *periodic_event_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == periodic_event_info) {
        return;
    }
    ogs_free(periodic_event_info);
}

cJSON *OpenAPI_periodic_event_info_convertToJSON(OpenAPI_periodic_event_info_t *periodic_event_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (periodic_event_info == NULL) {
        ogs_error("OpenAPI_periodic_event_info_convertToJSON() failed [PeriodicEventInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "reportingAmount", periodic_event_info->reporting_amount) == NULL) {
        ogs_error("OpenAPI_periodic_event_info_convertToJSON() failed [reporting_amount]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "reportingInterval", periodic_event_info->reporting_interval) == NULL) {
        ogs_error("OpenAPI_periodic_event_info_convertToJSON() failed [reporting_interval]");
        goto end;
    }

end:
    return item;
}

OpenAPI_periodic_event_info_t *OpenAPI_periodic_event_info_parseFromJSON(cJSON *periodic_event_infoJSON)
{
    OpenAPI_periodic_event_info_t *periodic_event_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *reporting_amount = NULL;
    cJSON *reporting_interval = NULL;
    reporting_amount = cJSON_GetObjectItemCaseSensitive(periodic_event_infoJSON, "reportingAmount");
    if (!reporting_amount) {
        ogs_error("OpenAPI_periodic_event_info_parseFromJSON() failed [reporting_amount]");
        goto end;
    }
    if (!cJSON_IsNumber(reporting_amount)) {
        ogs_error("OpenAPI_periodic_event_info_parseFromJSON() failed [reporting_amount]");
        goto end;
    }

    reporting_interval = cJSON_GetObjectItemCaseSensitive(periodic_event_infoJSON, "reportingInterval");
    if (!reporting_interval) {
        ogs_error("OpenAPI_periodic_event_info_parseFromJSON() failed [reporting_interval]");
        goto end;
    }
    if (!cJSON_IsNumber(reporting_interval)) {
        ogs_error("OpenAPI_periodic_event_info_parseFromJSON() failed [reporting_interval]");
        goto end;
    }

    periodic_event_info_local_var = OpenAPI_periodic_event_info_create (
        
        reporting_amount->valuedouble,
        
        reporting_interval->valuedouble
    );

    return periodic_event_info_local_var;
end:
    return NULL;
}

OpenAPI_periodic_event_info_t *OpenAPI_periodic_event_info_copy(OpenAPI_periodic_event_info_t *dst, OpenAPI_periodic_event_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_periodic_event_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_periodic_event_info_convertToJSON() failed");
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

    OpenAPI_periodic_event_info_free(dst);
    dst = OpenAPI_periodic_event_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

