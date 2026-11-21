
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eas_deployment_charging_information.h"

OpenAPI_eas_deployment_charging_information_t *OpenAPI_eas_deployment_charging_information_create(
    OpenAPI_eas_requirements_t *e_eas_deployment_requirements,
    OpenAPI_management_operation_e l_cm_event_type,
    char *l_cm_start_time,
    char *l_cm_end_time,
    OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information
)
{
    OpenAPI_eas_deployment_charging_information_t *eas_deployment_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_eas_deployment_charging_information_t));
    ogs_assert(eas_deployment_charging_information_local_var);

    eas_deployment_charging_information_local_var->e_eas_deployment_requirements = e_eas_deployment_requirements;
    eas_deployment_charging_information_local_var->l_cm_event_type = l_cm_event_type;
    eas_deployment_charging_information_local_var->l_cm_start_time = l_cm_start_time;
    eas_deployment_charging_information_local_var->l_cm_end_time = l_cm_end_time;
    eas_deployment_charging_information_local_var->satellite_backhaul_information = satellite_backhaul_information;

    return eas_deployment_charging_information_local_var;
}

void OpenAPI_eas_deployment_charging_information_free(OpenAPI_eas_deployment_charging_information_t *eas_deployment_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eas_deployment_charging_information) {
        return;
    }
    if (eas_deployment_charging_information->e_eas_deployment_requirements) {
        OpenAPI_eas_requirements_free(eas_deployment_charging_information->e_eas_deployment_requirements);
        eas_deployment_charging_information->e_eas_deployment_requirements = NULL;
    }
    if (eas_deployment_charging_information->l_cm_start_time) {
        ogs_free(eas_deployment_charging_information->l_cm_start_time);
        eas_deployment_charging_information->l_cm_start_time = NULL;
    }
    if (eas_deployment_charging_information->l_cm_end_time) {
        ogs_free(eas_deployment_charging_information->l_cm_end_time);
        eas_deployment_charging_information->l_cm_end_time = NULL;
    }
    if (eas_deployment_charging_information->satellite_backhaul_information) {
        OpenAPI_satellite_backhaul_information_free(eas_deployment_charging_information->satellite_backhaul_information);
        eas_deployment_charging_information->satellite_backhaul_information = NULL;
    }
    ogs_free(eas_deployment_charging_information);
}

