
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_component.h"

OpenAPI_media_component_t *OpenAPI_media_component_create(
    char *af_app_id,
    OpenAPI_af_routing_requirement_t *af_rout_req,
    char *qos_reference,
    bool is_dis_ue_notif,
    int dis_ue_notif,
    OpenAPI_list_t *alt_ser_reqs,
    OpenAPI_list_t *alt_ser_reqs_data,
    bool is_cont_ver,
    int cont_ver,
    OpenAPI_list_t *codecs,
    bool is_des_max_latency,
    float des_max_latency,
    bool is_des_max_loss,
    float des_max_loss,
    char *flus_id,
    OpenAPI_flow_status_e f_status,
    char *mar_bw_dl,
    char *mar_bw_ul,
    bool is_max_packet_loss_rate_dl_null,
    bool is_max_packet_loss_rate_dl,
    int max_packet_loss_rate_dl,
    bool is_max_packet_loss_rate_ul_null,
    bool is_max_packet_loss_rate_ul,
    int max_packet_loss_rate_ul,
    char *max_supp_bw_dl,
    char *max_supp_bw_ul,
    int med_comp_n,
    OpenAPI_list_t* med_sub_comps,
    OpenAPI_media_type_e med_type,
    char *min_des_bw_dl,
    char *min_des_bw_ul,
    char *mir_bw_dl,
    char *mir_bw_ul,
    OpenAPI_preemption_capability_e preempt_cap,
    OpenAPI_preemption_vulnerability_e preempt_vuln,
    OpenAPI_priority_sharing_indicator_e prio_sharing_ind,
    OpenAPI_reserv_priority_e res_prio,
    char *rr_bw,
    char *rs_bw,
    bool is_sharing_key_dl,
    int sharing_key_dl,
    bool is_sharing_key_ul,
    int sharing_key_ul,
    OpenAPI_tsn_qos_container_t *tsn_qos,
    bool is_tscai_input_dl_null,
    OpenAPI_tscai_input_container_t *tscai_input_dl,
    bool is_tscai_input_ul_null,
    OpenAPI_tscai_input_container_t *tscai_input_ul,
    bool is_tscai_time_dom,
    int tscai_time_dom
)
{
    OpenAPI_media_component_t *media_component_local_var = ogs_malloc(sizeof(OpenAPI_media_component_t));
    ogs_assert(media_component_local_var);

    media_component_local_var->af_app_id = af_app_id;
    media_component_local_var->af_rout_req = af_rout_req;
    media_component_local_var->qos_reference = qos_reference;
    media_component_local_var->is_dis_ue_notif = is_dis_ue_notif;
    media_component_local_var->dis_ue_notif = dis_ue_notif;
    media_component_local_var->alt_ser_reqs = alt_ser_reqs;
    media_component_local_var->alt_ser_reqs_data = alt_ser_reqs_data;
    media_component_local_var->is_cont_ver = is_cont_ver;
    media_component_local_var->cont_ver = cont_ver;
    media_component_local_var->codecs = codecs;
    media_component_local_var->is_des_max_latency = is_des_max_latency;
    media_component_local_var->des_max_latency = des_max_latency;
    media_component_local_var->is_des_max_loss = is_des_max_loss;
    media_component_local_var->des_max_loss = des_max_loss;
    media_component_local_var->flus_id = flus_id;
    media_component_local_var->f_status = f_status;
    media_component_local_var->mar_bw_dl = mar_bw_dl;
    media_component_local_var->mar_bw_ul = mar_bw_ul;
    media_component_local_var->is_max_packet_loss_rate_dl_null = is_max_packet_loss_rate_dl_null;
    media_component_local_var->is_max_packet_loss_rate_dl = is_max_packet_loss_rate_dl;
    media_component_local_var->max_packet_loss_rate_dl = max_packet_loss_rate_dl;
    media_component_local_var->is_max_packet_loss_rate_ul_null = is_max_packet_loss_rate_ul_null;
    media_component_local_var->is_max_packet_loss_rate_ul = is_max_packet_loss_rate_ul;
    media_component_local_var->max_packet_loss_rate_ul = max_packet_loss_rate_ul;
    media_component_local_var->max_supp_bw_dl = max_supp_bw_dl;
    media_component_local_var->max_supp_bw_ul = max_supp_bw_ul;
    media_component_local_var->med_comp_n = med_comp_n;
    media_component_local_var->med_sub_comps = med_sub_comps;
    media_component_local_var->med_type = med_type;
    media_component_local_var->min_des_bw_dl = min_des_bw_dl;
    media_component_local_var->min_des_bw_ul = min_des_bw_ul;
    media_component_local_var->mir_bw_dl = mir_bw_dl;
    media_component_local_var->mir_bw_ul = mir_bw_ul;
    media_component_local_var->preempt_cap = preempt_cap;
    media_component_local_var->preempt_vuln = preempt_vuln;
    media_component_local_var->prio_sharing_ind = prio_sharing_ind;
    media_component_local_var->res_prio = res_prio;
    media_component_local_var->rr_bw = rr_bw;
    media_component_local_var->rs_bw = rs_bw;
    media_component_local_var->is_sharing_key_dl = is_sharing_key_dl;
    media_component_local_var->sharing_key_dl = sharing_key_dl;
    media_component_local_var->is_sharing_key_ul = is_sharing_key_ul;
    media_component_local_var->sharing_key_ul = sharing_key_ul;
    media_component_local_var->tsn_qos = tsn_qos;
    media_component_local_var->is_tscai_input_dl_null = is_tscai_input_dl_null;
    media_component_local_var->tscai_input_dl = tscai_input_dl;
    media_component_local_var->is_tscai_input_ul_null = is_tscai_input_ul_null;
    media_component_local_var->tscai_input_ul = tscai_input_ul;
    media_component_local_var->is_tscai_time_dom = is_tscai_time_dom;
    media_component_local_var->tscai_time_dom = tscai_time_dom;

    return media_component_local_var;
}

