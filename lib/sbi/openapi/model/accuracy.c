
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accuracy.h"

OpenAPI_accuracy_t *OpenAPI_accuracy_create(
)
{
    OpenAPI_accuracy_t *accuracy_local_var = ogs_malloc(sizeof(OpenAPI_accuracy_t));
    ogs_assert(accuracy_local_var);


    return accuracy_local_var;
}

void OpenAPI_accuracy_free(OpenAPI_accuracy_t *accuracy)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == accuracy) {
        return;
    }
    ogs_free(accuracy);
}

cJSON *OpenAPI_accuracy_convertToJSON(OpenAPI_accuracy_t *accuracy)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (accuracy == NULL) {
        ogs_error("OpenAPI_accuracy_convertToJSON() failed [Accuracy]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_accuracy_t *OpenAPI_accuracy_parseFromJSON(cJSON *accuracyJSON)
{
    OpenAPI_accuracy_t *accuracy_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    accuracy_local_var = OpenAPI_accuracy_create (
    );

    return accuracy_local_var;
end:
    return NULL;
}

OpenAPI_accuracy_t *OpenAPI_accuracy_copy(OpenAPI_accuracy_t *dst, OpenAPI_accuracy_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_accuracy_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_accuracy_convertToJSON() failed");
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

    OpenAPI_accuracy_free(dst);
    dst = OpenAPI_accuracy_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

