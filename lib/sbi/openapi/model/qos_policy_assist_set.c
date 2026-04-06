
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_policy_assist_set.h"

OpenAPI_qos_policy_assist_set_t *OpenAPI_qos_policy_assist_set_create(
    OpenAPI_qos_para_t *qos_param_set,
    char *dnn,
    char *app_id,
    OpenAPI_list_t *f_descs,
    bool is_app_duration,
    int app_duration,
    bool is_predicted_avg_qoe,
    float predicted_avg_qoe,
    bool is_predicted_max_qoe,
    float predicted_max_qoe,
    bool is_predicted_min_qoe,
    float predicted_min_qoe,
    bool is_pred_qoe_variance,
    float pred_qoe_variance,
    OpenAPI_time_window_t *qos_pol_time_win,
    OpenAPI_list_t *freqs,
    OpenAPI_list_t *rat_types,
    OpenAPI_time_window_t *validity_period,
    OpenAPI_network_area_info_t *spatial_validity,
    bool is_max_qo_s_flow_usg_dur,
    int max_qo_s_flow_usg_dur,
    bool is_min_qo_s_flow_usg_dur,
    int min_qo_s_flow_usg_dur,
    bool is_avg_qo_s_flow_usg_dur,
    int avg_qo_s_flow_usg_dur,
    bool is_qos_flow_usg_number,
    int qos_flow_usg_number
)
{
    OpenAPI_qos_policy_assist_set_t *qos_policy_assist_set_local_var = ogs_malloc(sizeof(OpenAPI_qos_policy_assist_set_t));
    ogs_assert(qos_policy_assist_set_local_var);

    qos_policy_assist_set_local_var->qos_param_set = qos_param_set;
    qos_policy_assist_set_local_var->dnn = dnn;
    qos_policy_assist_set_local_var->app_id = app_id;
    qos_policy_assist_set_local_var->f_descs = f_descs;
    qos_policy_assist_set_local_var->is_app_duration = is_app_duration;
    qos_policy_assist_set_local_var->app_duration = app_duration;
    qos_policy_assist_set_local_var->is_predicted_avg_qoe = is_predicted_avg_qoe;
    qos_policy_assist_set_local_var->predicted_avg_qoe = predicted_avg_qoe;
    qos_policy_assist_set_local_var->is_predicted_max_qoe = is_predicted_max_qoe;
    qos_policy_assist_set_local_var->predicted_max_qoe = predicted_max_qoe;
    qos_policy_assist_set_local_var->is_predicted_min_qoe = is_predicted_min_qoe;
    qos_policy_assist_set_local_var->predicted_min_qoe = predicted_min_qoe;
    qos_policy_assist_set_local_var->is_pred_qoe_variance = is_pred_qoe_variance;
    qos_policy_assist_set_local_var->pred_qoe_variance = pred_qoe_variance;
    qos_policy_assist_set_local_var->qos_pol_time_win = qos_pol_time_win;
    qos_policy_assist_set_local_var->freqs = freqs;
    qos_policy_assist_set_local_var->rat_types = rat_types;
    qos_policy_assist_set_local_var->validity_period = validity_period;
    qos_policy_assist_set_local_var->spatial_validity = spatial_validity;
    qos_policy_assist_set_local_var->is_max_qo_s_flow_usg_dur = is_max_qo_s_flow_usg_dur;
    qos_policy_assist_set_local_var->max_qo_s_flow_usg_dur = max_qo_s_flow_usg_dur;
    qos_policy_assist_set_local_var->is_min_qo_s_flow_usg_dur = is_min_qo_s_flow_usg_dur;
    qos_policy_assist_set_local_var->min_qo_s_flow_usg_dur = min_qo_s_flow_usg_dur;
    qos_policy_assist_set_local_var->is_avg_qo_s_flow_usg_dur = is_avg_qo_s_flow_usg_dur;
    qos_policy_assist_set_local_var->avg_qo_s_flow_usg_dur = avg_qo_s_flow_usg_dur;
    qos_policy_assist_set_local_var->is_qos_flow_usg_number = is_qos_flow_usg_number;
    qos_policy_assist_set_local_var->qos_flow_usg_number = qos_flow_usg_number;

    return qos_policy_assist_set_local_var;
}

