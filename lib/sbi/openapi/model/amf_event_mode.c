
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_mode.h"

OpenAPI_amf_event_mode_t *OpenAPI_amf_event_mode_create(
    OpenAPI_amf_event_trigger_t *trigger,
    bool is_max_reports,
    int max_reports,
    char *expiry,
    bool is_rep_period,
    int rep_period,
    bool is_samp_ratio,
    int samp_ratio,
    OpenAPI_list_t *partitioning_criteria,
    OpenAPI_notification_flag_e notif_flag
)
{
    OpenAPI_amf_event_mode_t *amf_event_mode_local_var = ogs_malloc(sizeof(OpenAPI_amf_event_mode_t));
    ogs_assert(amf_event_mode_local_var);

    amf_event_mode_local_var->trigger = trigger;
    amf_event_mode_local_var->is_max_reports = is_max_reports;
    amf_event_mode_local_var->max_reports = max_reports;
    amf_event_mode_local_var->expiry = expiry;
    amf_event_mode_local_var->is_rep_period = is_rep_period;
    amf_event_mode_local_var->rep_period = rep_period;
    amf_event_mode_local_var->is_samp_ratio = is_samp_ratio;
    amf_event_mode_local_var->samp_ratio = samp_ratio;
    amf_event_mode_local_var->partitioning_criteria = partitioning_criteria;
    amf_event_mode_local_var->notif_flag = notif_flag;

    return amf_event_mode_local_var;
}

void OpenAPI_amf_event_mode_free(OpenAPI_amf_event_mode_t *amf_event_mode)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_event_mode) {
        return;
    }
    if (amf_event_mode->trigger) {
        OpenAPI_amf_event_trigger_free(amf_event_mode->trigger);
        amf_event_mode->trigger = NULL;
    }
    if (amf_event_mode->expiry) {
        ogs_free(amf_event_mode->expiry);
        amf_event_mode->expiry = NULL;
    }
    if (amf_event_mode->partitioning_criteria) {
        OpenAPI_list_free(amf_event_mode->partitioning_criteria);
        amf_event_mode->partitioning_criteria = NULL;
    }
    ogs_free(amf_event_mode);
}

