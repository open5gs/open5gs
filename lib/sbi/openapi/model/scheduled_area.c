
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scheduled_area.h"

OpenAPI_scheduled_area_t *OpenAPI_scheduled_area_create(
    OpenAPI_list_t *tai_list,
    char *enter_time,
    char *leave_time,
    bool is_altitude_low,
    double altitude_low,
    bool is_altitude_high,
    double altitude_high,
    OpenAPI_list_t *ran_node_id_list,
    OpenAPI_list_t *ncgi_list,
    OpenAPI_list_t *ecgi_list
)
{
    OpenAPI_scheduled_area_t *scheduled_area_local_var = ogs_malloc(sizeof(OpenAPI_scheduled_area_t));
    ogs_assert(scheduled_area_local_var);

    scheduled_area_local_var->tai_list = tai_list;
    scheduled_area_local_var->enter_time = enter_time;
    scheduled_area_local_var->leave_time = leave_time;
    scheduled_area_local_var->is_altitude_low = is_altitude_low;
    scheduled_area_local_var->altitude_low = altitude_low;
    scheduled_area_local_var->is_altitude_high = is_altitude_high;
    scheduled_area_local_var->altitude_high = altitude_high;
    scheduled_area_local_var->ran_node_id_list = ran_node_id_list;
    scheduled_area_local_var->ncgi_list = ncgi_list;
    scheduled_area_local_var->ecgi_list = ecgi_list;

    return scheduled_area_local_var;
}

