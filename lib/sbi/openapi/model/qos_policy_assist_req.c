
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_policy_assist_req.h"

OpenAPI_qos_policy_assist_req_t *OpenAPI_qos_policy_assist_req_create(
    OpenAPI_qos_pol_order_criterion_e order_criterion,
    OpenAPI_matching_direction_e order_direction,
    OpenAPI_list_t *freqs,
    OpenAPI_list_t *rat_types,
    OpenAPI_list_t *qos_param_sets,
    bool is_requested_qoe,
    float requested_qoe
)
{
    OpenAPI_qos_policy_assist_req_t *qos_policy_assist_req_local_var = ogs_malloc(sizeof(OpenAPI_qos_policy_assist_req_t));
    ogs_assert(qos_policy_assist_req_local_var);

    qos_policy_assist_req_local_var->order_criterion = order_criterion;
    qos_policy_assist_req_local_var->order_direction = order_direction;
    qos_policy_assist_req_local_var->freqs = freqs;
    qos_policy_assist_req_local_var->rat_types = rat_types;
    qos_policy_assist_req_local_var->qos_param_sets = qos_param_sets;
    qos_policy_assist_req_local_var->is_requested_qoe = is_requested_qoe;
    qos_policy_assist_req_local_var->requested_qoe = requested_qoe;

    return qos_policy_assist_req_local_var;
}

void OpenAPI_qos_policy_assist_req_free(OpenAPI_qos_policy_assist_req_t *qos_policy_assist_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_policy_assist_req) {
        return;
    }
    if (qos_policy_assist_req->freqs) {
        OpenAPI_list_for_each(qos_policy_assist_req->freqs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_policy_assist_req->freqs);
        qos_policy_assist_req->freqs = NULL;
    }
    if (qos_policy_assist_req->rat_types) {
        OpenAPI_list_free(qos_policy_assist_req->rat_types);
        qos_policy_assist_req->rat_types = NULL;
    }
    if (qos_policy_assist_req->qos_param_sets) {
        OpenAPI_list_for_each(qos_policy_assist_req->qos_param_sets, node) {
            OpenAPI_qos_para_free(node->data);
        }
        OpenAPI_list_free(qos_policy_assist_req->qos_param_sets);
        qos_policy_assist_req->qos_param_sets = NULL;
    }
    ogs_free(qos_policy_assist_req);
}

