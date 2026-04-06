
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_info.h"

OpenAPI_location_info_t *OpenAPI_location_info_create(
    OpenAPI_user_location_t *loc,
    OpenAPI_geographical_area_t *geo_loc,
    bool is_ratio,
    int ratio,
    bool is_confidence,
    int confidence,
    OpenAPI_list_t *geo_distr_infos,
    bool is_dist_threshold,
    int dist_threshold
)
{
    OpenAPI_location_info_t *location_info_local_var = ogs_malloc(sizeof(OpenAPI_location_info_t));
    ogs_assert(location_info_local_var);

    location_info_local_var->loc = loc;
    location_info_local_var->geo_loc = geo_loc;
    location_info_local_var->is_ratio = is_ratio;
    location_info_local_var->ratio = ratio;
    location_info_local_var->is_confidence = is_confidence;
    location_info_local_var->confidence = confidence;
    location_info_local_var->geo_distr_infos = geo_distr_infos;
    location_info_local_var->is_dist_threshold = is_dist_threshold;
    location_info_local_var->dist_threshold = dist_threshold;

    return location_info_local_var;
}

void OpenAPI_location_info_free(OpenAPI_location_info_t *location_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_info) {
        return;
    }
    if (location_info->loc) {
        OpenAPI_user_location_free(location_info->loc);
        location_info->loc = NULL;
    }
    if (location_info->geo_loc) {
        OpenAPI_geographical_area_free(location_info->geo_loc);
        location_info->geo_loc = NULL;
    }
    if (location_info->geo_distr_infos) {
        OpenAPI_list_for_each(location_info->geo_distr_infos, node) {
            OpenAPI_geo_distribution_info_free(node->data);
        }
        OpenAPI_list_free(location_info->geo_distr_infos);
        location_info->geo_distr_infos = NULL;
    }
    ogs_free(location_info);
}

cJSON *OpenAPI_location_info_convertToJSON(OpenAPI_location_info_t *location_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (location_info == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [LocationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!location_info->loc) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [loc]");
        return NULL;
    }
    cJSON *loc_local_JSON = OpenAPI_user_location_convertToJSON(location_info->loc);
    if (loc_local_JSON == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "loc", loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [loc]");
        goto end;
    }

    if (location_info->geo_loc) {
    cJSON *geo_loc_local_JSON = OpenAPI_geographical_area_convertToJSON(location_info->geo_loc);
    if (geo_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [geo_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "geoLoc", geo_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [geo_loc]");
        goto end;
    }
    }

    if (location_info->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", location_info->ratio) == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [ratio]");
        goto end;
    }
    }

    if (location_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", location_info->confidence) == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

    if (location_info->geo_distr_infos) {
    cJSON *geo_distr_infosList = cJSON_AddArrayToObject(item, "geoDistrInfos");
    if (geo_distr_infosList == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [geo_distr_infos]");
        goto end;
    }
    OpenAPI_list_for_each(location_info->geo_distr_infos, node) {
        cJSON *itemLocal = OpenAPI_geo_distribution_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_location_info_convertToJSON() failed [geo_distr_infos]");
            goto end;
        }
        cJSON_AddItemToArray(geo_distr_infosList, itemLocal);
    }
    }

    if (location_info->is_dist_threshold) {
    if (cJSON_AddNumberToObject(item, "distThreshold", location_info->dist_threshold) == NULL) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed [dist_threshold]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_location_info_t *OpenAPI_location_info_parseFromJSON(cJSON *location_infoJSON)
{
    OpenAPI_location_info_t *location_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *loc = NULL;
    OpenAPI_user_location_t *loc_local_nonprim = NULL;
    cJSON *geo_loc = NULL;
    OpenAPI_geographical_area_t *geo_loc_local_nonprim = NULL;
    cJSON *ratio = NULL;
    cJSON *confidence = NULL;
    cJSON *geo_distr_infos = NULL;
    OpenAPI_list_t *geo_distr_infosList = NULL;
    cJSON *dist_threshold = NULL;
    loc = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "loc");
    if (!loc) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [loc]");
        goto end;
    }
    loc_local_nonprim = OpenAPI_user_location_parseFromJSON(loc);
    if (!loc_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [loc]");
        goto end;
    }

    geo_loc = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "geoLoc");
    if (geo_loc) {
    geo_loc_local_nonprim = OpenAPI_geographical_area_parseFromJSON(geo_loc);
    if (!geo_loc_local_nonprim) {
        ogs_error("OpenAPI_geographical_area_parseFromJSON failed [geo_loc]");
        goto end;
    }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    geo_distr_infos = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "geoDistrInfos");
    if (geo_distr_infos) {
        cJSON *geo_distr_infos_local = NULL;
        if (!cJSON_IsArray(geo_distr_infos)) {
            ogs_error("OpenAPI_location_info_parseFromJSON() failed [geo_distr_infos]");
            goto end;
        }

        geo_distr_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(geo_distr_infos_local, geo_distr_infos) {
            if (!cJSON_IsObject(geo_distr_infos_local)) {
                ogs_error("OpenAPI_location_info_parseFromJSON() failed [geo_distr_infos]");
                goto end;
            }
            OpenAPI_geo_distribution_info_t *geo_distr_infosItem = OpenAPI_geo_distribution_info_parseFromJSON(geo_distr_infos_local);
            if (!geo_distr_infosItem) {
                ogs_error("No geo_distr_infosItem");
                goto end;
            }
            OpenAPI_list_add(geo_distr_infosList, geo_distr_infosItem);
        }
    }

    dist_threshold = cJSON_GetObjectItemCaseSensitive(location_infoJSON, "distThreshold");
    if (dist_threshold) {
    if (!cJSON_IsNumber(dist_threshold)) {
        ogs_error("OpenAPI_location_info_parseFromJSON() failed [dist_threshold]");
        goto end;
    }
    }

    location_info_local_var = OpenAPI_location_info_create (
        loc_local_nonprim,
        geo_loc ? geo_loc_local_nonprim : NULL,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0,
        geo_distr_infos ? geo_distr_infosList : NULL,
        dist_threshold ? true : false,
        dist_threshold ? dist_threshold->valuedouble : 0
    );

    return location_info_local_var;
end:
    if (loc_local_nonprim) {
        OpenAPI_user_location_free(loc_local_nonprim);
        loc_local_nonprim = NULL;
    }
    if (geo_loc_local_nonprim) {
        OpenAPI_geographical_area_free(geo_loc_local_nonprim);
        geo_loc_local_nonprim = NULL;
    }
    if (geo_distr_infosList) {
        OpenAPI_list_for_each(geo_distr_infosList, node) {
            OpenAPI_geo_distribution_info_free(node->data);
        }
        OpenAPI_list_free(geo_distr_infosList);
        geo_distr_infosList = NULL;
    }
    return NULL;
}

OpenAPI_location_info_t *OpenAPI_location_info_copy(OpenAPI_location_info_t *dst, OpenAPI_location_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_info_convertToJSON() failed");
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

    OpenAPI_location_info_free(dst);
    dst = OpenAPI_location_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

