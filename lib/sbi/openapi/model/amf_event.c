
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event.h"

OpenAPI_amf_event_t *OpenAPI_amf_event_create(
    OpenAPI_amf_event_type_t *type,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_list_t *area_list,
    OpenAPI_list_t *location_filter_list,
    bool is_ref_id,
    int ref_id,
    OpenAPI_list_t *traffic_descriptor_list,
    bool is_report_ue_reachable,
    int report_ue_reachable,
    OpenAPI_reachability_filter_t *reachability_filter,
    bool is_max_reports,
    int max_reports,
    bool is_max_response_time,
    int max_response_time
)
{
    OpenAPI_amf_event_t *amf_event_local_var = ogs_malloc(sizeof(OpenAPI_amf_event_t));
    ogs_assert(amf_event_local_var);

    amf_event_local_var->type = type;
    amf_event_local_var->is_immediate_flag = is_immediate_flag;
    amf_event_local_var->immediate_flag = immediate_flag;
    amf_event_local_var->area_list = area_list;
    amf_event_local_var->location_filter_list = location_filter_list;
    amf_event_local_var->is_ref_id = is_ref_id;
    amf_event_local_var->ref_id = ref_id;
    amf_event_local_var->traffic_descriptor_list = traffic_descriptor_list;
    amf_event_local_var->is_report_ue_reachable = is_report_ue_reachable;
    amf_event_local_var->report_ue_reachable = report_ue_reachable;
    amf_event_local_var->reachability_filter = reachability_filter;
    amf_event_local_var->is_max_reports = is_max_reports;
    amf_event_local_var->max_reports = max_reports;
    amf_event_local_var->is_max_response_time = is_max_response_time;
    amf_event_local_var->max_response_time = max_response_time;

    return amf_event_local_var;
}

void OpenAPI_amf_event_free(OpenAPI_amf_event_t *amf_event)
{
    if (NULL == amf_event) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_amf_event_type_free(amf_event->type);
    OpenAPI_list_for_each(amf_event->area_list, node) {
        OpenAPI_amf_event_area_free(node->data);
    }
    OpenAPI_list_free(amf_event->area_list);
    OpenAPI_list_for_each(amf_event->location_filter_list, node) {
        OpenAPI_location_filter_free(node->data);
    }
    OpenAPI_list_free(amf_event->location_filter_list);
    OpenAPI_list_for_each(amf_event->traffic_descriptor_list, node) {
        OpenAPI_traffic_descriptor_free(node->data);
    }
    OpenAPI_list_free(amf_event->traffic_descriptor_list);
    OpenAPI_reachability_filter_free(amf_event->reachability_filter);
    ogs_free(amf_event);
}

