
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdtq_data.h"

OpenAPI_pdtq_data_t *OpenAPI_pdtq_data_create(
    char *asp_id,
    OpenAPI_pdtq_policy_t *pdtq_policy,
    char *app_id,
    char *pdtq_ref_id,
    OpenAPI_network_area_info_2_t *nw_area_info,
    bool is_num_of_ues,
    int num_of_ues,
    OpenAPI_list_t *des_time_ints,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *alt_qos_param_sets,
    OpenAPI_list_t *alt_qos_refs,
    OpenAPI_qos_parameter_set_t *qos_param_set,
    char *qos_reference,
    char *notif_uri,
    bool is_warn_notif_enabled,
    int warn_notif_enabled,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_pdtq_data_t *pdtq_data_local_var = ogs_malloc(sizeof(OpenAPI_pdtq_data_t));
    ogs_assert(pdtq_data_local_var);

    pdtq_data_local_var->asp_id = asp_id;
    pdtq_data_local_var->pdtq_policy = pdtq_policy;
    pdtq_data_local_var->app_id = app_id;
    pdtq_data_local_var->pdtq_ref_id = pdtq_ref_id;
    pdtq_data_local_var->nw_area_info = nw_area_info;
    pdtq_data_local_var->is_num_of_ues = is_num_of_ues;
    pdtq_data_local_var->num_of_ues = num_of_ues;
    pdtq_data_local_var->des_time_ints = des_time_ints;
    pdtq_data_local_var->dnn = dnn;
    pdtq_data_local_var->snssai = snssai;
    pdtq_data_local_var->alt_qos_param_sets = alt_qos_param_sets;
    pdtq_data_local_var->alt_qos_refs = alt_qos_refs;
    pdtq_data_local_var->qos_param_set = qos_param_set;
    pdtq_data_local_var->qos_reference = qos_reference;
    pdtq_data_local_var->notif_uri = notif_uri;
    pdtq_data_local_var->is_warn_notif_enabled = is_warn_notif_enabled;
    pdtq_data_local_var->warn_notif_enabled = warn_notif_enabled;
    pdtq_data_local_var->supp_feat = supp_feat;
    pdtq_data_local_var->reset_ids = reset_ids;

    return pdtq_data_local_var;
}

void OpenAPI_pdtq_data_free(OpenAPI_pdtq_data_t *pdtq_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdtq_data) {
        return;
    }
    if (pdtq_data->asp_id) {
        ogs_free(pdtq_data->asp_id);
        pdtq_data->asp_id = NULL;
    }
    if (pdtq_data->pdtq_policy) {
        OpenAPI_pdtq_policy_free(pdtq_data->pdtq_policy);
        pdtq_data->pdtq_policy = NULL;
    }
    if (pdtq_data->app_id) {
        ogs_free(pdtq_data->app_id);
        pdtq_data->app_id = NULL;
    }
    if (pdtq_data->pdtq_ref_id) {
        ogs_free(pdtq_data->pdtq_ref_id);
        pdtq_data->pdtq_ref_id = NULL;
    }
    if (pdtq_data->nw_area_info) {
        OpenAPI_network_area_info_2_free(pdtq_data->nw_area_info);
        pdtq_data->nw_area_info = NULL;
    }
    if (pdtq_data->des_time_ints) {
        OpenAPI_list_for_each(pdtq_data->des_time_ints, node) {
            OpenAPI_time_window_free(node->data);
        }
        OpenAPI_list_free(pdtq_data->des_time_ints);
        pdtq_data->des_time_ints = NULL;
    }
    if (pdtq_data->dnn) {
        ogs_free(pdtq_data->dnn);
        pdtq_data->dnn = NULL;
    }
    if (pdtq_data->snssai) {
        OpenAPI_snssai_free(pdtq_data->snssai);
        pdtq_data->snssai = NULL;
    }
    if (pdtq_data->alt_qos_param_sets) {
        OpenAPI_list_for_each(pdtq_data->alt_qos_param_sets, node) {
            OpenAPI_alt_qos_param_set_free(node->data);
        }
        OpenAPI_list_free(pdtq_data->alt_qos_param_sets);
        pdtq_data->alt_qos_param_sets = NULL;
    }
    if (pdtq_data->alt_qos_refs) {
        OpenAPI_list_for_each(pdtq_data->alt_qos_refs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdtq_data->alt_qos_refs);
        pdtq_data->alt_qos_refs = NULL;
    }
    if (pdtq_data->qos_param_set) {
        OpenAPI_qos_parameter_set_free(pdtq_data->qos_param_set);
        pdtq_data->qos_param_set = NULL;
    }
    if (pdtq_data->qos_reference) {
        ogs_free(pdtq_data->qos_reference);
        pdtq_data->qos_reference = NULL;
    }
    if (pdtq_data->notif_uri) {
        ogs_free(pdtq_data->notif_uri);
        pdtq_data->notif_uri = NULL;
    }
    if (pdtq_data->supp_feat) {
        ogs_free(pdtq_data->supp_feat);
        pdtq_data->supp_feat = NULL;
    }
    if (pdtq_data->reset_ids) {
        OpenAPI_list_for_each(pdtq_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdtq_data->reset_ids);
        pdtq_data->reset_ids = NULL;
    }
    ogs_free(pdtq_data);
}