cJSON *OpenAPI_amf_event_mode_convertToJSON(OpenAPI_amf_event_mode_t *amf_event_mode)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_event_mode == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [AmfEventMode]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf_event_mode->trigger) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [trigger]");
        return NULL;
    }
    cJSON *trigger_local_JSON = OpenAPI_amf_event_trigger_convertToJSON(amf_event_mode->trigger);
    if (trigger_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [trigger]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trigger", trigger_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [trigger]");
        goto end;
    }

    if (amf_event_mode->is_max_reports) {
    if (cJSON_AddNumberToObject(item, "maxReports", amf_event_mode->max_reports) == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [max_reports]");
        goto end;
    }
    }

    if (amf_event_mode->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", amf_event_mode->expiry) == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (amf_event_mode->is_rep_period) {
    if (cJSON_AddNumberToObject(item, "repPeriod", amf_event_mode->rep_period) == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [rep_period]");
        goto end;
    }
    }

    if (amf_event_mode->is_samp_ratio) {
    if (cJSON_AddNumberToObject(item, "sampRatio", amf_event_mode->samp_ratio) == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [samp_ratio]");
        goto end;
    }
    }

    if (amf_event_mode->partitioning_criteria != OpenAPI_partitioning_criteria_NULL) {
    cJSON *partitioning_criteriaList = cJSON_AddArrayToObject(item, "partitioningCriteria");
    if (partitioning_criteriaList == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [partitioning_criteria]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event_mode->partitioning_criteria, node) {
        if (cJSON_AddStringToObject(partitioning_criteriaList, "", OpenAPI_partitioning_criteria_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [partitioning_criteria]");
            goto end;
        }
    }
    }

    if (amf_event_mode->notif_flag != OpenAPI_notification_flag_NULL) {
    if (cJSON_AddStringToObject(item, "notifFlag", OpenAPI_notification_flag_ToString(amf_event_mode->notif_flag)) == NULL) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed [notif_flag]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_event_mode_t *OpenAPI_amf_event_mode_parseFromJSON(cJSON *amf_event_modeJSON)
{
    OpenAPI_amf_event_mode_t *amf_event_mode_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *trigger = NULL;
    OpenAPI_amf_event_trigger_t *trigger_local_nonprim = NULL;
    cJSON *max_reports = NULL;
    cJSON *expiry = NULL;
    cJSON *rep_period = NULL;
    cJSON *samp_ratio = NULL;
    cJSON *partitioning_criteria = NULL;
    OpenAPI_list_t *partitioning_criteriaList = NULL;
    cJSON *notif_flag = NULL;
    OpenAPI_notification_flag_e notif_flagVariable = 0;
    trigger = cJSON_GetObjectItemCaseSensitive(amf_event_modeJSON, "trigger");
    if (!trigger) {
        ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed [trigger]");
        goto end;
    }
    trigger_local_nonprim = OpenAPI_amf_event_trigger_parseFromJSON(trigger);
    if (!trigger_local_nonprim) {
        ogs_error("OpenAPI_amf_event_trigger_parseFromJSON failed [trigger]");
        goto end;
    }

    max_reports = cJSON_GetObjectItemCaseSensitive(amf_event_modeJSON, "maxReports");
    if (max_reports) {
    if (!cJSON_IsNumber(max_reports)) {
        ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed [max_reports]");
        goto end;
    }
    }

    expiry = cJSON_GetObjectItemCaseSensitive(amf_event_modeJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    rep_period = cJSON_GetObjectItemCaseSensitive(amf_event_modeJSON, "repPeriod");
    if (rep_period) {
    if (!cJSON_IsNumber(rep_period)) {
        ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed [rep_period]");
        goto end;
    }
    }

    samp_ratio = cJSON_GetObjectItemCaseSensitive(amf_event_modeJSON, "sampRatio");
    if (samp_ratio) {
    if (!cJSON_IsNumber(samp_ratio)) {
        ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed [samp_ratio]");
        goto end;
    }
    }

    partitioning_criteria = cJSON_GetObjectItemCaseSensitive(amf_event_modeJSON, "partitioningCriteria");
    if (partitioning_criteria) {
        cJSON *partitioning_criteria_local = NULL;
        if (!cJSON_IsArray(partitioning_criteria)) {
            ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed [partitioning_criteria]");
            goto end;
        }

        partitioning_criteriaList = OpenAPI_list_create();

        cJSON_ArrayForEach(partitioning_criteria_local, partitioning_criteria) {
            OpenAPI_partitioning_criteria_e localEnum = OpenAPI_partitioning_criteria_NULL;
            if (!cJSON_IsString(partitioning_criteria_local)) {
                ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed [partitioning_criteria]");
                goto end;
            }
            localEnum = OpenAPI_partitioning_criteria_FromString(partitioning_criteria_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"partitioning_criteria\" is not supported. Ignoring it ...",
                         partitioning_criteria_local->valuestring);
            } else {
                OpenAPI_list_add(partitioning_criteriaList, (void *)localEnum);
            }
        }
        if (partitioning_criteriaList->count == 0) {
            ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed: Expected partitioning_criteriaList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    notif_flag = cJSON_GetObjectItemCaseSensitive(amf_event_modeJSON, "notifFlag");
    if (notif_flag) {
    if (!cJSON_IsString(notif_flag)) {
        ogs_error("OpenAPI_amf_event_mode_parseFromJSON() failed [notif_flag]");
        goto end;
    }
    notif_flagVariable = OpenAPI_notification_flag_FromString(notif_flag->valuestring);
    }

    amf_event_mode_local_var = OpenAPI_amf_event_mode_create (
        trigger_local_nonprim,
        max_reports ? true : false,
        max_reports ? max_reports->valuedouble : 0,
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        rep_period ? true : false,
        rep_period ? rep_period->valuedouble : 0,
        samp_ratio ? true : false,
        samp_ratio ? samp_ratio->valuedouble : 0,
        partitioning_criteria ? partitioning_criteriaList : NULL,
        notif_flag ? notif_flagVariable : 0
    );

    return amf_event_mode_local_var;
end:
    if (trigger_local_nonprim) {
        OpenAPI_amf_event_trigger_free(trigger_local_nonprim);
        trigger_local_nonprim = NULL;
    }
    if (partitioning_criteriaList) {
        OpenAPI_list_free(partitioning_criteriaList);
        partitioning_criteriaList = NULL;
    }
    return NULL;
}

OpenAPI_amf_event_mode_t *OpenAPI_amf_event_mode_copy(OpenAPI_amf_event_mode_t *dst, OpenAPI_amf_event_mode_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_mode_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_mode_convertToJSON() failed");
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

    OpenAPI_amf_event_mode_free(dst);
    dst = OpenAPI_amf_event_mode_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

