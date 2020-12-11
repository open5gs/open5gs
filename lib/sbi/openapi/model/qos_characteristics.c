
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_characteristics.h"

OpenAPI_qos_characteristics_t *OpenAPI_qos_characteristics_create(
    int _5qi,
    OpenAPI_qos_resource_type_t *resource_type,
    int priority_level,
    int packet_delay_budget,
    char *packet_error_rate,
    int averaging_window,
    int max_data_burst_vol,
    int ext_max_data_burst_vol
    )
{
    OpenAPI_qos_characteristics_t *qos_characteristics_local_var = OpenAPI_malloc(sizeof(OpenAPI_qos_characteristics_t));
    if (!qos_characteristics_local_var) {
        return NULL;
    }
    qos_characteristics_local_var->_5qi = _5qi;
    qos_characteristics_local_var->resource_type = resource_type;
    qos_characteristics_local_var->priority_level = priority_level;
    qos_characteristics_local_var->packet_delay_budget = packet_delay_budget;
    qos_characteristics_local_var->packet_error_rate = packet_error_rate;
    qos_characteristics_local_var->averaging_window = averaging_window;
    qos_characteristics_local_var->max_data_burst_vol = max_data_burst_vol;
    qos_characteristics_local_var->ext_max_data_burst_vol = ext_max_data_burst_vol;

    return qos_characteristics_local_var;
}

void OpenAPI_qos_characteristics_free(OpenAPI_qos_characteristics_t *qos_characteristics)
{
    if (NULL == qos_characteristics) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_qos_resource_type_free(qos_characteristics->resource_type);
    ogs_free(qos_characteristics->packet_error_rate);
    ogs_free(qos_characteristics);
}

cJSON *OpenAPI_qos_characteristics_convertToJSON(OpenAPI_qos_characteristics_t *qos_characteristics)
{
    cJSON *item = NULL;

    if (qos_characteristics == NULL) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [QosCharacteristics]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!qos_characteristics->_5qi) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [_5qi]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "5qi", qos_characteristics->_5qi) == NULL) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [_5qi]");
        goto end;
    }

    if (!qos_characteristics->resource_type) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [resource_type]");
        goto end;
    }
    cJSON *resource_type_local_JSON = OpenAPI_qos_resource_type_convertToJSON(qos_characteristics->resource_type);
    if (resource_type_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [resource_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "resourceType", resource_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [resource_type]");
        goto end;
    }

    if (!qos_characteristics->priority_level) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [priority_level]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "priorityLevel", qos_characteristics->priority_level) == NULL) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [priority_level]");
        goto end;
    }

    if (!qos_characteristics->packet_delay_budget) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [packet_delay_budget]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "packetDelayBudget", qos_characteristics->packet_delay_budget) == NULL) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [packet_delay_budget]");
        goto end;
    }

    if (!qos_characteristics->packet_error_rate) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [packet_error_rate]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "packetErrorRate", qos_characteristics->packet_error_rate) == NULL) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [packet_error_rate]");
        goto end;
    }

    if (qos_characteristics->averaging_window) {
        if (cJSON_AddNumberToObject(item, "averagingWindow", qos_characteristics->averaging_window) == NULL) {
            ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [averaging_window]");
            goto end;
        }
    }

    if (qos_characteristics->max_data_burst_vol) {
        if (cJSON_AddNumberToObject(item, "maxDataBurstVol", qos_characteristics->max_data_burst_vol) == NULL) {
            ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [max_data_burst_vol]");
            goto end;
        }
    }

    if (qos_characteristics->ext_max_data_burst_vol) {
        if (cJSON_AddNumberToObject(item, "extMaxDataBurstVol", qos_characteristics->ext_max_data_burst_vol) == NULL) {
            ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed [ext_max_data_burst_vol]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_qos_characteristics_t *OpenAPI_qos_characteristics_parseFromJSON(cJSON *qos_characteristicsJSON)
{
    OpenAPI_qos_characteristics_t *qos_characteristics_local_var = NULL;
    cJSON *_5qi = cJSON_GetObjectItemCaseSensitive(qos_characteristicsJSON, "5qi");
    if (!_5qi) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [_5qi]");
        goto end;
    }


    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [_5qi]");
        goto end;
    }

    cJSON *resource_type = cJSON_GetObjectItemCaseSensitive(qos_characteristicsJSON, "resourceType");
    if (!resource_type) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [resource_type]");
        goto end;
    }

    OpenAPI_qos_resource_type_t *resource_type_local_nonprim = NULL;

    resource_type_local_nonprim = OpenAPI_qos_resource_type_parseFromJSON(resource_type);

    cJSON *priority_level = cJSON_GetObjectItemCaseSensitive(qos_characteristicsJSON, "priorityLevel");
    if (!priority_level) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [priority_level]");
        goto end;
    }


    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [priority_level]");
        goto end;
    }

    cJSON *packet_delay_budget = cJSON_GetObjectItemCaseSensitive(qos_characteristicsJSON, "packetDelayBudget");
    if (!packet_delay_budget) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }


    if (!cJSON_IsNumber(packet_delay_budget)) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }

    cJSON *packet_error_rate = cJSON_GetObjectItemCaseSensitive(qos_characteristicsJSON, "packetErrorRate");
    if (!packet_error_rate) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [packet_error_rate]");
        goto end;
    }


    if (!cJSON_IsString(packet_error_rate)) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [packet_error_rate]");
        goto end;
    }

    cJSON *averaging_window = cJSON_GetObjectItemCaseSensitive(qos_characteristicsJSON, "averagingWindow");

    if (averaging_window) {
        if (!cJSON_IsNumber(averaging_window)) {
            ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [averaging_window]");
            goto end;
        }
    }

    cJSON *max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(qos_characteristicsJSON, "maxDataBurstVol");

    if (max_data_burst_vol) {
        if (!cJSON_IsNumber(max_data_burst_vol)) {
            ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [max_data_burst_vol]");
            goto end;
        }
    }

    cJSON *ext_max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(qos_characteristicsJSON, "extMaxDataBurstVol");

    if (ext_max_data_burst_vol) {
        if (!cJSON_IsNumber(ext_max_data_burst_vol)) {
            ogs_error("OpenAPI_qos_characteristics_parseFromJSON() failed [ext_max_data_burst_vol]");
            goto end;
        }
    }

    qos_characteristics_local_var = OpenAPI_qos_characteristics_create (
        _5qi->valuedouble,
        resource_type_local_nonprim,
        priority_level->valuedouble,
        packet_delay_budget->valuedouble,
        ogs_strdup(packet_error_rate->valuestring),
        averaging_window ? averaging_window->valuedouble : 0,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        ext_max_data_burst_vol ? ext_max_data_burst_vol->valuedouble : 0
        );

    return qos_characteristics_local_var;
end:
    return NULL;
}

OpenAPI_qos_characteristics_t *OpenAPI_qos_characteristics_copy(OpenAPI_qos_characteristics_t *dst, OpenAPI_qos_characteristics_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_characteristics_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_characteristics_convertToJSON() failed");
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

    OpenAPI_qos_characteristics_free(dst);
    dst = OpenAPI_qos_characteristics_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

