
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "e2e_data_vol_trans_time_req.h"

OpenAPI_e2e_data_vol_trans_time_req_t *OpenAPI_e2e_data_vol_trans_time_req_create(
    OpenAPI_e2e_data_vol_trans_time_criterion_e criterion,
    OpenAPI_matching_direction_e order,
    bool is_high_trans_tm_thr,
    int high_trans_tm_thr,
    bool is_low_trans_tm_thr,
    int low_trans_tm_thr,
    bool is_repeat_data_trans,
    int repeat_data_trans,
    bool is_ts_interval_data_trans,
    int ts_interval_data_trans,
    bool is_data_volume_null,
    OpenAPI_data_volume_t *data_volume,
    bool is_max_number_ues,
    int max_number_ues
)
{
    OpenAPI_e2e_data_vol_trans_time_req_t *e2e_data_vol_trans_time_req_local_var = ogs_malloc(sizeof(OpenAPI_e2e_data_vol_trans_time_req_t));
    ogs_assert(e2e_data_vol_trans_time_req_local_var);

    e2e_data_vol_trans_time_req_local_var->criterion = criterion;
    e2e_data_vol_trans_time_req_local_var->order = order;
    e2e_data_vol_trans_time_req_local_var->is_high_trans_tm_thr = is_high_trans_tm_thr;
    e2e_data_vol_trans_time_req_local_var->high_trans_tm_thr = high_trans_tm_thr;
    e2e_data_vol_trans_time_req_local_var->is_low_trans_tm_thr = is_low_trans_tm_thr;
    e2e_data_vol_trans_time_req_local_var->low_trans_tm_thr = low_trans_tm_thr;
    e2e_data_vol_trans_time_req_local_var->is_repeat_data_trans = is_repeat_data_trans;
    e2e_data_vol_trans_time_req_local_var->repeat_data_trans = repeat_data_trans;
    e2e_data_vol_trans_time_req_local_var->is_ts_interval_data_trans = is_ts_interval_data_trans;
    e2e_data_vol_trans_time_req_local_var->ts_interval_data_trans = ts_interval_data_trans;
    e2e_data_vol_trans_time_req_local_var->is_data_volume_null = is_data_volume_null;
    e2e_data_vol_trans_time_req_local_var->data_volume = data_volume;
    e2e_data_vol_trans_time_req_local_var->is_max_number_ues = is_max_number_ues;
    e2e_data_vol_trans_time_req_local_var->max_number_ues = max_number_ues;

    return e2e_data_vol_trans_time_req_local_var;
}

void OpenAPI_e2e_data_vol_trans_time_req_free(OpenAPI_e2e_data_vol_trans_time_req_t *e2e_data_vol_trans_time_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == e2e_data_vol_trans_time_req) {
        return;
    }
    if (e2e_data_vol_trans_time_req->data_volume) {
        OpenAPI_data_volume_free(e2e_data_vol_trans_time_req->data_volume);
        e2e_data_vol_trans_time_req->data_volume = NULL;
    }
    ogs_free(e2e_data_vol_trans_time_req);
}

