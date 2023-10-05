
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_data.h"

OpenAPI_qos_data_t *OpenAPI_qos_data_create(
    char *qos_id,
    bool is__5qi,
    int _5qi,
    bool is_maxbr_ul_null,
    char *maxbr_ul,
    bool is_maxbr_dl_null,
    char *maxbr_dl,
    bool is_gbr_ul_null,
    char *gbr_ul,
    bool is_gbr_dl_null,
    char *gbr_dl,
    OpenAPI_arp_t *arp,
    bool is_qnc,
    int qnc,
    bool is_priority_level_null,
    bool is_priority_level,
    int priority_level,
    bool is_aver_window_null,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol_null,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_reflective_qos,
    int reflective_qos,
    char *sharing_key_dl,
    char *sharing_key_ul,
    bool is_max_packet_loss_rate_dl_null,
    bool is_max_packet_loss_rate_dl,
    int max_packet_loss_rate_dl,
    bool is_max_packet_loss_rate_ul_null,
    bool is_max_packet_loss_rate_ul,
    int max_packet_loss_rate_ul,
    bool is_def_qos_flow_indication,
    int def_qos_flow_indication,
    bool is_ext_max_data_burst_vol_null,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol,
    bool is_packet_delay_budget,
    int packet_delay_budget,
    char *packet_error_rate
)
{
    OpenAPI_qos_data_t *qos_data_local_var = ogs_malloc(sizeof(OpenAPI_qos_data_t));
    ogs_assert(qos_data_local_var);

    qos_data_local_var->qos_id = qos_id;
    qos_data_local_var->is__5qi = is__5qi;
    qos_data_local_var->_5qi = _5qi;
    qos_data_local_var->is_maxbr_ul_null = is_maxbr_ul_null;
    qos_data_local_var->maxbr_ul = maxbr_ul;
    qos_data_local_var->is_maxbr_dl_null = is_maxbr_dl_null;
    qos_data_local_var->maxbr_dl = maxbr_dl;
    qos_data_local_var->is_gbr_ul_null = is_gbr_ul_null;
    qos_data_local_var->gbr_ul = gbr_ul;
    qos_data_local_var->is_gbr_dl_null = is_gbr_dl_null;
    qos_data_local_var->gbr_dl = gbr_dl;
    qos_data_local_var->arp = arp;
    qos_data_local_var->is_qnc = is_qnc;
    qos_data_local_var->qnc = qnc;
    qos_data_local_var->is_priority_level_null = is_priority_level_null;
    qos_data_local_var->is_priority_level = is_priority_level;
    qos_data_local_var->priority_level = priority_level;
    qos_data_local_var->is_aver_window_null = is_aver_window_null;
    qos_data_local_var->is_aver_window = is_aver_window;
    qos_data_local_var->aver_window = aver_window;
    qos_data_local_var->is_max_data_burst_vol_null = is_max_data_burst_vol_null;
    qos_data_local_var->is_max_data_burst_vol = is_max_data_burst_vol;
    qos_data_local_var->max_data_burst_vol = max_data_burst_vol;
    qos_data_local_var->is_reflective_qos = is_reflective_qos;
    qos_data_local_var->reflective_qos = reflective_qos;
    qos_data_local_var->sharing_key_dl = sharing_key_dl;
    qos_data_local_var->sharing_key_ul = sharing_key_ul;
    qos_data_local_var->is_max_packet_loss_rate_dl_null = is_max_packet_loss_rate_dl_null;
    qos_data_local_var->is_max_packet_loss_rate_dl = is_max_packet_loss_rate_dl;
    qos_data_local_var->max_packet_loss_rate_dl = max_packet_loss_rate_dl;
    qos_data_local_var->is_max_packet_loss_rate_ul_null = is_max_packet_loss_rate_ul_null;
    qos_data_local_var->is_max_packet_loss_rate_ul = is_max_packet_loss_rate_ul;
    qos_data_local_var->max_packet_loss_rate_ul = max_packet_loss_rate_ul;
    qos_data_local_var->is_def_qos_flow_indication = is_def_qos_flow_indication;
    qos_data_local_var->def_qos_flow_indication = def_qos_flow_indication;
    qos_data_local_var->is_ext_max_data_burst_vol_null = is_ext_max_data_burst_vol_null;
    qos_data_local_var->is_ext_max_data_burst_vol = is_ext_max_data_burst_vol;
    qos_data_local_var->ext_max_data_burst_vol = ext_max_data_burst_vol;
    qos_data_local_var->is_packet_delay_budget = is_packet_delay_budget;
    qos_data_local_var->packet_delay_budget = packet_delay_budget;
    qos_data_local_var->packet_error_rate = packet_error_rate;

    return qos_data_local_var;
}