void OpenAPI_qos_policy_assist_set_free(OpenAPI_qos_policy_assist_set_t *qos_policy_assist_set)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_policy_assist_set) {
        return;
    }
    if (qos_policy_assist_set->qos_param_set) {
        OpenAPI_qos_para_free(qos_policy_assist_set->qos_param_set);
        qos_policy_assist_set->qos_param_set = NULL;
    }
    if (qos_policy_assist_set->dnn) {
        ogs_free(qos_policy_assist_set->dnn);
        qos_policy_assist_set->dnn = NULL;
    }
    if (qos_policy_assist_set->app_id) {
        ogs_free(qos_policy_assist_set->app_id);
        qos_policy_assist_set->app_id = NULL;
    }
    if (qos_policy_assist_set->f_descs) {
        OpenAPI_list_for_each(qos_policy_assist_set->f_descs, node) {
            OpenAPI_ip_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(qos_policy_assist_set->f_descs);
        qos_policy_assist_set->f_descs = NULL;
    }
    if (qos_policy_assist_set->qos_pol_time_win) {
        OpenAPI_time_window_free(qos_policy_assist_set->qos_pol_time_win);
        qos_policy_assist_set->qos_pol_time_win = NULL;
    }
    if (qos_policy_assist_set->freqs) {
        OpenAPI_list_for_each(qos_policy_assist_set->freqs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_policy_assist_set->freqs);
        qos_policy_assist_set->freqs = NULL;
    }
    if (qos_policy_assist_set->rat_types) {
        OpenAPI_list_free(qos_policy_assist_set->rat_types);
        qos_policy_assist_set->rat_types = NULL;
    }
    if (qos_policy_assist_set->validity_period) {
        OpenAPI_time_window_free(qos_policy_assist_set->validity_period);
        qos_policy_assist_set->validity_period = NULL;
    }
    if (qos_policy_assist_set->spatial_validity) {
        OpenAPI_network_area_info_free(qos_policy_assist_set->spatial_validity);
        qos_policy_assist_set->spatial_validity = NULL;
    }
    ogs_free(qos_policy_assist_set);
}

