
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n6_jitter_information.h"

OpenAPI_n6_jitter_information_t *OpenAPI_n6_jitter_information_create(
    bool is_lower_jitter_info,
    int lower_jitter_info,
    bool is_higher_jitter_info,
    int higher_jitter_info
)
{
    OpenAPI_n6_jitter_information_t *n6_jitter_information_local_var = ogs_malloc(sizeof(OpenAPI_n6_jitter_information_t));
    ogs_assert(n6_jitter_information_local_var);

    n6_jitter_information_local_var->is_lower_jitter_info = is_lower_jitter_info;
    n6_jitter_information_local_var->lower_jitter_info = lower_jitter_info;
    n6_jitter_information_local_var->is_higher_jitter_info = is_higher_jitter_info;
    n6_jitter_information_local_var->higher_jitter_info = higher_jitter_info;

    return n6_jitter_information_local_var;
}

void OpenAPI_n6_jitter_information_free(OpenAPI_n6_jitter_information_t *n6_jitter_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n6_jitter_information) {
        return;
    }
    ogs_free(n6_jitter_information);
}

cJSON *OpenAPI_n6_jitter_information_convertToJSON(OpenAPI_n6_jitter_information_t *n6_jitter_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n6_jitter_information == NULL) {
        ogs_error("OpenAPI_n6_jitter_information_convertToJSON() failed [N6JitterInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n6_jitter_information->is_lower_jitter_info) {
    if (cJSON_AddNumberToObject(item, "lowerJitterInfo", n6_jitter_information->lower_jitter_info) == NULL) {
        ogs_error("OpenAPI_n6_jitter_information_convertToJSON() failed [lower_jitter_info]");
        goto end;
    }
    }

    if (n6_jitter_information->is_higher_jitter_info) {
    if (cJSON_AddNumberToObject(item, "higherJitterInfo", n6_jitter_information->higher_jitter_info) == NULL) {
        ogs_error("OpenAPI_n6_jitter_information_convertToJSON() failed [higher_jitter_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n6_jitter_information_t *OpenAPI_n6_jitter_information_parseFromJSON(cJSON *n6_jitter_informationJSON)
{
    OpenAPI_n6_jitter_information_t *n6_jitter_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lower_jitter_info = NULL;
    cJSON *higher_jitter_info = NULL;
    lower_jitter_info = cJSON_GetObjectItemCaseSensitive(n6_jitter_informationJSON, "lowerJitterInfo");
    if (lower_jitter_info) {
    if (!cJSON_IsNumber(lower_jitter_info)) {
        ogs_error("OpenAPI_n6_jitter_information_parseFromJSON() failed [lower_jitter_info]");
        goto end;
    }
    }

    higher_jitter_info = cJSON_GetObjectItemCaseSensitive(n6_jitter_informationJSON, "higherJitterInfo");
    if (higher_jitter_info) {
    if (!cJSON_IsNumber(higher_jitter_info)) {
        ogs_error("OpenAPI_n6_jitter_information_parseFromJSON() failed [higher_jitter_info]");
        goto end;
    }
    }

    n6_jitter_information_local_var = OpenAPI_n6_jitter_information_create (
        lower_jitter_info ? true : false,
        lower_jitter_info ? lower_jitter_info->valuedouble : 0,
        higher_jitter_info ? true : false,
        higher_jitter_info ? higher_jitter_info->valuedouble : 0
    );

    return n6_jitter_information_local_var;
end:
    return NULL;
}

OpenAPI_n6_jitter_information_t *OpenAPI_n6_jitter_information_copy(OpenAPI_n6_jitter_information_t *dst, OpenAPI_n6_jitter_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n6_jitter_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n6_jitter_information_convertToJSON() failed");
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

    OpenAPI_n6_jitter_information_free(dst);
    dst = OpenAPI_n6_jitter_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