cJSON *OpenAPI_qos_policy_assist_req_convertToJSON(OpenAPI_qos_policy_assist_req_t *qos_policy_assist_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_policy_assist_req == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [QosPolicyAssistReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_policy_assist_req->order_criterion != OpenAPI_qos_pol_order_criterion_NULL) {
    if (cJSON_AddStringToObject(item, "orderCriterion", OpenAPI_qos_pol_order_criterion_ToString(qos_policy_assist_req->order_criterion)) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [order_criterion]");
        goto end;
    }
    }

    if (qos_policy_assist_req->order_direction != OpenAPI_matching_direction_NULL) {
    if (cJSON_AddStringToObject(item, "orderDirection", OpenAPI_matching_direction_ToString(qos_policy_assist_req->order_direction)) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [order_direction]");
        goto end;
    }
    }

    if (qos_policy_assist_req->freqs) {
    cJSON *freqsList = cJSON_AddArrayToObject(item, "freqs");
    if (freqsList == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [freqs]");
        goto end;
    }
    OpenAPI_list_for_each(qos_policy_assist_req->freqs, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [freqs]");
            goto end;
        }
        if (cJSON_AddNumberToObject(freqsList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [freqs]");
            goto end;
        }
    }
    }

    if (qos_policy_assist_req->rat_types != OpenAPI_rat_type_NULL) {
    cJSON *rat_typesList = cJSON_AddArrayToObject(item, "ratTypes");
    if (rat_typesList == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [rat_types]");
        goto end;
    }
    OpenAPI_list_for_each(qos_policy_assist_req->rat_types, node) {
        if (cJSON_AddStringToObject(rat_typesList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [rat_types]");
            goto end;
        }
    }
    }

    if (!qos_policy_assist_req->qos_param_sets) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [qos_param_sets]");
        return NULL;
    }
    cJSON *qos_param_setsList = cJSON_AddArrayToObject(item, "qosParamSets");
    if (qos_param_setsList == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [qos_param_sets]");
        goto end;
    }
    OpenAPI_list_for_each(qos_policy_assist_req->qos_param_sets, node) {
        cJSON *itemLocal = OpenAPI_qos_para_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [qos_param_sets]");
            goto end;
        }
        cJSON_AddItemToArray(qos_param_setsList, itemLocal);
    }

    if (qos_policy_assist_req->is_requested_qoe) {
    if (cJSON_AddNumberToObject(item, "requestedQoe", qos_policy_assist_req->requested_qoe) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed [requested_qoe]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_policy_assist_req_t *OpenAPI_qos_policy_assist_req_parseFromJSON(cJSON *qos_policy_assist_reqJSON)
{
    OpenAPI_qos_policy_assist_req_t *qos_policy_assist_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *order_criterion = NULL;
    OpenAPI_qos_pol_order_criterion_e order_criterionVariable = 0;
    cJSON *order_direction = NULL;
    OpenAPI_matching_direction_e order_directionVariable = 0;
    cJSON *freqs = NULL;
    OpenAPI_list_t *freqsList = NULL;
    cJSON *rat_types = NULL;
    OpenAPI_list_t *rat_typesList = NULL;
    cJSON *qos_param_sets = NULL;
    OpenAPI_list_t *qos_param_setsList = NULL;
    cJSON *requested_qoe = NULL;
    order_criterion = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_reqJSON, "orderCriterion");
    if (order_criterion) {
    if (!cJSON_IsString(order_criterion)) {
        ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [order_criterion]");
        goto end;
    }
    order_criterionVariable = OpenAPI_qos_pol_order_criterion_FromString(order_criterion->valuestring);
    }

    order_direction = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_reqJSON, "orderDirection");
    if (order_direction) {
    if (!cJSON_IsString(order_direction)) {
        ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [order_direction]");
        goto end;
    }
    order_directionVariable = OpenAPI_matching_direction_FromString(order_direction->valuestring);
    }

    freqs = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_reqJSON, "freqs");
    if (freqs) {
        cJSON *freqs_local = NULL;
        if (!cJSON_IsArray(freqs)) {
            ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [freqs]");
            goto end;
        }

        freqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(freqs_local, freqs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(freqs_local)) {
                ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [freqs]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [freqs]");
                goto end;
            }
            *localDouble = freqs_local->valuedouble;
            OpenAPI_list_add(freqsList, localDouble);
        }
    }

    rat_types = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_reqJSON, "ratTypes");
    if (rat_types) {
        cJSON *rat_types_local = NULL;
        if (!cJSON_IsArray(rat_types)) {
            ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [rat_types]");
            goto end;
        }

        rat_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_types_local, rat_types) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(rat_types_local)) {
                ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [rat_types]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(rat_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rat_types\" is not supported. Ignoring it ...",
                         rat_types_local->valuestring);
            } else {
                OpenAPI_list_add(rat_typesList, (void *)localEnum);
            }
        }
        if (rat_typesList->count == 0) {
            ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed: Expected rat_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    qos_param_sets = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_reqJSON, "qosParamSets");
    if (!qos_param_sets) {
        ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [qos_param_sets]");
        goto end;
    }
        cJSON *qos_param_sets_local = NULL;
        if (!cJSON_IsArray(qos_param_sets)) {
            ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [qos_param_sets]");
            goto end;
        }

        qos_param_setsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_param_sets_local, qos_param_sets) {
            if (!cJSON_IsObject(qos_param_sets_local)) {
                ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [qos_param_sets]");
                goto end;
            }
            OpenAPI_qos_para_t *qos_param_setsItem = OpenAPI_qos_para_parseFromJSON(qos_param_sets_local);
            if (!qos_param_setsItem) {
                ogs_error("No qos_param_setsItem");
                goto end;
            }
            OpenAPI_list_add(qos_param_setsList, qos_param_setsItem);
        }

    requested_qoe = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_reqJSON, "requestedQoe");
    if (requested_qoe) {
    if (!cJSON_IsNumber(requested_qoe)) {
        ogs_error("OpenAPI_qos_policy_assist_req_parseFromJSON() failed [requested_qoe]");
        goto end;
    }
    }

    qos_policy_assist_req_local_var = OpenAPI_qos_policy_assist_req_create (
        order_criterion ? order_criterionVariable : 0,
        order_direction ? order_directionVariable : 0,
        freqs ? freqsList : NULL,
        rat_types ? rat_typesList : NULL,
        qos_param_setsList,
        requested_qoe ? true : false,
        requested_qoe ? requested_qoe->valuedouble : 0
    );

    return qos_policy_assist_req_local_var;
end:
    if (freqsList) {
        OpenAPI_list_for_each(freqsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(freqsList);
        freqsList = NULL;
    }
    if (rat_typesList) {
        OpenAPI_list_free(rat_typesList);
        rat_typesList = NULL;
    }
    if (qos_param_setsList) {
        OpenAPI_list_for_each(qos_param_setsList, node) {
            OpenAPI_qos_para_free(node->data);
        }
        OpenAPI_list_free(qos_param_setsList);
        qos_param_setsList = NULL;
    }
    return NULL;
}

OpenAPI_qos_policy_assist_req_t *OpenAPI_qos_policy_assist_req_copy(OpenAPI_qos_policy_assist_req_t *dst, OpenAPI_qos_policy_assist_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_policy_assist_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_policy_assist_req_convertToJSON() failed");
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

    OpenAPI_qos_policy_assist_req_free(dst);
    dst = OpenAPI_qos_policy_assist_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

