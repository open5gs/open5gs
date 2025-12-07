
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_qos_class.h"

OpenAPI_lcs_qos_class_t *OpenAPI_lcs_qos_class_create(
)
{
    OpenAPI_lcs_qos_class_t *lcs_qos_class_local_var = ogs_malloc(sizeof(OpenAPI_lcs_qos_class_t));
    ogs_assert(lcs_qos_class_local_var);


    return lcs_qos_class_local_var;
}

void OpenAPI_lcs_qos_class_free(OpenAPI_lcs_qos_class_t *lcs_qos_class)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_qos_class) {
        return;
    }
    ogs_free(lcs_qos_class);
}

cJSON *OpenAPI_lcs_qos_class_convertToJSON(OpenAPI_lcs_qos_class_t *lcs_qos_class)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_qos_class == NULL) {
        ogs_error("OpenAPI_lcs_qos_class_convertToJSON() failed [LcsQosClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_lcs_qos_class_t *OpenAPI_lcs_qos_class_parseFromJSON(cJSON *lcs_qos_classJSON)
{
    OpenAPI_lcs_qos_class_t *lcs_qos_class_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    lcs_qos_class_local_var = OpenAPI_lcs_qos_class_create (
    );

    return lcs_qos_class_local_var;
end:
    return NULL;
}

OpenAPI_lcs_qos_class_t *OpenAPI_lcs_qos_class_copy(OpenAPI_lcs_qos_class_t *dst, OpenAPI_lcs_qos_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_qos_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_qos_class_convertToJSON() failed");
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

    OpenAPI_lcs_qos_class_free(dst);
    dst = OpenAPI_lcs_qos_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