void OpenAPI_qos_data_free(OpenAPI_qos_data_t *qos_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_data) {
        return;
    }
    if (qos_data->qos_id) {
        ogs_free(qos_data->qos_id);
        qos_data->qos_id = NULL;
    }
    if (qos_data->maxbr_ul) {
        ogs_free(qos_data->maxbr_ul);
        qos_data->maxbr_ul = NULL;
    }
    if (qos_data->maxbr_dl) {
        ogs_free(qos_data->maxbr_dl);
        qos_data->maxbr_dl = NULL;
    }
    if (qos_data->gbr_ul) {
        ogs_free(qos_data->gbr_ul);
        qos_data->gbr_ul = NULL;
    }
    if (qos_data->gbr_dl) {
        ogs_free(qos_data->gbr_dl);
        qos_data->gbr_dl = NULL;
    }
    if (qos_data->arp) {
        OpenAPI_arp_free(qos_data->arp);
        qos_data->arp = NULL;
    }
    if (qos_data->sharing_key_dl) {
        ogs_free(qos_data->sharing_key_dl);
        qos_data->sharing_key_dl = NULL;
    }
    if (qos_data->sharing_key_ul) {
        ogs_free(qos_data->sharing_key_ul);
        qos_data->sharing_key_ul = NULL;
    }
    if (qos_data->packet_error_rate) {
        ogs_free(qos_data->packet_error_rate);
        qos_data->packet_error_rate = NULL;
    }
    ogs_free(qos_data);
}