cJSON *OpenAPI_amf_event_convertToJSON(OpenAPI_amf_event_t *amf_event)
{
    cJSON *item = NULL;

    if (amf_event == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [AmfEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *type_local_JSON = OpenAPI_amf_event_type_convertToJSON(amf_event->type);
    if (type_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "type", type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [type]");
        goto end;
    }

    if (amf_event->is_immediate_flag) {
    if (cJSON_AddBoolToObject(item, "immediateFlag", amf_event->immediate_flag) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [immediate_flag]");
        goto end;
    }
    }

    if (amf_event->area_list) {
    cJSON *area_listList = cJSON_AddArrayToObject(item, "areaList");
    if (area_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [area_list]");
        goto end;
    }

    OpenAPI_lnode_t *area_list_node;
    if (amf_event->area_list) {
        OpenAPI_list_for_each(amf_event->area_list, area_list_node) {
            cJSON *itemLocal = OpenAPI_amf_event_area_convertToJSON(area_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_event_convertToJSON() failed [area_list]");
                goto end;
            }
            cJSON_AddItemToArray(area_listList, itemLocal);
        }
    }
    }

    if (amf_event->location_filter_list) {
    cJSON *location_filter_listList = cJSON_AddArrayToObject(item, "locationFilterList");
    if (location_filter_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [location_filter_list]");
        goto end;
    }

    OpenAPI_lnode_t *location_filter_list_node;
    if (amf_event->location_filter_list) {
        OpenAPI_list_for_each(amf_event->location_filter_list, location_filter_list_node) {
            cJSON *itemLocal = OpenAPI_location_filter_convertToJSON(location_filter_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_event_convertToJSON() failed [location_filter_list]");
                goto end;
            }
            cJSON_AddItemToArray(location_filter_listList, itemLocal);
        }
    }
    }

    if (amf_event->is_ref_id) {
    if (cJSON_AddNumberToObject(item, "refId", amf_event->ref_id) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [ref_id]");
        goto end;
    }
    }

    if (amf_event->traffic_descriptor_list) {
    cJSON *traffic_descriptor_listList = cJSON_AddArrayToObject(item, "trafficDescriptorList");
    if (traffic_descriptor_listList == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [traffic_descriptor_list]");
        goto end;
    }

    OpenAPI_lnode_t *traffic_descriptor_list_node;
    if (amf_event->traffic_descriptor_list) {
        OpenAPI_list_for_each(amf_event->traffic_descriptor_list, traffic_descriptor_list_node) {
            cJSON *itemLocal = OpenAPI_traffic_descriptor_convertToJSON(traffic_descriptor_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_event_convertToJSON() failed [traffic_descriptor_list]");
                goto end;
            }
            cJSON_AddItemToArray(traffic_descriptor_listList, itemLocal);
        }
    }
    }

    if (amf_event->is_report_ue_reachable) {
    if (cJSON_AddBoolToObject(item, "reportUeReachable", amf_event->report_ue_reachable) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [report_ue_reachable]");
        goto end;
    }
    }

    if (amf_event->reachability_filter) {
    cJSON *reachability_filter_local_JSON = OpenAPI_reachability_filter_convertToJSON(amf_event->reachability_filter);
    if (reachability_filter_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [reachability_filter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reachabilityFilter", reachability_filter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [reachability_filter]");
        goto end;
    }
    }

    if (amf_event->is_max_reports) {
    if (cJSON_AddNumberToObject(item, "maxReports", amf_event->max_reports) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [max_reports]");
        goto end;
    }
    }

    if (amf_event->is_max_response_time) {
    if (cJSON_AddNumberToObject(item, "maxResponseTime", amf_event->max_response_time) == NULL) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [max_response_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_event_t *OpenAPI_amf_event_parseFromJSON(cJSON *amf_eventJSON)
{
    OpenAPI_amf_event_t *amf_event_local_var = NULL;
    cJSON *type = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "type");
    if (!type) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [type]");
        goto end;
    }

    OpenAPI_amf_event_type_t *type_local_nonprim = NULL;
    type_local_nonprim = OpenAPI_amf_event_type_parseFromJSON(type);

    cJSON *immediate_flag = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "immediateFlag");

    if (immediate_flag) {
    if (!cJSON_IsBool(immediate_flag)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [immediate_flag]");
        goto end;
    }
    }

    cJSON *area_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "areaList");

    OpenAPI_list_t *area_listList;
    if (area_list) {
    cJSON *area_list_local_nonprimitive;
    if (!cJSON_IsArray(area_list)){
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [area_list]");
        goto end;
    }

    area_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(area_list_local_nonprimitive, area_list ) {
        if (!cJSON_IsObject(area_list_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [area_list]");
            goto end;
        }
        OpenAPI_amf_event_area_t *area_listItem = OpenAPI_amf_event_area_parseFromJSON(area_list_local_nonprimitive);

        if (!area_listItem) {
            ogs_error("No area_listItem");
            OpenAPI_list_free(area_listList);
            goto end;
        }

        OpenAPI_list_add(area_listList, area_listItem);
    }
    }

    cJSON *location_filter_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "locationFilterList");

    OpenAPI_list_t *location_filter_listList;
    if (location_filter_list) {
    cJSON *location_filter_list_local_nonprimitive;
    if (!cJSON_IsArray(location_filter_list)){
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [location_filter_list]");
        goto end;
    }

    location_filter_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(location_filter_list_local_nonprimitive, location_filter_list ) {
        if (!cJSON_IsObject(location_filter_list_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [location_filter_list]");
            goto end;
        }
        OpenAPI_location_filter_t *location_filter_listItem = OpenAPI_location_filter_parseFromJSON(location_filter_list_local_nonprimitive);

        if (!location_filter_listItem) {
            ogs_error("No location_filter_listItem");
            OpenAPI_list_free(location_filter_listList);
            goto end;
        }

        OpenAPI_list_add(location_filter_listList, location_filter_listItem);
    }
    }

    cJSON *ref_id = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "refId");

    if (ref_id) {
    if (!cJSON_IsNumber(ref_id)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [ref_id]");
        goto end;
    }
    }

    cJSON *traffic_descriptor_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "trafficDescriptorList");

    OpenAPI_list_t *traffic_descriptor_listList;
    if (traffic_descriptor_list) {
    cJSON *traffic_descriptor_list_local_nonprimitive;
    if (!cJSON_IsArray(traffic_descriptor_list)){
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [traffic_descriptor_list]");
        goto end;
    }

    traffic_descriptor_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(traffic_descriptor_list_local_nonprimitive, traffic_descriptor_list ) {
        if (!cJSON_IsObject(traffic_descriptor_list_local_nonprimitive)) {
            ogs_error("OpenAPI_amf_event_parseFromJSON() failed [traffic_descriptor_list]");
            goto end;
        }
        OpenAPI_traffic_descriptor_t *traffic_descriptor_listItem = OpenAPI_traffic_descriptor_parseFromJSON(traffic_descriptor_list_local_nonprimitive);

        if (!traffic_descriptor_listItem) {
            ogs_error("No traffic_descriptor_listItem");
            OpenAPI_list_free(traffic_descriptor_listList);
            goto end;
        }

        OpenAPI_list_add(traffic_descriptor_listList, traffic_descriptor_listItem);
    }
    }

    cJSON *report_ue_reachable = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "reportUeReachable");

    if (report_ue_reachable) {
    if (!cJSON_IsBool(report_ue_reachable)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [report_ue_reachable]");
        goto end;
    }
    }

    cJSON *reachability_filter = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "reachabilityFilter");

    OpenAPI_reachability_filter_t *reachability_filter_local_nonprim = NULL;
    if (reachability_filter) {
    reachability_filter_local_nonprim = OpenAPI_reachability_filter_parseFromJSON(reachability_filter);
    }

    cJSON *max_reports = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "maxReports");

    if (max_reports) {
    if (!cJSON_IsNumber(max_reports)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [max_reports]");
        goto end;
    }
    }

    cJSON *max_response_time = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "maxResponseTime");

    if (max_response_time) {
    if (!cJSON_IsNumber(max_response_time)) {
        ogs_error("OpenAPI_amf_event_parseFromJSON() failed [max_response_time]");
        goto end;
    }
    }

    amf_event_local_var = OpenAPI_amf_event_create (
        type_local_nonprim,
        immediate_flag ? true : false,
        immediate_flag ? immediate_flag->valueint : 0,
        area_list ? area_listList : NULL,
        location_filter_list ? location_filter_listList : NULL,
        ref_id ? true : false,
        ref_id ? ref_id->valuedouble : 0,
        traffic_descriptor_list ? traffic_descriptor_listList : NULL,
        report_ue_reachable ? true : false,
        report_ue_reachable ? report_ue_reachable->valueint : 0,
        reachability_filter ? reachability_filter_local_nonprim : NULL,
        max_reports ? true : false,
        max_reports ? max_reports->valuedouble : 0,
        max_response_time ? true : false,
        max_response_time ? max_response_time->valuedouble : 0
    );

    return amf_event_local_var;
end:
    return NULL;
}

OpenAPI_amf_event_t *OpenAPI_amf_event_copy(OpenAPI_amf_event_t *dst, OpenAPI_amf_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed");
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

    OpenAPI_amf_event_free(dst);
    dst = OpenAPI_amf_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

