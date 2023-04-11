
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_period.h"

OpenAPI_time_period_t *OpenAPI_time_period_create(
    OpenAPI_periodicity_t *period,
    bool is_max_num_period,
    int max_num_period
)
{
    OpenAPI_time_period_t *time_period_local_var = ogs_malloc(sizeof(OpenAPI_time_period_t));
    ogs_assert(time_period_local_var);

    time_period_local_var->period = period;
    time_period_local_var->is_max_num_period = is_max_num_period;
    time_period_local_var->max_num_period = max_num_period;

    return time_period_local_var;
}

void OpenAPI_time_period_free(OpenAPI_time_period_t *time_period)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == time_period) {
        return;
    }
    if (time_period->period) {
        OpenAPI_periodicity_free(time_period->period);
        time_period->period = NULL;
    }
    ogs_free(time_period);
}

cJSON *OpenAPI_time_period_convertToJSON(OpenAPI_time_period_t *time_period)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (time_period == NULL) {
        ogs_error("OpenAPI_time_period_convertToJSON() failed [TimePeriod]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!time_period->period) {
        ogs_error("OpenAPI_time_period_convertToJSON() failed [period]");
        return NULL;
    }
    cJSON *period_local_JSON = OpenAPI_periodicity_convertToJSON(time_period->period);
    if (period_local_JSON == NULL) {
        ogs_error("OpenAPI_time_period_convertToJSON() failed [period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "period", period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_time_period_convertToJSON() failed [period]");
        goto end;
    }

    if (time_period->is_max_num_period) {
    if (cJSON_AddNumberToObject(item, "maxNumPeriod", time_period->max_num_period) == NULL) {
        ogs_error("OpenAPI_time_period_convertToJSON() failed [max_num_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_time_period_t *OpenAPI_time_period_parseFromJSON(cJSON *time_periodJSON)
{
    OpenAPI_time_period_t *time_period_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *period = NULL;
    OpenAPI_periodicity_t *period_local_nonprim = NULL;
    cJSON *max_num_period = NULL;
    period = cJSON_GetObjectItemCaseSensitive(time_periodJSON, "period");
    if (!period) {
        ogs_error("OpenAPI_time_period_parseFromJSON() failed [period]");
        goto end;
    }
    period_local_nonprim = OpenAPI_periodicity_parseFromJSON(period);
    if (!period_local_nonprim) {
        ogs_error("OpenAPI_periodicity_parseFromJSON failed [period]");
        goto end;
    }

    max_num_period = cJSON_GetObjectItemCaseSensitive(time_periodJSON, "maxNumPeriod");
    if (max_num_period) {
    if (!cJSON_IsNumber(max_num_period)) {
        ogs_error("OpenAPI_time_period_parseFromJSON() failed [max_num_period]");
        goto end;
    }
    }

    time_period_local_var = OpenAPI_time_period_create (
        period_local_nonprim,
        max_num_period ? true : false,
        max_num_period ? max_num_period->valuedouble : 0
    );

    return time_period_local_var;
end:
    if (period_local_nonprim) {
        OpenAPI_periodicity_free(period_local_nonprim);
        period_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_time_period_t *OpenAPI_time_period_copy(OpenAPI_time_period_t *dst, OpenAPI_time_period_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_time_period_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_time_period_convertToJSON() failed");
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

    OpenAPI_time_period_free(dst);
    dst = OpenAPI_time_period_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

