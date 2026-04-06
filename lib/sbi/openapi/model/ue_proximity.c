
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_proximity.h"

OpenAPI_ue_proximity_t *OpenAPI_ue_proximity_create(
    bool is_ue_distance,
    int ue_distance,
    OpenAPI_velocity_estimate_t *ue_velocity,
    bool is_avr_speed,
    float avr_speed,
    OpenAPI_location_orientation_e loc_orientation,
    OpenAPI_list_t *ue_trajectories,
    bool is_ratio,
    int ratio,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_ue_proximity_t *ue_proximity_local_var = ogs_malloc(sizeof(OpenAPI_ue_proximity_t));
    ogs_assert(ue_proximity_local_var);

    ue_proximity_local_var->is_ue_distance = is_ue_distance;
    ue_proximity_local_var->ue_distance = ue_distance;
    ue_proximity_local_var->ue_velocity = ue_velocity;
    ue_proximity_local_var->is_avr_speed = is_avr_speed;
    ue_proximity_local_var->avr_speed = avr_speed;
    ue_proximity_local_var->loc_orientation = loc_orientation;
    ue_proximity_local_var->ue_trajectories = ue_trajectories;
    ue_proximity_local_var->is_ratio = is_ratio;
    ue_proximity_local_var->ratio = ratio;
    ue_proximity_local_var->is_confidence = is_confidence;
    ue_proximity_local_var->confidence = confidence;

    return ue_proximity_local_var;
}

void OpenAPI_ue_proximity_free(OpenAPI_ue_proximity_t *ue_proximity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_proximity) {
        return;
    }
    if (ue_proximity->ue_velocity) {
        OpenAPI_velocity_estimate_free(ue_proximity->ue_velocity);
        ue_proximity->ue_velocity = NULL;
    }
    if (ue_proximity->ue_trajectories) {
        OpenAPI_list_for_each(ue_proximity->ue_trajectories, node) {
            OpenAPI_ue_trajectory_free(node->data);
        }
        OpenAPI_list_free(ue_proximity->ue_trajectories);
        ue_proximity->ue_trajectories = NULL;
    }
    ogs_free(ue_proximity);
}

