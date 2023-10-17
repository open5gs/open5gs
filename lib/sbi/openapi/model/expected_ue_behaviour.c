
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "expected_ue_behaviour.h"

OpenAPI_expected_ue_behaviour_t *OpenAPI_expected_ue_behaviour_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_stationary_indication_rm_t *stationary_indication,
    bool is_communication_duration_time_null,
    bool is_communication_duration_time,
    int communication_duration_time,
    OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type,
    bool is_periodic_time_null,
    bool is_periodic_time,
    int periodic_time,
    OpenAPI_scheduled_communication_time_rm_t *scheduled_communication_time,
    bool is_expected_umts_null,
    OpenAPI_list_t *expected_umts,
    OpenAPI_traffic_profile_rm_t *traffic_profile,
    OpenAPI_battery_indication_rm_t *battery_indication,
    char *validity_time,
    char *mtc_provider_information
)
{
    OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour_local_var = ogs_malloc(sizeof(OpenAPI_expected_ue_behaviour_t));
    ogs_assert(expected_ue_behaviour_local_var);

    expected_ue_behaviour_local_var->af_instance_id = af_instance_id;
    expected_ue_behaviour_local_var->reference_id = reference_id;
    expected_ue_behaviour_local_var->stationary_indication = stationary_indication;
    expected_ue_behaviour_local_var->is_communication_duration_time_null = is_communication_duration_time_null;
    expected_ue_behaviour_local_var->is_communication_duration_time = is_communication_duration_time;
    expected_ue_behaviour_local_var->communication_duration_time = communication_duration_time;
    expected_ue_behaviour_local_var->scheduled_communication_type = scheduled_communication_type;
    expected_ue_behaviour_local_var->is_periodic_time_null = is_periodic_time_null;
    expected_ue_behaviour_local_var->is_periodic_time = is_periodic_time;
    expected_ue_behaviour_local_var->periodic_time = periodic_time;
    expected_ue_behaviour_local_var->scheduled_communication_time = scheduled_communication_time;
    expected_ue_behaviour_local_var->is_expected_umts_null = is_expected_umts_null;
    expected_ue_behaviour_local_var->expected_umts = expected_umts;
    expected_ue_behaviour_local_var->traffic_profile = traffic_profile;
    expected_ue_behaviour_local_var->battery_indication = battery_indication;
    expected_ue_behaviour_local_var->validity_time = validity_time;
    expected_ue_behaviour_local_var->mtc_provider_information = mtc_provider_information;

    return expected_ue_behaviour_local_var;
}

void OpenAPI_expected_ue_behaviour_free(OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == expected_ue_behaviour) {
        return;
    }
    if (expected_ue_behaviour->af_instance_id) {
        ogs_free(expected_ue_behaviour->af_instance_id);
        expected_ue_behaviour->af_instance_id = NULL;
    }
    if (expected_ue_behaviour->stationary_indication) {
        OpenAPI_stationary_indication_rm_free(expected_ue_behaviour->stationary_indication);
        expected_ue_behaviour->stationary_indication = NULL;
    }
    if (expected_ue_behaviour->scheduled_communication_type) {
        OpenAPI_scheduled_communication_type_rm_free(expected_ue_behaviour->scheduled_communication_type);
        expected_ue_behaviour->scheduled_communication_type = NULL;
    }
    if (expected_ue_behaviour->scheduled_communication_time) {
        OpenAPI_scheduled_communication_time_rm_free(expected_ue_behaviour->scheduled_communication_time);
        expected_ue_behaviour->scheduled_communication_time = NULL;
    }
    if (expected_ue_behaviour->expected_umts) {
        OpenAPI_list_for_each(expected_ue_behaviour->expected_umts, node) {
            OpenAPI_location_area_free(node->data);
        }
        OpenAPI_list_free(expected_ue_behaviour->expected_umts);
        expected_ue_behaviour->expected_umts = NULL;
    }
    if (expected_ue_behaviour->traffic_profile) {
        OpenAPI_traffic_profile_rm_free(expected_ue_behaviour->traffic_profile);
        expected_ue_behaviour->traffic_profile = NULL;
    }
    if (expected_ue_behaviour->battery_indication) {
        OpenAPI_battery_indication_rm_free(expected_ue_behaviour->battery_indication);
        expected_ue_behaviour->battery_indication = NULL;
    }
    if (expected_ue_behaviour->validity_time) {
        ogs_free(expected_ue_behaviour->validity_time);
        expected_ue_behaviour->validity_time = NULL;
    }
    if (expected_ue_behaviour->mtc_provider_information) {
        ogs_free(expected_ue_behaviour->mtc_provider_information);
        expected_ue_behaviour->mtc_provider_information = NULL;
    }
    ogs_free(expected_ue_behaviour);
}