cJSON *OpenAPI_qos_policy_assist_set_convertToJSON(OpenAPI_qos_policy_assist_set_t *qos_policy_assist_set)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_policy_assist_set == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [QosPolicyAssistSet]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_policy_assist_set->qos_param_set) {
    cJSON *qos_param_set_local_JSON = OpenAPI_qos_para_convertToJSON(qos_policy_assist_set->qos_param_set);
    if (qos_param_set_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [qos_param_set]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosParamSet", qos_param_set_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [qos_param_set]");
        goto end;
    }
    }

    if (qos_policy_assist_set->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", qos_policy_assist_set->dnn) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (qos_policy_assist_set->app_id) {
    if (cJSON_AddStringToObject(item, "appId", qos_policy_assist_set->app_id) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (qos_policy_assist_set->f_descs) {
    cJSON *f_descsList = cJSON_AddArrayToObject(item, "fDescs");
    if (f_descsList == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [f_descs]");
        goto end;
    }
    OpenAPI_list_for_each(qos_policy_assist_set->f_descs, node) {
        cJSON *itemLocal = OpenAPI_ip_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [f_descs]");
            goto end;
        }
        cJSON_AddItemToArray(f_descsList, itemLocal);
    }
    }

    if (qos_policy_assist_set->is_app_duration) {
    if (cJSON_AddNumberToObject(item, "appDuration", qos_policy_assist_set->app_duration) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [app_duration]");
        goto end;
    }
    }

    if (qos_policy_assist_set->is_predicted_avg_qoe) {
    if (cJSON_AddNumberToObject(item, "predictedAvgQoe", qos_policy_assist_set->predicted_avg_qoe) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [predicted_avg_qoe]");
        goto end;
    }
    }

    if (qos_policy_assist_set->is_predicted_max_qoe) {
    if (cJSON_AddNumberToObject(item, "predictedMaxQoe", qos_policy_assist_set->predicted_max_qoe) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [predicted_max_qoe]");
        goto end;
    }
    }

    if (qos_policy_assist_set->is_predicted_min_qoe) {
    if (cJSON_AddNumberToObject(item, "predictedMinQoe", qos_policy_assist_set->predicted_min_qoe) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [predicted_min_qoe]");
        goto end;
    }
    }

    if (qos_policy_assist_set->is_pred_qoe_variance) {
    if (cJSON_AddNumberToObject(item, "predQoeVariance", qos_policy_assist_set->pred_qoe_variance) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [pred_qoe_variance]");
        goto end;
    }
    }

    if (qos_policy_assist_set->qos_pol_time_win) {
    cJSON *qos_pol_time_win_local_JSON = OpenAPI_time_window_convertToJSON(qos_policy_assist_set->qos_pol_time_win);
    if (qos_pol_time_win_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [qos_pol_time_win]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosPolTimeWin", qos_pol_time_win_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [qos_pol_time_win]");
        goto end;
    }
    }

    if (qos_policy_assist_set->freqs) {
    cJSON *freqsList = cJSON_AddArrayToObject(item, "freqs");
    if (freqsList == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [freqs]");
        goto end;
    }
    OpenAPI_list_for_each(qos_policy_assist_set->freqs, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [freqs]");
            goto end;
        }
        if (cJSON_AddNumberToObject(freqsList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [freqs]");
            goto end;
        }
    }
    }

    if (qos_policy_assist_set->rat_types != OpenAPI_rat_type_NULL) {
    cJSON *rat_typesList = cJSON_AddArrayToObject(item, "ratTypes");
    if (rat_typesList == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [rat_types]");
        goto end;
    }
    OpenAPI_list_for_each(qos_policy_assist_set->rat_types, node) {
        if (cJSON_AddStringToObject(rat_typesList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [rat_types]");
            goto end;
        }
    }
    }

    if (qos_policy_assist_set->validity_period) {
    cJSON *validity_period_local_JSON = OpenAPI_time_window_convertToJSON(qos_policy_assist_set->validity_period);
    if (validity_period_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [validity_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "validityPeriod", validity_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [validity_period]");
        goto end;
    }
    }

    if (qos_policy_assist_set->spatial_validity) {
    cJSON *spatial_validity_local_JSON = OpenAPI_network_area_info_convertToJSON(qos_policy_assist_set->spatial_validity);
    if (spatial_validity_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [spatial_validity]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidity", spatial_validity_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [spatial_validity]");
        goto end;
    }
    }

    if (qos_policy_assist_set->is_max_qo_s_flow_usg_dur) {
    if (cJSON_AddNumberToObject(item, "maxQoSFlowUsgDur", qos_policy_assist_set->max_qo_s_flow_usg_dur) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [max_qo_s_flow_usg_dur]");
        goto end;
    }
    }

    if (qos_policy_assist_set->is_min_qo_s_flow_usg_dur) {
    if (cJSON_AddNumberToObject(item, "minQoSFlowUsgDur", qos_policy_assist_set->min_qo_s_flow_usg_dur) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [min_qo_s_flow_usg_dur]");
        goto end;
    }
    }

    if (qos_policy_assist_set->is_avg_qo_s_flow_usg_dur) {
    if (cJSON_AddNumberToObject(item, "avgQoSFlowUsgDur", qos_policy_assist_set->avg_qo_s_flow_usg_dur) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [avg_qo_s_flow_usg_dur]");
        goto end;
    }
    }

    if (qos_policy_assist_set->is_qos_flow_usg_number) {
    if (cJSON_AddNumberToObject(item, "qosFlowUsgNumber", qos_policy_assist_set->qos_flow_usg_number) == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed [qos_flow_usg_number]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_policy_assist_set_t *OpenAPI_qos_policy_assist_set_parseFromJSON(cJSON *qos_policy_assist_setJSON)
{
    OpenAPI_qos_policy_assist_set_t *qos_policy_assist_set_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qos_param_set = NULL;
    OpenAPI_qos_para_t *qos_param_set_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *app_id = NULL;
    cJSON *f_descs = NULL;
    OpenAPI_list_t *f_descsList = NULL;
    cJSON *app_duration = NULL;
    cJSON *predicted_avg_qoe = NULL;
    cJSON *predicted_max_qoe = NULL;
    cJSON *predicted_min_qoe = NULL;
    cJSON *pred_qoe_variance = NULL;
    cJSON *qos_pol_time_win = NULL;
    OpenAPI_time_window_t *qos_pol_time_win_local_nonprim = NULL;
    cJSON *freqs = NULL;
    OpenAPI_list_t *freqsList = NULL;
    cJSON *rat_types = NULL;
    OpenAPI_list_t *rat_typesList = NULL;
    cJSON *validity_period = NULL;
    OpenAPI_time_window_t *validity_period_local_nonprim = NULL;
    cJSON *spatial_validity = NULL;
    OpenAPI_network_area_info_t *spatial_validity_local_nonprim = NULL;
    cJSON *max_qo_s_flow_usg_dur = NULL;
    cJSON *min_qo_s_flow_usg_dur = NULL;
    cJSON *avg_qo_s_flow_usg_dur = NULL;
    cJSON *qos_flow_usg_number = NULL;
    qos_param_set = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "qosParamSet");
    if (qos_param_set) {
    qos_param_set_local_nonprim = OpenAPI_qos_para_parseFromJSON(qos_param_set);
    if (!qos_param_set_local_nonprim) {
        ogs_error("OpenAPI_qos_para_parseFromJSON failed [qos_param_set]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    f_descs = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "fDescs");
    if (f_descs) {
        cJSON *f_descs_local = NULL;
        if (!cJSON_IsArray(f_descs)) {
            ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [f_descs]");
            goto end;
        }

        f_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(f_descs_local, f_descs) {
            if (!cJSON_IsObject(f_descs_local)) {
                ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [f_descs]");
                goto end;
            }
            OpenAPI_ip_eth_flow_description_t *f_descsItem = OpenAPI_ip_eth_flow_description_parseFromJSON(f_descs_local);
            if (!f_descsItem) {
                ogs_error("No f_descsItem");
                goto end;
            }
            OpenAPI_list_add(f_descsList, f_descsItem);
        }
    }

    app_duration = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "appDuration");
    if (app_duration) {
    if (!cJSON_IsNumber(app_duration)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [app_duration]");
        goto end;
    }
    }

    predicted_avg_qoe = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "predictedAvgQoe");
    if (predicted_avg_qoe) {
    if (!cJSON_IsNumber(predicted_avg_qoe)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [predicted_avg_qoe]");
        goto end;
    }
    }

    predicted_max_qoe = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "predictedMaxQoe");
    if (predicted_max_qoe) {
    if (!cJSON_IsNumber(predicted_max_qoe)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [predicted_max_qoe]");
        goto end;
    }
    }

    predicted_min_qoe = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "predictedMinQoe");
    if (predicted_min_qoe) {
    if (!cJSON_IsNumber(predicted_min_qoe)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [predicted_min_qoe]");
        goto end;
    }
    }

    pred_qoe_variance = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "predQoeVariance");
    if (pred_qoe_variance) {
    if (!cJSON_IsNumber(pred_qoe_variance)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [pred_qoe_variance]");
        goto end;
    }
    }

    qos_pol_time_win = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "qosPolTimeWin");
    if (qos_pol_time_win) {
    qos_pol_time_win_local_nonprim = OpenAPI_time_window_parseFromJSON(qos_pol_time_win);
    if (!qos_pol_time_win_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [qos_pol_time_win]");
        goto end;
    }
    }

    freqs = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "freqs");
    if (freqs) {
        cJSON *freqs_local = NULL;
        if (!cJSON_IsArray(freqs)) {
            ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [freqs]");
            goto end;
        }

        freqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(freqs_local, freqs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(freqs_local)) {
                ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [freqs]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [freqs]");
                goto end;
            }
            *localDouble = freqs_local->valuedouble;
            OpenAPI_list_add(freqsList, localDouble);
        }
    }

    rat_types = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "ratTypes");
    if (rat_types) {
        cJSON *rat_types_local = NULL;
        if (!cJSON_IsArray(rat_types)) {
            ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [rat_types]");
            goto end;
        }

        rat_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_types_local, rat_types) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(rat_types_local)) {
                ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [rat_types]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(rat_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rat_types\" is not supported. Ignoring it ...",
                         rat_types_local->valuestring);
            } else {
                OpenAPI_list_add(rat_typesList, (void *)localEnum);
            }
        }
        if (rat_typesList->count == 0) {
            ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed: Expected rat_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    validity_period = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "validityPeriod");
    if (validity_period) {
    validity_period_local_nonprim = OpenAPI_time_window_parseFromJSON(validity_period);
    if (!validity_period_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [validity_period]");
        goto end;
    }
    }

    spatial_validity = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "spatialValidity");
    if (spatial_validity) {
    spatial_validity_local_nonprim = OpenAPI_network_area_info_parseFromJSON(spatial_validity);
    if (!spatial_validity_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [spatial_validity]");
        goto end;
    }
    }

    max_qo_s_flow_usg_dur = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "maxQoSFlowUsgDur");
    if (max_qo_s_flow_usg_dur) {
    if (!cJSON_IsNumber(max_qo_s_flow_usg_dur)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [max_qo_s_flow_usg_dur]");
        goto end;
    }
    }

    min_qo_s_flow_usg_dur = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "minQoSFlowUsgDur");
    if (min_qo_s_flow_usg_dur) {
    if (!cJSON_IsNumber(min_qo_s_flow_usg_dur)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [min_qo_s_flow_usg_dur]");
        goto end;
    }
    }

    avg_qo_s_flow_usg_dur = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "avgQoSFlowUsgDur");
    if (avg_qo_s_flow_usg_dur) {
    if (!cJSON_IsNumber(avg_qo_s_flow_usg_dur)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [avg_qo_s_flow_usg_dur]");
        goto end;
    }
    }

    qos_flow_usg_number = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_setJSON, "qosFlowUsgNumber");
    if (qos_flow_usg_number) {
    if (!cJSON_IsNumber(qos_flow_usg_number)) {
        ogs_error("OpenAPI_qos_policy_assist_set_parseFromJSON() failed [qos_flow_usg_number]");
        goto end;
    }
    }

    qos_policy_assist_set_local_var = OpenAPI_qos_policy_assist_set_create (
        qos_param_set ? qos_param_set_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        f_descs ? f_descsList : NULL,
        app_duration ? true : false,
        app_duration ? app_duration->valuedouble : 0,
        predicted_avg_qoe ? true : false,
        predicted_avg_qoe ? predicted_avg_qoe->valuedouble : 0,
        predicted_max_qoe ? true : false,
        predicted_max_qoe ? predicted_max_qoe->valuedouble : 0,
        predicted_min_qoe ? true : false,
        predicted_min_qoe ? predicted_min_qoe->valuedouble : 0,
        pred_qoe_variance ? true : false,
        pred_qoe_variance ? pred_qoe_variance->valuedouble : 0,
        qos_pol_time_win ? qos_pol_time_win_local_nonprim : NULL,
        freqs ? freqsList : NULL,
        rat_types ? rat_typesList : NULL,
        validity_period ? validity_period_local_nonprim : NULL,
        spatial_validity ? spatial_validity_local_nonprim : NULL,
        max_qo_s_flow_usg_dur ? true : false,
        max_qo_s_flow_usg_dur ? max_qo_s_flow_usg_dur->valuedouble : 0,
        min_qo_s_flow_usg_dur ? true : false,
        min_qo_s_flow_usg_dur ? min_qo_s_flow_usg_dur->valuedouble : 0,
        avg_qo_s_flow_usg_dur ? true : false,
        avg_qo_s_flow_usg_dur ? avg_qo_s_flow_usg_dur->valuedouble : 0,
        qos_flow_usg_number ? true : false,
        qos_flow_usg_number ? qos_flow_usg_number->valuedouble : 0
    );

    return qos_policy_assist_set_local_var;
