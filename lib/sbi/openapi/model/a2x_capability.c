
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "a2x_capability.h"

OpenAPI_a2x_capability_t *OpenAPI_a2x_capability_create(
    bool is_lte_a2x,
    int lte_a2x,
    bool is_nr_a2x,
    int nr_a2x
)
{
    OpenAPI_a2x_capability_t *a2x_capability_local_var = ogs_malloc(sizeof(OpenAPI_a2x_capability_t));
    ogs_assert(a2x_capability_local_var);

    a2x_capability_local_var->is_lte_a2x = is_lte_a2x;
    a2x_capability_local_var->lte_a2x = lte_a2x;
    a2x_capability_local_var->is_nr_a2x = is_nr_a2x;
    a2x_capability_local_var->nr_a2x = nr_a2x;

    return a2x_capability_local_var;
}

void OpenAPI_a2x_capability_free(OpenAPI_a2x_capability_t *a2x_capability)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == a2x_capability) {
        return;
    }
    ogs_free(a2x_capability);
}

cJSON *OpenAPI_a2x_capability_convertToJSON(OpenAPI_a2x_capability_t *a2x_capability)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (a2x_capability == NULL) {
        ogs_error("OpenAPI_a2x_capability_convertToJSON() failed [A2xCapability]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (a2x_capability->is_lte_a2x) {
    if (cJSON_AddBoolToObject(item, "lteA2x", a2x_capability->lte_a2x) == NULL) {
        ogs_error("OpenAPI_a2x_capability_convertToJSON() failed [lte_a2x]");
        goto end;
    }
    }

    if (a2x_capability->is_nr_a2x) {
    if (cJSON_AddBoolToObject(item, "nrA2x", a2x_capability->nr_a2x) == NULL) {
        ogs_error("OpenAPI_a2x_capability_convertToJSON() failed [nr_a2x]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_a2x_capability_t *OpenAPI_a2x_capability_parseFromJSON(cJSON *a2x_capabilityJSON)
{
    OpenAPI_a2x_capability_t *a2x_capability_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lte_a2x = NULL;
    cJSON *nr_a2x = NULL;
    lte_a2x = cJSON_GetObjectItemCaseSensitive(a2x_capabilityJSON, "lteA2x");
    if (lte_a2x) {
    if (!cJSON_IsBool(lte_a2x)) {
        ogs_error("OpenAPI_a2x_capability_parseFromJSON() failed [lte_a2x]");
        goto end;
    }
    }

    nr_a2x = cJSON_GetObjectItemCaseSensitive(a2x_capabilityJSON, "nrA2x");
    if (nr_a2x) {
    if (!cJSON_IsBool(nr_a2x)) {
        ogs_error("OpenAPI_a2x_capability_parseFromJSON() failed [nr_a2x]");
        goto end;
    }
    }

    a2x_capability_local_var = OpenAPI_a2x_capability_create (
        lte_a2x ? true : false,
        lte_a2x ? lte_a2x->valueint : 0,
        nr_a2x ? true : false,
        nr_a2x ? nr_a2x->valueint : 0
    );

    return a2x_capability_local_var;
end:
    return NULL;
}

OpenAPI_a2x_capability_t *OpenAPI_a2x_capability_copy(OpenAPI_a2x_capability_t *dst, OpenAPI_a2x_capability_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_a2x_capability_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_a2x_capability_convertToJSON() failed");
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

    OpenAPI_a2x_capability_free(dst);
    dst = OpenAPI_a2x_capability_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

