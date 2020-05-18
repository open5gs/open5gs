
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_information_class.h"

OpenAPI_n2_information_class_t *OpenAPI_n2_information_class_create(
    )
{
    OpenAPI_n2_information_class_t *n2_information_class_local_var = OpenAPI_malloc(sizeof(OpenAPI_n2_information_class_t));
    if (!n2_information_class_local_var) {
        return NULL;
    }

    return n2_information_class_local_var;
}

void OpenAPI_n2_information_class_free(OpenAPI_n2_information_class_t *n2_information_class)
{
    if (NULL == n2_information_class) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(n2_information_class);
}

cJSON *OpenAPI_n2_information_class_convertToJSON(OpenAPI_n2_information_class_t *n2_information_class)
{
    cJSON *item = NULL;

    if (n2_information_class == NULL) {
        ogs_error("OpenAPI_n2_information_class_convertToJSON() failed [N2InformationClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_n2_information_class_t *OpenAPI_n2_information_class_parseFromJSON(cJSON *n2_information_classJSON)
{
    OpenAPI_n2_information_class_t *n2_information_class_local_var = NULL;
    n2_information_class_local_var = OpenAPI_n2_information_class_create (
        );

    return n2_information_class_local_var;
end:
    return NULL;
}

