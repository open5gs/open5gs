
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_data.h"

OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_create(
    char *qm_id,
    OpenAPI_qos_monitoring_param_type_e qos_mon_param_type,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_list_t *rep_freqs,
    bool is_rep_thresh_dl_null,
    bool is_rep_thresh_dl,
    int rep_thresh_dl,
    bool is_rep_thresh_ul_null,
    bool is_rep_thresh_ul,
    int rep_thresh_ul,
    bool is_rep_thresh_rp_null,
    bool is_rep_thresh_rp,
    int rep_thresh_rp,
    bool is_con_thresh_dl_null,
    bool is_con_thresh_dl,
    int con_thresh_dl,
    bool is_con_thresh_ul_null,
    bool is_con_thresh_ul,
    int con_thresh_ul,
    bool is_wait_time_null,
    bool is_wait_time,
    int wait_time,
    bool is_rep_period_null,
    bool is_rep_period,
    int rep_period,
    bool is_notify_uri_null,
    char *notify_uri,
    bool is_notify_corre_id_null,
    char *notify_corre_id,
    bool is_direct_notif_ind,
    int direct_notif_ind,
    bool is_avrg_wndw_null,
    bool is_avrg_wndw,
    int avrg_wndw,
    bool is_rep_thresh_dat_rate_ul_null,
    char *rep_thresh_dat_rate_ul,
    bool is_rep_thresh_dat_rate_dl_null,
    char *rep_thresh_dat_rate_dl,
    OpenAPI_list_t *avl_bitrate_ul_thrs,
    OpenAPI_list_t *avl_bitrate_dl_thrs,
    char *data_coll_app_id
)
{
    OpenAPI_qos_monitoring_data_t *qos_monitoring_data_local_var = ogs_malloc(sizeof(OpenAPI_qos_monitoring_data_t));
    ogs_assert(qos_monitoring_data_local_var);

    qos_monitoring_data_local_var->qm_id = qm_id;
    qos_monitoring_data_local_var->qos_mon_param_type = qos_mon_param_type;
    qos_monitoring_data_local_var->req_qos_mon_params = req_qos_mon_params;
    qos_monitoring_data_local_var->rep_freqs = rep_freqs;
    qos_monitoring_data_local_var->is_rep_thresh_dl_null = is_rep_thresh_dl_null;
    qos_monitoring_data_local_var->is_rep_thresh_dl = is_rep_thresh_dl;
    qos_monitoring_data_local_var->rep_thresh_dl = rep_thresh_dl;
    qos_monitoring_data_local_var->is_rep_thresh_ul_null = is_rep_thresh_ul_null;
    qos_monitoring_data_local_var->is_rep_thresh_ul = is_rep_thresh_ul;
    qos_monitoring_data_local_var->rep_thresh_ul = rep_thresh_ul;
    qos_monitoring_data_local_var->is_rep_thresh_rp_null = is_rep_thresh_rp_null;
    qos_monitoring_data_local_var->is_rep_thresh_rp = is_rep_thresh_rp;
    qos_monitoring_data_local_var->rep_thresh_rp = rep_thresh_rp;
    qos_monitoring_data_local_var->is_con_thresh_dl_null = is_con_thresh_dl_null;
    qos_monitoring_data_local_var->is_con_thresh_dl = is_con_thresh_dl;
    qos_monitoring_data_local_var->con_thresh_dl = con_thresh_dl;
    qos_monitoring_data_local_var->is_con_thresh_ul_null = is_con_thresh_ul_null;
    qos_monitoring_data_local_var->is_con_thresh_ul = is_con_thresh_ul;
    qos_monitoring_data_local_var->con_thresh_ul = con_thresh_ul;
    qos_monitoring_data_local_var->is_wait_time_null = is_wait_time_null;
    qos_monitoring_data_local_var->is_wait_time = is_wait_time;
    qos_monitoring_data_local_var->wait_time = wait_time;
    qos_monitoring_data_local_var->is_rep_period_null = is_rep_period_null;
    qos_monitoring_data_local_var->is_rep_period = is_rep_period;
    qos_monitoring_data_local_var->rep_period = rep_period;
    qos_monitoring_data_local_var->is_notify_uri_null = is_notify_uri_null;
    qos_monitoring_data_local_var->notify_uri = notify_uri;
    qos_monitoring_data_local_var->is_notify_corre_id_null = is_notify_corre_id_null;
    qos_monitoring_data_local_var->notify_corre_id = notify_corre_id;
    qos_monitoring_data_local_var->is_direct_notif_ind = is_direct_notif_ind;
    qos_monitoring_data_local_var->direct_notif_ind = direct_notif_ind;
    qos_monitoring_data_local_var->is_avrg_wndw_null = is_avrg_wndw_null;
    qos_monitoring_data_local_var->is_avrg_wndw = is_avrg_wndw;
    qos_monitoring_data_local_var->avrg_wndw = avrg_wndw;
    qos_monitoring_data_local_var->is_rep_thresh_dat_rate_ul_null = is_rep_thresh_dat_rate_ul_null;
    qos_monitoring_data_local_var->rep_thresh_dat_rate_ul = rep_thresh_dat_rate_ul;
    qos_monitoring_data_local_var->is_rep_thresh_dat_rate_dl_null = is_rep_thresh_dat_rate_dl_null;
    qos_monitoring_data_local_var->rep_thresh_dat_rate_dl = rep_thresh_dat_rate_dl;
    qos_monitoring_data_local_var->avl_bitrate_ul_thrs = avl_bitrate_ul_thrs;
    qos_monitoring_data_local_var->avl_bitrate_dl_thrs = avl_bitrate_dl_thrs;
    qos_monitoring_data_local_var->data_coll_app_id = data_coll_app_id;

    return qos_monitoring_data_local_var;
}

