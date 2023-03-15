
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "expected_ue_behavior.h"

OpenAPI_expected_ue_behavior_t *OpenAPI_expected_ue_behavior_create(
    OpenAPI_list_t *exp_move_trajectory,
    char *validity_time
)
{
    OpenAPI_expected_ue_behavior_t *expected_ue_behavior_local_var = ogs_malloc(sizeof(OpenAPI_expected_ue_behavior_t));
    ogs_assert(expected_ue_behavior_local_var);

    expected_ue_behavior_local_var->exp_move_trajectory = exp_move_trajectory;
    expected_ue_behavior_local_var->validity_time = validity_time;

    return expected_ue_behavior_local_var;
}

void OpenAPI_expected_ue_behavior_free(OpenAPI_expected_ue_behavior_t *expected_ue_behavior)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == expected_ue_behavior) {
        return;
    }
    if (expected_ue_behavior->exp_move_trajectory) {
        OpenAPI_list_for_each(expected_ue_behavior->exp_move_trajectory, node) {
            OpenAPI_user_location_free(node->data);
        }
        OpenAPI_list_free(expected_ue_behavior->exp_move_trajectory);
        expected_ue_behavior->exp_move_trajectory = NULL;
    }
    if (expected_ue_behavior->validity_time) {
        ogs_free(expected_ue_behavior->validity_time);
        expected_ue_behavior->validity_time = NULL;
    }
    ogs_free(expected_ue_behavior);
}

cJSON *OpenAPI_expected_ue_behavior_convertToJSON(OpenAPI_expected_ue_behavior_t *expected_ue_behavior)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (expected_ue_behavior == NULL) {
        ogs_error("OpenAPI_expected_ue_behavior_convertToJSON() failed [ExpectedUeBehavior]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!expected_ue_behavior->exp_move_trajectory) {
        ogs_error("OpenAPI_expected_ue_behavior_convertToJSON() failed [exp_move_trajectory]");
        return NULL;
    }
    cJSON *exp_move_trajectoryList = cJSON_AddArrayToObject(item, "expMoveTrajectory");
    if (exp_move_trajectoryList == NULL) {
        ogs_error("OpenAPI_expected_ue_behavior_convertToJSON() failed [exp_move_trajectory]");
        goto end;
    }
    OpenAPI_list_for_each(expected_ue_behavior->exp_move_trajectory, node) {
        cJSON *itemLocal = OpenAPI_user_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_expected_ue_behavior_convertToJSON() failed [exp_move_trajectory]");
            goto end;
        }
        cJSON_AddItemToArray(exp_move_trajectoryList, itemLocal);
    }

    if (!expected_ue_behavior->validity_time) {
        ogs_error("OpenAPI_expected_ue_behavior_convertToJSON() failed [validity_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "validityTime", expected_ue_behavior->validity_time) == NULL) {
        ogs_error("OpenAPI_expected_ue_behavior_convertToJSON() failed [validity_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_expected_ue_behavior_t *OpenAPI_expected_ue_behavior_parseFromJSON(cJSON *expected_ue_behaviorJSON)
{
    OpenAPI_expected_ue_behavior_t *expected_ue_behavior_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *exp_move_trajectory = NULL;
    OpenAPI_list_t *exp_move_trajectoryList = NULL;
    cJSON *validity_time = NULL;
    exp_move_trajectory = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviorJSON, "expMoveTrajectory");
    if (!exp_move_trajectory) {
        ogs_error("OpenAPI_expected_ue_behavior_parseFromJSON() failed [exp_move_trajectory]");
        goto end;
    }
        cJSON *exp_move_trajectory_local = NULL;
        if (!cJSON_IsArray(exp_move_trajectory)) {
            ogs_error("OpenAPI_expected_ue_behavior_parseFromJSON() failed [exp_move_trajectory]");
            goto end;
        }

        exp_move_trajectoryList = OpenAPI_list_create();

        cJSON_ArrayForEach(exp_move_trajectory_local, exp_move_trajectory) {
            if (!cJSON_IsObject(exp_move_trajectory_local)) {
                ogs_error("OpenAPI_expected_ue_behavior_parseFromJSON() failed [exp_move_trajectory]");
                goto end;
            }
            OpenAPI_user_location_t *exp_move_trajectoryItem = OpenAPI_user_location_parseFromJSON(exp_move_trajectory_local);
            if (!exp_move_trajectoryItem) {
                ogs_error("No exp_move_trajectoryItem");
                goto end;
            }
            OpenAPI_list_add(exp_move_trajectoryList, exp_move_trajectoryItem);
        }

    validity_time = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviorJSON, "validityTime");
    if (!validity_time) {
        ogs_error("OpenAPI_expected_ue_behavior_parseFromJSON() failed [validity_time]");
        goto end;
    }
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_expected_ue_behavior_parseFromJSON() failed [validity_time]");
        goto end;
    }

    expected_ue_behavior_local_var = OpenAPI_expected_ue_behavior_create (
        exp_move_trajectoryList,
        ogs_strdup(validity_time->valuestring)
    );

    return expected_ue_behavior_local_var;
end:
    if (exp_move_trajectoryList) {
        OpenAPI_list_for_each(exp_move_trajectoryList, node) {
            OpenAPI_user_location_free(node->data);
        }
        OpenAPI_list_free(exp_move_trajectoryList);
        exp_move_trajectoryList = NULL;
    }
    return NULL;
}

OpenAPI_expected_ue_behavior_t *OpenAPI_expected_ue_behavior_copy(OpenAPI_expected_ue_behavior_t *dst, OpenAPI_expected_ue_behavior_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_expected_ue_behavior_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_expected_ue_behavior_convertToJSON() failed");
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

    OpenAPI_expected_ue_behavior_free(dst);
    dst = OpenAPI_expected_ue_behavior_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

