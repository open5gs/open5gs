
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecs_addr_config_info.h"

OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_create(
    OpenAPI_ecs_server_addr_t *ecs_server_addr,
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond
)
{
    OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info_local_var = ogs_malloc(sizeof(OpenAPI_ecs_addr_config_info_t));
    ogs_assert(ecs_addr_config_info_local_var);

    ecs_addr_config_info_local_var->ecs_server_addr = ecs_server_addr;
    ecs_addr_config_info_local_var->spatial_validity_cond = spatial_validity_cond;

    return ecs_addr_config_info_local_var;
}

void OpenAPI_ecs_addr_config_info_free(OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ecs_addr_config_info) {
        return;
    }
    if (ecs_addr_config_info->ecs_server_addr) {
        OpenAPI_ecs_server_addr_free(ecs_addr_config_info->ecs_server_addr);
        ecs_addr_config_info->ecs_server_addr = NULL;
    }
    if (ecs_addr_config_info->spatial_validity_cond) {
        OpenAPI_spatial_validity_cond_free(ecs_addr_config_info->spatial_validity_cond);
        ecs_addr_config_info->spatial_validity_cond = NULL;
    }
    ogs_free(ecs_addr_config_info);
}

cJSON *OpenAPI_ecs_addr_config_info_convertToJSON(OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ecs_addr_config_info == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [EcsAddrConfigInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ecs_addr_config_info->ecs_server_addr) {
    cJSON *ecs_server_addr_local_JSON = OpenAPI_ecs_server_addr_convertToJSON(ecs_addr_config_info->ecs_server_addr);
    if (ecs_server_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [ecs_server_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecsServerAddr", ecs_server_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [ecs_server_addr]");
        goto end;
    }
    }

    if (ecs_addr_config_info->spatial_validity_cond) {
    cJSON *spatial_validity_cond_local_JSON = OpenAPI_spatial_validity_cond_convertToJSON(ecs_addr_config_info->spatial_validity_cond);
    if (spatial_validity_cond_local_JSON == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidityCond", spatial_validity_cond_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_parseFromJSON(cJSON *ecs_addr_config_infoJSON)
{
    OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ecs_server_addr = NULL;
    OpenAPI_ecs_server_addr_t *ecs_server_addr_local_nonprim = NULL;
    cJSON *spatial_validity_cond = NULL;
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond_local_nonprim = NULL;
    ecs_server_addr = cJSON_GetObjectItemCaseSensitive(ecs_addr_config_infoJSON, "ecsServerAddr");
    if (ecs_server_addr) {
    ecs_server_addr_local_nonprim = OpenAPI_ecs_server_addr_parseFromJSON(ecs_server_addr);
    if (!ecs_server_addr_local_nonprim) {
        ogs_error("OpenAPI_ecs_server_addr_parseFromJSON failed [ecs_server_addr]");
        goto end;
    }
    }

    spatial_validity_cond = cJSON_GetObjectItemCaseSensitive(ecs_addr_config_infoJSON, "spatialValidityCond");
    if (spatial_validity_cond) {
    spatial_validity_cond_local_nonprim = OpenAPI_spatial_validity_cond_parseFromJSON(spatial_validity_cond);
    if (!spatial_validity_cond_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_cond_parseFromJSON failed [spatial_validity_cond]");
        goto end;
    }
    }

    ecs_addr_config_info_local_var = OpenAPI_ecs_addr_config_info_create (
        ecs_server_addr ? ecs_server_addr_local_nonprim : NULL,
        spatial_validity_cond ? spatial_validity_cond_local_nonprim : NULL
    );

    return ecs_addr_config_info_local_var;
end:
    if (ecs_server_addr_local_nonprim) {
        OpenAPI_ecs_server_addr_free(ecs_server_addr_local_nonprim);
        ecs_server_addr_local_nonprim = NULL;
    }
    if (spatial_validity_cond_local_nonprim) {
        OpenAPI_spatial_validity_cond_free(spatial_validity_cond_local_nonprim);
        spatial_validity_cond_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_copy(OpenAPI_ecs_addr_config_info_t *dst, OpenAPI_ecs_addr_config_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ecs_addr_config_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ecs_addr_config_info_convertToJSON() failed");
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

    OpenAPI_ecs_addr_config_info_free(dst);
    dst = OpenAPI_ecs_addr_config_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

