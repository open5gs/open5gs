
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "datalink_reporting_configuration.h"

OpenAPI_datalink_reporting_configuration_t *OpenAPI_datalink_reporting_configuration_create(
    OpenAPI_list_t *ddd_traffic_des,
    char *dnn,
    OpenAPI_snssai_t *slice,
    OpenAPI_list_t *ddd_status_list
    )
{
    OpenAPI_datalink_reporting_configuration_t *datalink_reporting_configuration_local_var = OpenAPI_malloc(sizeof(OpenAPI_datalink_reporting_configuration_t));
    if (!datalink_reporting_configuration_local_var) {
        return NULL;
    }
    datalink_reporting_configuration_local_var->ddd_traffic_des = ddd_traffic_des;
    datalink_reporting_configuration_local_var->dnn = dnn;
    datalink_reporting_configuration_local_var->slice = slice;
    datalink_reporting_configuration_local_var->ddd_status_list = ddd_status_list;

    return datalink_reporting_configuration_local_var;
}

void OpenAPI_datalink_reporting_configuration_free(OpenAPI_datalink_reporting_configuration_t *datalink_reporting_configuration)
{
    if (NULL == datalink_reporting_configuration) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(datalink_reporting_configuration->ddd_traffic_des, node) {
        OpenAPI_ddd_traffic_descriptor_free(node->data);
    }
    OpenAPI_list_free(datalink_reporting_configuration->ddd_traffic_des);
    ogs_free(datalink_reporting_configuration->dnn);
    OpenAPI_snssai_free(datalink_reporting_configuration->slice);
    OpenAPI_list_for_each(datalink_reporting_configuration->ddd_status_list, node) {
        OpenAPI_dl_data_delivery_status_free(node->data);
    }
    OpenAPI_list_free(datalink_reporting_configuration->ddd_status_list);
    ogs_free(datalink_reporting_configuration);
}

