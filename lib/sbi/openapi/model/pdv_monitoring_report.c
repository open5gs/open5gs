
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdv_monitoring_report.h"

OpenAPI_pdv_monitoring_report_t *OpenAPI_pdv_monitoring_report_create(
    OpenAPI_list_t *flows,
    bool is_ul_pdv,
    int ul_pdv,
    bool is_dl_pdv,
    int dl_pdv,
    bool is_rt_pdv,
    int rt_pdv
)
{
    OpenAPI_pdv_monitoring_report_t *pdv_monitoring_report_local_var = ogs_malloc(sizeof(OpenAPI_pdv_monitoring_report_t));
    ogs_assert(pdv_monitoring_report_local_var);

    pdv_monitoring_report_local_var->flows = flows;
    pdv_monitoring_report_local_var->is_ul_pdv = is_ul_pdv;
    pdv_monitoring_report_local_var->ul_pdv = ul_pdv;
    pdv_monitoring_report_local_var->is_dl_pdv = is_dl_pdv;
    pdv_monitoring_report_local_var->dl_pdv = dl_pdv;
    pdv_monitoring_report_local_var->is_rt_pdv = is_rt_pdv;
    pdv_monitoring_report_local_var->rt_pdv = rt_pdv;

    return pdv_monitoring_report_local_var;
}

void OpenAPI_pdv_monitoring_report_free(OpenAPI_pdv_monitoring_report_t *pdv_monitoring_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdv_monitoring_report) {
        return;
    }
    if (pdv_monitoring_report->flows) {
        OpenAPI_list_for_each(pdv_monitoring_report->flows, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(pdv_monitoring_report->flows);
        pdv_monitoring_report->flows = NULL;
    }
    ogs_free(pdv_monitoring_report);
}

cJSON *OpenAPI_pdv_monitoring_report_convertToJSON(OpenAPI_pdv_monitoring_report_t *pdv_monitoring_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdv_monitoring_report == NULL) {
        ogs_error("OpenAPI_pdv_monitoring_report_convertToJSON() failed [PdvMonitoringReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdv_monitoring_report->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_pdv_monitoring_report_convertToJSON() failed [flows]");
        goto end;
    }
    OpenAPI_list_for_each(pdv_monitoring_report->flows, node) {
        cJSON *itemLocal = OpenAPI_flows_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdv_monitoring_report_convertToJSON() failed [flows]");
            goto end;
        }
        cJSON_AddItemToArray(flowsList, itemLocal);
    }
    }

    if (pdv_monitoring_report->is_ul_pdv) {
    if (cJSON_AddNumberToObject(item, "ulPdv", pdv_monitoring_report->ul_pdv) == NULL) {
        ogs_error("OpenAPI_pdv_monitoring_report_convertToJSON() failed [ul_pdv]");
        goto end;
    }
    }

    if (pdv_monitoring_report->is_dl_pdv) {
    if (cJSON_AddNumberToObject(item, "dlPdv", pdv_monitoring_report->dl_pdv) == NULL) {
        ogs_error("OpenAPI_pdv_monitoring_report_convertToJSON() failed [dl_pdv]");
        goto end;
    }
    }

    if (pdv_monitoring_report->is_rt_pdv) {
    if (cJSON_AddNumberToObject(item, "rtPdv", pdv_monitoring_report->rt_pdv) == NULL) {
        ogs_error("OpenAPI_pdv_monitoring_report_convertToJSON() failed [rt_pdv]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdv_monitoring_report_t *OpenAPI_pdv_monitoring_report_parseFromJSON(cJSON *pdv_monitoring_reportJSON)
{
    OpenAPI_pdv_monitoring_report_t *pdv_monitoring_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flows = NULL;
    OpenAPI_list_t *flowsList = NULL;
    cJSON *ul_pdv = NULL;
    cJSON *dl_pdv = NULL;
    cJSON *rt_pdv = NULL;
    flows = cJSON_GetObjectItemCaseSensitive(pdv_monitoring_reportJSON, "flows");
    if (flows) {
        cJSON *flows_local = NULL;
        if (!cJSON_IsArray(flows)) {
            ogs_error("OpenAPI_pdv_monitoring_report_parseFromJSON() failed [flows]");
            goto end;
        }

        flowsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flows_local, flows) {
            if (!cJSON_IsObject(flows_local)) {
                ogs_error("OpenAPI_pdv_monitoring_report_parseFromJSON() failed [flows]");
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

    ul_pdv = cJSON_GetObjectItemCaseSensitive(pdv_monitoring_reportJSON, "ulPdv");
    if (ul_pdv) {
    if (!cJSON_IsNumber(ul_pdv)) {
        ogs_error("OpenAPI_pdv_monitoring_report_parseFromJSON() failed [ul_pdv]");
        goto end;
    }
    }

    dl_pdv = cJSON_GetObjectItemCaseSensitive(pdv_monitoring_reportJSON, "dlPdv");
    if (dl_pdv) {
    if (!cJSON_IsNumber(dl_pdv)) {
        ogs_error("OpenAPI_pdv_monitoring_report_parseFromJSON() failed [dl_pdv]");
        goto end;
    }
    }

    rt_pdv = cJSON_GetObjectItemCaseSensitive(pdv_monitoring_reportJSON, "rtPdv");
    if (rt_pdv) {
    if (!cJSON_IsNumber(rt_pdv)) {
        ogs_error("OpenAPI_pdv_monitoring_report_parseFromJSON() failed [rt_pdv]");
        goto end;
    }
    }

    pdv_monitoring_report_local_var = OpenAPI_pdv_monitoring_report_create (
        flows ? flowsList : NULL,
        ul_pdv ? true : false,
        ul_pdv ? ul_pdv->valuedouble : 0,
        dl_pdv ? true : false,
        dl_pdv ? dl_pdv->valuedouble : 0,
        rt_pdv ? true : false,
        rt_pdv ? rt_pdv->valuedouble : 0
    );

    return pdv_monitoring_report_local_var;
end:
    if (flowsList) {
        OpenAPI_list_for_each(flowsList, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(flowsList);
        flowsList = NULL;
    }
    return NULL;
}

OpenAPI_pdv_monitoring_report_t *OpenAPI_pdv_monitoring_report_copy(OpenAPI_pdv_monitoring_report_t *dst, OpenAPI_pdv_monitoring_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdv_monitoring_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdv_monitoring_report_convertToJSON() failed");
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

    OpenAPI_pdv_monitoring_report_free(dst);
    dst = OpenAPI_pdv_monitoring_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

