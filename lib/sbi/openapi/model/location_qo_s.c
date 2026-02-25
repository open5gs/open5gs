
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_qo_s.h"

OpenAPI_location_qo_s_t *OpenAPI_location_qo_s_create(
    bool is_h_accuracy,
    float h_accuracy,
    bool is_v_accuracy,
    float v_accuracy,
    bool is_vertical_requested,
    int vertical_requested,
    OpenAPI_response_time_t *response_time,
    OpenAPI_list_t *minor_loc_qoses,
    OpenAPI_lcs_qos_class_t *lcs_qos_class
)
{
    OpenAPI_location_qo_s_t *location_qo_s_local_var = ogs_malloc(sizeof(OpenAPI_location_qo_s_t));
    ogs_assert(location_qo_s_local_var);

    location_qo_s_local_var->is_h_accuracy = is_h_accuracy;
    location_qo_s_local_var->h_accuracy = h_accuracy;
    location_qo_s_local_var->is_v_accuracy = is_v_accuracy;
    location_qo_s_local_var->v_accuracy = v_accuracy;
    location_qo_s_local_var->is_vertical_requested = is_vertical_requested;
    location_qo_s_local_var->vertical_requested = vertical_requested;
    location_qo_s_local_var->response_time = response_time;
    location_qo_s_local_var->minor_loc_qoses = minor_loc_qoses;
    location_qo_s_local_var->lcs_qos_class = lcs_qos_class;

    return location_qo_s_local_var;
}

void OpenAPI_location_qo_s_free(OpenAPI_location_qo_s_t *location_qo_s)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_qo_s) {
        return;
    }
    if (location_qo_s->response_time) {
        OpenAPI_response_time_free(location_qo_s->response_time);
        location_qo_s->response_time = NULL;
    }
    if (location_qo_s->minor_loc_qoses) {
        OpenAPI_list_for_each(location_qo_s->minor_loc_qoses, node) {
            OpenAPI_minor_location_qo_s_free(node->data);
        }
        OpenAPI_list_free(location_qo_s->minor_loc_qoses);
        location_qo_s->minor_loc_qoses = NULL;
    }
    if (location_qo_s->lcs_qos_class) {
        OpenAPI_lcs_qos_class_free(location_qo_s->lcs_qos_class);
        location_qo_s->lcs_qos_class = NULL;
    }
    ogs_free(location_qo_s);
}

