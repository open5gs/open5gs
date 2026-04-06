
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_report.h"

OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_create(
    OpenAPI_list_t *flows,
    OpenAPI_list_t *ul_delays,
    OpenAPI_list_t *dl_delays,
    OpenAPI_list_t *rt_delays,
    bool is_pdmf,
    int pdmf,
    OpenAPI_list_t *ul_con_info,
    OpenAPI_list_t *dl_con_info,
    char *ul_data_rate,
    char *dl_data_rate
)
{
    OpenAPI_qos_monitoring_report_t *qos_monitoring_report_local_var = ogs_malloc(sizeof(OpenAPI_qos_monitoring_report_t));
    ogs_assert(qos_monitoring_report_local_var);

    qos_monitoring_report_local_var->flows = flows;
    qos_monitoring_report_local_var->ul_delays = ul_delays;
    qos_monitoring_report_local_var->dl_delays = dl_delays;
    qos_monitoring_report_local_var->rt_delays = rt_delays;
    qos_monitoring_report_local_var->is_pdmf = is_pdmf;
    qos_monitoring_report_local_var->pdmf = pdmf;
    qos_monitoring_report_local_var->ul_con_info = ul_con_info;
    qos_monitoring_report_local_var->dl_con_info = dl_con_info;
    qos_monitoring_report_local_var->ul_data_rate = ul_data_rate;
    qos_monitoring_report_local_var->dl_data_rate = dl_data_rate;

    return qos_monitoring_report_local_var;
}

void OpenAPI_qos_monitoring_report_free(OpenAPI_qos_monitoring_report_t *qos_monitoring_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_monitoring_report) {
        return;
    }
    if (qos_monitoring_report->flows) {
        OpenAPI_list_for_each(qos_monitoring_report->flows, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_report->flows);
        qos_monitoring_report->flows = NULL;
    }
    if (qos_monitoring_report->ul_delays) {
        OpenAPI_list_for_each(qos_monitoring_report->ul_delays, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_report->ul_delays);
        qos_monitoring_report->ul_delays = NULL;
    }
    if (qos_monitoring_report->dl_delays) {
        OpenAPI_list_for_each(qos_monitoring_report->dl_delays, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_report->dl_delays);
        qos_monitoring_report->dl_delays = NULL;
    }
    if (qos_monitoring_report->rt_delays) {
        OpenAPI_list_for_each(qos_monitoring_report->rt_delays, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_report->rt_delays);
        qos_monitoring_report->rt_delays = NULL;
    }
    if (qos_monitoring_report->ul_con_info) {
        OpenAPI_list_for_each(qos_monitoring_report->ul_con_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_report->ul_con_info);
        qos_monitoring_report->ul_con_info = NULL;
    }
    if (qos_monitoring_report->dl_con_info) {
        OpenAPI_list_for_each(qos_monitoring_report->dl_con_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_report->dl_con_info);
        qos_monitoring_report->dl_con_info = NULL;
    }
    if (qos_monitoring_report->ul_data_rate) {
        ogs_free(qos_monitoring_report->ul_data_rate);
        qos_monitoring_report->ul_data_rate = NULL;
    }
    if (qos_monitoring_report->dl_data_rate) {
        ogs_free(qos_monitoring_report->dl_data_rate);
        qos_monitoring_report->dl_data_rate = NULL;
    }
    ogs_free(qos_monitoring_report);
}

