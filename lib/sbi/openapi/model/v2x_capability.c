
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "v2x_capability.h"

OpenAPI_v2x_capability_t *OpenAPI_v2x_capability_create(
    bool is_lte_v2x,
    int lte_v2x,
    bool is_nr_v2x,
    int nr_v2x
)
{
    OpenAPI_v2x_capability_t *v2x_capability_local_var = ogs_malloc(sizeof(OpenAPI_v2x_capability_t));
    ogs_assert(v2x_capability_local_var);

    v2x_capability_local_var->is_lte_v2x = is_lte_v2x;
    v2x_capability_local_var->lte_v2x = lte_v2x;
    v2x_capability_local_var->is_nr_v2x = is_nr_v2x;
    v2x_capability_local_var->nr_v2x = nr_v2x;

    return v2x_capability_local_var;
}

void OpenAPI_v2x_capability_free(OpenAPI_v2x_capability_t *v2x_capability)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == v2x_capability) {
        return;
    }
    ogs_free(v2x_capability);
}

cJSON *OpenAPI_v2x_capability_convertToJSON(OpenAPI_v2x_capability_t *v2x_capability)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (v2x_capability == NULL) {
        ogs_error("OpenAPI_v2x_capability_convertToJSON() failed [V2xCapability]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (v2x_capability->is_lte_v2x) {
    if (cJSON_AddBoolToObject(item, "lteV2x", v2x_capability->lte_v2x) == NULL) {
        ogs_error("OpenAPI_v2x_capability_convertToJSON() failed [lte_v2x]");
        goto end;
    }
    }

    if (v2x_capability->is_nr_v2x) {
    if (cJSON_AddBoolToObject(item, "nrV2x", v2x_capability->nr_v2x) == NULL) {
        ogs_error("OpenAPI_v2x_capability_convertToJSON() failed [nr_v2x]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_v2x_capability_t *OpenAPI_v2x_capability_parseFromJSON(cJSON *v2x_capabilityJSON)
{
    OpenAPI_v2x_capability_t *v2x_capability_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lte_v2x = NULL;
    cJSON *nr_v2x = NULL;
    lte_v2x = cJSON_GetObjectItemCaseSensitive(v2x_capabilityJSON, "lteV2x");
    if (lte_v2x) {
    if (!cJSON_IsBool(lte_v2x)) {
        ogs_error("OpenAPI_v2x_capability_parseFromJSON() failed [lte_v2x]");
        goto end;
    }
    }

    nr_v2x = cJSON_GetObjectItemCaseSensitive(v2x_capabilityJSON, "nrV2x");
    if (nr_v2x) {
    if (!cJSON_IsBool(nr_v2x)) {
        ogs_error("OpenAPI_v2x_capability_parseFromJSON() failed [nr_v2x]");
        goto end;
    }
    }

    v2x_capability_local_var = OpenAPI_v2x_capability_create (
        lte_v2x ? true : false,
        lte_v2x ? lte_v2x->valueint : 0,
        nr_v2x ? true : false,
        nr_v2x ? nr_v2x->valueint : 0
    );

    return v2x_capability_local_var;
end:
    return NULL;
}

OpenAPI_v2x_capability_t *OpenAPI_v2x_capability_copy(OpenAPI_v2x_capability_t *dst, OpenAPI_v2x_capability_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_v2x_capability_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_v2x_capability_convertToJSON() failed");
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

    OpenAPI_v2x_capability_free(dst);
    dst = OpenAPI_v2x_capability_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

