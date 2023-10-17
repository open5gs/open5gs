
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "route_selection_parameter_set.h"

OpenAPI_route_selection_parameter_set_t *OpenAPI_route_selection_parameter_set_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    bool is_precedence,
    int precedence,
    OpenAPI_list_t *spatial_validity_areas,
    OpenAPI_list_t *spatial_validity_tais
)
{
    OpenAPI_route_selection_parameter_set_t *route_selection_parameter_set_local_var = ogs_malloc(sizeof(OpenAPI_route_selection_parameter_set_t));
    ogs_assert(route_selection_parameter_set_local_var);

    route_selection_parameter_set_local_var->dnn = dnn;
    route_selection_parameter_set_local_var->snssai = snssai;
    route_selection_parameter_set_local_var->is_precedence = is_precedence;
    route_selection_parameter_set_local_var->precedence = precedence;
    route_selection_parameter_set_local_var->spatial_validity_areas = spatial_validity_areas;
    route_selection_parameter_set_local_var->spatial_validity_tais = spatial_validity_tais;

    return route_selection_parameter_set_local_var;
}

void OpenAPI_route_selection_parameter_set_free(OpenAPI_route_selection_parameter_set_t *route_selection_parameter_set)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == route_selection_parameter_set) {
        return;
    }
    if (route_selection_parameter_set->dnn) {
        ogs_free(route_selection_parameter_set->dnn);
        route_selection_parameter_set->dnn = NULL;
    }
    if (route_selection_parameter_set->snssai) {
        OpenAPI_snssai_free(route_selection_parameter_set->snssai);
        route_selection_parameter_set->snssai = NULL;
    }
    if (route_selection_parameter_set->spatial_validity_areas) {
        OpenAPI_list_for_each(route_selection_parameter_set->spatial_validity_areas, node) {
            OpenAPI_geographical_area_free(node->data);
        }
        OpenAPI_list_free(route_selection_parameter_set->spatial_validity_areas);
        route_selection_parameter_set->spatial_validity_areas = NULL;
    }
    if (route_selection_parameter_set->spatial_validity_tais) {
        OpenAPI_list_for_each(route_selection_parameter_set->spatial_validity_tais, node) {
            OpenAPI_tai_1_free(node->data);
        }
        OpenAPI_list_free(route_selection_parameter_set->spatial_validity_tais);
        route_selection_parameter_set->spatial_validity_tais = NULL;
    }
    ogs_free(route_selection_parameter_set);
}

