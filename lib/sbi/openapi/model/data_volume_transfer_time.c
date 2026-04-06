
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_volume_transfer_time.h"

OpenAPI_data_volume_transfer_time_t *OpenAPI_data_volume_transfer_time_create(
    bool is_uplink_volume,
    long uplink_volume,
    bool is_avg_trans_time_ul,
    int avg_trans_time_ul,
    bool is_var_trans_time_ul,
    float var_trans_time_ul,
    bool is_downlink_volume,
    long downlink_volume,
    bool is_avg_trans_time_dl,
    int avg_trans_time_dl,
    bool is_var_trans_time_dl,
    float var_trans_time_dl
)
{
    OpenAPI_data_volume_transfer_time_t *data_volume_transfer_time_local_var = ogs_malloc(sizeof(OpenAPI_data_volume_transfer_time_t));
    ogs_assert(data_volume_transfer_time_local_var);

    data_volume_transfer_time_local_var->is_uplink_volume = is_uplink_volume;
    data_volume_transfer_time_local_var->uplink_volume = uplink_volume;
    data_volume_transfer_time_local_var->is_avg_trans_time_ul = is_avg_trans_time_ul;
    data_volume_transfer_time_local_var->avg_trans_time_ul = avg_trans_time_ul;
    data_volume_transfer_time_local_var->is_var_trans_time_ul = is_var_trans_time_ul;
    data_volume_transfer_time_local_var->var_trans_time_ul = var_trans_time_ul;
    data_volume_transfer_time_local_var->is_downlink_volume = is_downlink_volume;
    data_volume_transfer_time_local_var->downlink_volume = downlink_volume;
    data_volume_transfer_time_local_var->is_avg_trans_time_dl = is_avg_trans_time_dl;
    data_volume_transfer_time_local_var->avg_trans_time_dl = avg_trans_time_dl;
    data_volume_transfer_time_local_var->is_var_trans_time_dl = is_var_trans_time_dl;
    data_volume_transfer_time_local_var->var_trans_time_dl = var_trans_time_dl;

    return data_volume_transfer_time_local_var;
}

void OpenAPI_data_volume_transfer_time_free(OpenAPI_data_volume_transfer_time_t *data_volume_transfer_time)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == data_volume_transfer_time) {
        return;
    }
    ogs_free(data_volume_transfer_time);
}

