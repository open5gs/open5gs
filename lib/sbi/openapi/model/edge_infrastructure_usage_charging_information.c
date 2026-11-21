
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "edge_infrastructure_usage_charging_information.h"

OpenAPI_edge_infrastructure_usage_charging_information_t *OpenAPI_edge_infrastructure_usage_charging_information_create(
    bool is_mean_virtual_cpu_usage,
    float mean_virtual_cpu_usage,
    bool is_mean_virtual_memory_usage,
    float mean_virtual_memory_usage,
    bool is_mean_virtual_disk_usage,
    float mean_virtual_disk_usage,
    bool is_measured_in_bytes,
    long measured_in_bytes,
    bool is_measured_out_bytes,
    long measured_out_bytes,
    char *duration_start_time,
    char *duration_end_time
)
{
    OpenAPI_edge_infrastructure_usage_charging_information_t *edge_infrastructure_usage_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_edge_infrastructure_usage_charging_information_t));
    ogs_assert(edge_infrastructure_usage_charging_information_local_var);

    edge_infrastructure_usage_charging_information_local_var->is_mean_virtual_cpu_usage = is_mean_virtual_cpu_usage;
    edge_infrastructure_usage_charging_information_local_var->mean_virtual_cpu_usage = mean_virtual_cpu_usage;
    edge_infrastructure_usage_charging_information_local_var->is_mean_virtual_memory_usage = is_mean_virtual_memory_usage;
    edge_infrastructure_usage_charging_information_local_var->mean_virtual_memory_usage = mean_virtual_memory_usage;
    edge_infrastructure_usage_charging_information_local_var->is_mean_virtual_disk_usage = is_mean_virtual_disk_usage;
    edge_infrastructure_usage_charging_information_local_var->mean_virtual_disk_usage = mean_virtual_disk_usage;
    edge_infrastructure_usage_charging_information_local_var->is_measured_in_bytes = is_measured_in_bytes;
    edge_infrastructure_usage_charging_information_local_var->measured_in_bytes = measured_in_bytes;
    edge_infrastructure_usage_charging_information_local_var->is_measured_out_bytes = is_measured_out_bytes;
    edge_infrastructure_usage_charging_information_local_var->measured_out_bytes = measured_out_bytes;
    edge_infrastructure_usage_charging_information_local_var->duration_start_time = duration_start_time;
    edge_infrastructure_usage_charging_information_local_var->duration_end_time = duration_end_time;

    return edge_infrastructure_usage_charging_information_local_var;
}

void OpenAPI_edge_infrastructure_usage_charging_information_free(OpenAPI_edge_infrastructure_usage_charging_information_t *edge_infrastructure_usage_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == edge_infrastructure_usage_charging_information) {
        return;
    }
    if (edge_infrastructure_usage_charging_information->duration_start_time) {
        ogs_free(edge_infrastructure_usage_charging_information->duration_start_time);
        edge_infrastructure_usage_charging_information->duration_start_time = NULL;
    }
    if (edge_infrastructure_usage_charging_information->duration_end_time) {
        ogs_free(edge_infrastructure_usage_charging_information->duration_end_time);
        edge_infrastructure_usage_charging_information->duration_end_time = NULL;
    }
    ogs_free(edge_infrastructure_usage_charging_information);
}

