
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_requirement.h"

OpenAPI_dispersion_requirement_t *OpenAPI_dispersion_requirement_create(
    OpenAPI_dispersion_type_t *disper_type,
    OpenAPI_list_t *class_criters,
    OpenAPI_list_t *rank_criters,
    OpenAPI_dispersion_ordering_criterion_t *disp_order_criter,
    OpenAPI_matching_direction_t *order
)
{
    OpenAPI_dispersion_requirement_t *dispersion_requirement_local_var = ogs_malloc(sizeof(OpenAPI_dispersion_requirement_t));
    ogs_assert(dispersion_requirement_local_var);

    dispersion_requirement_local_var->disper_type = disper_type;
    dispersion_requirement_local_var->class_criters = class_criters;
    dispersion_requirement_local_var->rank_criters = rank_criters;
    dispersion_requirement_local_var->disp_order_criter = disp_order_criter;
    dispersion_requirement_local_var->order = order;

    return dispersion_requirement_local_var;
}

void OpenAPI_dispersion_requirement_free(OpenAPI_dispersion_requirement_t *dispersion_requirement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dispersion_requirement) {
        return;
    }
    if (dispersion_requirement->disper_type) {
        OpenAPI_dispersion_type_free(dispersion_requirement->disper_type);
        dispersion_requirement->disper_type = NULL;
    }
    if (dispersion_requirement->class_criters) {
        OpenAPI_list_for_each(dispersion_requirement->class_criters, node) {
            OpenAPI_class_criterion_free(node->data);
        }
        OpenAPI_list_free(dispersion_requirement->class_criters);
        dispersion_requirement->class_criters = NULL;
    }
    if (dispersion_requirement->rank_criters) {
        OpenAPI_list_for_each(dispersion_requirement->rank_criters, node) {
            OpenAPI_ranking_criterion_free(node->data);
        }
        OpenAPI_list_free(dispersion_requirement->rank_criters);
        dispersion_requirement->rank_criters = NULL;
    }
    if (dispersion_requirement->disp_order_criter) {
        OpenAPI_dispersion_ordering_criterion_free(dispersion_requirement->disp_order_criter);
        dispersion_requirement->disp_order_criter = NULL;
    }
    if (dispersion_requirement->order) {
        OpenAPI_matching_direction_free(dispersion_requirement->order);
        dispersion_requirement->order = NULL;
    }
    ogs_free(dispersion_requirement);
}

