
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context.h"

OpenAPI_ue_context_t *OpenAPI_ue_context_create(
    char *supi,
    int supi_unauth_ind,
    OpenAPI_list_t *gpsi_list,
    char *pei,
    char *udm_group_id,
    char *ausf_group_id,
    char *routing_indicator,
    OpenAPI_list_t *group_list,
    char drx_parameter,
    int sub_rfsp,
    int used_rfsp,
    OpenAPI_ambr_t *sub_ue_ambr,
    char *smsf_id,
    OpenAPI_seaf_data_t *seaf_data,
    char _5g_mm_capability,
    char *pcf_id,
    char *pcf_set_id,
    char *pcf_amp_service_set_id,
    char *pcf_uep_service_set_id,
    OpenAPI_sbi_binding_level_e pcf_binding,
    char *pcf_am_policy_uri,
    OpenAPI_list_t *am_policy_req_trigger_list,
    char *pcf_ue_policy_uri,
    OpenAPI_list_t *ue_policy_req_trigger_list,
    char *hpcf_id,
    OpenAPI_list_t *restricted_rat_list,
    OpenAPI_list_t *forbidden_area_list,
    OpenAPI_service_area_restriction_t *service_area_restriction,
    OpenAPI_list_t *restricted_core_nw_type_list,
    OpenAPI_list_t *event_subscription_list,
    OpenAPI_list_t *mm_context_list,
    OpenAPI_list_t *session_context_list,
    OpenAPI_trace_data_t *trace_data,
    char *service_gap_expiry_time,
    char *stn_sr,
    char *c_msisdn,
    char ms_classmark2,
    OpenAPI_list_t *supported_codec_list,
    OpenAPI_list_t *small_data_rate_status_infos,
    OpenAPI_list_t *restricted_primary_rat_list,
    OpenAPI_list_t *restricted_secondary_rat_list,
    OpenAPI_list_t *v2x_context,
    int lte_cat_m_ind,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter
    )
{
    OpenAPI_ue_context_t *ue_context_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_context_t));
    if (!ue_context_local_var) {
        return NULL;
    }
    ue_context_local_var->supi = supi;
    ue_context_local_var->supi_unauth_ind = supi_unauth_ind;
    ue_context_local_var->gpsi_list = gpsi_list;
    ue_context_local_var->pei = pei;
    ue_context_local_var->udm_group_id = udm_group_id;
    ue_context_local_var->ausf_group_id = ausf_group_id;
    ue_context_local_var->routing_indicator = routing_indicator;
    ue_context_local_var->group_list = group_list;
    ue_context_local_var->drx_parameter = drx_parameter;
    ue_context_local_var->sub_rfsp = sub_rfsp;
    ue_context_local_var->used_rfsp = used_rfsp;
    ue_context_local_var->sub_ue_ambr = sub_ue_ambr;
    ue_context_local_var->smsf_id = smsf_id;
    ue_context_local_var->seaf_data = seaf_data;
    ue_context_local_var->_5g_mm_capability = _5g_mm_capability;
    ue_context_local_var->pcf_id = pcf_id;
    ue_context_local_var->pcf_set_id = pcf_set_id;
    ue_context_local_var->pcf_amp_service_set_id = pcf_amp_service_set_id;
    ue_context_local_var->pcf_uep_service_set_id = pcf_uep_service_set_id;
    ue_context_local_var->pcf_binding = pcf_binding;
    ue_context_local_var->pcf_am_policy_uri = pcf_am_policy_uri;
    ue_context_local_var->am_policy_req_trigger_list = am_policy_req_trigger_list;
    ue_context_local_var->pcf_ue_policy_uri = pcf_ue_policy_uri;
    ue_context_local_var->ue_policy_req_trigger_list = ue_policy_req_trigger_list;
    ue_context_local_var->hpcf_id = hpcf_id;
    ue_context_local_var->restricted_rat_list = restricted_rat_list;
    ue_context_local_var->forbidden_area_list = forbidden_area_list;
    ue_context_local_var->service_area_restriction = service_area_restriction;
    ue_context_local_var->restricted_core_nw_type_list = restricted_core_nw_type_list;
    ue_context_local_var->event_subscription_list = event_subscription_list;
    ue_context_local_var->mm_context_list = mm_context_list;
    ue_context_local_var->session_context_list = session_context_list;
    ue_context_local_var->trace_data = trace_data;
    ue_context_local_var->service_gap_expiry_time = service_gap_expiry_time;
    ue_context_local_var->stn_sr = stn_sr;
    ue_context_local_var->c_msisdn = c_msisdn;
    ue_context_local_var->ms_classmark2 = ms_classmark2;
    ue_context_local_var->supported_codec_list = supported_codec_list;
    ue_context_local_var->small_data_rate_status_infos = small_data_rate_status_infos;
    ue_context_local_var->restricted_primary_rat_list = restricted_primary_rat_list;
    ue_context_local_var->restricted_secondary_rat_list = restricted_secondary_rat_list;
    ue_context_local_var->v2x_context = v2x_context;
    ue_context_local_var->lte_cat_m_ind = lte_cat_m_ind;
    ue_context_local_var->mo_exp_data_counter = mo_exp_data_counter;

    return ue_context_local_var;
}

