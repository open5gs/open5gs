
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "perf_data.h"

OpenAPI_perf_data_t *OpenAPI_perf_data_create(
    char *avg_traffic_rate,
    char *max_traffic_rate,
    char *min_traffic_rate,
    char *agg_traffic_rate,
    bool is_var_traffic_rate,
    float var_traffic_rate,
    OpenAPI_list_t *traf_rate_ue_ids,
    bool is_ave_packet_delay,
    int ave_packet_delay,
    bool is_max_packet_delay,
    int max_packet_delay,
    bool is_var_packet_delay,
    float var_packet_delay,
    OpenAPI_list_t *pack_delay_ue_ids,
    bool is_avg_packet_loss_rate,
    int avg_packet_loss_rate,
    bool is_max_packet_loss_rate,
    int max_packet_loss_rate,
    bool is_var_packet_loss_rate,
    float var_packet_loss_rate,
    OpenAPI_list_t *pack_loss_ue_ids,
    bool is_num_of_ue,
    int num_of_ue
)
{
    OpenAPI_perf_data_t *perf_data_local_var = ogs_malloc(sizeof(OpenAPI_perf_data_t));
    ogs_assert(perf_data_local_var);

    perf_data_local_var->avg_traffic_rate = avg_traffic_rate;
    perf_data_local_var->max_traffic_rate = max_traffic_rate;
    perf_data_local_var->min_traffic_rate = min_traffic_rate;
    perf_data_local_var->agg_traffic_rate = agg_traffic_rate;
    perf_data_local_var->is_var_traffic_rate = is_var_traffic_rate;
    perf_data_local_var->var_traffic_rate = var_traffic_rate;
    perf_data_local_var->traf_rate_ue_ids = traf_rate_ue_ids;
    perf_data_local_var->is_ave_packet_delay = is_ave_packet_delay;
    perf_data_local_var->ave_packet_delay = ave_packet_delay;
    perf_data_local_var->is_max_packet_delay = is_max_packet_delay;
    perf_data_local_var->max_packet_delay = max_packet_delay;
    perf_data_local_var->is_var_packet_delay = is_var_packet_delay;
    perf_data_local_var->var_packet_delay = var_packet_delay;
    perf_data_local_var->pack_delay_ue_ids = pack_delay_ue_ids;
    perf_data_local_var->is_avg_packet_loss_rate = is_avg_packet_loss_rate;
    perf_data_local_var->avg_packet_loss_rate = avg_packet_loss_rate;
    perf_data_local_var->is_max_packet_loss_rate = is_max_packet_loss_rate;
    perf_data_local_var->max_packet_loss_rate = max_packet_loss_rate;
    perf_data_local_var->is_var_packet_loss_rate = is_var_packet_loss_rate;
    perf_data_local_var->var_packet_loss_rate = var_packet_loss_rate;
    perf_data_local_var->pack_loss_ue_ids = pack_loss_ue_ids;
    perf_data_local_var->is_num_of_ue = is_num_of_ue;
    perf_data_local_var->num_of_ue = num_of_ue;

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
    if (perf_data->min_traffic_rate) {
        ogs_free(perf_data->min_traffic_rate);
        perf_data->min_traffic_rate = NULL;
    }
    if (perf_data->agg_traffic_rate) {
        ogs_free(perf_data->agg_traffic_rate);
        perf_data->agg_traffic_rate = NULL;
    }
    if (perf_data->traf_rate_ue_ids) {
        OpenAPI_list_for_each(perf_data->traf_rate_ue_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(perf_data->traf_rate_ue_ids);
        perf_data->traf_rate_ue_ids = NULL;
    }
    if (perf_data->pack_delay_ue_ids) {
        OpenAPI_list_for_each(perf_data->pack_delay_ue_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(perf_data->pack_delay_ue_ids);
        perf_data->pack_delay_ue_ids = NULL;
    }
    if (perf_data->pack_loss_ue_ids) {
        OpenAPI_list_for_each(perf_data->pack_loss_ue_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(perf_data->pack_loss_ue_ids);
        perf_data->pack_loss_ue_ids = NULL;
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

    if (perf_data->min_traffic_rate) {
    if (cJSON_AddStringToObject(item, "minTrafficRate", perf_data->min_traffic_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [min_traffic_rate]");
        goto end;
    }
    }

    if (perf_data->agg_traffic_rate) {
    if (cJSON_AddStringToObject(item, "aggTrafficRate", perf_data->agg_traffic_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [agg_traffic_rate]");
        goto end;
    }
    }

    if (perf_data->is_var_traffic_rate) {
    if (cJSON_AddNumberToObject(item, "varTrafficRate", perf_data->var_traffic_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [var_traffic_rate]");
        goto end;
    }
    }

    if (perf_data->traf_rate_ue_ids) {
    cJSON *traf_rate_ue_idsList = cJSON_AddArrayToObject(item, "trafRateUeIds");
    if (traf_rate_ue_idsList == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [traf_rate_ue_ids]");
        goto end;
    }
    OpenAPI_list_for_each(perf_data->traf_rate_ue_ids, node) {
        if (cJSON_AddStringToObject(traf_rate_ue_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_perf_data_convertToJSON() failed [traf_rate_ue_ids]");
            goto end;
        }
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

    if (perf_data->is_var_packet_delay) {
    if (cJSON_AddNumberToObject(item, "varPacketDelay", perf_data->var_packet_delay) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [var_packet_delay]");
        goto end;
    }
    }

    if (perf_data->pack_delay_ue_ids) {
    cJSON *pack_delay_ue_idsList = cJSON_AddArrayToObject(item, "packDelayUeIds");
    if (pack_delay_ue_idsList == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [pack_delay_ue_ids]");
        goto end;
    }
    OpenAPI_list_for_each(perf_data->pack_delay_ue_ids, node) {
        if (cJSON_AddStringToObject(pack_delay_ue_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_perf_data_convertToJSON() failed [pack_delay_ue_ids]");
            goto end;
        }
    }
    }

    if (perf_data->is_avg_packet_loss_rate) {
    if (cJSON_AddNumberToObject(item, "avgPacketLossRate", perf_data->avg_packet_loss_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [avg_packet_loss_rate]");
        goto end;
    }
    }

    if (perf_data->is_max_packet_loss_rate) {
    if (cJSON_AddNumberToObject(item, "maxPacketLossRate", perf_data->max_packet_loss_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [max_packet_loss_rate]");
        goto end;
    }
    }

    if (perf_data->is_var_packet_loss_rate) {
    if (cJSON_AddNumberToObject(item, "varPacketLossRate", perf_data->var_packet_loss_rate) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [var_packet_loss_rate]");
        goto end;
    }
    }

    if (perf_data->pack_loss_ue_ids) {
    cJSON *pack_loss_ue_idsList = cJSON_AddArrayToObject(item, "packLossUeIds");
    if (pack_loss_ue_idsList == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [pack_loss_ue_ids]");
        goto end;
    }
    OpenAPI_list_for_each(perf_data->pack_loss_ue_ids, node) {
        if (cJSON_AddStringToObject(pack_loss_ue_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_perf_data_convertToJSON() failed [pack_loss_ue_ids]");
            goto end;
        }
    }
    }

    if (perf_data->is_num_of_ue) {
    if (cJSON_AddNumberToObject(item, "numOfUe", perf_data->num_of_ue) == NULL) {
        ogs_error("OpenAPI_perf_data_convertToJSON() failed [num_of_ue]");
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
    cJSON *min_traffic_rate = NULL;
    cJSON *agg_traffic_rate = NULL;
    cJSON *var_traffic_rate = NULL;
    cJSON *traf_rate_ue_ids = NULL;
    OpenAPI_list_t *traf_rate_ue_idsList = NULL;
    cJSON *ave_packet_delay = NULL;
    cJSON *max_packet_delay = NULL;
    cJSON *var_packet_delay = NULL;
    cJSON *pack_delay_ue_ids = NULL;
    OpenAPI_list_t *pack_delay_ue_idsList = NULL;
    cJSON *avg_packet_loss_rate = NULL;
    cJSON *max_packet_loss_rate = NULL;
    cJSON *var_packet_loss_rate = NULL;
    cJSON *pack_loss_ue_ids = NULL;
    OpenAPI_list_t *pack_loss_ue_idsList = NULL;
    cJSON *num_of_ue = NULL;
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

    min_traffic_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "minTrafficRate");
    if (min_traffic_rate) {
    if (!cJSON_IsString(min_traffic_rate) && !cJSON_IsNull(min_traffic_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [min_traffic_rate]");
        goto end;
    }
    }

    agg_traffic_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "aggTrafficRate");
    if (agg_traffic_rate) {
    if (!cJSON_IsString(agg_traffic_rate) && !cJSON_IsNull(agg_traffic_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [agg_traffic_rate]");
        goto end;
    }
    }

    var_traffic_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "varTrafficRate");
    if (var_traffic_rate) {
    if (!cJSON_IsNumber(var_traffic_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [var_traffic_rate]");
        goto end;
    }
    }

    traf_rate_ue_ids = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "trafRateUeIds");
    if (traf_rate_ue_ids) {
        cJSON *traf_rate_ue_ids_local = NULL;
        if (!cJSON_IsArray(traf_rate_ue_ids)) {
            ogs_error("OpenAPI_perf_data_parseFromJSON() failed [traf_rate_ue_ids]");
            goto end;
        }

        traf_rate_ue_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(traf_rate_ue_ids_local, traf_rate_ue_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(traf_rate_ue_ids_local)) {
                ogs_error("OpenAPI_perf_data_parseFromJSON() failed [traf_rate_ue_ids]");
                goto end;
            }
            OpenAPI_list_add(traf_rate_ue_idsList, ogs_strdup(traf_rate_ue_ids_local->valuestring));
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

    var_packet_delay = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "varPacketDelay");
    if (var_packet_delay) {
    if (!cJSON_IsNumber(var_packet_delay)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [var_packet_delay]");
        goto end;
    }
    }

    pack_delay_ue_ids = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "packDelayUeIds");
    if (pack_delay_ue_ids) {
        cJSON *pack_delay_ue_ids_local = NULL;
        if (!cJSON_IsArray(pack_delay_ue_ids)) {
            ogs_error("OpenAPI_perf_data_parseFromJSON() failed [pack_delay_ue_ids]");
            goto end;
        }

        pack_delay_ue_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pack_delay_ue_ids_local, pack_delay_ue_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(pack_delay_ue_ids_local)) {
                ogs_error("OpenAPI_perf_data_parseFromJSON() failed [pack_delay_ue_ids]");
                goto end;
            }
            OpenAPI_list_add(pack_delay_ue_idsList, ogs_strdup(pack_delay_ue_ids_local->valuestring));
        }
    }

    avg_packet_loss_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "avgPacketLossRate");
    if (avg_packet_loss_rate) {
    if (!cJSON_IsNumber(avg_packet_loss_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [avg_packet_loss_rate]");
        goto end;
    }
    }

    max_packet_loss_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "maxPacketLossRate");
    if (max_packet_loss_rate) {
    if (!cJSON_IsNumber(max_packet_loss_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [max_packet_loss_rate]");
        goto end;
    }
    }

    var_packet_loss_rate = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "varPacketLossRate");
    if (var_packet_loss_rate) {
    if (!cJSON_IsNumber(var_packet_loss_rate)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [var_packet_loss_rate]");
        goto end;
    }
    }

    pack_loss_ue_ids = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "packLossUeIds");
    if (pack_loss_ue_ids) {
        cJSON *pack_loss_ue_ids_local = NULL;
        if (!cJSON_IsArray(pack_loss_ue_ids)) {
            ogs_error("OpenAPI_perf_data_parseFromJSON() failed [pack_loss_ue_ids]");
            goto end;
        }

        pack_loss_ue_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pack_loss_ue_ids_local, pack_loss_ue_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(pack_loss_ue_ids_local)) {
                ogs_error("OpenAPI_perf_data_parseFromJSON() failed [pack_loss_ue_ids]");
                goto end;
            }
            OpenAPI_list_add(pack_loss_ue_idsList, ogs_strdup(pack_loss_ue_ids_local->valuestring));
        }
    }

    num_of_ue = cJSON_GetObjectItemCaseSensitive(perf_dataJSON, "numOfUe");
    if (num_of_ue) {
    if (!cJSON_IsNumber(num_of_ue)) {
        ogs_error("OpenAPI_perf_data_parseFromJSON() failed [num_of_ue]");
        goto end;
    }
    }

    perf_data_local_var = OpenAPI_perf_data_create (
        avg_traffic_rate && !cJSON_IsNull(avg_traffic_rate) ? ogs_strdup(avg_traffic_rate->valuestring) : NULL,
        max_traffic_rate && !cJSON_IsNull(max_traffic_rate) ? ogs_strdup(max_traffic_rate->valuestring) : NULL,
        min_traffic_rate && !cJSON_IsNull(min_traffic_rate) ? ogs_strdup(min_traffic_rate->valuestring) : NULL,
        agg_traffic_rate && !cJSON_IsNull(agg_traffic_rate) ? ogs_strdup(agg_traffic_rate->valuestring) : NULL,
        var_traffic_rate ? true : false,
        var_traffic_rate ? var_traffic_rate->valuedouble : 0,
        traf_rate_ue_ids ? traf_rate_ue_idsList : NULL,
        ave_packet_delay ? true : false,
        ave_packet_delay ? ave_packet_delay->valuedouble : 0,
        max_packet_delay ? true : false,
        max_packet_delay ? max_packet_delay->valuedouble : 0,
        var_packet_delay ? true : false,
        var_packet_delay ? var_packet_delay->valuedouble : 0,
        pack_delay_ue_ids ? pack_delay_ue_idsList : NULL,
        avg_packet_loss_rate ? true : false,
        avg_packet_loss_rate ? avg_packet_loss_rate->valuedouble : 0,
        max_packet_loss_rate ? true : false,
        max_packet_loss_rate ? max_packet_loss_rate->valuedouble : 0,
        var_packet_loss_rate ? true : false,
        var_packet_loss_rate ? var_packet_loss_rate->valuedouble : 0,
        pack_loss_ue_ids ? pack_loss_ue_idsList : NULL,
        num_of_ue ? true : false,
        num_of_ue ? num_of_ue->valuedouble : 0
    );

    return perf_data_local_var;
end:
    if (traf_rate_ue_idsList) {
        OpenAPI_list_for_each(traf_rate_ue_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traf_rate_ue_idsList);
        traf_rate_ue_idsList = NULL;
    }
    if (pack_delay_ue_idsList) {
        OpenAPI_list_for_each(pack_delay_ue_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pack_delay_ue_idsList);
        pack_delay_ue_idsList = NULL;
    }
    if (pack_loss_ue_idsList) {
        OpenAPI_list_for_each(pack_loss_ue_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pack_loss_ue_idsList);
        pack_loss_ue_idsList = NULL;
    }
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

