
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event.h"

OpenAPI_amf_event_t *OpenAPI_amf_event_create(
    OpenAPI_amf_event_type_t *type,
    int immediate_flag,
    OpenAPI_list_t *area_list,
    OpenAPI_list_t *location_filter_list,
    int ref_id,
    OpenAPI_list_t *traffic_descriptor_list
    )
{
    OpenAPI_amf_event_t *amf_event_local_var = OpenAPI_malloc(sizeof(OpenAPI_amf_event_t));
    if (!amf_event_local_var) {
        return NULL;
    }
    amf_event_local_var->type = type;
    amf_event_local_var->immediate_flag = immediate_flag;
    amf_event_local_var->area_list = area_list;
    amf_event_local_var->location_filter_list = location_filter_list;
    amf_event_local_var->ref_id = ref_id;
    amf_event_local_var->traffic_descriptor_list = traffic_descriptor_list;

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
    if (!amf_event->type) {
        ogs_error("OpenAPI_amf_event_convertToJSON() failed [type]");
        goto end;
    }
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

    if (amf_event->immediate_flag) {
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

    if (amf_event->ref_id) {
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
        if (!cJSON_IsArray(area_list)) {
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

            OpenAPI_list_add(area_listList, area_listItem);
        }
    }

    cJSON *location_filter_list = cJSON_GetObjectItemCaseSensitive(amf_eventJSON, "locationFilterList");

    OpenAPI_list_t *location_filter_listList;
    if (location_filter_list) {
        cJSON *location_filter_list_local_nonprimitive;
        if (!cJSON_IsArray(location_filter_list)) {
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
        if (!cJSON_IsArray(traffic_descriptor_list)) {
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

            OpenAPI_list_add(traffic_descriptor_listList, traffic_descriptor_listItem);
        }
    }

    amf_event_local_var = OpenAPI_amf_event_create (
        type_local_nonprim,
        immediate_flag ? immediate_flag->valueint : 0,
        area_list ? area_listList : NULL,
        location_filter_list ? location_filter_listList : NULL,
        ref_id ? ref_id->valuedouble : 0,
        traffic_descriptor_list ? traffic_descriptor_listList : NULL
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

