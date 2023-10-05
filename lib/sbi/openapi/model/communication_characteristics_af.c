
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "communication_characteristics_af.h"

OpenAPI_communication_characteristics_af_t *OpenAPI_communication_characteristics_af_create(
    bool is_pp_dl_packet_count_null,
    bool is_pp_dl_packet_count,
    int pp_dl_packet_count,
    bool is_maximum_response_time,
    int maximum_response_time,
    bool is_maximum_latency,
    int maximum_latency
)
{
    OpenAPI_communication_characteristics_af_t *communication_characteristics_af_local_var = ogs_malloc(sizeof(OpenAPI_communication_characteristics_af_t));
    ogs_assert(communication_characteristics_af_local_var);

    communication_characteristics_af_local_var->is_pp_dl_packet_count_null = is_pp_dl_packet_count_null;
    communication_characteristics_af_local_var->is_pp_dl_packet_count = is_pp_dl_packet_count;
    communication_characteristics_af_local_var->pp_dl_packet_count = pp_dl_packet_count;
    communication_characteristics_af_local_var->is_maximum_response_time = is_maximum_response_time;
    communication_characteristics_af_local_var->maximum_response_time = maximum_response_time;
    communication_characteristics_af_local_var->is_maximum_latency = is_maximum_latency;
    communication_characteristics_af_local_var->maximum_latency = maximum_latency;

    return communication_characteristics_af_local_var;
}

void OpenAPI_communication_characteristics_af_free(OpenAPI_communication_characteristics_af_t *communication_characteristics_af)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == communication_characteristics_af) {
        return;
    }
    ogs_free(communication_characteristics_af);
}

cJSON *OpenAPI_communication_characteristics_af_convertToJSON(OpenAPI_communication_characteristics_af_t *communication_characteristics_af)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (communication_characteristics_af == NULL) {
        ogs_error("OpenAPI_communication_characteristics_af_convertToJSON() failed [CommunicationCharacteristicsAF]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (communication_characteristics_af->is_pp_dl_packet_count) {
    if (cJSON_AddNumberToObject(item, "ppDlPacketCount", communication_characteristics_af->pp_dl_packet_count) == NULL) {
        ogs_error("OpenAPI_communication_characteristics_af_convertToJSON() failed [pp_dl_packet_count]");
        goto end;
    }
    } else if (communication_characteristics_af->is_pp_dl_packet_count_null) {
        if (cJSON_AddNullToObject(item, "ppDlPacketCount") == NULL) {
            ogs_error("OpenAPI_communication_characteristics_af_convertToJSON() failed [pp_dl_packet_count]");
            goto end;
        }
    }

    if (communication_characteristics_af->is_maximum_response_time) {
    if (cJSON_AddNumberToObject(item, "maximumResponseTime", communication_characteristics_af->maximum_response_time) == NULL) {
        ogs_error("OpenAPI_communication_characteristics_af_convertToJSON() failed [maximum_response_time]");
        goto end;
    }
    }

    if (communication_characteristics_af->is_maximum_latency) {
    if (cJSON_AddNumberToObject(item, "maximumLatency", communication_characteristics_af->maximum_latency) == NULL) {
        ogs_error("OpenAPI_communication_characteristics_af_convertToJSON() failed [maximum_latency]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_communication_characteristics_af_t *OpenAPI_communication_characteristics_af_parseFromJSON(cJSON *communication_characteristics_afJSON)
{
    OpenAPI_communication_characteristics_af_t *communication_characteristics_af_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pp_dl_packet_count = NULL;
    cJSON *maximum_response_time = NULL;
    cJSON *maximum_latency = NULL;
    pp_dl_packet_count = cJSON_GetObjectItemCaseSensitive(communication_characteristics_afJSON, "ppDlPacketCount");
    if (pp_dl_packet_count) {
    if (!cJSON_IsNull(pp_dl_packet_count)) {
    if (!cJSON_IsNumber(pp_dl_packet_count)) {
        ogs_error("OpenAPI_communication_characteristics_af_parseFromJSON() failed [pp_dl_packet_count]");
        goto end;
    }
    }
    }

    maximum_response_time = cJSON_GetObjectItemCaseSensitive(communication_characteristics_afJSON, "maximumResponseTime");
    if (maximum_response_time) {
    if (!cJSON_IsNumber(maximum_response_time)) {
        ogs_error("OpenAPI_communication_characteristics_af_parseFromJSON() failed [maximum_response_time]");
        goto end;
    }
    }

    maximum_latency = cJSON_GetObjectItemCaseSensitive(communication_characteristics_afJSON, "maximumLatency");
    if (maximum_latency) {
    if (!cJSON_IsNumber(maximum_latency)) {
        ogs_error("OpenAPI_communication_characteristics_af_parseFromJSON() failed [maximum_latency]");
        goto end;
    }
    }

    communication_characteristics_af_local_var = OpenAPI_communication_characteristics_af_create (
        pp_dl_packet_count && cJSON_IsNull(pp_dl_packet_count) ? true : false,
        pp_dl_packet_count ? true : false,
        pp_dl_packet_count ? pp_dl_packet_count->valuedouble : 0,
        maximum_response_time ? true : false,
        maximum_response_time ? maximum_response_time->valuedouble : 0,
        maximum_latency ? true : false,
        maximum_latency ? maximum_latency->valuedouble : 0
    );

    return communication_characteristics_af_local_var;
end:
    return NULL;
}

OpenAPI_communication_characteristics_af_t *OpenAPI_communication_characteristics_af_copy(OpenAPI_communication_characteristics_af_t *dst, OpenAPI_communication_characteristics_af_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_communication_characteristics_af_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_communication_characteristics_af_convertToJSON() failed");
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

    OpenAPI_communication_characteristics_af_free(dst);
    dst = OpenAPI_communication_characteristics_af_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

