
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "battery_indication_rm.h"

OpenAPI_battery_indication_rm_t *OpenAPI_battery_indication_rm_create(
    bool is_battery_ind,
    int battery_ind,
    bool is_replaceable_ind,
    int replaceable_ind,
    bool is_rechargeable_ind,
    int rechargeable_ind
)
{
    OpenAPI_battery_indication_rm_t *battery_indication_rm_local_var = ogs_malloc(sizeof(OpenAPI_battery_indication_rm_t));
    ogs_assert(battery_indication_rm_local_var);

    battery_indication_rm_local_var->is_battery_ind = is_battery_ind;
    battery_indication_rm_local_var->battery_ind = battery_ind;
    battery_indication_rm_local_var->is_replaceable_ind = is_replaceable_ind;
    battery_indication_rm_local_var->replaceable_ind = replaceable_ind;
    battery_indication_rm_local_var->is_rechargeable_ind = is_rechargeable_ind;
    battery_indication_rm_local_var->rechargeable_ind = rechargeable_ind;

    return battery_indication_rm_local_var;
}

void OpenAPI_battery_indication_rm_free(OpenAPI_battery_indication_rm_t *battery_indication_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == battery_indication_rm) {
        return;
    }
    ogs_free(battery_indication_rm);
}

cJSON *OpenAPI_battery_indication_rm_convertToJSON(OpenAPI_battery_indication_rm_t *battery_indication_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (battery_indication_rm == NULL) {
        ogs_error("OpenAPI_battery_indication_rm_convertToJSON() failed [BatteryIndicationRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (battery_indication_rm->is_battery_ind) {
    if (cJSON_AddBoolToObject(item, "batteryInd", battery_indication_rm->battery_ind) == NULL) {
        ogs_error("OpenAPI_battery_indication_rm_convertToJSON() failed [battery_ind]");
        goto end;
    }
    }

    if (battery_indication_rm->is_replaceable_ind) {
    if (cJSON_AddBoolToObject(item, "replaceableInd", battery_indication_rm->replaceable_ind) == NULL) {
        ogs_error("OpenAPI_battery_indication_rm_convertToJSON() failed [replaceable_ind]");
        goto end;
    }
    }

    if (battery_indication_rm->is_rechargeable_ind) {
    if (cJSON_AddBoolToObject(item, "rechargeableInd", battery_indication_rm->rechargeable_ind) == NULL) {
        ogs_error("OpenAPI_battery_indication_rm_convertToJSON() failed [rechargeable_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_battery_indication_rm_t *OpenAPI_battery_indication_rm_parseFromJSON(cJSON *battery_indication_rmJSON)
{
    OpenAPI_battery_indication_rm_t *battery_indication_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *battery_ind = NULL;
    cJSON *replaceable_ind = NULL;
    cJSON *rechargeable_ind = NULL;
    battery_ind = cJSON_GetObjectItemCaseSensitive(battery_indication_rmJSON, "batteryInd");
    if (battery_ind) {
    if (!cJSON_IsBool(battery_ind)) {
        ogs_error("OpenAPI_battery_indication_rm_parseFromJSON() failed [battery_ind]");
        goto end;
    }
    }

    replaceable_ind = cJSON_GetObjectItemCaseSensitive(battery_indication_rmJSON, "replaceableInd");
    if (replaceable_ind) {
    if (!cJSON_IsBool(replaceable_ind)) {
        ogs_error("OpenAPI_battery_indication_rm_parseFromJSON() failed [replaceable_ind]");
        goto end;
    }
    }

    rechargeable_ind = cJSON_GetObjectItemCaseSensitive(battery_indication_rmJSON, "rechargeableInd");
    if (rechargeable_ind) {
    if (!cJSON_IsBool(rechargeable_ind)) {
        ogs_error("OpenAPI_battery_indication_rm_parseFromJSON() failed [rechargeable_ind]");
        goto end;
    }
    }

    battery_indication_rm_local_var = OpenAPI_battery_indication_rm_create (
        battery_ind ? true : false,
        battery_ind ? battery_ind->valueint : 0,
        replaceable_ind ? true : false,
        replaceable_ind ? replaceable_ind->valueint : 0,
        rechargeable_ind ? true : false,
        rechargeable_ind ? rechargeable_ind->valueint : 0
    );

    return battery_indication_rm_local_var;
end:
    return NULL;
}

OpenAPI_battery_indication_rm_t *OpenAPI_battery_indication_rm_copy(OpenAPI_battery_indication_rm_t *dst, OpenAPI_battery_indication_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_battery_indication_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_battery_indication_rm_convertToJSON() failed");
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

    OpenAPI_battery_indication_rm_free(dst);
    dst = OpenAPI_battery_indication_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

