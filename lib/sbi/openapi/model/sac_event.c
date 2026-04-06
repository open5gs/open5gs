
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sac_event.h"

OpenAPI_sac_event_t *OpenAPI_sac_event_create(
    OpenAPI_sac_event_type_e event_type,
    OpenAPI_sac_event_trigger_e event_trigger,
    OpenAPI_list_t *event_filter,
    bool is_notification_period,
    int notification_period,
    OpenAPI_sac_info_t *notif_threshold,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_list_t *var_rep_period_info,
    OpenAPI_plmn_id_nid_t *plmn_id_nid,
    OpenAPI_list_t *access_type_filter,
    OpenAPI_list_t *service_area_filter
)
{
    OpenAPI_sac_event_t *sac_event_local_var = ogs_malloc(sizeof(OpenAPI_sac_event_t));
    ogs_assert(sac_event_local_var);

    sac_event_local_var->event_type = event_type;
    sac_event_local_var->event_trigger = event_trigger;
    sac_event_local_var->event_filter = event_filter;
    sac_event_local_var->is_notification_period = is_notification_period;
    sac_event_local_var->notification_period = notification_period;
    sac_event_local_var->notif_threshold = notif_threshold;
    sac_event_local_var->is_immediate_flag = is_immediate_flag;
    sac_event_local_var->immediate_flag = immediate_flag;
    sac_event_local_var->var_rep_period_info = var_rep_period_info;
    sac_event_local_var->plmn_id_nid = plmn_id_nid;
    sac_event_local_var->access_type_filter = access_type_filter;
    sac_event_local_var->service_area_filter = service_area_filter;

    return sac_event_local_var;
}

void OpenAPI_sac_event_free(OpenAPI_sac_event_t *sac_event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sac_event) {
        return;
    }
    if (sac_event->event_filter) {
        OpenAPI_list_for_each(sac_event->event_filter, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(sac_event->event_filter);
        sac_event->event_filter = NULL;
    }
    if (sac_event->notif_threshold) {
        OpenAPI_sac_info_free(sac_event->notif_threshold);
        sac_event->notif_threshold = NULL;
    }
    if (sac_event->var_rep_period_info) {
        OpenAPI_list_for_each(sac_event->var_rep_period_info, node) {
            OpenAPI_var_rep_period_free(node->data);
        }
        OpenAPI_list_free(sac_event->var_rep_period_info);
        sac_event->var_rep_period_info = NULL;
    }
    if (sac_event->plmn_id_nid) {
        OpenAPI_plmn_id_nid_free(sac_event->plmn_id_nid);
        sac_event->plmn_id_nid = NULL;
    }
    if (sac_event->access_type_filter) {
        OpenAPI_list_free(sac_event->access_type_filter);
        sac_event->access_type_filter = NULL;
    }
    if (sac_event->service_area_filter) {
        OpenAPI_list_for_each(sac_event->service_area_filter, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sac_event->service_area_filter);
        sac_event->service_area_filter = NULL;
    }
    ogs_free(sac_event);
}