cJSON *OpenAPI_qos_monitoring_report_convertToJSON(OpenAPI_qos_monitoring_report_t *qos_monitoring_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_monitoring_report == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [QosMonitoringReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_monitoring_report->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [flows]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_report->flows, node) {
        cJSON *itemLocal = OpenAPI_flows_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [flows]");
            goto end;
        }
        cJSON_AddItemToArray(flowsList, itemLocal);
    }
    }

    if (qos_monitoring_report->ul_delays) {
    cJSON *ul_delaysList = cJSON_AddArrayToObject(item, "ulDelays");
    if (ul_delaysList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_delays]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_report->ul_delays, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_delays]");
            goto end;
        }
        if (cJSON_AddNumberToObject(ul_delaysList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_delays]");
            goto end;
        }
    }
    }

    if (qos_monitoring_report->dl_delays) {
    cJSON *dl_delaysList = cJSON_AddArrayToObject(item, "dlDelays");
    if (dl_delaysList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_delays]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_report->dl_delays, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_delays]");
            goto end;
        }
        if (cJSON_AddNumberToObject(dl_delaysList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_delays]");
            goto end;
        }
    }
    }

    if (qos_monitoring_report->rt_delays) {
    cJSON *rt_delaysList = cJSON_AddArrayToObject(item, "rtDelays");
    if (rt_delaysList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [rt_delays]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_report->rt_delays, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [rt_delays]");
            goto end;
        }
        if (cJSON_AddNumberToObject(rt_delaysList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [rt_delays]");
            goto end;
        }
    }
    }

    if (qos_monitoring_report->is_pdmf) {
    if (cJSON_AddBoolToObject(item, "pdmf", qos_monitoring_report->pdmf) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [pdmf]");
        goto end;
    }
    }

    if (qos_monitoring_report->ul_con_info) {
    cJSON *ul_con_infoList = cJSON_AddArrayToObject(item, "ulConInfo");
    if (ul_con_infoList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_con_info]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_report->ul_con_info, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_con_info]");
            goto end;
        }
        if (cJSON_AddNumberToObject(ul_con_infoList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_con_info]");
            goto end;
        }
    }
    }

    if (qos_monitoring_report->dl_con_info) {
    cJSON *dl_con_infoList = cJSON_AddArrayToObject(item, "dlConInfo");
    if (dl_con_infoList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_con_info]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_report->dl_con_info, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_con_info]");
            goto end;
        }
        if (cJSON_AddNumberToObject(dl_con_infoList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_con_info]");
            goto end;
        }
    }
    }

    if (qos_monitoring_report->ul_data_rate) {
    if (cJSON_AddStringToObject(item, "ulDataRate", qos_monitoring_report->ul_data_rate) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_data_rate]");
        goto end;
    }
    }

    if (qos_monitoring_report->dl_data_rate) {
    if (cJSON_AddStringToObject(item, "dlDataRate", qos_monitoring_report->dl_data_rate) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_data_rate]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_parseFromJSON(cJSON *qos_monitoring_reportJSON)
{
    OpenAPI_qos_monitoring_report_t *qos_monitoring_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flows = NULL;
    OpenAPI_list_t *flowsList = NULL;
    cJSON *ul_delays = NULL;
    OpenAPI_list_t *ul_delaysList = NULL;
    cJSON *dl_delays = NULL;
    OpenAPI_list_t *dl_delaysList = NULL;
    cJSON *rt_delays = NULL;
    OpenAPI_list_t *rt_delaysList = NULL;
    cJSON *pdmf = NULL;
    cJSON *ul_con_info = NULL;
    OpenAPI_list_t *ul_con_infoList = NULL;
    cJSON *dl_con_info = NULL;
    OpenAPI_list_t *dl_con_infoList = NULL;
    cJSON *ul_data_rate = NULL;
    cJSON *dl_data_rate = NULL;
    flows = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "flows");
    if (flows) {
        cJSON *flows_local = NULL;
        if (!cJSON_IsArray(flows)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [flows]");
            goto end;
        }

        flowsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flows_local, flows) {
            if (!cJSON_IsObject(flows_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [flows]");
                goto end;
            }
            OpenAPI_flows_t *flowsItem = OpenAPI_flows_parseFromJSON(flows_local);
            if (!flowsItem) {
                ogs_error("No flowsItem");
                goto end;
            }
            OpenAPI_list_add(flowsList, flowsItem);
        }
    }

    ul_delays = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "ulDelays");
    if (ul_delays) {
        cJSON *ul_delays_local = NULL;
        if (!cJSON_IsArray(ul_delays)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_delays]");
            goto end;
        }

        ul_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(ul_delays_local, ul_delays) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(ul_delays_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_delays]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_delays]");
                goto end;
            }
            *localDouble = ul_delays_local->valuedouble;
            OpenAPI_list_add(ul_delaysList, localDouble);
        }
    }

    dl_delays = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "dlDelays");
    if (dl_delays) {
        cJSON *dl_delays_local = NULL;
        if (!cJSON_IsArray(dl_delays)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_delays]");
            goto end;
        }

        dl_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(dl_delays_local, dl_delays) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(dl_delays_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_delays]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_delays]");
                goto end;
            }
            *localDouble = dl_delays_local->valuedouble;
            OpenAPI_list_add(dl_delaysList, localDouble);
        }
    }

    rt_delays = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "rtDelays");
    if (rt_delays) {
        cJSON *rt_delays_local = NULL;
        if (!cJSON_IsArray(rt_delays)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [rt_delays]");
            goto end;
        }

        rt_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(rt_delays_local, rt_delays) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(rt_delays_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [rt_delays]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [rt_delays]");
                goto end;
            }
            *localDouble = rt_delays_local->valuedouble;
            OpenAPI_list_add(rt_delaysList, localDouble);
        }
    }

    pdmf = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "pdmf");
    if (pdmf) {
    if (!cJSON_IsBool(pdmf)) {
        ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [pdmf]");
        goto end;
    }
    }

    ul_con_info = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "ulConInfo");
    if (ul_con_info) {
        cJSON *ul_con_info_local = NULL;
        if (!cJSON_IsArray(ul_con_info)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_con_info]");
            goto end;
        }

        ul_con_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(ul_con_info_local, ul_con_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(ul_con_info_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_con_info]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_con_info]");
                goto end;
            }
            *localDouble = ul_con_info_local->valuedouble;
            OpenAPI_list_add(ul_con_infoList, localDouble);
        }
    }

    dl_con_info = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "dlConInfo");
    if (dl_con_info) {
        cJSON *dl_con_info_local = NULL;
        if (!cJSON_IsArray(dl_con_info)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_con_info]");
            goto end;
        }

        dl_con_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(dl_con_info_local, dl_con_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(dl_con_info_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_con_info]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_con_info]");
                goto end;
            }
            *localDouble = dl_con_info_local->valuedouble;
            OpenAPI_list_add(dl_con_infoList, localDouble);
        }
    }

    ul_data_rate = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "ulDataRate");
    if (ul_data_rate) {
    if (!cJSON_IsString(ul_data_rate) && !cJSON_IsNull(ul_data_rate)) {
        ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_data_rate]");
        goto end;
    }
    }

    dl_data_rate = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "dlDataRate");
    if (dl_data_rate) {
    if (!cJSON_IsString(dl_data_rate) && !cJSON_IsNull(dl_data_rate)) {
        ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_data_rate]");
        goto end;
    }
    }

    qos_monitoring_report_local_var = OpenAPI_qos_monitoring_report_create (
        flows ? flowsList : NULL,
        ul_delays ? ul_delaysList : NULL,
        dl_delays ? dl_delaysList : NULL,
        rt_delays ? rt_delaysList : NULL,
        pdmf ? true : false,
        pdmf ? pdmf->valueint : 0,
        ul_con_info ? ul_con_infoList : NULL,
        dl_con_info ? dl_con_infoList : NULL,
        ul_data_rate && !cJSON_IsNull(ul_data_rate) ? ogs_strdup(ul_data_rate->valuestring) : NULL,
        dl_data_rate && !cJSON_IsNull(dl_data_rate) ? ogs_strdup(dl_data_rate->valuestring) : NULL
    );

    return qos_monitoring_report_local_var;
end:
    if (flowsList) {
        OpenAPI_list_for_each(flowsList, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(flowsList);
        flowsList = NULL;
    }
    if (ul_delaysList) {
        OpenAPI_list_for_each(ul_delaysList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ul_delaysList);
        ul_delaysList = NULL;
    }
    if (dl_delaysList) {
        OpenAPI_list_for_each(dl_delaysList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dl_delaysList);
        dl_delaysList = NULL;
    }
    if (rt_delaysList) {
        OpenAPI_list_for_each(rt_delaysList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rt_delaysList);
        rt_delaysList = NULL;
    }
    if (ul_con_infoList) {
        OpenAPI_list_for_each(ul_con_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ul_con_infoList);
        ul_con_infoList = NULL;
    }
    if (dl_con_infoList) {
        OpenAPI_list_for_each(dl_con_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dl_con_infoList);
        dl_con_infoList = NULL;
    }
    return NULL;
}

OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_copy(OpenAPI_qos_monitoring_report_t *dst, OpenAPI_qos_monitoring_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_monitoring_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed");
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

    OpenAPI_qos_monitoring_report_free(dst);
    dst = OpenAPI_qos_monitoring_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

