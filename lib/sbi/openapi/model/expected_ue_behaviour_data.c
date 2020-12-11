
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "expected_ue_behaviour_data.h"

OpenAPI_expected_ue_behaviour_data_t *OpenAPI_expected_ue_behaviour_data_create(
    OpenAPI_stationary_indication_e stationary_indication,
    int communication_duration_time,
    int periodic_time,
    OpenAPI_scheduled_communication_time_t *scheduled_communication_time,
    OpenAPI_scheduled_communication_type_e scheduled_communication_type,
    OpenAPI_list_t *expected_umts,
    OpenAPI_traffic_profile_e traffic_profile,
    OpenAPI_battery_indication_t *battery_indication,
    char *validity_time
    )
{
    OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_expected_ue_behaviour_data_t));
    if (!expected_ue_behaviour_data_local_var) {
        return NULL;
    }
    expected_ue_behaviour_data_local_var->stationary_indication = stationary_indication;
    expected_ue_behaviour_data_local_var->communication_duration_time = communication_duration_time;
    expected_ue_behaviour_data_local_var->periodic_time = periodic_time;
    expected_ue_behaviour_data_local_var->scheduled_communication_time = scheduled_communication_time;
    expected_ue_behaviour_data_local_var->scheduled_communication_type = scheduled_communication_type;
    expected_ue_behaviour_data_local_var->expected_umts = expected_umts;
    expected_ue_behaviour_data_local_var->traffic_profile = traffic_profile;
    expected_ue_behaviour_data_local_var->battery_indication = battery_indication;
    expected_ue_behaviour_data_local_var->validity_time = validity_time;

    return expected_ue_behaviour_data_local_var;
}

