
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redundant_transmission_exp_per_ts.h"

OpenAPI_redundant_transmission_exp_per_ts_t *OpenAPI_redundant_transmission_exp_per_ts_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_observed_redundant_trans_exp_t *obsv_red_trans_exp,
    bool is_red_trans_status,
    int red_trans_status,
    bool is_ue_ratio,
    int ue_ratio,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_redundant_transmission_exp_per_ts_t *redundant_transmission_exp_per_ts_local_var = ogs_malloc(sizeof(OpenAPI_redundant_transmission_exp_per_ts_t));
    ogs_assert(redundant_transmission_exp_per_ts_local_var);

    redundant_transmission_exp_per_ts_local_var->ts_start = ts_start;
    redundant_transmission_exp_per_ts_local_var->ts_duration = ts_duration;
    redundant_transmission_exp_per_ts_local_var->obsv_red_trans_exp = obsv_red_trans_exp;
    redundant_transmission_exp_per_ts_local_var->is_red_trans_status = is_red_trans_status;
    redundant_transmission_exp_per_ts_local_var->red_trans_status = red_trans_status;
    redundant_transmission_exp_per_ts_local_var->is_ue_ratio = is_ue_ratio;
    redundant_transmission_exp_per_ts_local_var->ue_ratio = ue_ratio;
    redundant_transmission_exp_per_ts_local_var->is_confidence = is_confidence;
    redundant_transmission_exp_per_ts_local_var->confidence = confidence;

    return redundant_transmission_exp_per_ts_local_var;
}

void OpenAPI_redundant_transmission_exp_per_ts_free(OpenAPI_redundant_transmission_exp_per_ts_t *redundant_transmission_exp_per_ts)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == redundant_transmission_exp_per_ts) {
        return;
    }
    if (redundant_transmission_exp_per_ts->ts_start) {
        ogs_free(redundant_transmission_exp_per_ts->ts_start);
        redundant_transmission_exp_per_ts->ts_start = NULL;
    }
    if (redundant_transmission_exp_per_ts->obsv_red_trans_exp) {
        OpenAPI_observed_redundant_trans_exp_free(redundant_transmission_exp_per_ts->obsv_red_trans_exp);
        redundant_transmission_exp_per_ts->obsv_red_trans_exp = NULL;
    }
    ogs_free(redundant_transmission_exp_per_ts);
}

cJSON *OpenAPI_redundant_transmission_exp_per_ts_convertToJSON(OpenAPI_redundant_transmission_exp_per_ts_t *redundant_transmission_exp_per_ts)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (redundant_transmission_exp_per_ts == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [RedundantTransmissionExpPerTS]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!redundant_transmission_exp_per_ts->ts_start) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [ts_start]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tsStart", redundant_transmission_exp_per_ts->ts_start) == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [ts_start]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "tsDuration", redundant_transmission_exp_per_ts->ts_duration) == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [ts_duration]");
        goto end;
    }

    if (!redundant_transmission_exp_per_ts->obsv_red_trans_exp) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [obsv_red_trans_exp]");
        return NULL;
    }
    cJSON *obsv_red_trans_exp_local_JSON = OpenAPI_observed_redundant_trans_exp_convertToJSON(redundant_transmission_exp_per_ts->obsv_red_trans_exp);
    if (obsv_red_trans_exp_local_JSON == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [obsv_red_trans_exp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "obsvRedTransExp", obsv_red_trans_exp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [obsv_red_trans_exp]");
        goto end;
    }

    if (redundant_transmission_exp_per_ts->is_red_trans_status) {
    if (cJSON_AddBoolToObject(item, "redTransStatus", redundant_transmission_exp_per_ts->red_trans_status) == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [red_trans_status]");
        goto end;
    }
    }

    if (redundant_transmission_exp_per_ts->is_ue_ratio) {
    if (cJSON_AddNumberToObject(item, "ueRatio", redundant_transmission_exp_per_ts->ue_ratio) == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [ue_ratio]");
        goto end;
    }
    }

    if (redundant_transmission_exp_per_ts->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", redundant_transmission_exp_per_ts->confidence) == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_redundant_transmission_exp_per_ts_t *OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON(cJSON *redundant_transmission_exp_per_tsJSON)
{
    OpenAPI_redundant_transmission_exp_per_ts_t *redundant_transmission_exp_per_ts_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts_start = NULL;
    cJSON *ts_duration = NULL;
    cJSON *obsv_red_trans_exp = NULL;
    OpenAPI_observed_redundant_trans_exp_t *obsv_red_trans_exp_local_nonprim = NULL;
    cJSON *red_trans_status = NULL;
    cJSON *ue_ratio = NULL;
    cJSON *confidence = NULL;
    ts_start = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_per_tsJSON, "tsStart");
    if (!ts_start) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON() failed [ts_start]");
        goto end;
    }
    if (!cJSON_IsString(ts_start) && !cJSON_IsNull(ts_start)) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON() failed [ts_start]");
        goto end;
    }

    ts_duration = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_per_tsJSON, "tsDuration");
    if (!ts_duration) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON() failed [ts_duration]");
        goto end;
    }
    if (!cJSON_IsNumber(ts_duration)) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON() failed [ts_duration]");
        goto end;
    }

    obsv_red_trans_exp = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_per_tsJSON, "obsvRedTransExp");
    if (!obsv_red_trans_exp) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON() failed [obsv_red_trans_exp]");
        goto end;
    }
    obsv_red_trans_exp_local_nonprim = OpenAPI_observed_redundant_trans_exp_parseFromJSON(obsv_red_trans_exp);
    if (!obsv_red_trans_exp_local_nonprim) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON failed [obsv_red_trans_exp]");
        goto end;
    }

    red_trans_status = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_per_tsJSON, "redTransStatus");
    if (red_trans_status) {
    if (!cJSON_IsBool(red_trans_status)) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON() failed [red_trans_status]");
        goto end;
    }
    }

    ue_ratio = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_per_tsJSON, "ueRatio");
    if (ue_ratio) {
    if (!cJSON_IsNumber(ue_ratio)) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON() failed [ue_ratio]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_per_tsJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    redundant_transmission_exp_per_ts_local_var = OpenAPI_redundant_transmission_exp_per_ts_create (
        ogs_strdup(ts_start->valuestring),
        
        ts_duration->valuedouble,
        obsv_red_trans_exp_local_nonprim,
        red_trans_status ? true : false,
        red_trans_status ? red_trans_status->valueint : 0,
        ue_ratio ? true : false,
        ue_ratio ? ue_ratio->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return redundant_transmission_exp_per_ts_local_var;
end:
    if (obsv_red_trans_exp_local_nonprim) {
        OpenAPI_observed_redundant_trans_exp_free(obsv_red_trans_exp_local_nonprim);
        obsv_red_trans_exp_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_redundant_transmission_exp_per_ts_t *OpenAPI_redundant_transmission_exp_per_ts_copy(OpenAPI_redundant_transmission_exp_per_ts_t *dst, OpenAPI_redundant_transmission_exp_per_ts_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_redundant_transmission_exp_per_ts_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_redundant_transmission_exp_per_ts_convertToJSON() failed");
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

    OpenAPI_redundant_transmission_exp_per_ts_free(dst);
    dst = OpenAPI_redundant_transmission_exp_per_ts_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

