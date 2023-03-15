
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_perf_info.h"

OpenAPI_network_perf_info_t *OpenAPI_network_perf_info_create(
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_network_perf_type_t *nw_perf_type,
    bool is_relative_ratio,
    int relative_ratio,
    bool is_absolute_num,
    int absolute_num,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_network_perf_info_t *network_perf_info_local_var = ogs_malloc(sizeof(OpenAPI_network_perf_info_t));
    ogs_assert(network_perf_info_local_var);

    network_perf_info_local_var->network_area = network_area;
    network_perf_info_local_var->nw_perf_type = nw_perf_type;
    network_perf_info_local_var->is_relative_ratio = is_relative_ratio;
    network_perf_info_local_var->relative_ratio = relative_ratio;
    network_perf_info_local_var->is_absolute_num = is_absolute_num;
    network_perf_info_local_var->absolute_num = absolute_num;
    network_perf_info_local_var->is_confidence = is_confidence;
    network_perf_info_local_var->confidence = confidence;

    return network_perf_info_local_var;
}

void OpenAPI_network_perf_info_free(OpenAPI_network_perf_info_t *network_perf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_perf_info) {
        return;
    }
    if (network_perf_info->network_area) {
        OpenAPI_network_area_info_free(network_perf_info->network_area);
        network_perf_info->network_area = NULL;
    }
    if (network_perf_info->nw_perf_type) {
        OpenAPI_network_perf_type_free(network_perf_info->nw_perf_type);
        network_perf_info->nw_perf_type = NULL;
    }
    ogs_free(network_perf_info);
}

cJSON *OpenAPI_network_perf_info_convertToJSON(OpenAPI_network_perf_info_t *network_perf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_perf_info == NULL) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed [NetworkPerfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (network_perf_info->network_area) {
    cJSON *network_area_local_JSON = OpenAPI_network_area_info_convertToJSON(network_perf_info->network_area);
    if (network_area_local_JSON == NULL) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed [network_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkArea", network_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed [network_area]");
        goto end;
    }
    }

    if (network_perf_info->nw_perf_type) {
    cJSON *nw_perf_type_local_JSON = OpenAPI_network_perf_type_convertToJSON(network_perf_info->nw_perf_type);
    if (nw_perf_type_local_JSON == NULL) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed [nw_perf_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwPerfType", nw_perf_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed [nw_perf_type]");
        goto end;
    }
    }

    if (network_perf_info->is_relative_ratio) {
    if (cJSON_AddNumberToObject(item, "relativeRatio", network_perf_info->relative_ratio) == NULL) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed [relative_ratio]");
        goto end;
    }
    }

    if (network_perf_info->is_absolute_num) {
    if (cJSON_AddNumberToObject(item, "absoluteNum", network_perf_info->absolute_num) == NULL) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed [absolute_num]");
        goto end;
    }
    }

    if (network_perf_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", network_perf_info->confidence) == NULL) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_network_perf_info_t *OpenAPI_network_perf_info_parseFromJSON(cJSON *network_perf_infoJSON)
{
    OpenAPI_network_perf_info_t *network_perf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *network_area = NULL;
    OpenAPI_network_area_info_t *network_area_local_nonprim = NULL;
    cJSON *nw_perf_type = NULL;
    OpenAPI_network_perf_type_t *nw_perf_type_local_nonprim = NULL;
    cJSON *relative_ratio = NULL;
    cJSON *absolute_num = NULL;
    cJSON *confidence = NULL;
    network_area = cJSON_GetObjectItemCaseSensitive(network_perf_infoJSON, "networkArea");
    if (network_area) {
    network_area_local_nonprim = OpenAPI_network_area_info_parseFromJSON(network_area);
    if (!network_area_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [network_area]");
        goto end;
    }
    }

    nw_perf_type = cJSON_GetObjectItemCaseSensitive(network_perf_infoJSON, "nwPerfType");
    if (nw_perf_type) {
    nw_perf_type_local_nonprim = OpenAPI_network_perf_type_parseFromJSON(nw_perf_type);
    if (!nw_perf_type_local_nonprim) {
        ogs_error("OpenAPI_network_perf_type_parseFromJSON failed [nw_perf_type]");
        goto end;
    }
    }

    relative_ratio = cJSON_GetObjectItemCaseSensitive(network_perf_infoJSON, "relativeRatio");
    if (relative_ratio) {
    if (!cJSON_IsNumber(relative_ratio)) {
        ogs_error("OpenAPI_network_perf_info_parseFromJSON() failed [relative_ratio]");
        goto end;
    }
    }

    absolute_num = cJSON_GetObjectItemCaseSensitive(network_perf_infoJSON, "absoluteNum");
    if (absolute_num) {
    if (!cJSON_IsNumber(absolute_num)) {
        ogs_error("OpenAPI_network_perf_info_parseFromJSON() failed [absolute_num]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(network_perf_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_network_perf_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    network_perf_info_local_var = OpenAPI_network_perf_info_create (
        network_area ? network_area_local_nonprim : NULL,
        nw_perf_type ? nw_perf_type_local_nonprim : NULL,
        relative_ratio ? true : false,
        relative_ratio ? relative_ratio->valuedouble : 0,
        absolute_num ? true : false,
        absolute_num ? absolute_num->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return network_perf_info_local_var;
end:
    if (network_area_local_nonprim) {
        OpenAPI_network_area_info_free(network_area_local_nonprim);
        network_area_local_nonprim = NULL;
    }
    if (nw_perf_type_local_nonprim) {
        OpenAPI_network_perf_type_free(nw_perf_type_local_nonprim);
        nw_perf_type_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_network_perf_info_t *OpenAPI_network_perf_info_copy(OpenAPI_network_perf_info_t *dst, OpenAPI_network_perf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_perf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_perf_info_convertToJSON() failed");
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

    OpenAPI_network_perf_info_free(dst);
    dst = OpenAPI_network_perf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

