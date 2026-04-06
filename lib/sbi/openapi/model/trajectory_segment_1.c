
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trajectory_segment_1.h"

OpenAPI_trajectory_segment_1_t *OpenAPI_trajectory_segment_1_create(
    OpenAPI_list_t *expected_umts
)
{
    OpenAPI_trajectory_segment_1_t *trajectory_segment_1_local_var = ogs_malloc(sizeof(OpenAPI_trajectory_segment_1_t));
    ogs_assert(trajectory_segment_1_local_var);

    trajectory_segment_1_local_var->expected_umts = expected_umts;

    return trajectory_segment_1_local_var;
}

void OpenAPI_trajectory_segment_1_free(OpenAPI_trajectory_segment_1_t *trajectory_segment_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == trajectory_segment_1) {
        return;
    }
    if (trajectory_segment_1->expected_umts) {
        OpenAPI_list_for_each(trajectory_segment_1->expected_umts, node) {
            OpenAPI_location_area_free(node->data);
        }
        OpenAPI_list_free(trajectory_segment_1->expected_umts);
        trajectory_segment_1->expected_umts = NULL;
    }
    ogs_free(trajectory_segment_1);
}

cJSON *OpenAPI_trajectory_segment_1_convertToJSON(OpenAPI_trajectory_segment_1_t *trajectory_segment_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (trajectory_segment_1 == NULL) {
        ogs_error("OpenAPI_trajectory_segment_1_convertToJSON() failed [TrajectorySegment_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!trajectory_segment_1->expected_umts) {
        ogs_error("OpenAPI_trajectory_segment_1_convertToJSON() failed [expected_umts]");
        return NULL;
    }
    cJSON *expected_umtsList = cJSON_AddArrayToObject(item, "expectedUmts");
    if (expected_umtsList == NULL) {
        ogs_error("OpenAPI_trajectory_segment_1_convertToJSON() failed [expected_umts]");
        goto end;
    }
    OpenAPI_list_for_each(trajectory_segment_1->expected_umts, node) {
        cJSON *itemLocal = OpenAPI_location_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_trajectory_segment_1_convertToJSON() failed [expected_umts]");
            goto end;
        }
        cJSON_AddItemToArray(expected_umtsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_trajectory_segment_1_t *OpenAPI_trajectory_segment_1_parseFromJSON(cJSON *trajectory_segment_1JSON)
{
    OpenAPI_trajectory_segment_1_t *trajectory_segment_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *expected_umts = NULL;
    OpenAPI_list_t *expected_umtsList = NULL;
    expected_umts = cJSON_GetObjectItemCaseSensitive(trajectory_segment_1JSON, "expectedUmts");
    if (!expected_umts) {
        ogs_error("OpenAPI_trajectory_segment_1_parseFromJSON() failed [expected_umts]");
        goto end;
    }
        cJSON *expected_umts_local = NULL;
        if (!cJSON_IsArray(expected_umts)) {
            ogs_error("OpenAPI_trajectory_segment_1_parseFromJSON() failed [expected_umts]");
            goto end;
        }

        expected_umtsList = OpenAPI_list_create();

        cJSON_ArrayForEach(expected_umts_local, expected_umts) {
            if (!cJSON_IsObject(expected_umts_local)) {
                ogs_error("OpenAPI_trajectory_segment_1_parseFromJSON() failed [expected_umts]");
                goto end;
            }
            OpenAPI_location_area_t *expected_umtsItem = OpenAPI_location_area_parseFromJSON(expected_umts_local);
            if (!expected_umtsItem) {
                ogs_error("No expected_umtsItem");
                goto end;
            }
            OpenAPI_list_add(expected_umtsList, expected_umtsItem);
        }

    trajectory_segment_1_local_var = OpenAPI_trajectory_segment_1_create (
        expected_umtsList
    );

    return trajectory_segment_1_local_var;
end:
    if (expected_umtsList) {
        OpenAPI_list_for_each(expected_umtsList, node) {
            OpenAPI_location_area_free(node->data);
        }
        OpenAPI_list_free(expected_umtsList);
        expected_umtsList = NULL;
    }
    return NULL;
}

OpenAPI_trajectory_segment_1_t *OpenAPI_trajectory_segment_1_copy(OpenAPI_trajectory_segment_1_t *dst, OpenAPI_trajectory_segment_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trajectory_segment_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trajectory_segment_1_convertToJSON() failed");
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

    OpenAPI_trajectory_segment_1_free(dst);
    dst = OpenAPI_trajectory_segment_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

