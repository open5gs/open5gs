
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redundant_transmission_exp_req.h"

OpenAPI_redundant_transmission_exp_req_t *OpenAPI_redundant_transmission_exp_req_create(
    OpenAPI_red_trans_exp_ordering_criterion_e red_t_order_criter,
    OpenAPI_matching_direction_e order
)
{
    OpenAPI_redundant_transmission_exp_req_t *redundant_transmission_exp_req_local_var = ogs_malloc(sizeof(OpenAPI_redundant_transmission_exp_req_t));
    ogs_assert(redundant_transmission_exp_req_local_var);

    redundant_transmission_exp_req_local_var->red_t_order_criter = red_t_order_criter;
    redundant_transmission_exp_req_local_var->order = order;

    return redundant_transmission_exp_req_local_var;
}

void OpenAPI_redundant_transmission_exp_req_free(OpenAPI_redundant_transmission_exp_req_t *redundant_transmission_exp_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == redundant_transmission_exp_req) {
        return;
    }
    ogs_free(redundant_transmission_exp_req);
}

cJSON *OpenAPI_redundant_transmission_exp_req_convertToJSON(OpenAPI_redundant_transmission_exp_req_t *redundant_transmission_exp_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (redundant_transmission_exp_req == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_req_convertToJSON() failed [RedundantTransmissionExpReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (redundant_transmission_exp_req->red_t_order_criter != OpenAPI_red_trans_exp_ordering_criterion_NULL) {
    if (cJSON_AddStringToObject(item, "redTOrderCriter", OpenAPI_red_trans_exp_ordering_criterion_ToString(redundant_transmission_exp_req->red_t_order_criter)) == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_req_convertToJSON() failed [red_t_order_criter]");
        goto end;
    }
    }

    if (redundant_transmission_exp_req->order != OpenAPI_matching_direction_NULL) {
    if (cJSON_AddStringToObject(item, "order", OpenAPI_matching_direction_ToString(redundant_transmission_exp_req->order)) == NULL) {
        ogs_error("OpenAPI_redundant_transmission_exp_req_convertToJSON() failed [order]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_redundant_transmission_exp_req_t *OpenAPI_redundant_transmission_exp_req_parseFromJSON(cJSON *redundant_transmission_exp_reqJSON)
{
    OpenAPI_redundant_transmission_exp_req_t *redundant_transmission_exp_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *red_t_order_criter = NULL;
    OpenAPI_red_trans_exp_ordering_criterion_e red_t_order_criterVariable = 0;
    cJSON *order = NULL;
    OpenAPI_matching_direction_e orderVariable = 0;
    red_t_order_criter = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_reqJSON, "redTOrderCriter");
    if (red_t_order_criter) {
    if (!cJSON_IsString(red_t_order_criter)) {
        ogs_error("OpenAPI_redundant_transmission_exp_req_parseFromJSON() failed [red_t_order_criter]");
        goto end;
    }
    red_t_order_criterVariable = OpenAPI_red_trans_exp_ordering_criterion_FromString(red_t_order_criter->valuestring);
    }

    order = cJSON_GetObjectItemCaseSensitive(redundant_transmission_exp_reqJSON, "order");
    if (order) {
    if (!cJSON_IsString(order)) {
        ogs_error("OpenAPI_redundant_transmission_exp_req_parseFromJSON() failed [order]");
        goto end;
    }
    orderVariable = OpenAPI_matching_direction_FromString(order->valuestring);
    }

    redundant_transmission_exp_req_local_var = OpenAPI_redundant_transmission_exp_req_create (
        red_t_order_criter ? red_t_order_criterVariable : 0,
        order ? orderVariable : 0
    );

    return redundant_transmission_exp_req_local_var;
end:
    return NULL;
}

OpenAPI_redundant_transmission_exp_req_t *OpenAPI_redundant_transmission_exp_req_copy(OpenAPI_redundant_transmission_exp_req_t *dst, OpenAPI_redundant_transmission_exp_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_redundant_transmission_exp_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_redundant_transmission_exp_req_convertToJSON() failed");
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

    OpenAPI_redundant_transmission_exp_req_free(dst);
    dst = OpenAPI_redundant_transmission_exp_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