void OpenAPI_scheduled_area_free(OpenAPI_scheduled_area_t *scheduled_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == scheduled_area) {
        return;
    }
    if (scheduled_area->tai_list) {
        OpenAPI_list_for_each(scheduled_area->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(scheduled_area->tai_list);
        scheduled_area->tai_list = NULL;
    }
    if (scheduled_area->enter_time) {
        ogs_free(scheduled_area->enter_time);
        scheduled_area->enter_time = NULL;
    }
    if (scheduled_area->leave_time) {
        ogs_free(scheduled_area->leave_time);
        scheduled_area->leave_time = NULL;
    }
    if (scheduled_area->ran_node_id_list) {
        OpenAPI_list_for_each(scheduled_area->ran_node_id_list, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(scheduled_area->ran_node_id_list);
        scheduled_area->ran_node_id_list = NULL;
    }
    if (scheduled_area->ncgi_list) {
        OpenAPI_list_for_each(scheduled_area->ncgi_list, node) {
            OpenAPI_ncgi_free(node->data);
        }
        OpenAPI_list_free(scheduled_area->ncgi_list);
        scheduled_area->ncgi_list = NULL;
    }
    if (scheduled_area->ecgi_list) {
        OpenAPI_list_for_each(scheduled_area->ecgi_list, node) {
            OpenAPI_ecgi_free(node->data);
        }
        OpenAPI_list_free(scheduled_area->ecgi_list);
        scheduled_area->ecgi_list = NULL;
    }
    ogs_free(scheduled_area);
}

cJSON *OpenAPI_scheduled_area_convertToJSON(OpenAPI_scheduled_area_t *scheduled_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (scheduled_area == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [ScheduledArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (scheduled_area->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(scheduled_area->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (scheduled_area->enter_time) {
    if (cJSON_AddStringToObject(item, "enterTime", scheduled_area->enter_time) == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [enter_time]");
        goto end;
    }
    }

    if (scheduled_area->leave_time) {
    if (cJSON_AddStringToObject(item, "leaveTime", scheduled_area->leave_time) == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [leave_time]");
        goto end;
    }
    }

    if (scheduled_area->is_altitude_low) {
    if (cJSON_AddNumberToObject(item, "altitudeLow", scheduled_area->altitude_low) == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [altitude_low]");
        goto end;
    }
    }

    if (scheduled_area->is_altitude_high) {
    if (cJSON_AddNumberToObject(item, "altitudeHigh", scheduled_area->altitude_high) == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [altitude_high]");
        goto end;
    }
    }

    if (scheduled_area->ran_node_id_list) {
    cJSON *ran_node_id_listList = cJSON_AddArrayToObject(item, "ranNodeIdList");
    if (ran_node_id_listList == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [ran_node_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(scheduled_area->ran_node_id_list, node) {
        cJSON *itemLocal = OpenAPI_global_ran_node_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [ran_node_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(ran_node_id_listList, itemLocal);
    }
    }

    if (scheduled_area->ncgi_list) {
    cJSON *ncgi_listList = cJSON_AddArrayToObject(item, "ncgiList");
    if (ncgi_listList == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [ncgi_list]");
        goto end;
    }
    OpenAPI_list_for_each(scheduled_area->ncgi_list, node) {
        cJSON *itemLocal = OpenAPI_ncgi_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [ncgi_list]");
            goto end;
        }
        cJSON_AddItemToArray(ncgi_listList, itemLocal);
    }
    }

    if (scheduled_area->ecgi_list) {
    cJSON *ecgi_listList = cJSON_AddArrayToObject(item, "ecgiList");
    if (ecgi_listList == NULL) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [ecgi_list]");
        goto end;
    }
    OpenAPI_list_for_each(scheduled_area->ecgi_list, node) {
        cJSON *itemLocal = OpenAPI_ecgi_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_scheduled_area_convertToJSON() failed [ecgi_list]");
            goto end;
        }
        cJSON_AddItemToArray(ecgi_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_scheduled_area_t *OpenAPI_scheduled_area_parseFromJSON(cJSON *scheduled_areaJSON)
{
    OpenAPI_scheduled_area_t *scheduled_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *enter_time = NULL;
    cJSON *leave_time = NULL;
    cJSON *altitude_low = NULL;
    cJSON *altitude_high = NULL;
    cJSON *ran_node_id_list = NULL;
    OpenAPI_list_t *ran_node_id_listList = NULL;
    cJSON *ncgi_list = NULL;
    OpenAPI_list_t *ncgi_listList = NULL;
    cJSON *ecgi_list = NULL;
    OpenAPI_list_t *ecgi_listList = NULL;
    tai_list = cJSON_GetObjectItemCaseSensitive(scheduled_areaJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    enter_time = cJSON_GetObjectItemCaseSensitive(scheduled_areaJSON, "enterTime");
    if (enter_time) {
    if (!cJSON_IsString(enter_time) && !cJSON_IsNull(enter_time)) {
        ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [enter_time]");
        goto end;
    }
    }

    leave_time = cJSON_GetObjectItemCaseSensitive(scheduled_areaJSON, "leaveTime");
    if (leave_time) {
    if (!cJSON_IsString(leave_time) && !cJSON_IsNull(leave_time)) {
        ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [leave_time]");
        goto end;
    }
    }

    altitude_low = cJSON_GetObjectItemCaseSensitive(scheduled_areaJSON, "altitudeLow");
    if (altitude_low) {
    if (!cJSON_IsNumber(altitude_low)) {
        ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [altitude_low]");
        goto end;
    }
    }

    altitude_high = cJSON_GetObjectItemCaseSensitive(scheduled_areaJSON, "altitudeHigh");
    if (altitude_high) {
    if (!cJSON_IsNumber(altitude_high)) {
        ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [altitude_high]");
        goto end;
    }
    }

    ran_node_id_list = cJSON_GetObjectItemCaseSensitive(scheduled_areaJSON, "ranNodeIdList");
    if (ran_node_id_list) {
        cJSON *ran_node_id_list_local = NULL;
        if (!cJSON_IsArray(ran_node_id_list)) {
            ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [ran_node_id_list]");
            goto end;
        }

        ran_node_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ran_node_id_list_local, ran_node_id_list) {
            if (!cJSON_IsObject(ran_node_id_list_local)) {
                ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [ran_node_id_list]");
                goto end;
            }
            OpenAPI_global_ran_node_id_t *ran_node_id_listItem = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_id_list_local);
            if (!ran_node_id_listItem) {
                ogs_error("No ran_node_id_listItem");
                goto end;
            }
            OpenAPI_list_add(ran_node_id_listList, ran_node_id_listItem);
        }
    }

    ncgi_list = cJSON_GetObjectItemCaseSensitive(scheduled_areaJSON, "ncgiList");
    if (ncgi_list) {
        cJSON *ncgi_list_local = NULL;
        if (!cJSON_IsArray(ncgi_list)) {
            ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [ncgi_list]");
            goto end;
        }

        ncgi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ncgi_list_local, ncgi_list) {
            if (!cJSON_IsObject(ncgi_list_local)) {
                ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [ncgi_list]");
                goto end;
            }
            OpenAPI_ncgi_t *ncgi_listItem = OpenAPI_ncgi_parseFromJSON(ncgi_list_local);
            if (!ncgi_listItem) {
                ogs_error("No ncgi_listItem");
                goto end;
            }
            OpenAPI_list_add(ncgi_listList, ncgi_listItem);
        }
    }

    ecgi_list = cJSON_GetObjectItemCaseSensitive(scheduled_areaJSON, "ecgiList");
    if (ecgi_list) {
        cJSON *ecgi_list_local = NULL;
        if (!cJSON_IsArray(ecgi_list)) {
            ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [ecgi_list]");
            goto end;
        }

        ecgi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ecgi_list_local, ecgi_list) {
            if (!cJSON_IsObject(ecgi_list_local)) {
                ogs_error("OpenAPI_scheduled_area_parseFromJSON() failed [ecgi_list]");
                goto end;
            }
            OpenAPI_ecgi_t *ecgi_listItem = OpenAPI_ecgi_parseFromJSON(ecgi_list_local);
            if (!ecgi_listItem) {
                ogs_error("No ecgi_listItem");
                goto end;
            }
            OpenAPI_list_add(ecgi_listList, ecgi_listItem);
        }
    }

    scheduled_area_local_var = OpenAPI_scheduled_area_create (
        tai_list ? tai_listList : NULL,
        enter_time && !cJSON_IsNull(enter_time) ? ogs_strdup(enter_time->valuestring) : NULL,
        leave_time && !cJSON_IsNull(leave_time) ? ogs_strdup(leave_time->valuestring) : NULL,
        altitude_low ? true : false,
        altitude_low ? altitude_low->valuedouble : 0,
        altitude_high ? true : false,
        altitude_high ? altitude_high->valuedouble : 0,
        ran_node_id_list ? ran_node_id_listList : NULL,
        ncgi_list ? ncgi_listList : NULL,
        ecgi_list ? ecgi_listList : NULL
    );

    return scheduled_area_local_var;
end:
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (ran_node_id_listList) {
        OpenAPI_list_for_each(ran_node_id_listList, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(ran_node_id_listList);
        ran_node_id_listList = NULL;
    }
    if (ncgi_listList) {
        OpenAPI_list_for_each(ncgi_listList, node) {
            OpenAPI_ncgi_free(node->data);
        }
        OpenAPI_list_free(ncgi_listList);
        ncgi_listList = NULL;
    }
    if (ecgi_listList) {
        OpenAPI_list_for_each(ecgi_listList, node) {
            OpenAPI_ecgi_free(node->data);
        }
        OpenAPI_list_free(ecgi_listList);
        ecgi_listList = NULL;
    }
    return NULL;
}

OpenAPI_scheduled_area_t *OpenAPI_scheduled_area_copy(OpenAPI_scheduled_area_t *dst, OpenAPI_scheduled_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scheduled_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scheduled_area_convertToJSON() failed");
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

    OpenAPI_scheduled_area_free(dst);
    dst = OpenAPI_scheduled_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