void OpenAPI_qos_monitoring_data_free(OpenAPI_qos_monitoring_data_t *qos_monitoring_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_monitoring_data) {
        return;
    }
    if (qos_monitoring_data->qm_id) {
        ogs_free(qos_monitoring_data->qm_id);
        qos_monitoring_data->qm_id = NULL;
    }
    if (qos_monitoring_data->req_qos_mon_params) {
        OpenAPI_list_free(qos_monitoring_data->req_qos_mon_params);
        qos_monitoring_data->req_qos_mon_params = NULL;
    }
    if (qos_monitoring_data->rep_freqs) {
        OpenAPI_list_free(qos_monitoring_data->rep_freqs);
        qos_monitoring_data->rep_freqs = NULL;
    }
    if (qos_monitoring_data->notify_uri) {
        ogs_free(qos_monitoring_data->notify_uri);
        qos_monitoring_data->notify_uri = NULL;
    }
    if (qos_monitoring_data->notify_corre_id) {
        ogs_free(qos_monitoring_data->notify_corre_id);
        qos_monitoring_data->notify_corre_id = NULL;
    }
    if (qos_monitoring_data->rep_thresh_dat_rate_ul) {
        ogs_free(qos_monitoring_data->rep_thresh_dat_rate_ul);
        qos_monitoring_data->rep_thresh_dat_rate_ul = NULL;
    }
    if (qos_monitoring_data->rep_thresh_dat_rate_dl) {
        ogs_free(qos_monitoring_data->rep_thresh_dat_rate_dl);
        qos_monitoring_data->rep_thresh_dat_rate_dl = NULL;
    }
    if (qos_monitoring_data->avl_bitrate_ul_thrs) {
        OpenAPI_list_for_each(qos_monitoring_data->avl_bitrate_ul_thrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_data->avl_bitrate_ul_thrs);
        qos_monitoring_data->avl_bitrate_ul_thrs = NULL;
    }
    if (qos_monitoring_data->avl_bitrate_dl_thrs) {
        OpenAPI_list_for_each(qos_monitoring_data->avl_bitrate_dl_thrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qos_monitoring_data->avl_bitrate_dl_thrs);
        qos_monitoring_data->avl_bitrate_dl_thrs = NULL;
    }
    if (qos_monitoring_data->data_coll_app_id) {
        ogs_free(qos_monitoring_data->data_coll_app_id);
        qos_monitoring_data->data_coll_app_id = NULL;
    }
    ogs_free(qos_monitoring_data);
}

