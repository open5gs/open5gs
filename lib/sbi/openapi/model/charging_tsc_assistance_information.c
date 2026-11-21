
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_tsc_assistance_information.h"

OpenAPI_charging_tsc_assistance_information_t *OpenAPI_charging_tsc_assistance_information_create(
    OpenAPI_tsc_flow_direction_e flow_direction,
    bool is_periodicity,
    int periodicity
)
{
    OpenAPI_charging_tsc_assistance_information_t *charging_tsc_assistance_information_local_var = ogs_malloc(sizeof(OpenAPI_charging_tsc_assistance_information_t));
    ogs_assert(charging_tsc_assistance_information_local_var);

    charging_tsc_assistance_information_local_var->flow_direction = flow_direction;
    charging_tsc_assistance_information_local_var->is_periodicity = is_periodicity;
    charging_tsc_assistance_information_local_var->periodicity = periodicity;

    return charging_tsc_assistance_information_local_var;
}

void OpenAPI_charging_tsc_assistance_information_free(OpenAPI_charging_tsc_assistance_information_t *charging_tsc_assistance_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_tsc_assistance_information) {
        return;
    }
    ogs_free(charging_tsc_assistance_information);
}

cJSON *OpenAPI_charging_tsc_assistance_information_convertToJSON(OpenAPI_charging_tsc_assistance_information_t *charging_tsc_assistance_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_tsc_assistance_information == NULL) {
        ogs_error("OpenAPI_charging_tsc_assistance_information_convertToJSON() failed [ChargingTSCAssistanceInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_tsc_assistance_information->flow_direction != OpenAPI_tsc_flow_direction_NULL) {
    if (cJSON_AddStringToObject(item, "flowDirection", OpenAPI_tsc_flow_direction_ToString(charging_tsc_assistance_information->flow_direction)) == NULL) {
        ogs_error("OpenAPI_charging_tsc_assistance_information_convertToJSON() failed [flow_direction]");
        goto end;
    }
    }

    if (charging_tsc_assistance_information->is_periodicity) {
    if (cJSON_AddNumberToObject(item, "periodicity", charging_tsc_assistance_information->periodicity) == NULL) {
        ogs_error("OpenAPI_charging_tsc_assistance_information_convertToJSON() failed [periodicity]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_tsc_assistance_information_t *OpenAPI_charging_tsc_assistance_information_parseFromJSON(cJSON *charging_tsc_assistance_informationJSON)
{
    OpenAPI_charging_tsc_assistance_information_t *charging_tsc_assistance_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flow_direction = NULL;
    OpenAPI_tsc_flow_direction_e flow_directionVariable = 0;
    cJSON *periodicity = NULL;
    flow_direction = cJSON_GetObjectItemCaseSensitive(charging_tsc_assistance_informationJSON, "flowDirection");
    if (flow_direction) {
    if (!cJSON_IsString(flow_direction)) {
        ogs_error("OpenAPI_charging_tsc_assistance_information_parseFromJSON() failed [flow_direction]");
        goto end;
    }
    flow_directionVariable = OpenAPI_tsc_flow_direction_FromString(flow_direction->valuestring);
    }

    periodicity = cJSON_GetObjectItemCaseSensitive(charging_tsc_assistance_informationJSON, "periodicity");
    if (periodicity) {
    if (!cJSON_IsNumber(periodicity)) {
        ogs_error("OpenAPI_charging_tsc_assistance_information_parseFromJSON() failed [periodicity]");
        goto end;
    }
    }

    charging_tsc_assistance_information_local_var = OpenAPI_charging_tsc_assistance_information_create (
        flow_direction ? flow_directionVariable : 0,
        periodicity ? true : false,
        periodicity ? periodicity->valuedouble : 0
    );

    return charging_tsc_assistance_information_local_var;
end:
    return NULL;
}

OpenAPI_charging_tsc_assistance_information_t *OpenAPI_charging_tsc_assistance_information_copy(OpenAPI_charging_tsc_assistance_information_t *dst, OpenAPI_charging_tsc_assistance_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_tsc_assistance_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_tsc_assistance_information_convertToJSON() failed");
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

    OpenAPI_charging_tsc_assistance_information_free(dst);
    dst = OpenAPI_charging_tsc_assistance_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

