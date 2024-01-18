
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "communication_characteristics.h"

OpenAPI_communication_characteristics_t *OpenAPI_communication_characteristics_create(
    bool is_pp_subs_reg_timer_null,
    OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer,
    bool is_pp_active_time_null,
    OpenAPI_pp_active_time_t *pp_active_time,
    bool is_pp_dl_packet_count_null,
    bool is_pp_dl_packet_count,
    int pp_dl_packet_count,
    bool is_pp_dl_packet_count_ext_null,
    OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext,
    bool is_pp_maximum_response_time_null,
    OpenAPI_pp_maximum_response_time_t *pp_maximum_response_time,
    bool is_pp_maximum_latency_null,
    OpenAPI_pp_maximum_latency_t *pp_maximum_latency
)
{
    OpenAPI_communication_characteristics_t *communication_characteristics_local_var = ogs_malloc(sizeof(OpenAPI_communication_characteristics_t));
    ogs_assert(communication_characteristics_local_var);

    communication_characteristics_local_var->is_pp_subs_reg_timer_null = is_pp_subs_reg_timer_null;
    communication_characteristics_local_var->pp_subs_reg_timer = pp_subs_reg_timer;
    communication_characteristics_local_var->is_pp_active_time_null = is_pp_active_time_null;
    communication_characteristics_local_var->pp_active_time = pp_active_time;
    communication_characteristics_local_var->is_pp_dl_packet_count_null = is_pp_dl_packet_count_null;
    communication_characteristics_local_var->is_pp_dl_packet_count = is_pp_dl_packet_count;
    communication_characteristics_local_var->pp_dl_packet_count = pp_dl_packet_count;
    communication_characteristics_local_var->is_pp_dl_packet_count_ext_null = is_pp_dl_packet_count_ext_null;
    communication_characteristics_local_var->pp_dl_packet_count_ext = pp_dl_packet_count_ext;
    communication_characteristics_local_var->is_pp_maximum_response_time_null = is_pp_maximum_response_time_null;
    communication_characteristics_local_var->pp_maximum_response_time = pp_maximum_response_time;
    communication_characteristics_local_var->is_pp_maximum_latency_null = is_pp_maximum_latency_null;
    communication_characteristics_local_var->pp_maximum_latency = pp_maximum_latency;

    return communication_characteristics_local_var;
}

void OpenAPI_communication_characteristics_free(OpenAPI_communication_characteristics_t *communication_characteristics)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == communication_characteristics) {
        return;
    }
    if (communication_characteristics->pp_subs_reg_timer) {
        OpenAPI_pp_subs_reg_timer_free(communication_characteristics->pp_subs_reg_timer);
        communication_characteristics->pp_subs_reg_timer = NULL;
    }
    if (communication_characteristics->pp_active_time) {
        OpenAPI_pp_active_time_free(communication_characteristics->pp_active_time);
        communication_characteristics->pp_active_time = NULL;
    }
    if (communication_characteristics->pp_dl_packet_count_ext) {
        OpenAPI_pp_dl_packet_count_ext_free(communication_characteristics->pp_dl_packet_count_ext);
        communication_characteristics->pp_dl_packet_count_ext = NULL;
    }
    if (communication_characteristics->pp_maximum_response_time) {
        OpenAPI_pp_maximum_response_time_free(communication_characteristics->pp_maximum_response_time);
        communication_characteristics->pp_maximum_response_time = NULL;
    }
    if (communication_characteristics->pp_maximum_latency) {
        OpenAPI_pp_maximum_latency_free(communication_characteristics->pp_maximum_latency);
        communication_characteristics->pp_maximum_latency = NULL;
    }
    ogs_free(communication_characteristics);
}

