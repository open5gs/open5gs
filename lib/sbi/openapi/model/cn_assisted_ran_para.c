
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cn_assisted_ran_para.h"

OpenAPI_cn_assisted_ran_para_t *OpenAPI_cn_assisted_ran_para_create(
    OpenAPI_stationary_indication_e stationary_indication,
    int communication_duration_time,
    int periodic_time,
    OpenAPI_scheduled_communication_time_t *scheduled_communication_time,
    OpenAPI_scheduled_communication_type_e scheduled_communication_type,
    OpenAPI_traffic_profile_e traffic_profile,
    OpenAPI_battery_indication_t *battery_indication
    )
{
    OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para_local_var = OpenAPI_malloc(sizeof(OpenAPI_cn_assisted_ran_para_t));
    if (!cn_assisted_ran_para_local_var) {
        return NULL;
    }
    cn_assisted_ran_para_local_var->stationary_indication = stationary_indication;
    cn_assisted_ran_para_local_var->communication_duration_time = communication_duration_time;
    cn_assisted_ran_para_local_var->periodic_time = periodic_time;
    cn_assisted_ran_para_local_var->scheduled_communication_time = scheduled_communication_time;
    cn_assisted_ran_para_local_var->scheduled_communication_type = scheduled_communication_type;
    cn_assisted_ran_para_local_var->traffic_profile = traffic_profile;
    cn_assisted_ran_para_local_var->battery_indication = battery_indication;

    return cn_assisted_ran_para_local_var;
}

void OpenAPI_cn_assisted_ran_para_free(OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para)
{
    if (NULL == cn_assisted_ran_para) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_scheduled_communication_time_free(cn_assisted_ran_para->scheduled_communication_time);
    OpenAPI_battery_indication_free(cn_assisted_ran_para->battery_indication);
    ogs_free(cn_assisted_ran_para);
}