cJSON *OpenAPI_datalink_reporting_configuration_convertToJSON(OpenAPI_datalink_reporting_configuration_t *datalink_reporting_configuration)
{
    cJSON *item = NULL;

    if (datalink_reporting_configuration == NULL) {
        ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed [DatalinkReportingConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (datalink_reporting_configuration->ddd_traffic_des) {
        cJSON *ddd_traffic_desList = cJSON_AddArrayToObject(item, "dddTrafficDes");
        if (ddd_traffic_desList == NULL) {
            ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed [ddd_traffic_des]");
            goto end;
        }

        OpenAPI_lnode_t *ddd_traffic_des_node;
        if (datalink_reporting_configuration->ddd_traffic_des) {
            OpenAPI_list_for_each(datalink_reporting_configuration->ddd_traffic_des, ddd_traffic_des_node) {
                cJSON *itemLocal = OpenAPI_ddd_traffic_descriptor_convertToJSON(ddd_traffic_des_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed [ddd_traffic_des]");
                    goto end;
                }
                cJSON_AddItemToArray(ddd_traffic_desList, itemLocal);
            }
        }
    }

    if (datalink_reporting_configuration->dnn) {
        if (cJSON_AddStringToObject(item, "dnn", datalink_reporting_configuration->dnn) == NULL) {
            ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed [dnn]");
            goto end;
        }
    }

    if (datalink_reporting_configuration->slice) {
        cJSON *slice_local_JSON = OpenAPI_snssai_convertToJSON(datalink_reporting_configuration->slice);
        if (slice_local_JSON == NULL) {
            ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed [slice]");
            goto end;
        }
        cJSON_AddItemToObject(item, "slice", slice_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed [slice]");
            goto end;
        }
    }

    if (datalink_reporting_configuration->ddd_status_list) {
        cJSON *ddd_status_listList = cJSON_AddArrayToObject(item, "dddStatusList");
        if (ddd_status_listList == NULL) {
            ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed [ddd_status_list]");
            goto end;
        }

        OpenAPI_lnode_t *ddd_status_list_node;
        if (datalink_reporting_configuration->ddd_status_list) {
            OpenAPI_list_for_each(datalink_reporting_configuration->ddd_status_list, ddd_status_list_node) {
                cJSON *itemLocal = OpenAPI_dl_data_delivery_status_convertToJSON(ddd_status_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed [ddd_status_list]");
                    goto end;
                }
                cJSON_AddItemToArray(ddd_status_listList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_datalink_reporting_configuration_t *OpenAPI_datalink_reporting_configuration_parseFromJSON(cJSON *datalink_reporting_configurationJSON)
{
    OpenAPI_datalink_reporting_configuration_t *datalink_reporting_configuration_local_var = NULL;
    cJSON *ddd_traffic_des = cJSON_GetObjectItemCaseSensitive(datalink_reporting_configurationJSON, "dddTrafficDes");

    OpenAPI_list_t *ddd_traffic_desList;
    if (ddd_traffic_des) {
        cJSON *ddd_traffic_des_local_nonprimitive;
        if (!cJSON_IsArray(ddd_traffic_des)) {
            ogs_error("OpenAPI_datalink_reporting_configuration_parseFromJSON() failed [ddd_traffic_des]");
            goto end;
        }

        ddd_traffic_desList = OpenAPI_list_create();

        cJSON_ArrayForEach(ddd_traffic_des_local_nonprimitive, ddd_traffic_des ) {
            if (!cJSON_IsObject(ddd_traffic_des_local_nonprimitive)) {
                ogs_error("OpenAPI_datalink_reporting_configuration_parseFromJSON() failed [ddd_traffic_des]");
                goto end;
            }
            OpenAPI_ddd_traffic_descriptor_t *ddd_traffic_desItem = OpenAPI_ddd_traffic_descriptor_parseFromJSON(ddd_traffic_des_local_nonprimitive);

            OpenAPI_list_add(ddd_traffic_desList, ddd_traffic_desItem);
        }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(datalink_reporting_configurationJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_datalink_reporting_configuration_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *slice = cJSON_GetObjectItemCaseSensitive(datalink_reporting_configurationJSON, "slice");

    OpenAPI_snssai_t *slice_local_nonprim = NULL;
    if (slice) {
        slice_local_nonprim = OpenAPI_snssai_parseFromJSON(slice);
    }

    cJSON *ddd_status_list = cJSON_GetObjectItemCaseSensitive(datalink_reporting_configurationJSON, "dddStatusList");

    OpenAPI_list_t *ddd_status_listList;
    if (ddd_status_list) {
        cJSON *ddd_status_list_local_nonprimitive;
        if (!cJSON_IsArray(ddd_status_list)) {
            ogs_error("OpenAPI_datalink_reporting_configuration_parseFromJSON() failed [ddd_status_list]");
            goto end;
        }

        ddd_status_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ddd_status_list_local_nonprimitive, ddd_status_list ) {
            if (!cJSON_IsObject(ddd_status_list_local_nonprimitive)) {
                ogs_error("OpenAPI_datalink_reporting_configuration_parseFromJSON() failed [ddd_status_list]");
                goto end;
            }
            OpenAPI_dl_data_delivery_status_t *ddd_status_listItem = OpenAPI_dl_data_delivery_status_parseFromJSON(ddd_status_list_local_nonprimitive);

            OpenAPI_list_add(ddd_status_listList, ddd_status_listItem);
        }
    }

    datalink_reporting_configuration_local_var = OpenAPI_datalink_reporting_configuration_create (
        ddd_traffic_des ? ddd_traffic_desList : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        slice ? slice_local_nonprim : NULL,
        ddd_status_list ? ddd_status_listList : NULL
        );

    return datalink_reporting_configuration_local_var;
end:
    return NULL;
}

OpenAPI_datalink_reporting_configuration_t *OpenAPI_datalink_reporting_configuration_copy(OpenAPI_datalink_reporting_configuration_t *dst, OpenAPI_datalink_reporting_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_datalink_reporting_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_datalink_reporting_configuration_convertToJSON() failed");
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

    OpenAPI_datalink_reporting_configuration_free(dst);
    dst = OpenAPI_datalink_reporting_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

