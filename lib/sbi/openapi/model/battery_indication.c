
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "battery_indication.h"

OpenAPI_battery_indication_t *OpenAPI_battery_indication_create(
    bool is_battery_ind,
    int battery_ind,
    bool is_replaceable_ind,
    int replaceable_ind,
    bool is_rechargeable_ind,
    int rechargeable_ind
)
{
    OpenAPI_battery_indication_t *battery_indication_local_var = ogs_malloc(sizeof(OpenAPI_battery_indication_t));
    ogs_assert(battery_indication_local_var);

    battery_indication_local_var->is_battery_ind = is_battery_ind;
    battery_indication_local_var->battery_ind = battery_ind;
    battery_indication_local_var->is_replaceable_ind = is_replaceable_ind;
    battery_indication_local_var->replaceable_ind = replaceable_ind;
    battery_indication_local_var->is_rechargeable_ind = is_rechargeable_ind;
    battery_indication_local_var->rechargeable_ind = rechargeable_ind;

    return battery_indication_local_var;
}

void OpenAPI_battery_indication_free(OpenAPI_battery_indication_t *battery_indication)
{
    if (NULL == battery_indication) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(battery_indication);
}

cJSON *OpenAPI_battery_indication_convertToJSON(OpenAPI_battery_indication_t *battery_indication)
{
    cJSON *item = NULL;

    if (battery_indication == NULL) {
        ogs_error("OpenAPI_battery_indication_convertToJSON() failed [BatteryIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (battery_indication->is_battery_ind) {
    if (cJSON_AddBoolToObject(item, "batteryInd", battery_indication->battery_ind) == NULL) {
        ogs_error("OpenAPI_battery_indication_convertToJSON() failed [battery_ind]");
        goto end;
    }
    }

    if (battery_indication->is_replaceable_ind) {
    if (cJSON_AddBoolToObject(item, "replaceableInd", battery_indication->replaceable_ind) == NULL) {
        ogs_error("OpenAPI_battery_indication_convertToJSON() failed [replaceable_ind]");
        goto end;
    }
    }

    if (battery_indication->is_rechargeable_ind) {
    if (cJSON_AddBoolToObject(item, "rechargeableInd", battery_indication->rechargeable_ind) == NULL) {
        ogs_error("OpenAPI_battery_indication_convertToJSON() failed [rechargeable_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_battery_indication_t *OpenAPI_battery_indication_parseFromJSON(cJSON *battery_indicationJSON)
{
    OpenAPI_battery_indication_t *battery_indication_local_var = NULL;
    cJSON *battery_ind = cJSON_GetObjectItemCaseSensitive(battery_indicationJSON, "batteryInd");

    if (battery_ind) {
    if (!cJSON_IsBool(battery_ind)) {
        ogs_error("OpenAPI_battery_indication_parseFromJSON() failed [battery_ind]");
        goto end;
    }
    }

    cJSON *replaceable_ind = cJSON_GetObjectItemCaseSensitive(battery_indicationJSON, "replaceableInd");

    if (replaceable_ind) {
    if (!cJSON_IsBool(replaceable_ind)) {
        ogs_error("OpenAPI_battery_indication_parseFromJSON() failed [replaceable_ind]");
        goto end;
    }
    }

    cJSON *rechargeable_ind = cJSON_GetObjectItemCaseSensitive(battery_indicationJSON, "rechargeableInd");

    if (rechargeable_ind) {
    if (!cJSON_IsBool(rechargeable_ind)) {
        ogs_error("OpenAPI_battery_indication_parseFromJSON() failed [rechargeable_ind]");
        goto end;
    }
    }

    battery_indication_local_var = OpenAPI_battery_indication_create (
        battery_ind ? true : false,
        battery_ind ? battery_ind->valueint : 0,
        replaceable_ind ? true : false,
        replaceable_ind ? replaceable_ind->valueint : 0,
        rechargeable_ind ? true : false,
        rechargeable_ind ? rechargeable_ind->valueint : 0
    );

    return battery_indication_local_var;
end:
    return NULL;
}

OpenAPI_battery_indication_t *OpenAPI_battery_indication_copy(OpenAPI_battery_indication_t *dst, OpenAPI_battery_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_battery_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_battery_indication_convertToJSON() failed");
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

    OpenAPI_battery_indication_free(dst);
    dst = OpenAPI_battery_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

