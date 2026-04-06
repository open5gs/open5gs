
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_feedback_info.h"

OpenAPI_analytics_feedback_info_t *OpenAPI_analytics_feedback_info_create(
    OpenAPI_list_t *action_times,
    OpenAPI_list_t *used_ana_types,
    bool is_impact_ind,
    int impact_ind
)
{
    OpenAPI_analytics_feedback_info_t *analytics_feedback_info_local_var = ogs_malloc(sizeof(OpenAPI_analytics_feedback_info_t));
    ogs_assert(analytics_feedback_info_local_var);

    analytics_feedback_info_local_var->action_times = action_times;
    analytics_feedback_info_local_var->used_ana_types = used_ana_types;
    analytics_feedback_info_local_var->is_impact_ind = is_impact_ind;
    analytics_feedback_info_local_var->impact_ind = impact_ind;

    return analytics_feedback_info_local_var;
}

void OpenAPI_analytics_feedback_info_free(OpenAPI_analytics_feedback_info_t *analytics_feedback_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == analytics_feedback_info) {
        return;
    }
    if (analytics_feedback_info->action_times) {
        OpenAPI_list_for_each(analytics_feedback_info->action_times, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(analytics_feedback_info->action_times);
        analytics_feedback_info->action_times = NULL;
    }
    if (analytics_feedback_info->used_ana_types) {
        OpenAPI_list_free(analytics_feedback_info->used_ana_types);
        analytics_feedback_info->used_ana_types = NULL;
    }
    ogs_free(analytics_feedback_info);
}

cJSON *OpenAPI_analytics_feedback_info_convertToJSON(OpenAPI_analytics_feedback_info_t *analytics_feedback_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (analytics_feedback_info == NULL) {
        ogs_error("OpenAPI_analytics_feedback_info_convertToJSON() failed [AnalyticsFeedbackInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!analytics_feedback_info->action_times) {
        ogs_error("OpenAPI_analytics_feedback_info_convertToJSON() failed [action_times]");
        return NULL;
    }
    cJSON *action_timesList = cJSON_AddArrayToObject(item, "actionTimes");
    if (action_timesList == NULL) {
        ogs_error("OpenAPI_analytics_feedback_info_convertToJSON() failed [action_times]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_feedback_info->action_times, node) {
    }

    if (analytics_feedback_info->used_ana_types != OpenAPI_nwdaf_event_NULL) {
    cJSON *used_ana_typesList = cJSON_AddArrayToObject(item, "usedAnaTypes");
    if (used_ana_typesList == NULL) {
        ogs_error("OpenAPI_analytics_feedback_info_convertToJSON() failed [used_ana_types]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_feedback_info->used_ana_types, node) {
        if (cJSON_AddStringToObject(used_ana_typesList, "", OpenAPI_nwdaf_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_analytics_feedback_info_convertToJSON() failed [used_ana_types]");
            goto end;
        }
    }
    }

    if (analytics_feedback_info->is_impact_ind) {
    if (cJSON_AddBoolToObject(item, "impactInd", analytics_feedback_info->impact_ind) == NULL) {
        ogs_error("OpenAPI_analytics_feedback_info_convertToJSON() failed [impact_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_analytics_feedback_info_t *OpenAPI_analytics_feedback_info_parseFromJSON(cJSON *analytics_feedback_infoJSON)
{
    OpenAPI_analytics_feedback_info_t *analytics_feedback_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *action_times = NULL;
    OpenAPI_list_t *action_timesList = NULL;
    cJSON *used_ana_types = NULL;
    OpenAPI_list_t *used_ana_typesList = NULL;
    cJSON *impact_ind = NULL;
    action_times = cJSON_GetObjectItemCaseSensitive(analytics_feedback_infoJSON, "actionTimes");
    if (!action_times) {
        ogs_error("OpenAPI_analytics_feedback_info_parseFromJSON() failed [action_times]");
        goto end;
    }
        cJSON *action_times_local = NULL;
        if (!cJSON_IsArray(action_times)) {
            ogs_error("OpenAPI_analytics_feedback_info_parseFromJSON() failed [action_times]");
            goto end;
        }

        action_timesList = OpenAPI_list_create();

        cJSON_ArrayForEach(action_times_local, action_times) {
            double *localDouble = NULL;
            int *localInt = NULL;
        }

    used_ana_types = cJSON_GetObjectItemCaseSensitive(analytics_feedback_infoJSON, "usedAnaTypes");
    if (used_ana_types) {
        cJSON *used_ana_types_local = NULL;
        if (!cJSON_IsArray(used_ana_types)) {
            ogs_error("OpenAPI_analytics_feedback_info_parseFromJSON() failed [used_ana_types]");
            goto end;
        }

        used_ana_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(used_ana_types_local, used_ana_types) {
            OpenAPI_nwdaf_event_e localEnum = OpenAPI_nwdaf_event_NULL;
            if (!cJSON_IsString(used_ana_types_local)) {
                ogs_error("OpenAPI_analytics_feedback_info_parseFromJSON() failed [used_ana_types]");
                goto end;
            }
            localEnum = OpenAPI_nwdaf_event_FromString(used_ana_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"used_ana_types\" is not supported. Ignoring it ...",
                         used_ana_types_local->valuestring);
            } else {
                OpenAPI_list_add(used_ana_typesList, (void *)localEnum);
            }
        }
        if (used_ana_typesList->count == 0) {
            ogs_error("OpenAPI_analytics_feedback_info_parseFromJSON() failed: Expected used_ana_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    impact_ind = cJSON_GetObjectItemCaseSensitive(analytics_feedback_infoJSON, "impactInd");
    if (impact_ind) {
    if (!cJSON_IsBool(impact_ind)) {
        ogs_error("OpenAPI_analytics_feedback_info_parseFromJSON() failed [impact_ind]");
        goto end;
    }
    }

    analytics_feedback_info_local_var = OpenAPI_analytics_feedback_info_create (
        action_timesList,
        used_ana_types ? used_ana_typesList : NULL,
        impact_ind ? true : false,
        impact_ind ? impact_ind->valueint : 0
    );

    return analytics_feedback_info_local_var;
end:
    if (action_timesList) {
        OpenAPI_list_for_each(action_timesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(action_timesList);
        action_timesList = NULL;
    }
    if (used_ana_typesList) {
        OpenAPI_list_free(used_ana_typesList);
        used_ana_typesList = NULL;
    }
    return NULL;
}

OpenAPI_analytics_feedback_info_t *OpenAPI_analytics_feedback_info_copy(OpenAPI_analytics_feedback_info_t *dst, OpenAPI_analytics_feedback_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_analytics_feedback_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_analytics_feedback_info_convertToJSON() failed");
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

    OpenAPI_analytics_feedback_info_free(dst);
    dst = OpenAPI_analytics_feedback_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