cJSON *OpenAPI_location_qo_s_convertToJSON(OpenAPI_location_qo_s_t *location_qo_s)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (location_qo_s == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [LocationQoS]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (location_qo_s->is_h_accuracy) {
    if (cJSON_AddNumberToObject(item, "hAccuracy", location_qo_s->h_accuracy) == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [h_accuracy]");
        goto end;
    }
    }

    if (location_qo_s->is_v_accuracy) {
    if (cJSON_AddNumberToObject(item, "vAccuracy", location_qo_s->v_accuracy) == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [v_accuracy]");
        goto end;
    }
    }

    if (location_qo_s->is_vertical_requested) {
    if (cJSON_AddBoolToObject(item, "verticalRequested", location_qo_s->vertical_requested) == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [vertical_requested]");
        goto end;
    }
    }

    if (location_qo_s->response_time) {
    cJSON *response_time_local_JSON = OpenAPI_response_time_convertToJSON(location_qo_s->response_time);
    if (response_time_local_JSON == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [response_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "responseTime", response_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [response_time]");
        goto end;
    }
    }

    if (location_qo_s->minor_loc_qoses) {
    cJSON *minor_loc_qosesList = cJSON_AddArrayToObject(item, "minorLocQoses");
    if (minor_loc_qosesList == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [minor_loc_qoses]");
        goto end;
    }
    OpenAPI_list_for_each(location_qo_s->minor_loc_qoses, node) {
        cJSON *itemLocal = OpenAPI_minor_location_qo_s_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [minor_loc_qoses]");
            goto end;
        }
        cJSON_AddItemToArray(minor_loc_qosesList, itemLocal);
    }
    }

    if (location_qo_s->lcs_qos_class) {
    cJSON *lcs_qos_class_local_JSON = OpenAPI_lcs_qos_class_convertToJSON(location_qo_s->lcs_qos_class);
    if (lcs_qos_class_local_JSON == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [lcs_qos_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsQosClass", lcs_qos_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed [lcs_qos_class]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_location_qo_s_t *OpenAPI_location_qo_s_parseFromJSON(cJSON *location_qo_sJSON)
{
    OpenAPI_location_qo_s_t *location_qo_s_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_accuracy = NULL;
    cJSON *v_accuracy = NULL;
    cJSON *vertical_requested = NULL;
    cJSON *response_time = NULL;
    OpenAPI_response_time_t *response_time_local_nonprim = NULL;
    cJSON *minor_loc_qoses = NULL;
    OpenAPI_list_t *minor_loc_qosesList = NULL;
    cJSON *lcs_qos_class = NULL;
    OpenAPI_lcs_qos_class_t *lcs_qos_class_local_nonprim = NULL;
    h_accuracy = cJSON_GetObjectItemCaseSensitive(location_qo_sJSON, "hAccuracy");
    if (h_accuracy) {
    if (!cJSON_IsNumber(h_accuracy)) {
        ogs_error("OpenAPI_location_qo_s_parseFromJSON() failed [h_accuracy]");
        goto end;
    }
    }

    v_accuracy = cJSON_GetObjectItemCaseSensitive(location_qo_sJSON, "vAccuracy");
    if (v_accuracy) {
    if (!cJSON_IsNumber(v_accuracy)) {
        ogs_error("OpenAPI_location_qo_s_parseFromJSON() failed [v_accuracy]");
        goto end;
    }
    }

    vertical_requested = cJSON_GetObjectItemCaseSensitive(location_qo_sJSON, "verticalRequested");
    if (vertical_requested) {
    if (!cJSON_IsBool(vertical_requested)) {
        ogs_error("OpenAPI_location_qo_s_parseFromJSON() failed [vertical_requested]");
        goto end;
    }
    }

    response_time = cJSON_GetObjectItemCaseSensitive(location_qo_sJSON, "responseTime");
    if (response_time) {
    response_time_local_nonprim = OpenAPI_response_time_parseFromJSON(response_time);
    if (!response_time_local_nonprim) {
        ogs_error("OpenAPI_response_time_parseFromJSON failed [response_time]");
        goto end;
    }
    }

    minor_loc_qoses = cJSON_GetObjectItemCaseSensitive(location_qo_sJSON, "minorLocQoses");
    if (minor_loc_qoses) {
        cJSON *minor_loc_qoses_local = NULL;
        if (!cJSON_IsArray(minor_loc_qoses)) {
            ogs_error("OpenAPI_location_qo_s_parseFromJSON() failed [minor_loc_qoses]");
            goto end;
        }

        minor_loc_qosesList = OpenAPI_list_create();

        cJSON_ArrayForEach(minor_loc_qoses_local, minor_loc_qoses) {
            if (!cJSON_IsObject(minor_loc_qoses_local)) {
                ogs_error("OpenAPI_location_qo_s_parseFromJSON() failed [minor_loc_qoses]");
                goto end;
            }
            OpenAPI_minor_location_qo_s_t *minor_loc_qosesItem = OpenAPI_minor_location_qo_s_parseFromJSON(minor_loc_qoses_local);
            if (!minor_loc_qosesItem) {
                ogs_error("No minor_loc_qosesItem");
                goto end;
            }
            OpenAPI_list_add(minor_loc_qosesList, minor_loc_qosesItem);
        }
    }

    lcs_qos_class = cJSON_GetObjectItemCaseSensitive(location_qo_sJSON, "lcsQosClass");
    if (lcs_qos_class) {
    lcs_qos_class_local_nonprim = OpenAPI_lcs_qos_class_parseFromJSON(lcs_qos_class);
    if (!lcs_qos_class_local_nonprim) {
        ogs_error("OpenAPI_lcs_qos_class_parseFromJSON failed [lcs_qos_class]");
        goto end;
    }
    }

    location_qo_s_local_var = OpenAPI_location_qo_s_create (
        h_accuracy ? true : false,
        h_accuracy ? h_accuracy->valuedouble : 0,
        v_accuracy ? true : false,
        v_accuracy ? v_accuracy->valuedouble : 0,
        vertical_requested ? true : false,
        vertical_requested ? vertical_requested->valueint : 0,
        response_time ? response_time_local_nonprim : NULL,
        minor_loc_qoses ? minor_loc_qosesList : NULL,
        lcs_qos_class ? lcs_qos_class_local_nonprim : NULL
    );

    return location_qo_s_local_var;
end:
    if (response_time_local_nonprim) {
        OpenAPI_response_time_free(response_time_local_nonprim);
        response_time_local_nonprim = NULL;
    }
    if (minor_loc_qosesList) {
        OpenAPI_list_for_each(minor_loc_qosesList, node) {
            OpenAPI_minor_location_qo_s_free(node->data);
        }
        OpenAPI_list_free(minor_loc_qosesList);
        minor_loc_qosesList = NULL;
    }
    if (lcs_qos_class_local_nonprim) {
        OpenAPI_lcs_qos_class_free(lcs_qos_class_local_nonprim);
        lcs_qos_class_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_location_qo_s_t *OpenAPI_location_qo_s_copy(OpenAPI_location_qo_s_t *dst, OpenAPI_location_qo_s_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_qo_s_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_qo_s_convertToJSON() failed");
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

    OpenAPI_location_qo_s_free(dst);
    dst = OpenAPI_location_qo_s_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