void OpenAPI_expected_ue_behaviour_data_free(OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_data)
{
    if (NULL == expected_ue_behaviour_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_scheduled_communication_time_free(expected_ue_behaviour_data->scheduled_communication_time);
    OpenAPI_list_for_each(expected_ue_behaviour_data->expected_umts, node) {
        OpenAPI_location_area_free(node->data);
    }
    OpenAPI_list_free(expected_ue_behaviour_data->expected_umts);
    OpenAPI_battery_indication_free(expected_ue_behaviour_data->battery_indication);
    ogs_free(expected_ue_behaviour_data->validity_time);
    ogs_free(expected_ue_behaviour_data);
}

cJSON *OpenAPI_expected_ue_behaviour_data_convertToJSON(OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_data)
{
    cJSON *item = NULL;

    if (expected_ue_behaviour_data == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [ExpectedUeBehaviourData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (expected_ue_behaviour_data->stationary_indication) {
        if (cJSON_AddStringToObject(item, "stationaryIndication", OpenAPI_stationary_indication_ToString(expected_ue_behaviour_data->stationary_indication)) == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [stationary_indication]");
            goto end;
        }
    }

    if (expected_ue_behaviour_data->communication_duration_time) {
        if (cJSON_AddNumberToObject(item, "communicationDurationTime", expected_ue_behaviour_data->communication_duration_time) == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [communication_duration_time]");
            goto end;
        }
    }

    if (expected_ue_behaviour_data->periodic_time) {
        if (cJSON_AddNumberToObject(item, "periodicTime", expected_ue_behaviour_data->periodic_time) == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [periodic_time]");
            goto end;
        }
    }

    if (expected_ue_behaviour_data->scheduled_communication_time) {
        cJSON *scheduled_communication_time_local_JSON = OpenAPI_scheduled_communication_time_convertToJSON(expected_ue_behaviour_data->scheduled_communication_time);
        if (scheduled_communication_time_local_JSON == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [scheduled_communication_time]");
            goto end;
        }
        cJSON_AddItemToObject(item, "scheduledCommunicationTime", scheduled_communication_time_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [scheduled_communication_time]");
            goto end;
        }
    }

    if (expected_ue_behaviour_data->scheduled_communication_type) {
        if (cJSON_AddStringToObject(item, "scheduledCommunicationType", OpenAPI_scheduled_communication_type_ToString(expected_ue_behaviour_data->scheduled_communication_type)) == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [scheduled_communication_type]");
            goto end;
        }
    }

    if (expected_ue_behaviour_data->expected_umts) {
        cJSON *expected_umtsList = cJSON_AddArrayToObject(item, "expectedUmts");
        if (expected_umtsList == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [expected_umts]");
            goto end;
        }

        OpenAPI_lnode_t *expected_umts_node;
        if (expected_ue_behaviour_data->expected_umts) {
            OpenAPI_list_for_each(expected_ue_behaviour_data->expected_umts, expected_umts_node) {
                cJSON *itemLocal = OpenAPI_location_area_convertToJSON(expected_umts_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [expected_umts]");
                    goto end;
                }
                cJSON_AddItemToArray(expected_umtsList, itemLocal);
            }
        }
    }

    if (expected_ue_behaviour_data->traffic_profile) {
        if (cJSON_AddStringToObject(item, "trafficProfile", OpenAPI_traffic_profile_ToString(expected_ue_behaviour_data->traffic_profile)) == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [traffic_profile]");
            goto end;
        }
    }

    if (expected_ue_behaviour_data->battery_indication) {
        cJSON *battery_indication_local_JSON = OpenAPI_battery_indication_convertToJSON(expected_ue_behaviour_data->battery_indication);
        if (battery_indication_local_JSON == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [battery_indication]");
            goto end;
        }
        cJSON_AddItemToObject(item, "batteryIndication", battery_indication_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [battery_indication]");
            goto end;
        }
    }

    if (expected_ue_behaviour_data->validity_time) {
        if (cJSON_AddStringToObject(item, "validityTime", expected_ue_behaviour_data->validity_time) == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed [validity_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_expected_ue_behaviour_data_t *OpenAPI_expected_ue_behaviour_data_parseFromJSON(cJSON *expected_ue_behaviour_dataJSON)
{
    OpenAPI_expected_ue_behaviour_data_t *expected_ue_behaviour_data_local_var = NULL;
    cJSON *stationary_indication = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "stationaryIndication");

    OpenAPI_stationary_indication_e stationary_indicationVariable;
    if (stationary_indication) {
        if (!cJSON_IsString(stationary_indication)) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON() failed [stationary_indication]");
            goto end;
        }
        stationary_indicationVariable = OpenAPI_stationary_indication_FromString(stationary_indication->valuestring);
    }

    cJSON *communication_duration_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "communicationDurationTime");

    if (communication_duration_time) {
        if (!cJSON_IsNumber(communication_duration_time)) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON() failed [communication_duration_time]");
            goto end;
        }
    }

    cJSON *periodic_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "periodicTime");

    if (periodic_time) {
        if (!cJSON_IsNumber(periodic_time)) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON() failed [periodic_time]");
            goto end;
        }
    }

    cJSON *scheduled_communication_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "scheduledCommunicationTime");

    OpenAPI_scheduled_communication_time_t *scheduled_communication_time_local_nonprim = NULL;
    if (scheduled_communication_time) {
        scheduled_communication_time_local_nonprim = OpenAPI_scheduled_communication_time_parseFromJSON(scheduled_communication_time);
    }

    cJSON *scheduled_communication_type = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "scheduledCommunicationType");

    OpenAPI_scheduled_communication_type_e scheduled_communication_typeVariable;
    if (scheduled_communication_type) {
        if (!cJSON_IsString(scheduled_communication_type)) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON() failed [scheduled_communication_type]");
            goto end;
        }
        scheduled_communication_typeVariable = OpenAPI_scheduled_communication_type_FromString(scheduled_communication_type->valuestring);
    }

    cJSON *expected_umts = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "expectedUmts");

    OpenAPI_list_t *expected_umtsList;
    if (expected_umts) {
        cJSON *expected_umts_local_nonprimitive;
        if (!cJSON_IsArray(expected_umts)) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON() failed [expected_umts]");
            goto end;
        }

        expected_umtsList = OpenAPI_list_create();

        cJSON_ArrayForEach(expected_umts_local_nonprimitive, expected_umts ) {
            if (!cJSON_IsObject(expected_umts_local_nonprimitive)) {
                ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON() failed [expected_umts]");
                goto end;
            }
            OpenAPI_location_area_t *expected_umtsItem = OpenAPI_location_area_parseFromJSON(expected_umts_local_nonprimitive);

            OpenAPI_list_add(expected_umtsList, expected_umtsItem);
        }
    }

    cJSON *traffic_profile = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "trafficProfile");

    OpenAPI_traffic_profile_e traffic_profileVariable;
    if (traffic_profile) {
        if (!cJSON_IsString(traffic_profile)) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON() failed [traffic_profile]");
            goto end;
        }
        traffic_profileVariable = OpenAPI_traffic_profile_FromString(traffic_profile->valuestring);
    }

    cJSON *battery_indication = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "batteryIndication");

    OpenAPI_battery_indication_t *battery_indication_local_nonprim = NULL;
    if (battery_indication) {
        battery_indication_local_nonprim = OpenAPI_battery_indication_parseFromJSON(battery_indication);
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_dataJSON, "validityTime");

    if (validity_time) {
        if (!cJSON_IsString(validity_time)) {
            ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON() failed [validity_time]");
            goto end;
        }
    }

    expected_ue_behaviour_data_local_var = OpenAPI_expected_ue_behaviour_data_create (
        stationary_indication ? stationary_indicationVariable : 0,
        communication_duration_time ? communication_duration_time->valuedouble : 0,
        periodic_time ? periodic_time->valuedouble : 0,
        scheduled_communication_time ? scheduled_communication_time_local_nonprim : NULL,
        scheduled_communication_type ? scheduled_communication_typeVariable : 0,
        expected_umts ? expected_umtsList : NULL,
        traffic_profile ? traffic_profileVariable : 0,
        battery_indication ? battery_indication_local_nonprim : NULL,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL
        );

    return expected_ue_behaviour_data_local_var;
end:
    return NULL;
}

OpenAPI_expected_ue_behaviour_data_t *OpenAPI_expected_ue_behaviour_data_copy(OpenAPI_expected_ue_behaviour_data_t *dst, OpenAPI_expected_ue_behaviour_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_expected_ue_behaviour_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_expected_ue_behaviour_data_convertToJSON() failed");
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

    OpenAPI_expected_ue_behaviour_data_free(dst);
    dst = OpenAPI_expected_ue_behaviour_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