cJSON *OpenAPI_ue_proximity_convertToJSON(OpenAPI_ue_proximity_t *ue_proximity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_proximity == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [UeProximity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_proximity->is_ue_distance) {
    if (cJSON_AddNumberToObject(item, "ueDistance", ue_proximity->ue_distance) == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [ue_distance]");
        goto end;
    }
    }

    if (ue_proximity->ue_velocity) {
    cJSON *ue_velocity_local_JSON = OpenAPI_velocity_estimate_convertToJSON(ue_proximity->ue_velocity);
    if (ue_velocity_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [ue_velocity]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueVelocity", ue_velocity_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [ue_velocity]");
        goto end;
    }
    }

    if (ue_proximity->is_avr_speed) {
    if (cJSON_AddNumberToObject(item, "avrSpeed", ue_proximity->avr_speed) == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [avr_speed]");
        goto end;
    }
    }

    if (ue_proximity->loc_orientation != OpenAPI_location_orientation_NULL) {
    if (cJSON_AddStringToObject(item, "locOrientation", OpenAPI_location_orientation_ToString(ue_proximity->loc_orientation)) == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [loc_orientation]");
        goto end;
    }
    }

    if (ue_proximity->ue_trajectories) {
    cJSON *ue_trajectoriesList = cJSON_AddArrayToObject(item, "ueTrajectories");
    if (ue_trajectoriesList == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [ue_trajectories]");
        goto end;
    }
    OpenAPI_list_for_each(ue_proximity->ue_trajectories, node) {
        cJSON *itemLocal = OpenAPI_ue_trajectory_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [ue_trajectories]");
            goto end;
        }
        cJSON_AddItemToArray(ue_trajectoriesList, itemLocal);
    }
    }

    if (ue_proximity->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", ue_proximity->ratio) == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [ratio]");
        goto end;
    }
    }

    if (ue_proximity->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", ue_proximity->confidence) == NULL) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_proximity_t *OpenAPI_ue_proximity_parseFromJSON(cJSON *ue_proximityJSON)
{
    OpenAPI_ue_proximity_t *ue_proximity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_distance = NULL;
    cJSON *ue_velocity = NULL;
    OpenAPI_velocity_estimate_t *ue_velocity_local_nonprim = NULL;
    cJSON *avr_speed = NULL;
    cJSON *loc_orientation = NULL;
    OpenAPI_location_orientation_e loc_orientationVariable = 0;
    cJSON *ue_trajectories = NULL;
    OpenAPI_list_t *ue_trajectoriesList = NULL;
    cJSON *ratio = NULL;
    cJSON *confidence = NULL;
    ue_distance = cJSON_GetObjectItemCaseSensitive(ue_proximityJSON, "ueDistance");
    if (ue_distance) {
    if (!cJSON_IsNumber(ue_distance)) {
        ogs_error("OpenAPI_ue_proximity_parseFromJSON() failed [ue_distance]");
        goto end;
    }
    }

    ue_velocity = cJSON_GetObjectItemCaseSensitive(ue_proximityJSON, "ueVelocity");
    if (ue_velocity) {
    ue_velocity_local_nonprim = OpenAPI_velocity_estimate_parseFromJSON(ue_velocity);
    if (!ue_velocity_local_nonprim) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON failed [ue_velocity]");
        goto end;
    }
    }

    avr_speed = cJSON_GetObjectItemCaseSensitive(ue_proximityJSON, "avrSpeed");
    if (avr_speed) {
    if (!cJSON_IsNumber(avr_speed)) {
        ogs_error("OpenAPI_ue_proximity_parseFromJSON() failed [avr_speed]");
        goto end;
    }
    }

    loc_orientation = cJSON_GetObjectItemCaseSensitive(ue_proximityJSON, "locOrientation");
    if (loc_orientation) {
    if (!cJSON_IsString(loc_orientation)) {
        ogs_error("OpenAPI_ue_proximity_parseFromJSON() failed [loc_orientation]");
        goto end;
    }
    loc_orientationVariable = OpenAPI_location_orientation_FromString(loc_orientation->valuestring);
    }

    ue_trajectories = cJSON_GetObjectItemCaseSensitive(ue_proximityJSON, "ueTrajectories");
    if (ue_trajectories) {
        cJSON *ue_trajectories_local = NULL;
        if (!cJSON_IsArray(ue_trajectories)) {
            ogs_error("OpenAPI_ue_proximity_parseFromJSON() failed [ue_trajectories]");
            goto end;
        }

        ue_trajectoriesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_trajectories_local, ue_trajectories) {
            if (!cJSON_IsObject(ue_trajectories_local)) {
                ogs_error("OpenAPI_ue_proximity_parseFromJSON() failed [ue_trajectories]");
                goto end;
            }
            OpenAPI_ue_trajectory_t *ue_trajectoriesItem = OpenAPI_ue_trajectory_parseFromJSON(ue_trajectories_local);
            if (!ue_trajectoriesItem) {
                ogs_error("No ue_trajectoriesItem");
                goto end;
            }
            OpenAPI_list_add(ue_trajectoriesList, ue_trajectoriesItem);
        }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(ue_proximityJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_ue_proximity_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(ue_proximityJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_ue_proximity_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    ue_proximity_local_var = OpenAPI_ue_proximity_create (
        ue_distance ? true : false,
        ue_distance ? ue_distance->valuedouble : 0,
        ue_velocity ? ue_velocity_local_nonprim : NULL,
        avr_speed ? true : false,
        avr_speed ? avr_speed->valuedouble : 0,
        loc_orientation ? loc_orientationVariable : 0,
        ue_trajectories ? ue_trajectoriesList : NULL,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return ue_proximity_local_var;
end:
    if (ue_velocity_local_nonprim) {
        OpenAPI_velocity_estimate_free(ue_velocity_local_nonprim);
        ue_velocity_local_nonprim = NULL;
    }
    if (ue_trajectoriesList) {
        OpenAPI_list_for_each(ue_trajectoriesList, node) {
            OpenAPI_ue_trajectory_free(node->data);
        }
        OpenAPI_list_free(ue_trajectoriesList);
        ue_trajectoriesList = NULL;
    }
    return NULL;
}

OpenAPI_ue_proximity_t *OpenAPI_ue_proximity_copy(OpenAPI_ue_proximity_t *dst, OpenAPI_ue_proximity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_proximity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_proximity_convertToJSON() failed");
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

    OpenAPI_ue_proximity_free(dst);
    dst = OpenAPI_ue_proximity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