cJSON *OpenAPI_dispersion_requirement_convertToJSON(OpenAPI_dispersion_requirement_t *dispersion_requirement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dispersion_requirement == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [DispersionRequirement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dispersion_requirement->disper_type) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [disper_type]");
        return NULL;
    }
    cJSON *disper_type_local_JSON = OpenAPI_dispersion_type_convertToJSON(dispersion_requirement->disper_type);
    if (disper_type_local_JSON == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [disper_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "disperType", disper_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [disper_type]");
        goto end;
    }

    if (dispersion_requirement->class_criters) {
    cJSON *class_critersList = cJSON_AddArrayToObject(item, "classCriters");
    if (class_critersList == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [class_criters]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_requirement->class_criters, node) {
        cJSON *itemLocal = OpenAPI_class_criterion_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [class_criters]");
            goto end;
        }
        cJSON_AddItemToArray(class_critersList, itemLocal);
    }
    }

    if (dispersion_requirement->rank_criters) {
    cJSON *rank_critersList = cJSON_AddArrayToObject(item, "rankCriters");
    if (rank_critersList == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [rank_criters]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_requirement->rank_criters, node) {
        cJSON *itemLocal = OpenAPI_ranking_criterion_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [rank_criters]");
            goto end;
        }
        cJSON_AddItemToArray(rank_critersList, itemLocal);
    }
    }

    if (dispersion_requirement->disp_order_criter) {
    cJSON *disp_order_criter_local_JSON = OpenAPI_dispersion_ordering_criterion_convertToJSON(dispersion_requirement->disp_order_criter);
    if (disp_order_criter_local_JSON == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [disp_order_criter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dispOrderCriter", disp_order_criter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [disp_order_criter]");
        goto end;
    }
    }

    if (dispersion_requirement->order) {
    cJSON *order_local_JSON = OpenAPI_matching_direction_convertToJSON(dispersion_requirement->order);
    if (order_local_JSON == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [order]");
        goto end;
    }
    cJSON_AddItemToObject(item, "order", order_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed [order]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dispersion_requirement_t *OpenAPI_dispersion_requirement_parseFromJSON(cJSON *dispersion_requirementJSON)
{
    OpenAPI_dispersion_requirement_t *dispersion_requirement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *disper_type = NULL;
    OpenAPI_dispersion_type_t *disper_type_local_nonprim = NULL;
    cJSON *class_criters = NULL;
    OpenAPI_list_t *class_critersList = NULL;
    cJSON *rank_criters = NULL;
    OpenAPI_list_t *rank_critersList = NULL;
    cJSON *disp_order_criter = NULL;
    OpenAPI_dispersion_ordering_criterion_t *disp_order_criter_local_nonprim = NULL;
    cJSON *order = NULL;
    OpenAPI_matching_direction_t *order_local_nonprim = NULL;
    disper_type = cJSON_GetObjectItemCaseSensitive(dispersion_requirementJSON, "disperType");
    if (!disper_type) {
        ogs_error("OpenAPI_dispersion_requirement_parseFromJSON() failed [disper_type]");
        goto end;
    }
    disper_type_local_nonprim = OpenAPI_dispersion_type_parseFromJSON(disper_type);
    if (!disper_type_local_nonprim) {
        ogs_error("OpenAPI_dispersion_type_parseFromJSON failed [disper_type]");
        goto end;
    }

    class_criters = cJSON_GetObjectItemCaseSensitive(dispersion_requirementJSON, "classCriters");
    if (class_criters) {
        cJSON *class_criters_local = NULL;
        if (!cJSON_IsArray(class_criters)) {
            ogs_error("OpenAPI_dispersion_requirement_parseFromJSON() failed [class_criters]");
            goto end;
        }

        class_critersList = OpenAPI_list_create();

        cJSON_ArrayForEach(class_criters_local, class_criters) {
            if (!cJSON_IsObject(class_criters_local)) {
                ogs_error("OpenAPI_dispersion_requirement_parseFromJSON() failed [class_criters]");
                goto end;
            }
            OpenAPI_class_criterion_t *class_critersItem = OpenAPI_class_criterion_parseFromJSON(class_criters_local);
            if (!class_critersItem) {
                ogs_error("No class_critersItem");
                goto end;
            }
            OpenAPI_list_add(class_critersList, class_critersItem);
        }
    }

    rank_criters = cJSON_GetObjectItemCaseSensitive(dispersion_requirementJSON, "rankCriters");
    if (rank_criters) {
        cJSON *rank_criters_local = NULL;
        if (!cJSON_IsArray(rank_criters)) {
            ogs_error("OpenAPI_dispersion_requirement_parseFromJSON() failed [rank_criters]");
            goto end;
        }

        rank_critersList = OpenAPI_list_create();

        cJSON_ArrayForEach(rank_criters_local, rank_criters) {
            if (!cJSON_IsObject(rank_criters_local)) {
                ogs_error("OpenAPI_dispersion_requirement_parseFromJSON() failed [rank_criters]");
                goto end;
            }
            OpenAPI_ranking_criterion_t *rank_critersItem = OpenAPI_ranking_criterion_parseFromJSON(rank_criters_local);
            if (!rank_critersItem) {
                ogs_error("No rank_critersItem");
                goto end;
            }
            OpenAPI_list_add(rank_critersList, rank_critersItem);
        }
    }

    disp_order_criter = cJSON_GetObjectItemCaseSensitive(dispersion_requirementJSON, "dispOrderCriter");
    if (disp_order_criter) {
    disp_order_criter_local_nonprim = OpenAPI_dispersion_ordering_criterion_parseFromJSON(disp_order_criter);
    if (!disp_order_criter_local_nonprim) {
        ogs_error("OpenAPI_dispersion_ordering_criterion_parseFromJSON failed [disp_order_criter]");
        goto end;
    }
    }

    order = cJSON_GetObjectItemCaseSensitive(dispersion_requirementJSON, "order");
    if (order) {
    order_local_nonprim = OpenAPI_matching_direction_parseFromJSON(order);
    if (!order_local_nonprim) {
        ogs_error("OpenAPI_matching_direction_parseFromJSON failed [order]");
        goto end;
    }
    }

    dispersion_requirement_local_var = OpenAPI_dispersion_requirement_create (
        disper_type_local_nonprim,
        class_criters ? class_critersList : NULL,
        rank_criters ? rank_critersList : NULL,
        disp_order_criter ? disp_order_criter_local_nonprim : NULL,
        order ? order_local_nonprim : NULL
    );

    return dispersion_requirement_local_var;
end:
    if (disper_type_local_nonprim) {
        OpenAPI_dispersion_type_free(disper_type_local_nonprim);
        disper_type_local_nonprim = NULL;
    }
    if (class_critersList) {
        OpenAPI_list_for_each(class_critersList, node) {
            OpenAPI_class_criterion_free(node->data);
        }
        OpenAPI_list_free(class_critersList);
        class_critersList = NULL;
    }
    if (rank_critersList) {
        OpenAPI_list_for_each(rank_critersList, node) {
            OpenAPI_ranking_criterion_free(node->data);
        }
        OpenAPI_list_free(rank_critersList);
        rank_critersList = NULL;
    }
    if (disp_order_criter_local_nonprim) {
        OpenAPI_dispersion_ordering_criterion_free(disp_order_criter_local_nonprim);
        disp_order_criter_local_nonprim = NULL;
    }
    if (order_local_nonprim) {
        OpenAPI_matching_direction_free(order_local_nonprim);
        order_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_dispersion_requirement_t *OpenAPI_dispersion_requirement_copy(OpenAPI_dispersion_requirement_t *dst, OpenAPI_dispersion_requirement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dispersion_requirement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dispersion_requirement_convertToJSON() failed");
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

    OpenAPI_dispersion_requirement_free(dst);
    dst = OpenAPI_dispersion_requirement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

