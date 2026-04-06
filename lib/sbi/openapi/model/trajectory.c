
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trajectory.h"

OpenAPI_trajectory_t *OpenAPI_trajectory_create(
    OpenAPI_list_t *scheduled_area_list,
    OpenAPI_list_t *segment_list,
    bool is_acceptable_deviation_time,
    int acceptable_deviation_time
)
{
    OpenAPI_trajectory_t *trajectory_local_var = ogs_malloc(sizeof(OpenAPI_trajectory_t));
    ogs_assert(trajectory_local_var);

    trajectory_local_var->scheduled_area_list = scheduled_area_list;
    trajectory_local_var->segment_list = segment_list;
    trajectory_local_var->is_acceptable_deviation_time = is_acceptable_deviation_time;
    trajectory_local_var->acceptable_deviation_time = acceptable_deviation_time;

    return trajectory_local_var;
}

void OpenAPI_trajectory_free(OpenAPI_trajectory_t *trajectory)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == trajectory) {
        return;
    }
    if (trajectory->scheduled_area_list) {
        OpenAPI_list_for_each(trajectory->scheduled_area_list, node) {
            OpenAPI_scheduled_area_free(node->data);
        }
        OpenAPI_list_free(trajectory->scheduled_area_list);
        trajectory->scheduled_area_list = NULL;
    }
    if (trajectory->segment_list) {
        OpenAPI_list_for_each(trajectory->segment_list, node) {
            OpenAPI_trajectory_segment_free(node->data);
        }
        OpenAPI_list_free(trajectory->segment_list);
        trajectory->segment_list = NULL;
    }
    ogs_free(trajectory);
}

cJSON *OpenAPI_trajectory_convertToJSON(OpenAPI_trajectory_t *trajectory)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (trajectory == NULL) {
        ogs_error("OpenAPI_trajectory_convertToJSON() failed [Trajectory]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (trajectory->scheduled_area_list) {
    cJSON *scheduled_area_listList = cJSON_AddArrayToObject(item, "scheduledAreaList");
    if (scheduled_area_listList == NULL) {
        ogs_error("OpenAPI_trajectory_convertToJSON() failed [scheduled_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(trajectory->scheduled_area_list, node) {
        cJSON *itemLocal = OpenAPI_scheduled_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_trajectory_convertToJSON() failed [scheduled_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(scheduled_area_listList, itemLocal);
    }
    }

    if (trajectory->segment_list) {
    cJSON *segment_listList = cJSON_AddArrayToObject(item, "segmentList");
    if (segment_listList == NULL) {
        ogs_error("OpenAPI_trajectory_convertToJSON() failed [segment_list]");
        goto end;
    }
    OpenAPI_list_for_each(trajectory->segment_list, node) {
        cJSON *itemLocal = OpenAPI_trajectory_segment_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_trajectory_convertToJSON() failed [segment_list]");
            goto end;
        }
        cJSON_AddItemToArray(segment_listList, itemLocal);
    }
    }

    if (trajectory->is_acceptable_deviation_time) {
    if (cJSON_AddNumberToObject(item, "acceptableDeviationTime", trajectory->acceptable_deviation_time) == NULL) {
        ogs_error("OpenAPI_trajectory_convertToJSON() failed [acceptable_deviation_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_trajectory_t *OpenAPI_trajectory_parseFromJSON(cJSON *trajectoryJSON)
{
    OpenAPI_trajectory_t *trajectory_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *scheduled_area_list = NULL;
    OpenAPI_list_t *scheduled_area_listList = NULL;
    cJSON *segment_list = NULL;
    OpenAPI_list_t *segment_listList = NULL;
    cJSON *acceptable_deviation_time = NULL;
    scheduled_area_list = cJSON_GetObjectItemCaseSensitive(trajectoryJSON, "scheduledAreaList");
    if (scheduled_area_list) {
        cJSON *scheduled_area_list_local = NULL;
        if (!cJSON_IsArray(scheduled_area_list)) {
            ogs_error("OpenAPI_trajectory_parseFromJSON() failed [scheduled_area_list]");
            goto end;
        }

        scheduled_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(scheduled_area_list_local, scheduled_area_list) {
            if (!cJSON_IsObject(scheduled_area_list_local)) {
                ogs_error("OpenAPI_trajectory_parseFromJSON() failed [scheduled_area_list]");
                goto end;
            }
            OpenAPI_scheduled_area_t *scheduled_area_listItem = OpenAPI_scheduled_area_parseFromJSON(scheduled_area_list_local);
            if (!scheduled_area_listItem) {
                ogs_error("No scheduled_area_listItem");
                goto end;
            }
            OpenAPI_list_add(scheduled_area_listList, scheduled_area_listItem);
        }
    }

    segment_list = cJSON_GetObjectItemCaseSensitive(trajectoryJSON, "segmentList");
    if (segment_list) {
        cJSON *segment_list_local = NULL;
        if (!cJSON_IsArray(segment_list)) {
            ogs_error("OpenAPI_trajectory_parseFromJSON() failed [segment_list]");
            goto end;
        }

        segment_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(segment_list_local, segment_list) {
            if (!cJSON_IsObject(segment_list_local)) {
                ogs_error("OpenAPI_trajectory_parseFromJSON() failed [segment_list]");
                goto end;
            }
            OpenAPI_trajectory_segment_t *segment_listItem = OpenAPI_trajectory_segment_parseFromJSON(segment_list_local);
            if (!segment_listItem) {
                ogs_error("No segment_listItem");
                goto end;
            }
            OpenAPI_list_add(segment_listList, segment_listItem);
        }
    }

    acceptable_deviation_time = cJSON_GetObjectItemCaseSensitive(trajectoryJSON, "acceptableDeviationTime");
    if (acceptable_deviation_time) {
    if (!cJSON_IsNumber(acceptable_deviation_time)) {
        ogs_error("OpenAPI_trajectory_parseFromJSON() failed [acceptable_deviation_time]");
        goto end;
    }
    }

    trajectory_local_var = OpenAPI_trajectory_create (
        scheduled_area_list ? scheduled_area_listList : NULL,
        segment_list ? segment_listList : NULL,
        acceptable_deviation_time ? true : false,
        acceptable_deviation_time ? acceptable_deviation_time->valuedouble : 0
    );

    return trajectory_local_var;
end:
    if (scheduled_area_listList) {
        OpenAPI_list_for_each(scheduled_area_listList, node) {
            OpenAPI_scheduled_area_free(node->data);
        }
        OpenAPI_list_free(scheduled_area_listList);
        scheduled_area_listList = NULL;
    }
    if (segment_listList) {
        OpenAPI_list_for_each(segment_listList, node) {
            OpenAPI_trajectory_segment_free(node->data);
        }
        OpenAPI_list_free(segment_listList);
        segment_listList = NULL;
    }
    return NULL;
}

OpenAPI_trajectory_t *OpenAPI_trajectory_copy(OpenAPI_trajectory_t *dst, OpenAPI_trajectory_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trajectory_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trajectory_convertToJSON() failed");
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

    OpenAPI_trajectory_free(dst);
    dst = OpenAPI_trajectory_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