cJSON *OpenAPI_expected_ue_behaviour_convertToJSON(OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (expected_ue_behaviour == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [ExpectedUeBehaviour]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!expected_ue_behaviour->af_instance_id) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [af_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", expected_ue_behaviour->af_instance_id) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "referenceId", expected_ue_behaviour->reference_id) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (expected_ue_behaviour->stationary_indication) {
    cJSON *stationary_indication_local_JSON = OpenAPI_stationary_indication_rm_convertToJSON(expected_ue_behaviour->stationary_indication);
    if (stationary_indication_local_JSON == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [stationary_indication]");
        goto end;
    }
    cJSON_AddItemToObject(item, "stationaryIndication", stationary_indication_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [stationary_indication]");
        goto end;
    }
    }

    if (expected_ue_behaviour->is_communication_duration_time) {
    if (cJSON_AddNumberToObject(item, "communicationDurationTime", expected_ue_behaviour->communication_duration_time) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [communication_duration_time]");
        goto end;
    }
    } else if (expected_ue_behaviour->is_communication_duration_time_null) {
        if (cJSON_AddNullToObject(item, "communicationDurationTime") == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [communication_duration_time]");
            goto end;
        }
    }

    if (expected_ue_behaviour->scheduled_communication_type) {
    cJSON *scheduled_communication_type_local_JSON = OpenAPI_scheduled_communication_type_rm_convertToJSON(expected_ue_behaviour->scheduled_communication_type);
    if (scheduled_communication_type_local_JSON == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [scheduled_communication_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "scheduledCommunicationType", scheduled_communication_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [scheduled_communication_type]");
        goto end;
    }
    }

    if (expected_ue_behaviour->is_periodic_time) {
    if (cJSON_AddNumberToObject(item, "periodicTime", expected_ue_behaviour->periodic_time) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [periodic_time]");
        goto end;
    }
    } else if (expected_ue_behaviour->is_periodic_time_null) {
        if (cJSON_AddNullToObject(item, "periodicTime") == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [periodic_time]");
            goto end;
        }
    }

    if (expected_ue_behaviour->scheduled_communication_time) {
    cJSON *scheduled_communication_time_local_JSON = OpenAPI_scheduled_communication_time_rm_convertToJSON(expected_ue_behaviour->scheduled_communication_time);
    if (scheduled_communication_time_local_JSON == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [scheduled_communication_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "scheduledCommunicationTime", scheduled_communication_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [scheduled_communication_time]");
        goto end;
    }
    }

    if (expected_ue_behaviour->expected_umts) {
    cJSON *expected_umtsList = cJSON_AddArrayToObject(item, "expectedUmts");
    if (expected_umtsList == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [expected_umts]");
        goto end;
    }
    OpenAPI_list_for_each(expected_ue_behaviour->expected_umts, node) {
        cJSON *itemLocal = OpenAPI_location_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [expected_umts]");
            goto end;
        }
        cJSON_AddItemToArray(expected_umtsList, itemLocal);
    }
    } else if (expected_ue_behaviour->is_expected_umts_null) {
        if (cJSON_AddNullToObject(item, "expectedUmts") == NULL) {
            ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [expected_umts]");
            goto end;
        }
    }

    if (expected_ue_behaviour->traffic_profile) {
    cJSON *traffic_profile_local_JSON = OpenAPI_traffic_profile_rm_convertToJSON(expected_ue_behaviour->traffic_profile);
    if (traffic_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [traffic_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trafficProfile", traffic_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [traffic_profile]");
        goto end;
    }
    }

    if (expected_ue_behaviour->battery_indication) {
    cJSON *battery_indication_local_JSON = OpenAPI_battery_indication_rm_convertToJSON(expected_ue_behaviour->battery_indication);
    if (battery_indication_local_JSON == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [battery_indication]");
        goto end;
    }
    cJSON_AddItemToObject(item, "batteryIndication", battery_indication_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [battery_indication]");
        goto end;
    }
    }

    if (expected_ue_behaviour->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", expected_ue_behaviour->validity_time) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (expected_ue_behaviour->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", expected_ue_behaviour->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_expected_ue_behaviour_t *OpenAPI_expected_ue_behaviour_parseFromJSON(cJSON *expected_ue_behaviourJSON)
{
    OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *reference_id = NULL;
    cJSON *stationary_indication = NULL;
    OpenAPI_stationary_indication_rm_t *stationary_indication_local_nonprim = NULL;
    cJSON *communication_duration_time = NULL;
    cJSON *scheduled_communication_type = NULL;
    OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type_local_nonprim = NULL;
    cJSON *periodic_time = NULL;
    cJSON *scheduled_communication_time = NULL;
    OpenAPI_scheduled_communication_time_rm_t *scheduled_communication_time_local_nonprim = NULL;
    cJSON *expected_umts = NULL;
    OpenAPI_list_t *expected_umtsList = NULL;
    cJSON *traffic_profile = NULL;
    OpenAPI_traffic_profile_rm_t *traffic_profile_local_nonprim = NULL;
    cJSON *battery_indication = NULL;
    OpenAPI_battery_indication_rm_t *battery_indication_local_nonprim = NULL;
    cJSON *validity_time = NULL;
    cJSON *mtc_provider_information = NULL;
    af_instance_id = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [reference_id]");
        goto end;
    }
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [reference_id]");
        goto end;
    }

    stationary_indication = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "stationaryIndication");
    if (stationary_indication) {
    stationary_indication_local_nonprim = OpenAPI_stationary_indication_rm_parseFromJSON(stationary_indication);
    if (!stationary_indication_local_nonprim) {
        ogs_error("OpenAPI_stationary_indication_rm_parseFromJSON failed [stationary_indication]");
        goto end;
    }
    }

    communication_duration_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "communicationDurationTime");
    if (communication_duration_time) {
    if (!cJSON_IsNull(communication_duration_time)) {
    if (!cJSON_IsNumber(communication_duration_time)) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [communication_duration_time]");
        goto end;
    }
    }
    }

    scheduled_communication_type = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "scheduledCommunicationType");
    if (scheduled_communication_type) {
    scheduled_communication_type_local_nonprim = OpenAPI_scheduled_communication_type_rm_parseFromJSON(scheduled_communication_type);
    if (!scheduled_communication_type_local_nonprim) {
        ogs_error("OpenAPI_scheduled_communication_type_rm_parseFromJSON failed [scheduled_communication_type]");
        goto end;
    }
    }

    periodic_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "periodicTime");
    if (periodic_time) {
    if (!cJSON_IsNull(periodic_time)) {
    if (!cJSON_IsNumber(periodic_time)) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [periodic_time]");
        goto end;
    }
    }
    }

    scheduled_communication_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "scheduledCommunicationTime");
    if (scheduled_communication_time) {
    scheduled_communication_time_local_nonprim = OpenAPI_scheduled_communication_time_rm_parseFromJSON(scheduled_communication_time);
    if (!scheduled_communication_time_local_nonprim) {
        ogs_error("OpenAPI_scheduled_communication_time_rm_parseFromJSON failed [scheduled_communication_time]");
        goto end;
    }
    }

    expected_umts = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "expectedUmts");
    if (expected_umts) {
    if (!cJSON_IsNull(expected_umts)) {
        cJSON *expected_umts_local = NULL;
        if (!cJSON_IsArray(expected_umts)) {
            ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [expected_umts]");
            goto end;
        }

        expected_umtsList = OpenAPI_list_create();

        cJSON_ArrayForEach(expected_umts_local, expected_umts) {
            if (!cJSON_IsObject(expected_umts_local)) {
                ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [expected_umts]");
                goto end;
            }
            OpenAPI_location_area_t *expected_umtsItem = OpenAPI_location_area_parseFromJSON(expected_umts_local);
            if (!expected_umtsItem) {
                ogs_error("No expected_umtsItem");
                goto end;
            }
            OpenAPI_list_add(expected_umtsList, expected_umtsItem);
        }
    }
    }

    traffic_profile = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "trafficProfile");
    if (traffic_profile) {
    traffic_profile_local_nonprim = OpenAPI_traffic_profile_rm_parseFromJSON(traffic_profile);
    if (!traffic_profile_local_nonprim) {
        ogs_error("OpenAPI_traffic_profile_rm_parseFromJSON failed [traffic_profile]");
        goto end;
    }
    }

    battery_indication = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "batteryIndication");
    if (battery_indication) {
    battery_indication_local_nonprim = OpenAPI_battery_indication_rm_parseFromJSON(battery_indication);
    if (!battery_indication_local_nonprim) {
        ogs_error("OpenAPI_battery_indication_rm_parseFromJSON failed [battery_indication]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviourJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    expected_ue_behaviour_local_var = OpenAPI_expected_ue_behaviour_create (
        ogs_strdup(af_instance_id->valuestring),
        
        reference_id->valuedouble,
        stationary_indication ? stationary_indication_local_nonprim : NULL,
        communication_duration_time && cJSON_IsNull(communication_duration_time) ? true : false,
        communication_duration_time ? true : false,
        communication_duration_time ? communication_duration_time->valuedouble : 0,
        scheduled_communication_type ? scheduled_communication_type_local_nonprim : NULL,
        periodic_time && cJSON_IsNull(periodic_time) ? true : false,
        periodic_time ? true : false,
        periodic_time ? periodic_time->valuedouble : 0,
        scheduled_communication_time ? scheduled_communication_time_local_nonprim : NULL,
        expected_umts && cJSON_IsNull(expected_umts) ? true : false,
        expected_umts ? expected_umtsList : NULL,
        traffic_profile ? traffic_profile_local_nonprim : NULL,
        battery_indication ? battery_indication_local_nonprim : NULL,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL
    );

    return expected_ue_behaviour_local_var;
end:
    if (stationary_indication_local_nonprim) {
        OpenAPI_stationary_indication_rm_free(stationary_indication_local_nonprim);
        stationary_indication_local_nonprim = NULL;
    }
    if (scheduled_communication_type_local_nonprim) {
        OpenAPI_scheduled_communication_type_rm_free(scheduled_communication_type_local_nonprim);
        scheduled_communication_type_local_nonprim = NULL;
    }
    if (scheduled_communication_time_local_nonprim) {
        OpenAPI_scheduled_communication_time_rm_free(scheduled_communication_time_local_nonprim);
        scheduled_communication_time_local_nonprim = NULL;
    }
    if (expected_umtsList) {
        OpenAPI_list_for_each(expected_umtsList, node) {
            OpenAPI_location_area_free(node->data);
        }
        OpenAPI_list_free(expected_umtsList);
        expected_umtsList = NULL;
    }
    if (traffic_profile_local_nonprim) {
        OpenAPI_traffic_profile_rm_free(traffic_profile_local_nonprim);
        traffic_profile_local_nonprim = NULL;
    }
    if (battery_indication_local_nonprim) {
        OpenAPI_battery_indication_rm_free(battery_indication_local_nonprim);
        battery_indication_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_expected_ue_behaviour_t *OpenAPI_expected_ue_behaviour_copy(OpenAPI_expected_ue_behaviour_t *dst, OpenAPI_expected_ue_behaviour_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_expected_ue_behaviour_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_expected_ue_behaviour_convertToJSON() failed");
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

    OpenAPI_expected_ue_behaviour_free(dst);
    dst = OpenAPI_expected_ue_behaviour_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

