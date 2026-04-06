
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dn_performance_req.h"

OpenAPI_dn_performance_req_t *OpenAPI_dn_performance_req_create(
    OpenAPI_dn_perf_ordering_criterion_e dn_perf_order_criter,
    OpenAPI_matching_direction_e order,
    OpenAPI_list_t *report_thresholds
)
{
    OpenAPI_dn_performance_req_t *dn_performance_req_local_var = ogs_malloc(sizeof(OpenAPI_dn_performance_req_t));
    ogs_assert(dn_performance_req_local_var);

    dn_performance_req_local_var->dn_perf_order_criter = dn_perf_order_criter;
    dn_performance_req_local_var->order = order;
    dn_performance_req_local_var->report_thresholds = report_thresholds;

    return dn_performance_req_local_var;
}

void OpenAPI_dn_performance_req_free(OpenAPI_dn_performance_req_t *dn_performance_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dn_performance_req) {
        return;
    }
    if (dn_performance_req->report_thresholds) {
        OpenAPI_list_for_each(dn_performance_req->report_thresholds, node) {
            OpenAPI_threshold_level_free(node->data);
        }
        OpenAPI_list_free(dn_performance_req->report_thresholds);
        dn_performance_req->report_thresholds = NULL;
    }
    ogs_free(dn_performance_req);
}

cJSON *OpenAPI_dn_performance_req_convertToJSON(OpenAPI_dn_performance_req_t *dn_performance_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dn_performance_req == NULL) {
        ogs_error("OpenAPI_dn_performance_req_convertToJSON() failed [DnPerformanceReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dn_performance_req->dn_perf_order_criter != OpenAPI_dn_perf_ordering_criterion_NULL) {
    if (cJSON_AddStringToObject(item, "dnPerfOrderCriter", OpenAPI_dn_perf_ordering_criterion_ToString(dn_performance_req->dn_perf_order_criter)) == NULL) {
        ogs_error("OpenAPI_dn_performance_req_convertToJSON() failed [dn_perf_order_criter]");
        goto end;
    }
    }

    if (dn_performance_req->order != OpenAPI_matching_direction_NULL) {
    if (cJSON_AddStringToObject(item, "order", OpenAPI_matching_direction_ToString(dn_performance_req->order)) == NULL) {
        ogs_error("OpenAPI_dn_performance_req_convertToJSON() failed [order]");
        goto end;
    }
    }

    if (dn_performance_req->report_thresholds) {
    cJSON *report_thresholdsList = cJSON_AddArrayToObject(item, "reportThresholds");
    if (report_thresholdsList == NULL) {
        ogs_error("OpenAPI_dn_performance_req_convertToJSON() failed [report_thresholds]");
        goto end;
    }
    OpenAPI_list_for_each(dn_performance_req->report_thresholds, node) {
        cJSON *itemLocal = OpenAPI_threshold_level_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dn_performance_req_convertToJSON() failed [report_thresholds]");
            goto end;
        }
        cJSON_AddItemToArray(report_thresholdsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_dn_performance_req_t *OpenAPI_dn_performance_req_parseFromJSON(cJSON *dn_performance_reqJSON)
{
    OpenAPI_dn_performance_req_t *dn_performance_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dn_perf_order_criter = NULL;
    OpenAPI_dn_perf_ordering_criterion_e dn_perf_order_criterVariable = 0;
    cJSON *order = NULL;
    OpenAPI_matching_direction_e orderVariable = 0;
    cJSON *report_thresholds = NULL;
    OpenAPI_list_t *report_thresholdsList = NULL;
    dn_perf_order_criter = cJSON_GetObjectItemCaseSensitive(dn_performance_reqJSON, "dnPerfOrderCriter");
    if (dn_perf_order_criter) {
    if (!cJSON_IsString(dn_perf_order_criter)) {
        ogs_error("OpenAPI_dn_performance_req_parseFromJSON() failed [dn_perf_order_criter]");
        goto end;
    }
    dn_perf_order_criterVariable = OpenAPI_dn_perf_ordering_criterion_FromString(dn_perf_order_criter->valuestring);
    }

    order = cJSON_GetObjectItemCaseSensitive(dn_performance_reqJSON, "order");
    if (order) {
    if (!cJSON_IsString(order)) {
        ogs_error("OpenAPI_dn_performance_req_parseFromJSON() failed [order]");
        goto end;
    }
    orderVariable = OpenAPI_matching_direction_FromString(order->valuestring);
    }

    report_thresholds = cJSON_GetObjectItemCaseSensitive(dn_performance_reqJSON, "reportThresholds");
    if (report_thresholds) {
        cJSON *report_thresholds_local = NULL;
        if (!cJSON_IsArray(report_thresholds)) {
            ogs_error("OpenAPI_dn_performance_req_parseFromJSON() failed [report_thresholds]");
            goto end;
        }

        report_thresholdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(report_thresholds_local, report_thresholds) {
            if (!cJSON_IsObject(report_thresholds_local)) {
                ogs_error("OpenAPI_dn_performance_req_parseFromJSON() failed [report_thresholds]");
                goto end;
            }
            OpenAPI_threshold_level_t *report_thresholdsItem = OpenAPI_threshold_level_parseFromJSON(report_thresholds_local);
            if (!report_thresholdsItem) {
                ogs_error("No report_thresholdsItem");
                goto end;
            }
            OpenAPI_list_add(report_thresholdsList, report_thresholdsItem);
        }
    }

    dn_performance_req_local_var = OpenAPI_dn_performance_req_create (
        dn_perf_order_criter ? dn_perf_order_criterVariable : 0,
        order ? orderVariable : 0,
        report_thresholds ? report_thresholdsList : NULL
    );

    return dn_performance_req_local_var;
end:
    if (report_thresholdsList) {
        OpenAPI_list_for_each(report_thresholdsList, node) {
            OpenAPI_threshold_level_free(node->data);
        }
        OpenAPI_list_free(report_thresholdsList);
        report_thresholdsList = NULL;
    }
    return NULL;
}

OpenAPI_dn_performance_req_t *OpenAPI_dn_performance_req_copy(OpenAPI_dn_performance_req_t *dst, OpenAPI_dn_performance_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dn_performance_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dn_performance_req_convertToJSON() failed");
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

    OpenAPI_dn_performance_req_free(dst);
    dst = OpenAPI_dn_performance_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