cJSON *OpenAPI_data_volume_transfer_time_convertToJSON(OpenAPI_data_volume_transfer_time_t *data_volume_transfer_time)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (data_volume_transfer_time == NULL) {
        ogs_error("OpenAPI_data_volume_transfer_time_convertToJSON() failed [DataVolumeTransferTime]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (data_volume_transfer_time->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", data_volume_transfer_time->uplink_volume) == NULL) {
        ogs_error("OpenAPI_data_volume_transfer_time_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

    if (data_volume_transfer_time->is_avg_trans_time_ul) {
    if (cJSON_AddNumberToObject(item, "avgTransTimeUl", data_volume_transfer_time->avg_trans_time_ul) == NULL) {
        ogs_error("OpenAPI_data_volume_transfer_time_convertToJSON() failed [avg_trans_time_ul]");
        goto end;
    }
    }

    if (data_volume_transfer_time->is_var_trans_time_ul) {
    if (cJSON_AddNumberToObject(item, "varTransTimeUl", data_volume_transfer_time->var_trans_time_ul) == NULL) {
        ogs_error("OpenAPI_data_volume_transfer_time_convertToJSON() failed [var_trans_time_ul]");
        goto end;
    }
    }

    if (data_volume_transfer_time->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", data_volume_transfer_time->downlink_volume) == NULL) {
        ogs_error("OpenAPI_data_volume_transfer_time_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (data_volume_transfer_time->is_avg_trans_time_dl) {
    if (cJSON_AddNumberToObject(item, "avgTransTimeDl", data_volume_transfer_time->avg_trans_time_dl) == NULL) {
        ogs_error("OpenAPI_data_volume_transfer_time_convertToJSON() failed [avg_trans_time_dl]");
        goto end;
    }
    }

    if (data_volume_transfer_time->is_var_trans_time_dl) {
    if (cJSON_AddNumberToObject(item, "varTransTimeDl", data_volume_transfer_time->var_trans_time_dl) == NULL) {
        ogs_error("OpenAPI_data_volume_transfer_time_convertToJSON() failed [var_trans_time_dl]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_data_volume_transfer_time_t *OpenAPI_data_volume_transfer_time_parseFromJSON(cJSON *data_volume_transfer_timeJSON)
{
    OpenAPI_data_volume_transfer_time_t *data_volume_transfer_time_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uplink_volume = NULL;
    cJSON *avg_trans_time_ul = NULL;
    cJSON *var_trans_time_ul = NULL;
    cJSON *downlink_volume = NULL;
    cJSON *avg_trans_time_dl = NULL;
    cJSON *var_trans_time_dl = NULL;
    uplink_volume = cJSON_GetObjectItemCaseSensitive(data_volume_transfer_timeJSON, "uplinkVolume");
    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_data_volume_transfer_time_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    avg_trans_time_ul = cJSON_GetObjectItemCaseSensitive(data_volume_transfer_timeJSON, "avgTransTimeUl");
    if (avg_trans_time_ul) {
    if (!cJSON_IsNumber(avg_trans_time_ul)) {
        ogs_error("OpenAPI_data_volume_transfer_time_parseFromJSON() failed [avg_trans_time_ul]");
        goto end;
    }
    }

    var_trans_time_ul = cJSON_GetObjectItemCaseSensitive(data_volume_transfer_timeJSON, "varTransTimeUl");
    if (var_trans_time_ul) {
    if (!cJSON_IsNumber(var_trans_time_ul)) {
        ogs_error("OpenAPI_data_volume_transfer_time_parseFromJSON() failed [var_trans_time_ul]");
        goto end;
    }
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(data_volume_transfer_timeJSON, "downlinkVolume");
    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_data_volume_transfer_time_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    avg_trans_time_dl = cJSON_GetObjectItemCaseSensitive(data_volume_transfer_timeJSON, "avgTransTimeDl");
    if (avg_trans_time_dl) {
    if (!cJSON_IsNumber(avg_trans_time_dl)) {
        ogs_error("OpenAPI_data_volume_transfer_time_parseFromJSON() failed [avg_trans_time_dl]");
        goto end;
    }
    }

    var_trans_time_dl = cJSON_GetObjectItemCaseSensitive(data_volume_transfer_timeJSON, "varTransTimeDl");
    if (var_trans_time_dl) {
    if (!cJSON_IsNumber(var_trans_time_dl)) {
        ogs_error("OpenAPI_data_volume_transfer_time_parseFromJSON() failed [var_trans_time_dl]");
        goto end;
    }
    }

    data_volume_transfer_time_local_var = OpenAPI_data_volume_transfer_time_create (
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0,
        avg_trans_time_ul ? true : false,
        avg_trans_time_ul ? avg_trans_time_ul->valuedouble : 0,
        var_trans_time_ul ? true : false,
        var_trans_time_ul ? var_trans_time_ul->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0,
        avg_trans_time_dl ? true : false,
        avg_trans_time_dl ? avg_trans_time_dl->valuedouble : 0,
        var_trans_time_dl ? true : false,
        var_trans_time_dl ? var_trans_time_dl->valuedouble : 0
    );

    return data_volume_transfer_time_local_var;
end:
    return NULL;
}

OpenAPI_data_volume_transfer_time_t *OpenAPI_data_volume_transfer_time_copy(OpenAPI_data_volume_transfer_time_t *dst, OpenAPI_data_volume_transfer_time_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_data_volume_transfer_time_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_data_volume_transfer_time_convertToJSON() failed");
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

    OpenAPI_data_volume_transfer_time_free(dst);
    dst = OpenAPI_data_volume_transfer_time_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