cJSON *OpenAPI_qos_data_convertToJSON(OpenAPI_qos_data_t *qos_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_data == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [QosData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!qos_data->qos_id) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [qos_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "qosId", qos_data->qos_id) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [qos_id]");
        goto end;
    }

    if (qos_data->is__5qi) {
    if (cJSON_AddNumberToObject(item, "5qi", qos_data->_5qi) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [_5qi]");
        goto end;
    }
    }

    if (qos_data->maxbr_ul) {
    if (cJSON_AddStringToObject(item, "maxbrUl", qos_data->maxbr_ul) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [maxbr_ul]");
        goto end;
    }
    } else if (qos_data->is_maxbr_ul_null) {
        if (cJSON_AddNullToObject(item, "maxbrUl") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [maxbr_ul]");
            goto end;
        }
    }

    if (qos_data->maxbr_dl) {
    if (cJSON_AddStringToObject(item, "maxbrDl", qos_data->maxbr_dl) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [maxbr_dl]");
        goto end;
    }
    } else if (qos_data->is_maxbr_dl_null) {
        if (cJSON_AddNullToObject(item, "maxbrDl") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [maxbr_dl]");
            goto end;
        }
    }

    if (qos_data->gbr_ul) {
    if (cJSON_AddStringToObject(item, "gbrUl", qos_data->gbr_ul) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [gbr_ul]");
        goto end;
    }
    } else if (qos_data->is_gbr_ul_null) {
        if (cJSON_AddNullToObject(item, "gbrUl") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [gbr_ul]");
            goto end;
        }
    }

    if (qos_data->gbr_dl) {
    if (cJSON_AddStringToObject(item, "gbrDl", qos_data->gbr_dl) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [gbr_dl]");
        goto end;
    }
    } else if (qos_data->is_gbr_dl_null) {
        if (cJSON_AddNullToObject(item, "gbrDl") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [gbr_dl]");
            goto end;
        }
    }

    if (qos_data->arp) {
    cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(qos_data->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [arp]");
        goto end;
    }
    }

    if (qos_data->is_qnc) {
    if (cJSON_AddBoolToObject(item, "qnc", qos_data->qnc) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [qnc]");
        goto end;
    }
    }

    if (qos_data->is_priority_level) {
    if (cJSON_AddNumberToObject(item, "priorityLevel", qos_data->priority_level) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [priority_level]");
        goto end;
    }
    } else if (qos_data->is_priority_level_null) {
        if (cJSON_AddNullToObject(item, "priorityLevel") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [priority_level]");
            goto end;
        }
    }

    if (qos_data->is_aver_window) {
    if (cJSON_AddNumberToObject(item, "averWindow", qos_data->aver_window) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [aver_window]");
        goto end;
    }
    } else if (qos_data->is_aver_window_null) {
        if (cJSON_AddNullToObject(item, "averWindow") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [aver_window]");
            goto end;
        }
    }

    if (qos_data->is_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "maxDataBurstVol", qos_data->max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [max_data_burst_vol]");
        goto end;
    }
    } else if (qos_data->is_max_data_burst_vol_null) {
        if (cJSON_AddNullToObject(item, "maxDataBurstVol") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [max_data_burst_vol]");
            goto end;
        }
    }

    if (qos_data->is_reflective_qos) {
    if (cJSON_AddBoolToObject(item, "reflectiveQos", qos_data->reflective_qos) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [reflective_qos]");
        goto end;
    }
    }

    if (qos_data->sharing_key_dl) {
    if (cJSON_AddStringToObject(item, "sharingKeyDl", qos_data->sharing_key_dl) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [sharing_key_dl]");
        goto end;
    }
    }

    if (qos_data->sharing_key_ul) {
    if (cJSON_AddStringToObject(item, "sharingKeyUl", qos_data->sharing_key_ul) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [sharing_key_ul]");
        goto end;
    }
    }

    if (qos_data->is_max_packet_loss_rate_dl) {
    if (cJSON_AddNumberToObject(item, "maxPacketLossRateDl", qos_data->max_packet_loss_rate_dl) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [max_packet_loss_rate_dl]");
        goto end;
    }
    } else if (qos_data->is_max_packet_loss_rate_dl_null) {
        if (cJSON_AddNullToObject(item, "maxPacketLossRateDl") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [max_packet_loss_rate_dl]");
            goto end;
        }
    }

    if (qos_data->is_max_packet_loss_rate_ul) {
    if (cJSON_AddNumberToObject(item, "maxPacketLossRateUl", qos_data->max_packet_loss_rate_ul) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [max_packet_loss_rate_ul]");
        goto end;
    }
    } else if (qos_data->is_max_packet_loss_rate_ul_null) {
        if (cJSON_AddNullToObject(item, "maxPacketLossRateUl") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [max_packet_loss_rate_ul]");
            goto end;
        }
    }

    if (qos_data->is_def_qos_flow_indication) {
    if (cJSON_AddBoolToObject(item, "defQosFlowIndication", qos_data->def_qos_flow_indication) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [def_qos_flow_indication]");
        goto end;
    }
    }

    if (qos_data->is_ext_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "extMaxDataBurstVol", qos_data->ext_max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    } else if (qos_data->is_ext_max_data_burst_vol_null) {
        if (cJSON_AddNullToObject(item, "extMaxDataBurstVol") == NULL) {
            ogs_error("OpenAPI_qos_data_convertToJSON() failed [ext_max_data_burst_vol]");
            goto end;
        }
    }

    if (qos_data->is_packet_delay_budget) {
    if (cJSON_AddNumberToObject(item, "packetDelayBudget", qos_data->packet_delay_budget) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [packet_delay_budget]");
        goto end;
    }
    }

    if (qos_data->packet_error_rate) {
    if (cJSON_AddStringToObject(item, "packetErrorRate", qos_data->packet_error_rate) == NULL) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed [packet_error_rate]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_data_t *OpenAPI_qos_data_parseFromJSON(cJSON *qos_dataJSON)
{
    OpenAPI_qos_data_t *qos_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qos_id = NULL;
    cJSON *_5qi = NULL;
    cJSON *maxbr_ul = NULL;
    cJSON *maxbr_dl = NULL;
    cJSON *gbr_ul = NULL;
    cJSON *gbr_dl = NULL;
    cJSON *arp = NULL;
    OpenAPI_arp_t *arp_local_nonprim = NULL;
    cJSON *qnc = NULL;
    cJSON *priority_level = NULL;
    cJSON *aver_window = NULL;
    cJSON *max_data_burst_vol = NULL;
    cJSON *reflective_qos = NULL;
    cJSON *sharing_key_dl = NULL;
    cJSON *sharing_key_ul = NULL;
    cJSON *max_packet_loss_rate_dl = NULL;
    cJSON *max_packet_loss_rate_ul = NULL;
    cJSON *def_qos_flow_indication = NULL;
    cJSON *ext_max_data_burst_vol = NULL;
    cJSON *packet_delay_budget = NULL;
    cJSON *packet_error_rate = NULL;
    qos_id = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "qosId");
    if (!qos_id) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [qos_id]");
        goto end;
    }
    if (!cJSON_IsString(qos_id)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [qos_id]");
        goto end;
    }

    _5qi = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "5qi");
    if (_5qi) {
    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [_5qi]");
        goto end;
    }
    }

    maxbr_ul = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "maxbrUl");
    if (maxbr_ul) {
    if (!cJSON_IsNull(maxbr_ul)) {
    if (!cJSON_IsString(maxbr_ul) && !cJSON_IsNull(maxbr_ul)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [maxbr_ul]");
        goto end;
    }
    }
    }

    maxbr_dl = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "maxbrDl");
    if (maxbr_dl) {
    if (!cJSON_IsNull(maxbr_dl)) {
    if (!cJSON_IsString(maxbr_dl) && !cJSON_IsNull(maxbr_dl)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [maxbr_dl]");
        goto end;
    }
    }
    }

    gbr_ul = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "gbrUl");
    if (gbr_ul) {
    if (!cJSON_IsNull(gbr_ul)) {
    if (!cJSON_IsString(gbr_ul) && !cJSON_IsNull(gbr_ul)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [gbr_ul]");
        goto end;
    }
    }
    }

    gbr_dl = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "gbrDl");
    if (gbr_dl) {
    if (!cJSON_IsNull(gbr_dl)) {
    if (!cJSON_IsString(gbr_dl) && !cJSON_IsNull(gbr_dl)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [gbr_dl]");
        goto end;
    }
    }
    }

    arp = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "arp");
    if (arp) {
    arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);
    if (!arp_local_nonprim) {
        ogs_error("OpenAPI_arp_parseFromJSON failed [arp]");
        goto end;
    }
    }

    qnc = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "qnc");
    if (qnc) {
    if (!cJSON_IsBool(qnc)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [qnc]");
        goto end;
    }
    }

    priority_level = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "priorityLevel");
    if (priority_level) {
    if (!cJSON_IsNull(priority_level)) {
    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [priority_level]");
        goto end;
    }
    }
    }

    aver_window = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "averWindow");
    if (aver_window) {
    if (!cJSON_IsNull(aver_window)) {
    if (!cJSON_IsNumber(aver_window)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [aver_window]");
        goto end;
    }
    }
    }

    max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "maxDataBurstVol");
    if (max_data_burst_vol) {
    if (!cJSON_IsNull(max_data_burst_vol)) {
    if (!cJSON_IsNumber(max_data_burst_vol)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }
    }

    reflective_qos = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "reflectiveQos");
    if (reflective_qos) {
    if (!cJSON_IsBool(reflective_qos)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [reflective_qos]");
        goto end;
    }
    }

    sharing_key_dl = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "sharingKeyDl");
    if (sharing_key_dl) {
    if (!cJSON_IsString(sharing_key_dl) && !cJSON_IsNull(sharing_key_dl)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [sharing_key_dl]");
        goto end;
    }
    }

    sharing_key_ul = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "sharingKeyUl");
    if (sharing_key_ul) {
    if (!cJSON_IsString(sharing_key_ul) && !cJSON_IsNull(sharing_key_ul)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [sharing_key_ul]");
        goto end;
    }
    }

    max_packet_loss_rate_dl = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "maxPacketLossRateDl");
    if (max_packet_loss_rate_dl) {
    if (!cJSON_IsNull(max_packet_loss_rate_dl)) {
    if (!cJSON_IsNumber(max_packet_loss_rate_dl)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [max_packet_loss_rate_dl]");
        goto end;
    }
    }
    }

    max_packet_loss_rate_ul = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "maxPacketLossRateUl");
    if (max_packet_loss_rate_ul) {
    if (!cJSON_IsNull(max_packet_loss_rate_ul)) {
    if (!cJSON_IsNumber(max_packet_loss_rate_ul)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [max_packet_loss_rate_ul]");
        goto end;
    }
    }
    }

    def_qos_flow_indication = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "defQosFlowIndication");
    if (def_qos_flow_indication) {
    if (!cJSON_IsBool(def_qos_flow_indication)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [def_qos_flow_indication]");
        goto end;
    }
    }

    ext_max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "extMaxDataBurstVol");
    if (ext_max_data_burst_vol) {
    if (!cJSON_IsNull(ext_max_data_burst_vol)) {
    if (!cJSON_IsNumber(ext_max_data_burst_vol)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }
    }

    packet_delay_budget = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "packetDelayBudget");
    if (packet_delay_budget) {
    if (!cJSON_IsNumber(packet_delay_budget)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }
    }

    packet_error_rate = cJSON_GetObjectItemCaseSensitive(qos_dataJSON, "packetErrorRate");
    if (packet_error_rate) {
    if (!cJSON_IsString(packet_error_rate) && !cJSON_IsNull(packet_error_rate)) {
        ogs_error("OpenAPI_qos_data_parseFromJSON() failed [packet_error_rate]");
        goto end;
    }
    }

    qos_data_local_var = OpenAPI_qos_data_create (
        ogs_strdup(qos_id->valuestring),
        _5qi ? true : false,
        _5qi ? _5qi->valuedouble : 0,
        maxbr_ul && cJSON_IsNull(maxbr_ul) ? true : false,
        maxbr_ul && !cJSON_IsNull(maxbr_ul) ? ogs_strdup(maxbr_ul->valuestring) : NULL,
        maxbr_dl && cJSON_IsNull(maxbr_dl) ? true : false,
        maxbr_dl && !cJSON_IsNull(maxbr_dl) ? ogs_strdup(maxbr_dl->valuestring) : NULL,
        gbr_ul && cJSON_IsNull(gbr_ul) ? true : false,
        gbr_ul && !cJSON_IsNull(gbr_ul) ? ogs_strdup(gbr_ul->valuestring) : NULL,
        gbr_dl && cJSON_IsNull(gbr_dl) ? true : false,
        gbr_dl && !cJSON_IsNull(gbr_dl) ? ogs_strdup(gbr_dl->valuestring) : NULL,
        arp ? arp_local_nonprim : NULL,
        qnc ? true : false,
        qnc ? qnc->valueint : 0,
        priority_level && cJSON_IsNull(priority_level) ? true : false,
        priority_level ? true : false,
        priority_level ? priority_level->valuedouble : 0,
        aver_window && cJSON_IsNull(aver_window) ? true : false,
        aver_window ? true : false,
        aver_window ? aver_window->valuedouble : 0,
        max_data_burst_vol && cJSON_IsNull(max_data_burst_vol) ? true : false,
        max_data_burst_vol ? true : false,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        reflective_qos ? true : false,
        reflective_qos ? reflective_qos->valueint : 0,
        sharing_key_dl && !cJSON_IsNull(sharing_key_dl) ? ogs_strdup(sharing_key_dl->valuestring) : NULL,
        sharing_key_ul && !cJSON_IsNull(sharing_key_ul) ? ogs_strdup(sharing_key_ul->valuestring) : NULL,
        max_packet_loss_rate_dl && cJSON_IsNull(max_packet_loss_rate_dl) ? true : false,
        max_packet_loss_rate_dl ? true : false,
        max_packet_loss_rate_dl ? max_packet_loss_rate_dl->valuedouble : 0,
        max_packet_loss_rate_ul && cJSON_IsNull(max_packet_loss_rate_ul) ? true : false,
        max_packet_loss_rate_ul ? true : false,
        max_packet_loss_rate_ul ? max_packet_loss_rate_ul->valuedouble : 0,
        def_qos_flow_indication ? true : false,
        def_qos_flow_indication ? def_qos_flow_indication->valueint : 0,
        ext_max_data_burst_vol && cJSON_IsNull(ext_max_data_burst_vol) ? true : false,
        ext_max_data_burst_vol ? true : false,
        ext_max_data_burst_vol ? ext_max_data_burst_vol->valuedouble : 0,
        packet_delay_budget ? true : false,
        packet_delay_budget ? packet_delay_budget->valuedouble : 0,
        packet_error_rate && !cJSON_IsNull(packet_error_rate) ? ogs_strdup(packet_error_rate->valuestring) : NULL
    );

    return qos_data_local_var;
end:
    if (arp_local_nonprim) {
        OpenAPI_arp_free(arp_local_nonprim);
        arp_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_qos_data_t *OpenAPI_qos_data_copy(OpenAPI_qos_data_t *dst, OpenAPI_qos_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_data_convertToJSON() failed");
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

    OpenAPI_qos_data_free(dst);
    dst = OpenAPI_qos_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

