
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reflective_qo_s_attribute.h"

OpenAPI_reflective_qo_s_attribute_t *OpenAPI_reflective_qo_s_attribute_create(
    )
{
    OpenAPI_reflective_qo_s_attribute_t *reflective_qo_s_attribute_local_var = OpenAPI_malloc(sizeof(OpenAPI_reflective_qo_s_attribute_t));
    if (!reflective_qo_s_attribute_local_var) {
        return NULL;
    }

    return reflective_qo_s_attribute_local_var;
}

void OpenAPI_reflective_qo_s_attribute_free(OpenAPI_reflective_qo_s_attribute_t *reflective_qo_s_attribute)
{
    if (NULL == reflective_qo_s_attribute) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(reflective_qo_s_attribute);
}

cJSON *OpenAPI_reflective_qo_s_attribute_convertToJSON(OpenAPI_reflective_qo_s_attribute_t *reflective_qo_s_attribute)
{
    cJSON *item = NULL;

    if (reflective_qo_s_attribute == NULL) {
        ogs_error("OpenAPI_reflective_qo_s_attribute_convertToJSON() failed [ReflectiveQoSAttribute]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_reflective_qo_s_attribute_t *OpenAPI_reflective_qo_s_attribute_parseFromJSON(cJSON *reflective_qo_s_attributeJSON)
{
    OpenAPI_reflective_qo_s_attribute_t *reflective_qo_s_attribute_local_var = NULL;
    reflective_qo_s_attribute_local_var = OpenAPI_reflective_qo_s_attribute_create (
        );

    return reflective_qo_s_attribute_local_var;
end:
    return NULL;
}

OpenAPI_reflective_qo_s_attribute_t *OpenAPI_reflective_qo_s_attribute_copy(OpenAPI_reflective_qo_s_attribute_t *dst, OpenAPI_reflective_qo_s_attribute_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reflective_qo_s_attribute_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reflective_qo_s_attribute_convertToJSON() failed");
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

    OpenAPI_reflective_qo_s_attribute_free(dst);
    dst = OpenAPI_reflective_qo_s_attribute_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

