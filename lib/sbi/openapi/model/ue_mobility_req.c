
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_mobility_req.h"

OpenAPI_ue_mobility_req_t *OpenAPI_ue_mobility_req_create(
    OpenAPI_ue_mobility_order_criterion_e order_criterion,
    OpenAPI_matching_direction_e order_direction,
    bool is_ue_loc_order_ind,
    int ue_loc_order_ind,
    OpenAPI_list_t *dist_thresholds
)
{
    OpenAPI_ue_mobility_req_t *ue_mobility_req_local_var = ogs_malloc(sizeof(OpenAPI_ue_mobility_req_t));
    ogs_assert(ue_mobility_req_local_var);

    ue_mobility_req_local_var->order_criterion = order_criterion;
    ue_mobility_req_local_var->order_direction = order_direction;
    ue_mobility_req_local_var->is_ue_loc_order_ind = is_ue_loc_order_ind;
    ue_mobility_req_local_var->ue_loc_order_ind = ue_loc_order_ind;
    ue_mobility_req_local_var->dist_thresholds = dist_thresholds;

    return ue_mobility_req_local_var;
}

void OpenAPI_ue_mobility_req_free(OpenAPI_ue_mobility_req_t *ue_mobility_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_mobility_req) {
        return;
    }
    if (ue_mobility_req->dist_thresholds) {
        OpenAPI_list_for_each(ue_mobility_req->dist_thresholds, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_mobility_req->dist_thresholds);
        ue_mobility_req->dist_thresholds = NULL;
    }
    ogs_free(ue_mobility_req);
}

cJSON *OpenAPI_ue_mobility_req_convertToJSON(OpenAPI_ue_mobility_req_t *ue_mobility_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_mobility_req == NULL) {
        ogs_error("OpenAPI_ue_mobility_req_convertToJSON() failed [UeMobilityReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_mobility_req->order_criterion != OpenAPI_ue_mobility_order_criterion_NULL) {
    if (cJSON_AddStringToObject(item, "orderCriterion", OpenAPI_ue_mobility_order_criterion_ToString(ue_mobility_req->order_criterion)) == NULL) {
        ogs_error("OpenAPI_ue_mobility_req_convertToJSON() failed [order_criterion]");
        goto end;
    }
    }

    if (ue_mobility_req->order_direction != OpenAPI_matching_direction_NULL) {
    if (cJSON_AddStringToObject(item, "orderDirection", OpenAPI_matching_direction_ToString(ue_mobility_req->order_direction)) == NULL) {
        ogs_error("OpenAPI_ue_mobility_req_convertToJSON() failed [order_direction]");
        goto end;
    }
    }

    if (ue_mobility_req->is_ue_loc_order_ind) {
    if (cJSON_AddBoolToObject(item, "ueLocOrderInd", ue_mobility_req->ue_loc_order_ind) == NULL) {
        ogs_error("OpenAPI_ue_mobility_req_convertToJSON() failed [ue_loc_order_ind]");
        goto end;
    }
    }

    if (ue_mobility_req->dist_thresholds) {
    cJSON *dist_thresholdsList = cJSON_AddArrayToObject(item, "distThresholds");
    if (dist_thresholdsList == NULL) {
        ogs_error("OpenAPI_ue_mobility_req_convertToJSON() failed [dist_thresholds]");
        goto end;
    }
    OpenAPI_list_for_each(ue_mobility_req->dist_thresholds, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_ue_mobility_req_convertToJSON() failed [dist_thresholds]");
            goto end;
        }
        if (cJSON_AddNumberToObject(dist_thresholdsList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_ue_mobility_req_convertToJSON() failed [dist_thresholds]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_ue_mobility_req_t *OpenAPI_ue_mobility_req_parseFromJSON(cJSON *ue_mobility_reqJSON)
{
    OpenAPI_ue_mobility_req_t *ue_mobility_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *order_criterion = NULL;
    OpenAPI_ue_mobility_order_criterion_e order_criterionVariable = 0;
    cJSON *order_direction = NULL;
    OpenAPI_matching_direction_e order_directionVariable = 0;
    cJSON *ue_loc_order_ind = NULL;
    cJSON *dist_thresholds = NULL;
    OpenAPI_list_t *dist_thresholdsList = NULL;
    order_criterion = cJSON_GetObjectItemCaseSensitive(ue_mobility_reqJSON, "orderCriterion");
    if (order_criterion) {
    if (!cJSON_IsString(order_criterion)) {
        ogs_error("OpenAPI_ue_mobility_req_parseFromJSON() failed [order_criterion]");
        goto end;
    }
    order_criterionVariable = OpenAPI_ue_mobility_order_criterion_FromString(order_criterion->valuestring);
    }

    order_direction = cJSON_GetObjectItemCaseSensitive(ue_mobility_reqJSON, "orderDirection");
    if (order_direction) {
    if (!cJSON_IsString(order_direction)) {
        ogs_error("OpenAPI_ue_mobility_req_parseFromJSON() failed [order_direction]");
        goto end;
    }
    order_directionVariable = OpenAPI_matching_direction_FromString(order_direction->valuestring);
    }

    ue_loc_order_ind = cJSON_GetObjectItemCaseSensitive(ue_mobility_reqJSON, "ueLocOrderInd");
    if (ue_loc_order_ind) {
    if (!cJSON_IsBool(ue_loc_order_ind)) {
        ogs_error("OpenAPI_ue_mobility_req_parseFromJSON() failed [ue_loc_order_ind]");
        goto end;
    }
    }

    dist_thresholds = cJSON_GetObjectItemCaseSensitive(ue_mobility_reqJSON, "distThresholds");
    if (dist_thresholds) {
        cJSON *dist_thresholds_local = NULL;
        if (!cJSON_IsArray(dist_thresholds)) {
            ogs_error("OpenAPI_ue_mobility_req_parseFromJSON() failed [dist_thresholds]");
            goto end;
        }

        dist_thresholdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dist_thresholds_local, dist_thresholds) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(dist_thresholds_local)) {
                ogs_error("OpenAPI_ue_mobility_req_parseFromJSON() failed [dist_thresholds]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_ue_mobility_req_parseFromJSON() failed [dist_thresholds]");
                goto end;
            }
            *localDouble = dist_thresholds_local->valuedouble;
            OpenAPI_list_add(dist_thresholdsList, localDouble);
        }
    }

    ue_mobility_req_local_var = OpenAPI_ue_mobility_req_create (
        order_criterion ? order_criterionVariable : 0,
        order_direction ? order_directionVariable : 0,
        ue_loc_order_ind ? true : false,
        ue_loc_order_ind ? ue_loc_order_ind->valueint : 0,
        dist_thresholds ? dist_thresholdsList : NULL
    );

    return ue_mobility_req_local_var;
end:
    if (dist_thresholdsList) {
        OpenAPI_list_for_each(dist_thresholdsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dist_thresholdsList);
        dist_thresholdsList = NULL;
    }
    return NULL;
}

OpenAPI_ue_mobility_req_t *OpenAPI_ue_mobility_req_copy(OpenAPI_ue_mobility_req_t *dst, OpenAPI_ue_mobility_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_mobility_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_mobility_req_convertToJSON() failed");
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

    OpenAPI_ue_mobility_req_free(dst);
    dst = OpenAPI_ue_mobility_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

