
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allocate_unit.h"

OpenAPI_allocate_unit_t *OpenAPI_allocate_unit_create(
    OpenAPI_allocate_unit_indicator_e allocate_unit_indicator,
    OpenAPI_nsac_container_information_t *n_sac_container_information
)
{
    OpenAPI_allocate_unit_t *allocate_unit_local_var = ogs_malloc(sizeof(OpenAPI_allocate_unit_t));
    ogs_assert(allocate_unit_local_var);

    allocate_unit_local_var->allocate_unit_indicator = allocate_unit_indicator;
    allocate_unit_local_var->n_sac_container_information = n_sac_container_information;

    return allocate_unit_local_var;
}

void OpenAPI_allocate_unit_free(OpenAPI_allocate_unit_t *allocate_unit)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == allocate_unit) {
        return;
    }
    if (allocate_unit->n_sac_container_information) {
        OpenAPI_nsac_container_information_free(allocate_unit->n_sac_container_information);
        allocate_unit->n_sac_container_information = NULL;
    }
    ogs_free(allocate_unit);
}

cJSON *OpenAPI_allocate_unit_convertToJSON(OpenAPI_allocate_unit_t *allocate_unit)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (allocate_unit == NULL) {
        ogs_error("OpenAPI_allocate_unit_convertToJSON() failed [AllocateUnit]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (allocate_unit->allocate_unit_indicator != OpenAPI_allocate_unit_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "allocateUnitIndicator", OpenAPI_allocate_unit_indicator_ToString(allocate_unit->allocate_unit_indicator)) == NULL) {
        ogs_error("OpenAPI_allocate_unit_convertToJSON() failed [allocate_unit_indicator]");
        goto end;
    }
    }

    if (allocate_unit->n_sac_container_information) {
    cJSON *n_sac_container_information_local_JSON = OpenAPI_nsac_container_information_convertToJSON(allocate_unit->n_sac_container_information);
    if (n_sac_container_information_local_JSON == NULL) {
        ogs_error("OpenAPI_allocate_unit_convertToJSON() failed [n_sac_container_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nSACContainerInformation", n_sac_container_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_allocate_unit_convertToJSON() failed [n_sac_container_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_allocate_unit_t *OpenAPI_allocate_unit_parseFromJSON(cJSON *allocate_unitJSON)
{
    OpenAPI_allocate_unit_t *allocate_unit_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *allocate_unit_indicator = NULL;
    OpenAPI_allocate_unit_indicator_e allocate_unit_indicatorVariable = 0;
    cJSON *n_sac_container_information = NULL;
    OpenAPI_nsac_container_information_t *n_sac_container_information_local_nonprim = NULL;
    allocate_unit_indicator = cJSON_GetObjectItemCaseSensitive(allocate_unitJSON, "allocateUnitIndicator");
    if (allocate_unit_indicator) {
    if (!cJSON_IsString(allocate_unit_indicator)) {
        ogs_error("OpenAPI_allocate_unit_parseFromJSON() failed [allocate_unit_indicator]");
        goto end;
    }
    allocate_unit_indicatorVariable = OpenAPI_allocate_unit_indicator_FromString(allocate_unit_indicator->valuestring);
    }

    n_sac_container_information = cJSON_GetObjectItemCaseSensitive(allocate_unitJSON, "nSACContainerInformation");
    if (n_sac_container_information) {
    n_sac_container_information_local_nonprim = OpenAPI_nsac_container_information_parseFromJSON(n_sac_container_information);
    if (!n_sac_container_information_local_nonprim) {
        ogs_error("OpenAPI_nsac_container_information_parseFromJSON failed [n_sac_container_information]");
        goto end;
    }
    }

    allocate_unit_local_var = OpenAPI_allocate_unit_create (
        allocate_unit_indicator ? allocate_unit_indicatorVariable : 0,
        n_sac_container_information ? n_sac_container_information_local_nonprim : NULL
    );

    return allocate_unit_local_var;
end:
    if (n_sac_container_information_local_nonprim) {
        OpenAPI_nsac_container_information_free(n_sac_container_information_local_nonprim);
        n_sac_container_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_allocate_unit_t *OpenAPI_allocate_unit_copy(OpenAPI_allocate_unit_t *dst, OpenAPI_allocate_unit_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_allocate_unit_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_allocate_unit_convertToJSON() failed");
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

    OpenAPI_allocate_unit_free(dst);
    dst = OpenAPI_allocate_unit_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

