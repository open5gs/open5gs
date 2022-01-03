
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "communication_characteristics.h"

OpenAPI_communication_characteristics_t *OpenAPI_communication_characteristics_create(
    OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer,
    OpenAPI_pp_active_time_t *pp_active_time,
    bool is_pp_dl_packet_count,
    int pp_dl_packet_count,
    OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext,
    OpenAPI_pp_maximum_response_time_t *pp_maximum_response_time,
    OpenAPI_pp_maximum_latency_t *pp_maximum_latency
)
{
    OpenAPI_communication_characteristics_t *communication_characteristics_local_var = ogs_malloc(sizeof(OpenAPI_communication_characteristics_t));
    ogs_assert(communication_characteristics_local_var);

    communication_characteristics_local_var->pp_subs_reg_timer = pp_subs_reg_timer;
    communication_characteristics_local_var->pp_active_time = pp_active_time;
    communication_characteristics_local_var->is_pp_dl_packet_count = is_pp_dl_packet_count;
    communication_characteristics_local_var->pp_dl_packet_count = pp_dl_packet_count;
    communication_characteristics_local_var->pp_dl_packet_count_ext = pp_dl_packet_count_ext;
    communication_characteristics_local_var->pp_maximum_response_time = pp_maximum_response_time;
    communication_characteristics_local_var->pp_maximum_latency = pp_maximum_latency;

    return communication_characteristics_local_var;
}

void OpenAPI_communication_characteristics_free(OpenAPI_communication_characteristics_t *communication_characteristics)
{
    if (NULL == communication_characteristics) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_pp_subs_reg_timer_free(communication_characteristics->pp_subs_reg_timer);
    OpenAPI_pp_active_time_free(communication_characteristics->pp_active_time);
    OpenAPI_pp_dl_packet_count_ext_free(communication_characteristics->pp_dl_packet_count_ext);
    OpenAPI_pp_maximum_response_time_free(communication_characteristics->pp_maximum_response_time);
    OpenAPI_pp_maximum_latency_free(communication_characteristics->pp_maximum_latency);
    ogs_free(communication_characteristics);
}

