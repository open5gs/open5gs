
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_influence_data.h"

OpenAPI_traffic_influence_data_t *OpenAPI_traffic_influence_data_create(
    OpenAPI_list_t *flow_infos,
    char *app_id,
    bool is_precedence,
    int precedence,
    OpenAPI_list_t *ref_tc_data
)
{
    OpenAPI_traffic_influence_data_t *traffic_influence_data_local_var = ogs_malloc(sizeof(OpenAPI_traffic_influence_data_t));
    ogs_assert(traffic_influence_data_local_var);

    traffic_influence_data_local_var->flow_infos = flow_infos;
    traffic_influence_data_local_var->app_id = app_id;
    traffic_influence_data_local_var->is_precedence = is_precedence;
    traffic_influence_data_local_var->precedence = precedence;
    traffic_influence_data_local_var->ref_tc_data = ref_tc_data;

    return traffic_influence_data_local_var;
}

void OpenAPI_traffic_influence_data_free(OpenAPI_traffic_influence_data_t *traffic_influence_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_influence_data) {
        return;
    }
    if (traffic_influence_data->flow_infos) {
        OpenAPI_list_for_each(traffic_influence_data->flow_infos, node) {
            OpenAPI_flow_information_free(node->data);
        }
        OpenAPI_list_free(traffic_influence_data->flow_infos);
        traffic_influence_data->flow_infos = NULL;
    }
    if (traffic_influence_data->app_id) {
        ogs_free(traffic_influence_data->app_id);
        traffic_influence_data->app_id = NULL;
    }
    if (traffic_influence_data->ref_tc_data) {
        OpenAPI_list_for_each(traffic_influence_data->ref_tc_data, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influence_data->ref_tc_data);
        traffic_influence_data->ref_tc_data = NULL;
    }
    ogs_free(traffic_influence_data);
}

cJSON *OpenAPI_traffic_influence_data_convertToJSON(OpenAPI_traffic_influence_data_t *traffic_influence_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_influence_data == NULL) {
        ogs_error("OpenAPI_traffic_influence_data_convertToJSON() failed [TrafficInfluenceData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_influence_data->flow_infos) {
    cJSON *flow_infosList = cJSON_AddArrayToObject(item, "flowInfos");
    if (flow_infosList == NULL) {
        ogs_error("OpenAPI_traffic_influence_data_convertToJSON() failed [flow_infos]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influence_data->flow_infos, node) {
        cJSON *itemLocal = OpenAPI_flow_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influence_data_convertToJSON() failed [flow_infos]");
            goto end;
        }
        cJSON_AddItemToArray(flow_infosList, itemLocal);
    }
    }

    if (traffic_influence_data->app_id) {
    if (cJSON_AddStringToObject(item, "appId", traffic_influence_data->app_id) == NULL) {
        ogs_error("OpenAPI_traffic_influence_data_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (traffic_influence_data->is_precedence) {
    if (cJSON_AddNumberToObject(item, "precedence", traffic_influence_data->precedence) == NULL) {
        ogs_error("OpenAPI_traffic_influence_data_convertToJSON() failed [precedence]");
        goto end;
    }
    }

    if (traffic_influence_data->ref_tc_data) {
    cJSON *ref_tc_dataList = cJSON_AddArrayToObject(item, "refTcData");
    if (ref_tc_dataList == NULL) {
        ogs_error("OpenAPI_traffic_influence_data_convertToJSON() failed [ref_tc_data]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influence_data->ref_tc_data, node) {
        if (cJSON_AddStringToObject(ref_tc_dataList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influence_data_convertToJSON() failed [ref_tc_data]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_traffic_influence_data_t *OpenAPI_traffic_influence_data_parseFromJSON(cJSON *traffic_influence_dataJSON)
{
    OpenAPI_traffic_influence_data_t *traffic_influence_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flow_infos = NULL;
    OpenAPI_list_t *flow_infosList = NULL;
    cJSON *app_id = NULL;
    cJSON *precedence = NULL;
    cJSON *ref_tc_data = NULL;
    OpenAPI_list_t *ref_tc_dataList = NULL;
    flow_infos = cJSON_GetObjectItemCaseSensitive(traffic_influence_dataJSON, "flowInfos");
    if (flow_infos) {
        cJSON *flow_infos_local = NULL;
        if (!cJSON_IsArray(flow_infos)) {
            ogs_error("OpenAPI_traffic_influence_data_parseFromJSON() failed [flow_infos]");
            goto end;
        }

        flow_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_infos_local, flow_infos) {
            if (!cJSON_IsObject(flow_infos_local)) {
                ogs_error("OpenAPI_traffic_influence_data_parseFromJSON() failed [flow_infos]");
                goto end;
            }
            OpenAPI_flow_information_t *flow_infosItem = OpenAPI_flow_information_parseFromJSON(flow_infos_local);
            if (!flow_infosItem) {
                ogs_error("No flow_infosItem");
                goto end;
            }
            OpenAPI_list_add(flow_infosList, flow_infosItem);
        }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(traffic_influence_dataJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_traffic_influence_data_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    precedence = cJSON_GetObjectItemCaseSensitive(traffic_influence_dataJSON, "precedence");
    if (precedence) {
    if (!cJSON_IsNumber(precedence)) {
        ogs_error("OpenAPI_traffic_influence_data_parseFromJSON() failed [precedence]");
        goto end;
    }
    }

    ref_tc_data = cJSON_GetObjectItemCaseSensitive(traffic_influence_dataJSON, "refTcData");
    if (ref_tc_data) {
        cJSON *ref_tc_data_local = NULL;
        if (!cJSON_IsArray(ref_tc_data)) {
            ogs_error("OpenAPI_traffic_influence_data_parseFromJSON() failed [ref_tc_data]");
            goto end;
        }

        ref_tc_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_tc_data_local, ref_tc_data) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_tc_data_local)) {
                ogs_error("OpenAPI_traffic_influence_data_parseFromJSON() failed [ref_tc_data]");
                goto end;
            }
            OpenAPI_list_add(ref_tc_dataList, ogs_strdup(ref_tc_data_local->valuestring));
        }
    }

    traffic_influence_data_local_var = OpenAPI_traffic_influence_data_create (
        flow_infos ? flow_infosList : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        precedence ? true : false,
        precedence ? precedence->valuedouble : 0,
        ref_tc_data ? ref_tc_dataList : NULL
    );

    return traffic_influence_data_local_var;
end:
    if (flow_infosList) {
        OpenAPI_list_for_each(flow_infosList, node) {
            OpenAPI_flow_information_free(node->data);
        }
        OpenAPI_list_free(flow_infosList);
        flow_infosList = NULL;
    }
    if (ref_tc_dataList) {
        OpenAPI_list_for_each(ref_tc_dataList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_tc_dataList);
        ref_tc_dataList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_influence_data_t *OpenAPI_traffic_influence_data_copy(OpenAPI_traffic_influence_data_t *dst, OpenAPI_traffic_influence_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_influence_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_influence_data_convertToJSON() failed");
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

    OpenAPI_traffic_influence_data_free(dst);
    dst = OpenAPI_traffic_influence_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

