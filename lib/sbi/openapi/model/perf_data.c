
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "perf_data.h"

OpenAPI_perf_data_t *OpenAPI_perf_data_create(
    char *avg_traffic_rate,
    char *max_traffic_rate,
    bool is_ave_packet_delay,
    int ave_packet_delay,
    bool is_max_packet_delay,
    int max_packet_delay,
    bool is_avg_packet_loss_rate,
    int avg_packet_loss_rate
)
{
    OpenAPI_perf_data_t *perf_data_local_var = ogs_malloc(sizeof(OpenAPI_perf_data_t));
    ogs_assert(perf_data_local_var);

    perf_data_local_var->avg_traffic_rate = avg_traffic_rate;
    perf_data_local_var->max_traffic_rate = max_traffic_rate;
    perf_data_local_var->is_ave_packet_delay = is_ave_packet_delay;
    perf_data_local_var->ave_packet_delay = ave_packet_delay;
    perf_data_local_var->is_max_packet_delay = is_max_packet_delay;
    perf_data_local_var->max_packet_delay = max_packet_delay;
    perf_data_local_var->is_avg_packet_loss_rate = is_avg_packet_loss_rate;
    perf_data_local_var->avg_packet_loss_rate = avg_packet_loss_rate;

    return perf_data_local_var;
}

void OpenAPI_perf_data_free(OpenAPI_perf_data_t *perf_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == perf_data) {
        return;
    }
    if (perf_data->avg_traffic_rate) {
        ogs_free(perf_data->avg_traffic_rate);
        perf_data->avg_traffic_rate = NULL;
    }
    if (perf_data->max_traffic_rate) {
        ogs_free(perf_data->max_traffic_rate);
        perf_data->max_traffic_rate = NULL;
    }
    ogs_free(perf_data);
}

cJSON *OpenAPI_perf_data_convertToJSON(OpenAPI_perf_data_t *perf_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (perf_data == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [PerfData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (perf_data->avg_traffic_rate) {
    if (cJSON_AddStringToObject(item, "avgTrafficRate", perf_data->avg_traffic_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [avg_traffic_rate]");
        goto end;
    }
    }

    if (perf_data->max_traffic_rate) {
    if (cJSON_AddStringToObject(item, "maxTrafficRate", perf_data->max_traffic_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [max_traffic_rate]");
        goto end;
    }
    }

    if (perf_data->is_ave_packet_delay) {
    if (cJSON_AddNumberToObject(item, "avePacketDelay", perf_data->ave_packet_delay) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [ave_packet_delay]");
        goto end;
    }
    }

    if (perf_data->is_max_packet_delay) {
    if (cJSON_AddNumberToObject(item, "maxPacketDelay", perf_data->max_packet_delay) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [max_packet_delay]");
        goto end;
    }
    }

    if (perf_data->is_avg_packet_loss_rate) {
    if (cJSON_AddNumberToObject(item, "avgPacketLossRate", perf_data->avg_packet_loss_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [avg_packet_loss_rate]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_perf_data_t *OpenAPI_perf_data_parseFromJSON(cJSON *perf_dataJSON)
{
    OpenAPI_perf_data_t *perf_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *avg_traffic_rate = NULL;
    cJSON *max_traffic_rate = NULL;
    cJSON *ave_packet_delay = NULL;
    cJSON *max_packet_delay = NULL;
    cJSON *avg_packet_loss_rate = NULL;
    avg_traffic_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "avgTrafficRate");
    if (avg_traffic_rate) {
    if (!cJSON_IsString(avg_traffic_rate) && !cJSON_IsNull(avg_traffic_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [avg_traffic_rate]");
        goto end;
    }
    }

    max_traffic_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "maxTrafficRate");
    if (max_traffic_rate) {
    if (!cJSON_IsString(max_traffic_rate) && !cJSON_IsNull(max_traffic_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [max_traffic_rate]");
        goto end;
    }
    }

    ave_packet_delay = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "avePacketDelay");
    if (ave_packet_delay) {
    if (!cJSON_IsNumber(ave_packet_delay)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [ave_packet_delay]");
        goto end;
    }
    }

    max_packet_delay = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "maxPacketDelay");
    if (max_packet_delay) {
    if (!cJSON_IsNumber(max_packet_delay)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [max_packet_delay]");
        goto end;
    }
    }

    avg_packet_loss_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "avgPacketLossRate");
    if (avg_packet_loss_rate) {
    if (!cJSON_IsNumber(avg_packet_loss_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [avg_packet_loss_rate]");
        goto end;
    }
    }

    perf_data_local_var = OpenAPI_perf_data_create (
        avg_traffic_rate && !cJSON_IsNull(avg_traffic_rate) ? ogs_strdup(avg_traffic_rate->valuestring) : NULL,
        max_traffic_rate && !cJSON_IsNull(max_traffic_rate) ? ogs_strdup(max_traffic_rate->valuestring) : NULL,
        ave_packet_delay ? true : false,
        ave_packet_delay ? ave_packet_delay->valuedouble : 0,
        max_packet_delay ? true : false,
        max_packet_delay ? max_packet_delay->valuedouble : 0,
        avg_packet_loss_rate ? true : false,
        avg_packet_loss_rate ? avg_packet_loss_rate->valuedouble : 0
    );

    return perf_data_local_var;
end:
    return NULL;
}

OpenAPI_perf_data_t *OpenAPI_perf_data_copy(OpenAPI_perf_data_t *dst, OpenAPI_perf_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_perf_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed");
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

    OpenAPI_perf_data_free(dst);
    dst = OpenAPI_perf_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

