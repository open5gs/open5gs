
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_trajectory.h"

OpenAPI_ue_trajectory_t *OpenAPI_ue_trajectory_create(
    char *supi,
    char *gpsi,
    OpenAPI_list_t *timestamped_locs
)
{
    OpenAPI_ue_trajectory_t *ue_trajectory_local_var = ogs_malloc(sizeof(OpenAPI_ue_trajectory_t));
    ogs_assert(ue_trajectory_local_var);

    ue_trajectory_local_var->supi = supi;
    ue_trajectory_local_var->gpsi = gpsi;
    ue_trajectory_local_var->timestamped_locs = timestamped_locs;

    return ue_trajectory_local_var;
}

void OpenAPI_ue_trajectory_free(OpenAPI_ue_trajectory_t *ue_trajectory)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_trajectory) {
        return;
    }
    if (ue_trajectory->supi) {
        ogs_free(ue_trajectory->supi);
        ue_trajectory->supi = NULL;
    }
    if (ue_trajectory->gpsi) {
        ogs_free(ue_trajectory->gpsi);
        ue_trajectory->gpsi = NULL;
    }
    if (ue_trajectory->timestamped_locs) {
        OpenAPI_list_for_each(ue_trajectory->timestamped_locs, node) {
            OpenAPI_timestamped_location_free(node->data);
        }
        OpenAPI_list_free(ue_trajectory->timestamped_locs);
        ue_trajectory->timestamped_locs = NULL;
    }
    ogs_free(ue_trajectory);
}

cJSON *OpenAPI_ue_trajectory_convertToJSON(OpenAPI_ue_trajectory_t *ue_trajectory)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_trajectory == NULL) {
        ogs_error("OpenAPI_ue_trajectory_convertToJSON() failed [UeTrajectory]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_trajectory->supi) {
    if (cJSON_AddStringToObject(item, "supi", ue_trajectory->supi) == NULL) {
        ogs_error("OpenAPI_ue_trajectory_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (ue_trajectory->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", ue_trajectory->gpsi) == NULL) {
        ogs_error("OpenAPI_ue_trajectory_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (!ue_trajectory->timestamped_locs) {
        ogs_error("OpenAPI_ue_trajectory_convertToJSON() failed [timestamped_locs]");
        return NULL;
    }
    cJSON *timestamped_locsList = cJSON_AddArrayToObject(item, "timestampedLocs");
    if (timestamped_locsList == NULL) {
        ogs_error("OpenAPI_ue_trajectory_convertToJSON() failed [timestamped_locs]");
        goto end;
    }
    OpenAPI_list_for_each(ue_trajectory->timestamped_locs, node) {
        cJSON *itemLocal = OpenAPI_timestamped_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_trajectory_convertToJSON() failed [timestamped_locs]");
            goto end;
        }
        cJSON_AddItemToArray(timestamped_locsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_ue_trajectory_t *OpenAPI_ue_trajectory_parseFromJSON(cJSON *ue_trajectoryJSON)
{
    OpenAPI_ue_trajectory_t *ue_trajectory_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *timestamped_locs = NULL;
    OpenAPI_list_t *timestamped_locsList = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(ue_trajectoryJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_ue_trajectory_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(ue_trajectoryJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_ue_trajectory_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    timestamped_locs = cJSON_GetObjectItemCaseSensitive(ue_trajectoryJSON, "timestampedLocs");
    if (!timestamped_locs) {
        ogs_error("OpenAPI_ue_trajectory_parseFromJSON() failed [timestamped_locs]");
        goto end;
    }
        cJSON *timestamped_locs_local = NULL;
        if (!cJSON_IsArray(timestamped_locs)) {
            ogs_error("OpenAPI_ue_trajectory_parseFromJSON() failed [timestamped_locs]");
            goto end;
        }

        timestamped_locsList = OpenAPI_list_create();

        cJSON_ArrayForEach(timestamped_locs_local, timestamped_locs) {
            if (!cJSON_IsObject(timestamped_locs_local)) {
                ogs_error("OpenAPI_ue_trajectory_parseFromJSON() failed [timestamped_locs]");
                goto end;
            }
            OpenAPI_timestamped_location_t *timestamped_locsItem = OpenAPI_timestamped_location_parseFromJSON(timestamped_locs_local);
            if (!timestamped_locsItem) {
                ogs_error("No timestamped_locsItem");
                goto end;
            }
            OpenAPI_list_add(timestamped_locsList, timestamped_locsItem);
        }

    ue_trajectory_local_var = OpenAPI_ue_trajectory_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        timestamped_locsList
    );

    return ue_trajectory_local_var;
end:
    if (timestamped_locsList) {
        OpenAPI_list_for_each(timestamped_locsList, node) {
            OpenAPI_timestamped_location_free(node->data);
        }
        OpenAPI_list_free(timestamped_locsList);
        timestamped_locsList = NULL;
    }
    return NULL;
}

OpenAPI_ue_trajectory_t *OpenAPI_ue_trajectory_copy(OpenAPI_ue_trajectory_t *dst, OpenAPI_ue_trajectory_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_trajectory_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_trajectory_convertToJSON() failed");
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

    OpenAPI_ue_trajectory_free(dst);
    dst = OpenAPI_ue_trajectory_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

