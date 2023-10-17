
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "number_average.h"

OpenAPI_number_average_t *OpenAPI_number_average_create(
    float number,
    float variance,
    bool is_skewness,
    float skewness
)
{
    OpenAPI_number_average_t *number_average_local_var = ogs_malloc(sizeof(OpenAPI_number_average_t));
    ogs_assert(number_average_local_var);

    number_average_local_var->number = number;
    number_average_local_var->variance = variance;
    number_average_local_var->is_skewness = is_skewness;
    number_average_local_var->skewness = skewness;

    return number_average_local_var;
}

void OpenAPI_number_average_free(OpenAPI_number_average_t *number_average)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == number_average) {
        return;
    }
    ogs_free(number_average);
}

cJSON *OpenAPI_number_average_convertToJSON(OpenAPI_number_average_t *number_average)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (number_average == NULL) {
        ogs_error("OpenAPI_number_average_convertToJSON() failed [NumberAverage]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "number", number_average->number) == NULL) {
        ogs_error("OpenAPI_number_average_convertToJSON() failed [number]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "variance", number_average->variance) == NULL) {
        ogs_error("OpenAPI_number_average_convertToJSON() failed [variance]");
        goto end;
    }

    if (number_average->is_skewness) {
    if (cJSON_AddNumberToObject(item, "skewness", number_average->skewness) == NULL) {
        ogs_error("OpenAPI_number_average_convertToJSON() failed [skewness]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_number_average_t *OpenAPI_number_average_parseFromJSON(cJSON *number_averageJSON)
{
    OpenAPI_number_average_t *number_average_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *number = NULL;
    cJSON *variance = NULL;
    cJSON *skewness = NULL;
    number = cJSON_GetObjectItemCaseSensitive(number_averageJSON, "number");
    if (!number) {
        ogs_error("OpenAPI_number_average_parseFromJSON() failed [number]");
        goto end;
    }
    if (!cJSON_IsNumber(number)) {
        ogs_error("OpenAPI_number_average_parseFromJSON() failed [number]");
        goto end;
    }

    variance = cJSON_GetObjectItemCaseSensitive(number_averageJSON, "variance");
    if (!variance) {
        ogs_error("OpenAPI_number_average_parseFromJSON() failed [variance]");
        goto end;
    }
    if (!cJSON_IsNumber(variance)) {
        ogs_error("OpenAPI_number_average_parseFromJSON() failed [variance]");
        goto end;
    }

    skewness = cJSON_GetObjectItemCaseSensitive(number_averageJSON, "skewness");
    if (skewness) {
    if (!cJSON_IsNumber(skewness)) {
        ogs_error("OpenAPI_number_average_parseFromJSON() failed [skewness]");
        goto end;
    }
    }

    number_average_local_var = OpenAPI_number_average_create (
        
        number->valuedouble,
        
        variance->valuedouble,
        skewness ? true : false,
        skewness ? skewness->valuedouble : 0
    );

    return number_average_local_var;
end:
    return NULL;
}

OpenAPI_number_average_t *OpenAPI_number_average_copy(OpenAPI_number_average_t *dst, OpenAPI_number_average_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_number_average_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_number_average_convertToJSON() failed");
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

    OpenAPI_number_average_free(dst);
    dst = OpenAPI_number_average_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