cJSON *OpenAPI_eas_deployment_charging_information_convertToJSON(OpenAPI_eas_deployment_charging_information_t *eas_deployment_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eas_deployment_charging_information == NULL) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed [EASDeploymentChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (eas_deployment_charging_information->e_eas_deployment_requirements) {
    cJSON *e_eas_deployment_requirements_local_JSON = OpenAPI_eas_requirements_convertToJSON(eas_deployment_charging_information->e_eas_deployment_requirements);
    if (e_eas_deployment_requirements_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed [e_eas_deployment_requirements]");
        goto end;
    }
    cJSON_AddItemToObject(item, "eEASDeploymentRequirements", e_eas_deployment_requirements_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed [e_eas_deployment_requirements]");
        goto end;
    }
    }

    if (eas_deployment_charging_information->l_cm_event_type != OpenAPI_management_operation_NULL) {
    if (cJSON_AddStringToObject(item, "lCMEventType", OpenAPI_management_operation_ToString(eas_deployment_charging_information->l_cm_event_type)) == NULL) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed [l_cm_event_type]");
        goto end;
    }
    }

    if (eas_deployment_charging_information->l_cm_start_time) {
    if (cJSON_AddStringToObject(item, "lCMStartTime", eas_deployment_charging_information->l_cm_start_time) == NULL) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed [l_cm_start_time]");
        goto end;
    }
    }

    if (eas_deployment_charging_information->l_cm_end_time) {
    if (cJSON_AddStringToObject(item, "lCMEndTime", eas_deployment_charging_information->l_cm_end_time) == NULL) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed [l_cm_end_time]");
        goto end;
    }
    }

    if (eas_deployment_charging_information->satellite_backhaul_information) {
    cJSON *satellite_backhaul_information_local_JSON = OpenAPI_satellite_backhaul_information_convertToJSON(eas_deployment_charging_information->satellite_backhaul_information);
    if (satellite_backhaul_information_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed [satellite_backhaul_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "satelliteBackhaulInformation", satellite_backhaul_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed [satellite_backhaul_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_eas_deployment_charging_information_t *OpenAPI_eas_deployment_charging_information_parseFromJSON(cJSON *eas_deployment_charging_informationJSON)
{
    OpenAPI_eas_deployment_charging_information_t *eas_deployment_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *e_eas_deployment_requirements = NULL;
    OpenAPI_eas_requirements_t *e_eas_deployment_requirements_local_nonprim = NULL;
    cJSON *l_cm_event_type = NULL;
    OpenAPI_management_operation_e l_cm_event_typeVariable = 0;
    cJSON *l_cm_start_time = NULL;
    cJSON *l_cm_end_time = NULL;
    cJSON *satellite_backhaul_information = NULL;
    OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information_local_nonprim = NULL;
    e_eas_deployment_requirements = cJSON_GetObjectItemCaseSensitive(eas_deployment_charging_informationJSON, "eEASDeploymentRequirements");
    if (e_eas_deployment_requirements) {
    e_eas_deployment_requirements_local_nonprim = OpenAPI_eas_requirements_parseFromJSON(e_eas_deployment_requirements);
    if (!e_eas_deployment_requirements_local_nonprim) {
        ogs_error("OpenAPI_eas_requirements_parseFromJSON failed [e_eas_deployment_requirements]");
        goto end;
    }
    }

    l_cm_event_type = cJSON_GetObjectItemCaseSensitive(eas_deployment_charging_informationJSON, "lCMEventType");
    if (l_cm_event_type) {
    if (!cJSON_IsString(l_cm_event_type)) {
        ogs_error("OpenAPI_eas_deployment_charging_information_parseFromJSON() failed [l_cm_event_type]");
        goto end;
    }
    l_cm_event_typeVariable = OpenAPI_management_operation_FromString(l_cm_event_type->valuestring);
    }

    l_cm_start_time = cJSON_GetObjectItemCaseSensitive(eas_deployment_charging_informationJSON, "lCMStartTime");
    if (l_cm_start_time) {
    if (!cJSON_IsString(l_cm_start_time) && !cJSON_IsNull(l_cm_start_time)) {
        ogs_error("OpenAPI_eas_deployment_charging_information_parseFromJSON() failed [l_cm_start_time]");
        goto end;
    }
    }

    l_cm_end_time = cJSON_GetObjectItemCaseSensitive(eas_deployment_charging_informationJSON, "lCMEndTime");
    if (l_cm_end_time) {
    if (!cJSON_IsString(l_cm_end_time) && !cJSON_IsNull(l_cm_end_time)) {
        ogs_error("OpenAPI_eas_deployment_charging_information_parseFromJSON() failed [l_cm_end_time]");
        goto end;
    }
    }

    satellite_backhaul_information = cJSON_GetObjectItemCaseSensitive(eas_deployment_charging_informationJSON, "satelliteBackhaulInformation");
    if (satellite_backhaul_information) {
    satellite_backhaul_information_local_nonprim = OpenAPI_satellite_backhaul_information_parseFromJSON(satellite_backhaul_information);
    if (!satellite_backhaul_information_local_nonprim) {
        ogs_error("OpenAPI_satellite_backhaul_information_parseFromJSON failed [satellite_backhaul_information]");
        goto end;
    }
    }

    eas_deployment_charging_information_local_var = OpenAPI_eas_deployment_charging_information_create (
        e_eas_deployment_requirements ? e_eas_deployment_requirements_local_nonprim : NULL,
        l_cm_event_type ? l_cm_event_typeVariable : 0,
        l_cm_start_time && !cJSON_IsNull(l_cm_start_time) ? ogs_strdup(l_cm_start_time->valuestring) : NULL,
        l_cm_end_time && !cJSON_IsNull(l_cm_end_time) ? ogs_strdup(l_cm_end_time->valuestring) : NULL,
        satellite_backhaul_information ? satellite_backhaul_information_local_nonprim : NULL
    );

    return eas_deployment_charging_information_local_var;
end:
    if (e_eas_deployment_requirements_local_nonprim) {
        OpenAPI_eas_requirements_free(e_eas_deployment_requirements_local_nonprim);
        e_eas_deployment_requirements_local_nonprim = NULL;
    }
    if (satellite_backhaul_information_local_nonprim) {
        OpenAPI_satellite_backhaul_information_free(satellite_backhaul_information_local_nonprim);
        satellite_backhaul_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_eas_deployment_charging_information_t *OpenAPI_eas_deployment_charging_information_copy(OpenAPI_eas_deployment_charging_information_t *dst, OpenAPI_eas_deployment_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eas_deployment_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eas_deployment_charging_information_convertToJSON() failed");
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

    OpenAPI_eas_deployment_charging_information_free(dst);
    dst = OpenAPI_eas_deployment_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

