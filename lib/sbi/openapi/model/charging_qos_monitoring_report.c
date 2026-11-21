
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_qos_monitoring_report.h"

OpenAPI_charging_qos_monitoring_report_t *OpenAPI_charging_qos_monitoring_report_create(
    OpenAPI_list_t *ul_delays,
    OpenAPI_list_t *dl_delays,
    OpenAPI_list_t *rt_delays
)
{
    OpenAPI_charging_qos_monitoring_report_t *charging_qos_monitoring_report_local_var = ogs_malloc(sizeof(OpenAPI_charging_qos_monitoring_report_t));
    ogs_assert(charging_qos_monitoring_report_local_var);

    charging_qos_monitoring_report_local_var->ul_delays = ul_delays;
    charging_qos_monitoring_report_local_var->dl_delays = dl_delays;
    charging_qos_monitoring_report_local_var->rt_delays = rt_delays;

    return charging_qos_monitoring_report_local_var;
}

void OpenAPI_charging_qos_monitoring_report_free(OpenAPI_charging_qos_monitoring_report_t *charging_qos_monitoring_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_qos_monitoring_report) {
        return;
    }
    if (charging_qos_monitoring_report->ul_delays) {
        OpenAPI_list_for_each(charging_qos_monitoring_report->ul_delays, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(charging_qos_monitoring_report->ul_delays);
        charging_qos_monitoring_report->ul_delays = NULL;
    }
    if (charging_qos_monitoring_report->dl_delays) {
        OpenAPI_list_for_each(charging_qos_monitoring_report->dl_delays, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(charging_qos_monitoring_report->dl_delays);
        charging_qos_monitoring_report->dl_delays = NULL;
    }
    if (charging_qos_monitoring_report->rt_delays) {
        OpenAPI_list_for_each(charging_qos_monitoring_report->rt_delays, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(charging_qos_monitoring_report->rt_delays);
        charging_qos_monitoring_report->rt_delays = NULL;
    }
    ogs_free(charging_qos_monitoring_report);
}

cJSON *OpenAPI_charging_qos_monitoring_report_convertToJSON(OpenAPI_charging_qos_monitoring_report_t *charging_qos_monitoring_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_qos_monitoring_report == NULL) {
        ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [ChargingQosMonitoringReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_qos_monitoring_report->ul_delays) {
    cJSON *ul_delaysList = cJSON_AddArrayToObject(item, "ulDelays");
    if (ul_delaysList == NULL) {
        ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [ul_delays]");
        goto end;
    }
    OpenAPI_list_for_each(charging_qos_monitoring_report->ul_delays, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [ul_delays]");
            goto end;
        }
        if (cJSON_AddNumberToObject(ul_delaysList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [ul_delays]");
            goto end;
        }
    }
    }

    if (charging_qos_monitoring_report->dl_delays) {
    cJSON *dl_delaysList = cJSON_AddArrayToObject(item, "dlDelays");
    if (dl_delaysList == NULL) {
        ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [dl_delays]");
        goto end;
    }
    OpenAPI_list_for_each(charging_qos_monitoring_report->dl_delays, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [dl_delays]");
            goto end;
        }
        if (cJSON_AddNumberToObject(dl_delaysList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [dl_delays]");
            goto end;
        }
    }
    }

    if (charging_qos_monitoring_report->rt_delays) {
    cJSON *rt_delaysList = cJSON_AddArrayToObject(item, "rtDelays");
    if (rt_delaysList == NULL) {
        ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [rt_delays]");
        goto end;
    }
    OpenAPI_list_for_each(charging_qos_monitoring_report->rt_delays, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [rt_delays]");
            goto end;
        }
        if (cJSON_AddNumberToObject(rt_delaysList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed [rt_delays]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_charging_qos_monitoring_report_t *OpenAPI_charging_qos_monitoring_report_parseFromJSON(cJSON *charging_qos_monitoring_reportJSON)
{
    OpenAPI_charging_qos_monitoring_report_t *charging_qos_monitoring_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ul_delays = NULL;
    OpenAPI_list_t *ul_delaysList = NULL;
    cJSON *dl_delays = NULL;
    OpenAPI_list_t *dl_delaysList = NULL;
    cJSON *rt_delays = NULL;
    OpenAPI_list_t *rt_delaysList = NULL;
    ul_delays = cJSON_GetObjectItemCaseSensitive(charging_qos_monitoring_reportJSON, "ulDelays");
    if (ul_delays) {
        cJSON *ul_delays_local = NULL;
        if (!cJSON_IsArray(ul_delays)) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [ul_delays]");
            goto end;
        }

        ul_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(ul_delays_local, ul_delays) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(ul_delays_local)) {
                ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [ul_delays]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [ul_delays]");
                goto end;
            }
            *localDouble = ul_delays_local->valuedouble;
            OpenAPI_list_add(ul_delaysList, localDouble);
        }
    }

    dl_delays = cJSON_GetObjectItemCaseSensitive(charging_qos_monitoring_reportJSON, "dlDelays");
    if (dl_delays) {
        cJSON *dl_delays_local = NULL;
        if (!cJSON_IsArray(dl_delays)) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [dl_delays]");
            goto end;
        }

        dl_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(dl_delays_local, dl_delays) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(dl_delays_local)) {
                ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [dl_delays]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [dl_delays]");
                goto end;
            }
            *localDouble = dl_delays_local->valuedouble;
            OpenAPI_list_add(dl_delaysList, localDouble);
        }
    }

    rt_delays = cJSON_GetObjectItemCaseSensitive(charging_qos_monitoring_reportJSON, "rtDelays");
    if (rt_delays) {
        cJSON *rt_delays_local = NULL;
        if (!cJSON_IsArray(rt_delays)) {
            ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [rt_delays]");
            goto end;
        }

        rt_delaysList = OpenAPI_list_create();

        cJSON_ArrayForEach(rt_delays_local, rt_delays) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(rt_delays_local)) {
                ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [rt_delays]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_charging_qos_monitoring_report_parseFromJSON() failed [rt_delays]");
                goto end;
            }
            *localDouble = rt_delays_local->valuedouble;
            OpenAPI_list_add(rt_delaysList, localDouble);
        }
    }

    charging_qos_monitoring_report_local_var = OpenAPI_charging_qos_monitoring_report_create (
        ul_delays ? ul_delaysList : NULL,
        dl_delays ? dl_delaysList : NULL,
        rt_delays ? rt_delaysList : NULL
    );

    return charging_qos_monitoring_report_local_var;
end:
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
    return NULL;
}

OpenAPI_charging_qos_monitoring_report_t *OpenAPI_charging_qos_monitoring_report_copy(OpenAPI_charging_qos_monitoring_report_t *dst, OpenAPI_charging_qos_monitoring_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_qos_monitoring_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_qos_monitoring_report_convertToJSON() failed");
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

    OpenAPI_charging_qos_monitoring_report_free(dst);
    dst = OpenAPI_charging_qos_monitoring_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

