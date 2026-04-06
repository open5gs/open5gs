
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_notification.h"

OpenAPI_event_notification_t *OpenAPI_event_notification_create(
    OpenAPI_nwdaf_event_e event,
    char *start,
    char *expiry,
    char *time_stamp_gen,
    OpenAPI_nwdaf_failure_code_e fail_notify_code,
    bool is_rv_wait_time,
    int rv_wait_time,
    OpenAPI_analytics_metadata_info_t *ana_meta_info,
    OpenAPI_list_t *nf_load_level_infos,
    OpenAPI_list_t *nsi_load_level_infos,
    OpenAPI_list_t *pfd_determ_infos,
    OpenAPI_slice_load_level_information_t *slice_load_level_info,
    OpenAPI_list_t *svc_exps,
    OpenAPI_list_t *qos_sustain_infos,
    OpenAPI_list_t *ue_comms,
    OpenAPI_list_t *ue_mobs,
    OpenAPI_list_t *user_data_cong_infos,
    OpenAPI_list_t *abnor_behavrs,
    OpenAPI_list_t *nw_perfs,
    OpenAPI_list_t *dn_perf_infos,
    OpenAPI_list_t *disper_infos,
    OpenAPI_list_t *red_trans_infos,
    OpenAPI_list_t *wlan_infos,
    OpenAPI_list_t *smcc_exps,
    OpenAPI_list_t *pdu_ses_traf_infos,
    OpenAPI_list_t *data_vl_trns_tm_infos,
    OpenAPI_accuracy_info_t *accu_info,
    bool is_cancel_accu_ind,
    int cancel_accu_ind,
    bool is_pause_ind,
    int pause_ind,
    bool is_resume_ind,
    int resume_ind,
    OpenAPI_list_t *mov_behav_infos,
    OpenAPI_list_t *loc_acc_infos,
    OpenAPI_list_t *rel_prox_infos,
    OpenAPI_list_t *signal_storm_infos,
    OpenAPI_list_t *qos_pol_assist_infos
)
{
    OpenAPI_event_notification_t *event_notification_local_var = ogs_malloc(sizeof(OpenAPI_event_notification_t));
    ogs_assert(event_notification_local_var);

    event_notification_local_var->event = event;
    event_notification_local_var->start = start;
    event_notification_local_var->expiry = expiry;
    event_notification_local_var->time_stamp_gen = time_stamp_gen;
    event_notification_local_var->fail_notify_code = fail_notify_code;
    event_notification_local_var->is_rv_wait_time = is_rv_wait_time;
    event_notification_local_var->rv_wait_time = rv_wait_time;
    event_notification_local_var->ana_meta_info = ana_meta_info;
    event_notification_local_var->nf_load_level_infos = nf_load_level_infos;
    event_notification_local_var->nsi_load_level_infos = nsi_load_level_infos;
    event_notification_local_var->pfd_determ_infos = pfd_determ_infos;
    event_notification_local_var->slice_load_level_info = slice_load_level_info;
    event_notification_local_var->svc_exps = svc_exps;
    event_notification_local_var->qos_sustain_infos = qos_sustain_infos;
    event_notification_local_var->ue_comms = ue_comms;
    event_notification_local_var->ue_mobs = ue_mobs;
    event_notification_local_var->user_data_cong_infos = user_data_cong_infos;
    event_notification_local_var->abnor_behavrs = abnor_behavrs;
    event_notification_local_var->nw_perfs = nw_perfs;
    event_notification_local_var->dn_perf_infos = dn_perf_infos;
    event_notification_local_var->disper_infos = disper_infos;
    event_notification_local_var->red_trans_infos = red_trans_infos;
    event_notification_local_var->wlan_infos = wlan_infos;
    event_notification_local_var->smcc_exps = smcc_exps;
    event_notification_local_var->pdu_ses_traf_infos = pdu_ses_traf_infos;
    event_notification_local_var->data_vl_trns_tm_infos = data_vl_trns_tm_infos;
    event_notification_local_var->accu_info = accu_info;
    event_notification_local_var->is_cancel_accu_ind = is_cancel_accu_ind;
    event_notification_local_var->cancel_accu_ind = cancel_accu_ind;
    event_notification_local_var->is_pause_ind = is_pause_ind;
    event_notification_local_var->pause_ind = pause_ind;
    event_notification_local_var->is_resume_ind = is_resume_ind;
    event_notification_local_var->resume_ind = resume_ind;
    event_notification_local_var->mov_behav_infos = mov_behav_infos;
    event_notification_local_var->loc_acc_infos = loc_acc_infos;
    event_notification_local_var->rel_prox_infos = rel_prox_infos;
    event_notification_local_var->signal_storm_infos = signal_storm_infos;
    event_notification_local_var->qos_pol_assist_infos = qos_pol_assist_infos;

    return event_notification_local_var;
}

