
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exception.h"

OpenAPI_exception_t *OpenAPI_exception_create(
    OpenAPI_exception_id_t *excep_id,
    bool is_excep_level,
    int excep_level,
    OpenAPI_exception_trend_t *excep_trend
)
{
    OpenAPI_exception_t *exception_local_var = ogs_malloc(sizeof(OpenAPI_exception_t));
    ogs_assert(exception_local_var);

    exception_local_var->excep_id = excep_id;
    exception_local_var->is_excep_level = is_excep_level;
    exception_local_var->excep_level = excep_level;
    exception_local_var->excep_trend = excep_trend;

    return exception_local_var;
}

void OpenAPI_exception_free(OpenAPI_exception_t *exception)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == exception) {
        return;
    }
    if (exception->excep_id) {
        OpenAPI_exception_id_free(exception->excep_id);
        exception->excep_id = NULL;
    }
    if (exception->excep_trend) {
        OpenAPI_exception_trend_free(exception->excep_trend);
        exception->excep_trend = NULL;
    }
    ogs_free(exception);
}

cJSON *OpenAPI_exception_convertToJSON(OpenAPI_exception_t *exception)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (exception == NULL) {
        ogs_error("OpenAPI_exception_convertToJSON() failed [Exception]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!exception->excep_id) {
        ogs_error("OpenAPI_exception_convertToJSON() failed [excep_id]");
        return NULL;
    }
    cJSON *excep_id_local_JSON = OpenAPI_exception_id_convertToJSON(exception->excep_id);
    if (excep_id_local_JSON == NULL) {
        ogs_error("OpenAPI_exception_convertToJSON() failed [excep_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "excepId", excep_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_exception_convertToJSON() failed [excep_id]");
        goto end;
    }

    if (exception->is_excep_level) {
    if (cJSON_AddNumberToObject(item, "excepLevel", exception->excep_level) == NULL) {
        ogs_error("OpenAPI_exception_convertToJSON() failed [excep_level]");
        goto end;
    }
    }

    if (exception->excep_trend) {
    cJSON *excep_trend_local_JSON = OpenAPI_exception_trend_convertToJSON(exception->excep_trend);
    if (excep_trend_local_JSON == NULL) {
        ogs_error("OpenAPI_exception_convertToJSON() failed [excep_trend]");
        goto end;
    }
    cJSON_AddItemToObject(item, "excepTrend", excep_trend_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_exception_convertToJSON() failed [excep_trend]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_exception_t *OpenAPI_exception_parseFromJSON(cJSON *exceptionJSON)
{
    OpenAPI_exception_t *exception_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *excep_id = NULL;
    OpenAPI_exception_id_t *excep_id_local_nonprim = NULL;
    cJSON *excep_level = NULL;
    cJSON *excep_trend = NULL;
    OpenAPI_exception_trend_t *excep_trend_local_nonprim = NULL;
    excep_id = cJSON_GetObjectItemCaseSensitive(exceptionJSON, "excepId");
    if (!excep_id) {
        ogs_error("OpenAPI_exception_parseFromJSON() failed [excep_id]");
        goto end;
    }
    excep_id_local_nonprim = OpenAPI_exception_id_parseFromJSON(excep_id);
    if (!excep_id_local_nonprim) {
        ogs_error("OpenAPI_exception_id_parseFromJSON failed [excep_id]");
        goto end;
    }

    excep_level = cJSON_GetObjectItemCaseSensitive(exceptionJSON, "excepLevel");
    if (excep_level) {
    if (!cJSON_IsNumber(excep_level)) {
        ogs_error("OpenAPI_exception_parseFromJSON() failed [excep_level]");
        goto end;
    }
    }

    excep_trend = cJSON_GetObjectItemCaseSensitive(exceptionJSON, "excepTrend");
    if (excep_trend) {
    excep_trend_local_nonprim = OpenAPI_exception_trend_parseFromJSON(excep_trend);
    if (!excep_trend_local_nonprim) {
        ogs_error("OpenAPI_exception_trend_parseFromJSON failed [excep_trend]");
        goto end;
    }
    }

    exception_local_var = OpenAPI_exception_create (
        excep_id_local_nonprim,
        excep_level ? true : false,
        excep_level ? excep_level->valuedouble : 0,
        excep_trend ? excep_trend_local_nonprim : NULL
    );

    return exception_local_var;
end:
    if (excep_id_local_nonprim) {
        OpenAPI_exception_id_free(excep_id_local_nonprim);
        excep_id_local_nonprim = NULL;
    }
    if (excep_trend_local_nonprim) {
        OpenAPI_exception_trend_free(excep_trend_local_nonprim);
        excep_trend_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_exception_t *OpenAPI_exception_copy(OpenAPI_exception_t *dst, OpenAPI_exception_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_exception_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_exception_convertToJSON() failed");
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

    OpenAPI_exception_free(dst);
    dst = OpenAPI_exception_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

