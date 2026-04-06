
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mov_behav_info.h"

OpenAPI_mov_behav_info_t *OpenAPI_mov_behav_info_create(
    OpenAPI_geographical_coordinates_t *geo_loc,
    bool is_altitude,
    double altitude,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *mov_behavs,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_mov_behav_info_t *mov_behav_info_local_var = ogs_malloc(sizeof(OpenAPI_mov_behav_info_t));
    ogs_assert(mov_behav_info_local_var);

    mov_behav_info_local_var->geo_loc = geo_loc;
    mov_behav_info_local_var->is_altitude = is_altitude;
    mov_behav_info_local_var->altitude = altitude;
    mov_behav_info_local_var->supis = supis;
    mov_behav_info_local_var->gpsis = gpsis;
    mov_behav_info_local_var->mov_behavs = mov_behavs;
    mov_behav_info_local_var->is_confidence = is_confidence;
    mov_behav_info_local_var->confidence = confidence;

    return mov_behav_info_local_var;
}

void OpenAPI_mov_behav_info_free(OpenAPI_mov_behav_info_t *mov_behav_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mov_behav_info) {
        return;
    }
    if (mov_behav_info->geo_loc) {
        OpenAPI_geographical_coordinates_free(mov_behav_info->geo_loc);
        mov_behav_info->geo_loc = NULL;
    }
    if (mov_behav_info->supis) {
        OpenAPI_list_for_each(mov_behav_info->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mov_behav_info->supis);
        mov_behav_info->supis = NULL;
    }
    if (mov_behav_info->gpsis) {
        OpenAPI_list_for_each(mov_behav_info->gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mov_behav_info->gpsis);
        mov_behav_info->gpsis = NULL;
    }
    if (mov_behav_info->mov_behavs) {
        OpenAPI_list_for_each(mov_behav_info->mov_behavs, node) {
            OpenAPI_mov_behav_free(node->data);
        }
        OpenAPI_list_free(mov_behav_info->mov_behavs);
        mov_behav_info->mov_behavs = NULL;
    }
    ogs_free(mov_behav_info);
}

