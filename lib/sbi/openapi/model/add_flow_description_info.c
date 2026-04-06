
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "add_flow_description_info.h"

OpenAPI_add_flow_description_info_t *OpenAPI_add_flow_description_info_create(
    char *spi,
    char *flow_label,
    OpenAPI_flow_direction_e flow_dir
)
{
    OpenAPI_add_flow_description_info_t *add_flow_description_info_local_var = ogs_malloc(sizeof(OpenAPI_add_flow_description_info_t));
    ogs_assert(add_flow_description_info_local_var);

    add_flow_description_info_local_var->spi = spi;
    add_flow_description_info_local_var->flow_label = flow_label;
    add_flow_description_info_local_var->flow_dir = flow_dir;

    return add_flow_description_info_local_var;
}

void OpenAPI_add_flow_description_info_free(OpenAPI_add_flow_description_info_t *add_flow_description_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == add_flow_description_info) {
        return;
    }
    if (add_flow_description_info->spi) {
        ogs_free(add_flow_description_info->spi);
        add_flow_description_info->spi = NULL;
    }
    if (add_flow_description_info->flow_label) {
        ogs_free(add_flow_description_info->flow_label);
        add_flow_description_info->flow_label = NULL;
    }
    ogs_free(add_flow_description_info);
}

cJSON *OpenAPI_add_flow_description_info_convertToJSON(OpenAPI_add_flow_description_info_t *add_flow_description_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (add_flow_description_info == NULL) {
        ogs_error("OpenAPI_add_flow_description_info_convertToJSON() failed [AddFlowDescriptionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (add_flow_description_info->spi) {
    if (cJSON_AddStringToObject(item, "spi", add_flow_description_info->spi) == NULL) {
        ogs_error("OpenAPI_add_flow_description_info_convertToJSON() failed [spi]");
        goto end;
    }
    }

    if (add_flow_description_info->flow_label) {
    if (cJSON_AddStringToObject(item, "flowLabel", add_flow_description_info->flow_label) == NULL) {
        ogs_error("OpenAPI_add_flow_description_info_convertToJSON() failed [flow_label]");
        goto end;
    }
    }

    if (add_flow_description_info->flow_dir != OpenAPI_flow_direction_NULL) {
    if (cJSON_AddStringToObject(item, "flowDir", OpenAPI_flow_direction_ToString(add_flow_description_info->flow_dir)) == NULL) {
        ogs_error("OpenAPI_add_flow_description_info_convertToJSON() failed [flow_dir]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_add_flow_description_info_t *OpenAPI_add_flow_description_info_parseFromJSON(cJSON *add_flow_description_infoJSON)
{
    OpenAPI_add_flow_description_info_t *add_flow_description_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *spi = NULL;
    cJSON *flow_label = NULL;
    cJSON *flow_dir = NULL;
    OpenAPI_flow_direction_e flow_dirVariable = 0;
    spi = cJSON_GetObjectItemCaseSensitive(add_flow_description_infoJSON, "spi");
    if (spi) {
    if (!cJSON_IsString(spi) && !cJSON_IsNull(spi)) {
        ogs_error("OpenAPI_add_flow_description_info_parseFromJSON() failed [spi]");
        goto end;
    }
    }

    flow_label = cJSON_GetObjectItemCaseSensitive(add_flow_description_infoJSON, "flowLabel");
    if (flow_label) {
    if (!cJSON_IsString(flow_label) && !cJSON_IsNull(flow_label)) {
        ogs_error("OpenAPI_add_flow_description_info_parseFromJSON() failed [flow_label]");
        goto end;
    }
    }

    flow_dir = cJSON_GetObjectItemCaseSensitive(add_flow_description_infoJSON, "flowDir");
    if (flow_dir) {
    if (!cJSON_IsString(flow_dir)) {
        ogs_error("OpenAPI_add_flow_description_info_parseFromJSON() failed [flow_dir]");
        goto end;
    }
    flow_dirVariable = OpenAPI_flow_direction_FromString(flow_dir->valuestring);
    }

    add_flow_description_info_local_var = OpenAPI_add_flow_description_info_create (
        spi && !cJSON_IsNull(spi) ? ogs_strdup(spi->valuestring) : NULL,
        flow_label && !cJSON_IsNull(flow_label) ? ogs_strdup(flow_label->valuestring) : NULL,
        flow_dir ? flow_dirVariable : 0
    );

    return add_flow_description_info_local_var;
end:
    return NULL;
}

OpenAPI_add_flow_description_info_t *OpenAPI_add_flow_description_info_copy(OpenAPI_add_flow_description_info_t *dst, OpenAPI_add_flow_description_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_add_flow_description_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_add_flow_description_info_convertToJSON() failed");
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

    OpenAPI_add_flow_description_info_free(dst);
    dst = OpenAPI_add_flow_description_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

