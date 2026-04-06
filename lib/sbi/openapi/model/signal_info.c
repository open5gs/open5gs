
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "signal_info.h"

OpenAPI_signal_info_t *OpenAPI_signal_info_create(
    OpenAPI_list_t *impact_ref_points,
    OpenAPI_list_t *impact_srv_ops,
    OpenAPI_list_t *sig_analytics
)
{
    OpenAPI_signal_info_t *signal_info_local_var = ogs_malloc(sizeof(OpenAPI_signal_info_t));
    ogs_assert(signal_info_local_var);

    signal_info_local_var->impact_ref_points = impact_ref_points;
    signal_info_local_var->impact_srv_ops = impact_srv_ops;
    signal_info_local_var->sig_analytics = sig_analytics;

    return signal_info_local_var;
}

void OpenAPI_signal_info_free(OpenAPI_signal_info_t *signal_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == signal_info) {
        return;
    }
    if (signal_info->impact_ref_points) {
        OpenAPI_list_for_each(signal_info->impact_ref_points, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_info->impact_ref_points);
        signal_info->impact_ref_points = NULL;
    }
    if (signal_info->impact_srv_ops) {
        OpenAPI_list_for_each(signal_info->impact_srv_ops, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_info->impact_srv_ops);
        signal_info->impact_srv_ops = NULL;
    }
    if (signal_info->sig_analytics) {
        OpenAPI_list_for_each(signal_info->sig_analytics, node) {
            OpenAPI_signal_analytics_free(node->data);
        }
        OpenAPI_list_free(signal_info->sig_analytics);
        signal_info->sig_analytics = NULL;
    }
    ogs_free(signal_info);
}

