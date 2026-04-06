
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "e2e_data_vol_trans_time_per_ts.h"

OpenAPI_e2e_data_vol_trans_time_per_ts_t *OpenAPI_e2e_data_vol_trans_time_per_ts_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_list_t *e2e_data_vol_trans_time_per_ue
)
{
    OpenAPI_e2e_data_vol_trans_time_per_ts_t *e2e_data_vol_trans_time_per_ts_local_var = ogs_malloc(sizeof(OpenAPI_e2e_data_vol_trans_time_per_ts_t));
    ogs_assert(e2e_data_vol_trans_time_per_ts_local_var);

    e2e_data_vol_trans_time_per_ts_local_var->ts_start = ts_start;
    e2e_data_vol_trans_time_per_ts_local_var->ts_duration = ts_duration;
    e2e_data_vol_trans_time_per_ts_local_var->e2e_data_vol_trans_time_per_ue = e2e_data_vol_trans_time_per_ue;

    return e2e_data_vol_trans_time_per_ts_local_var;
}

void OpenAPI_e2e_data_vol_trans_time_per_ts_free(OpenAPI_e2e_data_vol_trans_time_per_ts_t *e2e_data_vol_trans_time_per_ts)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == e2e_data_vol_trans_time_per_ts) {
        return;
    }
    if (e2e_data_vol_trans_time_per_ts->ts_start) {
        ogs_free(e2e_data_vol_trans_time_per_ts->ts_start);
        e2e_data_vol_trans_time_per_ts->ts_start = NULL;
    }
    if (e2e_data_vol_trans_time_per_ts->e2e_data_vol_trans_time_per_ue) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_per_ts->e2e_data_vol_trans_time_per_ue, node) {
            OpenAPI_e2e_data_vol_trans_time_per_ue_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_per_ts->e2e_data_vol_trans_time_per_ue);
        e2e_data_vol_trans_time_per_ts->e2e_data_vol_trans_time_per_ue = NULL;
    }
    ogs_free(e2e_data_vol_trans_time_per_ts);
}

cJSON *OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON(OpenAPI_e2e_data_vol_trans_time_per_ts_t *e2e_data_vol_trans_time_per_ts)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (e2e_data_vol_trans_time_per_ts == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON() failed [E2eDataVolTransTimePerTS]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!e2e_data_vol_trans_time_per_ts->ts_start) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON() failed [ts_start]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tsStart", e2e_data_vol_trans_time_per_ts->ts_start) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON() failed [ts_start]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "tsDuration", e2e_data_vol_trans_time_per_ts->ts_duration) == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON() failed [ts_duration]");
        goto end;
    }

    if (!e2e_data_vol_trans_time_per_ts->e2e_data_vol_trans_time_per_ue) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON() failed [e2e_data_vol_trans_time_per_ue]");
        return NULL;
    }
    cJSON *e2e_data_vol_trans_time_per_ueList = cJSON_AddArrayToObject(item, "e2eDataVolTransTimePerUe");
    if (e2e_data_vol_trans_time_per_ueList == NULL) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON() failed [e2e_data_vol_trans_time_per_ue]");
        goto end;
    }
    OpenAPI_list_for_each(e2e_data_vol_trans_time_per_ts->e2e_data_vol_trans_time_per_ue, node) {
        cJSON *itemLocal = OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON() failed [e2e_data_vol_trans_time_per_ue]");
            goto end;
        }
        cJSON_AddItemToArray(e2e_data_vol_trans_time_per_ueList, itemLocal);
    }

end:
    return item;
}

OpenAPI_e2e_data_vol_trans_time_per_ts_t *OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON(cJSON *e2e_data_vol_trans_time_per_tsJSON)
{
    OpenAPI_e2e_data_vol_trans_time_per_ts_t *e2e_data_vol_trans_time_per_ts_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts_start = NULL;
    cJSON *ts_duration = NULL;
    cJSON *e2e_data_vol_trans_time_per_ue = NULL;
    OpenAPI_list_t *e2e_data_vol_trans_time_per_ueList = NULL;
    ts_start = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_tsJSON, "tsStart");
    if (!ts_start) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON() failed [ts_start]");
        goto end;
    }
    if (!cJSON_IsString(ts_start) && !cJSON_IsNull(ts_start)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON() failed [ts_start]");
        goto end;
    }

    ts_duration = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_tsJSON, "tsDuration");
    if (!ts_duration) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON() failed [ts_duration]");
        goto end;
    }
    if (!cJSON_IsNumber(ts_duration)) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON() failed [ts_duration]");
        goto end;
    }

    e2e_data_vol_trans_time_per_ue = cJSON_GetObjectItemCaseSensitive(e2e_data_vol_trans_time_per_tsJSON, "e2eDataVolTransTimePerUe");
    if (!e2e_data_vol_trans_time_per_ue) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON() failed [e2e_data_vol_trans_time_per_ue]");
        goto end;
    }
        cJSON *e2e_data_vol_trans_time_per_ue_local = NULL;
        if (!cJSON_IsArray(e2e_data_vol_trans_time_per_ue)) {
            ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON() failed [e2e_data_vol_trans_time_per_ue]");
            goto end;
        }

        e2e_data_vol_trans_time_per_ueList = OpenAPI_list_create();

        cJSON_ArrayForEach(e2e_data_vol_trans_time_per_ue_local, e2e_data_vol_trans_time_per_ue) {
            if (!cJSON_IsObject(e2e_data_vol_trans_time_per_ue_local)) {
                ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON() failed [e2e_data_vol_trans_time_per_ue]");
                goto end;
            }
            OpenAPI_e2e_data_vol_trans_time_per_ue_t *e2e_data_vol_trans_time_per_ueItem = OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON(e2e_data_vol_trans_time_per_ue_local);
            if (!e2e_data_vol_trans_time_per_ueItem) {
                ogs_error("No e2e_data_vol_trans_time_per_ueItem");
                goto end;
            }
            OpenAPI_list_add(e2e_data_vol_trans_time_per_ueList, e2e_data_vol_trans_time_per_ueItem);
        }

    e2e_data_vol_trans_time_per_ts_local_var = OpenAPI_e2e_data_vol_trans_time_per_ts_create (
        ogs_strdup(ts_start->valuestring),
        
        ts_duration->valuedouble,
        e2e_data_vol_trans_time_per_ueList
    );

    return e2e_data_vol_trans_time_per_ts_local_var;
end:
    if (e2e_data_vol_trans_time_per_ueList) {
        OpenAPI_list_for_each(e2e_data_vol_trans_time_per_ueList, node) {
            OpenAPI_e2e_data_vol_trans_time_per_ue_free(node->data);
        }
        OpenAPI_list_free(e2e_data_vol_trans_time_per_ueList);
        e2e_data_vol_trans_time_per_ueList = NULL;
    }
    return NULL;
}

OpenAPI_e2e_data_vol_trans_time_per_ts_t *OpenAPI_e2e_data_vol_trans_time_per_ts_copy(OpenAPI_e2e_data_vol_trans_time_per_ts_t *dst, OpenAPI_e2e_data_vol_trans_time_per_ts_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON() failed");
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

    OpenAPI_e2e_data_vol_trans_time_per_ts_free(dst);
    dst = OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