void OpenAPI_ue_context_free(OpenAPI_ue_context_t *ue_context)
{
    if (NULL == ue_context) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ue_context->supi);
    OpenAPI_list_for_each(ue_context->gpsi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ue_context->gpsi_list);
    ogs_free(ue_context->pei);
    ogs_free(ue_context->udm_group_id);
    ogs_free(ue_context->ausf_group_id);
    ogs_free(ue_context->routing_indicator);
    OpenAPI_list_for_each(ue_context->group_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ue_context->group_list);
    OpenAPI_ambr_free(ue_context->sub_ue_ambr);
    ogs_free(ue_context->smsf_id);
    OpenAPI_seaf_data_free(ue_context->seaf_data);
    ogs_free(ue_context->pcf_id);
    ogs_free(ue_context->pcf_set_id);
    ogs_free(ue_context->pcf_amp_service_set_id);
    ogs_free(ue_context->pcf_uep_service_set_id);
    ogs_free(ue_context->pcf_am_policy_uri);
    OpenAPI_list_free(ue_context->am_policy_req_trigger_list);
    ogs_free(ue_context->pcf_ue_policy_uri);
    OpenAPI_list_free(ue_context->ue_policy_req_trigger_list);
    ogs_free(ue_context->hpcf_id);
    OpenAPI_list_free(ue_context->restricted_rat_list);
    OpenAPI_list_for_each(ue_context->forbidden_area_list, node) {
        OpenAPI_area_free(node->data);
    }
    OpenAPI_list_free(ue_context->forbidden_area_list);
    OpenAPI_service_area_restriction_free(ue_context->service_area_restriction);
    OpenAPI_list_free(ue_context->restricted_core_nw_type_list);
    OpenAPI_list_for_each(ue_context->event_subscription_list, node) {
        OpenAPI_amf_event_subscription_free(node->data);
    }
    OpenAPI_list_free(ue_context->event_subscription_list);
    OpenAPI_list_for_each(ue_context->mm_context_list, node) {
        OpenAPI_mm_context_free(node->data);
    }
    OpenAPI_list_free(ue_context->mm_context_list);
    OpenAPI_list_for_each(ue_context->session_context_list, node) {
        OpenAPI_pdu_session_context_free(node->data);
    }
    OpenAPI_list_free(ue_context->session_context_list);
    OpenAPI_trace_data_free(ue_context->trace_data);
    ogs_free(ue_context->service_gap_expiry_time);
    ogs_free(ue_context->stn_sr);
    ogs_free(ue_context->c_msisdn);
    OpenAPI_list_for_each(ue_context->supported_codec_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ue_context->supported_codec_list);
    OpenAPI_list_for_each(ue_context->small_data_rate_status_infos, node) {
        OpenAPI_small_data_rate_status_info_free(node->data);
    }
    OpenAPI_list_free(ue_context->small_data_rate_status_infos);
    OpenAPI_list_free(ue_context->restricted_primary_rat_list);
    OpenAPI_list_free(ue_context->restricted_secondary_rat_list);
    OpenAPI_list_for_each(ue_context->v2x_context, node) {
        OpenAPI_v2x_context_free(node->data);
    }
    OpenAPI_list_free(ue_context->v2x_context);
    OpenAPI_mo_exp_data_counter_free(ue_context->mo_exp_data_counter);
    ogs_free(ue_context);
}

