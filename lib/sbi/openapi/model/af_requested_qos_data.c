
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_requested_qos_data.h"

OpenAPI_af_requested_qos_data_t *OpenAPI_af_requested_qos_data_create(
    char *supi,
    char *inter_group_id,
    char *af_app_id,
    char *dnn,
    OpenAPI_snssai_t *slice_info,
    OpenAPI_list_t *flow_info,
    OpenAPI_list_t *eth_flow_info,
    OpenAPI_list_t *en_eth_flow_info,
    OpenAPI_events_subsc_req_data_t *ev_subsc,
    char *qos_reference,
    OpenAPI_qos_requirements_t *qos_reqs,
    OpenAPI_list_t *alt_ser_reqs,
    OpenAPI_list_t *alt_ser_reqs_data,
    bool is_dis_ue_notif,
    int dis_ue_notif,
    OpenAPI_temporal_in_validity_t *temp_in_validity,
    OpenAPI_list_t *headers,
    char *supp_feat
)
{
    OpenAPI_af_requested_qos_data_t *af_requested_qos_data_local_var = ogs_malloc(sizeof(OpenAPI_af_requested_qos_data_t));
    ogs_assert(af_requested_qos_data_local_var);

    af_requested_qos_data_local_var->supi = supi;
    af_requested_qos_data_local_var->inter_group_id = inter_group_id;
    af_requested_qos_data_local_var->af_app_id = af_app_id;
    af_requested_qos_data_local_var->dnn = dnn;
    af_requested_qos_data_local_var->slice_info = slice_info;
    af_requested_qos_data_local_var->flow_info = flow_info;
    af_requested_qos_data_local_var->eth_flow_info = eth_flow_info;
    af_requested_qos_data_local_var->en_eth_flow_info = en_eth_flow_info;
    af_requested_qos_data_local_var->ev_subsc = ev_subsc;
    af_requested_qos_data_local_var->qos_reference = qos_reference;
    af_requested_qos_data_local_var->qos_reqs = qos_reqs;
    af_requested_qos_data_local_var->alt_ser_reqs = alt_ser_reqs;
    af_requested_qos_data_local_var->alt_ser_reqs_data = alt_ser_reqs_data;
    af_requested_qos_data_local_var->is_dis_ue_notif = is_dis_ue_notif;
    af_requested_qos_data_local_var->dis_ue_notif = dis_ue_notif;
    af_requested_qos_data_local_var->temp_in_validity = temp_in_validity;
    af_requested_qos_data_local_var->headers = headers;
    af_requested_qos_data_local_var->supp_feat = supp_feat;

    return af_requested_qos_data_local_var;
}

