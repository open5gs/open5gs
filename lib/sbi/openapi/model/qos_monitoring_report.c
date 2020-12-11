
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_report.h"

OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_create(
    OpenAPI_list_t *ref_pcc_rule_ids,
    OpenAPI_list_t *ul_delays,
    OpenAPI_list_t *dl_delays,
    OpenAPI_list_t *rt_delays
    )
{
    OpenAPI_qos_monitoring_report_t *qos_monitoring_report_local_var = OpenAPI_malloc(sizeof(OpenAPI_qos_monitoring_report_t));
    if (!qos_monitoring_report_local_var) {
        return NULL;
    }
    qos_monitoring_report_local_var->ref_pcc_rule_ids = ref_pcc_rule_ids;
    qos_monitoring_report_local_var->ul_delays = ul_delays;
    qos_monitoring_report_local_var->dl_delays = dl_delays;
    qos_monitoring_report_local_var->rt_delays = rt_delays;

    return qos_monitoring_report_local_var;
}

void OpenAPI_qos_monitoring_report_free(OpenAPI_qos_monitoring_report_t *qos_monitoring_report)
{
    if (NULL == qos_monitoring_report) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(qos_monitoring_report->ref_pcc_rule_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(qos_monitoring_report->ref_pcc_rule_ids);
    OpenAPI_list_for_each(qos_monitoring_report->ul_delays, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(qos_monitoring_report->ul_delays);
    OpenAPI_list_for_each(qos_monitoring_report->dl_delays, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(qos_monitoring_report->dl_delays);
    OpenAPI_list_for_each(qos_monitoring_report->rt_delays, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(qos_monitoring_report->rt_delays);
    ogs_free(qos_monitoring_report);
}

cJSON *OpenAPI_qos_monitoring_report_convertToJSON(OpenAPI_qos_monitoring_report_t *qos_monitoring_report)
{
    cJSON *item = NULL;

    if (qos_monitoring_report == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [QosMonitoringReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!qos_monitoring_report->ref_pcc_rule_ids) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    cJSON *ref_pcc_rule_ids = cJSON_AddArrayToObject(item, "refPccRuleIds");
    if (ref_pcc_rule_ids == NULL) {
        ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ref_pcc_rule_ids_node;
    OpenAPI_list_for_each(qos_monitoring_report->ref_pcc_rule_ids, ref_pcc_rule_ids_node)  {
        if (cJSON_AddStringToObject(ref_pcc_rule_ids, "", (char*)ref_pcc_rule_ids_node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
    }

    if (qos_monitoring_report->ul_delays) {
        cJSON *ul_delays = cJSON_AddArrayToObject(item, "ulDelays");
        if (ul_delays == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_delays]");
            goto end;
        }

        OpenAPI_lnode_t *ul_delays_node;
        OpenAPI_list_for_each(qos_monitoring_report->ul_delays, ul_delays_node)  {
            if (cJSON_AddNumberToObject(ul_delays, "", *(double *)ul_delays_node->data) == NULL) {
                ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [ul_delays]");
                goto end;
            }
        }
    }

    if (qos_monitoring_report->dl_delays) {
        cJSON *dl_delays = cJSON_AddArrayToObject(item, "dlDelays");
        if (dl_delays == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_delays]");
            goto end;
        }

        OpenAPI_lnode_t *dl_delays_node;
        OpenAPI_list_for_each(qos_monitoring_report->dl_delays, dl_delays_node)  {
            if (cJSON_AddNumberToObject(dl_delays, "", *(double *)dl_delays_node->data) == NULL) {
                ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [dl_delays]");
                goto end;
            }
        }
    }

    if (qos_monitoring_report->rt_delays) {
        cJSON *rt_delays = cJSON_AddArrayToObject(item, "rtDelays");
        if (rt_delays == NULL) {
            ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [rt_delays]");
            goto end;
        }

        OpenAPI_lnode_t *rt_delays_node;
        OpenAPI_list_for_each(qos_monitoring_report->rt_delays, rt_delays_node)  {
            if (cJSON_AddNumberToObject(rt_delays, "", *(double *)rt_delays_node->data) == NULL) {
                ogs_error("OpenAPI_qos_monitoring_report_convertToJSON() failed [rt_delays]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_parseFromJSON(cJSON *qos_monitoring_reportJSON)
{
    OpenAPI_qos_monitoring_report_t *qos_monitoring_report_local_var = NULL;
    cJSON *ref_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "refPccRuleIds");
    if (!ref_pcc_rule_ids) {
        ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }

    OpenAPI_list_t *ref_pcc_rule_idsList;

    cJSON *ref_pcc_rule_ids_local;
    if (!cJSON_IsArray(ref_pcc_rule_ids)) {
        ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    ref_pcc_rule_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ref_pcc_rule_ids_local, ref_pcc_rule_ids) {
        if (!cJSON_IsString(ref_pcc_rule_ids_local)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
        OpenAPI_list_add(ref_pcc_rule_idsList, ogs_strdup(ref_pcc_rule_ids_local->valuestring));
    }

    cJSON *ul_delays = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "ulDelays");

    OpenAPI_list_t *ul_delaysList;
    if (ul_delays) {
        cJSON *ul_delays_local;
        if (!cJSON_IsArray(ul_delays)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_delays]");
            goto end;
        }
        ul_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(ul_delays_local, ul_delays) {
            if (!cJSON_IsNumber(ul_delays_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [ul_delays]");
                goto end;
            }
            OpenAPI_list_add(ul_delaysList, &ul_delays_local->valuedouble);
        }
    }

    cJSON *dl_delays = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "dlDelays");

    OpenAPI_list_t *dl_delaysList;
    if (dl_delays) {
        cJSON *dl_delays_local;
        if (!cJSON_IsArray(dl_delays)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_delays]");
            goto end;
        }
        dl_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(dl_delays_local, dl_delays) {
            if (!cJSON_IsNumber(dl_delays_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [dl_delays]");
                goto end;
            }
            OpenAPI_list_add(dl_delaysList, &dl_delays_local->valuedouble);
        }
    }

    cJSON *rt_delays = cJSON_GetObjectItemCaseSensitive(qos_monitoring_reportJSON, "rtDelays");

    OpenAPI_list_t *rt_delaysList;
    if (rt_delays) {
        cJSON *rt_delays_local;
        if (!cJSON_IsArray(rt_delays)) {
            ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [rt_delays]");
            goto end;
        }
        rt_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(rt_delays_local, rt_delays) {
            if (!cJSON_IsNumber(rt_delays_local)) {
                ogs_error("OpenAPI_qos_monitoring_report_parseFromJSON() failed [rt_delays]");
                goto end;
            }
            OpenAPI_list_add(rt_delaysList, &rt_delays_local->valuedouble);
        }
    }

    qos_monitoring_report_local_var = OpenAPI_qos_monitoring_report_create (
        ref_pcc_rule_idsList,
        ul_delays ? ul_delaysList : NULL,
        dl_delays ? dl_delaysList : NULL,
        rt_delays ? rt_delaysList : NULL
        );

    return qos_monitoring_report_local_var;
end:
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