void OpenAPI_event_notification_free(OpenAPI_event_notification_t *event_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event_notification) {
        return;
    }
    if (event_notification->start) {
        ogs_free(event_notification->start);
        event_notification->start = NULL;
    }
    if (event_notification->expiry) {
        ogs_free(event_notification->expiry);
        event_notification->expiry = NULL;
    }
    if (event_notification->time_stamp_gen) {
        ogs_free(event_notification->time_stamp_gen);
        event_notification->time_stamp_gen = NULL;
    }
    if (event_notification->ana_meta_info) {
        OpenAPI_analytics_metadata_info_free(event_notification->ana_meta_info);
        event_notification->ana_meta_info = NULL;
    }
    if (event_notification->nf_load_level_infos) {
        OpenAPI_list_for_each(event_notification->nf_load_level_infos, node) {
            OpenAPI_nf_load_level_information_free(node->data);
        }
        OpenAPI_list_free(event_notification->nf_load_level_infos);
        event_notification->nf_load_level_infos = NULL;
    }
    if (event_notification->nsi_load_level_infos) {
        OpenAPI_list_for_each(event_notification->nsi_load_level_infos, node) {
            OpenAPI_nsi_load_level_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->nsi_load_level_infos);
        event_notification->nsi_load_level_infos = NULL;
    }
    if (event_notification->pfd_determ_infos) {
        OpenAPI_list_for_each(event_notification->pfd_determ_infos, node) {
            OpenAPI_pfd_determination_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->pfd_determ_infos);
        event_notification->pfd_determ_infos = NULL;
    }
    if (event_notification->slice_load_level_info) {
        OpenAPI_slice_load_level_information_free(event_notification->slice_load_level_info);
        event_notification->slice_load_level_info = NULL;
    }
    if (event_notification->svc_exps) {
        OpenAPI_list_for_each(event_notification->svc_exps, node) {
            OpenAPI_service_experience_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->svc_exps);
        event_notification->svc_exps = NULL;
    }
    if (event_notification->qos_sustain_infos) {
        OpenAPI_list_for_each(event_notification->qos_sustain_infos, node) {
            OpenAPI_qos_sustainability_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->qos_sustain_infos);
        event_notification->qos_sustain_infos = NULL;
    }
    if (event_notification->ue_comms) {
        OpenAPI_list_for_each(event_notification->ue_comms, node) {
            OpenAPI_ue_communication_free(node->data);
        }
        OpenAPI_list_free(event_notification->ue_comms);
        event_notification->ue_comms = NULL;
    }
    if (event_notification->ue_mobs) {
        OpenAPI_list_for_each(event_notification->ue_mobs, node) {
            OpenAPI_ue_mobility_free(node->data);
        }
        OpenAPI_list_free(event_notification->ue_mobs);
        event_notification->ue_mobs = NULL;
    }
    if (event_notification->user_data_cong_infos) {
        OpenAPI_list_for_each(event_notification->user_data_cong_infos, node) {
            OpenAPI_user_data_congestion_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->user_data_cong_infos);
        event_notification->user_data_cong_infos = NULL;
    }
    if (event_notification->abnor_behavrs) {
        OpenAPI_list_for_each(event_notification->abnor_behavrs, node) {
            OpenAPI_abnormal_behaviour_free(node->data);
        }
        OpenAPI_list_free(event_notification->abnor_behavrs);
        event_notification->abnor_behavrs = NULL;
    }
    if (event_notification->nw_perfs) {
        OpenAPI_list_for_each(event_notification->nw_perfs, node) {
            OpenAPI_network_perf_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->nw_perfs);
        event_notification->nw_perfs = NULL;
    }
    if (event_notification->dn_perf_infos) {
        OpenAPI_list_for_each(event_notification->dn_perf_infos, node) {
            OpenAPI_dn_perf_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->dn_perf_infos);
        event_notification->dn_perf_infos = NULL;
    }
    if (event_notification->disper_infos) {
        OpenAPI_list_for_each(event_notification->disper_infos, node) {
            OpenAPI_dispersion_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->disper_infos);
        event_notification->disper_infos = NULL;
    }
    if (event_notification->red_trans_infos) {
        OpenAPI_list_for_each(event_notification->red_trans_infos, node) {
            OpenAPI_redundant_transmission_exp_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->red_trans_infos);
        event_notification->red_trans_infos = NULL;
    }
    if (event_notification->wlan_infos) {
        OpenAPI_list_for_each(event_notification->wlan_infos, node) {
            OpenAPI_wlan_performance_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->wlan_infos);
        event_notification->wlan_infos = NULL;
    }
    if (event_notification->smcc_exps) {
        OpenAPI_list_for_each(event_notification->smcc_exps, node) {
            OpenAPI_smcce_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->smcc_exps);
        event_notification->smcc_exps = NULL;
    }
    if (event_notification->pdu_ses_traf_infos) {
        OpenAPI_list_for_each(event_notification->pdu_ses_traf_infos, node) {
            OpenAPI_pdu_ses_traffic_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->pdu_ses_traf_infos);
        event_notification->pdu_ses_traf_infos = NULL;
    }
    if (event_notification->data_vl_trns_tm_infos) {
        OpenAPI_list_for_each(event_notification->data_vl_trns_tm_infos, node) {
            OpenAPI_e2e_data_vol_trans_time_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->data_vl_trns_tm_infos);
        event_notification->data_vl_trns_tm_infos = NULL;
    }
    if (event_notification->accu_info) {
        OpenAPI_accuracy_info_free(event_notification->accu_info);
        event_notification->accu_info = NULL;
    }
    if (event_notification->mov_behav_infos) {
        OpenAPI_list_for_each(event_notification->mov_behav_infos, node) {
            OpenAPI_mov_behav_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->mov_behav_infos);
        event_notification->mov_behav_infos = NULL;
    }
    if (event_notification->loc_acc_infos) {
        OpenAPI_list_for_each(event_notification->loc_acc_infos, node) {
            OpenAPI_loc_accuracy_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->loc_acc_infos);
        event_notification->loc_acc_infos = NULL;
    }
    if (event_notification->rel_prox_infos) {
        OpenAPI_list_for_each(event_notification->rel_prox_infos, node) {
            OpenAPI_rel_prox_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->rel_prox_infos);
        event_notification->rel_prox_infos = NULL;
    }
    if (event_notification->signal_storm_infos) {
        OpenAPI_list_for_each(event_notification->signal_storm_infos, node) {
            OpenAPI_signal_storm_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->signal_storm_infos);
        event_notification->signal_storm_infos = NULL;
    }
    if (event_notification->qos_pol_assist_infos) {
        OpenAPI_list_for_each(event_notification->qos_pol_assist_infos, node) {
            OpenAPI_qos_policy_assist_info_free(node->data);
        }
        OpenAPI_list_free(event_notification->qos_pol_assist_infos);
        event_notification->qos_pol_assist_infos = NULL;
    }
    ogs_free(event_notification);
}