cJSON *OpenAPI_communication_characteristics_convertToJSON(OpenAPI_communication_characteristics_t *communication_characteristics)
{
    cJSON *item = NULL;

    if (communication_characteristics == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [CommunicationCharacteristics]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (communication_characteristics->pp_subs_reg_timer) {
    cJSON *pp_subs_reg_timer_local_JSON = OpenAPI_pp_subs_reg_timer_convertToJSON(communication_characteristics->pp_subs_reg_timer);
    if (pp_subs_reg_timer_local_JSON == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_subs_reg_timer]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ppSubsRegTimer", pp_subs_reg_timer_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_subs_reg_timer]");
        goto end;
    }
    }

    if (communication_characteristics->pp_active_time) {
    cJSON *pp_active_time_local_JSON = OpenAPI_pp_active_time_convertToJSON(communication_characteristics->pp_active_time);
    if (pp_active_time_local_JSON == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_active_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ppActiveTime", pp_active_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_active_time]");
        goto end;
    }
    }

    if (communication_characteristics->is_pp_dl_packet_count) {
    if (cJSON_AddNumberToObject(item, "ppDlPacketCount", communication_characteristics->pp_dl_packet_count) == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_dl_packet_count]");
        goto end;
    }
    }

    if (communication_characteristics->pp_dl_packet_count_ext) {
    cJSON *pp_dl_packet_count_ext_local_JSON = OpenAPI_pp_dl_packet_count_ext_convertToJSON(communication_characteristics->pp_dl_packet_count_ext);
    if (pp_dl_packet_count_ext_local_JSON == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_dl_packet_count_ext]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ppDlPacketCountExt", pp_dl_packet_count_ext_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_dl_packet_count_ext]");
        goto end;
    }
    }

    if (communication_characteristics->pp_maximum_response_time) {
    cJSON *pp_maximum_response_time_local_JSON = OpenAPI_pp_maximum_response_time_convertToJSON(communication_characteristics->pp_maximum_response_time);
    if (pp_maximum_response_time_local_JSON == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_maximum_response_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ppMaximumResponseTime", pp_maximum_response_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_maximum_response_time]");
        goto end;
    }
    }

    if (communication_characteristics->pp_maximum_latency) {
    cJSON *pp_maximum_latency_local_JSON = OpenAPI_pp_maximum_latency_convertToJSON(communication_characteristics->pp_maximum_latency);
    if (pp_maximum_latency_local_JSON == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_maximum_latency]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ppMaximumLatency", pp_maximum_latency_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_maximum_latency]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_communication_characteristics_t *OpenAPI_communication_characteristics_parseFromJSON(cJSON *communication_characteristicsJSON)
{
    OpenAPI_communication_characteristics_t *communication_characteristics_local_var = NULL;
    cJSON *pp_subs_reg_timer = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppSubsRegTimer");

    OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer_local_nonprim = NULL;
    if (pp_subs_reg_timer) {
    pp_subs_reg_timer_local_nonprim = OpenAPI_pp_subs_reg_timer_parseFromJSON(pp_subs_reg_timer);
    }

    cJSON *pp_active_time = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppActiveTime");

    OpenAPI_pp_active_time_t *pp_active_time_local_nonprim = NULL;
    if (pp_active_time) {
    pp_active_time_local_nonprim = OpenAPI_pp_active_time_parseFromJSON(pp_active_time);
    }

    cJSON *pp_dl_packet_count = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppDlPacketCount");

    if (pp_dl_packet_count) {
    if (!cJSON_IsNumber(pp_dl_packet_count)) {
        ogs_error("OpenAPI_communication_characteristics_parseFromJSON() failed [pp_dl_packet_count]");
        goto end;
    }
    }

    cJSON *pp_dl_packet_count_ext = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppDlPacketCountExt");

    OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext_local_nonprim = NULL;
    if (pp_dl_packet_count_ext) {
    pp_dl_packet_count_ext_local_nonprim = OpenAPI_pp_dl_packet_count_ext_parseFromJSON(pp_dl_packet_count_ext);
    }

    cJSON *pp_maximum_response_time = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppMaximumResponseTime");

    OpenAPI_pp_maximum_response_time_t *pp_maximum_response_time_local_nonprim = NULL;
    if (pp_maximum_response_time) {
    pp_maximum_response_time_local_nonprim = OpenAPI_pp_maximum_response_time_parseFromJSON(pp_maximum_response_time);
    }

    cJSON *pp_maximum_latency = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppMaximumLatency");

    OpenAPI_pp_maximum_latency_t *pp_maximum_latency_local_nonprim = NULL;
    if (pp_maximum_latency) {
    pp_maximum_latency_local_nonprim = OpenAPI_pp_maximum_latency_parseFromJSON(pp_maximum_latency);
    }

    communication_characteristics_local_var = OpenAPI_communication_characteristics_create (
        pp_subs_reg_timer ? pp_subs_reg_timer_local_nonprim : NULL,
        pp_active_time ? pp_active_time_local_nonprim : NULL,
        pp_dl_packet_count ? true : false,
        pp_dl_packet_count ? pp_dl_packet_count->valuedouble : 0,
        pp_dl_packet_count_ext ? pp_dl_packet_count_ext_local_nonprim : NULL,
        pp_maximum_response_time ? pp_maximum_response_time_local_nonprim : NULL,
        pp_maximum_latency ? pp_maximum_latency_local_nonprim : NULL
    );

    return communication_characteristics_local_var;
end:
    return NULL;
}

OpenAPI_communication_characteristics_t *OpenAPI_communication_characteristics_copy(OpenAPI_communication_characteristics_t *dst, OpenAPI_communication_characteristics_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_communication_characteristics_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed");
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

    OpenAPI_communication_characteristics_free(dst);
    dst = OpenAPI_communication_characteristics_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