void OpenAPI_media_component_free(OpenAPI_media_component_t *media_component)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == media_component) {
        return;
    }
    if (media_component->af_app_id) {
        ogs_free(media_component->af_app_id);
        media_component->af_app_id = NULL;
    }
    if (media_component->af_rout_req) {
        OpenAPI_af_routing_requirement_free(media_component->af_rout_req);
        media_component->af_rout_req = NULL;
    }
    if (media_component->qos_reference) {
        ogs_free(media_component->qos_reference);
        media_component->qos_reference = NULL;
    }
    if (media_component->alt_ser_reqs) {
        OpenAPI_list_for_each(media_component->alt_ser_reqs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(media_component->alt_ser_reqs);
        media_component->alt_ser_reqs = NULL;
    }
    if (media_component->alt_ser_reqs_data) {
        OpenAPI_list_for_each(media_component->alt_ser_reqs_data, node) {
            OpenAPI_alternative_service_requirements_data_free(node->data);
        }
        OpenAPI_list_free(media_component->alt_ser_reqs_data);
        media_component->alt_ser_reqs_data = NULL;
    }
    if (media_component->codecs) {
        OpenAPI_list_for_each(media_component->codecs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(media_component->codecs);
        media_component->codecs = NULL;
    }
    if (media_component->flus_id) {
        ogs_free(media_component->flus_id);
        media_component->flus_id = NULL;
    }
    if (media_component->mar_bw_dl) {
        ogs_free(media_component->mar_bw_dl);
        media_component->mar_bw_dl = NULL;
    }
    if (media_component->mar_bw_ul) {
        ogs_free(media_component->mar_bw_ul);
        media_component->mar_bw_ul = NULL;
    }
    if (media_component->max_supp_bw_dl) {
        ogs_free(media_component->max_supp_bw_dl);
        media_component->max_supp_bw_dl = NULL;
    }
    if (media_component->max_supp_bw_ul) {
        ogs_free(media_component->max_supp_bw_ul);
        media_component->max_supp_bw_ul = NULL;
    }
    if (media_component->med_sub_comps) {
        OpenAPI_list_for_each(media_component->med_sub_comps, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_media_sub_component_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(media_component->med_sub_comps);
        media_component->med_sub_comps = NULL;
    }
    if (media_component->min_des_bw_dl) {
        ogs_free(media_component->min_des_bw_dl);
        media_component->min_des_bw_dl = NULL;
    }
    if (media_component->min_des_bw_ul) {
        ogs_free(media_component->min_des_bw_ul);
        media_component->min_des_bw_ul = NULL;
    }
    if (media_component->mir_bw_dl) {
        ogs_free(media_component->mir_bw_dl);
        media_component->mir_bw_dl = NULL;
    }
    if (media_component->mir_bw_ul) {
        ogs_free(media_component->mir_bw_ul);
        media_component->mir_bw_ul = NULL;
    }
    if (media_component->rr_bw) {
        ogs_free(media_component->rr_bw);
        media_component->rr_bw = NULL;
    }
    if (media_component->rs_bw) {
        ogs_free(media_component->rs_bw);
        media_component->rs_bw = NULL;
    }
    if (media_component->tsn_qos) {
        OpenAPI_tsn_qos_container_free(media_component->tsn_qos);
        media_component->tsn_qos = NULL;
    }
    if (media_component->tscai_input_dl) {
        OpenAPI_tscai_input_container_free(media_component->tscai_input_dl);
        media_component->tscai_input_dl = NULL;
    }
    if (media_component->tscai_input_ul) {
        OpenAPI_tscai_input_container_free(media_component->tscai_input_ul);
        media_component->tscai_input_ul = NULL;
    }
    ogs_free(media_component);
}

cJSON *OpenAPI_media_component_convertToJSON(OpenAPI_media_component_t *media_component)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (media_component == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [MediaComponent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (media_component->af_app_id) {
    if (cJSON_AddStringToObject(item, "afAppId", media_component->af_app_id) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [af_app_id]");
        goto end;
    }
    }

    if (media_component->af_rout_req) {
    cJSON *af_rout_req_local_JSON = OpenAPI_af_routing_requirement_convertToJSON(media_component->af_rout_req);
    if (af_rout_req_local_JSON == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [af_rout_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afRoutReq", af_rout_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [af_rout_req]");
        goto end;
    }
    }

    if (media_component->qos_reference) {
    if (cJSON_AddStringToObject(item, "qosReference", media_component->qos_reference) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [qos_reference]");
        goto end;
    }
    }

    if (media_component->is_dis_ue_notif) {
    if (cJSON_AddBoolToObject(item, "disUeNotif", media_component->dis_ue_notif) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [dis_ue_notif]");
        goto end;
    }
    }

    if (media_component->alt_ser_reqs) {
    cJSON *alt_ser_reqsList = cJSON_AddArrayToObject(item, "altSerReqs");
    if (alt_ser_reqsList == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [alt_ser_reqs]");
        goto end;
    }
    OpenAPI_list_for_each(media_component->alt_ser_reqs, node) {
        if (cJSON_AddStringToObject(alt_ser_reqsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_media_component_convertToJSON() failed [alt_ser_reqs]");
            goto end;
        }
    }
    }

    if (media_component->alt_ser_reqs_data) {
    cJSON *alt_ser_reqs_dataList = cJSON_AddArrayToObject(item, "altSerReqsData");
    if (alt_ser_reqs_dataList == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [alt_ser_reqs_data]");
        goto end;
    }
    OpenAPI_list_for_each(media_component->alt_ser_reqs_data, node) {
        cJSON *itemLocal = OpenAPI_alternative_service_requirements_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_media_component_convertToJSON() failed [alt_ser_reqs_data]");
            goto end;
        }
        cJSON_AddItemToArray(alt_ser_reqs_dataList, itemLocal);
    }
    }

    if (media_component->is_cont_ver) {
    if (cJSON_AddNumberToObject(item, "contVer", media_component->cont_ver) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [cont_ver]");
        goto end;
    }
    }

    if (media_component->codecs) {
    cJSON *codecsList = cJSON_AddArrayToObject(item, "codecs");
    if (codecsList == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [codecs]");
        goto end;
    }
    OpenAPI_list_for_each(media_component->codecs, node) {
        if (cJSON_AddStringToObject(codecsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_media_component_convertToJSON() failed [codecs]");
            goto end;
        }
    }
    }

    if (media_component->is_des_max_latency) {
    if (cJSON_AddNumberToObject(item, "desMaxLatency", media_component->des_max_latency) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [des_max_latency]");
        goto end;
    }
    }

    if (media_component->is_des_max_loss) {
    if (cJSON_AddNumberToObject(item, "desMaxLoss", media_component->des_max_loss) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [des_max_loss]");
        goto end;
    }
    }

    if (media_component->flus_id) {
    if (cJSON_AddStringToObject(item, "flusId", media_component->flus_id) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [flus_id]");
        goto end;
    }
    }

    if (media_component->f_status != OpenAPI_flow_status_NULL) {
    if (cJSON_AddStringToObject(item, "fStatus", OpenAPI_flow_status_ToString(media_component->f_status)) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [f_status]");
        goto end;
    }
    }

    if (media_component->mar_bw_dl) {
    if (cJSON_AddStringToObject(item, "marBwDl", media_component->mar_bw_dl) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    if (media_component->mar_bw_ul) {
    if (cJSON_AddStringToObject(item, "marBwUl", media_component->mar_bw_ul) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    if (media_component->is_max_packet_loss_rate_dl) {
    if (cJSON_AddNumberToObject(item, "maxPacketLossRateDl", media_component->max_packet_loss_rate_dl) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [max_packet_loss_rate_dl]");
        goto end;
    }
    } else if (media_component->is_max_packet_loss_rate_dl_null) {
        if (cJSON_AddNullToObject(item, "maxPacketLossRateDl") == NULL) {
            ogs_error("OpenAPI_media_component_convertToJSON() failed [max_packet_loss_rate_dl]");
            goto end;
        }
    }

    if (media_component->is_max_packet_loss_rate_ul) {
    if (cJSON_AddNumberToObject(item, "maxPacketLossRateUl", media_component->max_packet_loss_rate_ul) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [max_packet_loss_rate_ul]");
        goto end;
    }
    } else if (media_component->is_max_packet_loss_rate_ul_null) {
        if (cJSON_AddNullToObject(item, "maxPacketLossRateUl") == NULL) {
            ogs_error("OpenAPI_media_component_convertToJSON() failed [max_packet_loss_rate_ul]");
            goto end;
        }
    }

    if (media_component->max_supp_bw_dl) {
    if (cJSON_AddStringToObject(item, "maxSuppBwDl", media_component->max_supp_bw_dl) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [max_supp_bw_dl]");
        goto end;
    }
    }

    if (media_component->max_supp_bw_ul) {
    if (cJSON_AddStringToObject(item, "maxSuppBwUl", media_component->max_supp_bw_ul) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [max_supp_bw_ul]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "medCompN", media_component->med_comp_n) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [med_comp_n]");
        goto end;
    }

    if (media_component->med_sub_comps) {
    cJSON *med_sub_comps = cJSON_AddObjectToObject(item, "medSubComps");
    if (med_sub_comps == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [med_sub_comps]");
        goto end;
    }
    cJSON *localMapObject = med_sub_comps;
    if (media_component->med_sub_comps) {
        OpenAPI_list_for_each(media_component->med_sub_comps, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_media_component_convertToJSON() failed [med_sub_comps]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_media_component_convertToJSON() failed [med_sub_comps]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_media_sub_component_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_media_component_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (media_component->med_type != OpenAPI_media_type_NULL) {
    if (cJSON_AddStringToObject(item, "medType", OpenAPI_media_type_ToString(media_component->med_type)) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [med_type]");
        goto end;
    }
    }

    if (media_component->min_des_bw_dl) {
    if (cJSON_AddStringToObject(item, "minDesBwDl", media_component->min_des_bw_dl) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [min_des_bw_dl]");
        goto end;
    }
    }

    if (media_component->min_des_bw_ul) {
    if (cJSON_AddStringToObject(item, "minDesBwUl", media_component->min_des_bw_ul) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [min_des_bw_ul]");
        goto end;
    }
    }

    if (media_component->mir_bw_dl) {
    if (cJSON_AddStringToObject(item, "mirBwDl", media_component->mir_bw_dl) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [mir_bw_dl]");
        goto end;
    }
    }

    if (media_component->mir_bw_ul) {
    if (cJSON_AddStringToObject(item, "mirBwUl", media_component->mir_bw_ul) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [mir_bw_ul]");
        goto end;
    }
    }

    if (media_component->preempt_cap != OpenAPI_preemption_capability_NULL) {
    if (cJSON_AddStringToObject(item, "preemptCap", OpenAPI_preemption_capability_ToString(media_component->preempt_cap)) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [preempt_cap]");
        goto end;
    }
    }

    if (media_component->preempt_vuln != OpenAPI_preemption_vulnerability_NULL) {
    if (cJSON_AddStringToObject(item, "preemptVuln", OpenAPI_preemption_vulnerability_ToString(media_component->preempt_vuln)) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [preempt_vuln]");
        goto end;
    }
    }

    if (media_component->prio_sharing_ind != OpenAPI_priority_sharing_indicator_NULL) {
    if (cJSON_AddStringToObject(item, "prioSharingInd", OpenAPI_priority_sharing_indicator_ToString(media_component->prio_sharing_ind)) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [prio_sharing_ind]");
        goto end;
    }
    }

    if (media_component->res_prio != OpenAPI_reserv_priority_NULL) {
    if (cJSON_AddStringToObject(item, "resPrio", OpenAPI_reserv_priority_ToString(media_component->res_prio)) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [res_prio]");
        goto end;
    }
    }

    if (media_component->rr_bw) {
    if (cJSON_AddStringToObject(item, "rrBw", media_component->rr_bw) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [rr_bw]");
        goto end;
    }
    }

    if (media_component->rs_bw) {
    if (cJSON_AddStringToObject(item, "rsBw", media_component->rs_bw) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [rs_bw]");
        goto end;
    }
    }

    if (media_component->is_sharing_key_dl) {
    if (cJSON_AddNumberToObject(item, "sharingKeyDl", media_component->sharing_key_dl) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [sharing_key_dl]");
        goto end;
    }
    }

    if (media_component->is_sharing_key_ul) {
    if (cJSON_AddNumberToObject(item, "sharingKeyUl", media_component->sharing_key_ul) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [sharing_key_ul]");
        goto end;
    }
    }

    if (media_component->tsn_qos) {
    cJSON *tsn_qos_local_JSON = OpenAPI_tsn_qos_container_convertToJSON(media_component->tsn_qos);
    if (tsn_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [tsn_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnQos", tsn_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [tsn_qos]");
        goto end;
    }
    }

    if (media_component->tscai_input_dl) {
    cJSON *tscai_input_dl_local_JSON = OpenAPI_tscai_input_container_convertToJSON(media_component->tscai_input_dl);
    if (tscai_input_dl_local_JSON == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [tscai_input_dl]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tscaiInputDl", tscai_input_dl_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [tscai_input_dl]");
        goto end;
    }
    } else if (media_component->is_tscai_input_dl_null) {
        if (cJSON_AddNullToObject(item, "tscaiInputDl") == NULL) {
            ogs_error("OpenAPI_media_component_convertToJSON() failed [tscai_input_dl]");
            goto end;
        }
    }

    if (media_component->tscai_input_ul) {
    cJSON *tscai_input_ul_local_JSON = OpenAPI_tscai_input_container_convertToJSON(media_component->tscai_input_ul);
    if (tscai_input_ul_local_JSON == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [tscai_input_ul]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tscaiInputUl", tscai_input_ul_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [tscai_input_ul]");
        goto end;
    }
    } else if (media_component->is_tscai_input_ul_null) {
        if (cJSON_AddNullToObject(item, "tscaiInputUl") == NULL) {
            ogs_error("OpenAPI_media_component_convertToJSON() failed [tscai_input_ul]");
            goto end;
        }
    }

    if (media_component->is_tscai_time_dom) {
    if (cJSON_AddNumberToObject(item, "tscaiTimeDom", media_component->tscai_time_dom) == NULL) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed [tscai_time_dom]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_media_component_t *OpenAPI_media_component_parseFromJSON(cJSON *media_componentJSON)
{
    OpenAPI_media_component_t *media_component_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_app_id = NULL;
    cJSON *af_rout_req = NULL;
    OpenAPI_af_routing_requirement_t *af_rout_req_local_nonprim = NULL;
    cJSON *qos_reference = NULL;
    cJSON *dis_ue_notif = NULL;
    cJSON *alt_ser_reqs = NULL;
    OpenAPI_list_t *alt_ser_reqsList = NULL;
    cJSON *alt_ser_reqs_data = NULL;
    OpenAPI_list_t *alt_ser_reqs_dataList = NULL;
    cJSON *cont_ver = NULL;
    cJSON *codecs = NULL;
    OpenAPI_list_t *codecsList = NULL;
    cJSON *des_max_latency = NULL;
    cJSON *des_max_loss = NULL;
    cJSON *flus_id = NULL;
    cJSON *f_status = NULL;
    OpenAPI_flow_status_e f_statusVariable = 0;
    cJSON *mar_bw_dl = NULL;
    cJSON *mar_bw_ul = NULL;
    cJSON *max_packet_loss_rate_dl = NULL;
    cJSON *max_packet_loss_rate_ul = NULL;
    cJSON *max_supp_bw_dl = NULL;
    cJSON *max_supp_bw_ul = NULL;
    cJSON *med_comp_n = NULL;
    cJSON *med_sub_comps = NULL;
    OpenAPI_list_t *med_sub_compsList = NULL;
    cJSON *med_type = NULL;
    OpenAPI_media_type_e med_typeVariable = 0;
    cJSON *min_des_bw_dl = NULL;
    cJSON *min_des_bw_ul = NULL;
    cJSON *mir_bw_dl = NULL;
    cJSON *mir_bw_ul = NULL;
    cJSON *preempt_cap = NULL;
    OpenAPI_preemption_capability_e preempt_capVariable = 0;
    cJSON *preempt_vuln = NULL;
    OpenAPI_preemption_vulnerability_e preempt_vulnVariable = 0;
    cJSON *prio_sharing_ind = NULL;
    OpenAPI_priority_sharing_indicator_e prio_sharing_indVariable = 0;
    cJSON *res_prio = NULL;
    OpenAPI_reserv_priority_e res_prioVariable = 0;
    cJSON *rr_bw = NULL;
    cJSON *rs_bw = NULL;
    cJSON *sharing_key_dl = NULL;
    cJSON *sharing_key_ul = NULL;
    cJSON *tsn_qos = NULL;
    OpenAPI_tsn_qos_container_t *tsn_qos_local_nonprim = NULL;
    cJSON *tscai_input_dl = NULL;
    OpenAPI_tscai_input_container_t *tscai_input_dl_local_nonprim = NULL;
    cJSON *tscai_input_ul = NULL;
    OpenAPI_tscai_input_container_t *tscai_input_ul_local_nonprim = NULL;
    cJSON *tscai_time_dom = NULL;
    af_app_id = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "afAppId");
    if (af_app_id) {
    if (!cJSON_IsString(af_app_id) && !cJSON_IsNull(af_app_id)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [af_app_id]");
        goto end;
    }
    }

    af_rout_req = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "afRoutReq");
    if (af_rout_req) {
    af_rout_req_local_nonprim = OpenAPI_af_routing_requirement_parseFromJSON(af_rout_req);
    if (!af_rout_req_local_nonprim) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON failed [af_rout_req]");
        goto end;
    }
    }

    qos_reference = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "qosReference");
    if (qos_reference) {
    if (!cJSON_IsString(qos_reference) && !cJSON_IsNull(qos_reference)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [qos_reference]");
        goto end;
    }
    }

    dis_ue_notif = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "disUeNotif");
    if (dis_ue_notif) {
    if (!cJSON_IsBool(dis_ue_notif)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [dis_ue_notif]");
        goto end;
    }
    }

    alt_ser_reqs = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "altSerReqs");
    if (alt_ser_reqs) {
        cJSON *alt_ser_reqs_local = NULL;
        if (!cJSON_IsArray(alt_ser_reqs)) {
            ogs_error("OpenAPI_media_component_parseFromJSON() failed [alt_ser_reqs]");
            goto end;
        }

        alt_ser_reqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_ser_reqs_local, alt_ser_reqs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(alt_ser_reqs_local)) {
                ogs_error("OpenAPI_media_component_parseFromJSON() failed [alt_ser_reqs]");
                goto end;
            }
            OpenAPI_list_add(alt_ser_reqsList, ogs_strdup(alt_ser_reqs_local->valuestring));
        }
    }

    alt_ser_reqs_data = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "altSerReqsData");
    if (alt_ser_reqs_data) {
        cJSON *alt_ser_reqs_data_local = NULL;
        if (!cJSON_IsArray(alt_ser_reqs_data)) {
            ogs_error("OpenAPI_media_component_parseFromJSON() failed [alt_ser_reqs_data]");
            goto end;
        }

        alt_ser_reqs_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_ser_reqs_data_local, alt_ser_reqs_data) {
            if (!cJSON_IsObject(alt_ser_reqs_data_local)) {
                ogs_error("OpenAPI_media_component_parseFromJSON() failed [alt_ser_reqs_data]");
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

    cont_ver = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "contVer");
    if (cont_ver) {
    if (!cJSON_IsNumber(cont_ver)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [cont_ver]");
        goto end;
    }
    }

    codecs = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "codecs");
    if (codecs) {
        cJSON *codecs_local = NULL;
        if (!cJSON_IsArray(codecs)) {
            ogs_error("OpenAPI_media_component_parseFromJSON() failed [codecs]");
            goto end;
        }

        codecsList = OpenAPI_list_create();

        cJSON_ArrayForEach(codecs_local, codecs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(codecs_local)) {
                ogs_error("OpenAPI_media_component_parseFromJSON() failed [codecs]");
                goto end;
            }
            OpenAPI_list_add(codecsList, ogs_strdup(codecs_local->valuestring));
        }
    }

    des_max_latency = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "desMaxLatency");
    if (des_max_latency) {
    if (!cJSON_IsNumber(des_max_latency)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [des_max_latency]");
        goto end;
    }
    }

    des_max_loss = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "desMaxLoss");
    if (des_max_loss) {
    if (!cJSON_IsNumber(des_max_loss)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [des_max_loss]");
        goto end;
    }
    }

    flus_id = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "flusId");
    if (flus_id) {
    if (!cJSON_IsString(flus_id) && !cJSON_IsNull(flus_id)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [flus_id]");
        goto end;
    }
    }

    f_status = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "fStatus");
    if (f_status) {
    if (!cJSON_IsString(f_status)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [f_status]");
        goto end;
    }
    f_statusVariable = OpenAPI_flow_status_FromString(f_status->valuestring);
    }

    mar_bw_dl = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "marBwDl");
    if (mar_bw_dl) {
    if (!cJSON_IsString(mar_bw_dl) && !cJSON_IsNull(mar_bw_dl)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    mar_bw_ul = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "marBwUl");
    if (mar_bw_ul) {
    if (!cJSON_IsString(mar_bw_ul) && !cJSON_IsNull(mar_bw_ul)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    max_packet_loss_rate_dl = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "maxPacketLossRateDl");
    if (max_packet_loss_rate_dl) {
    if (!cJSON_IsNull(max_packet_loss_rate_dl)) {
    if (!cJSON_IsNumber(max_packet_loss_rate_dl)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [max_packet_loss_rate_dl]");
        goto end;
    }
    }
    }

    max_packet_loss_rate_ul = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "maxPacketLossRateUl");
    if (max_packet_loss_rate_ul) {
    if (!cJSON_IsNull(max_packet_loss_rate_ul)) {
    if (!cJSON_IsNumber(max_packet_loss_rate_ul)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [max_packet_loss_rate_ul]");
        goto end;
    }
    }
    }

    max_supp_bw_dl = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "maxSuppBwDl");
    if (max_supp_bw_dl) {
    if (!cJSON_IsString(max_supp_bw_dl) && !cJSON_IsNull(max_supp_bw_dl)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [max_supp_bw_dl]");
        goto end;
    }
    }

    max_supp_bw_ul = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "maxSuppBwUl");
    if (max_supp_bw_ul) {
    if (!cJSON_IsString(max_supp_bw_ul) && !cJSON_IsNull(max_supp_bw_ul)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [max_supp_bw_ul]");
        goto end;
    }
    }

    med_comp_n = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "medCompN");
    if (!med_comp_n) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [med_comp_n]");
        goto end;
    }
    if (!cJSON_IsNumber(med_comp_n)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [med_comp_n]");
        goto end;
    }

    med_sub_comps = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "medSubComps");
    if (med_sub_comps) {
        cJSON *med_sub_comps_local_map = NULL;
        if (!cJSON_IsObject(med_sub_comps) && !cJSON_IsNull(med_sub_comps)) {
            ogs_error("OpenAPI_media_component_parseFromJSON() failed [med_sub_comps]");
            goto end;
        }
        if (cJSON_IsObject(med_sub_comps)) {
            med_sub_compsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(med_sub_comps_local_map, med_sub_comps) {
                cJSON *localMapObject = med_sub_comps_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_media_sub_component_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_media_component_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(med_sub_compsList, localMapKeyPair);
            }
        }
    }

    med_type = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "medType");
    if (med_type) {
    if (!cJSON_IsString(med_type)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [med_type]");
        goto end;
    }
    med_typeVariable = OpenAPI_media_type_FromString(med_type->valuestring);
    }

    min_des_bw_dl = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "minDesBwDl");
    if (min_des_bw_dl) {
    if (!cJSON_IsString(min_des_bw_dl) && !cJSON_IsNull(min_des_bw_dl)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [min_des_bw_dl]");
        goto end;
    }
    }

    min_des_bw_ul = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "minDesBwUl");
    if (min_des_bw_ul) {
    if (!cJSON_IsString(min_des_bw_ul) && !cJSON_IsNull(min_des_bw_ul)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [min_des_bw_ul]");
        goto end;
    }
    }

    mir_bw_dl = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "mirBwDl");
    if (mir_bw_dl) {
    if (!cJSON_IsString(mir_bw_dl) && !cJSON_IsNull(mir_bw_dl)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [mir_bw_dl]");
        goto end;
    }
    }

    mir_bw_ul = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "mirBwUl");
    if (mir_bw_ul) {
    if (!cJSON_IsString(mir_bw_ul) && !cJSON_IsNull(mir_bw_ul)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [mir_bw_ul]");
        goto end;
    }
    }

    preempt_cap = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "preemptCap");
    if (preempt_cap) {
    if (!cJSON_IsString(preempt_cap)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [preempt_cap]");
        goto end;
    }
    preempt_capVariable = OpenAPI_preemption_capability_FromString(preempt_cap->valuestring);
    }

    preempt_vuln = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "preemptVuln");
    if (preempt_vuln) {
    if (!cJSON_IsString(preempt_vuln)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [preempt_vuln]");
        goto end;
    }
    preempt_vulnVariable = OpenAPI_preemption_vulnerability_FromString(preempt_vuln->valuestring);
    }

    prio_sharing_ind = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "prioSharingInd");
    if (prio_sharing_ind) {
    if (!cJSON_IsString(prio_sharing_ind)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [prio_sharing_ind]");
        goto end;
    }
    prio_sharing_indVariable = OpenAPI_priority_sharing_indicator_FromString(prio_sharing_ind->valuestring);
    }

    res_prio = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "resPrio");
    if (res_prio) {
    if (!cJSON_IsString(res_prio)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [res_prio]");
        goto end;
    }
    res_prioVariable = OpenAPI_reserv_priority_FromString(res_prio->valuestring);
    }

    rr_bw = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "rrBw");
    if (rr_bw) {
    if (!cJSON_IsString(rr_bw) && !cJSON_IsNull(rr_bw)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [rr_bw]");
        goto end;
    }
    }

    rs_bw = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "rsBw");
    if (rs_bw) {
    if (!cJSON_IsString(rs_bw) && !cJSON_IsNull(rs_bw)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [rs_bw]");
        goto end;
    }
    }

    sharing_key_dl = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "sharingKeyDl");
    if (sharing_key_dl) {
    if (!cJSON_IsNumber(sharing_key_dl)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [sharing_key_dl]");
        goto end;
    }
    }

    sharing_key_ul = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "sharingKeyUl");
    if (sharing_key_ul) {
    if (!cJSON_IsNumber(sharing_key_ul)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [sharing_key_ul]");
        goto end;
    }
    }

    tsn_qos = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "tsnQos");
    if (tsn_qos) {
    tsn_qos_local_nonprim = OpenAPI_tsn_qos_container_parseFromJSON(tsn_qos);
    if (!tsn_qos_local_nonprim) {
        ogs_error("OpenAPI_tsn_qos_container_parseFromJSON failed [tsn_qos]");
        goto end;
    }
    }

    tscai_input_dl = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "tscaiInputDl");
    if (tscai_input_dl) {
    if (!cJSON_IsNull(tscai_input_dl)) {
    tscai_input_dl_local_nonprim = OpenAPI_tscai_input_container_parseFromJSON(tscai_input_dl);
    if (!tscai_input_dl_local_nonprim) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON failed [tscai_input_dl]");
        goto end;
    }
    }
    }

    tscai_input_ul = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "tscaiInputUl");
    if (tscai_input_ul) {
    if (!cJSON_IsNull(tscai_input_ul)) {
    tscai_input_ul_local_nonprim = OpenAPI_tscai_input_container_parseFromJSON(tscai_input_ul);
    if (!tscai_input_ul_local_nonprim) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON failed [tscai_input_ul]");
        goto end;
    }
    }
    }

    tscai_time_dom = cJSON_GetObjectItemCaseSensitive(media_componentJSON, "tscaiTimeDom");
    if (tscai_time_dom) {
    if (!cJSON_IsNumber(tscai_time_dom)) {
        ogs_error("OpenAPI_media_component_parseFromJSON() failed [tscai_time_dom]");
        goto end;
    }
    }

    media_component_local_var = OpenAPI_media_component_create (
        af_app_id && !cJSON_IsNull(af_app_id) ? ogs_strdup(af_app_id->valuestring) : NULL,
        af_rout_req ? af_rout_req_local_nonprim : NULL,
        qos_reference && !cJSON_IsNull(qos_reference) ? ogs_strdup(qos_reference->valuestring) : NULL,
        dis_ue_notif ? true : false,
        dis_ue_notif ? dis_ue_notif->valueint : 0,
        alt_ser_reqs ? alt_ser_reqsList : NULL,
        alt_ser_reqs_data ? alt_ser_reqs_dataList : NULL,
        cont_ver ? true : false,
        cont_ver ? cont_ver->valuedouble : 0,
        codecs ? codecsList : NULL,
        des_max_latency ? true : false,
        des_max_latency ? des_max_latency->valuedouble : 0,
        des_max_loss ? true : false,
        des_max_loss ? des_max_loss->valuedouble : 0,
        flus_id && !cJSON_IsNull(flus_id) ? ogs_strdup(flus_id->valuestring) : NULL,
        f_status ? f_statusVariable : 0,
        mar_bw_dl && !cJSON_IsNull(mar_bw_dl) ? ogs_strdup(mar_bw_dl->valuestring) : NULL,
        mar_bw_ul && !cJSON_IsNull(mar_bw_ul) ? ogs_strdup(mar_bw_ul->valuestring) : NULL,
        max_packet_loss_rate_dl && cJSON_IsNull(max_packet_loss_rate_dl) ? true : false,
        max_packet_loss_rate_dl ? true : false,
        max_packet_loss_rate_dl ? max_packet_loss_rate_dl->valuedouble : 0,
        max_packet_loss_rate_ul && cJSON_IsNull(max_packet_loss_rate_ul) ? true : false,
        max_packet_loss_rate_ul ? true : false,
        max_packet_loss_rate_ul ? max_packet_loss_rate_ul->valuedouble : 0,
        max_supp_bw_dl && !cJSON_IsNull(max_supp_bw_dl) ? ogs_strdup(max_supp_bw_dl->valuestring) : NULL,
        max_supp_bw_ul && !cJSON_IsNull(max_supp_bw_ul) ? ogs_strdup(max_supp_bw_ul->valuestring) : NULL,
        
        med_comp_n->valuedouble,
        med_sub_comps ? med_sub_compsList : NULL,
        med_type ? med_typeVariable : 0,
        min_des_bw_dl && !cJSON_IsNull(min_des_bw_dl) ? ogs_strdup(min_des_bw_dl->valuestring) : NULL,
        min_des_bw_ul && !cJSON_IsNull(min_des_bw_ul) ? ogs_strdup(min_des_bw_ul->valuestring) : NULL,
        mir_bw_dl && !cJSON_IsNull(mir_bw_dl) ? ogs_strdup(mir_bw_dl->valuestring) : NULL,
        mir_bw_ul && !cJSON_IsNull(mir_bw_ul) ? ogs_strdup(mir_bw_ul->valuestring) : NULL,
        preempt_cap ? preempt_capVariable : 0,
        preempt_vuln ? preempt_vulnVariable : 0,
        prio_sharing_ind ? prio_sharing_indVariable : 0,
        res_prio ? res_prioVariable : 0,
        rr_bw && !cJSON_IsNull(rr_bw) ? ogs_strdup(rr_bw->valuestring) : NULL,
        rs_bw && !cJSON_IsNull(rs_bw) ? ogs_strdup(rs_bw->valuestring) : NULL,
        sharing_key_dl ? true : false,
        sharing_key_dl ? sharing_key_dl->valuedouble : 0,
        sharing_key_ul ? true : false,
        sharing_key_ul ? sharing_key_ul->valuedouble : 0,
        tsn_qos ? tsn_qos_local_nonprim : NULL,
        tscai_input_dl && cJSON_IsNull(tscai_input_dl) ? true : false,
        tscai_input_dl ? tscai_input_dl_local_nonprim : NULL,
        tscai_input_ul && cJSON_IsNull(tscai_input_ul) ? true : false,
        tscai_input_ul ? tscai_input_ul_local_nonprim : NULL,
        tscai_time_dom ? true : false,
        tscai_time_dom ? tscai_time_dom->valuedouble : 0
    );

    return media_component_local_var;
