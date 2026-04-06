
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_policy_assist_sets_per_ts.h"

OpenAPI_qos_policy_assist_sets_per_ts_t *OpenAPI_qos_policy_assist_sets_per_ts_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_list_t *qos_pol_assist_sets,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_qos_policy_assist_sets_per_ts_t *qos_policy_assist_sets_per_ts_local_var = ogs_malloc(sizeof(OpenAPI_qos_policy_assist_sets_per_ts_t));
    ogs_assert(qos_policy_assist_sets_per_ts_local_var);

    qos_policy_assist_sets_per_ts_local_var->ts_start = ts_start;
    qos_policy_assist_sets_per_ts_local_var->ts_duration = ts_duration;
    qos_policy_assist_sets_per_ts_local_var->qos_pol_assist_sets = qos_pol_assist_sets;
    qos_policy_assist_sets_per_ts_local_var->is_confidence = is_confidence;
    qos_policy_assist_sets_per_ts_local_var->confidence = confidence;

    return qos_policy_assist_sets_per_ts_local_var;
}

void OpenAPI_qos_policy_assist_sets_per_ts_free(OpenAPI_qos_policy_assist_sets_per_ts_t *qos_policy_assist_sets_per_ts)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_policy_assist_sets_per_ts) {
        return;
    }
    if (qos_policy_assist_sets_per_ts->ts_start) {
        ogs_free(qos_policy_assist_sets_per_ts->ts_start);
        qos_policy_assist_sets_per_ts->ts_start = NULL;
    }
    if (qos_policy_assist_sets_per_ts->qos_pol_assist_sets) {
        OpenAPI_list_for_each(qos_policy_assist_sets_per_ts->qos_pol_assist_sets, node) {
            OpenAPI_qos_policy_assist_set_free(node->data);
        }
        OpenAPI_list_free(qos_policy_assist_sets_per_ts->qos_pol_assist_sets);
        qos_policy_assist_sets_per_ts->qos_pol_assist_sets = NULL;
    }
    ogs_free(qos_policy_assist_sets_per_ts);
}

cJSON *OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON(OpenAPI_qos_policy_assist_sets_per_ts_t *qos_policy_assist_sets_per_ts)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_policy_assist_sets_per_ts == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed [QosPolicyAssistSetsPerTS]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!qos_policy_assist_sets_per_ts->ts_start) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed [ts_start]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tsStart", qos_policy_assist_sets_per_ts->ts_start) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed [ts_start]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "tsDuration", qos_policy_assist_sets_per_ts->ts_duration) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed [ts_duration]");
        goto end;
    }

    if (!qos_policy_assist_sets_per_ts->qos_pol_assist_sets) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed [qos_pol_assist_sets]");
        return NULL;
    }
    cJSON *qos_pol_assist_setsList = cJSON_AddArrayToObject(item, "qosPolAssistSets");
    if (qos_pol_assist_setsList == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed [qos_pol_assist_sets]");
        goto end;
    }
    OpenAPI_list_for_each(qos_policy_assist_sets_per_ts->qos_pol_assist_sets, node) {
        cJSON *itemLocal = OpenAPI_qos_policy_assist_set_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed [qos_pol_assist_sets]");
            goto end;
        }
        cJSON_AddItemToArray(qos_pol_assist_setsList, itemLocal);
    }

    if (qos_policy_assist_sets_per_ts->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", qos_policy_assist_sets_per_ts->confidence) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_policy_assist_sets_per_ts_t *OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON(cJSON *qos_policy_assist_sets_per_tsJSON)
{
    OpenAPI_qos_policy_assist_sets_per_ts_t *qos_policy_assist_sets_per_ts_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts_start = NULL;
    cJSON *ts_duration = NULL;
    cJSON *qos_pol_assist_sets = NULL;
    OpenAPI_list_t *qos_pol_assist_setsList = NULL;
    cJSON *confidence = NULL;
    ts_start = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_sets_per_tsJSON, "tsStart");
    if (!ts_start) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON() failed [ts_start]");
        goto end;
    }
    if (!cJSON_IsString(ts_start) && !cJSON_IsNull(ts_start)) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON() failed [ts_start]");
        goto end;
    }

    ts_duration = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_sets_per_tsJSON, "tsDuration");
    if (!ts_duration) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON() failed [ts_duration]");
        goto end;
    }
    if (!cJSON_IsNumber(ts_duration)) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON() failed [ts_duration]");
        goto end;
    }

    qos_pol_assist_sets = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_sets_per_tsJSON, "qosPolAssistSets");
    if (!qos_pol_assist_sets) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON() failed [qos_pol_assist_sets]");
        goto end;
    }
        cJSON *qos_pol_assist_sets_local = NULL;
        if (!cJSON_IsArray(qos_pol_assist_sets)) {
            ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON() failed [qos_pol_assist_sets]");
            goto end;
        }

        qos_pol_assist_setsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_pol_assist_sets_local, qos_pol_assist_sets) {
            if (!cJSON_IsObject(qos_pol_assist_sets_local)) {
                ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON() failed [qos_pol_assist_sets]");
                goto end;
            }
            OpenAPI_qos_policy_assist_set_t *qos_pol_assist_setsItem = OpenAPI_qos_policy_assist_set_parseFromJSON(qos_pol_assist_sets_local);
            if (!qos_pol_assist_setsItem) {
                ogs_error("No qos_pol_assist_setsItem");
                goto end;
            }
            OpenAPI_list_add(qos_pol_assist_setsList, qos_pol_assist_setsItem);
        }

    confidence = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_sets_per_tsJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    qos_policy_assist_sets_per_ts_local_var = OpenAPI_qos_policy_assist_sets_per_ts_create (
        ogs_strdup(ts_start->valuestring),
        
        ts_duration->valuedouble,
        qos_pol_assist_setsList,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return qos_policy_assist_sets_per_ts_local_var;
end:
    if (qos_pol_assist_setsList) {
        OpenAPI_list_for_each(qos_pol_assist_setsList, node) {
            OpenAPI_qos_policy_assist_set_free(node->data);
        }
        OpenAPI_list_free(qos_pol_assist_setsList);
        qos_pol_assist_setsList = NULL;
    }
    return NULL;
}

OpenAPI_qos_policy_assist_sets_per_ts_t *OpenAPI_qos_policy_assist_sets_per_ts_copy(OpenAPI_qos_policy_assist_sets_per_ts_t *dst, OpenAPI_qos_policy_assist_sets_per_ts_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON() failed");
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

    OpenAPI_qos_policy_assist_sets_per_ts_free(dst);
    dst = OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

