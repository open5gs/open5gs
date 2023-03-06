
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exception_trend.h"

OpenAPI_exception_trend_t *OpenAPI_exception_trend_create(
)
{
    OpenAPI_exception_trend_t *exception_trend_local_var = ogs_malloc(sizeof(OpenAPI_exception_trend_t));
    ogs_assert(exception_trend_local_var);


    return exception_trend_local_var;
}

void OpenAPI_exception_trend_free(OpenAPI_exception_trend_t *exception_trend)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == exception_trend) {
        return;
    }
    ogs_free(exception_trend);
}

cJSON *OpenAPI_exception_trend_convertToJSON(OpenAPI_exception_trend_t *exception_trend)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (exception_trend == NULL) {
        ogs_error("OpenAPI_exception_trend_convertToJSON() failed [ExceptionTrend]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_exception_trend_t *OpenAPI_exception_trend_parseFromJSON(cJSON *exception_trendJSON)
{
    OpenAPI_exception_trend_t *exception_trend_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    exception_trend_local_var = OpenAPI_exception_trend_create (
    );

    return exception_trend_local_var;
end:
    return NULL;
}

OpenAPI_exception_trend_t *OpenAPI_exception_trend_copy(OpenAPI_exception_trend_t *dst, OpenAPI_exception_trend_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_exception_trend_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_exception_trend_convertToJSON() failed");
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

    OpenAPI_exception_trend_free(dst);
    dst = OpenAPI_exception_trend_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

