
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accuracy_req.h"

OpenAPI_accuracy_req_t *OpenAPI_accuracy_req_create(
    OpenAPI_time_window_t *accu_time_win,
    bool is_accu_period,
    int accu_period,
    bool is_accu_dev_thr,
    int accu_dev_thr,
    bool is_min_num,
    int min_num,
    bool is_updated_ana_flg,
    int updated_ana_flg,
    bool is_correction_interval,
    int correction_interval
)
{
    OpenAPI_accuracy_req_t *accuracy_req_local_var = ogs_malloc(sizeof(OpenAPI_accuracy_req_t));
    ogs_assert(accuracy_req_local_var);

    accuracy_req_local_var->accu_time_win = accu_time_win;
    accuracy_req_local_var->is_accu_period = is_accu_period;
    accuracy_req_local_var->accu_period = accu_period;
    accuracy_req_local_var->is_accu_dev_thr = is_accu_dev_thr;
    accuracy_req_local_var->accu_dev_thr = accu_dev_thr;
    accuracy_req_local_var->is_min_num = is_min_num;
    accuracy_req_local_var->min_num = min_num;
    accuracy_req_local_var->is_updated_ana_flg = is_updated_ana_flg;
    accuracy_req_local_var->updated_ana_flg = updated_ana_flg;
    accuracy_req_local_var->is_correction_interval = is_correction_interval;
    accuracy_req_local_var->correction_interval = correction_interval;

    return accuracy_req_local_var;
}

void OpenAPI_accuracy_req_free(OpenAPI_accuracy_req_t *accuracy_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == accuracy_req) {
        return;
    }
    if (accuracy_req->accu_time_win) {
        OpenAPI_time_window_free(accuracy_req->accu_time_win);
        accuracy_req->accu_time_win = NULL;
    }
    ogs_free(accuracy_req);
}

cJSON *OpenAPI_accuracy_req_convertToJSON(OpenAPI_accuracy_req_t *accuracy_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (accuracy_req == NULL) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed [AccuracyReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (accuracy_req->accu_time_win) {
    cJSON *accu_time_win_local_JSON = OpenAPI_time_window_convertToJSON(accuracy_req->accu_time_win);
    if (accu_time_win_local_JSON == NULL) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed [accu_time_win]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accuTimeWin", accu_time_win_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed [accu_time_win]");
        goto end;
    }
    }

    if (accuracy_req->is_accu_period) {
    if (cJSON_AddNumberToObject(item, "accuPeriod", accuracy_req->accu_period) == NULL) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed [accu_period]");
        goto end;
    }
    }

    if (accuracy_req->is_accu_dev_thr) {
    if (cJSON_AddNumberToObject(item, "accuDevThr", accuracy_req->accu_dev_thr) == NULL) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed [accu_dev_thr]");
        goto end;
    }
    }

    if (accuracy_req->is_min_num) {
    if (cJSON_AddNumberToObject(item, "minNum", accuracy_req->min_num) == NULL) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed [min_num]");
        goto end;
    }
    }

    if (accuracy_req->is_updated_ana_flg) {
    if (cJSON_AddBoolToObject(item, "updatedAnaFlg", accuracy_req->updated_ana_flg) == NULL) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed [updated_ana_flg]");
        goto end;
    }
    }

    if (accuracy_req->is_correction_interval) {
    if (cJSON_AddNumberToObject(item, "correctionInterval", accuracy_req->correction_interval) == NULL) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed [correction_interval]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_accuracy_req_t *OpenAPI_accuracy_req_parseFromJSON(cJSON *accuracy_reqJSON)
{
    OpenAPI_accuracy_req_t *accuracy_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *accu_time_win = NULL;
    OpenAPI_time_window_t *accu_time_win_local_nonprim = NULL;
    cJSON *accu_period = NULL;
    cJSON *accu_dev_thr = NULL;
    cJSON *min_num = NULL;
    cJSON *updated_ana_flg = NULL;
    cJSON *correction_interval = NULL;
    accu_time_win = cJSON_GetObjectItemCaseSensitive(accuracy_reqJSON, "accuTimeWin");
    if (accu_time_win) {
    accu_time_win_local_nonprim = OpenAPI_time_window_parseFromJSON(accu_time_win);
    if (!accu_time_win_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [accu_time_win]");
        goto end;
    }
    }

    accu_period = cJSON_GetObjectItemCaseSensitive(accuracy_reqJSON, "accuPeriod");
    if (accu_period) {
    if (!cJSON_IsNumber(accu_period)) {
        ogs_error("OpenAPI_accuracy_req_parseFromJSON() failed [accu_period]");
        goto end;
    }
    }

    accu_dev_thr = cJSON_GetObjectItemCaseSensitive(accuracy_reqJSON, "accuDevThr");
    if (accu_dev_thr) {
    if (!cJSON_IsNumber(accu_dev_thr)) {
        ogs_error("OpenAPI_accuracy_req_parseFromJSON() failed [accu_dev_thr]");
        goto end;
    }
    }

    min_num = cJSON_GetObjectItemCaseSensitive(accuracy_reqJSON, "minNum");
    if (min_num) {
    if (!cJSON_IsNumber(min_num)) {
        ogs_error("OpenAPI_accuracy_req_parseFromJSON() failed [min_num]");
        goto end;
    }
    }

    updated_ana_flg = cJSON_GetObjectItemCaseSensitive(accuracy_reqJSON, "updatedAnaFlg");
    if (updated_ana_flg) {
    if (!cJSON_IsBool(updated_ana_flg)) {
        ogs_error("OpenAPI_accuracy_req_parseFromJSON() failed [updated_ana_flg]");
        goto end;
    }
    }

    correction_interval = cJSON_GetObjectItemCaseSensitive(accuracy_reqJSON, "correctionInterval");
    if (correction_interval) {
    if (!cJSON_IsNumber(correction_interval)) {
        ogs_error("OpenAPI_accuracy_req_parseFromJSON() failed [correction_interval]");
        goto end;
    }
    }

    accuracy_req_local_var = OpenAPI_accuracy_req_create (
        accu_time_win ? accu_time_win_local_nonprim : NULL,
        accu_period ? true : false,
        accu_period ? accu_period->valuedouble : 0,
        accu_dev_thr ? true : false,
        accu_dev_thr ? accu_dev_thr->valuedouble : 0,
        min_num ? true : false,
        min_num ? min_num->valuedouble : 0,
        updated_ana_flg ? true : false,
        updated_ana_flg ? updated_ana_flg->valueint : 0,
        correction_interval ? true : false,
        correction_interval ? correction_interval->valuedouble : 0
    );

    return accuracy_req_local_var;
end:
    if (accu_time_win_local_nonprim) {
        OpenAPI_time_window_free(accu_time_win_local_nonprim);
        accu_time_win_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_accuracy_req_t *OpenAPI_accuracy_req_copy(OpenAPI_accuracy_req_t *dst, OpenAPI_accuracy_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_accuracy_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_accuracy_req_convertToJSON() failed");
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

    OpenAPI_accuracy_req_free(dst);
    dst = OpenAPI_accuracy_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

