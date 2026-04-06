
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non3gpp_device_information.h"

OpenAPI_non3gpp_device_information_t *OpenAPI_non3gpp_device_information_create(
    char *non3gpp_dev_id,
    OpenAPI_dnn_snssai_information_t *dnn_snssai_info,
    OpenAPI_list_t *flow_infos,
    OpenAPI_list_t *eth_flow_infos,
    char *qos_reference,
    OpenAPI_qos_parameter_set_t *ind_qos_param_set
)
{
    OpenAPI_non3gpp_device_information_t *non3gpp_device_information_local_var = ogs_malloc(sizeof(OpenAPI_non3gpp_device_information_t));
    ogs_assert(non3gpp_device_information_local_var);

    non3gpp_device_information_local_var->non3gpp_dev_id = non3gpp_dev_id;
    non3gpp_device_information_local_var->dnn_snssai_info = dnn_snssai_info;
    non3gpp_device_information_local_var->flow_infos = flow_infos;
    non3gpp_device_information_local_var->eth_flow_infos = eth_flow_infos;
    non3gpp_device_information_local_var->qos_reference = qos_reference;
    non3gpp_device_information_local_var->ind_qos_param_set = ind_qos_param_set;

    return non3gpp_device_information_local_var;
}

void OpenAPI_non3gpp_device_information_free(OpenAPI_non3gpp_device_information_t *non3gpp_device_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == non3gpp_device_information) {
        return;
    }
    if (non3gpp_device_information->non3gpp_dev_id) {
        ogs_free(non3gpp_device_information->non3gpp_dev_id);
        non3gpp_device_information->non3gpp_dev_id = NULL;
    }
    if (non3gpp_device_information->dnn_snssai_info) {
        OpenAPI_dnn_snssai_information_free(non3gpp_device_information->dnn_snssai_info);
        non3gpp_device_information->dnn_snssai_info = NULL;
    }
    if (non3gpp_device_information->flow_infos) {
        OpenAPI_list_for_each(non3gpp_device_information->flow_infos, node) {
            OpenAPI_flow_info_1_free(node->data);
        }
        OpenAPI_list_free(non3gpp_device_information->flow_infos);
        non3gpp_device_information->flow_infos = NULL;
    }
    if (non3gpp_device_information->eth_flow_infos) {
        OpenAPI_list_for_each(non3gpp_device_information->eth_flow_infos, node) {
            OpenAPI_eth_flow_info_free(node->data);
        }
        OpenAPI_list_free(non3gpp_device_information->eth_flow_infos);
        non3gpp_device_information->eth_flow_infos = NULL;
    }
    if (non3gpp_device_information->qos_reference) {
        ogs_free(non3gpp_device_information->qos_reference);
        non3gpp_device_information->qos_reference = NULL;
    }
    if (non3gpp_device_information->ind_qos_param_set) {
        OpenAPI_qos_parameter_set_free(non3gpp_device_information->ind_qos_param_set);
        non3gpp_device_information->ind_qos_param_set = NULL;
    }
    ogs_free(non3gpp_device_information);
}

