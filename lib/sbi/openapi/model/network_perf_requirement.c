
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_perf_requirement.h"

OpenAPI_network_perf_requirement_t *OpenAPI_network_perf_requirement_create(
    OpenAPI_network_perf_type_t *nw_perf_type,
    bool is_relative_ratio,
    int relative_ratio,
    bool is_absolute_num,
    int absolute_num
)
{
    OpenAPI_network_perf_requirement_t *network_perf_requirement_local_var = ogs_malloc(sizeof(OpenAPI_network_perf_requirement_t));
    ogs_assert(network_perf_requirement_local_var);

    network_perf_requirement_local_var->nw_perf_type = nw_perf_type;
    network_perf_requirement_local_var->is_relative_ratio = is_relative_ratio;
    network_perf_requirement_local_var->relative_ratio = relative_ratio;
    network_perf_requirement_local_var->is_absolute_num = is_absolute_num;
    network_perf_requirement_local_var->absolute_num = absolute_num;

    return network_perf_requirement_local_var;
}

void OpenAPI_network_perf_requirement_free(OpenAPI_network_perf_requirement_t *network_perf_requirement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_perf_requirement) {
        return;
    }
    if (network_perf_requirement->nw_perf_type) {
        OpenAPI_network_perf_type_free(network_perf_requirement->nw_perf_type);
        network_perf_requirement->nw_perf_type = NULL;
    }
    ogs_free(network_perf_requirement);
}

cJSON *OpenAPI_network_perf_requirement_convertToJSON(OpenAPI_network_perf_requirement_t *network_perf_requirement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_perf_requirement == NULL) {
        ogs_error("OpenAPI_network_perf_requirement_convertToJSON() failed [NetworkPerfRequirement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!network_perf_requirement->nw_perf_type) {
        ogs_error("OpenAPI_network_perf_requirement_convertToJSON() failed [nw_perf_type]");
        return NULL;
    }
    cJSON *nw_perf_type_local_JSON = OpenAPI_network_perf_type_convertToJSON(network_perf_requirement->nw_perf_type);
    if (nw_perf_type_local_JSON == NULL) {
        ogs_error("OpenAPI_network_perf_requirement_convertToJSON() failed [nw_perf_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwPerfType", nw_perf_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_perf_requirement_convertToJSON() failed [nw_perf_type]");
        goto end;
    }

    if (network_perf_requirement->is_relative_ratio) {
    if (cJSON_AddNumberToObject(item, "relativeRatio", network_perf_requirement->relative_ratio) == NULL) {
        ogs_error("OpenAPI_network_perf_requirement_convertToJSON() failed [relative_ratio]");
        goto end;
    }
    }

    if (network_perf_requirement->is_absolute_num) {
    if (cJSON_AddNumberToObject(item, "absoluteNum", network_perf_requirement->absolute_num) == NULL) {
        ogs_error("OpenAPI_network_perf_requirement_convertToJSON() failed [absolute_num]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_network_perf_requirement_t *OpenAPI_network_perf_requirement_parseFromJSON(cJSON *network_perf_requirementJSON)
{
    OpenAPI_network_perf_requirement_t *network_perf_requirement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nw_perf_type = NULL;
    OpenAPI_network_perf_type_t *nw_perf_type_local_nonprim = NULL;
    cJSON *relative_ratio = NULL;
    cJSON *absolute_num = NULL;
    nw_perf_type = cJSON_GetObjectItemCaseSensitive(network_perf_requirementJSON, "nwPerfType");
    if (!nw_perf_type) {
        ogs_error("OpenAPI_network_perf_requirement_parseFromJSON() failed [nw_perf_type]");
        goto end;
    }
    nw_perf_type_local_nonprim = OpenAPI_network_perf_type_parseFromJSON(nw_perf_type);
    if (!nw_perf_type_local_nonprim) {
        ogs_error("OpenAPI_network_perf_type_parseFromJSON failed [nw_perf_type]");
        goto end;
    }

    relative_ratio = cJSON_GetObjectItemCaseSensitive(network_perf_requirementJSON, "relativeRatio");
    if (relative_ratio) {
    if (!cJSON_IsNumber(relative_ratio)) {
        ogs_error("OpenAPI_network_perf_requirement_parseFromJSON() failed [relative_ratio]");
        goto end;
    }
    }

    absolute_num = cJSON_GetObjectItemCaseSensitive(network_perf_requirementJSON, "absoluteNum");
    if (absolute_num) {
    if (!cJSON_IsNumber(absolute_num)) {
        ogs_error("OpenAPI_network_perf_requirement_parseFromJSON() failed [absolute_num]");
        goto end;
    }
    }

    network_perf_requirement_local_var = OpenAPI_network_perf_requirement_create (
        nw_perf_type_local_nonprim,
        relative_ratio ? true : false,
        relative_ratio ? relative_ratio->valuedouble : 0,
        absolute_num ? true : false,
        absolute_num ? absolute_num->valuedouble : 0
    );

    return network_perf_requirement_local_var;
end:
    if (nw_perf_type_local_nonprim) {
        OpenAPI_network_perf_type_free(nw_perf_type_local_nonprim);
        nw_perf_type_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_network_perf_requirement_t *OpenAPI_network_perf_requirement_copy(OpenAPI_network_perf_requirement_t *dst, OpenAPI_network_perf_requirement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_perf_requirement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_perf_requirement_convertToJSON() failed");
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

    OpenAPI_network_perf_requirement_free(dst);
    dst = OpenAPI_network_perf_requirement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

