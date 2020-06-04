
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "operator_managed.h"

OpenAPI_operator_managed_t *OpenAPI_operator_managed_create(
    )
{
    OpenAPI_operator_managed_t *operator_managed_local_var = OpenAPI_malloc(sizeof(OpenAPI_operator_managed_t));
    if (!operator_managed_local_var) {
        return NULL;
    }

    return operator_managed_local_var;
}

void OpenAPI_operator_managed_free(OpenAPI_operator_managed_t *operator_managed)
{
    if (NULL == operator_managed) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(operator_managed);
}

cJSON *OpenAPI_operator_managed_convertToJSON(OpenAPI_operator_managed_t *operator_managed)
{
    cJSON *item = NULL;

    if (operator_managed == NULL) {
        ogs_error("OpenAPI_operator_managed_convertToJSON() failed [OperatorManaged]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_operator_managed_t *OpenAPI_operator_managed_parseFromJSON(cJSON *operator_managedJSON)
{
    OpenAPI_operator_managed_t *operator_managed_local_var = NULL;
    operator_managed_local_var = OpenAPI_operator_managed_create (
        );

    return operator_managed_local_var;
end:
    return NULL;
}