cJSON *OpenAPI_cn_assisted_ran_para_convertToJSON(OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para)
{
    cJSON *item = NULL;

    if (cn_assisted_ran_para == NULL) {
        ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [CnAssistedRanPara]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cn_assisted_ran_para->stationary_indication) {
        if (cJSON_AddStringToObject(item, "stationaryIndication", OpenAPI_stationary_indication_ToString(cn_assisted_ran_para->stationary_indication)) == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [stationary_indication]");
            goto end;
        }
    }

    if (cn_assisted_ran_para->communication_duration_time) {
        if (cJSON_AddNumberToObject(item, "communicationDurationTime", cn_assisted_ran_para->communication_duration_time) == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [communication_duration_time]");
            goto end;
        }
    }

    if (cn_assisted_ran_para->periodic_time) {
        if (cJSON_AddNumberToObject(item, "periodicTime", cn_assisted_ran_para->periodic_time) == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [periodic_time]");
            goto end;
        }
    }

    if (cn_assisted_ran_para->scheduled_communication_time) {
        cJSON *scheduled_communication_time_local_JSON = OpenAPI_scheduled_communication_time_convertToJSON(cn_assisted_ran_para->scheduled_communication_time);
        if (scheduled_communication_time_local_JSON == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [scheduled_communication_time]");
            goto end;
        }
        cJSON_AddItemToObject(item, "scheduledCommunicationTime", scheduled_communication_time_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [scheduled_communication_time]");
            goto end;
        }
    }

    if (cn_assisted_ran_para->scheduled_communication_type) {
        if (cJSON_AddStringToObject(item, "scheduledCommunicationType", OpenAPI_scheduled_communication_type_ToString(cn_assisted_ran_para->scheduled_communication_type)) == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [scheduled_communication_type]");
            goto end;
        }
    }

    if (cn_assisted_ran_para->traffic_profile) {
        if (cJSON_AddStringToObject(item, "trafficProfile", OpenAPI_traffic_profile_ToString(cn_assisted_ran_para->traffic_profile)) == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [traffic_profile]");
            goto end;
        }
    }

    if (cn_assisted_ran_para->battery_indication) {
        cJSON *battery_indication_local_JSON = OpenAPI_battery_indication_convertToJSON(cn_assisted_ran_para->battery_indication);
        if (battery_indication_local_JSON == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [battery_indication]");
            goto end;
        }
        cJSON_AddItemToObject(item, "batteryIndication", battery_indication_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed [battery_indication]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_cn_assisted_ran_para_t *OpenAPI_cn_assisted_ran_para_parseFromJSON(cJSON *cn_assisted_ran_paraJSON)
{
    OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para_local_var = NULL;
    cJSON *stationary_indication = cJSON_GetObjectItemCaseSensitive(cn_assisted_ran_paraJSON, "stationaryIndication");

    OpenAPI_stationary_indication_e stationary_indicationVariable;
    if (stationary_indication) {
        if (!cJSON_IsString(stationary_indication)) {
            ogs_error("OpenAPI_cn_assisted_ran_para_parseFromJSON() failed [stationary_indication]");
            goto end;
        }
        stationary_indicationVariable = OpenAPI_stationary_indication_FromString(stationary_indication->valuestring);
    }

    cJSON *communication_duration_time = cJSON_GetObjectItemCaseSensitive(cn_assisted_ran_paraJSON, "communicationDurationTime");

    if (communication_duration_time) {
        if (!cJSON_IsNumber(communication_duration_time)) {
            ogs_error("OpenAPI_cn_assisted_ran_para_parseFromJSON() failed [communication_duration_time]");
            goto end;
        }
    }

    cJSON *periodic_time = cJSON_GetObjectItemCaseSensitive(cn_assisted_ran_paraJSON, "periodicTime");

    if (periodic_time) {
        if (!cJSON_IsNumber(periodic_time)) {
            ogs_error("OpenAPI_cn_assisted_ran_para_parseFromJSON() failed [periodic_time]");
            goto end;
        }
    }

    cJSON *scheduled_communication_time = cJSON_GetObjectItemCaseSensitive(cn_assisted_ran_paraJSON, "scheduledCommunicationTime");

    OpenAPI_scheduled_communication_time_t *scheduled_communication_time_local_nonprim = NULL;
    if (scheduled_communication_time) {
        scheduled_communication_time_local_nonprim = OpenAPI_scheduled_communication_time_parseFromJSON(scheduled_communication_time);
    }

    cJSON *scheduled_communication_type = cJSON_GetObjectItemCaseSensitive(cn_assisted_ran_paraJSON, "scheduledCommunicationType");

    OpenAPI_scheduled_communication_type_e scheduled_communication_typeVariable;
    if (scheduled_communication_type) {
        if (!cJSON_IsString(scheduled_communication_type)) {
            ogs_error("OpenAPI_cn_assisted_ran_para_parseFromJSON() failed [scheduled_communication_type]");
            goto end;
        }
        scheduled_communication_typeVariable = OpenAPI_scheduled_communication_type_FromString(scheduled_communication_type->valuestring);
    }

    cJSON *traffic_profile = cJSON_GetObjectItemCaseSensitive(cn_assisted_ran_paraJSON, "trafficProfile");

    OpenAPI_traffic_profile_e traffic_profileVariable;
    if (traffic_profile) {
        if (!cJSON_IsString(traffic_profile)) {
            ogs_error("OpenAPI_cn_assisted_ran_para_parseFromJSON() failed [traffic_profile]");
            goto end;
        }
        traffic_profileVariable = OpenAPI_traffic_profile_FromString(traffic_profile->valuestring);
    }

    cJSON *battery_indication = cJSON_GetObjectItemCaseSensitive(cn_assisted_ran_paraJSON, "batteryIndication");

    OpenAPI_battery_indication_t *battery_indication_local_nonprim = NULL;
    if (battery_indication) {
        battery_indication_local_nonprim = OpenAPI_battery_indication_parseFromJSON(battery_indication);
    }

    cn_assisted_ran_para_local_var = OpenAPI_cn_assisted_ran_para_create (
        stationary_indication ? stationary_indicationVariable : 0,
        communication_duration_time ? communication_duration_time->valuedouble : 0,
        periodic_time ? periodic_time->valuedouble : 0,
        scheduled_communication_time ? scheduled_communication_time_local_nonprim : NULL,
        scheduled_communication_type ? scheduled_communication_typeVariable : 0,
        traffic_profile ? traffic_profileVariable : 0,
        battery_indication ? battery_indication_local_nonprim : NULL
        );

    return cn_assisted_ran_para_local_var;
end:
    return NULL;
}

OpenAPI_cn_assisted_ran_para_t *OpenAPI_cn_assisted_ran_para_copy(OpenAPI_cn_assisted_ran_para_t *dst, OpenAPI_cn_assisted_ran_para_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cn_assisted_ran_para_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cn_assisted_ran_para_convertToJSON() failed");
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

    OpenAPI_cn_assisted_ran_para_free(dst);
    dst = OpenAPI_cn_assisted_ran_para_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

