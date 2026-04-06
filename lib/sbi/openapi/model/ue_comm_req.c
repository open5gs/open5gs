
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_comm_req.h"

OpenAPI_ue_comm_req_t *OpenAPI_ue_comm_req_create(
    OpenAPI_ue_comm_order_criterion_e order_criterion,
    OpenAPI_matching_direction_e order_direction
)
{
    OpenAPI_ue_comm_req_t *ue_comm_req_local_var = ogs_malloc(sizeof(OpenAPI_ue_comm_req_t));
    ogs_assert(ue_comm_req_local_var);

    ue_comm_req_local_var->order_criterion = order_criterion;
    ue_comm_req_local_var->order_direction = order_direction;

    return ue_comm_req_local_var;
}

void OpenAPI_ue_comm_req_free(OpenAPI_ue_comm_req_t *ue_comm_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_comm_req) {
        return;
    }
    ogs_free(ue_comm_req);
}

cJSON *OpenAPI_ue_comm_req_convertToJSON(OpenAPI_ue_comm_req_t *ue_comm_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_comm_req == NULL) {
        ogs_error("OpenAPI_ue_comm_req_convertToJSON() failed [UeCommReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_comm_req->order_criterion != OpenAPI_ue_comm_order_criterion_NULL) {
    if (cJSON_AddStringToObject(item, "orderCriterion", OpenAPI_ue_comm_order_criterion_ToString(ue_comm_req->order_criterion)) == NULL) {
        ogs_error("OpenAPI_ue_comm_req_convertToJSON() failed [order_criterion]");
        goto end;
    }
    }

    if (ue_comm_req->order_direction != OpenAPI_matching_direction_NULL) {
    if (cJSON_AddStringToObject(item, "orderDirection", OpenAPI_matching_direction_ToString(ue_comm_req->order_direction)) == NULL) {
        ogs_error("OpenAPI_ue_comm_req_convertToJSON() failed [order_direction]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_comm_req_t *OpenAPI_ue_comm_req_parseFromJSON(cJSON *ue_comm_reqJSON)
{
    OpenAPI_ue_comm_req_t *ue_comm_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *order_criterion = NULL;
    OpenAPI_ue_comm_order_criterion_e order_criterionVariable = 0;
    cJSON *order_direction = NULL;
    OpenAPI_matching_direction_e order_directionVariable = 0;
    order_criterion = cJSON_GetObjectItemCaseSensitive(ue_comm_reqJSON, "orderCriterion");
    if (order_criterion) {
    if (!cJSON_IsString(order_criterion)) {
        ogs_error("OpenAPI_ue_comm_req_parseFromJSON() failed [order_criterion]");
        goto end;
    }
    order_criterionVariable = OpenAPI_ue_comm_order_criterion_FromString(order_criterion->valuestring);
    }

    order_direction = cJSON_GetObjectItemCaseSensitive(ue_comm_reqJSON, "orderDirection");
    if (order_direction) {
    if (!cJSON_IsString(order_direction)) {
        ogs_error("OpenAPI_ue_comm_req_parseFromJSON() failed [order_direction]");
        goto end;
    }
    order_directionVariable = OpenAPI_matching_direction_FromString(order_direction->valuestring);
    }

    ue_comm_req_local_var = OpenAPI_ue_comm_req_create (
        order_criterion ? order_criterionVariable : 0,
        order_direction ? order_directionVariable : 0
    );

    return ue_comm_req_local_var;
end:
    return NULL;
}

OpenAPI_ue_comm_req_t *OpenAPI_ue_comm_req_copy(OpenAPI_ue_comm_req_t *dst, OpenAPI_ue_comm_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_comm_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_comm_req_convertToJSON() failed");
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

    OpenAPI_ue_comm_req_free(dst);
    dst = OpenAPI_ue_comm_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

