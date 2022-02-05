
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_area_restriction_1.h"

OpenAPI_service_area_restriction_1_t *OpenAPI_service_area_restriction_1_create(
    OpenAPI_restriction_type_e restriction_type,
    OpenAPI_list_t *areas,
    bool is_max_num_of_tas,
    int max_num_of_tas,
    bool is_max_num_of_tas_for_not_allowed_areas,
    int max_num_of_tas_for_not_allowed_areas
)
{
    OpenAPI_service_area_restriction_1_t *service_area_restriction_1_local_var = ogs_malloc(sizeof(OpenAPI_service_area_restriction_1_t));
    ogs_assert(service_area_restriction_1_local_var);

    service_area_restriction_1_local_var->restriction_type = restriction_type;
    service_area_restriction_1_local_var->areas = areas;
    service_area_restriction_1_local_var->is_max_num_of_tas = is_max_num_of_tas;
    service_area_restriction_1_local_var->max_num_of_tas = max_num_of_tas;
    service_area_restriction_1_local_var->is_max_num_of_tas_for_not_allowed_areas = is_max_num_of_tas_for_not_allowed_areas;
    service_area_restriction_1_local_var->max_num_of_tas_for_not_allowed_areas = max_num_of_tas_for_not_allowed_areas;

    return service_area_restriction_1_local_var;
}

void OpenAPI_service_area_restriction_1_free(OpenAPI_service_area_restriction_1_t *service_area_restriction_1)
{
    if (NULL == service_area_restriction_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(service_area_restriction_1->areas, node) {
        OpenAPI_area_1_free(node->data);
    }
    OpenAPI_list_free(service_area_restriction_1->areas);
    ogs_free(service_area_restriction_1);
}

cJSON *OpenAPI_service_area_restriction_1_convertToJSON(OpenAPI_service_area_restriction_1_t *service_area_restriction_1)
{
    cJSON *item = NULL;

    if (service_area_restriction_1 == NULL) {
        ogs_error("OpenAPI_service_area_restriction_1_convertToJSON() failed [ServiceAreaRestriction_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_area_restriction_1->restriction_type) {
    if (cJSON_AddStringToObject(item, "restrictionType", OpenAPI_restriction_type_ToString(service_area_restriction_1->restriction_type)) == NULL) {
        ogs_error("OpenAPI_service_area_restriction_1_convertToJSON() failed [restriction_type]");
        goto end;
    }
    }

    if (service_area_restriction_1->areas) {
    cJSON *areasList = cJSON_AddArrayToObject(item, "areas");
    if (areasList == NULL) {
        ogs_error("OpenAPI_service_area_restriction_1_convertToJSON() failed [areas]");
        goto end;
    }

    OpenAPI_lnode_t *areas_node;
    if (service_area_restriction_1->areas) {
        OpenAPI_list_for_each(service_area_restriction_1->areas, areas_node) {
            cJSON *itemLocal = OpenAPI_area_1_convertToJSON(areas_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_service_area_restriction_1_convertToJSON() failed [areas]");
                goto end;
            }
            cJSON_AddItemToArray(areasList, itemLocal);
        }
    }
    }

    if (service_area_restriction_1->is_max_num_of_tas) {
    if (cJSON_AddNumberToObject(item, "maxNumOfTAs", service_area_restriction_1->max_num_of_tas) == NULL) {
        ogs_error("OpenAPI_service_area_restriction_1_convertToJSON() failed [max_num_of_tas]");
        goto end;
    }
    }

    if (service_area_restriction_1->is_max_num_of_tas_for_not_allowed_areas) {
    if (cJSON_AddNumberToObject(item, "maxNumOfTAsForNotAllowedAreas", service_area_restriction_1->max_num_of_tas_for_not_allowed_areas) == NULL) {
        ogs_error("OpenAPI_service_area_restriction_1_convertToJSON() failed [max_num_of_tas_for_not_allowed_areas]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_service_area_restriction_1_t *OpenAPI_service_area_restriction_1_parseFromJSON(cJSON *service_area_restriction_1JSON)
{
    OpenAPI_service_area_restriction_1_t *service_area_restriction_1_local_var = NULL;
    cJSON *restriction_type = cJSON_GetObjectItemCaseSensitive(service_area_restriction_1JSON, "restrictionType");

    OpenAPI_restriction_type_e restriction_typeVariable;
    if (restriction_type) {
    if (!cJSON_IsString(restriction_type)) {
        ogs_error("OpenAPI_service_area_restriction_1_parseFromJSON() failed [restriction_type]");
        goto end;
    }
    restriction_typeVariable = OpenAPI_restriction_type_FromString(restriction_type->valuestring);
    }

    cJSON *areas = cJSON_GetObjectItemCaseSensitive(service_area_restriction_1JSON, "areas");

    OpenAPI_list_t *areasList;
    if (areas) {
    cJSON *areas_local_nonprimitive;
    if (!cJSON_IsArray(areas)){
        ogs_error("OpenAPI_service_area_restriction_1_parseFromJSON() failed [areas]");
        goto end;
    }

    areasList = OpenAPI_list_create();

    cJSON_ArrayForEach(areas_local_nonprimitive, areas ) {
        if (!cJSON_IsObject(areas_local_nonprimitive)) {
            ogs_error("OpenAPI_service_area_restriction_1_parseFromJSON() failed [areas]");
            goto end;
        }
        OpenAPI_area_1_t *areasItem = OpenAPI_area_1_parseFromJSON(areas_local_nonprimitive);

        if (!areasItem) {
            ogs_error("No areasItem");
            OpenAPI_list_free(areasList);
            goto end;
        }

        OpenAPI_list_add(areasList, areasItem);
    }
    }

    cJSON *max_num_of_tas = cJSON_GetObjectItemCaseSensitive(service_area_restriction_1JSON, "maxNumOfTAs");

    if (max_num_of_tas) {
    if (!cJSON_IsNumber(max_num_of_tas)) {
        ogs_error("OpenAPI_service_area_restriction_1_parseFromJSON() failed [max_num_of_tas]");
        goto end;
    }
    }

    cJSON *max_num_of_tas_for_not_allowed_areas = cJSON_GetObjectItemCaseSensitive(service_area_restriction_1JSON, "maxNumOfTAsForNotAllowedAreas");

    if (max_num_of_tas_for_not_allowed_areas) {
    if (!cJSON_IsNumber(max_num_of_tas_for_not_allowed_areas)) {
        ogs_error("OpenAPI_service_area_restriction_1_parseFromJSON() failed [max_num_of_tas_for_not_allowed_areas]");
        goto end;
    }
    }

    service_area_restriction_1_local_var = OpenAPI_service_area_restriction_1_create (
        restriction_type ? restriction_typeVariable : 0,
        areas ? areasList : NULL,
        max_num_of_tas ? true : false,
        max_num_of_tas ? max_num_of_tas->valuedouble : 0,
        max_num_of_tas_for_not_allowed_areas ? true : false,
        max_num_of_tas_for_not_allowed_areas ? max_num_of_tas_for_not_allowed_areas->valuedouble : 0
    );

    return service_area_restriction_1_local_var;
end:
    return NULL;
}

OpenAPI_service_area_restriction_1_t *OpenAPI_service_area_restriction_1_copy(OpenAPI_service_area_restriction_1_t *dst, OpenAPI_service_area_restriction_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_area_restriction_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_area_restriction_1_convertToJSON() failed");
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

    OpenAPI_service_area_restriction_1_free(dst);
    dst = OpenAPI_service_area_restriction_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