end:
    if (af_rout_req_local_nonprim) {
        OpenAPI_af_routing_requirement_free(af_rout_req_local_nonprim);
        af_rout_req_local_nonprim = NULL;
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
    if (codecsList) {
        OpenAPI_list_for_each(codecsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(codecsList);
        codecsList = NULL;
    }
    if (med_sub_compsList) {
        OpenAPI_list_for_each(med_sub_compsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_media_sub_component_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(med_sub_compsList);
        med_sub_compsList = NULL;
    }
    if (tsn_qos_local_nonprim) {
        OpenAPI_tsn_qos_container_free(tsn_qos_local_nonprim);
        tsn_qos_local_nonprim = NULL;
    }
    if (tscai_input_dl_local_nonprim) {
        OpenAPI_tscai_input_container_free(tscai_input_dl_local_nonprim);
        tscai_input_dl_local_nonprim = NULL;
    }
    if (tscai_input_ul_local_nonprim) {
        OpenAPI_tscai_input_container_free(tscai_input_ul_local_nonprim);
        tscai_input_ul_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_media_component_t *OpenAPI_media_component_copy(OpenAPI_media_component_t *dst, OpenAPI_media_component_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_media_component_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_media_component_convertToJSON() failed");
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

    OpenAPI_media_component_free(dst);
    dst = OpenAPI_media_component_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