cJSON *OpenAPI_event_notification_convertToJSON(OpenAPI_event_notification_t *event_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (event_notification == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [EventNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (event_notification->event == OpenAPI_nwdaf_event_NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [event]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "event", OpenAPI_nwdaf_event_ToString(event_notification->event)) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [event]");
        goto end;
    }

    if (event_notification->start) {
    if (cJSON_AddStringToObject(item, "start", event_notification->start) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (event_notification->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", event_notification->expiry) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (event_notification->time_stamp_gen) {
    if (cJSON_AddStringToObject(item, "timeStampGen", event_notification->time_stamp_gen) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [time_stamp_gen]");
        goto end;
    }
    }

    if (event_notification->fail_notify_code != OpenAPI_nwdaf_failure_code_NULL) {
    if (cJSON_AddStringToObject(item, "failNotifyCode", OpenAPI_nwdaf_failure_code_ToString(event_notification->fail_notify_code)) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [fail_notify_code]");
        goto end;
    }
    }

    if (event_notification->is_rv_wait_time) {
    if (cJSON_AddNumberToObject(item, "rvWaitTime", event_notification->rv_wait_time) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [rv_wait_time]");
        goto end;
    }
    }

    if (event_notification->ana_meta_info) {
    cJSON *ana_meta_info_local_JSON = OpenAPI_analytics_metadata_info_convertToJSON(event_notification->ana_meta_info);
    if (ana_meta_info_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [ana_meta_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anaMetaInfo", ana_meta_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [ana_meta_info]");
        goto end;
    }
    }

    if (event_notification->nf_load_level_infos) {
    cJSON *nf_load_level_infosList = cJSON_AddArrayToObject(item, "nfLoadLevelInfos");
    if (nf_load_level_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [nf_load_level_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->nf_load_level_infos, node) {
        cJSON *itemLocal = OpenAPI_nf_load_level_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [nf_load_level_infos]");
            goto end;
        }
        cJSON_AddItemToArray(nf_load_level_infosList, itemLocal);
    }
    }

    if (event_notification->nsi_load_level_infos) {
    cJSON *nsi_load_level_infosList = cJSON_AddArrayToObject(item, "nsiLoadLevelInfos");
    if (nsi_load_level_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [nsi_load_level_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->nsi_load_level_infos, node) {
        cJSON *itemLocal = OpenAPI_nsi_load_level_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [nsi_load_level_infos]");
            goto end;
        }
        cJSON_AddItemToArray(nsi_load_level_infosList, itemLocal);
    }
    }

    if (event_notification->pfd_determ_infos) {
    cJSON *pfd_determ_infosList = cJSON_AddArrayToObject(item, "pfdDetermInfos");
    if (pfd_determ_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [pfd_determ_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->pfd_determ_infos, node) {
        cJSON *itemLocal = OpenAPI_pfd_determination_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [pfd_determ_infos]");
            goto end;
        }
        cJSON_AddItemToArray(pfd_determ_infosList, itemLocal);
    }
    }

    if (event_notification->slice_load_level_info) {
    cJSON *slice_load_level_info_local_JSON = OpenAPI_slice_load_level_information_convertToJSON(event_notification->slice_load_level_info);
    if (slice_load_level_info_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [slice_load_level_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sliceLoadLevelInfo", slice_load_level_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [slice_load_level_info]");
        goto end;
    }
    }

    if (event_notification->svc_exps) {
    cJSON *svc_expsList = cJSON_AddArrayToObject(item, "svcExps");
    if (svc_expsList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [svc_exps]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->svc_exps, node) {
        cJSON *itemLocal = OpenAPI_service_experience_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [svc_exps]");
            goto end;
        }
        cJSON_AddItemToArray(svc_expsList, itemLocal);
    }
    }

    if (event_notification->qos_sustain_infos) {
    cJSON *qos_sustain_infosList = cJSON_AddArrayToObject(item, "qosSustainInfos");
    if (qos_sustain_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [qos_sustain_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->qos_sustain_infos, node) {
        cJSON *itemLocal = OpenAPI_qos_sustainability_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [qos_sustain_infos]");
            goto end;
        }
        cJSON_AddItemToArray(qos_sustain_infosList, itemLocal);
    }
    }

    if (event_notification->ue_comms) {
    cJSON *ue_commsList = cJSON_AddArrayToObject(item, "ueComms");
    if (ue_commsList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [ue_comms]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->ue_comms, node) {
        cJSON *itemLocal = OpenAPI_ue_communication_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [ue_comms]");
            goto end;
        }
        cJSON_AddItemToArray(ue_commsList, itemLocal);
    }
    }

    if (event_notification->ue_mobs) {
    cJSON *ue_mobsList = cJSON_AddArrayToObject(item, "ueMobs");
    if (ue_mobsList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [ue_mobs]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->ue_mobs, node) {
        cJSON *itemLocal = OpenAPI_ue_mobility_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [ue_mobs]");
            goto end;
        }
        cJSON_AddItemToArray(ue_mobsList, itemLocal);
    }
    }

    if (event_notification->user_data_cong_infos) {
    cJSON *user_data_cong_infosList = cJSON_AddArrayToObject(item, "userDataCongInfos");
    if (user_data_cong_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [user_data_cong_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->user_data_cong_infos, node) {
        cJSON *itemLocal = OpenAPI_user_data_congestion_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [user_data_cong_infos]");
            goto end;
        }
        cJSON_AddItemToArray(user_data_cong_infosList, itemLocal);
    }
    }

    if (event_notification->abnor_behavrs) {
    cJSON *abnor_behavrsList = cJSON_AddArrayToObject(item, "abnorBehavrs");
    if (abnor_behavrsList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [abnor_behavrs]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->abnor_behavrs, node) {
        cJSON *itemLocal = OpenAPI_abnormal_behaviour_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [abnor_behavrs]");
            goto end;
        }
        cJSON_AddItemToArray(abnor_behavrsList, itemLocal);
    }
    }

    if (event_notification->nw_perfs) {
    cJSON *nw_perfsList = cJSON_AddArrayToObject(item, "nwPerfs");
    if (nw_perfsList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [nw_perfs]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->nw_perfs, node) {
        cJSON *itemLocal = OpenAPI_network_perf_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [nw_perfs]");
            goto end;
        }
        cJSON_AddItemToArray(nw_perfsList, itemLocal);
    }
    }

    if (event_notification->dn_perf_infos) {
    cJSON *dn_perf_infosList = cJSON_AddArrayToObject(item, "dnPerfInfos");
    if (dn_perf_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [dn_perf_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->dn_perf_infos, node) {
        cJSON *itemLocal = OpenAPI_dn_perf_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [dn_perf_infos]");
            goto end;
        }
        cJSON_AddItemToArray(dn_perf_infosList, itemLocal);
    }
    }

    if (event_notification->disper_infos) {
    cJSON *disper_infosList = cJSON_AddArrayToObject(item, "disperInfos");
    if (disper_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [disper_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->disper_infos, node) {
        cJSON *itemLocal = OpenAPI_dispersion_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [disper_infos]");
            goto end;
        }
        cJSON_AddItemToArray(disper_infosList, itemLocal);
    }
    }

    if (event_notification->red_trans_infos) {
    cJSON *red_trans_infosList = cJSON_AddArrayToObject(item, "redTransInfos");
    if (red_trans_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [red_trans_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->red_trans_infos, node) {
        cJSON *itemLocal = OpenAPI_redundant_transmission_exp_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [red_trans_infos]");
            goto end;
        }
        cJSON_AddItemToArray(red_trans_infosList, itemLocal);
    }
    }

    if (event_notification->wlan_infos) {
    cJSON *wlan_infosList = cJSON_AddArrayToObject(item, "wlanInfos");
    if (wlan_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [wlan_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->wlan_infos, node) {
        cJSON *itemLocal = OpenAPI_wlan_performance_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [wlan_infos]");
            goto end;
        }
        cJSON_AddItemToArray(wlan_infosList, itemLocal);
    }
    }

    if (event_notification->smcc_exps) {
    cJSON *smcc_expsList = cJSON_AddArrayToObject(item, "smccExps");
    if (smcc_expsList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [smcc_exps]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->smcc_exps, node) {
        cJSON *itemLocal = OpenAPI_smcce_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [smcc_exps]");
            goto end;
        }
        cJSON_AddItemToArray(smcc_expsList, itemLocal);
    }
    }

    if (event_notification->pdu_ses_traf_infos) {
    cJSON *pdu_ses_traf_infosList = cJSON_AddArrayToObject(item, "pduSesTrafInfos");
    if (pdu_ses_traf_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [pdu_ses_traf_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->pdu_ses_traf_infos, node) {
        cJSON *itemLocal = OpenAPI_pdu_ses_traffic_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [pdu_ses_traf_infos]");
            goto end;
        }
        cJSON_AddItemToArray(pdu_ses_traf_infosList, itemLocal);
    }
    }

    if (event_notification->data_vl_trns_tm_infos) {
    cJSON *data_vl_trns_tm_infosList = cJSON_AddArrayToObject(item, "dataVlTrnsTmInfos");
    if (data_vl_trns_tm_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [data_vl_trns_tm_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->data_vl_trns_tm_infos, node) {
        cJSON *itemLocal = OpenAPI_e2e_data_vol_trans_time_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [data_vl_trns_tm_infos]");
            goto end;
        }
        cJSON_AddItemToArray(data_vl_trns_tm_infosList, itemLocal);
    }
    }

    if (event_notification->accu_info) {
    cJSON *accu_info_local_JSON = OpenAPI_accuracy_info_convertToJSON(event_notification->accu_info);
    if (accu_info_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [accu_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accuInfo", accu_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [accu_info]");
        goto end;
    }
    }

    if (event_notification->is_cancel_accu_ind) {
    if (cJSON_AddBoolToObject(item, "cancelAccuInd", event_notification->cancel_accu_ind) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [cancel_accu_ind]");
        goto end;
    }
    }

    if (event_notification->is_pause_ind) {
    if (cJSON_AddBoolToObject(item, "pauseInd", event_notification->pause_ind) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [pause_ind]");
        goto end;
    }
    }

    if (event_notification->is_resume_ind) {
    if (cJSON_AddBoolToObject(item, "resumeInd", event_notification->resume_ind) == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [resume_ind]");
        goto end;
    }
    }

    if (event_notification->mov_behav_infos) {
    cJSON *mov_behav_infosList = cJSON_AddArrayToObject(item, "movBehavInfos");
    if (mov_behav_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [mov_behav_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->mov_behav_infos, node) {
        cJSON *itemLocal = OpenAPI_mov_behav_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [mov_behav_infos]");
            goto end;
        }
        cJSON_AddItemToArray(mov_behav_infosList, itemLocal);
    }
    }

    if (event_notification->loc_acc_infos) {
    cJSON *loc_acc_infosList = cJSON_AddArrayToObject(item, "locAccInfos");
    if (loc_acc_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [loc_acc_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->loc_acc_infos, node) {
        cJSON *itemLocal = OpenAPI_loc_accuracy_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [loc_acc_infos]");
            goto end;
        }
        cJSON_AddItemToArray(loc_acc_infosList, itemLocal);
    }
    }

    if (event_notification->rel_prox_infos) {
    cJSON *rel_prox_infosList = cJSON_AddArrayToObject(item, "relProxInfos");
    if (rel_prox_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [rel_prox_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->rel_prox_infos, node) {
        cJSON *itemLocal = OpenAPI_rel_prox_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [rel_prox_infos]");
            goto end;
        }
        cJSON_AddItemToArray(rel_prox_infosList, itemLocal);
    }
    }

    if (event_notification->signal_storm_infos) {
    cJSON *signal_storm_infosList = cJSON_AddArrayToObject(item, "signalStormInfos");
    if (signal_storm_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [signal_storm_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->signal_storm_infos, node) {
        cJSON *itemLocal = OpenAPI_signal_storm_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [signal_storm_infos]");
            goto end;
        }
        cJSON_AddItemToArray(signal_storm_infosList, itemLocal);
    }
    }

    if (event_notification->qos_pol_assist_infos) {
    cJSON *qos_pol_assist_infosList = cJSON_AddArrayToObject(item, "qosPolAssistInfos");
    if (qos_pol_assist_infosList == NULL) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed [qos_pol_assist_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_notification->qos_pol_assist_infos, node) {
        cJSON *itemLocal = OpenAPI_qos_policy_assist_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notification_convertToJSON() failed [qos_pol_assist_infos]");
            goto end;
        }
        cJSON_AddItemToArray(qos_pol_assist_infosList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_event_notification_t *OpenAPI_event_notification_parseFromJSON(cJSON *event_notificationJSON)
{
    OpenAPI_event_notification_t *event_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event = NULL;
    OpenAPI_nwdaf_event_e eventVariable = 0;
    cJSON *start = NULL;
    cJSON *expiry = NULL;
    cJSON *time_stamp_gen = NULL;
    cJSON *fail_notify_code = NULL;
    OpenAPI_nwdaf_failure_code_e fail_notify_codeVariable = 0;
    cJSON *rv_wait_time = NULL;
    cJSON *ana_meta_info = NULL;
    OpenAPI_analytics_metadata_info_t *ana_meta_info_local_nonprim = NULL;
    cJSON *nf_load_level_infos = NULL;
    OpenAPI_list_t *nf_load_level_infosList = NULL;
    cJSON *nsi_load_level_infos = NULL;
    OpenAPI_list_t *nsi_load_level_infosList = NULL;
    cJSON *pfd_determ_infos = NULL;
    OpenAPI_list_t *pfd_determ_infosList = NULL;
    cJSON *slice_load_level_info = NULL;
    OpenAPI_slice_load_level_information_t *slice_load_level_info_local_nonprim = NULL;
    cJSON *svc_exps = NULL;
    OpenAPI_list_t *svc_expsList = NULL;
    cJSON *qos_sustain_infos = NULL;
    OpenAPI_list_t *qos_sustain_infosList = NULL;
    cJSON *ue_comms = NULL;
    OpenAPI_list_t *ue_commsList = NULL;
    cJSON *ue_mobs = NULL;
    OpenAPI_list_t *ue_mobsList = NULL;
    cJSON *user_data_cong_infos = NULL;
    OpenAPI_list_t *user_data_cong_infosList = NULL;
    cJSON *abnor_behavrs = NULL;
    OpenAPI_list_t *abnor_behavrsList = NULL;
    cJSON *nw_perfs = NULL;
    OpenAPI_list_t *nw_perfsList = NULL;
    cJSON *dn_perf_infos = NULL;
    OpenAPI_list_t *dn_perf_infosList = NULL;
    cJSON *disper_infos = NULL;
    OpenAPI_list_t *disper_infosList = NULL;
    cJSON *red_trans_infos = NULL;
    OpenAPI_list_t *red_trans_infosList = NULL;
    cJSON *wlan_infos = NULL;
    OpenAPI_list_t *wlan_infosList = NULL;
    cJSON *smcc_exps = NULL;
    OpenAPI_list_t *smcc_expsList = NULL;
    cJSON *pdu_ses_traf_infos = NULL;
    OpenAPI_list_t *pdu_ses_traf_infosList = NULL;
    cJSON *data_vl_trns_tm_infos = NULL;
    OpenAPI_list_t *data_vl_trns_tm_infosList = NULL;
    cJSON *accu_info = NULL;
    OpenAPI_accuracy_info_t *accu_info_local_nonprim = NULL;
    cJSON *cancel_accu_ind = NULL;
    cJSON *pause_ind = NULL;
    cJSON *resume_ind = NULL;
    cJSON *mov_behav_infos = NULL;
    OpenAPI_list_t *mov_behav_infosList = NULL;
    cJSON *loc_acc_infos = NULL;
    OpenAPI_list_t *loc_acc_infosList = NULL;
    cJSON *rel_prox_infos = NULL;
    OpenAPI_list_t *rel_prox_infosList = NULL;
    cJSON *signal_storm_infos = NULL;
    OpenAPI_list_t *signal_storm_infosList = NULL;
    cJSON *qos_pol_assist_infos = NULL;
    OpenAPI_list_t *qos_pol_assist_infosList = NULL;
    event = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [event]");
        goto end;
    }
    if (!cJSON_IsString(event)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [event]");
        goto end;
    }
    eventVariable = OpenAPI_nwdaf_event_FromString(event->valuestring);

    start = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "start");
    if (start) {
    if (!cJSON_IsString(start) && !cJSON_IsNull(start)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [start]");
        goto end;
    }
    }

    expiry = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    time_stamp_gen = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "timeStampGen");
    if (time_stamp_gen) {
    if (!cJSON_IsString(time_stamp_gen) && !cJSON_IsNull(time_stamp_gen)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [time_stamp_gen]");
        goto end;
    }
    }

    fail_notify_code = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "failNotifyCode");
    if (fail_notify_code) {
    if (!cJSON_IsString(fail_notify_code)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [fail_notify_code]");
        goto end;
    }
    fail_notify_codeVariable = OpenAPI_nwdaf_failure_code_FromString(fail_notify_code->valuestring);
    }

    rv_wait_time = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "rvWaitTime");
    if (rv_wait_time) {
    if (!cJSON_IsNumber(rv_wait_time)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [rv_wait_time]");
        goto end;
    }
    }

    ana_meta_info = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "anaMetaInfo");
    if (ana_meta_info) {
    ana_meta_info_local_nonprim = OpenAPI_analytics_metadata_info_parseFromJSON(ana_meta_info);
    if (!ana_meta_info_local_nonprim) {
        ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON failed [ana_meta_info]");
        goto end;
    }
    }

    nf_load_level_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "nfLoadLevelInfos");
    if (nf_load_level_infos) {
        cJSON *nf_load_level_infos_local = NULL;
        if (!cJSON_IsArray(nf_load_level_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [nf_load_level_infos]");
            goto end;
        }

        nf_load_level_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_load_level_infos_local, nf_load_level_infos) {
            if (!cJSON_IsObject(nf_load_level_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [nf_load_level_infos]");
                goto end;
            }
            OpenAPI_nf_load_level_information_t *nf_load_level_infosItem = OpenAPI_nf_load_level_information_parseFromJSON(nf_load_level_infos_local);
            if (!nf_load_level_infosItem) {
                ogs_error("No nf_load_level_infosItem");
                goto end;
            }
            OpenAPI_list_add(nf_load_level_infosList, nf_load_level_infosItem);
        }
    }

    nsi_load_level_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "nsiLoadLevelInfos");
    if (nsi_load_level_infos) {
        cJSON *nsi_load_level_infos_local = NULL;
        if (!cJSON_IsArray(nsi_load_level_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [nsi_load_level_infos]");
            goto end;
        }

        nsi_load_level_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsi_load_level_infos_local, nsi_load_level_infos) {
            if (!cJSON_IsObject(nsi_load_level_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [nsi_load_level_infos]");
                goto end;
            }
            OpenAPI_nsi_load_level_info_t *nsi_load_level_infosItem = OpenAPI_nsi_load_level_info_parseFromJSON(nsi_load_level_infos_local);
            if (!nsi_load_level_infosItem) {
                ogs_error("No nsi_load_level_infosItem");
                goto end;
            }
            OpenAPI_list_add(nsi_load_level_infosList, nsi_load_level_infosItem);
        }
    }

    pfd_determ_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "pfdDetermInfos");
    if (pfd_determ_infos) {
        cJSON *pfd_determ_infos_local = NULL;
        if (!cJSON_IsArray(pfd_determ_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [pfd_determ_infos]");
            goto end;
        }

        pfd_determ_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(pfd_determ_infos_local, pfd_determ_infos) {
            if (!cJSON_IsObject(pfd_determ_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [pfd_determ_infos]");
                goto end;
            }
            OpenAPI_pfd_determination_info_t *pfd_determ_infosItem = OpenAPI_pfd_determination_info_parseFromJSON(pfd_determ_infos_local);
            if (!pfd_determ_infosItem) {
                ogs_error("No pfd_determ_infosItem");
                goto end;
            }
            OpenAPI_list_add(pfd_determ_infosList, pfd_determ_infosItem);
        }
    }

    slice_load_level_info = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "sliceLoadLevelInfo");
    if (slice_load_level_info) {
    slice_load_level_info_local_nonprim = OpenAPI_slice_load_level_information_parseFromJSON(slice_load_level_info);
    if (!slice_load_level_info_local_nonprim) {
        ogs_error("OpenAPI_slice_load_level_information_parseFromJSON failed [slice_load_level_info]");
        goto end;
    }
    }

    svc_exps = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "svcExps");
    if (svc_exps) {
        cJSON *svc_exps_local = NULL;
        if (!cJSON_IsArray(svc_exps)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [svc_exps]");
            goto end;
        }

        svc_expsList = OpenAPI_list_create();

        cJSON_ArrayForEach(svc_exps_local, svc_exps) {
            if (!cJSON_IsObject(svc_exps_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [svc_exps]");
                goto end;
            }
            OpenAPI_service_experience_info_t *svc_expsItem = OpenAPI_service_experience_info_parseFromJSON(svc_exps_local);
            if (!svc_expsItem) {
                ogs_error("No svc_expsItem");
                goto end;
            }
            OpenAPI_list_add(svc_expsList, svc_expsItem);
        }
    }

    qos_sustain_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "qosSustainInfos");
    if (qos_sustain_infos) {
        cJSON *qos_sustain_infos_local = NULL;
        if (!cJSON_IsArray(qos_sustain_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [qos_sustain_infos]");
            goto end;
        }

        qos_sustain_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_sustain_infos_local, qos_sustain_infos) {
            if (!cJSON_IsObject(qos_sustain_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [qos_sustain_infos]");
                goto end;
            }
            OpenAPI_qos_sustainability_info_t *qos_sustain_infosItem = OpenAPI_qos_sustainability_info_parseFromJSON(qos_sustain_infos_local);
            if (!qos_sustain_infosItem) {
                ogs_error("No qos_sustain_infosItem");
                goto end;
            }
            OpenAPI_list_add(qos_sustain_infosList, qos_sustain_infosItem);
        }
    }

    ue_comms = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "ueComms");
    if (ue_comms) {
        cJSON *ue_comms_local = NULL;
        if (!cJSON_IsArray(ue_comms)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [ue_comms]");
            goto end;
        }

        ue_commsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_comms_local, ue_comms) {
            if (!cJSON_IsObject(ue_comms_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [ue_comms]");
                goto end;
            }
            OpenAPI_ue_communication_t *ue_commsItem = OpenAPI_ue_communication_parseFromJSON(ue_comms_local);
            if (!ue_commsItem) {
                ogs_error("No ue_commsItem");
                goto end;
            }
            OpenAPI_list_add(ue_commsList, ue_commsItem);
        }
    }

    ue_mobs = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "ueMobs");
    if (ue_mobs) {
        cJSON *ue_mobs_local = NULL;
        if (!cJSON_IsArray(ue_mobs)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [ue_mobs]");
            goto end;
        }

        ue_mobsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_mobs_local, ue_mobs) {
            if (!cJSON_IsObject(ue_mobs_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [ue_mobs]");
                goto end;
            }
            OpenAPI_ue_mobility_t *ue_mobsItem = OpenAPI_ue_mobility_parseFromJSON(ue_mobs_local);
            if (!ue_mobsItem) {
                ogs_error("No ue_mobsItem");
                goto end;
            }
            OpenAPI_list_add(ue_mobsList, ue_mobsItem);
        }
    }

    user_data_cong_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "userDataCongInfos");
    if (user_data_cong_infos) {
        cJSON *user_data_cong_infos_local = NULL;
        if (!cJSON_IsArray(user_data_cong_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [user_data_cong_infos]");
            goto end;
        }

        user_data_cong_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(user_data_cong_infos_local, user_data_cong_infos) {
            if (!cJSON_IsObject(user_data_cong_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [user_data_cong_infos]");
                goto end;
            }
            OpenAPI_user_data_congestion_info_t *user_data_cong_infosItem = OpenAPI_user_data_congestion_info_parseFromJSON(user_data_cong_infos_local);
            if (!user_data_cong_infosItem) {
                ogs_error("No user_data_cong_infosItem");
                goto end;
            }
            OpenAPI_list_add(user_data_cong_infosList, user_data_cong_infosItem);
        }
    }

    abnor_behavrs = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "abnorBehavrs");
    if (abnor_behavrs) {
        cJSON *abnor_behavrs_local = NULL;
        if (!cJSON_IsArray(abnor_behavrs)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [abnor_behavrs]");
            goto end;
        }

        abnor_behavrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(abnor_behavrs_local, abnor_behavrs) {
            if (!cJSON_IsObject(abnor_behavrs_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [abnor_behavrs]");
                goto end;
            }
            OpenAPI_abnormal_behaviour_t *abnor_behavrsItem = OpenAPI_abnormal_behaviour_parseFromJSON(abnor_behavrs_local);
            if (!abnor_behavrsItem) {
                ogs_error("No abnor_behavrsItem");
                goto end;
            }
            OpenAPI_list_add(abnor_behavrsList, abnor_behavrsItem);
        }
    }

    nw_perfs = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "nwPerfs");
    if (nw_perfs) {
        cJSON *nw_perfs_local = NULL;
        if (!cJSON_IsArray(nw_perfs)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [nw_perfs]");
            goto end;
        }

        nw_perfsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nw_perfs_local, nw_perfs) {
            if (!cJSON_IsObject(nw_perfs_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [nw_perfs]");
                goto end;
            }
            OpenAPI_network_perf_info_t *nw_perfsItem = OpenAPI_network_perf_info_parseFromJSON(nw_perfs_local);
            if (!nw_perfsItem) {
                ogs_error("No nw_perfsItem");
                goto end;
            }
            OpenAPI_list_add(nw_perfsList, nw_perfsItem);
        }
    }

    dn_perf_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "dnPerfInfos");
    if (dn_perf_infos) {
        cJSON *dn_perf_infos_local = NULL;
        if (!cJSON_IsArray(dn_perf_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [dn_perf_infos]");
            goto end;
        }

        dn_perf_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(dn_perf_infos_local, dn_perf_infos) {
            if (!cJSON_IsObject(dn_perf_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [dn_perf_infos]");
                goto end;
            }
            OpenAPI_dn_perf_info_t *dn_perf_infosItem = OpenAPI_dn_perf_info_parseFromJSON(dn_perf_infos_local);
            if (!dn_perf_infosItem) {
                ogs_error("No dn_perf_infosItem");
                goto end;
            }
            OpenAPI_list_add(dn_perf_infosList, dn_perf_infosItem);
        }
    }

    disper_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "disperInfos");
    if (disper_infos) {
        cJSON *disper_infos_local = NULL;
        if (!cJSON_IsArray(disper_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [disper_infos]");
            goto end;
        }

        disper_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(disper_infos_local, disper_infos) {
            if (!cJSON_IsObject(disper_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [disper_infos]");
                goto end;
            }
            OpenAPI_dispersion_info_t *disper_infosItem = OpenAPI_dispersion_info_parseFromJSON(disper_infos_local);
            if (!disper_infosItem) {
                ogs_error("No disper_infosItem");
                goto end;
            }
            OpenAPI_list_add(disper_infosList, disper_infosItem);
        }
    }

    red_trans_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "redTransInfos");
    if (red_trans_infos) {
        cJSON *red_trans_infos_local = NULL;
        if (!cJSON_IsArray(red_trans_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [red_trans_infos]");
            goto end;
        }

        red_trans_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(red_trans_infos_local, red_trans_infos) {
            if (!cJSON_IsObject(red_trans_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [red_trans_infos]");
                goto end;
            }
            OpenAPI_redundant_transmission_exp_info_t *red_trans_infosItem = OpenAPI_redundant_transmission_exp_info_parseFromJSON(red_trans_infos_local);
            if (!red_trans_infosItem) {
                ogs_error("No red_trans_infosItem");
                goto end;
            }
            OpenAPI_list_add(red_trans_infosList, red_trans_infosItem);
        }
    }

    wlan_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "wlanInfos");
    if (wlan_infos) {
        cJSON *wlan_infos_local = NULL;
        if (!cJSON_IsArray(wlan_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [wlan_infos]");
            goto end;
        }

        wlan_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(wlan_infos_local, wlan_infos) {
            if (!cJSON_IsObject(wlan_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [wlan_infos]");
                goto end;
            }
            OpenAPI_wlan_performance_info_t *wlan_infosItem = OpenAPI_wlan_performance_info_parseFromJSON(wlan_infos_local);
            if (!wlan_infosItem) {
                ogs_error("No wlan_infosItem");
                goto end;
            }
            OpenAPI_list_add(wlan_infosList, wlan_infosItem);
        }
    }

    smcc_exps = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "smccExps");
    if (smcc_exps) {
        cJSON *smcc_exps_local = NULL;
        if (!cJSON_IsArray(smcc_exps)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [smcc_exps]");
            goto end;
        }

        smcc_expsList = OpenAPI_list_create();

        cJSON_ArrayForEach(smcc_exps_local, smcc_exps) {
            if (!cJSON_IsObject(smcc_exps_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [smcc_exps]");
                goto end;
            }
            OpenAPI_smcce_info_t *smcc_expsItem = OpenAPI_smcce_info_parseFromJSON(smcc_exps_local);
            if (!smcc_expsItem) {
                ogs_error("No smcc_expsItem");
                goto end;
            }
            OpenAPI_list_add(smcc_expsList, smcc_expsItem);
        }
    }

    pdu_ses_traf_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "pduSesTrafInfos");
    if (pdu_ses_traf_infos) {
        cJSON *pdu_ses_traf_infos_local = NULL;
        if (!cJSON_IsArray(pdu_ses_traf_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [pdu_ses_traf_infos]");
            goto end;
        }

        pdu_ses_traf_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_ses_traf_infos_local, pdu_ses_traf_infos) {
            if (!cJSON_IsObject(pdu_ses_traf_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [pdu_ses_traf_infos]");
                goto end;
            }
            OpenAPI_pdu_ses_traffic_info_t *pdu_ses_traf_infosItem = OpenAPI_pdu_ses_traffic_info_parseFromJSON(pdu_ses_traf_infos_local);
            if (!pdu_ses_traf_infosItem) {
                ogs_error("No pdu_ses_traf_infosItem");
                goto end;
            }
            OpenAPI_list_add(pdu_ses_traf_infosList, pdu_ses_traf_infosItem);
        }
    }

    data_vl_trns_tm_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "dataVlTrnsTmInfos");
    if (data_vl_trns_tm_infos) {
        cJSON *data_vl_trns_tm_infos_local = NULL;
        if (!cJSON_IsArray(data_vl_trns_tm_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [data_vl_trns_tm_infos]");
            goto end;
        }

        data_vl_trns_tm_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(data_vl_trns_tm_infos_local, data_vl_trns_tm_infos) {
            if (!cJSON_IsObject(data_vl_trns_tm_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [data_vl_trns_tm_infos]");
                goto end;
            }
            OpenAPI_e2e_data_vol_trans_time_info_t *data_vl_trns_tm_infosItem = OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON(data_vl_trns_tm_infos_local);
            if (!data_vl_trns_tm_infosItem) {
                ogs_error("No data_vl_trns_tm_infosItem");
                goto end;
            }
            OpenAPI_list_add(data_vl_trns_tm_infosList, data_vl_trns_tm_infosItem);
        }
    }

    accu_info = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "accuInfo");
    if (accu_info) {
    accu_info_local_nonprim = OpenAPI_accuracy_info_parseFromJSON(accu_info);
    if (!accu_info_local_nonprim) {
        ogs_error("OpenAPI_accuracy_info_parseFromJSON failed [accu_info]");
        goto end;
    }
    }

    cancel_accu_ind = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "cancelAccuInd");
    if (cancel_accu_ind) {
    if (!cJSON_IsBool(cancel_accu_ind)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [cancel_accu_ind]");
        goto end;
    }
    }

    pause_ind = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "pauseInd");
    if (pause_ind) {
    if (!cJSON_IsBool(pause_ind)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [pause_ind]");
        goto end;
    }
    }

    resume_ind = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "resumeInd");
    if (resume_ind) {
    if (!cJSON_IsBool(resume_ind)) {
        ogs_error("OpenAPI_event_notification_parseFromJSON() failed [resume_ind]");
        goto end;
    }
    }

    mov_behav_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "movBehavInfos");
    if (mov_behav_infos) {
        cJSON *mov_behav_infos_local = NULL;
        if (!cJSON_IsArray(mov_behav_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [mov_behav_infos]");
            goto end;
        }

        mov_behav_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(mov_behav_infos_local, mov_behav_infos) {
            if (!cJSON_IsObject(mov_behav_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [mov_behav_infos]");
                goto end;
            }
            OpenAPI_mov_behav_info_t *mov_behav_infosItem = OpenAPI_mov_behav_info_parseFromJSON(mov_behav_infos_local);
            if (!mov_behav_infosItem) {
                ogs_error("No mov_behav_infosItem");
                goto end;
            }
            OpenAPI_list_add(mov_behav_infosList, mov_behav_infosItem);
        }
    }

    loc_acc_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "locAccInfos");
    if (loc_acc_infos) {
        cJSON *loc_acc_infos_local = NULL;
        if (!cJSON_IsArray(loc_acc_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [loc_acc_infos]");
            goto end;
        }

        loc_acc_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(loc_acc_infos_local, loc_acc_infos) {
            if (!cJSON_IsObject(loc_acc_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [loc_acc_infos]");
                goto end;
            }
            OpenAPI_loc_accuracy_info_t *loc_acc_infosItem = OpenAPI_loc_accuracy_info_parseFromJSON(loc_acc_infos_local);
            if (!loc_acc_infosItem) {
                ogs_error("No loc_acc_infosItem");
                goto end;
            }
            OpenAPI_list_add(loc_acc_infosList, loc_acc_infosItem);
        }
    }

    rel_prox_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "relProxInfos");
    if (rel_prox_infos) {
        cJSON *rel_prox_infos_local = NULL;
        if (!cJSON_IsArray(rel_prox_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [rel_prox_infos]");
            goto end;
        }

        rel_prox_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(rel_prox_infos_local, rel_prox_infos) {
            if (!cJSON_IsObject(rel_prox_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [rel_prox_infos]");
                goto end;
            }
            OpenAPI_rel_prox_info_t *rel_prox_infosItem = OpenAPI_rel_prox_info_parseFromJSON(rel_prox_infos_local);
            if (!rel_prox_infosItem) {
                ogs_error("No rel_prox_infosItem");
                goto end;
            }
            OpenAPI_list_add(rel_prox_infosList, rel_prox_infosItem);
        }
    }

    signal_storm_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "signalStormInfos");
    if (signal_storm_infos) {
        cJSON *signal_storm_infos_local = NULL;
        if (!cJSON_IsArray(signal_storm_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [signal_storm_infos]");
            goto end;
        }

        signal_storm_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(signal_storm_infos_local, signal_storm_infos) {
            if (!cJSON_IsObject(signal_storm_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [signal_storm_infos]");
                goto end;
            }
            OpenAPI_signal_storm_info_t *signal_storm_infosItem = OpenAPI_signal_storm_info_parseFromJSON(signal_storm_infos_local);
            if (!signal_storm_infosItem) {
                ogs_error("No signal_storm_infosItem");
                goto end;
            }
            OpenAPI_list_add(signal_storm_infosList, signal_storm_infosItem);
        }
    }

    qos_pol_assist_infos = cJSON_GetObjectItemCaseSensitive(event_notificationJSON, "qosPolAssistInfos");
    if (qos_pol_assist_infos) {
        cJSON *qos_pol_assist_infos_local = NULL;
        if (!cJSON_IsArray(qos_pol_assist_infos)) {
            ogs_error("OpenAPI_event_notification_parseFromJSON() failed [qos_pol_assist_infos]");
            goto end;
        }

        qos_pol_assist_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_pol_assist_infos_local, qos_pol_assist_infos) {
            if (!cJSON_IsObject(qos_pol_assist_infos_local)) {
                ogs_error("OpenAPI_event_notification_parseFromJSON() failed [qos_pol_assist_infos]");
                goto end;
            }
            OpenAPI_qos_policy_assist_info_t *qos_pol_assist_infosItem = OpenAPI_qos_policy_assist_info_parseFromJSON(qos_pol_assist_infos_local);
            if (!qos_pol_assist_infosItem) {
                ogs_error("No qos_pol_assist_infosItem");
                goto end;
            }
            OpenAPI_list_add(qos_pol_assist_infosList, qos_pol_assist_infosItem);
        }
    }

    event_notification_local_var = OpenAPI_event_notification_create (
        eventVariable,
        start && !cJSON_IsNull(start) ? ogs_strdup(start->valuestring) : NULL,
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        time_stamp_gen && !cJSON_IsNull(time_stamp_gen) ? ogs_strdup(time_stamp_gen->valuestring) : NULL,
        fail_notify_code ? fail_notify_codeVariable : 0,
        rv_wait_time ? true : false,
        rv_wait_time ? rv_wait_time->valuedouble : 0,
        ana_meta_info ? ana_meta_info_local_nonprim : NULL,
        nf_load_level_infos ? nf_load_level_infosList : NULL,
        nsi_load_level_infos ? nsi_load_level_infosList : NULL,
        pfd_determ_infos ? pfd_determ_infosList : NULL,
        slice_load_level_info ? slice_load_level_info_local_nonprim : NULL,
        svc_exps ? svc_expsList : NULL,
        qos_sustain_infos ? qos_sustain_infosList : NULL,
        ue_comms ? ue_commsList : NULL,
        ue_mobs ? ue_mobsList : NULL,
        user_data_cong_infos ? user_data_cong_infosList : NULL,
        abnor_behavrs ? abnor_behavrsList : NULL,
        nw_perfs ? nw_perfsList : NULL,
        dn_perf_infos ? dn_perf_infosList : NULL,
        disper_infos ? disper_infosList : NULL,
        red_trans_infos ? red_trans_infosList : NULL,
        wlan_infos ? wlan_infosList : NULL,
        smcc_exps ? smcc_expsList : NULL,
        pdu_ses_traf_infos ? pdu_ses_traf_infosList : NULL,
        data_vl_trns_tm_infos ? data_vl_trns_tm_infosList : NULL,
        accu_info ? accu_info_local_nonprim : NULL,
        cancel_accu_ind ? true : false,
        cancel_accu_ind ? cancel_accu_ind->valueint : 0,
        pause_ind ? true : false,
        pause_ind ? pause_ind->valueint : 0,
        resume_ind ? true : false,
        resume_ind ? resume_ind->valueint : 0,
        mov_behav_infos ? mov_behav_infosList : NULL,
        loc_acc_infos ? loc_acc_infosList : NULL,
        rel_prox_infos ? rel_prox_infosList : NULL,
        signal_storm_infos ? signal_storm_infosList : NULL,
        qos_pol_assist_infos ? qos_pol_assist_infosList : NULL
    );

    return event_notification_local_var;
end:
    if (ana_meta_info_local_nonprim) {
        OpenAPI_analytics_metadata_info_free(ana_meta_info_local_nonprim);
        ana_meta_info_local_nonprim = NULL;
    }
    if (nf_load_level_infosList) {
        OpenAPI_list_for_each(nf_load_level_infosList, node) {
            OpenAPI_nf_load_level_information_free(node->data);
        }
        OpenAPI_list_free(nf_load_level_infosList);
        nf_load_level_infosList = NULL;
    }
    if (nsi_load_level_infosList) {
        OpenAPI_list_for_each(nsi_load_level_infosList, node) {
            OpenAPI_nsi_load_level_info_free(node->data);
        }
        OpenAPI_list_free(nsi_load_level_infosList);
        nsi_load_level_infosList = NULL;
    }
    if (pfd_determ_infosList) {
        OpenAPI_list_for_each(pfd_determ_infosList, node) {
            OpenAPI_pfd_determination_info_free(node->data);
        }
        OpenAPI_list_free(pfd_determ_infosList);
        pfd_determ_infosList = NULL;
    }
    if (slice_load_level_info_local_nonprim) {
        OpenAPI_slice_load_level_information_free(slice_load_level_info_local_nonprim);
        slice_load_level_info_local_nonprim = NULL;
    }
    if (svc_expsList) {
        OpenAPI_list_for_each(svc_expsList, node) {
            OpenAPI_service_experience_info_free(node->data);
        }
        OpenAPI_list_free(svc_expsList);
        svc_expsList = NULL;
    }
    if (qos_sustain_infosList) {
        OpenAPI_list_for_each(qos_sustain_infosList, node) {
            OpenAPI_qos_sustainability_info_free(node->data);
        }
        OpenAPI_list_free(qos_sustain_infosList);
        qos_sustain_infosList = NULL;
    }
    if (ue_commsList) {
        OpenAPI_list_for_each(ue_commsList, node) {
            OpenAPI_ue_communication_free(node->data);
        }
        OpenAPI_list_free(ue_commsList);
        ue_commsList = NULL;
    }
    if (ue_mobsList) {
        OpenAPI_list_for_each(ue_mobsList, node) {
            OpenAPI_ue_mobility_free(node->data);
        }
        OpenAPI_list_free(ue_mobsList);
        ue_mobsList = NULL;
    }
    if (user_data_cong_infosList) {
        OpenAPI_list_for_each(user_data_cong_infosList, node) {
            OpenAPI_user_data_congestion_info_free(node->data);
        }
        OpenAPI_list_free(user_data_cong_infosList);
        user_data_cong_infosList = NULL;
    }
    if (abnor_behavrsList) {
        OpenAPI_list_for_each(abnor_behavrsList, node) {
            OpenAPI_abnormal_behaviour_free(node->data);
        }
        OpenAPI_list_free(abnor_behavrsList);
        abnor_behavrsList = NULL;
    }
    if (nw_perfsList) {
        OpenAPI_list_for_each(nw_perfsList, node) {
            OpenAPI_network_perf_info_free(node->data);
        }
        OpenAPI_list_free(nw_perfsList);
        nw_perfsList = NULL;
    }
    if (dn_perf_infosList) {
        OpenAPI_list_for_each(dn_perf_infosList, node) {
            OpenAPI_dn_perf_info_free(node->data);
        }
        OpenAPI_list_free(dn_perf_infosList);
        dn_perf_infosList = NULL;
    }
    if (disper_infosList) {
        OpenAPI_list_for_each(disper_infosList, node) {
            OpenAPI_dispersion_info_free(node->data);
        }
        OpenAPI_list_free(disper_infosList);
        disper_infosList = NULL;
    }
    if (red_trans_infosList) {
        OpenAPI_list_for_each(red_trans_infosList, node) {
            OpenAPI_redundant_transmission_exp_info_free(node->data);
        }
        OpenAPI_list_free(red_trans_infosList);
        red_trans_infosList = NULL;
    }
    if (wlan_infosList) {
        OpenAPI_list_for_each(wlan_infosList, node) {
            OpenAPI_wlan_performance_info_free(node->data);
        }
        OpenAPI_list_free(wlan_infosList);
        wlan_infosList = NULL;
    }
    if (smcc_expsList) {
        OpenAPI_list_for_each(smcc_expsList, node) {
            OpenAPI_smcce_info_free(node->data);
        }
        OpenAPI_list_free(smcc_expsList);
        smcc_expsList = NULL;
    }
    if (pdu_ses_traf_infosList) {
        OpenAPI_list_for_each(pdu_ses_traf_infosList, node) {
            OpenAPI_pdu_ses_traffic_info_free(node->data);
        }
        OpenAPI_list_free(pdu_ses_traf_infosList);
        pdu_ses_traf_infosList = NULL;
    }
    if (data_vl_trns_tm_infosList) {
        OpenAPI_list_for_each(data_vl_trns_tm_infosList, node) {
            OpenAPI_e2e_data_vol_trans_time_info_free(node->data);
        }
        OpenAPI_list_free(data_vl_trns_tm_infosList);
        data_vl_trns_tm_infosList = NULL;
    }
    if (accu_info_local_nonprim) {
        OpenAPI_accuracy_info_free(accu_info_local_nonprim);
        accu_info_local_nonprim = NULL;
    }
    if (mov_behav_infosList) {
        OpenAPI_list_for_each(mov_behav_infosList, node) {
            OpenAPI_mov_behav_info_free(node->data);
        }
        OpenAPI_list_free(mov_behav_infosList);
        mov_behav_infosList = NULL;
    }
    if (loc_acc_infosList) {
        OpenAPI_list_for_each(loc_acc_infosList, node) {
            OpenAPI_loc_accuracy_info_free(node->data);
        }
        OpenAPI_list_free(loc_acc_infosList);
        loc_acc_infosList = NULL;
    }
    if (rel_prox_infosList) {
        OpenAPI_list_for_each(rel_prox_infosList, node) {
            OpenAPI_rel_prox_info_free(node->data);
        }
        OpenAPI_list_free(rel_prox_infosList);
        rel_prox_infosList = NULL;
    }
    if (signal_storm_infosList) {
        OpenAPI_list_for_each(signal_storm_infosList, node) {
            OpenAPI_signal_storm_info_free(node->data);
        }
        OpenAPI_list_free(signal_storm_infosList);
        signal_storm_infosList = NULL;
    }
    if (qos_pol_assist_infosList) {
        OpenAPI_list_for_each(qos_pol_assist_infosList, node) {
            OpenAPI_qos_policy_assist_info_free(node->data);
        }
        OpenAPI_list_free(qos_pol_assist_infosList);
        qos_pol_assist_infosList = NULL;
    }
    return NULL;
}

OpenAPI_event_notification_t *OpenAPI_event_notification_copy(OpenAPI_event_notification_t *dst, OpenAPI_event_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_notification_convertToJSON() failed");
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

    OpenAPI_event_notification_free(dst);
    dst = OpenAPI_event_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