cJSON *OpenAPI_sac_event_convertToJSON(OpenAPI_sac_event_t *sac_event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sac_event == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [SACEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sac_event->event_type == OpenAPI_sac_event_type_NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [event_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "eventType", OpenAPI_sac_event_type_ToString(sac_event->event_type)) == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [event_type]");
        goto end;
    }

    if (sac_event->event_trigger != OpenAPI_sac_event_trigger_NULL) {
    if (cJSON_AddStringToObject(item, "eventTrigger", OpenAPI_sac_event_trigger_ToString(sac_event->event_trigger)) == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [event_trigger]");
        goto end;
    }
    }

    if (!sac_event->event_filter) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [event_filter]");
        return NULL;
    }
    cJSON *event_filterList = cJSON_AddArrayToObject(item, "eventFilter");
    if (event_filterList == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [event_filter]");
        goto end;
    }
    OpenAPI_list_for_each(sac_event->event_filter, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sac_event_convertToJSON() failed [event_filter]");
            goto end;
        }
        cJSON_AddItemToArray(event_filterList, itemLocal);
    }

    if (sac_event->is_notification_period) {
    if (cJSON_AddNumberToObject(item, "notificationPeriod", sac_event->notification_period) == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [notification_period]");
        goto end;
    }
    }

    if (sac_event->notif_threshold) {
    cJSON *notif_threshold_local_JSON = OpenAPI_sac_info_convertToJSON(sac_event->notif_threshold);
    if (notif_threshold_local_JSON == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [notif_threshold]");
        goto end;
    }
    cJSON_AddItemToObject(item, "notifThreshold", notif_threshold_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [notif_threshold]");
        goto end;
    }
    }

    if (sac_event->is_immediate_flag) {
    if (cJSON_AddBoolToObject(item, "immediateFlag", sac_event->immediate_flag) == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [immediate_flag]");
        goto end;
    }
    }

    if (sac_event->var_rep_period_info) {
    cJSON *var_rep_period_infoList = cJSON_AddArrayToObject(item, "varRepPeriodInfo");
    if (var_rep_period_infoList == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [var_rep_period_info]");
        goto end;
    }
    OpenAPI_list_for_each(sac_event->var_rep_period_info, node) {
        cJSON *itemLocal = OpenAPI_var_rep_period_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sac_event_convertToJSON() failed [var_rep_period_info]");
            goto end;
        }
        cJSON_AddItemToArray(var_rep_period_infoList, itemLocal);
    }
    }

    if (sac_event->plmn_id_nid) {
    cJSON *plmn_id_nid_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(sac_event->plmn_id_nid);
    if (plmn_id_nid_local_JSON == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [plmn_id_nid]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnIdNid", plmn_id_nid_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [plmn_id_nid]");
        goto end;
    }
    }

    if (sac_event->access_type_filter != OpenAPI_access_type_NULL) {
    cJSON *access_type_filterList = cJSON_AddArrayToObject(item, "accessTypeFilter");
    if (access_type_filterList == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [access_type_filter]");
        goto end;
    }
    OpenAPI_list_for_each(sac_event->access_type_filter, node) {
        if (cJSON_AddStringToObject(access_type_filterList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_sac_event_convertToJSON() failed [access_type_filter]");
            goto end;
        }
    }
    }

    if (sac_event->service_area_filter) {
    cJSON *service_area_filterList = cJSON_AddArrayToObject(item, "serviceAreaFilter");
    if (service_area_filterList == NULL) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed [service_area_filter]");
        goto end;
    }
    OpenAPI_list_for_each(sac_event->service_area_filter, node) {
        if (cJSON_AddStringToObject(service_area_filterList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sac_event_convertToJSON() failed [service_area_filter]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_sac_event_t *OpenAPI_sac_event_parseFromJSON(cJSON *sac_eventJSON)
{
    OpenAPI_sac_event_t *sac_event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_type = NULL;
    OpenAPI_sac_event_type_e event_typeVariable = 0;
    cJSON *event_trigger = NULL;
    OpenAPI_sac_event_trigger_e event_triggerVariable = 0;
    cJSON *event_filter = NULL;
    OpenAPI_list_t *event_filterList = NULL;
    cJSON *notification_period = NULL;
    cJSON *notif_threshold = NULL;
    OpenAPI_sac_info_t *notif_threshold_local_nonprim = NULL;
    cJSON *immediate_flag = NULL;
    cJSON *var_rep_period_info = NULL;
    OpenAPI_list_t *var_rep_period_infoList = NULL;
    cJSON *plmn_id_nid = NULL;
    OpenAPI_plmn_id_nid_t *plmn_id_nid_local_nonprim = NULL;
    cJSON *access_type_filter = NULL;
    OpenAPI_list_t *access_type_filterList = NULL;
    cJSON *service_area_filter = NULL;
    OpenAPI_list_t *service_area_filterList = NULL;
    event_type = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "eventType");
    if (!event_type) {
        ogs_error("OpenAPI_sac_event_parseFromJSON() failed [event_type]");
        goto end;
    }
    if (!cJSON_IsString(event_type)) {
        ogs_error("OpenAPI_sac_event_parseFromJSON() failed [event_type]");
        goto end;
    }
    event_typeVariable = OpenAPI_sac_event_type_FromString(event_type->valuestring);

    event_trigger = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "eventTrigger");
    if (event_trigger) {
    if (!cJSON_IsString(event_trigger)) {
        ogs_error("OpenAPI_sac_event_parseFromJSON() failed [event_trigger]");
        goto end;
    }
    event_triggerVariable = OpenAPI_sac_event_trigger_FromString(event_trigger->valuestring);
    }

    event_filter = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "eventFilter");
    if (!event_filter) {
        ogs_error("OpenAPI_sac_event_parseFromJSON() failed [event_filter]");
        goto end;
    }
        cJSON *event_filter_local = NULL;
        if (!cJSON_IsArray(event_filter)) {
            ogs_error("OpenAPI_sac_event_parseFromJSON() failed [event_filter]");
            goto end;
        }

        event_filterList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_filter_local, event_filter) {
            if (!cJSON_IsObject(event_filter_local)) {
                ogs_error("OpenAPI_sac_event_parseFromJSON() failed [event_filter]");
                goto end;
            }
            OpenAPI_snssai_t *event_filterItem = OpenAPI_snssai_parseFromJSON(event_filter_local);
            if (!event_filterItem) {
                ogs_error("No event_filterItem");
                goto end;
            }
            OpenAPI_list_add(event_filterList, event_filterItem);
        }

    notification_period = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "notificationPeriod");
    if (notification_period) {
    if (!cJSON_IsNumber(notification_period)) {
        ogs_error("OpenAPI_sac_event_parseFromJSON() failed [notification_period]");
        goto end;
    }
    }

    notif_threshold = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "notifThreshold");
    if (notif_threshold) {
    notif_threshold_local_nonprim = OpenAPI_sac_info_parseFromJSON(notif_threshold);
    if (!notif_threshold_local_nonprim) {
        ogs_error("OpenAPI_sac_info_parseFromJSON failed [notif_threshold]");
        goto end;
    }
    }

    immediate_flag = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "immediateFlag");
    if (immediate_flag) {
    if (!cJSON_IsBool(immediate_flag)) {
        ogs_error("OpenAPI_sac_event_parseFromJSON() failed [immediate_flag]");
        goto end;
    }
    }

    var_rep_period_info = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "varRepPeriodInfo");
    if (var_rep_period_info) {
        cJSON *var_rep_period_info_local = NULL;
        if (!cJSON_IsArray(var_rep_period_info)) {
            ogs_error("OpenAPI_sac_event_parseFromJSON() failed [var_rep_period_info]");
            goto end;
        }

        var_rep_period_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(var_rep_period_info_local, var_rep_period_info) {
            if (!cJSON_IsObject(var_rep_period_info_local)) {
                ogs_error("OpenAPI_sac_event_parseFromJSON() failed [var_rep_period_info]");
                goto end;
            }
            OpenAPI_var_rep_period_t *var_rep_period_infoItem = OpenAPI_var_rep_period_parseFromJSON(var_rep_period_info_local);
            if (!var_rep_period_infoItem) {
                ogs_error("No var_rep_period_infoItem");
                goto end;
            }
            OpenAPI_list_add(var_rep_period_infoList, var_rep_period_infoItem);
        }
    }

    plmn_id_nid = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "plmnIdNid");
    if (plmn_id_nid) {
    plmn_id_nid_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(plmn_id_nid);
    if (!plmn_id_nid_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [plmn_id_nid]");
        goto end;
    }
    }

    access_type_filter = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "accessTypeFilter");
    if (access_type_filter) {
        cJSON *access_type_filter_local = NULL;
        if (!cJSON_IsArray(access_type_filter)) {
            ogs_error("OpenAPI_sac_event_parseFromJSON() failed [access_type_filter]");
            goto end;
        }

        access_type_filterList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_type_filter_local, access_type_filter) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(access_type_filter_local)) {
                ogs_error("OpenAPI_sac_event_parseFromJSON() failed [access_type_filter]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(access_type_filter_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"access_type_filter\" is not supported. Ignoring it ...",
                         access_type_filter_local->valuestring);
            } else {
                OpenAPI_list_add(access_type_filterList, (void *)localEnum);
            }
        }
        if (access_type_filterList->count == 0) {
            ogs_error("OpenAPI_sac_event_parseFromJSON() failed: Expected access_type_filterList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    service_area_filter = cJSON_GetObjectItemCaseSensitive(sac_eventJSON, "serviceAreaFilter");
    if (service_area_filter) {
        cJSON *service_area_filter_local = NULL;
        if (!cJSON_IsArray(service_area_filter)) {
            ogs_error("OpenAPI_sac_event_parseFromJSON() failed [service_area_filter]");
            goto end;
        }

        service_area_filterList = OpenAPI_list_create();

        cJSON_ArrayForEach(service_area_filter_local, service_area_filter) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(service_area_filter_local)) {
                ogs_error("OpenAPI_sac_event_parseFromJSON() failed [service_area_filter]");
                goto end;
            }
            OpenAPI_list_add(service_area_filterList, ogs_strdup(service_area_filter_local->valuestring));
        }
    }

    sac_event_local_var = OpenAPI_sac_event_create (
        event_typeVariable,
        event_trigger ? event_triggerVariable : 0,
        event_filterList,
        notification_period ? true : false,
        notification_period ? notification_period->valuedouble : 0,
        notif_threshold ? notif_threshold_local_nonprim : NULL,
        immediate_flag ? true : false,
        immediate_flag ? immediate_flag->valueint : 0,
        var_rep_period_info ? var_rep_period_infoList : NULL,
        plmn_id_nid ? plmn_id_nid_local_nonprim : NULL,
        access_type_filter ? access_type_filterList : NULL,
        service_area_filter ? service_area_filterList : NULL
    );

    return sac_event_local_var;
