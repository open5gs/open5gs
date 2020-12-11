
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wireline_service_area_restriction.h"

OpenAPI_wireline_service_area_restriction_t *OpenAPI_wireline_service_area_restriction_create(
    OpenAPI_restriction_type_e restriction_type,
    OpenAPI_list_t *areas
    )
{
    OpenAPI_wireline_service_area_restriction_t *wireline_service_area_restriction_local_var = OpenAPI_malloc(sizeof(OpenAPI_wireline_service_area_restriction_t));
    if (!wireline_service_area_restriction_local_var) {
        return NULL;
    }
    wireline_service_area_restriction_local_var->restriction_type = restriction_type;
    wireline_service_area_restriction_local_var->areas = areas;

    return wireline_service_area_restriction_local_var;
}

void OpenAPI_wireline_service_area_restriction_free(OpenAPI_wireline_service_area_restriction_t *wireline_service_area_restriction)
{
    if (NULL == wireline_service_area_restriction) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(wireline_service_area_restriction->areas, node) {
        OpenAPI_wireline_area_free(node->data);
    }
    OpenAPI_list_free(wireline_service_area_restriction->areas);
    ogs_free(wireline_service_area_restriction);
}

cJSON *OpenAPI_wireline_service_area_restriction_convertToJSON(OpenAPI_wireline_service_area_restriction_t *wireline_service_area_restriction)
{
    cJSON *item = NULL;

    if (wireline_service_area_restriction == NULL) {
        ogs_error("OpenAPI_wireline_service_area_restriction_convertToJSON() failed [WirelineServiceAreaRestriction]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (wireline_service_area_restriction->restriction_type) {
        if (cJSON_AddStringToObject(item, "restrictionType", OpenAPI_restriction_type_ToString(wireline_service_area_restriction->restriction_type)) == NULL) {
            ogs_error("OpenAPI_wireline_service_area_restriction_convertToJSON() failed [restriction_type]");
            goto end;
        }
    }

    if (wireline_service_area_restriction->areas) {
        cJSON *areasList = cJSON_AddArrayToObject(item, "areas");
        if (areasList == NULL) {
            ogs_error("OpenAPI_wireline_service_area_restriction_convertToJSON() failed [areas]");
            goto end;
        }

        OpenAPI_lnode_t *areas_node;
        if (wireline_service_area_restriction->areas) {
            OpenAPI_list_for_each(wireline_service_area_restriction->areas, areas_node) {
                cJSON *itemLocal = OpenAPI_wireline_area_convertToJSON(areas_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_wireline_service_area_restriction_convertToJSON() failed [areas]");
                    goto end;
                }
                cJSON_AddItemToArray(areasList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_wireline_service_area_restriction_t *OpenAPI_wireline_service_area_restriction_parseFromJSON(cJSON *wireline_service_area_restrictionJSON)
{
    OpenAPI_wireline_service_area_restriction_t *wireline_service_area_restriction_local_var = NULL;
    cJSON *restriction_type = cJSON_GetObjectItemCaseSensitive(wireline_service_area_restrictionJSON, "restrictionType");

    OpenAPI_restriction_type_e restriction_typeVariable;
    if (restriction_type) {
        if (!cJSON_IsString(restriction_type)) {
            ogs_error("OpenAPI_wireline_service_area_restriction_parseFromJSON() failed [restriction_type]");
            goto end;
        }
        restriction_typeVariable = OpenAPI_restriction_type_FromString(restriction_type->valuestring);
    }

    cJSON *areas = cJSON_GetObjectItemCaseSensitive(wireline_service_area_restrictionJSON, "areas");

    OpenAPI_list_t *areasList;
    if (areas) {
        cJSON *areas_local_nonprimitive;
        if (!cJSON_IsArray(areas)) {
            ogs_error("OpenAPI_wireline_service_area_restriction_parseFromJSON() failed [areas]");
            goto end;
        }

        areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(areas_local_nonprimitive, areas ) {
            if (!cJSON_IsObject(areas_local_nonprimitive)) {
                ogs_error("OpenAPI_wireline_service_area_restriction_parseFromJSON() failed [areas]");
                goto end;
            }
            OpenAPI_wireline_area_t *areasItem = OpenAPI_wireline_area_parseFromJSON(areas_local_nonprimitive);

            OpenAPI_list_add(areasList, areasItem);
        }
    }

    wireline_service_area_restriction_local_var = OpenAPI_wireline_service_area_restriction_create (
        restriction_type ? restriction_typeVariable : 0,
        areas ? areasList : NULL
        );

    return wireline_service_area_restriction_local_var;
end:
    return NULL;
}

OpenAPI_wireline_service_area_restriction_t *OpenAPI_wireline_service_area_restriction_copy(OpenAPI_wireline_service_area_restriction_t *dst, OpenAPI_wireline_service_area_restriction_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wireline_service_area_restriction_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wireline_service_area_restriction_convertToJSON() failed");
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

    OpenAPI_wireline_service_area_restriction_free(dst);
    dst = OpenAPI_wireline_service_area_restriction_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