cJSON *OpenAPI_signal_info_convertToJSON(OpenAPI_signal_info_t *signal_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (signal_info == NULL) {
        ogs_error("OpenAPI_signal_info_convertToJSON() failed [SignalInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (signal_info->impact_ref_points) {
    cJSON *impact_ref_pointsList = cJSON_AddArrayToObject(item, "impactRefPoints");
    if (impact_ref_pointsList == NULL) {
        ogs_error("OpenAPI_signal_info_convertToJSON() failed [impact_ref_points]");
        goto end;
    }
    OpenAPI_list_for_each(signal_info->impact_ref_points, node) {
        if (cJSON_AddStringToObject(impact_ref_pointsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_info_convertToJSON() failed [impact_ref_points]");
            goto end;
        }
    }
    }

    if (signal_info->impact_srv_ops) {
    cJSON *impact_srv_opsList = cJSON_AddArrayToObject(item, "impactSrvOps");
    if (impact_srv_opsList == NULL) {
        ogs_error("OpenAPI_signal_info_convertToJSON() failed [impact_srv_ops]");
        goto end;
    }
    OpenAPI_list_for_each(signal_info->impact_srv_ops, node) {
        if (cJSON_AddStringToObject(impact_srv_opsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_info_convertToJSON() failed [impact_srv_ops]");
            goto end;
        }
    }
    }

    if (signal_info->sig_analytics) {
    cJSON *sig_analyticsList = cJSON_AddArrayToObject(item, "sigAnalytics");
    if (sig_analyticsList == NULL) {
        ogs_error("OpenAPI_signal_info_convertToJSON() failed [sig_analytics]");
        goto end;
    }
    OpenAPI_list_for_each(signal_info->sig_analytics, node) {
        cJSON *itemLocal = OpenAPI_signal_analytics_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_signal_info_convertToJSON() failed [sig_analytics]");
            goto end;
        }
        cJSON_AddItemToArray(sig_analyticsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_signal_info_t *OpenAPI_signal_info_parseFromJSON(cJSON *signal_infoJSON)
{
    OpenAPI_signal_info_t *signal_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *impact_ref_points = NULL;
    OpenAPI_list_t *impact_ref_pointsList = NULL;
    cJSON *impact_srv_ops = NULL;
    OpenAPI_list_t *impact_srv_opsList = NULL;
    cJSON *sig_analytics = NULL;
    OpenAPI_list_t *sig_analyticsList = NULL;
    impact_ref_points = cJSON_GetObjectItemCaseSensitive(signal_infoJSON, "impactRefPoints");
    if (impact_ref_points) {
        cJSON *impact_ref_points_local = NULL;
        if (!cJSON_IsArray(impact_ref_points)) {
            ogs_error("OpenAPI_signal_info_parseFromJSON() failed [impact_ref_points]");
            goto end;
        }

        impact_ref_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(impact_ref_points_local, impact_ref_points) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(impact_ref_points_local)) {
                ogs_error("OpenAPI_signal_info_parseFromJSON() failed [impact_ref_points]");
                goto end;
            }
            OpenAPI_list_add(impact_ref_pointsList, ogs_strdup(impact_ref_points_local->valuestring));
        }
    }

    impact_srv_ops = cJSON_GetObjectItemCaseSensitive(signal_infoJSON, "impactSrvOps");
    if (impact_srv_ops) {
        cJSON *impact_srv_ops_local = NULL;
        if (!cJSON_IsArray(impact_srv_ops)) {
            ogs_error("OpenAPI_signal_info_parseFromJSON() failed [impact_srv_ops]");
            goto end;
        }

        impact_srv_opsList = OpenAPI_list_create();

        cJSON_ArrayForEach(impact_srv_ops_local, impact_srv_ops) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(impact_srv_ops_local)) {
                ogs_error("OpenAPI_signal_info_parseFromJSON() failed [impact_srv_ops]");
                goto end;
            }
            OpenAPI_list_add(impact_srv_opsList, ogs_strdup(impact_srv_ops_local->valuestring));
        }
    }

    sig_analytics = cJSON_GetObjectItemCaseSensitive(signal_infoJSON, "sigAnalytics");
    if (sig_analytics) {
        cJSON *sig_analytics_local = NULL;
        if (!cJSON_IsArray(sig_analytics)) {
            ogs_error("OpenAPI_signal_info_parseFromJSON() failed [sig_analytics]");
            goto end;
        }

        sig_analyticsList = OpenAPI_list_create();

        cJSON_ArrayForEach(sig_analytics_local, sig_analytics) {
            if (!cJSON_IsObject(sig_analytics_local)) {
                ogs_error("OpenAPI_signal_info_parseFromJSON() failed [sig_analytics]");
                goto end;
            }
            OpenAPI_signal_analytics_t *sig_analyticsItem = OpenAPI_signal_analytics_parseFromJSON(sig_analytics_local);
            if (!sig_analyticsItem) {
                ogs_error("No sig_analyticsItem");
                goto end;
            }
            OpenAPI_list_add(sig_analyticsList, sig_analyticsItem);
        }
    }

    signal_info_local_var = OpenAPI_signal_info_create (
        impact_ref_points ? impact_ref_pointsList : NULL,
        impact_srv_ops ? impact_srv_opsList : NULL,
        sig_analytics ? sig_analyticsList : NULL
    );

    return signal_info_local_var;
end:
    if (impact_ref_pointsList) {
        OpenAPI_list_for_each(impact_ref_pointsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(impact_ref_pointsList);
        impact_ref_pointsList = NULL;
    }
    if (impact_srv_opsList) {
        OpenAPI_list_for_each(impact_srv_opsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(impact_srv_opsList);
        impact_srv_opsList = NULL;
    }
    if (sig_analyticsList) {
        OpenAPI_list_for_each(sig_analyticsList, node) {
            OpenAPI_signal_analytics_free(node->data);
        }
        OpenAPI_list_free(sig_analyticsList);
        sig_analyticsList = NULL;
    }
    return NULL;
}

OpenAPI_signal_info_t *OpenAPI_signal_info_copy(OpenAPI_signal_info_t *dst, OpenAPI_signal_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_signal_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_signal_info_convertToJSON() failed");
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

    OpenAPI_signal_info_free(dst);
    dst = OpenAPI_signal_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

