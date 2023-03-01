
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "final_unit_action.h"

OpenAPI_final_unit_action_t *OpenAPI_final_unit_action_create(
)
{
    OpenAPI_final_unit_action_t *final_unit_action_local_var = ogs_malloc(sizeof(OpenAPI_final_unit_action_t));
    ogs_assert(final_unit_action_local_var);


    return final_unit_action_local_var;
}

void OpenAPI_final_unit_action_free(OpenAPI_final_unit_action_t *final_unit_action)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == final_unit_action) {
        return;
    }
    ogs_free(final_unit_action);
}

cJSON *OpenAPI_final_unit_action_convertToJSON(OpenAPI_final_unit_action_t *final_unit_action)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (final_unit_action == NULL) {
        ogs_error("OpenAPI_final_unit_action_convertToJSON() failed [FinalUnitAction]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_final_unit_action_t *OpenAPI_final_unit_action_parseFromJSON(cJSON *final_unit_actionJSON)
{
    OpenAPI_final_unit_action_t *final_unit_action_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    final_unit_action_local_var = OpenAPI_final_unit_action_create (
    );

    return final_unit_action_local_var;
end:
    return NULL;
}

OpenAPI_final_unit_action_t *OpenAPI_final_unit_action_copy(OpenAPI_final_unit_action_t *dst, OpenAPI_final_unit_action_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_final_unit_action_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_final_unit_action_convertToJSON() failed");
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

    OpenAPI_final_unit_action_free(dst);
    dst = OpenAPI_final_unit_action_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

