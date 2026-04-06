
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trajectory_segment.h"

OpenAPI_trajectory_segment_t *OpenAPI_trajectory_segment_create(
    OpenAPI_list_t *scheduled_area_list
)
{
    OpenAPI_trajectory_segment_t *trajectory_segment_local_var = ogs_malloc(sizeof(OpenAPI_trajectory_segment_t));
    ogs_assert(trajectory_segment_local_var);

    trajectory_segment_local_var->scheduled_area_list = scheduled_area_list;

    return trajectory_segment_local_var;
}

void OpenAPI_trajectory_segment_free(OpenAPI_trajectory_segment_t *trajectory_segment)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == trajectory_segment) {
        return;
    }
    if (trajectory_segment->scheduled_area_list) {
        OpenAPI_list_for_each(trajectory_segment->scheduled_area_list, node) {
            OpenAPI_scheduled_area_free(node->data);
        }
        OpenAPI_list_free(trajectory_segment->scheduled_area_list);
        trajectory_segment->scheduled_area_list = NULL;
    }
    ogs_free(trajectory_segment);
}

cJSON *OpenAPI_trajectory_segment_convertToJSON(OpenAPI_trajectory_segment_t *trajectory_segment)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (trajectory_segment == NULL) {
        ogs_error("OpenAPI_trajectory_segment_convertToJSON() failed [TrajectorySegment]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!trajectory_segment->scheduled_area_list) {
        ogs_error("OpenAPI_trajectory_segment_convertToJSON() failed [scheduled_area_list]");
        return NULL;
    }
    cJSON *scheduled_area_listList = cJSON_AddArrayToObject(item, "scheduledAreaList");
    if (scheduled_area_listList == NULL) {
        ogs_error("OpenAPI_trajectory_segment_convertToJSON() failed [scheduled_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(trajectory_segment->scheduled_area_list, node) {
        cJSON *itemLocal = OpenAPI_scheduled_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_trajectory_segment_convertToJSON() failed [scheduled_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(scheduled_area_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_trajectory_segment_t *OpenAPI_trajectory_segment_parseFromJSON(cJSON *trajectory_segmentJSON)
{
    OpenAPI_trajectory_segment_t *trajectory_segment_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *scheduled_area_list = NULL;
    OpenAPI_list_t *scheduled_area_listList = NULL;
    scheduled_area_list = cJSON_GetObjectItemCaseSensitive(trajectory_segmentJSON, "scheduledAreaList");
    if (!scheduled_area_list) {
        ogs_error("OpenAPI_trajectory_segment_parseFromJSON() failed [scheduled_area_list]");
        goto end;
    }
        cJSON *scheduled_area_list_local = NULL;
        if (!cJSON_IsArray(scheduled_area_list)) {
            ogs_error("OpenAPI_trajectory_segment_parseFromJSON() failed [scheduled_area_list]");
            goto end;
        }

        scheduled_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(scheduled_area_list_local, scheduled_area_list) {
            if (!cJSON_IsObject(scheduled_area_list_local)) {
                ogs_error("OpenAPI_trajectory_segment_parseFromJSON() failed [scheduled_area_list]");
                goto end;
            }
            OpenAPI_scheduled_area_t *scheduled_area_listItem = OpenAPI_scheduled_area_parseFromJSON(scheduled_area_list_local);
            if (!scheduled_area_listItem) {
                ogs_error("No scheduled_area_listItem");
                goto end;
            }
            OpenAPI_list_add(scheduled_area_listList, scheduled_area_listItem);
        }

    trajectory_segment_local_var = OpenAPI_trajectory_segment_create (
        scheduled_area_listList
    );

    return trajectory_segment_local_var;
end:
    if (scheduled_area_listList) {
        OpenAPI_list_for_each(scheduled_area_listList, node) {
            OpenAPI_scheduled_area_free(node->data);
        }
        OpenAPI_list_free(scheduled_area_listList);
        scheduled_area_listList = NULL;
    }
    return NULL;
}

OpenAPI_trajectory_segment_t *OpenAPI_trajectory_segment_copy(OpenAPI_trajectory_segment_t *dst, OpenAPI_trajectory_segment_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trajectory_segment_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trajectory_segment_convertToJSON() failed");
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

    OpenAPI_trajectory_segment_free(dst);
    dst = OpenAPI_trajectory_segment_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

