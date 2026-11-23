
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_geo_distribution_info.h"

OpenAPI_charging_geo_distribution_info_t *OpenAPI_charging_geo_distribution_info_create(
    OpenAPI_charging_user_location_t *loc,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis
)
{
    OpenAPI_charging_geo_distribution_info_t *charging_geo_distribution_info_local_var = ogs_malloc(sizeof(OpenAPI_charging_geo_distribution_info_t));
    ogs_assert(charging_geo_distribution_info_local_var);

    charging_geo_distribution_info_local_var->loc = loc;
    charging_geo_distribution_info_local_var->supis = supis;
    charging_geo_distribution_info_local_var->gpsis = gpsis;

    return charging_geo_distribution_info_local_var;
}

void OpenAPI_charging_geo_distribution_info_free(OpenAPI_charging_geo_distribution_info_t *charging_geo_distribution_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_geo_distribution_info) {
        return;
    }
    if (charging_geo_distribution_info->loc) {
        OpenAPI_charging_user_location_free(charging_geo_distribution_info->loc);
        charging_geo_distribution_info->loc = NULL;
    }
    if (charging_geo_distribution_info->supis) {
        OpenAPI_list_for_each(charging_geo_distribution_info->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(charging_geo_distribution_info->supis);
        charging_geo_distribution_info->supis = NULL;
    }
    if (charging_geo_distribution_info->gpsis) {
        OpenAPI_list_for_each(charging_geo_distribution_info->gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(charging_geo_distribution_info->gpsis);
        charging_geo_distribution_info->gpsis = NULL;
    }
    ogs_free(charging_geo_distribution_info);
}

cJSON *OpenAPI_charging_geo_distribution_info_convertToJSON(OpenAPI_charging_geo_distribution_info_t *charging_geo_distribution_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_geo_distribution_info == NULL) {
        ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed [ChargingGeoDistributionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!charging_geo_distribution_info->loc) {
        ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed [loc]");
        return NULL;
    }
    cJSON *loc_local_JSON = OpenAPI_charging_user_location_convertToJSON(charging_geo_distribution_info->loc);
    if (loc_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed [loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "loc", loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed [loc]");
        goto end;
    }

    if (charging_geo_distribution_info->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(charging_geo_distribution_info->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (charging_geo_distribution_info->gpsis) {
    cJSON *gpsisList = cJSON_AddArrayToObject(item, "gpsis");
    if (gpsisList == NULL) {
        ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed [gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(charging_geo_distribution_info->gpsis, node) {
        if (cJSON_AddStringToObject(gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed [gpsis]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_charging_geo_distribution_info_t *OpenAPI_charging_geo_distribution_info_parseFromJSON(cJSON *charging_geo_distribution_infoJSON)
{
    OpenAPI_charging_geo_distribution_info_t *charging_geo_distribution_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *loc = NULL;
    OpenAPI_charging_user_location_t *loc_local_nonprim = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *gpsis = NULL;
    OpenAPI_list_t *gpsisList = NULL;
    loc = cJSON_GetObjectItemCaseSensitive(charging_geo_distribution_infoJSON, "loc");
    if (!loc) {
        ogs_error("OpenAPI_charging_geo_distribution_info_parseFromJSON() failed [loc]");
        goto end;
    }
    loc_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(loc);
    if (!loc_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [loc]");
        goto end;
    }

    supis = cJSON_GetObjectItemCaseSensitive(charging_geo_distribution_infoJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_charging_geo_distribution_info_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_charging_geo_distribution_info_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    gpsis = cJSON_GetObjectItemCaseSensitive(charging_geo_distribution_infoJSON, "gpsis");
    if (gpsis) {
        cJSON *gpsis_local = NULL;
        if (!cJSON_IsArray(gpsis)) {
            ogs_error("OpenAPI_charging_geo_distribution_info_parseFromJSON() failed [gpsis]");
            goto end;
        }

        gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsis_local, gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsis_local)) {
                ogs_error("OpenAPI_charging_geo_distribution_info_parseFromJSON() failed [gpsis]");
                goto end;
            }
            OpenAPI_list_add(gpsisList, ogs_strdup(gpsis_local->valuestring));
        }
    }

    charging_geo_distribution_info_local_var = OpenAPI_charging_geo_distribution_info_create (
        loc_local_nonprim,
        supis ? supisList : NULL,
        gpsis ? gpsisList : NULL
    );

    return charging_geo_distribution_info_local_var;
end:
    if (loc_local_nonprim) {
        OpenAPI_charging_user_location_free(loc_local_nonprim);
        loc_local_nonprim = NULL;
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
    return NULL;
}

OpenAPI_charging_geo_distribution_info_t *OpenAPI_charging_geo_distribution_info_copy(OpenAPI_charging_geo_distribution_info_t *dst, OpenAPI_charging_geo_distribution_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_geo_distribution_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_geo_distribution_info_convertToJSON() failed");
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

    OpenAPI_charging_geo_distribution_info_free(dst);
    dst = OpenAPI_charging_geo_distribution_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