cJSON *OpenAPI_non3gpp_device_information_convertToJSON(OpenAPI_non3gpp_device_information_t *non3gpp_device_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (non3gpp_device_information == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [Non3gppDeviceInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!non3gpp_device_information->non3gpp_dev_id) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [non3gpp_dev_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "non3gppDevId", non3gpp_device_information->non3gpp_dev_id) == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [non3gpp_dev_id]");
        goto end;
    }

    if (non3gpp_device_information->dnn_snssai_info) {
    cJSON *dnn_snssai_info_local_JSON = OpenAPI_dnn_snssai_information_convertToJSON(non3gpp_device_information->dnn_snssai_info);
    if (dnn_snssai_info_local_JSON == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [dnn_snssai_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnnSnssaiInfo", dnn_snssai_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [dnn_snssai_info]");
        goto end;
    }
    }

    if (non3gpp_device_information->flow_infos) {
    cJSON *flow_infosList = cJSON_AddArrayToObject(item, "flowInfos");
    if (flow_infosList == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [flow_infos]");
        goto end;
    }
    OpenAPI_list_for_each(non3gpp_device_information->flow_infos, node) {
        cJSON *itemLocal = OpenAPI_flow_info_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [flow_infos]");
            goto end;
        }
        cJSON_AddItemToArray(flow_infosList, itemLocal);
    }
    }

    if (non3gpp_device_information->eth_flow_infos) {
    cJSON *eth_flow_infosList = cJSON_AddArrayToObject(item, "ethFlowInfos");
    if (eth_flow_infosList == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [eth_flow_infos]");
        goto end;
    }
    OpenAPI_list_for_each(non3gpp_device_information->eth_flow_infos, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [eth_flow_infos]");
            goto end;
        }
        cJSON_AddItemToArray(eth_flow_infosList, itemLocal);
    }
    }

    if (non3gpp_device_information->qos_reference) {
    if (cJSON_AddStringToObject(item, "qosReference", non3gpp_device_information->qos_reference) == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [qos_reference]");
        goto end;
    }
    }

    if (non3gpp_device_information->ind_qos_param_set) {
    cJSON *ind_qos_param_set_local_JSON = OpenAPI_qos_parameter_set_convertToJSON(non3gpp_device_information->ind_qos_param_set);
    if (ind_qos_param_set_local_JSON == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [ind_qos_param_set]");
        goto end;
    }
    cJSON_AddItemToObject(item, "indQosParamSet", ind_qos_param_set_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed [ind_qos_param_set]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_non3gpp_device_information_t *OpenAPI_non3gpp_device_information_parseFromJSON(cJSON *non3gpp_device_informationJSON)
{
    OpenAPI_non3gpp_device_information_t *non3gpp_device_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *non3gpp_dev_id = NULL;
    cJSON *dnn_snssai_info = NULL;
    OpenAPI_dnn_snssai_information_t *dnn_snssai_info_local_nonprim = NULL;
    cJSON *flow_infos = NULL;
    OpenAPI_list_t *flow_infosList = NULL;
    cJSON *eth_flow_infos = NULL;
    OpenAPI_list_t *eth_flow_infosList = NULL;
    cJSON *qos_reference = NULL;
    cJSON *ind_qos_param_set = NULL;
    OpenAPI_qos_parameter_set_t *ind_qos_param_set_local_nonprim = NULL;
    non3gpp_dev_id = cJSON_GetObjectItemCaseSensitive(non3gpp_device_informationJSON, "non3gppDevId");
    if (!non3gpp_dev_id) {
        ogs_error("OpenAPI_non3gpp_device_information_parseFromJSON() failed [non3gpp_dev_id]");
        goto end;
    }
    if (!cJSON_IsString(non3gpp_dev_id)) {
        ogs_error("OpenAPI_non3gpp_device_information_parseFromJSON() failed [non3gpp_dev_id]");
        goto end;
    }

    dnn_snssai_info = cJSON_GetObjectItemCaseSensitive(non3gpp_device_informationJSON, "dnnSnssaiInfo");
    if (dnn_snssai_info) {
    dnn_snssai_info_local_nonprim = OpenAPI_dnn_snssai_information_parseFromJSON(dnn_snssai_info);
    if (!dnn_snssai_info_local_nonprim) {
        ogs_error("OpenAPI_dnn_snssai_information_parseFromJSON failed [dnn_snssai_info]");
        goto end;
    }
    }

    flow_infos = cJSON_GetObjectItemCaseSensitive(non3gpp_device_informationJSON, "flowInfos");
    if (flow_infos) {
        cJSON *flow_infos_local = NULL;
        if (!cJSON_IsArray(flow_infos)) {
            ogs_error("OpenAPI_non3gpp_device_information_parseFromJSON() failed [flow_infos]");
            goto end;
        }

        flow_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_infos_local, flow_infos) {
            if (!cJSON_IsObject(flow_infos_local)) {
                ogs_error("OpenAPI_non3gpp_device_information_parseFromJSON() failed [flow_infos]");
                goto end;
            }
            OpenAPI_flow_info_1_t *flow_infosItem = OpenAPI_flow_info_1_parseFromJSON(flow_infos_local);
            if (!flow_infosItem) {
                ogs_error("No flow_infosItem");
                goto end;
            }
            OpenAPI_list_add(flow_infosList, flow_infosItem);
        }
    }

    eth_flow_infos = cJSON_GetObjectItemCaseSensitive(non3gpp_device_informationJSON, "ethFlowInfos");
    if (eth_flow_infos) {
        cJSON *eth_flow_infos_local = NULL;
        if (!cJSON_IsArray(eth_flow_infos)) {
            ogs_error("OpenAPI_non3gpp_device_information_parseFromJSON() failed [eth_flow_infos]");
            goto end;
        }

        eth_flow_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_flow_infos_local, eth_flow_infos) {
            if (!cJSON_IsObject(eth_flow_infos_local)) {
                ogs_error("OpenAPI_non3gpp_device_information_parseFromJSON() failed [eth_flow_infos]");
                goto end;
            }
            OpenAPI_eth_flow_info_t *eth_flow_infosItem = OpenAPI_eth_flow_info_parseFromJSON(eth_flow_infos_local);
            if (!eth_flow_infosItem) {
                ogs_error("No eth_flow_infosItem");
                goto end;
            }
            OpenAPI_list_add(eth_flow_infosList, eth_flow_infosItem);
        }
    }

    qos_reference = cJSON_GetObjectItemCaseSensitive(non3gpp_device_informationJSON, "qosReference");
    if (qos_reference) {
    if (!cJSON_IsString(qos_reference) && !cJSON_IsNull(qos_reference)) {
        ogs_error("OpenAPI_non3gpp_device_information_parseFromJSON() failed [qos_reference]");
        goto end;
    }
    }

    ind_qos_param_set = cJSON_GetObjectItemCaseSensitive(non3gpp_device_informationJSON, "indQosParamSet");
    if (ind_qos_param_set) {
    ind_qos_param_set_local_nonprim = OpenAPI_qos_parameter_set_parseFromJSON(ind_qos_param_set);
    if (!ind_qos_param_set_local_nonprim) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON failed [ind_qos_param_set]");
        goto end;
    }
    }

    non3gpp_device_information_local_var = OpenAPI_non3gpp_device_information_create (
        ogs_strdup(non3gpp_dev_id->valuestring),
        dnn_snssai_info ? dnn_snssai_info_local_nonprim : NULL,
        flow_infos ? flow_infosList : NULL,
        eth_flow_infos ? eth_flow_infosList : NULL,
        qos_reference && !cJSON_IsNull(qos_reference) ? ogs_strdup(qos_reference->valuestring) : NULL,
        ind_qos_param_set ? ind_qos_param_set_local_nonprim : NULL
    );

    return non3gpp_device_information_local_var;
