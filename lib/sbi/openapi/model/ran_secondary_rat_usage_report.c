
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ran_secondary_rat_usage_report.h"

OpenAPI_ran_secondary_rat_usage_report_t *OpenAPI_ran_secondary_rat_usage_report_create(
    OpenAPI_rat_type_e r_an_secondary_rat_type,
    OpenAPI_list_t *qos_flows_usage_reports
)
{
    OpenAPI_ran_secondary_rat_usage_report_t *ran_secondary_rat_usage_report_local_var = ogs_malloc(sizeof(OpenAPI_ran_secondary_rat_usage_report_t));
    ogs_assert(ran_secondary_rat_usage_report_local_var);

    ran_secondary_rat_usage_report_local_var->r_an_secondary_rat_type = r_an_secondary_rat_type;
    ran_secondary_rat_usage_report_local_var->qos_flows_usage_reports = qos_flows_usage_reports;

    return ran_secondary_rat_usage_report_local_var;
}

void OpenAPI_ran_secondary_rat_usage_report_free(OpenAPI_ran_secondary_rat_usage_report_t *ran_secondary_rat_usage_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ran_secondary_rat_usage_report) {
        return;
    }
    if (ran_secondary_rat_usage_report->qos_flows_usage_reports) {
        OpenAPI_list_for_each(ran_secondary_rat_usage_report->qos_flows_usage_reports, node) {
            OpenAPI_qos_flows_usage_report_free(node->data);
        }
        OpenAPI_list_free(ran_secondary_rat_usage_report->qos_flows_usage_reports);
        ran_secondary_rat_usage_report->qos_flows_usage_reports = NULL;
    }
    ogs_free(ran_secondary_rat_usage_report);
}

cJSON *OpenAPI_ran_secondary_rat_usage_report_convertToJSON(OpenAPI_ran_secondary_rat_usage_report_t *ran_secondary_rat_usage_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ran_secondary_rat_usage_report == NULL) {
        ogs_error("OpenAPI_ran_secondary_rat_usage_report_convertToJSON() failed [RANSecondaryRATUsageReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ran_secondary_rat_usage_report->r_an_secondary_rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "rANSecondaryRATType", OpenAPI_rat_type_ToString(ran_secondary_rat_usage_report->r_an_secondary_rat_type)) == NULL) {
        ogs_error("OpenAPI_ran_secondary_rat_usage_report_convertToJSON() failed [r_an_secondary_rat_type]");
        goto end;
    }
    }

    if (ran_secondary_rat_usage_report->qos_flows_usage_reports) {
    cJSON *qos_flows_usage_reportsList = cJSON_AddArrayToObject(item, "qosFlowsUsageReports");
    if (qos_flows_usage_reportsList == NULL) {
        ogs_error("OpenAPI_ran_secondary_rat_usage_report_convertToJSON() failed [qos_flows_usage_reports]");
        goto end;
    }
    OpenAPI_list_for_each(ran_secondary_rat_usage_report->qos_flows_usage_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_flows_usage_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ran_secondary_rat_usage_report_convertToJSON() failed [qos_flows_usage_reports]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flows_usage_reportsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_ran_secondary_rat_usage_report_t *OpenAPI_ran_secondary_rat_usage_report_parseFromJSON(cJSON *ran_secondary_rat_usage_reportJSON)
{
    OpenAPI_ran_secondary_rat_usage_report_t *ran_secondary_rat_usage_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *r_an_secondary_rat_type = NULL;
    OpenAPI_rat_type_e r_an_secondary_rat_typeVariable = 0;
    cJSON *qos_flows_usage_reports = NULL;
    OpenAPI_list_t *qos_flows_usage_reportsList = NULL;
    r_an_secondary_rat_type = cJSON_GetObjectItemCaseSensitive(ran_secondary_rat_usage_reportJSON, "rANSecondaryRATType");
    if (r_an_secondary_rat_type) {
    if (!cJSON_IsString(r_an_secondary_rat_type)) {
        ogs_error("OpenAPI_ran_secondary_rat_usage_report_parseFromJSON() failed [r_an_secondary_rat_type]");
        goto end;
    }
    r_an_secondary_rat_typeVariable = OpenAPI_rat_type_FromString(r_an_secondary_rat_type->valuestring);
    }

    qos_flows_usage_reports = cJSON_GetObjectItemCaseSensitive(ran_secondary_rat_usage_reportJSON, "qosFlowsUsageReports");
    if (qos_flows_usage_reports) {
        cJSON *qos_flows_usage_reports_local = NULL;
        if (!cJSON_IsArray(qos_flows_usage_reports)) {
            ogs_error("OpenAPI_ran_secondary_rat_usage_report_parseFromJSON() failed [qos_flows_usage_reports]");
            goto end;
        }

        qos_flows_usage_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_usage_reports_local, qos_flows_usage_reports) {
            if (!cJSON_IsObject(qos_flows_usage_reports_local)) {
                ogs_error("OpenAPI_ran_secondary_rat_usage_report_parseFromJSON() failed [qos_flows_usage_reports]");
                goto end;
            }
            OpenAPI_qos_flows_usage_report_t *qos_flows_usage_reportsItem = OpenAPI_qos_flows_usage_report_parseFromJSON(qos_flows_usage_reports_local);
            if (!qos_flows_usage_reportsItem) {
                ogs_error("No qos_flows_usage_reportsItem");
                goto end;
            }
            OpenAPI_list_add(qos_flows_usage_reportsList, qos_flows_usage_reportsItem);
        }
    }

    ran_secondary_rat_usage_report_local_var = OpenAPI_ran_secondary_rat_usage_report_create (
        r_an_secondary_rat_type ? r_an_secondary_rat_typeVariable : 0,
        qos_flows_usage_reports ? qos_flows_usage_reportsList : NULL
    );

    return ran_secondary_rat_usage_report_local_var;
end:
    if (qos_flows_usage_reportsList) {
        OpenAPI_list_for_each(qos_flows_usage_reportsList, node) {
            OpenAPI_qos_flows_usage_report_free(node->data);
        }
        OpenAPI_list_free(qos_flows_usage_reportsList);
        qos_flows_usage_reportsList = NULL;
    }
    return NULL;
}

OpenAPI_ran_secondary_rat_usage_report_t *OpenAPI_ran_secondary_rat_usage_report_copy(OpenAPI_ran_secondary_rat_usage_report_t *dst, OpenAPI_ran_secondary_rat_usage_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ran_secondary_rat_usage_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ran_secondary_rat_usage_report_convertToJSON() failed");
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

    OpenAPI_ran_secondary_rat_usage_report_free(dst);
    dst = OpenAPI_ran_secondary_rat_usage_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