void OpenAPI_af_requested_qos_data_free(OpenAPI_af_requested_qos_data_t *af_requested_qos_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_requested_qos_data) {
        return;
    }
    if (af_requested_qos_data->supi) {
        ogs_free(af_requested_qos_data->supi);
        af_requested_qos_data->supi = NULL;
    }
    if (af_requested_qos_data->inter_group_id) {
        ogs_free(af_requested_qos_data->inter_group_id);
        af_requested_qos_data->inter_group_id = NULL;
    }
    if (af_requested_qos_data->af_app_id) {
        ogs_free(af_requested_qos_data->af_app_id);
        af_requested_qos_data->af_app_id = NULL;
    }
    if (af_requested_qos_data->dnn) {
        ogs_free(af_requested_qos_data->dnn);
        af_requested_qos_data->dnn = NULL;
    }
    if (af_requested_qos_data->slice_info) {
        OpenAPI_snssai_free(af_requested_qos_data->slice_info);
        af_requested_qos_data->slice_info = NULL;
    }
    if (af_requested_qos_data->flow_info) {
        OpenAPI_list_for_each(af_requested_qos_data->flow_info, node) {
            OpenAPI_flow_info_1_free(node->data);
        }
        OpenAPI_list_free(af_requested_qos_data->flow_info);
        af_requested_qos_data->flow_info = NULL;
    }
    if (af_requested_qos_data->eth_flow_info) {
        OpenAPI_list_for_each(af_requested_qos_data->eth_flow_info, node) {
            OpenAPI_eth_flow_description_1_free(node->data);
        }
        OpenAPI_list_free(af_requested_qos_data->eth_flow_info);
        af_requested_qos_data->eth_flow_info = NULL;
    }
    if (af_requested_qos_data->en_eth_flow_info) {
        OpenAPI_list_for_each(af_requested_qos_data->en_eth_flow_info, node) {
            OpenAPI_eth_flow_info_free(node->data);
        }
        OpenAPI_list_free(af_requested_qos_data->en_eth_flow_info);
        af_requested_qos_data->en_eth_flow_info = NULL;
    }
    if (af_requested_qos_data->ev_subsc) {
        OpenAPI_events_subsc_req_data_free(af_requested_qos_data->ev_subsc);
        af_requested_qos_data->ev_subsc = NULL;
    }
    if (af_requested_qos_data->qos_reference) {
        ogs_free(af_requested_qos_data->qos_reference);
        af_requested_qos_data->qos_reference = NULL;
    }
    if (af_requested_qos_data->qos_reqs) {
        OpenAPI_qos_requirements_free(af_requested_qos_data->qos_reqs);
        af_requested_qos_data->qos_reqs = NULL;
    }
    if (af_requested_qos_data->alt_ser_reqs) {
        OpenAPI_list_for_each(af_requested_qos_data->alt_ser_reqs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_requested_qos_data->alt_ser_reqs);
        af_requested_qos_data->alt_ser_reqs = NULL;
    }
    if (af_requested_qos_data->alt_ser_reqs_data) {
        OpenAPI_list_for_each(af_requested_qos_data->alt_ser_reqs_data, node) {
            OpenAPI_alternative_service_requirements_data_free(node->data);
        }
        OpenAPI_list_free(af_requested_qos_data->alt_ser_reqs_data);
        af_requested_qos_data->alt_ser_reqs_data = NULL;
    }
    if (af_requested_qos_data->temp_in_validity) {
        OpenAPI_temporal_in_validity_free(af_requested_qos_data->temp_in_validity);
        af_requested_qos_data->temp_in_validity = NULL;
    }
    if (af_requested_qos_data->headers) {
        OpenAPI_list_for_each(af_requested_qos_data->headers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_requested_qos_data->headers);
        af_requested_qos_data->headers = NULL;
    }
    if (af_requested_qos_data->supp_feat) {
        ogs_free(af_requested_qos_data->supp_feat);
        af_requested_qos_data->supp_feat = NULL;
    }
    ogs_free(af_requested_qos_data);
}

