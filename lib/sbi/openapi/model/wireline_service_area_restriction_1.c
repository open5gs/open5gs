
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wireline_service_area_restriction_1.h"

OpenAPI_wireline_service_area_restriction_1_t *OpenAPI_wireline_service_area_restriction_1_create(
    OpenAPI_restriction_type_e restriction_type,
    OpenAPI_list_t *areas
)
{
    OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction_1_local_var = ogs_malloc(sizeof(OpenAPI_wireline_service_area_restriction_1_t));
    ogs_assert(wireline_service_area_restriction_1_local_var);

    wireline_service_area_restriction_1_local_var->restriction_type = restriction_type;
    wireline_service_area_restriction_1_local_var->areas = areas;

    return wireline_service_area_restriction_1_local_var;
}

void OpenAPI_wireline_service_area_restriction_1_free(OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == wireline_service_area_restriction_1) {
        return;
    }
    if (wireline_service_area_restriction_1->areas) {
        OpenAPI_list_for_each(wireline_service_area_restriction_1->areas, node) {
            OpenAPI_wireline_area_1_free(node->data);
        }
        OpenAPI_list_free(wireline_service_area_restriction_1->areas);
        wireline_service_area_restriction_1->areas = NULL;
    }
    ogs_free(wireline_service_area_restriction_1);
}

cJSON *OpenAPI_wireline_service_area_restriction_1_convertToJSON(OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (wireline_service_area_restriction_1 == NULL) {
        ogs_error("OpenAPI_wireline_service_area_restriction_1_convertToJSON() failed [WirelineServiceAreaRestriction_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (wireline_service_area_restriction_1->restriction_type != OpenAPI_restriction_type_NULL) {
    if (cJSON_AddStringToObject(item, "restrictionType", OpenAPI_restriction_type_ToString(wireline_service_area_restriction_1->restriction_type)) == NULL) {
        ogs_error("OpenAPI_wireline_service_area_restriction_1_convertToJSON() failed [restriction_type]");
        goto end;
    }
    }

    if (wireline_service_area_restriction_1->areas) {
    cJSON *areasList = cJSON_AddArrayToObject(item, "areas");
    if (areasList == NULL) {
        ogs_error("OpenAPI_wireline_service_area_restriction_1_convertToJSON() failed [areas]");
        goto end;
    }
    OpenAPI_list_for_each(wireline_service_area_restriction_1->areas, node) {
        cJSON *itemLocal = OpenAPI_wireline_area_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_wireline_service_area_restriction_1_convertToJSON() failed [areas]");
            goto end;
        }
        cJSON_AddItemToArray(areasList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_wireline_service_area_restriction_1_t *OpenAPI_wireline_service_area_restriction_1_parseFromJSON(cJSON *wireline_service_area_restriction_1JSON)
{
    OpenAPI_wireline_service_area_restriction_1_t *wireline_service_area_restriction_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *restriction_type = NULL;
    OpenAPI_restriction_type_e restriction_typeVariable = 0;
    cJSON *areas = NULL;
    OpenAPI_list_t *areasList = NULL;
    restriction_type = cJSON_GetObjectItemCaseSensitive(wireline_service_area_restriction_1JSON, "restrictionType");
    if (restriction_type) {
    if (!cJSON_IsString(restriction_type)) {
        ogs_error("OpenAPI_wireline_service_area_restriction_1_parseFromJSON() failed [restriction_type]");
        goto end;
    }
    restriction_typeVariable = OpenAPI_restriction_type_FromString(restriction_type->valuestring);
    }

    areas = cJSON_GetObjectItemCaseSensitive(wireline_service_area_restriction_1JSON, "areas");
    if (areas) {
        cJSON *areas_local = NULL;
        if (!cJSON_IsArray(areas)) {
            ogs_error("OpenAPI_wireline_service_area_restriction_1_parseFromJSON() failed [areas]");
            goto end;
        }

        areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(areas_local, areas) {
            if (!cJSON_IsObject(areas_local)) {
                ogs_error("OpenAPI_wireline_service_area_restriction_1_parseFromJSON() failed [areas]");
                goto end;
            }
            OpenAPI_wireline_area_1_t *areasItem = OpenAPI_wireline_area_1_parseFromJSON(areas_local);
            if (!areasItem) {
                ogs_error("No areasItem");
                goto end;
            }
            OpenAPI_list_add(areasList, areasItem);
        }
    }

    wireline_service_area_restriction_1_local_var = OpenAPI_wireline_service_area_restriction_1_create (
        restriction_type ? restriction_typeVariable : 0,
        areas ? areasList : NULL
    );

    return wireline_service_area_restriction_1_local_var;
end:
    if (areasList) {
        OpenAPI_list_for_each(areasList, node) {
            OpenAPI_wireline_area_1_free(node->data);
        }
        OpenAPI_list_free(areasList);
        areasList = NULL;
    }
    return NULL;
}

OpenAPI_wireline_service_area_restriction_1_t *OpenAPI_wireline_service_area_restriction_1_copy(OpenAPI_wireline_service_area_restriction_1_t *dst, OpenAPI_wireline_service_area_restriction_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wireline_service_area_restriction_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wireline_service_area_restriction_1_convertToJSON() failed");
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

    OpenAPI_wireline_service_area_restriction_1_free(dst);
    dst = OpenAPI_wireline_service_area_restriction_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

