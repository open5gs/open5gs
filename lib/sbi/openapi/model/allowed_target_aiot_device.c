
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allowed_target_aiot_device.h"

OpenAPI_allowed_target_aiot_device_t *OpenAPI_allowed_target_aiot_device_create(
    char *aiot_dev_perm_id,
    char *filtering_info
)
{
    OpenAPI_allowed_target_aiot_device_t *allowed_target_aiot_device_local_var = ogs_malloc(sizeof(OpenAPI_allowed_target_aiot_device_t));
    ogs_assert(allowed_target_aiot_device_local_var);

    allowed_target_aiot_device_local_var->aiot_dev_perm_id = aiot_dev_perm_id;
    allowed_target_aiot_device_local_var->filtering_info = filtering_info;

    return allowed_target_aiot_device_local_var;
}

void OpenAPI_allowed_target_aiot_device_free(OpenAPI_allowed_target_aiot_device_t *allowed_target_aiot_device)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == allowed_target_aiot_device) {
        return;
    }
    if (allowed_target_aiot_device->aiot_dev_perm_id) {
        ogs_free(allowed_target_aiot_device->aiot_dev_perm_id);
        allowed_target_aiot_device->aiot_dev_perm_id = NULL;
    }
    if (allowed_target_aiot_device->filtering_info) {
        ogs_free(allowed_target_aiot_device->filtering_info);
        allowed_target_aiot_device->filtering_info = NULL;
    }
    ogs_free(allowed_target_aiot_device);
}

cJSON *OpenAPI_allowed_target_aiot_device_convertToJSON(OpenAPI_allowed_target_aiot_device_t *allowed_target_aiot_device)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (allowed_target_aiot_device == NULL) {
        ogs_error("OpenAPI_allowed_target_aiot_device_convertToJSON() failed [AllowedTargetAiotDevice]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (allowed_target_aiot_device->aiot_dev_perm_id) {
    if (cJSON_AddStringToObject(item, "aiotDevPermId", allowed_target_aiot_device->aiot_dev_perm_id) == NULL) {
        ogs_error("OpenAPI_allowed_target_aiot_device_convertToJSON() failed [aiot_dev_perm_id]");
        goto end;
    }
    }

    if (allowed_target_aiot_device->filtering_info) {
    if (cJSON_AddStringToObject(item, "filteringInfo", allowed_target_aiot_device->filtering_info) == NULL) {
        ogs_error("OpenAPI_allowed_target_aiot_device_convertToJSON() failed [filtering_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_allowed_target_aiot_device_t *OpenAPI_allowed_target_aiot_device_parseFromJSON(cJSON *allowed_target_aiot_deviceJSON)
{
    OpenAPI_allowed_target_aiot_device_t *allowed_target_aiot_device_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *aiot_dev_perm_id = NULL;
    cJSON *filtering_info = NULL;
    aiot_dev_perm_id = cJSON_GetObjectItemCaseSensitive(allowed_target_aiot_deviceJSON, "aiotDevPermId");
    if (aiot_dev_perm_id) {
    if (!cJSON_IsString(aiot_dev_perm_id) && !cJSON_IsNull(aiot_dev_perm_id)) {
        ogs_error("OpenAPI_allowed_target_aiot_device_parseFromJSON() failed [aiot_dev_perm_id]");
        goto end;
    }
    }

    filtering_info = cJSON_GetObjectItemCaseSensitive(allowed_target_aiot_deviceJSON, "filteringInfo");
    if (filtering_info) {
    if (!cJSON_IsString(filtering_info) && !cJSON_IsNull(filtering_info)) {
        ogs_error("OpenAPI_allowed_target_aiot_device_parseFromJSON() failed [filtering_info]");
        goto end;
    }
    }

    allowed_target_aiot_device_local_var = OpenAPI_allowed_target_aiot_device_create (
        aiot_dev_perm_id && !cJSON_IsNull(aiot_dev_perm_id) ? ogs_strdup(aiot_dev_perm_id->valuestring) : NULL,
        filtering_info && !cJSON_IsNull(filtering_info) ? ogs_strdup(filtering_info->valuestring) : NULL
    );

    return allowed_target_aiot_device_local_var;
end:
    return NULL;
}

OpenAPI_allowed_target_aiot_device_t *OpenAPI_allowed_target_aiot_device_copy(OpenAPI_allowed_target_aiot_device_t *dst, OpenAPI_allowed_target_aiot_device_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_allowed_target_aiot_device_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_allowed_target_aiot_device_convertToJSON() failed");
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

    OpenAPI_allowed_target_aiot_device_free(dst);
    dst = OpenAPI_allowed_target_aiot_device_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