cJSON *OpenAPI_af_requested_qos_data_convertToJSON(OpenAPI_af_requested_qos_data_t *af_requested_qos_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_requested_qos_data == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [AfRequestedQosData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (af_requested_qos_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", af_requested_qos_data->supi) == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (af_requested_qos_data->inter_group_id) {
    if (cJSON_AddStringToObject(item, "interGroupId", af_requested_qos_data->inter_group_id) == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [inter_group_id]");
        goto end;
    }
    }

    if (af_requested_qos_data->af_app_id) {
    if (cJSON_AddStringToObject(item, "afAppId", af_requested_qos_data->af_app_id) == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [af_app_id]");
        goto end;
    }
    }

    if (af_requested_qos_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", af_requested_qos_data->dnn) == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (af_requested_qos_data->slice_info) {
    cJSON *slice_info_local_JSON = OpenAPI_snssai_convertToJSON(af_requested_qos_data->slice_info);
    if (slice_info_local_JSON == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [slice_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sliceInfo", slice_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [slice_info]");
        goto end;
    }
    }

    if (af_requested_qos_data->flow_info) {
    cJSON *flow_infoList = cJSON_AddArrayToObject(item, "flowInfo");
    if (flow_infoList == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [flow_info]");
        goto end;
    }
    OpenAPI_list_for_each(af_requested_qos_data->flow_info, node) {
        cJSON *itemLocal = OpenAPI_flow_info_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [flow_info]");
            goto end;
        }
        cJSON_AddItemToArray(flow_infoList, itemLocal);
    }
    }

    if (af_requested_qos_data->eth_flow_info) {
    cJSON *eth_flow_infoList = cJSON_AddArrayToObject(item, "ethFlowInfo");
    if (eth_flow_infoList == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [eth_flow_info]");
        goto end;
    }
    OpenAPI_list_for_each(af_requested_qos_data->eth_flow_info, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [eth_flow_info]");
            goto end;
        }
        cJSON_AddItemToArray(eth_flow_infoList, itemLocal);
    }
    }

    if (af_requested_qos_data->en_eth_flow_info) {
    cJSON *en_eth_flow_infoList = cJSON_AddArrayToObject(item, "enEthFlowInfo");
    if (en_eth_flow_infoList == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [en_eth_flow_info]");
        goto end;
    }
    OpenAPI_list_for_each(af_requested_qos_data->en_eth_flow_info, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [en_eth_flow_info]");
            goto end;
        }
        cJSON_AddItemToArray(en_eth_flow_infoList, itemLocal);
    }
    }

    if (af_requested_qos_data->ev_subsc) {
    cJSON *ev_subsc_local_JSON = OpenAPI_events_subsc_req_data_convertToJSON(af_requested_qos_data->ev_subsc);
    if (ev_subsc_local_JSON == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [ev_subsc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "evSubsc", ev_subsc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [ev_subsc]");
        goto end;
    }
    }

    if (af_requested_qos_data->qos_reference) {
    if (cJSON_AddStringToObject(item, "qosReference", af_requested_qos_data->qos_reference) == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [qos_reference]");
        goto end;
    }
    }

    if (af_requested_qos_data->qos_reqs) {
    cJSON *qos_reqs_local_JSON = OpenAPI_qos_requirements_convertToJSON(af_requested_qos_data->qos_reqs);
    if (qos_reqs_local_JSON == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [qos_reqs]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosReqs", qos_reqs_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [qos_reqs]");
        goto end;
    }
    }

    if (af_requested_qos_data->alt_ser_reqs) {
    cJSON *alt_ser_reqsList = cJSON_AddArrayToObject(item, "altSerReqs");
    if (alt_ser_reqsList == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [alt_ser_reqs]");
        goto end;
    }
    OpenAPI_list_for_each(af_requested_qos_data->alt_ser_reqs, node) {
        if (cJSON_AddStringToObject(alt_ser_reqsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [alt_ser_reqs]");
            goto end;
        }
    }
    }

    if (af_requested_qos_data->alt_ser_reqs_data) {
    cJSON *alt_ser_reqs_dataList = cJSON_AddArrayToObject(item, "altSerReqsData");
    if (alt_ser_reqs_dataList == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [alt_ser_reqs_data]");
        goto end;
    }
    OpenAPI_list_for_each(af_requested_qos_data->alt_ser_reqs_data, node) {
        cJSON *itemLocal = OpenAPI_alternative_service_requirements_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [alt_ser_reqs_data]");
            goto end;
        }
        cJSON_AddItemToArray(alt_ser_reqs_dataList, itemLocal);
    }
    }

    if (af_requested_qos_data->is_dis_ue_notif) {
    if (cJSON_AddBoolToObject(item, "disUeNotif", af_requested_qos_data->dis_ue_notif) == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [dis_ue_notif]");
        goto end;
    }
    }

    if (af_requested_qos_data->temp_in_validity) {
    cJSON *temp_in_validity_local_JSON = OpenAPI_temporal_in_validity_convertToJSON(af_requested_qos_data->temp_in_validity);
    if (temp_in_validity_local_JSON == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [temp_in_validity]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tempInValidity", temp_in_validity_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [temp_in_validity]");
        goto end;
    }
    }

    if (af_requested_qos_data->headers) {
    cJSON *headersList = cJSON_AddArrayToObject(item, "headers");
    if (headersList == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [headers]");
        goto end;
    }
    OpenAPI_list_for_each(af_requested_qos_data->headers, node) {
        if (cJSON_AddStringToObject(headersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [headers]");
            goto end;
        }
    }
    }

    if (af_requested_qos_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", af_requested_qos_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_af_requested_qos_data_t *OpenAPI_af_requested_qos_data_parseFromJSON(cJSON *af_requested_qos_dataJSON)
{
    OpenAPI_af_requested_qos_data_t *af_requested_qos_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *inter_group_id = NULL;
    cJSON *af_app_id = NULL;
    cJSON *dnn = NULL;
    cJSON *slice_info = NULL;
    OpenAPI_snssai_t *slice_info_local_nonprim = NULL;
    cJSON *flow_info = NULL;
    OpenAPI_list_t *flow_infoList = NULL;
    cJSON *eth_flow_info = NULL;
    OpenAPI_list_t *eth_flow_infoList = NULL;
    cJSON *en_eth_flow_info = NULL;
    OpenAPI_list_t *en_eth_flow_infoList = NULL;
    cJSON *ev_subsc = NULL;
    OpenAPI_events_subsc_req_data_t *ev_subsc_local_nonprim = NULL;
    cJSON *qos_reference = NULL;
    cJSON *qos_reqs = NULL;
    OpenAPI_qos_requirements_t *qos_reqs_local_nonprim = NULL;
    cJSON *alt_ser_reqs = NULL;
    OpenAPI_list_t *alt_ser_reqsList = NULL;
    cJSON *alt_ser_reqs_data = NULL;
    OpenAPI_list_t *alt_ser_reqs_dataList = NULL;
    cJSON *dis_ue_notif = NULL;
    cJSON *temp_in_validity = NULL;
    OpenAPI_temporal_in_validity_t *temp_in_validity_local_nonprim = NULL;
    cJSON *headers = NULL;
    OpenAPI_list_t *headersList = NULL;
    cJSON *supp_feat = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    inter_group_id = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "interGroupId");
    if (inter_group_id) {
    if (!cJSON_IsString(inter_group_id) && !cJSON_IsNull(inter_group_id)) {
        ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [inter_group_id]");
        goto end;
    }
    }

    af_app_id = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "afAppId");
    if (af_app_id) {
    if (!cJSON_IsString(af_app_id) && !cJSON_IsNull(af_app_id)) {
        ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [af_app_id]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    slice_info = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "sliceInfo");
    if (slice_info) {
    slice_info_local_nonprim = OpenAPI_snssai_parseFromJSON(slice_info);
    if (!slice_info_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [slice_info]");
        goto end;
    }
    }

    flow_info = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "flowInfo");
    if (flow_info) {
        cJSON *flow_info_local = NULL;
        if (!cJSON_IsArray(flow_info)) {
            ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [flow_info]");
            goto end;
        }

        flow_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_info_local, flow_info) {
            if (!cJSON_IsObject(flow_info_local)) {
                ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [flow_info]");
                goto end;
            }
            OpenAPI_flow_info_1_t *flow_infoItem = OpenAPI_flow_info_1_parseFromJSON(flow_info_local);
            if (!flow_infoItem) {
                ogs_error("No flow_infoItem");
                goto end;
            }
            OpenAPI_list_add(flow_infoList, flow_infoItem);
        }
    }

    eth_flow_info = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "ethFlowInfo");
    if (eth_flow_info) {
        cJSON *eth_flow_info_local = NULL;
        if (!cJSON_IsArray(eth_flow_info)) {
            ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [eth_flow_info]");
            goto end;
        }

        eth_flow_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_flow_info_local, eth_flow_info) {
            if (!cJSON_IsObject(eth_flow_info_local)) {
                ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [eth_flow_info]");
                goto end;
            }
            OpenAPI_eth_flow_description_1_t *eth_flow_infoItem = OpenAPI_eth_flow_description_1_parseFromJSON(eth_flow_info_local);
            if (!eth_flow_infoItem) {
                ogs_error("No eth_flow_infoItem");
                goto end;
            }
            OpenAPI_list_add(eth_flow_infoList, eth_flow_infoItem);
        }
    }

    en_eth_flow_info = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "enEthFlowInfo");
    if (en_eth_flow_info) {
        cJSON *en_eth_flow_info_local = NULL;
        if (!cJSON_IsArray(en_eth_flow_info)) {
            ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [en_eth_flow_info]");
            goto end;
        }

        en_eth_flow_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(en_eth_flow_info_local, en_eth_flow_info) {
            if (!cJSON_IsObject(en_eth_flow_info_local)) {
                ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [en_eth_flow_info]");
                goto end;
            }
            OpenAPI_eth_flow_info_t *en_eth_flow_infoItem = OpenAPI_eth_flow_info_parseFromJSON(en_eth_flow_info_local);
            if (!en_eth_flow_infoItem) {
                ogs_error("No en_eth_flow_infoItem");
                goto end;
            }
            OpenAPI_list_add(en_eth_flow_infoList, en_eth_flow_infoItem);
        }
    }

    ev_subsc = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "evSubsc");
    if (ev_subsc) {
    ev_subsc_local_nonprim = OpenAPI_events_subsc_req_data_parseFromJSON(ev_subsc);
    if (!ev_subsc_local_nonprim) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON failed [ev_subsc]");
        goto end;
    }
    }

    qos_reference = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "qosReference");
    if (qos_reference) {
    if (!cJSON_IsString(qos_reference) && !cJSON_IsNull(qos_reference)) {
        ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [qos_reference]");
        goto end;
    }
    }

    qos_reqs = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "qosReqs");
    if (qos_reqs) {
    qos_reqs_local_nonprim = OpenAPI_qos_requirements_parseFromJSON(qos_reqs);
    if (!qos_reqs_local_nonprim) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON failed [qos_reqs]");
        goto end;
    }
    }

    alt_ser_reqs = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "altSerReqs");
    if (alt_ser_reqs) {
        cJSON *alt_ser_reqs_local = NULL;
        if (!cJSON_IsArray(alt_ser_reqs)) {
            ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [alt_ser_reqs]");
            goto end;
        }

        alt_ser_reqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_ser_reqs_local, alt_ser_reqs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(alt_ser_reqs_local)) {
                ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [alt_ser_reqs]");
                goto end;
            }
            OpenAPI_list_add(alt_ser_reqsList, ogs_strdup(alt_ser_reqs_local->valuestring));
        }
    }

    alt_ser_reqs_data = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "altSerReqsData");
    if (alt_ser_reqs_data) {
        cJSON *alt_ser_reqs_data_local = NULL;
        if (!cJSON_IsArray(alt_ser_reqs_data)) {
            ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [alt_ser_reqs_data]");
            goto end;
        }

        alt_ser_reqs_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_ser_reqs_data_local, alt_ser_reqs_data) {
            if (!cJSON_IsObject(alt_ser_reqs_data_local)) {
                ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [alt_ser_reqs_data]");
                goto end;
            }
            OpenAPI_alternative_service_requirements_data_t *alt_ser_reqs_dataItem = OpenAPI_alternative_service_requirements_data_parseFromJSON(alt_ser_reqs_data_local);
            if (!alt_ser_reqs_dataItem) {
                ogs_error("No alt_ser_reqs_dataItem");
                goto end;
            }
            OpenAPI_list_add(alt_ser_reqs_dataList, alt_ser_reqs_dataItem);
        }
    }

    dis_ue_notif = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "disUeNotif");
    if (dis_ue_notif) {
    if (!cJSON_IsBool(dis_ue_notif)) {
        ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [dis_ue_notif]");
        goto end;
    }
    }

    temp_in_validity = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "tempInValidity");
    if (temp_in_validity) {
    temp_in_validity_local_nonprim = OpenAPI_temporal_in_validity_parseFromJSON(temp_in_validity);
    if (!temp_in_validity_local_nonprim) {
        ogs_error("OpenAPI_temporal_in_validity_parseFromJSON failed [temp_in_validity]");
        goto end;
    }
    }

    headers = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "headers");
    if (headers) {
        cJSON *headers_local = NULL;
        if (!cJSON_IsArray(headers)) {
            ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [headers]");
            goto end;
        }

        headersList = OpenAPI_list_create();

        cJSON_ArrayForEach(headers_local, headers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(headers_local)) {
                ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [headers]");
                goto end;
            }
            OpenAPI_list_add(headersList, ogs_strdup(headers_local->valuestring));
        }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(af_requested_qos_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_af_requested_qos_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    af_requested_qos_data_local_var = OpenAPI_af_requested_qos_data_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        inter_group_id && !cJSON_IsNull(inter_group_id) ? ogs_strdup(inter_group_id->valuestring) : NULL,
        af_app_id && !cJSON_IsNull(af_app_id) ? ogs_strdup(af_app_id->valuestring) : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        slice_info ? slice_info_local_nonprim : NULL,
        flow_info ? flow_infoList : NULL,
        eth_flow_info ? eth_flow_infoList : NULL,
        en_eth_flow_info ? en_eth_flow_infoList : NULL,
        ev_subsc ? ev_subsc_local_nonprim : NULL,
        qos_reference && !cJSON_IsNull(qos_reference) ? ogs_strdup(qos_reference->valuestring) : NULL,
        qos_reqs ? qos_reqs_local_nonprim : NULL,
        alt_ser_reqs ? alt_ser_reqsList : NULL,
        alt_ser_reqs_data ? alt_ser_reqs_dataList : NULL,
        dis_ue_notif ? true : false,
        dis_ue_notif ? dis_ue_notif->valueint : 0,
        temp_in_validity ? temp_in_validity_local_nonprim : NULL,
        headers ? headersList : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return af_requested_qos_data_local_var;
end:
    if (slice_info_local_nonprim) {
        OpenAPI_snssai_free(slice_info_local_nonprim);
        slice_info_local_nonprim = NULL;
    }
    if (flow_infoList) {
        OpenAPI_list_for_each(flow_infoList, node) {
            OpenAPI_flow_info_1_free(node->data);
        }
        OpenAPI_list_free(flow_infoList);
        flow_infoList = NULL;
    }
    if (eth_flow_infoList) {
        OpenAPI_list_for_each(eth_flow_infoList, node) {
            OpenAPI_eth_flow_description_1_free(node->data);
        }
        OpenAPI_list_free(eth_flow_infoList);
        eth_flow_infoList = NULL;
    }
    if (en_eth_flow_infoList) {
        OpenAPI_list_for_each(en_eth_flow_infoList, node) {
            OpenAPI_eth_flow_info_free(node->data);
        }
        OpenAPI_list_free(en_eth_flow_infoList);
        en_eth_flow_infoList = NULL;
    }
    if (ev_subsc_local_nonprim) {
        OpenAPI_events_subsc_req_data_free(ev_subsc_local_nonprim);
        ev_subsc_local_nonprim = NULL;
    }
    if (qos_reqs_local_nonprim) {
        OpenAPI_qos_requirements_free(qos_reqs_local_nonprim);
        qos_reqs_local_nonprim = NULL;
    }
    if (alt_ser_reqsList) {
        OpenAPI_list_for_each(alt_ser_reqsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(alt_ser_reqsList);
        alt_ser_reqsList = NULL;
    }
    if (alt_ser_reqs_dataList) {
        OpenAPI_list_for_each(alt_ser_reqs_dataList, node) {
            OpenAPI_alternative_service_requirements_data_free(node->data);
        }
        OpenAPI_list_free(alt_ser_reqs_dataList);
        alt_ser_reqs_dataList = NULL;
    }
    if (temp_in_validity_local_nonprim) {
        OpenAPI_temporal_in_validity_free(temp_in_validity_local_nonprim);
        temp_in_validity_local_nonprim = NULL;
    }
    if (headersList) {
        OpenAPI_list_for_each(headersList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(headersList);
        headersList = NULL;
    }
    return NULL;
}

OpenAPI_af_requested_qos_data_t *OpenAPI_af_requested_qos_data_copy(OpenAPI_af_requested_qos_data_t *dst, OpenAPI_af_requested_qos_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_requested_qos_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_requested_qos_data_convertToJSON() failed");
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

    OpenAPI_af_requested_qos_data_free(dst);
    dst = OpenAPI_af_requested_qos_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