cJSON *OpenAPI_ue_context_convertToJSON(OpenAPI_ue_context_t *ue_context)
{
    cJSON *item = NULL;

    if (ue_context == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [UeContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context->supi) {
        if (cJSON_AddStringToObject(item, "supi", ue_context->supi) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [supi]");
            goto end;
        }
    }

    if (ue_context->supi_unauth_ind) {
        if (cJSON_AddBoolToObject(item, "supiUnauthInd", ue_context->supi_unauth_ind) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [supi_unauth_ind]");
            goto end;
        }
    }

    if (ue_context->gpsi_list) {
        cJSON *gpsi_list = cJSON_AddArrayToObject(item, "gpsiList");
        if (gpsi_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [gpsi_list]");
            goto end;
        }

        OpenAPI_lnode_t *gpsi_list_node;
        OpenAPI_list_for_each(ue_context->gpsi_list, gpsi_list_node)  {
            if (cJSON_AddStringToObject(gpsi_list, "", (char*)gpsi_list_node->data) == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [gpsi_list]");
                goto end;
            }
        }
    }

    if (ue_context->pei) {
        if (cJSON_AddStringToObject(item, "pei", ue_context->pei) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pei]");
            goto end;
        }
    }

    if (ue_context->udm_group_id) {
        if (cJSON_AddStringToObject(item, "udmGroupId", ue_context->udm_group_id) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [udm_group_id]");
            goto end;
        }
    }

    if (ue_context->ausf_group_id) {
        if (cJSON_AddStringToObject(item, "ausfGroupId", ue_context->ausf_group_id) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [ausf_group_id]");
            goto end;
        }
    }

    if (ue_context->routing_indicator) {
        if (cJSON_AddStringToObject(item, "routingIndicator", ue_context->routing_indicator) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [routing_indicator]");
            goto end;
        }
    }

    if (ue_context->group_list) {
        cJSON *group_list = cJSON_AddArrayToObject(item, "groupList");
        if (group_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [group_list]");
            goto end;
        }

        OpenAPI_lnode_t *group_list_node;
        OpenAPI_list_for_each(ue_context->group_list, group_list_node)  {
            if (cJSON_AddStringToObject(group_list, "", (char*)group_list_node->data) == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [group_list]");
                goto end;
            }
        }
    }

    if (ue_context->drx_parameter) {
        if (cJSON_AddNumberToObject(item, "drxParameter", ue_context->drx_parameter) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [drx_parameter]");
            goto end;
        }
    }

    if (ue_context->sub_rfsp) {
        if (cJSON_AddNumberToObject(item, "subRfsp", ue_context->sub_rfsp) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_rfsp]");
            goto end;
        }
    }

    if (ue_context->used_rfsp) {
        if (cJSON_AddNumberToObject(item, "usedRfsp", ue_context->used_rfsp) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [used_rfsp]");
            goto end;
        }
    }

    if (ue_context->sub_ue_ambr) {
        cJSON *sub_ue_ambr_local_JSON = OpenAPI_ambr_convertToJSON(ue_context->sub_ue_ambr);
        if (sub_ue_ambr_local_JSON == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_ue_ambr]");
            goto end;
        }
        cJSON_AddItemToObject(item, "subUeAmbr", sub_ue_ambr_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_ue_ambr]");
            goto end;
        }
    }

    if (ue_context->smsf_id) {
        if (cJSON_AddStringToObject(item, "smsfId", ue_context->smsf_id) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [smsf_id]");
            goto end;
        }
    }

    if (ue_context->seaf_data) {
        cJSON *seaf_data_local_JSON = OpenAPI_seaf_data_convertToJSON(ue_context->seaf_data);
        if (seaf_data_local_JSON == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [seaf_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "seafData", seaf_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [seaf_data]");
            goto end;
        }
    }

    if (ue_context->_5g_mm_capability) {
        if (cJSON_AddNumberToObject(item, "5gMmCapability", ue_context->_5g_mm_capability) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [_5g_mm_capability]");
            goto end;
        }
    }

    if (ue_context->pcf_id) {
        if (cJSON_AddStringToObject(item, "pcfId", ue_context->pcf_id) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_id]");
            goto end;
        }
    }

    if (ue_context->pcf_set_id) {
        if (cJSON_AddStringToObject(item, "pcfSetId", ue_context->pcf_set_id) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_set_id]");
            goto end;
        }
    }

    if (ue_context->pcf_amp_service_set_id) {
        if (cJSON_AddStringToObject(item, "pcfAmpServiceSetId", ue_context->pcf_amp_service_set_id) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_amp_service_set_id]");
            goto end;
        }
    }

    if (ue_context->pcf_uep_service_set_id) {
        if (cJSON_AddStringToObject(item, "pcfUepServiceSetId", ue_context->pcf_uep_service_set_id) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_uep_service_set_id]");
            goto end;
        }
    }

    if (ue_context->pcf_binding) {
        if (cJSON_AddStringToObject(item, "pcfBinding", OpenAPI_sbi_binding_level_ToString(ue_context->pcf_binding)) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_binding]");
            goto end;
        }
    }

    if (ue_context->pcf_am_policy_uri) {
        if (cJSON_AddStringToObject(item, "pcfAmPolicyUri", ue_context->pcf_am_policy_uri) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_am_policy_uri]");
            goto end;
        }
    }

    if (ue_context->am_policy_req_trigger_list) {
        cJSON *am_policy_req_trigger_list = cJSON_AddArrayToObject(item, "amPolicyReqTriggerList");
        if (am_policy_req_trigger_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [am_policy_req_trigger_list]");
            goto end;
        }
        OpenAPI_lnode_t *am_policy_req_trigger_list_node;
        OpenAPI_list_for_each(ue_context->am_policy_req_trigger_list, am_policy_req_trigger_list_node) {
            if (cJSON_AddStringToObject(am_policy_req_trigger_list, "", OpenAPI_policy_req_trigger_ToString((OpenAPI_policy_req_trigger_e)am_policy_req_trigger_list_node->data)) == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [am_policy_req_trigger_list]");
                goto end;
            }
        }
    }

    if (ue_context->pcf_ue_policy_uri) {
        if (cJSON_AddStringToObject(item, "pcfUePolicyUri", ue_context->pcf_ue_policy_uri) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_ue_policy_uri]");
            goto end;
        }
    }

    if (ue_context->ue_policy_req_trigger_list) {
        cJSON *ue_policy_req_trigger_list = cJSON_AddArrayToObject(item, "uePolicyReqTriggerList");
        if (ue_policy_req_trigger_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [ue_policy_req_trigger_list]");
            goto end;
        }
        OpenAPI_lnode_t *ue_policy_req_trigger_list_node;
        OpenAPI_list_for_each(ue_context->ue_policy_req_trigger_list, ue_policy_req_trigger_list_node) {
            if (cJSON_AddStringToObject(ue_policy_req_trigger_list, "", OpenAPI_policy_req_trigger_ToString((OpenAPI_policy_req_trigger_e)ue_policy_req_trigger_list_node->data)) == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [ue_policy_req_trigger_list]");
                goto end;
            }
        }
    }

    if (ue_context->hpcf_id) {
        if (cJSON_AddStringToObject(item, "hpcfId", ue_context->hpcf_id) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [hpcf_id]");
            goto end;
        }
    }

    if (ue_context->restricted_rat_list) {
        cJSON *restricted_rat_list = cJSON_AddArrayToObject(item, "restrictedRatList");
        if (restricted_rat_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_rat_list]");
            goto end;
        }
        OpenAPI_lnode_t *restricted_rat_list_node;
        OpenAPI_list_for_each(ue_context->restricted_rat_list, restricted_rat_list_node) {
            if (cJSON_AddStringToObject(restricted_rat_list, "", OpenAPI_rat_type_ToString((OpenAPI_rat_type_e)restricted_rat_list_node->data)) == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_rat_list]");
                goto end;
            }
        }
    }

    if (ue_context->forbidden_area_list) {
        cJSON *forbidden_area_listList = cJSON_AddArrayToObject(item, "forbiddenAreaList");
        if (forbidden_area_listList == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [forbidden_area_list]");
            goto end;
        }

        OpenAPI_lnode_t *forbidden_area_list_node;
        if (ue_context->forbidden_area_list) {
            OpenAPI_list_for_each(ue_context->forbidden_area_list, forbidden_area_list_node) {
                cJSON *itemLocal = OpenAPI_area_convertToJSON(forbidden_area_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_context_convertToJSON() failed [forbidden_area_list]");
                    goto end;
                }
                cJSON_AddItemToArray(forbidden_area_listList, itemLocal);
            }
        }
    }

    if (ue_context->service_area_restriction) {
        cJSON *service_area_restriction_local_JSON = OpenAPI_service_area_restriction_convertToJSON(ue_context->service_area_restriction);
        if (service_area_restriction_local_JSON == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [service_area_restriction]");
            goto end;
        }
        cJSON_AddItemToObject(item, "serviceAreaRestriction", service_area_restriction_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [service_area_restriction]");
            goto end;
        }
    }

    if (ue_context->restricted_core_nw_type_list) {
        cJSON *restricted_core_nw_type_list = cJSON_AddArrayToObject(item, "restrictedCoreNwTypeList");
        if (restricted_core_nw_type_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_core_nw_type_list]");
            goto end;
        }
        OpenAPI_lnode_t *restricted_core_nw_type_list_node;
        OpenAPI_list_for_each(ue_context->restricted_core_nw_type_list, restricted_core_nw_type_list_node) {
            if (cJSON_AddStringToObject(restricted_core_nw_type_list, "", OpenAPI_core_network_type_ToString((OpenAPI_core_network_type_e)restricted_core_nw_type_list_node->data)) == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_core_nw_type_list]");
                goto end;
            }
        }
    }

    if (ue_context->event_subscription_list) {
        cJSON *event_subscription_listList = cJSON_AddArrayToObject(item, "eventSubscriptionList");
        if (event_subscription_listList == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [event_subscription_list]");
            goto end;
        }

        OpenAPI_lnode_t *event_subscription_list_node;
        if (ue_context->event_subscription_list) {
            OpenAPI_list_for_each(ue_context->event_subscription_list, event_subscription_list_node) {
                cJSON *itemLocal = OpenAPI_amf_event_subscription_convertToJSON(event_subscription_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_context_convertToJSON() failed [event_subscription_list]");
                    goto end;
                }
                cJSON_AddItemToArray(event_subscription_listList, itemLocal);
            }
        }
    }

    if (ue_context->mm_context_list) {
        cJSON *mm_context_listList = cJSON_AddArrayToObject(item, "mmContextList");
        if (mm_context_listList == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [mm_context_list]");
            goto end;
        }

        OpenAPI_lnode_t *mm_context_list_node;
        if (ue_context->mm_context_list) {
            OpenAPI_list_for_each(ue_context->mm_context_list, mm_context_list_node) {
                cJSON *itemLocal = OpenAPI_mm_context_convertToJSON(mm_context_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_context_convertToJSON() failed [mm_context_list]");
                    goto end;
                }
                cJSON_AddItemToArray(mm_context_listList, itemLocal);
            }
        }
    }

    if (ue_context->session_context_list) {
        cJSON *session_context_listList = cJSON_AddArrayToObject(item, "sessionContextList");
        if (session_context_listList == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [session_context_list]");
            goto end;
        }

        OpenAPI_lnode_t *session_context_list_node;
        if (ue_context->session_context_list) {
            OpenAPI_list_for_each(ue_context->session_context_list, session_context_list_node) {
                cJSON *itemLocal = OpenAPI_pdu_session_context_convertToJSON(session_context_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_context_convertToJSON() failed [session_context_list]");
                    goto end;
                }
                cJSON_AddItemToArray(session_context_listList, itemLocal);
            }
        }
    }

    if (ue_context->trace_data) {
        cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(ue_context->trace_data);
        if (trace_data_local_JSON == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [trace_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (ue_context->service_gap_expiry_time) {
        if (cJSON_AddStringToObject(item, "serviceGapExpiryTime", ue_context->service_gap_expiry_time) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [service_gap_expiry_time]");
            goto end;
        }
    }

    if (ue_context->stn_sr) {
        if (cJSON_AddStringToObject(item, "stnSr", ue_context->stn_sr) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [stn_sr]");
            goto end;
        }
    }

    if (ue_context->c_msisdn) {
        if (cJSON_AddStringToObject(item, "cMsisdn", ue_context->c_msisdn) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [c_msisdn]");
            goto end;
        }
    }

    if (ue_context->ms_classmark2) {
        if (cJSON_AddNumberToObject(item, "msClassmark2", ue_context->ms_classmark2) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [ms_classmark2]");
            goto end;
        }
    }

    if (ue_context->supported_codec_list) {
        cJSON *supported_codec_list = cJSON_AddArrayToObject(item, "supportedCodecList");
        if (supported_codec_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [supported_codec_list]");
            goto end;
        }

        OpenAPI_lnode_t *supported_codec_list_node;
        OpenAPI_list_for_each(ue_context->supported_codec_list, supported_codec_list_node)  {
        }
    }

    if (ue_context->small_data_rate_status_infos) {
        cJSON *small_data_rate_status_infosList = cJSON_AddArrayToObject(item, "smallDataRateStatusInfos");
        if (small_data_rate_status_infosList == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [small_data_rate_status_infos]");
            goto end;
        }

        OpenAPI_lnode_t *small_data_rate_status_infos_node;
        if (ue_context->small_data_rate_status_infos) {
            OpenAPI_list_for_each(ue_context->small_data_rate_status_infos, small_data_rate_status_infos_node) {
                cJSON *itemLocal = OpenAPI_small_data_rate_status_info_convertToJSON(small_data_rate_status_infos_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_context_convertToJSON() failed [small_data_rate_status_infos]");
                    goto end;
                }
                cJSON_AddItemToArray(small_data_rate_status_infosList, itemLocal);
            }
        }
    }

    if (ue_context->restricted_primary_rat_list) {
        cJSON *restricted_primary_rat_list = cJSON_AddArrayToObject(item, "restrictedPrimaryRatList");
        if (restricted_primary_rat_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_primary_rat_list]");
            goto end;
        }
        OpenAPI_lnode_t *restricted_primary_rat_list_node;
        OpenAPI_list_for_each(ue_context->restricted_primary_rat_list, restricted_primary_rat_list_node) {
            if (cJSON_AddStringToObject(restricted_primary_rat_list, "", OpenAPI_rat_type_ToString((OpenAPI_rat_type_e)restricted_primary_rat_list_node->data)) == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_primary_rat_list]");
                goto end;
            }
        }
    }

    if (ue_context->restricted_secondary_rat_list) {
        cJSON *restricted_secondary_rat_list = cJSON_AddArrayToObject(item, "restrictedSecondaryRatList");
        if (restricted_secondary_rat_list == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_secondary_rat_list]");
            goto end;
        }
        OpenAPI_lnode_t *restricted_secondary_rat_list_node;
        OpenAPI_list_for_each(ue_context->restricted_secondary_rat_list, restricted_secondary_rat_list_node) {
            if (cJSON_AddStringToObject(restricted_secondary_rat_list, "", OpenAPI_rat_type_ToString((OpenAPI_rat_type_e)restricted_secondary_rat_list_node->data)) == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_secondary_rat_list]");
                goto end;
            }
        }
    }

    if (ue_context->v2x_context) {
        cJSON *v2x_contextList = cJSON_AddArrayToObject(item, "v2xContext");
        if (v2x_contextList == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [v2x_context]");
            goto end;
        }

        OpenAPI_lnode_t *v2x_context_node;
        if (ue_context->v2x_context) {
            OpenAPI_list_for_each(ue_context->v2x_context, v2x_context_node) {
                cJSON *itemLocal = OpenAPI_v2x_context_convertToJSON(v2x_context_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_context_convertToJSON() failed [v2x_context]");
                    goto end;
                }
                cJSON_AddItemToArray(v2x_contextList, itemLocal);
            }
        }
    }

    if (ue_context->lte_cat_m_ind) {
        if (cJSON_AddBoolToObject(item, "lteCatMInd", ue_context->lte_cat_m_ind) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [lte_cat_m_ind]");
            goto end;
        }
    }

    if (ue_context->mo_exp_data_counter) {
        cJSON *mo_exp_data_counter_local_JSON = OpenAPI_mo_exp_data_counter_convertToJSON(ue_context->mo_exp_data_counter);
        if (mo_exp_data_counter_local_JSON == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [mo_exp_data_counter]");
            goto end;
        }
        cJSON_AddItemToObject(item, "moExpDataCounter", mo_exp_data_counter_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [mo_exp_data_counter]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ue_context_t *OpenAPI_ue_context_parseFromJSON(cJSON *ue_contextJSON)
{
    OpenAPI_ue_context_t *ue_context_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "supi");

    if (supi) {
        if (!cJSON_IsString(supi)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [supi]");
            goto end;
        }
    }

    cJSON *supi_unauth_ind = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "supiUnauthInd");

    if (supi_unauth_ind) {
        if (!cJSON_IsBool(supi_unauth_ind)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [supi_unauth_ind]");
            goto end;
        }
    }

    cJSON *gpsi_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "gpsiList");

    OpenAPI_list_t *gpsi_listList;
    if (gpsi_list) {
        cJSON *gpsi_list_local;
        if (!cJSON_IsArray(gpsi_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [gpsi_list]");
            goto end;
        }
        gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_list_local, gpsi_list) {
            if (!cJSON_IsString(gpsi_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(gpsi_listList, ogs_strdup(gpsi_list_local->valuestring));
        }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pei");

    if (pei) {
        if (!cJSON_IsString(pei)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pei]");
            goto end;
        }
    }

    cJSON *udm_group_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "udmGroupId");

    if (udm_group_id) {
        if (!cJSON_IsString(udm_group_id)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [udm_group_id]");
            goto end;
        }
    }

    cJSON *ausf_group_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "ausfGroupId");

    if (ausf_group_id) {
        if (!cJSON_IsString(ausf_group_id)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ausf_group_id]");
            goto end;
        }
    }

    cJSON *routing_indicator = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "routingIndicator");

    if (routing_indicator) {
        if (!cJSON_IsString(routing_indicator)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [routing_indicator]");
            goto end;
        }
    }

    cJSON *group_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "groupList");

    OpenAPI_list_t *group_listList;
    if (group_list) {
        cJSON *group_list_local;
        if (!cJSON_IsArray(group_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [group_list]");
            goto end;
        }
        group_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(group_list_local, group_list) {
            if (!cJSON_IsString(group_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [group_list]");
                goto end;
            }
            OpenAPI_list_add(group_listList, ogs_strdup(group_list_local->valuestring));
        }
    }

    cJSON *drx_parameter = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "drxParameter");

    if (drx_parameter) {
        if (!cJSON_IsNumber(drx_parameter)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [drx_parameter]");
            goto end;
        }
    }

    cJSON *sub_rfsp = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "subRfsp");

    if (sub_rfsp) {
        if (!cJSON_IsNumber(sub_rfsp)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [sub_rfsp]");
            goto end;
        }
    }

    cJSON *used_rfsp = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "usedRfsp");

    if (used_rfsp) {
        if (!cJSON_IsNumber(used_rfsp)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [used_rfsp]");
            goto end;
        }
    }

    cJSON *sub_ue_ambr = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "subUeAmbr");

    OpenAPI_ambr_t *sub_ue_ambr_local_nonprim = NULL;
    if (sub_ue_ambr) {
        sub_ue_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(sub_ue_ambr);
    }

    cJSON *smsf_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smsfId");

    if (smsf_id) {
        if (!cJSON_IsString(smsf_id)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [smsf_id]");
            goto end;
        }
    }

    cJSON *seaf_data = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "seafData");

    OpenAPI_seaf_data_t *seaf_data_local_nonprim = NULL;
    if (seaf_data) {
        seaf_data_local_nonprim = OpenAPI_seaf_data_parseFromJSON(seaf_data);
    }

    cJSON *_5g_mm_capability = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "5gMmCapability");

    if (_5g_mm_capability) {
        if (!cJSON_IsNumber(_5g_mm_capability)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [_5g_mm_capability]");
            goto end;
        }
    }

    cJSON *pcf_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfId");

    if (pcf_id) {
        if (!cJSON_IsString(pcf_id)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_id]");
            goto end;
        }
    }

    cJSON *pcf_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfSetId");

    if (pcf_set_id) {
        if (!cJSON_IsString(pcf_set_id)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_set_id]");
            goto end;
        }
    }

    cJSON *pcf_amp_service_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfAmpServiceSetId");

    if (pcf_amp_service_set_id) {
        if (!cJSON_IsString(pcf_amp_service_set_id)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_amp_service_set_id]");
            goto end;
        }
    }

    cJSON *pcf_uep_service_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfUepServiceSetId");

    if (pcf_uep_service_set_id) {
        if (!cJSON_IsString(pcf_uep_service_set_id)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_uep_service_set_id]");
            goto end;
        }
    }

    cJSON *pcf_binding = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfBinding");

    OpenAPI_sbi_binding_level_e pcf_bindingVariable;
    if (pcf_binding) {
        if (!cJSON_IsString(pcf_binding)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_binding]");
            goto end;
        }
        pcf_bindingVariable = OpenAPI_sbi_binding_level_FromString(pcf_binding->valuestring);
    }

    cJSON *pcf_am_policy_uri = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfAmPolicyUri");

    if (pcf_am_policy_uri) {
        if (!cJSON_IsString(pcf_am_policy_uri)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_am_policy_uri]");
            goto end;
        }
    }

    cJSON *am_policy_req_trigger_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "amPolicyReqTriggerList");

    OpenAPI_list_t *am_policy_req_trigger_listList;
    if (am_policy_req_trigger_list) {
        cJSON *am_policy_req_trigger_list_local_nonprimitive;
        if (!cJSON_IsArray(am_policy_req_trigger_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [am_policy_req_trigger_list]");
            goto end;
        }

        am_policy_req_trigger_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(am_policy_req_trigger_list_local_nonprimitive, am_policy_req_trigger_list ) {
            if (!cJSON_IsString(am_policy_req_trigger_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [am_policy_req_trigger_list]");
                goto end;
            }

            OpenAPI_list_add(am_policy_req_trigger_listList, (void *)OpenAPI_policy_req_trigger_FromString(am_policy_req_trigger_list_local_nonprimitive->valuestring));
        }
    }

    cJSON *pcf_ue_policy_uri = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfUePolicyUri");

    if (pcf_ue_policy_uri) {
        if (!cJSON_IsString(pcf_ue_policy_uri)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_ue_policy_uri]");
            goto end;
        }
    }

    cJSON *ue_policy_req_trigger_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "uePolicyReqTriggerList");

    OpenAPI_list_t *ue_policy_req_trigger_listList;
    if (ue_policy_req_trigger_list) {
        cJSON *ue_policy_req_trigger_list_local_nonprimitive;
        if (!cJSON_IsArray(ue_policy_req_trigger_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ue_policy_req_trigger_list]");
            goto end;
        }

        ue_policy_req_trigger_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_policy_req_trigger_list_local_nonprimitive, ue_policy_req_trigger_list ) {
            if (!cJSON_IsString(ue_policy_req_trigger_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ue_policy_req_trigger_list]");
                goto end;
            }

            OpenAPI_list_add(ue_policy_req_trigger_listList, (void *)OpenAPI_policy_req_trigger_FromString(ue_policy_req_trigger_list_local_nonprimitive->valuestring));
        }
    }

    cJSON *hpcf_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "hpcfId");

    if (hpcf_id) {
        if (!cJSON_IsString(hpcf_id)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [hpcf_id]");
            goto end;
        }
    }

    cJSON *restricted_rat_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "restrictedRatList");

    OpenAPI_list_t *restricted_rat_listList;
    if (restricted_rat_list) {
        cJSON *restricted_rat_list_local_nonprimitive;
        if (!cJSON_IsArray(restricted_rat_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_rat_list]");
            goto end;
        }

        restricted_rat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_rat_list_local_nonprimitive, restricted_rat_list ) {
            if (!cJSON_IsString(restricted_rat_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_rat_list]");
                goto end;
            }

            OpenAPI_list_add(restricted_rat_listList, (void *)OpenAPI_rat_type_FromString(restricted_rat_list_local_nonprimitive->valuestring));
        }
    }

    cJSON *forbidden_area_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "forbiddenAreaList");

    OpenAPI_list_t *forbidden_area_listList;
    if (forbidden_area_list) {
        cJSON *forbidden_area_list_local_nonprimitive;
        if (!cJSON_IsArray(forbidden_area_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [forbidden_area_list]");
            goto end;
        }

        forbidden_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(forbidden_area_list_local_nonprimitive, forbidden_area_list ) {
            if (!cJSON_IsObject(forbidden_area_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [forbidden_area_list]");
                goto end;
            }
            OpenAPI_area_t *forbidden_area_listItem = OpenAPI_area_parseFromJSON(forbidden_area_list_local_nonprimitive);

            OpenAPI_list_add(forbidden_area_listList, forbidden_area_listItem);
        }
    }

    cJSON *service_area_restriction = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "serviceAreaRestriction");

    OpenAPI_service_area_restriction_t *service_area_restriction_local_nonprim = NULL;
    if (service_area_restriction) {
        service_area_restriction_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(service_area_restriction);
    }

    cJSON *restricted_core_nw_type_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "restrictedCoreNwTypeList");

    OpenAPI_list_t *restricted_core_nw_type_listList;
    if (restricted_core_nw_type_list) {
        cJSON *restricted_core_nw_type_list_local_nonprimitive;
        if (!cJSON_IsArray(restricted_core_nw_type_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_core_nw_type_list]");
            goto end;
        }

        restricted_core_nw_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_core_nw_type_list_local_nonprimitive, restricted_core_nw_type_list ) {
            if (!cJSON_IsString(restricted_core_nw_type_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_core_nw_type_list]");
                goto end;
            }

            OpenAPI_list_add(restricted_core_nw_type_listList, (void *)OpenAPI_core_network_type_FromString(restricted_core_nw_type_list_local_nonprimitive->valuestring));
        }
    }

    cJSON *event_subscription_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "eventSubscriptionList");

    OpenAPI_list_t *event_subscription_listList;
    if (event_subscription_list) {
        cJSON *event_subscription_list_local_nonprimitive;
        if (!cJSON_IsArray(event_subscription_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [event_subscription_list]");
            goto end;
        }

        event_subscription_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_subscription_list_local_nonprimitive, event_subscription_list ) {
            if (!cJSON_IsObject(event_subscription_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [event_subscription_list]");
                goto end;
            }
            OpenAPI_amf_event_subscription_t *event_subscription_listItem = OpenAPI_amf_event_subscription_parseFromJSON(event_subscription_list_local_nonprimitive);

            OpenAPI_list_add(event_subscription_listList, event_subscription_listItem);
        }
    }

    cJSON *mm_context_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "mmContextList");

    OpenAPI_list_t *mm_context_listList;
    if (mm_context_list) {
        cJSON *mm_context_list_local_nonprimitive;
        if (!cJSON_IsArray(mm_context_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [mm_context_list]");
            goto end;
        }

        mm_context_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(mm_context_list_local_nonprimitive, mm_context_list ) {
            if (!cJSON_IsObject(mm_context_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [mm_context_list]");
                goto end;
            }
            OpenAPI_mm_context_t *mm_context_listItem = OpenAPI_mm_context_parseFromJSON(mm_context_list_local_nonprimitive);

            OpenAPI_list_add(mm_context_listList, mm_context_listItem);
        }
    }

    cJSON *session_context_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "sessionContextList");

    OpenAPI_list_t *session_context_listList;
    if (session_context_list) {
        cJSON *session_context_list_local_nonprimitive;
        if (!cJSON_IsArray(session_context_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [session_context_list]");
            goto end;
        }

        session_context_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(session_context_list_local_nonprimitive, session_context_list ) {
            if (!cJSON_IsObject(session_context_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [session_context_list]");
                goto end;
            }
            OpenAPI_pdu_session_context_t *session_context_listItem = OpenAPI_pdu_session_context_parseFromJSON(session_context_list_local_nonprimitive);

            OpenAPI_list_add(session_context_listList, session_context_listItem);
        }
    }

    cJSON *trace_data = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "traceData");

    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    if (trace_data) {
        trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    }

    cJSON *service_gap_expiry_time = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "serviceGapExpiryTime");

    if (service_gap_expiry_time) {
        if (!cJSON_IsString(service_gap_expiry_time)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [service_gap_expiry_time]");
            goto end;
        }
    }

    cJSON *stn_sr = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "stnSr");

    if (stn_sr) {
        if (!cJSON_IsString(stn_sr)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [stn_sr]");
            goto end;
        }
    }

    cJSON *c_msisdn = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "cMsisdn");

    if (c_msisdn) {
        if (!cJSON_IsString(c_msisdn)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [c_msisdn]");
            goto end;
        }
    }

    cJSON *ms_classmark2 = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "msClassmark2");

    if (ms_classmark2) {
        if (!cJSON_IsNumber(ms_classmark2)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ms_classmark2]");
            goto end;
        }
    }

    cJSON *supported_codec_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "supportedCodecList");

    OpenAPI_list_t *supported_codec_listList;
    if (supported_codec_list) {
        cJSON *supported_codec_list_local;
        if (!cJSON_IsArray(supported_codec_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [supported_codec_list]");
            goto end;
        }
        supported_codec_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_codec_list_local, supported_codec_list) {
        }
    }

    cJSON *small_data_rate_status_infos = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smallDataRateStatusInfos");

    OpenAPI_list_t *small_data_rate_status_infosList;
    if (small_data_rate_status_infos) {
        cJSON *small_data_rate_status_infos_local_nonprimitive;
        if (!cJSON_IsArray(small_data_rate_status_infos)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [small_data_rate_status_infos]");
            goto end;
        }

        small_data_rate_status_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(small_data_rate_status_infos_local_nonprimitive, small_data_rate_status_infos ) {
            if (!cJSON_IsObject(small_data_rate_status_infos_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [small_data_rate_status_infos]");
                goto end;
            }
            OpenAPI_small_data_rate_status_info_t *small_data_rate_status_infosItem = OpenAPI_small_data_rate_status_info_parseFromJSON(small_data_rate_status_infos_local_nonprimitive);

            OpenAPI_list_add(small_data_rate_status_infosList, small_data_rate_status_infosItem);
        }
    }

    cJSON *restricted_primary_rat_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "restrictedPrimaryRatList");

    OpenAPI_list_t *restricted_primary_rat_listList;
    if (restricted_primary_rat_list) {
        cJSON *restricted_primary_rat_list_local_nonprimitive;
        if (!cJSON_IsArray(restricted_primary_rat_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_primary_rat_list]");
            goto end;
        }

        restricted_primary_rat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_primary_rat_list_local_nonprimitive, restricted_primary_rat_list ) {
            if (!cJSON_IsString(restricted_primary_rat_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_primary_rat_list]");
                goto end;
            }

            OpenAPI_list_add(restricted_primary_rat_listList, (void *)OpenAPI_rat_type_FromString(restricted_primary_rat_list_local_nonprimitive->valuestring));
        }
    }

    cJSON *restricted_secondary_rat_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "restrictedSecondaryRatList");

    OpenAPI_list_t *restricted_secondary_rat_listList;
    if (restricted_secondary_rat_list) {
        cJSON *restricted_secondary_rat_list_local_nonprimitive;
        if (!cJSON_IsArray(restricted_secondary_rat_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_secondary_rat_list]");
            goto end;
        }

        restricted_secondary_rat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_secondary_rat_list_local_nonprimitive, restricted_secondary_rat_list ) {
            if (!cJSON_IsString(restricted_secondary_rat_list_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_secondary_rat_list]");
                goto end;
            }

            OpenAPI_list_add(restricted_secondary_rat_listList, (void *)OpenAPI_rat_type_FromString(restricted_secondary_rat_list_local_nonprimitive->valuestring));
        }
    }

    cJSON *v2x_context = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "v2xContext");

    OpenAPI_list_t *v2x_contextList;
    if (v2x_context) {
        cJSON *v2x_context_local_nonprimitive;
        if (!cJSON_IsArray(v2x_context)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [v2x_context]");
            goto end;
        }

        v2x_contextList = OpenAPI_list_create();

        cJSON_ArrayForEach(v2x_context_local_nonprimitive, v2x_context ) {
            if (!cJSON_IsObject(v2x_context_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [v2x_context]");
                goto end;
            }
            OpenAPI_v2x_context_t *v2x_contextItem = OpenAPI_v2x_context_parseFromJSON(v2x_context_local_nonprimitive);

            OpenAPI_list_add(v2x_contextList, v2x_contextItem);
        }
    }

    cJSON *lte_cat_m_ind = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "lteCatMInd");

    if (lte_cat_m_ind) {
        if (!cJSON_IsBool(lte_cat_m_ind)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [lte_cat_m_ind]");
            goto end;
        }
    }

    cJSON *mo_exp_data_counter = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "moExpDataCounter");

    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_nonprim = NULL;
    if (mo_exp_data_counter) {
        mo_exp_data_counter_local_nonprim = OpenAPI_mo_exp_data_counter_parseFromJSON(mo_exp_data_counter);
    }

    ue_context_local_var = OpenAPI_ue_context_create (
        supi ? ogs_strdup(supi->valuestring) : NULL,
        supi_unauth_ind ? supi_unauth_ind->valueint : 0,
        gpsi_list ? gpsi_listList : NULL,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        udm_group_id ? ogs_strdup(udm_group_id->valuestring) : NULL,
        ausf_group_id ? ogs_strdup(ausf_group_id->valuestring) : NULL,
        routing_indicator ? ogs_strdup(routing_indicator->valuestring) : NULL,
        group_list ? group_listList : NULL,
        drx_parameter ? drx_parameter->valueint : 0,
        sub_rfsp ? sub_rfsp->valuedouble : 0,
        used_rfsp ? used_rfsp->valuedouble : 0,
        sub_ue_ambr ? sub_ue_ambr_local_nonprim : NULL,
        smsf_id ? ogs_strdup(smsf_id->valuestring) : NULL,
        seaf_data ? seaf_data_local_nonprim : NULL,
        _5g_mm_capability ? _5g_mm_capability->valueint : 0,
        pcf_id ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        pcf_amp_service_set_id ? ogs_strdup(pcf_amp_service_set_id->valuestring) : NULL,
        pcf_uep_service_set_id ? ogs_strdup(pcf_uep_service_set_id->valuestring) : NULL,
        pcf_binding ? pcf_bindingVariable : 0,
        pcf_am_policy_uri ? ogs_strdup(pcf_am_policy_uri->valuestring) : NULL,
        am_policy_req_trigger_list ? am_policy_req_trigger_listList : NULL,
        pcf_ue_policy_uri ? ogs_strdup(pcf_ue_policy_uri->valuestring) : NULL,
        ue_policy_req_trigger_list ? ue_policy_req_trigger_listList : NULL,
        hpcf_id ? ogs_strdup(hpcf_id->valuestring) : NULL,
        restricted_rat_list ? restricted_rat_listList : NULL,
        forbidden_area_list ? forbidden_area_listList : NULL,
        service_area_restriction ? service_area_restriction_local_nonprim : NULL,
        restricted_core_nw_type_list ? restricted_core_nw_type_listList : NULL,
        event_subscription_list ? event_subscription_listList : NULL,
        mm_context_list ? mm_context_listList : NULL,
        session_context_list ? session_context_listList : NULL,
        trace_data ? trace_data_local_nonprim : NULL,
        service_gap_expiry_time ? ogs_strdup(service_gap_expiry_time->valuestring) : NULL,
        stn_sr ? ogs_strdup(stn_sr->valuestring) : NULL,
        c_msisdn ? ogs_strdup(c_msisdn->valuestring) : NULL,
        ms_classmark2 ? ms_classmark2->valueint : 0,
        supported_codec_list ? supported_codec_listList : NULL,
        small_data_rate_status_infos ? small_data_rate_status_infosList : NULL,
        restricted_primary_rat_list ? restricted_primary_rat_listList : NULL,
        restricted_secondary_rat_list ? restricted_secondary_rat_listList : NULL,
        v2x_context ? v2x_contextList : NULL,
        lte_cat_m_ind ? lte_cat_m_ind->valueint : 0,
        mo_exp_data_counter ? mo_exp_data_counter_local_nonprim : NULL
        );

    return ue_context_local_var;
end:
    return NULL;
}

OpenAPI_ue_context_t *OpenAPI_ue_context_copy(OpenAPI_ue_context_t *dst, OpenAPI_ue_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed");
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

    OpenAPI_ue_context_free(dst);
    dst = OpenAPI_ue_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

