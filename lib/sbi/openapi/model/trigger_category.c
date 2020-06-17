
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trigger_category.h"

OpenAPI_trigger_category_t *OpenAPI_trigger_category_create(
    )
{
    OpenAPI_trigger_category_t *trigger_category_local_var = OpenAPI_malloc(sizeof(OpenAPI_trigger_category_t));
    if (!trigger_category_local_var) {
        return NULL;
    }

    return trigger_category_local_var;
}

void OpenAPI_trigger_category_free(OpenAPI_trigger_category_t *trigger_category)
{
    if (NULL == trigger_category) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(trigger_category);
}

cJSON *OpenAPI_trigger_category_convertToJSON(OpenAPI_trigger_category_t *trigger_category)
{
    cJSON *item = NULL;

    if (trigger_category == NULL) {
        ogs_error("OpenAPI_trigger_category_convertToJSON() failed [TriggerCategory]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_trigger_category_t *OpenAPI_trigger_category_parseFromJSON(cJSON *trigger_categoryJSON)
{
    OpenAPI_trigger_category_t *trigger_category_local_var = NULL;
    trigger_category_local_var = OpenAPI_trigger_category_create (
        );

    return trigger_category_local_var;
end:
    return NULL;
}

OpenAPI_trigger_category_t *OpenAPI_trigger_category_copy(OpenAPI_trigger_category_t *dst, OpenAPI_trigger_category_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trigger_category_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trigger_category_convertToJSON() failed");
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

    OpenAPI_trigger_category_free(dst);
    dst = OpenAPI_trigger_category_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