cJSON *OpenAPI_route_selection_parameter_set_convertToJSON(OpenAPI_route_selection_parameter_set_t *route_selection_parameter_set)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (route_selection_parameter_set == NULL) {
        ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [RouteSelectionParameterSet]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (route_selection_parameter_set->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", route_selection_parameter_set->dnn) == NULL) {
        ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (route_selection_parameter_set->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(route_selection_parameter_set->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (route_selection_parameter_set->is_precedence) {
    if (cJSON_AddNumberToObject(item, "precedence", route_selection_parameter_set->precedence) == NULL) {
        ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [precedence]");
        goto end;
    }
    }

    if (route_selection_parameter_set->spatial_validity_areas) {
    cJSON *spatial_validity_areasList = cJSON_AddArrayToObject(item, "spatialValidityAreas");
    if (spatial_validity_areasList == NULL) {
        ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [spatial_validity_areas]");
        goto end;
    }
    OpenAPI_list_for_each(route_selection_parameter_set->spatial_validity_areas, node) {
        cJSON *itemLocal = OpenAPI_geographical_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [spatial_validity_areas]");
            goto end;
        }
        cJSON_AddItemToArray(spatial_validity_areasList, itemLocal);
    }
    }

    if (route_selection_parameter_set->spatial_validity_tais) {
    cJSON *spatial_validity_taisList = cJSON_AddArrayToObject(item, "spatialValidityTais");
    if (spatial_validity_taisList == NULL) {
        ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [spatial_validity_tais]");
        goto end;
    }
    OpenAPI_list_for_each(route_selection_parameter_set->spatial_validity_tais, node) {
        cJSON *itemLocal = OpenAPI_tai_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed [spatial_validity_tais]");
            goto end;
        }
        cJSON_AddItemToArray(spatial_validity_taisList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_route_selection_parameter_set_t *OpenAPI_route_selection_parameter_set_parseFromJSON(cJSON *route_selection_parameter_setJSON)
{
    OpenAPI_route_selection_parameter_set_t *route_selection_parameter_set_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *precedence = NULL;
    cJSON *spatial_validity_areas = NULL;
    OpenAPI_list_t *spatial_validity_areasList = NULL;
    cJSON *spatial_validity_tais = NULL;
    OpenAPI_list_t *spatial_validity_taisList = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(route_selection_parameter_setJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_route_selection_parameter_set_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(route_selection_parameter_setJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    precedence = cJSON_GetObjectItemCaseSensitive(route_selection_parameter_setJSON, "precedence");
    if (precedence) {
    if (!cJSON_IsNumber(precedence)) {
        ogs_error("OpenAPI_route_selection_parameter_set_parseFromJSON() failed [precedence]");
        goto end;
    }
    }

    spatial_validity_areas = cJSON_GetObjectItemCaseSensitive(route_selection_parameter_setJSON, "spatialValidityAreas");
    if (spatial_validity_areas) {
        cJSON *spatial_validity_areas_local = NULL;
        if (!cJSON_IsArray(spatial_validity_areas)) {
            ogs_error("OpenAPI_route_selection_parameter_set_parseFromJSON() failed [spatial_validity_areas]");
            goto end;
        }

        spatial_validity_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(spatial_validity_areas_local, spatial_validity_areas) {
            if (!cJSON_IsObject(spatial_validity_areas_local)) {
                ogs_error("OpenAPI_route_selection_parameter_set_parseFromJSON() failed [spatial_validity_areas]");
                goto end;
            }
            OpenAPI_geographical_area_t *spatial_validity_areasItem = OpenAPI_geographical_area_parseFromJSON(spatial_validity_areas_local);
            if (!spatial_validity_areasItem) {
                ogs_error("No spatial_validity_areasItem");
                goto end;
            }
            OpenAPI_list_add(spatial_validity_areasList, spatial_validity_areasItem);
        }
    }

    spatial_validity_tais = cJSON_GetObjectItemCaseSensitive(route_selection_parameter_setJSON, "spatialValidityTais");
    if (spatial_validity_tais) {
        cJSON *spatial_validity_tais_local = NULL;
        if (!cJSON_IsArray(spatial_validity_tais)) {
            ogs_error("OpenAPI_route_selection_parameter_set_parseFromJSON() failed [spatial_validity_tais]");
            goto end;
        }

        spatial_validity_taisList = OpenAPI_list_create();

        cJSON_ArrayForEach(spatial_validity_tais_local, spatial_validity_tais) {
            if (!cJSON_IsObject(spatial_validity_tais_local)) {
                ogs_error("OpenAPI_route_selection_parameter_set_parseFromJSON() failed [spatial_validity_tais]");
                goto end;
            }
            OpenAPI_tai_1_t *spatial_validity_taisItem = OpenAPI_tai_1_parseFromJSON(spatial_validity_tais_local);
            if (!spatial_validity_taisItem) {
                ogs_error("No spatial_validity_taisItem");
                goto end;
            }
            OpenAPI_list_add(spatial_validity_taisList, spatial_validity_taisItem);
        }
    }

    route_selection_parameter_set_local_var = OpenAPI_route_selection_parameter_set_create (
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        precedence ? true : false,
        precedence ? precedence->valuedouble : 0,
        spatial_validity_areas ? spatial_validity_areasList : NULL,
        spatial_validity_tais ? spatial_validity_taisList : NULL
    );

    return route_selection_parameter_set_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (spatial_validity_areasList) {
        OpenAPI_list_for_each(spatial_validity_areasList, node) {
            OpenAPI_geographical_area_free(node->data);
        }
        OpenAPI_list_free(spatial_validity_areasList);
        spatial_validity_areasList = NULL;
    }
    if (spatial_validity_taisList) {
        OpenAPI_list_for_each(spatial_validity_taisList, node) {
            OpenAPI_tai_1_free(node->data);
        }
        OpenAPI_list_free(spatial_validity_taisList);
        spatial_validity_taisList = NULL;
    }
    return NULL;
}

OpenAPI_route_selection_parameter_set_t *OpenAPI_route_selection_parameter_set_copy(OpenAPI_route_selection_parameter_set_t *dst, OpenAPI_route_selection_parameter_set_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_route_selection_parameter_set_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_route_selection_parameter_set_convertToJSON() failed");
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

    OpenAPI_route_selection_parameter_set_free(dst);
    dst = OpenAPI_route_selection_parameter_set_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

