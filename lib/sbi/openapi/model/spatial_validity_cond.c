
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "spatial_validity_cond.h"

OpenAPI_spatial_validity_cond_t *OpenAPI_spatial_validity_cond_create(
    OpenAPI_list_t *tracking_area_list,
    OpenAPI_list_t *countries,
    OpenAPI_geo_service_area_t *geographical_service_area
)
{
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond_local_var = ogs_malloc(sizeof(OpenAPI_spatial_validity_cond_t));
    ogs_assert(spatial_validity_cond_local_var);

    spatial_validity_cond_local_var->tracking_area_list = tracking_area_list;
    spatial_validity_cond_local_var->countries = countries;
    spatial_validity_cond_local_var->geographical_service_area = geographical_service_area;

    return spatial_validity_cond_local_var;
}

void OpenAPI_spatial_validity_cond_free(OpenAPI_spatial_validity_cond_t *spatial_validity_cond)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == spatial_validity_cond) {
        return;
    }
    if (spatial_validity_cond->tracking_area_list) {
        OpenAPI_list_for_each(spatial_validity_cond->tracking_area_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(spatial_validity_cond->tracking_area_list);
        spatial_validity_cond->tracking_area_list = NULL;
    }
    if (spatial_validity_cond->countries) {
        OpenAPI_list_for_each(spatial_validity_cond->countries, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(spatial_validity_cond->countries);
        spatial_validity_cond->countries = NULL;
    }
    if (spatial_validity_cond->geographical_service_area) {
        OpenAPI_geo_service_area_free(spatial_validity_cond->geographical_service_area);
        spatial_validity_cond->geographical_service_area = NULL;
    }
    ogs_free(spatial_validity_cond);
}

cJSON *OpenAPI_spatial_validity_cond_convertToJSON(OpenAPI_spatial_validity_cond_t *spatial_validity_cond)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (spatial_validity_cond == NULL) {
        ogs_error("OpenAPI_spatial_validity_cond_convertToJSON() failed [SpatialValidityCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (spatial_validity_cond->tracking_area_list) {
    cJSON *tracking_area_listList = cJSON_AddArrayToObject(item, "trackingAreaList");
    if (tracking_area_listList == NULL) {
        ogs_error("OpenAPI_spatial_validity_cond_convertToJSON() failed [tracking_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(spatial_validity_cond->tracking_area_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_spatial_validity_cond_convertToJSON() failed [tracking_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(tracking_area_listList, itemLocal);
    }
    }

    if (spatial_validity_cond->countries) {
    cJSON *countriesList = cJSON_AddArrayToObject(item, "countries");
    if (countriesList == NULL) {
        ogs_error("OpenAPI_spatial_validity_cond_convertToJSON() failed [countries]");
        goto end;
    }
    OpenAPI_list_for_each(spatial_validity_cond->countries, node) {
        if (cJSON_AddStringToObject(countriesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_spatial_validity_cond_convertToJSON() failed [countries]");
            goto end;
        }
    }
    }

    if (spatial_validity_cond->geographical_service_area) {
    cJSON *geographical_service_area_local_JSON = OpenAPI_geo_service_area_convertToJSON(spatial_validity_cond->geographical_service_area);
    if (geographical_service_area_local_JSON == NULL) {
        ogs_error("OpenAPI_spatial_validity_cond_convertToJSON() failed [geographical_service_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "geographicalServiceArea", geographical_service_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_spatial_validity_cond_convertToJSON() failed [geographical_service_area]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_spatial_validity_cond_t *OpenAPI_spatial_validity_cond_parseFromJSON(cJSON *spatial_validity_condJSON)
{
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tracking_area_list = NULL;
    OpenAPI_list_t *tracking_area_listList = NULL;
    cJSON *countries = NULL;
    OpenAPI_list_t *countriesList = NULL;
    cJSON *geographical_service_area = NULL;
    OpenAPI_geo_service_area_t *geographical_service_area_local_nonprim = NULL;
    tracking_area_list = cJSON_GetObjectItemCaseSensitive(spatial_validity_condJSON, "trackingAreaList");
    if (tracking_area_list) {
        cJSON *tracking_area_list_local = NULL;
        if (!cJSON_IsArray(tracking_area_list)) {
            ogs_error("OpenAPI_spatial_validity_cond_parseFromJSON() failed [tracking_area_list]");
            goto end;
        }

        tracking_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tracking_area_list_local, tracking_area_list) {
            if (!cJSON_IsObject(tracking_area_list_local)) {
                ogs_error("OpenAPI_spatial_validity_cond_parseFromJSON() failed [tracking_area_list]");
                goto end;
            }
            OpenAPI_tai_t *tracking_area_listItem = OpenAPI_tai_parseFromJSON(tracking_area_list_local);
            if (!tracking_area_listItem) {
                ogs_error("No tracking_area_listItem");
                goto end;
            }
            OpenAPI_list_add(tracking_area_listList, tracking_area_listItem);
        }
    }

    countries = cJSON_GetObjectItemCaseSensitive(spatial_validity_condJSON, "countries");
    if (countries) {
        cJSON *countries_local = NULL;
        if (!cJSON_IsArray(countries)) {
            ogs_error("OpenAPI_spatial_validity_cond_parseFromJSON() failed [countries]");
            goto end;
        }

        countriesList = OpenAPI_list_create();

        cJSON_ArrayForEach(countries_local, countries) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(countries_local)) {
                ogs_error("OpenAPI_spatial_validity_cond_parseFromJSON() failed [countries]");
                goto end;
            }
            OpenAPI_list_add(countriesList, ogs_strdup(countries_local->valuestring));
        }
    }

    geographical_service_area = cJSON_GetObjectItemCaseSensitive(spatial_validity_condJSON, "geographicalServiceArea");
    if (geographical_service_area) {
    geographical_service_area_local_nonprim = OpenAPI_geo_service_area_parseFromJSON(geographical_service_area);
    if (!geographical_service_area_local_nonprim) {
        ogs_error("OpenAPI_geo_service_area_parseFromJSON failed [geographical_service_area]");
        goto end;
    }
    }

    spatial_validity_cond_local_var = OpenAPI_spatial_validity_cond_create (
        tracking_area_list ? tracking_area_listList : NULL,
        countries ? countriesList : NULL,
        geographical_service_area ? geographical_service_area_local_nonprim : NULL
    );

    return spatial_validity_cond_local_var;
end:
    if (tracking_area_listList) {
        OpenAPI_list_for_each(tracking_area_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tracking_area_listList);
        tracking_area_listList = NULL;
    }
    if (countriesList) {
        OpenAPI_list_for_each(countriesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(countriesList);
        countriesList = NULL;
    }
    if (geographical_service_area_local_nonprim) {
        OpenAPI_geo_service_area_free(geographical_service_area_local_nonprim);
        geographical_service_area_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_spatial_validity_cond_t *OpenAPI_spatial_validity_cond_copy(OpenAPI_spatial_validity_cond_t *dst, OpenAPI_spatial_validity_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_spatial_validity_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_spatial_validity_cond_convertToJSON() failed");
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

    OpenAPI_spatial_validity_cond_free(dst);
    dst = OpenAPI_spatial_validity_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