cJSON *OpenAPI_qos_monitoring_data_convertToJSON(OpenAPI_qos_monitoring_data_t *qos_monitoring_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_monitoring_data == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [QosMonitoringData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!qos_monitoring_data->qm_id) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [qm_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "qmId", qos_monitoring_data->qm_id) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [qm_id]");
        goto end;
    }

    if (qos_monitoring_data->qos_mon_param_type != OpenAPI_qos_monitoring_param_type_NULL) {
    if (cJSON_AddStringToObject(item, "qosMonParamType", OpenAPI_qos_monitoring_param_type_ToString(qos_monitoring_data->qos_mon_param_type)) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [qos_mon_param_type]");
        goto end;
    }
    }

    if (qos_monitoring_data->req_qos_mon_params == OpenAPI_requested_qos_monitoring_parameter_NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [req_qos_mon_params]");
        return NULL;
    }
    cJSON *req_qos_mon_paramsList = cJSON_AddArrayToObject(item, "reqQosMonParams");
    if (req_qos_mon_paramsList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [req_qos_mon_params]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_data->req_qos_mon_params, node) {
        if (cJSON_AddStringToObject(req_qos_mon_paramsList, "", OpenAPI_requested_qos_monitoring_parameter_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [req_qos_mon_params]");
            goto end;
        }
    }

    if (qos_monitoring_data->rep_freqs == OpenAPI_reporting_frequency_NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_freqs]");
        return NULL;
    }
    cJSON *rep_freqsList = cJSON_AddArrayToObject(item, "repFreqs");
    if (rep_freqsList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_freqs]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_data->rep_freqs, node) {
        if (cJSON_AddStringToObject(rep_freqsList, "", OpenAPI_reporting_frequency_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_freqs]");
            goto end;
        }
    }

    if (qos_monitoring_data->is_rep_thresh_dl) {
    if (cJSON_AddNumberToObject(item, "repThreshDl", qos_monitoring_data->rep_thresh_dl) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_dl]");
        goto end;
    }
    } else if (qos_monitoring_data->is_rep_thresh_dl_null) {
        if (cJSON_AddNullToObject(item, "repThreshDl") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_dl]");
            goto end;
        }
    }

    if (qos_monitoring_data->is_rep_thresh_ul) {
    if (cJSON_AddNumberToObject(item, "repThreshUl", qos_monitoring_data->rep_thresh_ul) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_ul]");
        goto end;
    }
    } else if (qos_monitoring_data->is_rep_thresh_ul_null) {
        if (cJSON_AddNullToObject(item, "repThreshUl") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_ul]");
            goto end;
        }
    }

    if (qos_monitoring_data->is_rep_thresh_rp) {
    if (cJSON_AddNumberToObject(item, "repThreshRp", qos_monitoring_data->rep_thresh_rp) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_rp]");
        goto end;
    }
    } else if (qos_monitoring_data->is_rep_thresh_rp_null) {
        if (cJSON_AddNullToObject(item, "repThreshRp") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_rp]");
            goto end;
        }
    }

    if (qos_monitoring_data->is_con_thresh_dl) {
    if (cJSON_AddNumberToObject(item, "conThreshDl", qos_monitoring_data->con_thresh_dl) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [con_thresh_dl]");
        goto end;
    }
    } else if (qos_monitoring_data->is_con_thresh_dl_null) {
        if (cJSON_AddNullToObject(item, "conThreshDl") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [con_thresh_dl]");
            goto end;
        }
    }

    if (qos_monitoring_data->is_con_thresh_ul) {
    if (cJSON_AddNumberToObject(item, "conThreshUl", qos_monitoring_data->con_thresh_ul) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [con_thresh_ul]");
        goto end;
    }
    } else if (qos_monitoring_data->is_con_thresh_ul_null) {
        if (cJSON_AddNullToObject(item, "conThreshUl") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [con_thresh_ul]");
            goto end;
        }
    }

    if (qos_monitoring_data->is_wait_time) {
    if (cJSON_AddNumberToObject(item, "waitTime", qos_monitoring_data->wait_time) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [wait_time]");
        goto end;
    }
    } else if (qos_monitoring_data->is_wait_time_null) {
        if (cJSON_AddNullToObject(item, "waitTime") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [wait_time]");
            goto end;
        }
    }

    if (qos_monitoring_data->is_rep_period) {
    if (cJSON_AddNumberToObject(item, "repPeriod", qos_monitoring_data->rep_period) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_period]");
        goto end;
    }
    } else if (qos_monitoring_data->is_rep_period_null) {
        if (cJSON_AddNullToObject(item, "repPeriod") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_period]");
            goto end;
        }
    }

    if (qos_monitoring_data->notify_uri) {
    if (cJSON_AddStringToObject(item, "notifyUri", qos_monitoring_data->notify_uri) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [notify_uri]");
        goto end;
    }
    } else if (qos_monitoring_data->is_notify_uri_null) {
        if (cJSON_AddNullToObject(item, "notifyUri") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [notify_uri]");
            goto end;
        }
    }

    if (qos_monitoring_data->notify_corre_id) {
    if (cJSON_AddStringToObject(item, "notifyCorreId", qos_monitoring_data->notify_corre_id) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [notify_corre_id]");
        goto end;
    }
    } else if (qos_monitoring_data->is_notify_corre_id_null) {
        if (cJSON_AddNullToObject(item, "notifyCorreId") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [notify_corre_id]");
            goto end;
        }
    }

    if (qos_monitoring_data->is_direct_notif_ind) {
    if (cJSON_AddBoolToObject(item, "directNotifInd", qos_monitoring_data->direct_notif_ind) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [direct_notif_ind]");
        goto end;
    }
    }

    if (qos_monitoring_data->is_avrg_wndw) {
    if (cJSON_AddNumberToObject(item, "avrgWndw", qos_monitoring_data->avrg_wndw) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [avrg_wndw]");
        goto end;
    }
    } else if (qos_monitoring_data->is_avrg_wndw_null) {
        if (cJSON_AddNullToObject(item, "avrgWndw") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [avrg_wndw]");
            goto end;
        }
    }

    if (qos_monitoring_data->rep_thresh_dat_rate_ul) {
    if (cJSON_AddStringToObject(item, "repThreshDatRateUl", qos_monitoring_data->rep_thresh_dat_rate_ul) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_dat_rate_ul]");
        goto end;
    }
    } else if (qos_monitoring_data->is_rep_thresh_dat_rate_ul_null) {
        if (cJSON_AddNullToObject(item, "repThreshDatRateUl") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_dat_rate_ul]");
            goto end;
        }
    }

    if (qos_monitoring_data->rep_thresh_dat_rate_dl) {
    if (cJSON_AddStringToObject(item, "repThreshDatRateDl", qos_monitoring_data->rep_thresh_dat_rate_dl) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_dat_rate_dl]");
        goto end;
    }
    } else if (qos_monitoring_data->is_rep_thresh_dat_rate_dl_null) {
        if (cJSON_AddNullToObject(item, "repThreshDatRateDl") == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [rep_thresh_dat_rate_dl]");
            goto end;
        }
    }

    if (qos_monitoring_data->avl_bitrate_ul_thrs) {
    cJSON *avl_bitrate_ul_thrsList = cJSON_AddArrayToObject(item, "avlBitrateUlThrs");
    if (avl_bitrate_ul_thrsList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [avl_bitrate_ul_thrs]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_data->avl_bitrate_ul_thrs, node) {
        if (cJSON_AddStringToObject(avl_bitrate_ul_thrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [avl_bitrate_ul_thrs]");
            goto end;
        }
    }
    }

    if (qos_monitoring_data->avl_bitrate_dl_thrs) {
    cJSON *avl_bitrate_dl_thrsList = cJSON_AddArrayToObject(item, "avlBitrateDlThrs");
    if (avl_bitrate_dl_thrsList == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [avl_bitrate_dl_thrs]");
        goto end;
    }
    OpenAPI_list_for_each(qos_monitoring_data->avl_bitrate_dl_thrs, node) {
        if (cJSON_AddStringToObject(avl_bitrate_dl_thrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [avl_bitrate_dl_thrs]");
            goto end;
        }
    }
    }

    if (qos_monitoring_data->data_coll_app_id) {
    if (cJSON_AddStringToObject(item, "dataCollAppId", qos_monitoring_data->data_coll_app_id) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed [data_coll_app_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_parseFromJSON(cJSON *qos_monitoring_dataJSON)
{
    OpenAPI_qos_monitoring_data_t *qos_monitoring_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qm_id = NULL;
    cJSON *qos_mon_param_type = NULL;
    OpenAPI_qos_monitoring_param_type_e qos_mon_param_typeVariable = 0;
    cJSON *req_qos_mon_params = NULL;
    OpenAPI_list_t *req_qos_mon_paramsList = NULL;
    cJSON *rep_freqs = NULL;
    OpenAPI_list_t *rep_freqsList = NULL;
    cJSON *rep_thresh_dl = NULL;
    cJSON *rep_thresh_ul = NULL;
    cJSON *rep_thresh_rp = NULL;
    cJSON *con_thresh_dl = NULL;
    cJSON *con_thresh_ul = NULL;
    cJSON *wait_time = NULL;
    cJSON *rep_period = NULL;
    cJSON *notify_uri = NULL;
    cJSON *notify_corre_id = NULL;
    cJSON *direct_notif_ind = NULL;
    cJSON *avrg_wndw = NULL;
    cJSON *rep_thresh_dat_rate_ul = NULL;
    cJSON *rep_thresh_dat_rate_dl = NULL;
    cJSON *avl_bitrate_ul_thrs = NULL;
    OpenAPI_list_t *avl_bitrate_ul_thrsList = NULL;
    cJSON *avl_bitrate_dl_thrs = NULL;
    OpenAPI_list_t *avl_bitrate_dl_thrsList = NULL;
    cJSON *data_coll_app_id = NULL;
    qm_id = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "qmId");
    if (!qm_id) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [qm_id]");
        goto end;
    }
    if (!cJSON_IsString(qm_id)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [qm_id]");
        goto end;
    }

    qos_mon_param_type = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "qosMonParamType");
    if (qos_mon_param_type) {
    if (!cJSON_IsString(qos_mon_param_type)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [qos_mon_param_type]");
        goto end;
    }
    qos_mon_param_typeVariable = OpenAPI_qos_monitoring_param_type_FromString(qos_mon_param_type->valuestring);
    }

    req_qos_mon_params = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "reqQosMonParams");
    if (!req_qos_mon_params) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [req_qos_mon_params]");
        goto end;
    }
        cJSON *req_qos_mon_params_local = NULL;
        if (!cJSON_IsArray(req_qos_mon_params)) {
            ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [req_qos_mon_params]");
            goto end;
        }

        req_qos_mon_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_qos_mon_params_local, req_qos_mon_params) {
            OpenAPI_requested_qos_monitoring_parameter_e localEnum = OpenAPI_requested_qos_monitoring_parameter_NULL;
            if (!cJSON_IsString(req_qos_mon_params_local)) {
                ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [req_qos_mon_params]");
                goto end;
            }
            localEnum = OpenAPI_requested_qos_monitoring_parameter_FromString(req_qos_mon_params_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"req_qos_mon_params\" is not supported. Ignoring it ...",
                         req_qos_mon_params_local->valuestring);
            } else {
                OpenAPI_list_add(req_qos_mon_paramsList, (void *)localEnum);
            }
        }
        if (req_qos_mon_paramsList->count == 0) {
            ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed: Expected req_qos_mon_paramsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    rep_freqs = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "repFreqs");
    if (!rep_freqs) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_freqs]");
        goto end;
    }
        cJSON *rep_freqs_local = NULL;
        if (!cJSON_IsArray(rep_freqs)) {
            ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_freqs]");
            goto end;
        }

        rep_freqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rep_freqs_local, rep_freqs) {
            OpenAPI_reporting_frequency_e localEnum = OpenAPI_reporting_frequency_NULL;
            if (!cJSON_IsString(rep_freqs_local)) {
                ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_freqs]");
                goto end;
            }
            localEnum = OpenAPI_reporting_frequency_FromString(rep_freqs_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rep_freqs\" is not supported. Ignoring it ...",
                         rep_freqs_local->valuestring);
            } else {
                OpenAPI_list_add(rep_freqsList, (void *)localEnum);
            }
        }
        if (rep_freqsList->count == 0) {
            ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed: Expected rep_freqsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    rep_thresh_dl = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "repThreshDl");
    if (rep_thresh_dl) {
    if (!cJSON_IsNull(rep_thresh_dl)) {
    if (!cJSON_IsNumber(rep_thresh_dl)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_thresh_dl]");
        goto end;
    }
    }
    }

    rep_thresh_ul = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "repThreshUl");
    if (rep_thresh_ul) {
    if (!cJSON_IsNull(rep_thresh_ul)) {
    if (!cJSON_IsNumber(rep_thresh_ul)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_thresh_ul]");
        goto end;
    }
    }
    }

    rep_thresh_rp = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "repThreshRp");
    if (rep_thresh_rp) {
    if (!cJSON_IsNull(rep_thresh_rp)) {
    if (!cJSON_IsNumber(rep_thresh_rp)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_thresh_rp]");
        goto end;
    }
    }
    }

    con_thresh_dl = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "conThreshDl");
    if (con_thresh_dl) {
    if (!cJSON_IsNull(con_thresh_dl)) {
    if (!cJSON_IsNumber(con_thresh_dl)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [con_thresh_dl]");
        goto end;
    }
    }
    }

    con_thresh_ul = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "conThreshUl");
    if (con_thresh_ul) {
    if (!cJSON_IsNull(con_thresh_ul)) {
    if (!cJSON_IsNumber(con_thresh_ul)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [con_thresh_ul]");
        goto end;
    }
    }
    }

    wait_time = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "waitTime");
    if (wait_time) {
    if (!cJSON_IsNull(wait_time)) {
    if (!cJSON_IsNumber(wait_time)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [wait_time]");
        goto end;
    }
    }
    }

    rep_period = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "repPeriod");
    if (rep_period) {
    if (!cJSON_IsNull(rep_period)) {
    if (!cJSON_IsNumber(rep_period)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_period]");
        goto end;
    }
    }
    }

    notify_uri = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "notifyUri");
    if (notify_uri) {
    if (!cJSON_IsNull(notify_uri)) {
    if (!cJSON_IsString(notify_uri) && !cJSON_IsNull(notify_uri)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [notify_uri]");
        goto end;
    }
    }
    }

    notify_corre_id = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "notifyCorreId");
    if (notify_corre_id) {
    if (!cJSON_IsNull(notify_corre_id)) {
    if (!cJSON_IsString(notify_corre_id) && !cJSON_IsNull(notify_corre_id)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [notify_corre_id]");
        goto end;
    }
    }
    }

    direct_notif_ind = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "directNotifInd");
    if (direct_notif_ind) {
    if (!cJSON_IsBool(direct_notif_ind)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [direct_notif_ind]");
        goto end;
    }
    }

    avrg_wndw = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "avrgWndw");
    if (avrg_wndw) {
    if (!cJSON_IsNull(avrg_wndw)) {
    if (!cJSON_IsNumber(avrg_wndw)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [avrg_wndw]");
        goto end;
    }
    }
    }

    rep_thresh_dat_rate_ul = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "repThreshDatRateUl");
    if (rep_thresh_dat_rate_ul) {
    if (!cJSON_IsNull(rep_thresh_dat_rate_ul)) {
    if (!cJSON_IsString(rep_thresh_dat_rate_ul) && !cJSON_IsNull(rep_thresh_dat_rate_ul)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_thresh_dat_rate_ul]");
        goto end;
    }
    }
    }

    rep_thresh_dat_rate_dl = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "repThreshDatRateDl");
    if (rep_thresh_dat_rate_dl) {
    if (!cJSON_IsNull(rep_thresh_dat_rate_dl)) {
    if (!cJSON_IsString(rep_thresh_dat_rate_dl) && !cJSON_IsNull(rep_thresh_dat_rate_dl)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [rep_thresh_dat_rate_dl]");
        goto end;
    }
    }
    }

    avl_bitrate_ul_thrs = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "avlBitrateUlThrs");
    if (avl_bitrate_ul_thrs) {
        cJSON *avl_bitrate_ul_thrs_local = NULL;
        if (!cJSON_IsArray(avl_bitrate_ul_thrs)) {
            ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [avl_bitrate_ul_thrs]");
            goto end;
        }

        avl_bitrate_ul_thrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(avl_bitrate_ul_thrs_local, avl_bitrate_ul_thrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(avl_bitrate_ul_thrs_local)) {
                ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [avl_bitrate_ul_thrs]");
                goto end;
            }
            OpenAPI_list_add(avl_bitrate_ul_thrsList, ogs_strdup(avl_bitrate_ul_thrs_local->valuestring));
        }
    }

    avl_bitrate_dl_thrs = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "avlBitrateDlThrs");
    if (avl_bitrate_dl_thrs) {
        cJSON *avl_bitrate_dl_thrs_local = NULL;
        if (!cJSON_IsArray(avl_bitrate_dl_thrs)) {
            ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [avl_bitrate_dl_thrs]");
            goto end;
        }

        avl_bitrate_dl_thrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(avl_bitrate_dl_thrs_local, avl_bitrate_dl_thrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(avl_bitrate_dl_thrs_local)) {
                ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [avl_bitrate_dl_thrs]");
                goto end;
            }
            OpenAPI_list_add(avl_bitrate_dl_thrsList, ogs_strdup(avl_bitrate_dl_thrs_local->valuestring));
        }
    }

    data_coll_app_id = cJSON_GetObjectItemCaseSensitive(qos_monitoring_dataJSON, "dataCollAppId");
    if (data_coll_app_id) {
    if (!cJSON_IsString(data_coll_app_id) && !cJSON_IsNull(data_coll_app_id)) {
        ogs_error("OpenAPI_qos_monitoring_data_parseFromJSON() failed [data_coll_app_id]");
        goto end;
    }
    }

    qos_monitoring_data_local_var = OpenAPI_qos_monitoring_data_create (
        ogs_strdup(qm_id->valuestring),
        qos_mon_param_type ? qos_mon_param_typeVariable : 0,
        req_qos_mon_paramsList,
        rep_freqsList,
        rep_thresh_dl && cJSON_IsNull(rep_thresh_dl) ? true : false,
        rep_thresh_dl ? true : false,
        rep_thresh_dl ? rep_thresh_dl->valuedouble : 0,
        rep_thresh_ul && cJSON_IsNull(rep_thresh_ul) ? true : false,
        rep_thresh_ul ? true : false,
        rep_thresh_ul ? rep_thresh_ul->valuedouble : 0,
        rep_thresh_rp && cJSON_IsNull(rep_thresh_rp) ? true : false,
        rep_thresh_rp ? true : false,
        rep_thresh_rp ? rep_thresh_rp->valuedouble : 0,
        con_thresh_dl && cJSON_IsNull(con_thresh_dl) ? true : false,
        con_thresh_dl ? true : false,
        con_thresh_dl ? con_thresh_dl->valuedouble : 0,
        con_thresh_ul && cJSON_IsNull(con_thresh_ul) ? true : false,
        con_thresh_ul ? true : false,
        con_thresh_ul ? con_thresh_ul->valuedouble : 0,
        wait_time && cJSON_IsNull(wait_time) ? true : false,
        wait_time ? true : false,
        wait_time ? wait_time->valuedouble : 0,
        rep_period && cJSON_IsNull(rep_period) ? true : false,
        rep_period ? true : false,
        rep_period ? rep_period->valuedouble : 0,
        notify_uri && cJSON_IsNull(notify_uri) ? true : false,
        notify_uri && !cJSON_IsNull(notify_uri) ? ogs_strdup(notify_uri->valuestring) : NULL,
        notify_corre_id && cJSON_IsNull(notify_corre_id) ? true : false,
        notify_corre_id && !cJSON_IsNull(notify_corre_id) ? ogs_strdup(notify_corre_id->valuestring) : NULL,
        direct_notif_ind ? true : false,
        direct_notif_ind ? direct_notif_ind->valueint : 0,
        avrg_wndw && cJSON_IsNull(avrg_wndw) ? true : false,
        avrg_wndw ? true : false,
        avrg_wndw ? avrg_wndw->valuedouble : 0,
        rep_thresh_dat_rate_ul && cJSON_IsNull(rep_thresh_dat_rate_ul) ? true : false,
        rep_thresh_dat_rate_ul && !cJSON_IsNull(rep_thresh_dat_rate_ul) ? ogs_strdup(rep_thresh_dat_rate_ul->valuestring) : NULL,
        rep_thresh_dat_rate_dl && cJSON_IsNull(rep_thresh_dat_rate_dl) ? true : false,
        rep_thresh_dat_rate_dl && !cJSON_IsNull(rep_thresh_dat_rate_dl) ? ogs_strdup(rep_thresh_dat_rate_dl->valuestring) : NULL,
        avl_bitrate_ul_thrs ? avl_bitrate_ul_thrsList : NULL,
        avl_bitrate_dl_thrs ? avl_bitrate_dl_thrsList : NULL,
        data_coll_app_id && !cJSON_IsNull(data_coll_app_id) ? ogs_strdup(data_coll_app_id->valuestring) : NULL
    );

    return qos_monitoring_data_local_var;
end:
    if (req_qos_mon_paramsList) {
        OpenAPI_list_free(req_qos_mon_paramsList);
        req_qos_mon_paramsList = NULL;
    }
    if (rep_freqsList) {
        OpenAPI_list_free(rep_freqsList);
        rep_freqsList = NULL;
    }
    if (avl_bitrate_ul_thrsList) {
        OpenAPI_list_for_each(avl_bitrate_ul_thrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(avl_bitrate_ul_thrsList);
        avl_bitrate_ul_thrsList = NULL;
    }
    if (avl_bitrate_dl_thrsList) {
        OpenAPI_list_for_each(avl_bitrate_dl_thrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(avl_bitrate_dl_thrsList);
        avl_bitrate_dl_thrsList = NULL;
    }
    return NULL;
}

OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_copy(OpenAPI_qos_monitoring_data_t *dst, OpenAPI_qos_monitoring_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_monitoring_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_monitoring_data_convertToJSON() failed");
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

    OpenAPI_qos_monitoring_data_free(dst);
    dst = OpenAPI_qos_monitoring_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