end:
    if (dnn_snssai_info_local_nonprim) {
        OpenAPI_dnn_snssai_information_free(dnn_snssai_info_local_nonprim);
        dnn_snssai_info_local_nonprim = NULL;
    }
    if (flow_infosList) {
        OpenAPI_list_for_each(flow_infosList, node) {
            OpenAPI_flow_info_1_free(node->data);
        }
        OpenAPI_list_free(flow_infosList);
        flow_infosList = NULL;
    }
    if (eth_flow_infosList) {
        OpenAPI_list_for_each(eth_flow_infosList, node) {
            OpenAPI_eth_flow_info_free(node->data);
        }
        OpenAPI_list_free(eth_flow_infosList);
        eth_flow_infosList = NULL;
    }
    if (ind_qos_param_set_local_nonprim) {
        OpenAPI_qos_parameter_set_free(ind_qos_param_set_local_nonprim);
        ind_qos_param_set_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_non3gpp_device_information_t *OpenAPI_non3gpp_device_information_copy(OpenAPI_non3gpp_device_information_t *dst, OpenAPI_non3gpp_device_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non3gpp_device_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non3gpp_device_information_convertToJSON() failed");
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

    OpenAPI_non3gpp_device_information_free(dst);
    dst = OpenAPI_non3gpp_device_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