cJSON *OpenAPI_pdtq_data_convertToJSON(OpenAPI_pdtq_data_t *pdtq_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdtq_data == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [PdtqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pdtq_data->asp_id) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [asp_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "aspId", pdtq_data->asp_id) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [asp_id]");
        goto end;
    }

    if (!pdtq_data->pdtq_policy) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [pdtq_policy]");
        return NULL;
    }
    cJSON *pdtq_policy_local_JSON = OpenAPI_pdtq_policy_convertToJSON(pdtq_data->pdtq_policy);
    if (pdtq_policy_local_JSON == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [pdtq_policy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pdtqPolicy", pdtq_policy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [pdtq_policy]");
        goto end;
    }

    if (pdtq_data->app_id) {
    if (cJSON_AddStringToObject(item, "appId", pdtq_data->app_id) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (pdtq_data->pdtq_ref_id) {
    if (cJSON_AddStringToObject(item, "pdtqRefId", pdtq_data->pdtq_ref_id) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [pdtq_ref_id]");
        goto end;
    }
    }

    if (pdtq_data->nw_area_info) {
    cJSON *nw_area_info_local_JSON = OpenAPI_network_area_info_2_convertToJSON(pdtq_data->nw_area_info);
    if (nw_area_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwAreaInfo", nw_area_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    }

    if (pdtq_data->is_num_of_ues) {
    if (cJSON_AddNumberToObject(item, "numOfUes", pdtq_data->num_of_ues) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [num_of_ues]");
        goto end;
    }
    }

    if (pdtq_data->des_time_ints) {
    cJSON *des_time_intsList = cJSON_AddArrayToObject(item, "desTimeInts");
    if (des_time_intsList == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [des_time_ints]");
        goto end;
    }
    OpenAPI_list_for_each(pdtq_data->des_time_ints, node) {
        cJSON *itemLocal = OpenAPI_time_window_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [des_time_ints]");
            goto end;
        }
        cJSON_AddItemToArray(des_time_intsList, itemLocal);
    }
    }

    if (pdtq_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", pdtq_data->dnn) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (pdtq_data->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(pdtq_data->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (pdtq_data->alt_qos_param_sets) {
    cJSON *alt_qos_param_setsList = cJSON_AddArrayToObject(item, "altQosParamSets");
    if (alt_qos_param_setsList == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [alt_qos_param_sets]");
        goto end;
    }
    OpenAPI_list_for_each(pdtq_data->alt_qos_param_sets, node) {
        cJSON *itemLocal = OpenAPI_alt_qos_param_set_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [alt_qos_param_sets]");
            goto end;
        }
        cJSON_AddItemToArray(alt_qos_param_setsList, itemLocal);
    }
    }

    if (pdtq_data->alt_qos_refs) {
    cJSON *alt_qos_refsList = cJSON_AddArrayToObject(item, "altQosRefs");
    if (alt_qos_refsList == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [alt_qos_refs]");
        goto end;
    }
    OpenAPI_list_for_each(pdtq_data->alt_qos_refs, node) {
        if (cJSON_AddStringToObject(alt_qos_refsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [alt_qos_refs]");
            goto end;
        }
    }
    }

    if (pdtq_data->qos_param_set) {
    cJSON *qos_param_set_local_JSON = OpenAPI_qos_parameter_set_convertToJSON(pdtq_data->qos_param_set);
    if (qos_param_set_local_JSON == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [qos_param_set]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosParamSet", qos_param_set_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [qos_param_set]");
        goto end;
    }
    }

    if (pdtq_data->qos_reference) {
    if (cJSON_AddStringToObject(item, "qosReference", pdtq_data->qos_reference) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [qos_reference]");
        goto end;
    }
    }

    if (pdtq_data->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", pdtq_data->notif_uri) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (pdtq_data->is_warn_notif_enabled) {
    if (cJSON_AddBoolToObject(item, "warnNotifEnabled", pdtq_data->warn_notif_enabled) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [warn_notif_enabled]");
        goto end;
    }
    }

    if (pdtq_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", pdtq_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (pdtq_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(pdtq_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdtq_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pdtq_data_t *OpenAPI_pdtq_data_parseFromJSON(cJSON *pdtq_dataJSON)
{
    OpenAPI_pdtq_data_t *pdtq_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *asp_id = NULL;
    cJSON *pdtq_policy = NULL;
    OpenAPI_pdtq_policy_t *pdtq_policy_local_nonprim = NULL;
    cJSON *app_id = NULL;
    cJSON *pdtq_ref_id = NULL;
    cJSON *nw_area_info = NULL;
    OpenAPI_network_area_info_2_t *nw_area_info_local_nonprim = NULL;
    cJSON *num_of_ues = NULL;
    cJSON *des_time_ints = NULL;
    OpenAPI_list_t *des_time_intsList = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *alt_qos_param_sets = NULL;
    OpenAPI_list_t *alt_qos_param_setsList = NULL;
    cJSON *alt_qos_refs = NULL;
    OpenAPI_list_t *alt_qos_refsList = NULL;
    cJSON *qos_param_set = NULL;
    OpenAPI_qos_parameter_set_t *qos_param_set_local_nonprim = NULL;
    cJSON *qos_reference = NULL;
    cJSON *notif_uri = NULL;
    cJSON *warn_notif_enabled = NULL;
    cJSON *supp_feat = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    asp_id = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "aspId");
    if (!asp_id) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [asp_id]");
        goto end;
    }
    if (!cJSON_IsString(asp_id)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [asp_id]");
        goto end;
    }

    pdtq_policy = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "pdtqPolicy");
    if (!pdtq_policy) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [pdtq_policy]");
        goto end;
    }
    pdtq_policy_local_nonprim = OpenAPI_pdtq_policy_parseFromJSON(pdtq_policy);
    if (!pdtq_policy_local_nonprim) {
        ogs_error("OpenAPI_pdtq_policy_parseFromJSON failed [pdtq_policy]");
        goto end;
    }

    app_id = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    pdtq_ref_id = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "pdtqRefId");
    if (pdtq_ref_id) {
    if (!cJSON_IsString(pdtq_ref_id) && !cJSON_IsNull(pdtq_ref_id)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [pdtq_ref_id]");
        goto end;
    }
    }

    nw_area_info = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "nwAreaInfo");
    if (nw_area_info) {
    nw_area_info_local_nonprim = OpenAPI_network_area_info_2_parseFromJSON(nw_area_info);
    if (!nw_area_info_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_2_parseFromJSON failed [nw_area_info]");
        goto end;
    }
    }

    num_of_ues = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "numOfUes");
    if (num_of_ues) {
    if (!cJSON_IsNumber(num_of_ues)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [num_of_ues]");
        goto end;
    }
    }

    des_time_ints = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "desTimeInts");
    if (des_time_ints) {
        cJSON *des_time_ints_local = NULL;
        if (!cJSON_IsArray(des_time_ints)) {
            ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [des_time_ints]");
            goto end;
        }

        des_time_intsList = OpenAPI_list_create();

        cJSON_ArrayForEach(des_time_ints_local, des_time_ints) {
            if (!cJSON_IsObject(des_time_ints_local)) {
                ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [des_time_ints]");
                goto end;
            }
            OpenAPI_time_window_t *des_time_intsItem = OpenAPI_time_window_parseFromJSON(des_time_ints_local);
            if (!des_time_intsItem) {
                ogs_error("No des_time_intsItem");
                goto end;
            }
            OpenAPI_list_add(des_time_intsList, des_time_intsItem);
        }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    alt_qos_param_sets = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "altQosParamSets");
    if (alt_qos_param_sets) {
        cJSON *alt_qos_param_sets_local = NULL;
        if (!cJSON_IsArray(alt_qos_param_sets)) {
            ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [alt_qos_param_sets]");
            goto end;
        }

        alt_qos_param_setsList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_qos_param_sets_local, alt_qos_param_sets) {
            if (!cJSON_IsObject(alt_qos_param_sets_local)) {
                ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [alt_qos_param_sets]");
                goto end;
            }
            OpenAPI_alt_qos_param_set_t *alt_qos_param_setsItem = OpenAPI_alt_qos_param_set_parseFromJSON(alt_qos_param_sets_local);
            if (!alt_qos_param_setsItem) {
                ogs_error("No alt_qos_param_setsItem");
                goto end;
            }
            OpenAPI_list_add(alt_qos_param_setsList, alt_qos_param_setsItem);
        }
    }

    alt_qos_refs = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "altQosRefs");
    if (alt_qos_refs) {
        cJSON *alt_qos_refs_local = NULL;
        if (!cJSON_IsArray(alt_qos_refs)) {
            ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [alt_qos_refs]");
            goto end;
        }

        alt_qos_refsList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_qos_refs_local, alt_qos_refs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(alt_qos_refs_local)) {
                ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [alt_qos_refs]");
                goto end;
            }
            OpenAPI_list_add(alt_qos_refsList, ogs_strdup(alt_qos_refs_local->valuestring));
        }
    }

    qos_param_set = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "qosParamSet");
    if (qos_param_set) {
    qos_param_set_local_nonprim = OpenAPI_qos_parameter_set_parseFromJSON(qos_param_set);
    if (!qos_param_set_local_nonprim) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON failed [qos_param_set]");
        goto end;
    }
    }

    qos_reference = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "qosReference");
    if (qos_reference) {
    if (!cJSON_IsString(qos_reference) && !cJSON_IsNull(qos_reference)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [qos_reference]");
        goto end;
    }
    }

    notif_uri = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    warn_notif_enabled = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "warnNotifEnabled");
    if (warn_notif_enabled) {
    if (!cJSON_IsBool(warn_notif_enabled)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [warn_notif_enabled]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(pdtq_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_pdtq_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    pdtq_data_local_var = OpenAPI_pdtq_data_create (
        ogs_strdup(asp_id->valuestring),
        pdtq_policy_local_nonprim,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        pdtq_ref_id && !cJSON_IsNull(pdtq_ref_id) ? ogs_strdup(pdtq_ref_id->valuestring) : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL,
        num_of_ues ? true : false,
        num_of_ues ? num_of_ues->valuedouble : 0,
        des_time_ints ? des_time_intsList : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        alt_qos_param_sets ? alt_qos_param_setsList : NULL,
        alt_qos_refs ? alt_qos_refsList : NULL,
        qos_param_set ? qos_param_set_local_nonprim : NULL,
        qos_reference && !cJSON_IsNull(qos_reference) ? ogs_strdup(qos_reference->valuestring) : NULL,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        warn_notif_enabled ? true : false,
        warn_notif_enabled ? warn_notif_enabled->valueint : 0,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return pdtq_data_local_var;
end:
    if (pdtq_policy_local_nonprim) {
        OpenAPI_pdtq_policy_free(pdtq_policy_local_nonprim);
        pdtq_policy_local_nonprim = NULL;
    }
    if (nw_area_info_local_nonprim) {
        OpenAPI_network_area_info_2_free(nw_area_info_local_nonprim);
        nw_area_info_local_nonprim = NULL;
    }
    if (des_time_intsList) {
        OpenAPI_list_for_each(des_time_intsList, node) {
            OpenAPI_time_window_free(node->data);
        }
        OpenAPI_list_free(des_time_intsList);
        des_time_intsList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (alt_qos_param_setsList) {
        OpenAPI_list_for_each(alt_qos_param_setsList, node) {
            OpenAPI_alt_qos_param_set_free(node->data);
        }
        OpenAPI_list_free(alt_qos_param_setsList);
        alt_qos_param_setsList = NULL;
    }
    if (alt_qos_refsList) {
        OpenAPI_list_for_each(alt_qos_refsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(alt_qos_refsList);
        alt_qos_refsList = NULL;
    }
    if (qos_param_set_local_nonprim) {
        OpenAPI_qos_parameter_set_free(qos_param_set_local_nonprim);
        qos_param_set_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    return NULL;
}

OpenAPI_pdtq_data_t *OpenAPI_pdtq_data_copy(OpenAPI_pdtq_data_t *dst, OpenAPI_pdtq_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdtq_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdtq_data_convertToJSON() failed");
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

    OpenAPI_pdtq_data_free(dst);
    dst = OpenAPI_pdtq_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