end:
    if (qos_param_set_local_nonprim) {
        OpenAPI_qos_para_free(qos_param_set_local_nonprim);
        qos_param_set_local_nonprim = NULL;
    }
    if (f_descsList) {
        OpenAPI_list_for_each(f_descsList, node) {
            OpenAPI_ip_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(f_descsList);
        f_descsList = NULL;
    }
    if (qos_pol_time_win_local_nonprim) {
        OpenAPI_time_window_free(qos_pol_time_win_local_nonprim);
        qos_pol_time_win_local_nonprim = NULL;
    }
    if (freqsList) {
        OpenAPI_list_for_each(freqsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(freqsList);
        freqsList = NULL;
    }
    if (rat_typesList) {
        OpenAPI_list_free(rat_typesList);
        rat_typesList = NULL;
    }
    if (validity_period_local_nonprim) {
        OpenAPI_time_window_free(validity_period_local_nonprim);
        validity_period_local_nonprim = NULL;
    }
    if (spatial_validity_local_nonprim) {
        OpenAPI_network_area_info_free(spatial_validity_local_nonprim);
        spatial_validity_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_qos_policy_assist_set_t *OpenAPI_qos_policy_assist_set_copy(OpenAPI_qos_policy_assist_set_t *dst, OpenAPI_qos_policy_assist_set_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_policy_assist_set_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_policy_assist_set_convertToJSON() failed");
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

    OpenAPI_qos_policy_assist_set_free(dst);
    dst = OpenAPI_qos_policy_assist_set_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