cJSON *OpenAPI_communication_characteristics_convertToJSON(OpenAPI_communication_characteristics_t *communication_characteristics)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    } else if (communication_characteristics->is_pp_subs_reg_timer_null) {
        if (cJSON_AddNullToObject(item, "ppSubsRegTimer") == NULL) {
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
    } else if (communication_characteristics->is_pp_active_time_null) {
        if (cJSON_AddNullToObject(item, "ppActiveTime") == NULL) {
            ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_active_time]");
            goto end;
        }
    }

    if (communication_characteristics->is_pp_dl_packet_count) {
    if (cJSON_AddNumberToObject(item, "ppDlPacketCount", communication_characteristics->pp_dl_packet_count) == NULL) {
        ogs_error("OpenAPI_communication_characteristics_convertToJSON() failed [pp_dl_packet_count]");
        goto end;
    }
    } else if (communication_characteristics->is_pp_dl_packet_count_null) {
        if (cJSON_AddNullToObject(item, "ppDlPacketCount") == NULL) {
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
    } else if (communication_characteristics->is_pp_dl_packet_count_ext_null) {
        if (cJSON_AddNullToObject(item, "ppDlPacketCountExt") == NULL) {
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
    } else if (communication_characteristics->is_pp_maximum_response_time_null) {
        if (cJSON_AddNullToObject(item, "ppMaximumResponseTime") == NULL) {
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
    } else if (communication_characteristics->is_pp_maximum_latency_null) {
        if (cJSON_AddNullToObject(item, "ppMaximumLatency") == NULL) {
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *pp_subs_reg_timer = NULL;
    OpenAPI_pp_subs_reg_timer_t *pp_subs_reg_timer_local_nonprim = NULL;
    cJSON *pp_active_time = NULL;
    OpenAPI_pp_active_time_t *pp_active_time_local_nonprim = NULL;
    cJSON *pp_dl_packet_count = NULL;
    cJSON *pp_dl_packet_count_ext = NULL;
    OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext_local_nonprim = NULL;
    cJSON *pp_maximum_response_time = NULL;
    OpenAPI_pp_maximum_response_time_t *pp_maximum_response_time_local_nonprim = NULL;
    cJSON *pp_maximum_latency = NULL;
    OpenAPI_pp_maximum_latency_t *pp_maximum_latency_local_nonprim = NULL;
    pp_subs_reg_timer = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppSubsRegTimer");
    if (pp_subs_reg_timer) {
    if (!cJSON_IsNull(pp_subs_reg_timer)) {
    pp_subs_reg_timer_local_nonprim = OpenAPI_pp_subs_reg_timer_parseFromJSON(pp_subs_reg_timer);
    if (!pp_subs_reg_timer_local_nonprim) {
        ogs_error("OpenAPI_pp_subs_reg_timer_parseFromJSON failed [pp_subs_reg_timer]");
        goto end;
    }
    }
    }

    pp_active_time = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppActiveTime");
    if (pp_active_time) {
    if (!cJSON_IsNull(pp_active_time)) {
    pp_active_time_local_nonprim = OpenAPI_pp_active_time_parseFromJSON(pp_active_time);
    if (!pp_active_time_local_nonprim) {
        ogs_error("OpenAPI_pp_active_time_parseFromJSON failed [pp_active_time]");
        goto end;
    }
    }
    }

    pp_dl_packet_count = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppDlPacketCount");
    if (pp_dl_packet_count) {
    if (!cJSON_IsNull(pp_dl_packet_count)) {
    if (!cJSON_IsNumber(pp_dl_packet_count)) {
        ogs_error("OpenAPI_communication_characteristics_parseFromJSON() failed [pp_dl_packet_count]");
        goto end;
    }
    }
    }

    pp_dl_packet_count_ext = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppDlPacketCountExt");
    if (pp_dl_packet_count_ext) {
    if (!cJSON_IsNull(pp_dl_packet_count_ext)) {
    pp_dl_packet_count_ext_local_nonprim = OpenAPI_pp_dl_packet_count_ext_parseFromJSON(pp_dl_packet_count_ext);
    if (!pp_dl_packet_count_ext_local_nonprim) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_parseFromJSON failed [pp_dl_packet_count_ext]");
        goto end;
    }
    }
    }

    pp_maximum_response_time = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppMaximumResponseTime");
    if (pp_maximum_response_time) {
    if (!cJSON_IsNull(pp_maximum_response_time)) {
    pp_maximum_response_time_local_nonprim = OpenAPI_pp_maximum_response_time_parseFromJSON(pp_maximum_response_time);
    if (!pp_maximum_response_time_local_nonprim) {
        ogs_error("OpenAPI_pp_maximum_response_time_parseFromJSON failed [pp_maximum_response_time]");
        goto end;
    }
    }
    }

    pp_maximum_latency = cJSON_GetObjectItemCaseSensitive(communication_characteristicsJSON, "ppMaximumLatency");
    if (pp_maximum_latency) {
    if (!cJSON_IsNull(pp_maximum_latency)) {
    pp_maximum_latency_local_nonprim = OpenAPI_pp_maximum_latency_parseFromJSON(pp_maximum_latency);
    if (!pp_maximum_latency_local_nonprim) {
        ogs_error("OpenAPI_pp_maximum_latency_parseFromJSON failed [pp_maximum_latency]");
        goto end;
    }
    }
    }

    communication_characteristics_local_var = OpenAPI_communication_characteristics_create (
        pp_subs_reg_timer && cJSON_IsNull(pp_subs_reg_timer) ? true : false,
        pp_subs_reg_timer ? pp_subs_reg_timer_local_nonprim : NULL,
        pp_active_time && cJSON_IsNull(pp_active_time) ? true : false,
        pp_active_time ? pp_active_time_local_nonprim : NULL,
        pp_dl_packet_count && cJSON_IsNull(pp_dl_packet_count) ? true : false,
        pp_dl_packet_count ? true : false,
        pp_dl_packet_count ? pp_dl_packet_count->valuedouble : 0,
        pp_dl_packet_count_ext && cJSON_IsNull(pp_dl_packet_count_ext) ? true : false,
        pp_dl_packet_count_ext ? pp_dl_packet_count_ext_local_nonprim : NULL,
        pp_maximum_response_time && cJSON_IsNull(pp_maximum_response_time) ? true : false,
        pp_maximum_response_time ? pp_maximum_response_time_local_nonprim : NULL,
        pp_maximum_latency && cJSON_IsNull(pp_maximum_latency) ? true : false,
        pp_maximum_latency ? pp_maximum_latency_local_nonprim : NULL
    );

    return communication_characteristics_local_var;
end:
    if (pp_subs_reg_timer_local_nonprim) {
        OpenAPI_pp_subs_reg_timer_free(pp_subs_reg_timer_local_nonprim);
        pp_subs_reg_timer_local_nonprim = NULL;
    }
    if (pp_active_time_local_nonprim) {
        OpenAPI_pp_active_time_free(pp_active_time_local_nonprim);
        pp_active_time_local_nonprim = NULL;
    }
    if (pp_dl_packet_count_ext_local_nonprim) {
        OpenAPI_pp_dl_packet_count_ext_free(pp_dl_packet_count_ext_local_nonprim);
        pp_dl_packet_count_ext_local_nonprim = NULL;
    }
    if (pp_maximum_response_time_local_nonprim) {
        OpenAPI_pp_maximum_response_time_free(pp_maximum_response_time_local_nonprim);
        pp_maximum_response_time_local_nonprim = NULL;
    }
    if (pp_maximum_latency_local_nonprim) {
        OpenAPI_pp_maximum_latency_free(pp_maximum_latency_local_nonprim);
        pp_maximum_latency_local_nonprim = NULL;
    }
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

