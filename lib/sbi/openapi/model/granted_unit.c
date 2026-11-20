
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "granted_unit.h"

OpenAPI_granted_unit_t *OpenAPI_granted_unit_create(
    char *tariff_time_change,
    bool is_time,
    int time,
    bool is_total_volume,
    long total_volume,
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume,
    bool is_service_specific_units,
    long service_specific_units
)
{
    OpenAPI_granted_unit_t *granted_unit_local_var = ogs_malloc(sizeof(OpenAPI_granted_unit_t));
    ogs_assert(granted_unit_local_var);

    granted_unit_local_var->tariff_time_change = tariff_time_change;
    granted_unit_local_var->is_time = is_time;
    granted_unit_local_var->time = time;
    granted_unit_local_var->is_total_volume = is_total_volume;
    granted_unit_local_var->total_volume = total_volume;
    granted_unit_local_var->is_uplink_volume = is_uplink_volume;
    granted_unit_local_var->uplink_volume = uplink_volume;
    granted_unit_local_var->is_downlink_volume = is_downlink_volume;
    granted_unit_local_var->downlink_volume = downlink_volume;
    granted_unit_local_var->is_service_specific_units = is_service_specific_units;
    granted_unit_local_var->service_specific_units = service_specific_units;

    return granted_unit_local_var;
}

void OpenAPI_granted_unit_free(OpenAPI_granted_unit_t *granted_unit)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == granted_unit) {
        return;
    }
    if (granted_unit->tariff_time_change) {
        ogs_free(granted_unit->tariff_time_change);
        granted_unit->tariff_time_change = NULL;
    }
    ogs_free(granted_unit);
}

cJSON *OpenAPI_granted_unit_convertToJSON(OpenAPI_granted_unit_t *granted_unit)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (granted_unit == NULL) {
        ogs_error("OpenAPI_granted_unit_convertToJSON() failed [GrantedUnit]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (granted_unit->tariff_time_change) {
    if (cJSON_AddStringToObject(item, "tariffTimeChange", granted_unit->tariff_time_change) == NULL) {
        ogs_error("OpenAPI_granted_unit_convertToJSON() failed [tariff_time_change]");
        goto end;
    }
    }

    if (granted_unit->is_time) {
    if (cJSON_AddNumberToObject(item, "time", granted_unit->time) == NULL) {
        ogs_error("OpenAPI_granted_unit_convertToJSON() failed [time]");
        goto end;
    }
    }

    if (granted_unit->is_total_volume) {
    if (cJSON_AddNumberToObject(item, "totalVolume", granted_unit->total_volume) == NULL) {
        ogs_error("OpenAPI_granted_unit_convertToJSON() failed [total_volume]");
        goto end;
    }
    }

    if (granted_unit->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", granted_unit->uplink_volume) == NULL) {
        ogs_error("OpenAPI_granted_unit_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

    if (granted_unit->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", granted_unit->downlink_volume) == NULL) {
        ogs_error("OpenAPI_granted_unit_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

    if (granted_unit->is_service_specific_units) {
    if (cJSON_AddNumberToObject(item, "serviceSpecificUnits", granted_unit->service_specific_units) == NULL) {
        ogs_error("OpenAPI_granted_unit_convertToJSON() failed [service_specific_units]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_granted_unit_t *OpenAPI_granted_unit_parseFromJSON(cJSON *granted_unitJSON)
{
    OpenAPI_granted_unit_t *granted_unit_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tariff_time_change = NULL;
    cJSON *time = NULL;
    cJSON *total_volume = NULL;
    cJSON *uplink_volume = NULL;
    cJSON *downlink_volume = NULL;
    cJSON *service_specific_units = NULL;
    tariff_time_change = cJSON_GetObjectItemCaseSensitive(granted_unitJSON, "tariffTimeChange");
    if (tariff_time_change) {
    if (!cJSON_IsString(tariff_time_change) && !cJSON_IsNull(tariff_time_change)) {
        ogs_error("OpenAPI_granted_unit_parseFromJSON() failed [tariff_time_change]");
        goto end;
    }
    }

    time = cJSON_GetObjectItemCaseSensitive(granted_unitJSON, "time");
    if (time) {
    if (!cJSON_IsNumber(time)) {
        ogs_error("OpenAPI_granted_unit_parseFromJSON() failed [time]");
        goto end;
    }
    }

    total_volume = cJSON_GetObjectItemCaseSensitive(granted_unitJSON, "totalVolume");
    if (total_volume) {
    if (!cJSON_IsNumber(total_volume)) {
        ogs_error("OpenAPI_granted_unit_parseFromJSON() failed [total_volume]");
        goto end;
    }
    }

    uplink_volume = cJSON_GetObjectItemCaseSensitive(granted_unitJSON, "uplinkVolume");
    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_granted_unit_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(granted_unitJSON, "downlinkVolume");
    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_granted_unit_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    service_specific_units = cJSON_GetObjectItemCaseSensitive(granted_unitJSON, "serviceSpecificUnits");
    if (service_specific_units) {
    if (!cJSON_IsNumber(service_specific_units)) {
        ogs_error("OpenAPI_granted_unit_parseFromJSON() failed [service_specific_units]");
        goto end;
    }
    }

    granted_unit_local_var = OpenAPI_granted_unit_create (
        tariff_time_change && !cJSON_IsNull(tariff_time_change) ? ogs_strdup(tariff_time_change->valuestring) : NULL,
        time ? true : false,
        time ? time->valuedouble : 0,
        total_volume ? true : false,
        total_volume ? total_volume->valuedouble : 0,
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0,
        service_specific_units ? true : false,
        service_specific_units ? service_specific_units->valuedouble : 0
    );

    return granted_unit_local_var;
end:
    return NULL;
}

OpenAPI_granted_unit_t *OpenAPI_granted_unit_copy(OpenAPI_granted_unit_t *dst, OpenAPI_granted_unit_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_granted_unit_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_granted_unit_convertToJSON() failed");
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

    OpenAPI_granted_unit_free(dst);
    dst = OpenAPI_granted_unit_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

