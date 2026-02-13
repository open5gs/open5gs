
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accuracy_fulfilment_indicator.h"

OpenAPI_accuracy_fulfilment_indicator_t *OpenAPI_accuracy_fulfilment_indicator_create(
)
{
    OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator_local_var = ogs_malloc(sizeof(OpenAPI_accuracy_fulfilment_indicator_t));
    ogs_assert(accuracy_fulfilment_indicator_local_var);


    return accuracy_fulfilment_indicator_local_var;
}

void OpenAPI_accuracy_fulfilment_indicator_free(OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == accuracy_fulfilment_indicator) {
        return;
    }
    ogs_free(accuracy_fulfilment_indicator);
}

cJSON *OpenAPI_accuracy_fulfilment_indicator_convertToJSON(OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (accuracy_fulfilment_indicator == NULL) {
        ogs_error("OpenAPI_accuracy_fulfilment_indicator_convertToJSON() failed [AccuracyFulfilmentIndicator]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_accuracy_fulfilment_indicator_t *OpenAPI_accuracy_fulfilment_indicator_parseFromJSON(cJSON *accuracy_fulfilment_indicatorJSON)
{
    OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    accuracy_fulfilment_indicator_local_var = OpenAPI_accuracy_fulfilment_indicator_create (
    );

    return accuracy_fulfilment_indicator_local_var;
end:
    return NULL;
}

OpenAPI_accuracy_fulfilment_indicator_t *OpenAPI_accuracy_fulfilment_indicator_copy(OpenAPI_accuracy_fulfilment_indicator_t *dst, OpenAPI_accuracy_fulfilment_indicator_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_accuracy_fulfilment_indicator_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_accuracy_fulfilment_indicator_convertToJSON() failed");
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

    OpenAPI_accuracy_fulfilment_indicator_free(dst);
    dst = OpenAPI_accuracy_fulfilment_indicator_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