cJSON *OpenAPI_e2e_data_vol_trans_time_req_convertToJSON(OpenAPI_e2e_data_vol_trans_time_req_t *e2e_data_vol_trans_time_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (e2e_data_vol_trans_time_req == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [E2eDataVolTransTimeReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (e2e_data_vol_trans_time_req->criterion != OpenAPI_e2e_data_vol_trans_time_criterion_NULL) {
    if (cJSON_AddStringToObject(item, "criterion", OpenAPI_e2e_data_vol_trans_time_criterion_ToString(e2e_data_vol_trans_time_req->criterion)) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [criterion]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_req->order != OpenAPI_matching_direction_NULL) {
    if (cJSON_AddStringToObject(item, "order", OpenAPI_matching_direction_ToString(e2e_data_vol_trans_time_req->order)) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [order]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_req->is_high_trans_tm_thr) {
    if (cJSON_AddNumberToObject(item, "highTransTmThr", e2e_data_vol_trans_time_req->high_trans_tm_thr) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [high_trans_tm_thr]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_req->is_low_trans_tm_thr) {
    if (cJSON_AddNumberToObject(item, "lowTransTmThr", e2e_data_vol_trans_time_req->low_trans_tm_thr) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [low_trans_tm_thr]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_req->is_repeat_data_trans) {
    if (cJSON_AddNumberToObject(item, "repeatDataTrans", e2e_data_vol_trans_time_req->repeat_data_trans) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [repeat_data_trans]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_req->is_ts_interval_data_trans) {
    if (cJSON_AddNumberToObject(item, "tsIntervalDataTrans", e2e_data_vol_trans_time_req->ts_interval_data_trans) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [ts_interval_data_trans]");
        goto end;
    }
    }

    if (e2e_data_vol_trans_time_req->data_volume) {
    cJSON *data_volume_local_JSON = OpenAPI_data_volume_convertToJSON(e2e_data_vol_trans_time_req->data_volume);
    if (data_volume_local_JSON == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [data_volume]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dataVolume", data_volume_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [data_volume]");
        goto end;
    }
    } else if (e2e_data_vol_trans_time_req->is_data_volume_null) {
        if (cJSON_AddNullToObject(item, "dataVolume") == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [data_volume]");
            goto end;
        }
    }

    if (e2e_data_vol_trans_time_req->is_max_number_ues) {
    if (cJSON_AddNumberToObject(item, "maxNumberUes", e2e_data_vol_trans_time_req->max_number_ues) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed [max_number_ues]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_e2e_data_vol_trans_time_req_t *OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON(cJSON *e2e_data_vol_trans_time_reqJSON)
{
    OpenAPI_e2e_data_vol_trans_time_req_t *e2e_data_vol_trans_time_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *criterion = NULL;
    OpenAPI_e2e_data_vol_trans_time_criterion_e criterionVariable = 0;
    cJSON *order = NULL;
    OpenAPI_matching_direction_e orderVariable = 0;
    cJSON *high_trans_tm_thr = NULL;
    cJSON *low_trans_tm_thr = NULL;
    cJSON *repeat_data_trans = NULL;
    cJSON *ts_interval_data_trans = NULL;
    cJSON *data_volume = NULL;
    OpenAPI_data_volume_t *data_volume_local_nonprim = NULL;
    cJSON *max_number_ues = NULL;
    criterion = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_reqJSON, "criterion");
    if (criterion) {
    if (!cJSON_IsString(criterion)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON() failed [criterion]");
        goto end;
    }
    criterionVariable = OpenAPI_e2e_data_vol_trans_time_criterion_FromString(criterion->valuestring);
    }

    order = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_reqJSON, "order");
    if (order) {
    if (!cJSON_IsString(order)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON() failed [order]");
        goto end;
    }
    orderVariable = OpenAPI_matching_direction_FromString(order->valuestring);
    }

    high_trans_tm_thr = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_reqJSON, "highTransTmThr");
    if (high_trans_tm_thr) {
    if (!cJSON_IsNumber(high_trans_tm_thr)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON() failed [high_trans_tm_thr]");
        goto end;
    }
    }

    low_trans_tm_thr = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_reqJSON, "lowTransTmThr");
    if (low_trans_tm_thr) {
    if (!cJSON_IsNumber(low_trans_tm_thr)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON() failed [low_trans_tm_thr]");
        goto end;
    }
    }

    repeat_data_trans = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_reqJSON, "repeatDataTrans");
    if (repeat_data_trans) {
    if (!cJSON_IsNumber(repeat_data_trans)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON() failed [repeat_data_trans]");
        goto end;
    }
    }

    ts_interval_data_trans = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_reqJSON, "tsIntervalDataTrans");
    if (ts_interval_data_trans) {
    if (!cJSON_IsNumber(ts_interval_data_trans)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON() failed [ts_interval_data_trans]");
        goto end;
    }
    }

    data_volume = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_reqJSON, "dataVolume");
    if (data_volume) {
    if (!cJSON_IsNull(data_volume)) {
    data_volume_local_nonprim = OpenAPI_data_volume_parseFromJSON(data_volume);
    if (!data_volume_local_nonprim) {
        ogs_error("OpenAPI_data_volume_parseFromJSON failed [data_volume]");
        goto end;
    }
    }
    }

    max_number_ues = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_reqJSON, "maxNumberUes");
    if (max_number_ues) {
    if (!cJSON_IsNumber(max_number_ues)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON() failed [max_number_ues]");
        goto end;
    }
    }

    e2e_data_vol_trans_time_req_local_var = OpenAPI_e2e_data_vol_trans_time_req_create (
        criterion ? criterionVariable : 0,
        order ? orderVariable : 0,
        high_trans_tm_thr ? true : false,
        high_trans_tm_thr ? high_trans_tm_thr->valuedouble : 0,
        low_trans_tm_thr ? true : false,
        low_trans_tm_thr ? low_trans_tm_thr->valuedouble : 0,
        repeat_data_trans ? true : false,
        repeat_data_trans ? repeat_data_trans->valuedouble : 0,
        ts_interval_data_trans ? true : false,
        ts_interval_data_trans ? ts_interval_data_trans->valuedouble : 0,
        data_volume && cJSON_IsNull(data_volume) ? true : false,
        data_volume ? data_volume_local_nonprim : NULL,
        max_number_ues ? true : false,
        max_number_ues ? max_number_ues->valuedouble : 0
    );

    return e2e_data_vol_trans_time_req_local_var;
end:
    if (data_volume_local_nonprim) {
        OpenAPI_data_volume_free(data_volume_local_nonprim);
        data_volume_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_e2e_data_vol_trans_time_req_t *OpenAPI_e2e_data_vol_trans_time_req_copy(OpenAPI_e2e_data_vol_trans_time_req_t *dst, OpenAPI_e2e_data_vol_trans_time_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_e2e_data_vol_trans_time_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_req_convertToJSON() failed");
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

    OpenAPI_e2e_data_vol_trans_time_req_free(dst);
    dst = OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

