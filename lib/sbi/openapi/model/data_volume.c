
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_volume.h"

OpenAPI_data_volume_t *OpenAPI_data_volume_create(
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume
)
{
    OpenAPI_data_volume_t *data_volume_local_var = ogs_malloc(sizeof(OpenAPI_data_volume_t));
    ogs_assert(data_volume_local_var);

    data_volume_local_var->is_uplink_volume = is_uplink_volume;
    data_volume_local_var->uplink_volume = uplink_volume;
    data_volume_local_var->is_downlink_volume = is_downlink_volume;
    data_volume_local_var->downlink_volume = downlink_volume;

    return data_volume_local_var;
}

void OpenAPI_data_volume_free(OpenAPI_data_volume_t *data_volume)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == data_volume) {
        return;
    }
    ogs_free(data_volume);
}

cJSON *OpenAPI_data_volume_convertToJSON(OpenAPI_data_volume_t *data_volume)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (data_volume == NULL) {
        ogs_error("OpenAPI_data_volume_convertToJSON() failed [DataVolume]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (data_volume->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", data_volume->uplink_volume) == NULL) {
        ogs_error("OpenAPI_data_volume_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

    if (data_volume->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", data_volume->downlink_volume) == NULL) {
        ogs_error("OpenAPI_data_volume_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_data_volume_t *OpenAPI_data_volume_parseFromJSON(cJSON *data_volumeJSON)
{
    OpenAPI_data_volume_t *data_volume_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uplink_volume = NULL;
    cJSON *downlink_volume = NULL;
    uplink_volume = cJSON_GetObjectItemCaseSensitive(data_volumeJSON, "uplinkVolume");
    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_data_volume_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(data_volumeJSON, "downlinkVolume");
    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_data_volume_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    data_volume_local_var = OpenAPI_data_volume_create (
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0
    );

    return data_volume_local_var;
end:
    return NULL;
}

OpenAPI_data_volume_t *OpenAPI_data_volume_copy(OpenAPI_data_volume_t *dst, OpenAPI_data_volume_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_data_volume_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_data_volume_convertToJSON() failed");
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

    OpenAPI_data_volume_free(dst);
    dst = OpenAPI_data_volume_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

