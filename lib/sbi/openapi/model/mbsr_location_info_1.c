
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbsr_location_info_1.h"

OpenAPI_mbsr_location_info_1_t *OpenAPI_mbsr_location_info_1_create(
    OpenAPI_list_t *mbsr_location,
    OpenAPI_list_t *mbsr_location_areas
)
{
    OpenAPI_mbsr_location_info_1_t *mbsr_location_info_1_local_var = ogs_malloc(sizeof(OpenAPI_mbsr_location_info_1_t));
    ogs_assert(mbsr_location_info_1_local_var);

    mbsr_location_info_1_local_var->mbsr_location = mbsr_location;
    mbsr_location_info_1_local_var->mbsr_location_areas = mbsr_location_areas;

    return mbsr_location_info_1_local_var;
}

void OpenAPI_mbsr_location_info_1_free(OpenAPI_mbsr_location_info_1_t *mbsr_location_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbsr_location_info_1) {
        return;
    }
    if (mbsr_location_info_1->mbsr_location) {
        OpenAPI_list_for_each(mbsr_location_info_1->mbsr_location, node) {
            OpenAPI_tai_1_free(node->data);
        }
        OpenAPI_list_free(mbsr_location_info_1->mbsr_location);
        mbsr_location_info_1->mbsr_location = NULL;
    }
    if (mbsr_location_info_1->mbsr_location_areas) {
        OpenAPI_list_for_each(mbsr_location_info_1->mbsr_location_areas, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mbsr_location_info_1->mbsr_location_areas);
        mbsr_location_info_1->mbsr_location_areas = NULL;
    }
    ogs_free(mbsr_location_info_1);
}

cJSON *OpenAPI_mbsr_location_info_1_convertToJSON(OpenAPI_mbsr_location_info_1_t *mbsr_location_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbsr_location_info_1 == NULL) {
        ogs_error("OpenAPI_mbsr_location_info_1_convertToJSON() failed [MbsrLocationInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbsr_location_info_1->mbsr_location) {
    cJSON *mbsr_locationList = cJSON_AddArrayToObject(item, "mbsrLocation");
    if (mbsr_locationList == NULL) {
        ogs_error("OpenAPI_mbsr_location_info_1_convertToJSON() failed [mbsr_location]");
        goto end;
    }
    OpenAPI_list_for_each(mbsr_location_info_1->mbsr_location, node) {
        cJSON *itemLocal = OpenAPI_tai_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mbsr_location_info_1_convertToJSON() failed [mbsr_location]");
            goto end;
        }
        cJSON_AddItemToArray(mbsr_locationList, itemLocal);
    }
    }

    if (mbsr_location_info_1->mbsr_location_areas) {
    cJSON *mbsr_location_areasList = cJSON_AddArrayToObject(item, "mbsrLocationAreas");
    if (mbsr_location_areasList == NULL) {
        ogs_error("OpenAPI_mbsr_location_info_1_convertToJSON() failed [mbsr_location_areas]");
        goto end;
    }
    OpenAPI_list_for_each(mbsr_location_info_1->mbsr_location_areas, node) {
        if (cJSON_AddStringToObject(mbsr_location_areasList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mbsr_location_info_1_convertToJSON() failed [mbsr_location_areas]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_mbsr_location_info_1_t *OpenAPI_mbsr_location_info_1_parseFromJSON(cJSON *mbsr_location_info_1JSON)
{
    OpenAPI_mbsr_location_info_1_t *mbsr_location_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbsr_location = NULL;
    OpenAPI_list_t *mbsr_locationList = NULL;
    cJSON *mbsr_location_areas = NULL;
    OpenAPI_list_t *mbsr_location_areasList = NULL;
    mbsr_location = cJSON_GetObjectItemCaseSensitive(mbsr_location_info_1JSON, "mbsrLocation");
    if (mbsr_location) {
        cJSON *mbsr_location_local = NULL;
        if (!cJSON_IsArray(mbsr_location)) {
            ogs_error("OpenAPI_mbsr_location_info_1_parseFromJSON() failed [mbsr_location]");
            goto end;
        }

        mbsr_locationList = OpenAPI_list_create();

        cJSON_ArrayForEach(mbsr_location_local, mbsr_location) {
            if (!cJSON_IsObject(mbsr_location_local)) {
                ogs_error("OpenAPI_mbsr_location_info_1_parseFromJSON() failed [mbsr_location]");
                goto end;
            }
            OpenAPI_tai_1_t *mbsr_locationItem = OpenAPI_tai_1_parseFromJSON(mbsr_location_local);
            if (!mbsr_locationItem) {
                ogs_error("No mbsr_locationItem");
                goto end;
            }
            OpenAPI_list_add(mbsr_locationList, mbsr_locationItem);
        }
    }

    mbsr_location_areas = cJSON_GetObjectItemCaseSensitive(mbsr_location_info_1JSON, "mbsrLocationAreas");
    if (mbsr_location_areas) {
        cJSON *mbsr_location_areas_local = NULL;
        if (!cJSON_IsArray(mbsr_location_areas)) {
            ogs_error("OpenAPI_mbsr_location_info_1_parseFromJSON() failed [mbsr_location_areas]");
            goto end;
        }

        mbsr_location_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(mbsr_location_areas_local, mbsr_location_areas) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(mbsr_location_areas_local)) {
                ogs_error("OpenAPI_mbsr_location_info_1_parseFromJSON() failed [mbsr_location_areas]");
                goto end;
            }
            OpenAPI_list_add(mbsr_location_areasList, ogs_strdup(mbsr_location_areas_local->valuestring));
        }
    }

    mbsr_location_info_1_local_var = OpenAPI_mbsr_location_info_1_create (
        mbsr_location ? mbsr_locationList : NULL,
        mbsr_location_areas ? mbsr_location_areasList : NULL
    );

    return mbsr_location_info_1_local_var;
end:
    if (mbsr_locationList) {
        OpenAPI_list_for_each(mbsr_locationList, node) {
            OpenAPI_tai_1_free(node->data);
        }
        OpenAPI_list_free(mbsr_locationList);
        mbsr_locationList = NULL;
    }
    if (mbsr_location_areasList) {
        OpenAPI_list_for_each(mbsr_location_areasList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mbsr_location_areasList);
        mbsr_location_areasList = NULL;
    }
    return NULL;
}

OpenAPI_mbsr_location_info_1_t *OpenAPI_mbsr_location_info_1_copy(OpenAPI_mbsr_location_info_1_t *dst, OpenAPI_mbsr_location_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbsr_location_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbsr_location_info_1_convertToJSON() failed");
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

    OpenAPI_mbsr_location_info_1_free(dst);
    dst = OpenAPI_mbsr_location_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