cJSON *OpenAPI_mov_behav_info_convertToJSON(OpenAPI_mov_behav_info_t *mov_behav_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mov_behav_info == NULL) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [MovBehavInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mov_behav_info->geo_loc) {
    cJSON *geo_loc_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(mov_behav_info->geo_loc);
    if (geo_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [geo_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "geoLoc", geo_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [geo_loc]");
        goto end;
    }
    }

    if (mov_behav_info->is_altitude) {
    if (cJSON_AddNumberToObject(item, "altitude", mov_behav_info->altitude) == NULL) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [altitude]");
        goto end;
    }
    }

    if (mov_behav_info->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(mov_behav_info->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (mov_behav_info->gpsis) {
    cJSON *gpsisList = cJSON_AddArrayToObject(item, "gpsis");
    if (gpsisList == NULL) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(mov_behav_info->gpsis, node) {
        if (cJSON_AddStringToObject(gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [gpsis]");
            goto end;
        }
    }
    }

    if (mov_behav_info->mov_behavs) {
    cJSON *mov_behavsList = cJSON_AddArrayToObject(item, "movBehavs");
    if (mov_behavsList == NULL) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [mov_behavs]");
        goto end;
    }
    OpenAPI_list_for_each(mov_behav_info->mov_behavs, node) {
        cJSON *itemLocal = OpenAPI_mov_behav_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [mov_behavs]");
            goto end;
        }
        cJSON_AddItemToArray(mov_behavsList, itemLocal);
    }
    }

    if (mov_behav_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", mov_behav_info->confidence) == NULL) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mov_behav_info_t *OpenAPI_mov_behav_info_parseFromJSON(cJSON *mov_behav_infoJSON)
{
    OpenAPI_mov_behav_info_t *mov_behav_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *geo_loc = NULL;
    OpenAPI_geographical_coordinates_t *geo_loc_local_nonprim = NULL;
    cJSON *altitude = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *gpsis = NULL;
    OpenAPI_list_t *gpsisList = NULL;
    cJSON *mov_behavs = NULL;
    OpenAPI_list_t *mov_behavsList = NULL;
    cJSON *confidence = NULL;
    geo_loc = cJSON_GetObjectItemCaseSensitive(mov_behav_infoJSON, "geoLoc");
    if (geo_loc) {
    geo_loc_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(geo_loc);
    if (!geo_loc_local_nonprim) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON failed [geo_loc]");
        goto end;
    }
    }

    altitude = cJSON_GetObjectItemCaseSensitive(mov_behav_infoJSON, "altitude");
    if (altitude) {
    if (!cJSON_IsNumber(altitude)) {
        ogs_error("OpenAPI_mov_behav_info_parseFromJSON() failed [altitude]");
        goto end;
    }
    }

    supis = cJSON_GetObjectItemCaseSensitive(mov_behav_infoJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_mov_behav_info_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_mov_behav_info_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    gpsis = cJSON_GetObjectItemCaseSensitive(mov_behav_infoJSON, "gpsis");
    if (gpsis) {
        cJSON *gpsis_local = NULL;
        if (!cJSON_IsArray(gpsis)) {
            ogs_error("OpenAPI_mov_behav_info_parseFromJSON() failed [gpsis]");
            goto end;
        }

        gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsis_local, gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsis_local)) {
                ogs_error("OpenAPI_mov_behav_info_parseFromJSON() failed [gpsis]");
                goto end;
            }
            OpenAPI_list_add(gpsisList, ogs_strdup(gpsis_local->valuestring));
        }
    }

    mov_behavs = cJSON_GetObjectItemCaseSensitive(mov_behav_infoJSON, "movBehavs");
    if (mov_behavs) {
        cJSON *mov_behavs_local = NULL;
        if (!cJSON_IsArray(mov_behavs)) {
            ogs_error("OpenAPI_mov_behav_info_parseFromJSON() failed [mov_behavs]");
            goto end;
        }

        mov_behavsList = OpenAPI_list_create();

        cJSON_ArrayForEach(mov_behavs_local, mov_behavs) {
            if (!cJSON_IsObject(mov_behavs_local)) {
                ogs_error("OpenAPI_mov_behav_info_parseFromJSON() failed [mov_behavs]");
                goto end;
            }
            OpenAPI_mov_behav_t *mov_behavsItem = OpenAPI_mov_behav_parseFromJSON(mov_behavs_local);
            if (!mov_behavsItem) {
                ogs_error("No mov_behavsItem");
                goto end;
            }
            OpenAPI_list_add(mov_behavsList, mov_behavsItem);
        }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(mov_behav_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_mov_behav_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    mov_behav_info_local_var = OpenAPI_mov_behav_info_create (
        geo_loc ? geo_loc_local_nonprim : NULL,
        altitude ? true : false,
        altitude ? altitude->valuedouble : 0,
        supis ? supisList : NULL,
        gpsis ? gpsisList : NULL,
        mov_behavs ? mov_behavsList : NULL,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return mov_behav_info_local_var;
end:
    if (geo_loc_local_nonprim) {
        OpenAPI_geographical_coordinates_free(geo_loc_local_nonprim);
        geo_loc_local_nonprim = NULL;
    }
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (gpsisList) {
        OpenAPI_list_for_each(gpsisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsisList);
        gpsisList = NULL;
    }
    if (mov_behavsList) {
        OpenAPI_list_for_each(mov_behavsList, node) {
            OpenAPI_mov_behav_free(node->data);
        }
        OpenAPI_list_free(mov_behavsList);
        mov_behavsList = NULL;
    }
    return NULL;
}

OpenAPI_mov_behav_info_t *OpenAPI_mov_behav_info_copy(OpenAPI_mov_behav_info_t *dst, OpenAPI_mov_behav_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mov_behav_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mov_behav_info_convertToJSON() failed");
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

    OpenAPI_mov_behav_info_free(dst);
    dst = OpenAPI_mov_behav_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

