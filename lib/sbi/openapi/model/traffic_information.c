
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_information.h"

OpenAPI_traffic_information_t *OpenAPI_traffic_information_create(
    char *uplink_rate,
    char *downlink_rate,
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume,
    bool is_total_volume,
    long total_volume
)
{
    OpenAPI_traffic_information_t *traffic_information_local_var = ogs_malloc(sizeof(OpenAPI_traffic_information_t));
    ogs_assert(traffic_information_local_var);

    traffic_information_local_var->uplink_rate = uplink_rate;
    traffic_information_local_var->downlink_rate = downlink_rate;
    traffic_information_local_var->is_uplink_volume = is_uplink_volume;
    traffic_information_local_var->uplink_volume = uplink_volume;
    traffic_information_local_var->is_downlink_volume = is_downlink_volume;
    traffic_information_local_var->downlink_volume = downlink_volume;
    traffic_information_local_var->is_total_volume = is_total_volume;
    traffic_information_local_var->total_volume = total_volume;

    return traffic_information_local_var;
}

void OpenAPI_traffic_information_free(OpenAPI_traffic_information_t *traffic_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_information) {
        return;
    }
    if (traffic_information->uplink_rate) {
        ogs_free(traffic_information->uplink_rate);
        traffic_information->uplink_rate = NULL;
    }
    if (traffic_information->downlink_rate) {
        ogs_free(traffic_information->downlink_rate);
        traffic_information->downlink_rate = NULL;
    }
    ogs_free(traffic_information);
}

cJSON *OpenAPI_traffic_information_convertToJSON(OpenAPI_traffic_information_t *traffic_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_information == NULL) {
        ogs_error("OpenAPI_traffic_information_convertToJSON() failed [TrafficInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_information->uplink_rate) {
    if (cJSON_AddStringToObject(item, "uplinkRate", traffic_information->uplink_rate) == NULL) {
        ogs_error("OpenAPI_traffic_information_convertToJSON() failed [uplink_rate]");
        goto end;
    }
    }

    if (traffic_information->downlink_rate) {
    if (cJSON_AddStringToObject(item, "downlinkRate", traffic_information->downlink_rate) == NULL) {
        ogs_error("OpenAPI_traffic_information_convertToJSON() failed [downlink_rate]");
        goto end;
    }
    }

    if (traffic_information->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", traffic_information->uplink_volume) == NULL) {
        ogs_error("OpenAPI_traffic_information_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

    if (traffic_information->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", traffic_information->downlink_volume) == NULL) {
        ogs_error("OpenAPI_traffic_information_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (traffic_information->is_total_volume) {
    if (cJSON_AddNumberToObject(item, "totalVolume", traffic_information->total_volume) == NULL) {
        ogs_error("OpenAPI_traffic_information_convertToJSON() failed [total_volume]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_traffic_information_t *OpenAPI_traffic_information_parseFromJSON(cJSON *traffic_informationJSON)
{
    OpenAPI_traffic_information_t *traffic_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uplink_rate = NULL;
    cJSON *downlink_rate = NULL;
    cJSON *uplink_volume = NULL;
    cJSON *downlink_volume = NULL;
    cJSON *total_volume = NULL;
    uplink_rate = cJSON_GetObjectItemCaseSensitive(traffic_informationJSON, "uplinkRate");
    if (uplink_rate) {
    if (!cJSON_IsString(uplink_rate) && !cJSON_IsNull(uplink_rate)) {
        ogs_error("OpenAPI_traffic_information_parseFromJSON() failed [uplink_rate]");
        goto end;
    }
    }

    downlink_rate = cJSON_GetObjectItemCaseSensitive(traffic_informationJSON, "downlinkRate");
    if (downlink_rate) {
    if (!cJSON_IsString(downlink_rate) && !cJSON_IsNull(downlink_rate)) {
        ogs_error("OpenAPI_traffic_information_parseFromJSON() failed [downlink_rate]");
        goto end;
    }
    }

    uplink_volume = cJSON_GetObjectItemCaseSensitive(traffic_informationJSON, "uplinkVolume");
    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_traffic_information_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(traffic_informationJSON, "downlinkVolume");
    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_traffic_information_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    total_volume = cJSON_GetObjectItemCaseSensitive(traffic_informationJSON, "totalVolume");
    if (total_volume) {
    if (!cJSON_IsNumber(total_volume)) {
        ogs_error("OpenAPI_traffic_information_parseFromJSON() failed [total_volume]");
        goto end;
    }
    }

    traffic_information_local_var = OpenAPI_traffic_information_create (
        uplink_rate && !cJSON_IsNull(uplink_rate) ? ogs_strdup(uplink_rate->valuestring) : NULL,
        downlink_rate && !cJSON_IsNull(downlink_rate) ? ogs_strdup(downlink_rate->valuestring) : NULL,
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0,
        total_volume ? true : false,
        total_volume ? total_volume->valuedouble : 0
    );

    return traffic_information_local_var;
end:
    return NULL;
}

OpenAPI_traffic_information_t *OpenAPI_traffic_information_copy(OpenAPI_traffic_information_t *dst, OpenAPI_traffic_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_information_convertToJSON() failed");
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

    OpenAPI_traffic_information_free(dst);
    dst = OpenAPI_traffic_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