cJSON *OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON(OpenAPI_edge_infrastructure_usage_charging_information_t *edge_infrastructure_usage_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (edge_infrastructure_usage_charging_information == NULL) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed [EdgeInfrastructureUsageChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (edge_infrastructure_usage_charging_information->is_mean_virtual_cpu_usage) {
    if (cJSON_AddNumberToObject(item, "meanVirtualCPUUsage", edge_infrastructure_usage_charging_information->mean_virtual_cpu_usage) == NULL) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed [mean_virtual_cpu_usage]");
        goto end;
    }
    }

    if (edge_infrastructure_usage_charging_information->is_mean_virtual_memory_usage) {
    if (cJSON_AddNumberToObject(item, "meanVirtualMemoryUsage", edge_infrastructure_usage_charging_information->mean_virtual_memory_usage) == NULL) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed [mean_virtual_memory_usage]");
        goto end;
    }
    }

    if (edge_infrastructure_usage_charging_information->is_mean_virtual_disk_usage) {
    if (cJSON_AddNumberToObject(item, "meanVirtualDiskUsage", edge_infrastructure_usage_charging_information->mean_virtual_disk_usage) == NULL) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed [mean_virtual_disk_usage]");
        goto end;
    }
    }

    if (edge_infrastructure_usage_charging_information->is_measured_in_bytes) {
    if (cJSON_AddNumberToObject(item, "measuredInBytes", edge_infrastructure_usage_charging_information->measured_in_bytes) == NULL) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed [measured_in_bytes]");
        goto end;
    }
    }

    if (edge_infrastructure_usage_charging_information->is_measured_out_bytes) {
    if (cJSON_AddNumberToObject(item, "measuredOutBytes", edge_infrastructure_usage_charging_information->measured_out_bytes) == NULL) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed [measured_out_bytes]");
        goto end;
    }
    }

    if (edge_infrastructure_usage_charging_information->duration_start_time) {
    if (cJSON_AddStringToObject(item, "durationStartTime", edge_infrastructure_usage_charging_information->duration_start_time) == NULL) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed [duration_start_time]");
        goto end;
    }
    }

    if (edge_infrastructure_usage_charging_information->duration_end_time) {
    if (cJSON_AddStringToObject(item, "durationEndTime", edge_infrastructure_usage_charging_information->duration_end_time) == NULL) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed [duration_end_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_edge_infrastructure_usage_charging_information_t *OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON(cJSON *edge_infrastructure_usage_charging_informationJSON)
{
    OpenAPI_edge_infrastructure_usage_charging_information_t *edge_infrastructure_usage_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mean_virtual_cpu_usage = NULL;
    cJSON *mean_virtual_memory_usage = NULL;
    cJSON *mean_virtual_disk_usage = NULL;
    cJSON *measured_in_bytes = NULL;
    cJSON *measured_out_bytes = NULL;
    cJSON *duration_start_time = NULL;
    cJSON *duration_end_time = NULL;
    mean_virtual_cpu_usage = cJSON_GetObjectItemCaseSensitive(edge_infrastructure_usage_charging_informationJSON, "meanVirtualCPUUsage");
    if (mean_virtual_cpu_usage) {
    if (!cJSON_IsNumber(mean_virtual_cpu_usage)) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON() failed [mean_virtual_cpu_usage]");
        goto end;
    }
    }

    mean_virtual_memory_usage = cJSON_GetObjectItemCaseSensitive(edge_infrastructure_usage_charging_informationJSON, "meanVirtualMemoryUsage");
    if (mean_virtual_memory_usage) {
    if (!cJSON_IsNumber(mean_virtual_memory_usage)) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON() failed [mean_virtual_memory_usage]");
        goto end;
    }
    }

    mean_virtual_disk_usage = cJSON_GetObjectItemCaseSensitive(edge_infrastructure_usage_charging_informationJSON, "meanVirtualDiskUsage");
    if (mean_virtual_disk_usage) {
    if (!cJSON_IsNumber(mean_virtual_disk_usage)) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON() failed [mean_virtual_disk_usage]");
        goto end;
    }
    }

    measured_in_bytes = cJSON_GetObjectItemCaseSensitive(edge_infrastructure_usage_charging_informationJSON, "measuredInBytes");
    if (measured_in_bytes) {
    if (!cJSON_IsNumber(measured_in_bytes)) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON() failed [measured_in_bytes]");
        goto end;
    }
    }

    measured_out_bytes = cJSON_GetObjectItemCaseSensitive(edge_infrastructure_usage_charging_informationJSON, "measuredOutBytes");
    if (measured_out_bytes) {
    if (!cJSON_IsNumber(measured_out_bytes)) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON() failed [measured_out_bytes]");
        goto end;
    }
    }

    duration_start_time = cJSON_GetObjectItemCaseSensitive(edge_infrastructure_usage_charging_informationJSON, "durationStartTime");
    if (duration_start_time) {
    if (!cJSON_IsString(duration_start_time) && !cJSON_IsNull(duration_start_time)) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON() failed [duration_start_time]");
        goto end;
    }
    }

    duration_end_time = cJSON_GetObjectItemCaseSensitive(edge_infrastructure_usage_charging_informationJSON, "durationEndTime");
    if (duration_end_time) {
    if (!cJSON_IsString(duration_end_time) && !cJSON_IsNull(duration_end_time)) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON() failed [duration_end_time]");
        goto end;
    }
    }

    edge_infrastructure_usage_charging_information_local_var = OpenAPI_edge_infrastructure_usage_charging_information_create (
        mean_virtual_cpu_usage ? true : false,
        mean_virtual_cpu_usage ? mean_virtual_cpu_usage->valuedouble : 0,
        mean_virtual_memory_usage ? true : false,
        mean_virtual_memory_usage ? mean_virtual_memory_usage->valuedouble : 0,
        mean_virtual_disk_usage ? true : false,
        mean_virtual_disk_usage ? mean_virtual_disk_usage->valuedouble : 0,
        measured_in_bytes ? true : false,
        measured_in_bytes ? measured_in_bytes->valuedouble : 0,
        measured_out_bytes ? true : false,
        measured_out_bytes ? measured_out_bytes->valuedouble : 0,
        duration_start_time && !cJSON_IsNull(duration_start_time) ? ogs_strdup(duration_start_time->valuestring) : NULL,
        duration_end_time && !cJSON_IsNull(duration_end_time) ? ogs_strdup(duration_end_time->valuestring) : NULL
    );

    return edge_infrastructure_usage_charging_information_local_var;
end:
    return NULL;
}

OpenAPI_edge_infrastructure_usage_charging_information_t *OpenAPI_edge_infrastructure_usage_charging_information_copy(OpenAPI_edge_infrastructure_usage_charging_information_t *dst, OpenAPI_edge_infrastructure_usage_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON() failed");
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

    OpenAPI_edge_infrastructure_usage_charging_information_free(dst);
    dst = OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