end:
    if (event_filterList) {
        OpenAPI_list_for_each(event_filterList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(event_filterList);
        event_filterList = NULL;
    }
    if (notif_threshold_local_nonprim) {
        OpenAPI_sac_info_free(notif_threshold_local_nonprim);
        notif_threshold_local_nonprim = NULL;
    }
    if (var_rep_period_infoList) {
        OpenAPI_list_for_each(var_rep_period_infoList, node) {
            OpenAPI_var_rep_period_free(node->data);
        }
        OpenAPI_list_free(var_rep_period_infoList);
        var_rep_period_infoList = NULL;
    }
    if (plmn_id_nid_local_nonprim) {
        OpenAPI_plmn_id_nid_free(plmn_id_nid_local_nonprim);
        plmn_id_nid_local_nonprim = NULL;
    }
    if (access_type_filterList) {
        OpenAPI_list_free(access_type_filterList);
        access_type_filterList = NULL;
    }
    if (service_area_filterList) {
        OpenAPI_list_for_each(service_area_filterList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_area_filterList);
        service_area_filterList = NULL;
    }
    return NULL;
}

OpenAPI_sac_event_t *OpenAPI_sac_event_copy(OpenAPI_sac_event_t *dst, OpenAPI_sac_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sac_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sac_event_convertToJSON() failed");
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

    OpenAPI_sac_event_free(dst);
    dst = OpenAPI_sac_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

