
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dynamic5_qi.h"

OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_create(
    OpenAPI_qos_resource_type_t *resource_type,
    int priority_level,
    int packet_delay_budget,
    char *packet_err_rate,
    int aver_window,
    int max_data_burst_vol,
    int ext_max_data_burst_vol
    )
{
    OpenAPI_dynamic5_qi_t *dynamic5_qi_local_var = OpenAPI_malloc(sizeof(OpenAPI_dynamic5_qi_t));
    if (!dynamic5_qi_local_var) {
        return NULL;
    }
    dynamic5_qi_local_var->resource_type = resource_type;
    dynamic5_qi_local_var->priority_level = priority_level;
    dynamic5_qi_local_var->packet_delay_budget = packet_delay_budget;
    dynamic5_qi_local_var->packet_err_rate = packet_err_rate;
    dynamic5_qi_local_var->aver_window = aver_window;
    dynamic5_qi_local_var->max_data_burst_vol = max_data_burst_vol;
    dynamic5_qi_local_var->ext_max_data_burst_vol = ext_max_data_burst_vol;

    return dynamic5_qi_local_var;
}

void OpenAPI_dynamic5_qi_free(OpenAPI_dynamic5_qi_t *dynamic5_qi)
{
    if (NULL == dynamic5_qi) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_qos_resource_type_free(dynamic5_qi->resource_type);
    ogs_free(dynamic5_qi->packet_err_rate);
    ogs_free(dynamic5_qi);
}

cJSON *OpenAPI_dynamic5_qi_convertToJSON(OpenAPI_dynamic5_qi_t *dynamic5_qi)
{
    cJSON *item = NULL;

    if (dynamic5_qi == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [Dynamic5Qi]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dynamic5_qi->resource_type) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [resource_type]");
        goto end;
    }
    cJSON *resource_type_local_JSON = OpenAPI_qos_resource_type_convertToJSON(dynamic5_qi->resource_type);
    if (resource_type_local_JSON == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [resource_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "resourceType", resource_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [resource_type]");
        goto end;
    }

    if (!dynamic5_qi->priority_level) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [priority_level]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "priorityLevel", dynamic5_qi->priority_level) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [priority_level]");
        goto end;
    }

    if (!dynamic5_qi->packet_delay_budget) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [packet_delay_budget]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "packetDelayBudget", dynamic5_qi->packet_delay_budget) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [packet_delay_budget]");
        goto end;
    }

    if (!dynamic5_qi->packet_err_rate) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [packet_err_rate]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "packetErrRate", dynamic5_qi->packet_err_rate) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [packet_err_rate]");
        goto end;
    }

    if (dynamic5_qi->aver_window) {
        if (cJSON_AddNumberToObject(item, "averWindow", dynamic5_qi->aver_window) == NULL) {
            ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [aver_window]");
            goto end;
        }
    }

    if (dynamic5_qi->max_data_burst_vol) {
        if (cJSON_AddNumberToObject(item, "maxDataBurstVol", dynamic5_qi->max_data_burst_vol) == NULL) {
            ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [max_data_burst_vol]");
            goto end;
        }
    }

    if (dynamic5_qi->ext_max_data_burst_vol) {
        if (cJSON_AddNumberToObject(item, "extMaxDataBurstVol", dynamic5_qi->ext_max_data_burst_vol) == NULL) {
            ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [ext_max_data_burst_vol]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_parseFromJSON(cJSON *dynamic5_qiJSON)
{
    OpenAPI_dynamic5_qi_t *dynamic5_qi_local_var = NULL;
    cJSON *resource_type = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "resourceType");
    if (!resource_type) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [resource_type]");
        goto end;
    }

    OpenAPI_qos_resource_type_t *resource_type_local_nonprim = NULL;

    resource_type_local_nonprim = OpenAPI_qos_resource_type_parseFromJSON(resource_type);

    cJSON *priority_level = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "priorityLevel");
    if (!priority_level) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [priority_level]");
        goto end;
    }


    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [priority_level]");
        goto end;
    }

    cJSON *packet_delay_budget = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "packetDelayBudget");
    if (!packet_delay_budget) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }


    if (!cJSON_IsNumber(packet_delay_budget)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }

    cJSON *packet_err_rate = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "packetErrRate");
    if (!packet_err_rate) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [packet_err_rate]");
        goto end;
    }


    if (!cJSON_IsString(packet_err_rate)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [packet_err_rate]");
        goto end;
    }

    cJSON *aver_window = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "averWindow");

    if (aver_window) {
        if (!cJSON_IsNumber(aver_window)) {
            ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [aver_window]");
            goto end;
        }
    }

    cJSON *max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "maxDataBurstVol");

    if (max_data_burst_vol) {
        if (!cJSON_IsNumber(max_data_burst_vol)) {
            ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [max_data_burst_vol]");
            goto end;
        }
    }

    cJSON *ext_max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "extMaxDataBurstVol");

    if (ext_max_data_burst_vol) {
        if (!cJSON_IsNumber(ext_max_data_burst_vol)) {
            ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [ext_max_data_burst_vol]");
            goto end;
        }
    }

    dynamic5_qi_local_var = OpenAPI_dynamic5_qi_create (
        resource_type_local_nonprim,
        priority_level->valuedouble,
        packet_delay_budget->valuedouble,
        ogs_strdup(packet_err_rate->valuestring),
        aver_window ? aver_window->valuedouble : 0,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        ext_max_data_burst_vol ? ext_max_data_burst_vol->valuedouble : 0
        );

    return dynamic5_qi_local_var;
end:
    return NULL;
}

OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_copy(OpenAPI_dynamic5_qi_t *dst, OpenAPI_dynamic5_qi_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dynamic5_qi_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed");
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

    OpenAPI_dynamic5_qi_free(dst);
    dst = OpenAPI_dynamic5_qi_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

