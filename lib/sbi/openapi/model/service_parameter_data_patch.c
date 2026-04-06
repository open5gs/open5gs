
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_parameter_data_patch.h"

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_create(
    bool is_param_over_pc5_null,
    char *param_over_pc5,
    bool is_param_over_uu_null,
    char *param_over_uu,
    bool is_a2x_params_pc5_null,
    char *a2x_params_pc5,
    bool is_a2x_params_uu_null,
    char *a2x_params_uu,
    bool is_param_for_pro_se_dd_null,
    char *param_for_pro_se_dd,
    bool is_param_for_pro_se_dc_null,
    char *param_for_pro_se_dc,
    bool is_param_for_pro_se_u2_n_rel_ue_null,
    char *param_for_pro_se_u2_n_rel_ue,
    bool is_param_for_pro_se_rem_ue_null,
    char *param_for_pro_se_rem_ue,
    bool is_param_for_pro_se_u2_u_rel_ue_null,
    char *param_for_pro_se_u2_u_rel_ue,
    bool is_param_for_pro_se_end_ue_null,
    char *param_for_pro_se_end_ue,
    bool is_multi_hop_u2_u_rel_ue_null,
    char *multi_hop_u2_u_rel_ue,
    bool is_multi_hop_u2_u_end_ue_null,
    char *multi_hop_u2_u_end_ue,
    bool is_multi_hop_u2_n_rel_ue_null,
    char *multi_hop_u2_n_rel_ue,
    bool is_multi_hop_u2_n_rem_ue_null,
    char *multi_hop_u2_n_rem_ue,
    bool is_multi_hop_u2_n_interm_ue_null,
    char *multi_hop_u2_n_interm_ue,
    OpenAPI_list_t *ursp_influence,
    bool is_ursp_guidance_null,
    OpenAPI_list_t *ursp_guidance,
    bool is_vps_ursp_guidance_null,
    OpenAPI_list_t *vps_ursp_guidance,
    bool is_tnaps_null,
    OpenAPI_list_t *tnaps,
    bool is_delivery_events_null,
    OpenAPI_list_t *delivery_events,
    char *polic_deliv_notif_uri,
    OpenAPI_list_t *headers,
    bool is_param_for_ranging_sl_pos_null,
    char *param_for_ranging_sl_pos
)
{
    OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_service_parameter_data_patch_t));
    ogs_assert(service_parameter_data_patch_local_var);

    service_parameter_data_patch_local_var->is_param_over_pc5_null = is_param_over_pc5_null;
    service_parameter_data_patch_local_var->param_over_pc5 = param_over_pc5;
    service_parameter_data_patch_local_var->is_param_over_uu_null = is_param_over_uu_null;
    service_parameter_data_patch_local_var->param_over_uu = param_over_uu;
    service_parameter_data_patch_local_var->is_a2x_params_pc5_null = is_a2x_params_pc5_null;
    service_parameter_data_patch_local_var->a2x_params_pc5 = a2x_params_pc5;
    service_parameter_data_patch_local_var->is_a2x_params_uu_null = is_a2x_params_uu_null;
    service_parameter_data_patch_local_var->a2x_params_uu = a2x_params_uu;
    service_parameter_data_patch_local_var->is_param_for_pro_se_dd_null = is_param_for_pro_se_dd_null;
    service_parameter_data_patch_local_var->param_for_pro_se_dd = param_for_pro_se_dd;
    service_parameter_data_patch_local_var->is_param_for_pro_se_dc_null = is_param_for_pro_se_dc_null;
    service_parameter_data_patch_local_var->param_for_pro_se_dc = param_for_pro_se_dc;
    service_parameter_data_patch_local_var->is_param_for_pro_se_u2_n_rel_ue_null = is_param_for_pro_se_u2_n_rel_ue_null;
    service_parameter_data_patch_local_var->param_for_pro_se_u2_n_rel_ue = param_for_pro_se_u2_n_rel_ue;
    service_parameter_data_patch_local_var->is_param_for_pro_se_rem_ue_null = is_param_for_pro_se_rem_ue_null;
    service_parameter_data_patch_local_var->param_for_pro_se_rem_ue = param_for_pro_se_rem_ue;
    service_parameter_data_patch_local_var->is_param_for_pro_se_u2_u_rel_ue_null = is_param_for_pro_se_u2_u_rel_ue_null;
    service_parameter_data_patch_local_var->param_for_pro_se_u2_u_rel_ue = param_for_pro_se_u2_u_rel_ue;
    service_parameter_data_patch_local_var->is_param_for_pro_se_end_ue_null = is_param_for_pro_se_end_ue_null;
    service_parameter_data_patch_local_var->param_for_pro_se_end_ue = param_for_pro_se_end_ue;
    service_parameter_data_patch_local_var->is_multi_hop_u2_u_rel_ue_null = is_multi_hop_u2_u_rel_ue_null;
    service_parameter_data_patch_local_var->multi_hop_u2_u_rel_ue = multi_hop_u2_u_rel_ue;
    service_parameter_data_patch_local_var->is_multi_hop_u2_u_end_ue_null = is_multi_hop_u2_u_end_ue_null;
    service_parameter_data_patch_local_var->multi_hop_u2_u_end_ue = multi_hop_u2_u_end_ue;
    service_parameter_data_patch_local_var->is_multi_hop_u2_n_rel_ue_null = is_multi_hop_u2_n_rel_ue_null;
    service_parameter_data_patch_local_var->multi_hop_u2_n_rel_ue = multi_hop_u2_n_rel_ue;
    service_parameter_data_patch_local_var->is_multi_hop_u2_n_rem_ue_null = is_multi_hop_u2_n_rem_ue_null;
    service_parameter_data_patch_local_var->multi_hop_u2_n_rem_ue = multi_hop_u2_n_rem_ue;
    service_parameter_data_patch_local_var->is_multi_hop_u2_n_interm_ue_null = is_multi_hop_u2_n_interm_ue_null;
    service_parameter_data_patch_local_var->multi_hop_u2_n_interm_ue = multi_hop_u2_n_interm_ue;
    service_parameter_data_patch_local_var->ursp_influence = ursp_influence;
    service_parameter_data_patch_local_var->is_ursp_guidance_null = is_ursp_guidance_null;
    service_parameter_data_patch_local_var->ursp_guidance = ursp_guidance;
    service_parameter_data_patch_local_var->is_vps_ursp_guidance_null = is_vps_ursp_guidance_null;
    service_parameter_data_patch_local_var->vps_ursp_guidance = vps_ursp_guidance;
    service_parameter_data_patch_local_var->is_tnaps_null = is_tnaps_null;
    service_parameter_data_patch_local_var->tnaps = tnaps;
    service_parameter_data_patch_local_var->is_delivery_events_null = is_delivery_events_null;
    service_parameter_data_patch_local_var->delivery_events = delivery_events;
    service_parameter_data_patch_local_var->polic_deliv_notif_uri = polic_deliv_notif_uri;
    service_parameter_data_patch_local_var->headers = headers;
    service_parameter_data_patch_local_var->is_param_for_ranging_sl_pos_null = is_param_for_ranging_sl_pos_null;
    service_parameter_data_patch_local_var->param_for_ranging_sl_pos = param_for_ranging_sl_pos;

    return service_parameter_data_patch_local_var;
}

void OpenAPI_service_parameter_data_patch_free(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_parameter_data_patch) {
        return;
    }
    if (service_parameter_data_patch->param_over_pc5) {
        ogs_free(service_parameter_data_patch->param_over_pc5);
        service_parameter_data_patch->param_over_pc5 = NULL;
    }
    if (service_parameter_data_patch->param_over_uu) {
        ogs_free(service_parameter_data_patch->param_over_uu);
        service_parameter_data_patch->param_over_uu = NULL;
    }
    if (service_parameter_data_patch->a2x_params_pc5) {
        ogs_free(service_parameter_data_patch->a2x_params_pc5);
        service_parameter_data_patch->a2x_params_pc5 = NULL;
    }
    if (service_parameter_data_patch->a2x_params_uu) {
        ogs_free(service_parameter_data_patch->a2x_params_uu);
        service_parameter_data_patch->a2x_params_uu = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_dd) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_dd);
        service_parameter_data_patch->param_for_pro_se_dd = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_dc) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_dc);
        service_parameter_data_patch->param_for_pro_se_dc = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue);
        service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_rem_ue) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_rem_ue);
        service_parameter_data_patch->param_for_pro_se_rem_ue = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_u2_u_rel_ue) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_u2_u_rel_ue);
        service_parameter_data_patch->param_for_pro_se_u2_u_rel_ue = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_end_ue) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_end_ue);
        service_parameter_data_patch->param_for_pro_se_end_ue = NULL;
    }
    if (service_parameter_data_patch->multi_hop_u2_u_rel_ue) {
        ogs_free(service_parameter_data_patch->multi_hop_u2_u_rel_ue);
        service_parameter_data_patch->multi_hop_u2_u_rel_ue = NULL;
    }
    if (service_parameter_data_patch->multi_hop_u2_u_end_ue) {
        ogs_free(service_parameter_data_patch->multi_hop_u2_u_end_ue);
        service_parameter_data_patch->multi_hop_u2_u_end_ue = NULL;
    }
    if (service_parameter_data_patch->multi_hop_u2_n_rel_ue) {
        ogs_free(service_parameter_data_patch->multi_hop_u2_n_rel_ue);
        service_parameter_data_patch->multi_hop_u2_n_rel_ue = NULL;
    }
    if (service_parameter_data_patch->multi_hop_u2_n_rem_ue) {
        ogs_free(service_parameter_data_patch->multi_hop_u2_n_rem_ue);
        service_parameter_data_patch->multi_hop_u2_n_rem_ue = NULL;
    }
    if (service_parameter_data_patch->multi_hop_u2_n_interm_ue) {
        ogs_free(service_parameter_data_patch->multi_hop_u2_n_interm_ue);
        service_parameter_data_patch->multi_hop_u2_n_interm_ue = NULL;
    }
    if (service_parameter_data_patch->ursp_influence) {
        OpenAPI_list_for_each(service_parameter_data_patch->ursp_influence, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data_patch->ursp_influence);
        service_parameter_data_patch->ursp_influence = NULL;
    }
    if (service_parameter_data_patch->ursp_guidance) {
        OpenAPI_list_for_each(service_parameter_data_patch->ursp_guidance, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data_patch->ursp_guidance);
        service_parameter_data_patch->ursp_guidance = NULL;
    }
    if (service_parameter_data_patch->vps_ursp_guidance) {
        OpenAPI_list_for_each(service_parameter_data_patch->vps_ursp_guidance, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data_patch->vps_ursp_guidance);
        service_parameter_data_patch->vps_ursp_guidance = NULL;
    }
    if (service_parameter_data_patch->tnaps) {
        OpenAPI_list_for_each(service_parameter_data_patch->tnaps, node) {
            OpenAPI_tnap_id_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data_patch->tnaps);
        service_parameter_data_patch->tnaps = NULL;
    }
    if (service_parameter_data_patch->delivery_events) {
        OpenAPI_list_free(service_parameter_data_patch->delivery_events);
        service_parameter_data_patch->delivery_events = NULL;
    }
    if (service_parameter_data_patch->polic_deliv_notif_uri) {
        ogs_free(service_parameter_data_patch->polic_deliv_notif_uri);
        service_parameter_data_patch->polic_deliv_notif_uri = NULL;
    }
    if (service_parameter_data_patch->headers) {
        OpenAPI_list_for_each(service_parameter_data_patch->headers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data_patch->headers);
        service_parameter_data_patch->headers = NULL;
    }
    if (service_parameter_data_patch->param_for_ranging_sl_pos) {
        ogs_free(service_parameter_data_patch->param_for_ranging_sl_pos);
        service_parameter_data_patch->param_for_ranging_sl_pos = NULL;
    }
    ogs_free(service_parameter_data_patch);
}

cJSON *OpenAPI_service_parameter_data_patch_convertToJSON(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_parameter_data_patch == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ServiceParameterDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_parameter_data_patch->param_over_pc5) {
    if (cJSON_AddStringToObject(item, "paramOverPc5", service_parameter_data_patch->param_over_pc5) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_over_pc5]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_over_pc5_null) {
        if (cJSON_AddNullToObject(item, "paramOverPc5") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_over_pc5]");
            goto end;
        }
    }

    if (service_parameter_data_patch->param_over_uu) {
    if (cJSON_AddStringToObject(item, "paramOverUu", service_parameter_data_patch->param_over_uu) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_over_uu]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_over_uu_null) {
        if (cJSON_AddNullToObject(item, "paramOverUu") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_over_uu]");
            goto end;
        }
    }

    if (service_parameter_data_patch->a2x_params_pc5) {
    if (cJSON_AddStringToObject(item, "a2xParamsPc5", service_parameter_data_patch->a2x_params_pc5) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [a2x_params_pc5]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_a2x_params_pc5_null) {
        if (cJSON_AddNullToObject(item, "a2xParamsPc5") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [a2x_params_pc5]");
            goto end;
        }
    }

    if (service_parameter_data_patch->a2x_params_uu) {
    if (cJSON_AddStringToObject(item, "a2xParamsUu", service_parameter_data_patch->a2x_params_uu) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [a2x_params_uu]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_a2x_params_uu_null) {
        if (cJSON_AddNullToObject(item, "a2xParamsUu") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [a2x_params_uu]");
            goto end;
        }
    }

    if (service_parameter_data_patch->param_for_pro_se_dd) {
    if (cJSON_AddStringToObject(item, "paramForProSeDd", service_parameter_data_patch->param_for_pro_se_dd) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_dd]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_for_pro_se_dd_null) {
        if (cJSON_AddNullToObject(item, "paramForProSeDd") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_dd]");
            goto end;
        }
    }

    if (service_parameter_data_patch->param_for_pro_se_dc) {
    if (cJSON_AddStringToObject(item, "paramForProSeDc", service_parameter_data_patch->param_for_pro_se_dc) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_dc]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_for_pro_se_dc_null) {
        if (cJSON_AddNullToObject(item, "paramForProSeDc") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_dc]");
            goto end;
        }
    }

    if (service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue) {
    if (cJSON_AddStringToObject(item, "paramForProSeU2NRelUe", service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_u2_n_rel_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_for_pro_se_u2_n_rel_ue_null) {
        if (cJSON_AddNullToObject(item, "paramForProSeU2NRelUe") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_u2_n_rel_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->param_for_pro_se_rem_ue) {
    if (cJSON_AddStringToObject(item, "paramForProSeRemUe", service_parameter_data_patch->param_for_pro_se_rem_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_rem_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_for_pro_se_rem_ue_null) {
        if (cJSON_AddNullToObject(item, "paramForProSeRemUe") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_rem_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->param_for_pro_se_u2_u_rel_ue) {
    if (cJSON_AddStringToObject(item, "paramForProSeU2URelUE", service_parameter_data_patch->param_for_pro_se_u2_u_rel_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_u2_u_rel_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_for_pro_se_u2_u_rel_ue_null) {
        if (cJSON_AddNullToObject(item, "paramForProSeU2URelUE") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_u2_u_rel_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->param_for_pro_se_end_ue) {
    if (cJSON_AddStringToObject(item, "paramForProSeEndUe", service_parameter_data_patch->param_for_pro_se_end_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_end_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_for_pro_se_end_ue_null) {
        if (cJSON_AddNullToObject(item, "paramForProSeEndUe") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_end_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->multi_hop_u2_u_rel_ue) {
    if (cJSON_AddStringToObject(item, "multiHopU2URelUe", service_parameter_data_patch->multi_hop_u2_u_rel_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_u_rel_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_multi_hop_u2_u_rel_ue_null) {
        if (cJSON_AddNullToObject(item, "multiHopU2URelUe") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_u_rel_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->multi_hop_u2_u_end_ue) {
    if (cJSON_AddStringToObject(item, "multiHopU2UEndUe", service_parameter_data_patch->multi_hop_u2_u_end_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_u_end_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_multi_hop_u2_u_end_ue_null) {
        if (cJSON_AddNullToObject(item, "multiHopU2UEndUe") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_u_end_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->multi_hop_u2_n_rel_ue) {
    if (cJSON_AddStringToObject(item, "multiHopU2NRelUe", service_parameter_data_patch->multi_hop_u2_n_rel_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_n_rel_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_multi_hop_u2_n_rel_ue_null) {
        if (cJSON_AddNullToObject(item, "multiHopU2NRelUe") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_n_rel_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->multi_hop_u2_n_rem_ue) {
    if (cJSON_AddStringToObject(item, "multiHopU2NRemUe", service_parameter_data_patch->multi_hop_u2_n_rem_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_n_rem_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_multi_hop_u2_n_rem_ue_null) {
        if (cJSON_AddNullToObject(item, "multiHopU2NRemUe") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_n_rem_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->multi_hop_u2_n_interm_ue) {
    if (cJSON_AddStringToObject(item, "multiHopU2NIntermUe", service_parameter_data_patch->multi_hop_u2_n_interm_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_n_interm_ue]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_multi_hop_u2_n_interm_ue_null) {
        if (cJSON_AddNullToObject(item, "multiHopU2NIntermUe") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [multi_hop_u2_n_interm_ue]");
            goto end;
        }
    }

    if (service_parameter_data_patch->ursp_influence) {
    cJSON *ursp_influenceList = cJSON_AddArrayToObject(item, "urspInfluence");
    if (ursp_influenceList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ursp_influence]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data_patch->ursp_influence, node) {
        cJSON *itemLocal = OpenAPI_ursp_rule_request_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ursp_influence]");
            goto end;
        }
        cJSON_AddItemToArray(ursp_influenceList, itemLocal);
    }
    }

    if (service_parameter_data_patch->ursp_guidance) {
    cJSON *ursp_guidanceList = cJSON_AddArrayToObject(item, "urspGuidance");
    if (ursp_guidanceList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ursp_guidance]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data_patch->ursp_guidance, node) {
        cJSON *itemLocal = OpenAPI_ursp_rule_request_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ursp_guidance]");
            goto end;
        }
        cJSON_AddItemToArray(ursp_guidanceList, itemLocal);
    }
    } else if (service_parameter_data_patch->is_ursp_guidance_null) {
        if (cJSON_AddNullToObject(item, "urspGuidance") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ursp_guidance]");
            goto end;
        }
    }

    if (service_parameter_data_patch->vps_ursp_guidance) {
    cJSON *vps_ursp_guidanceList = cJSON_AddArrayToObject(item, "vpsUrspGuidance");
    if (vps_ursp_guidanceList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [vps_ursp_guidance]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data_patch->vps_ursp_guidance, node) {
        cJSON *itemLocal = OpenAPI_ursp_rule_request_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [vps_ursp_guidance]");
            goto end;
        }
        cJSON_AddItemToArray(vps_ursp_guidanceList, itemLocal);
    }
    } else if (service_parameter_data_patch->is_vps_ursp_guidance_null) {
        if (cJSON_AddNullToObject(item, "vpsUrspGuidance") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [vps_ursp_guidance]");
            goto end;
        }
    }

    if (service_parameter_data_patch->tnaps) {
    cJSON *tnapsList = cJSON_AddArrayToObject(item, "tnaps");
    if (tnapsList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [tnaps]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data_patch->tnaps, node) {
        cJSON *itemLocal = OpenAPI_tnap_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [tnaps]");
            goto end;
        }
        cJSON_AddItemToArray(tnapsList, itemLocal);
    }
    } else if (service_parameter_data_patch->is_tnaps_null) {
        if (cJSON_AddNullToObject(item, "tnaps") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [tnaps]");
            goto end;
        }
    }

    if (service_parameter_data_patch->delivery_events != OpenAPI_event_NULL) {
    cJSON *delivery_eventsList = cJSON_AddArrayToObject(item, "deliveryEvents");
    if (delivery_eventsList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [delivery_events]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data_patch->delivery_events, node) {
        if (cJSON_AddStringToObject(delivery_eventsList, "", OpenAPI_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [delivery_events]");
            goto end;
        }
    }
    } else if (service_parameter_data_patch->is_delivery_events_null) {
        if (cJSON_AddNullToObject(item, "deliveryEvents") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [delivery_events]");
            goto end;
        }
    }

    if (service_parameter_data_patch->polic_deliv_notif_uri) {
    if (cJSON_AddStringToObject(item, "policDelivNotifUri", service_parameter_data_patch->polic_deliv_notif_uri) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [polic_deliv_notif_uri]");
        goto end;
    }
    }

    if (service_parameter_data_patch->headers) {
    cJSON *headersList = cJSON_AddArrayToObject(item, "headers");
    if (headersList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [headers]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data_patch->headers, node) {
        if (cJSON_AddStringToObject(headersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [headers]");
            goto end;
        }
    }
    }

    if (service_parameter_data_patch->param_for_ranging_sl_pos) {
    if (cJSON_AddStringToObject(item, "paramForRangingSlPos", service_parameter_data_patch->param_for_ranging_sl_pos) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_ranging_sl_pos]");
        goto end;
    }
    } else if (service_parameter_data_patch->is_param_for_ranging_sl_pos_null) {
        if (cJSON_AddNullToObject(item, "paramForRangingSlPos") == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_ranging_sl_pos]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_parseFromJSON(cJSON *service_parameter_data_patchJSON)
{
    OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *param_over_pc5 = NULL;
    cJSON *param_over_uu = NULL;
    cJSON *a2x_params_pc5 = NULL;
    cJSON *a2x_params_uu = NULL;
    cJSON *param_for_pro_se_dd = NULL;
    cJSON *param_for_pro_se_dc = NULL;
    cJSON *param_for_pro_se_u2_n_rel_ue = NULL;
    cJSON *param_for_pro_se_rem_ue = NULL;
    cJSON *param_for_pro_se_u2_u_rel_ue = NULL;
    cJSON *param_for_pro_se_end_ue = NULL;
    cJSON *multi_hop_u2_u_rel_ue = NULL;
    cJSON *multi_hop_u2_u_end_ue = NULL;
    cJSON *multi_hop_u2_n_rel_ue = NULL;
    cJSON *multi_hop_u2_n_rem_ue = NULL;
    cJSON *multi_hop_u2_n_interm_ue = NULL;
    cJSON *ursp_influence = NULL;
    OpenAPI_list_t *ursp_influenceList = NULL;
    cJSON *ursp_guidance = NULL;
    OpenAPI_list_t *ursp_guidanceList = NULL;
    cJSON *vps_ursp_guidance = NULL;
    OpenAPI_list_t *vps_ursp_guidanceList = NULL;
    cJSON *tnaps = NULL;
    OpenAPI_list_t *tnapsList = NULL;
    cJSON *delivery_events = NULL;
    OpenAPI_list_t *delivery_eventsList = NULL;
    cJSON *polic_deliv_notif_uri = NULL;
    cJSON *headers = NULL;
    OpenAPI_list_t *headersList = NULL;
    cJSON *param_for_ranging_sl_pos = NULL;
    param_over_pc5 = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramOverPc5");
    if (param_over_pc5) {
    if (!cJSON_IsNull(param_over_pc5)) {
    if (!cJSON_IsString(param_over_pc5) && !cJSON_IsNull(param_over_pc5)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_over_pc5]");
        goto end;
    }
    }
    }

    param_over_uu = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramOverUu");
    if (param_over_uu) {
    if (!cJSON_IsNull(param_over_uu)) {
    if (!cJSON_IsString(param_over_uu) && !cJSON_IsNull(param_over_uu)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_over_uu]");
        goto end;
    }
    }
    }

    a2x_params_pc5 = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "a2xParamsPc5");
    if (a2x_params_pc5) {
    if (!cJSON_IsNull(a2x_params_pc5)) {
    if (!cJSON_IsString(a2x_params_pc5) && !cJSON_IsNull(a2x_params_pc5)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [a2x_params_pc5]");
        goto end;
    }
    }
    }

    a2x_params_uu = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "a2xParamsUu");
    if (a2x_params_uu) {
    if (!cJSON_IsNull(a2x_params_uu)) {
    if (!cJSON_IsString(a2x_params_uu) && !cJSON_IsNull(a2x_params_uu)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [a2x_params_uu]");
        goto end;
    }
    }
    }

    param_for_pro_se_dd = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeDd");
    if (param_for_pro_se_dd) {
    if (!cJSON_IsNull(param_for_pro_se_dd)) {
    if (!cJSON_IsString(param_for_pro_se_dd) && !cJSON_IsNull(param_for_pro_se_dd)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_dd]");
        goto end;
    }
    }
    }

    param_for_pro_se_dc = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeDc");
    if (param_for_pro_se_dc) {
    if (!cJSON_IsNull(param_for_pro_se_dc)) {
    if (!cJSON_IsString(param_for_pro_se_dc) && !cJSON_IsNull(param_for_pro_se_dc)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_dc]");
        goto end;
    }
    }
    }

    param_for_pro_se_u2_n_rel_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeU2NRelUe");
    if (param_for_pro_se_u2_n_rel_ue) {
    if (!cJSON_IsNull(param_for_pro_se_u2_n_rel_ue)) {
    if (!cJSON_IsString(param_for_pro_se_u2_n_rel_ue) && !cJSON_IsNull(param_for_pro_se_u2_n_rel_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_u2_n_rel_ue]");
        goto end;
    }
    }
    }

    param_for_pro_se_rem_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeRemUe");
    if (param_for_pro_se_rem_ue) {
    if (!cJSON_IsNull(param_for_pro_se_rem_ue)) {
    if (!cJSON_IsString(param_for_pro_se_rem_ue) && !cJSON_IsNull(param_for_pro_se_rem_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_rem_ue]");
        goto end;
    }
    }
    }

    param_for_pro_se_u2_u_rel_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeU2URelUE");
    if (param_for_pro_se_u2_u_rel_ue) {
    if (!cJSON_IsNull(param_for_pro_se_u2_u_rel_ue)) {
    if (!cJSON_IsString(param_for_pro_se_u2_u_rel_ue) && !cJSON_IsNull(param_for_pro_se_u2_u_rel_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_u2_u_rel_ue]");
        goto end;
    }
    }
    }

    param_for_pro_se_end_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeEndUe");
    if (param_for_pro_se_end_ue) {
    if (!cJSON_IsNull(param_for_pro_se_end_ue)) {
    if (!cJSON_IsString(param_for_pro_se_end_ue) && !cJSON_IsNull(param_for_pro_se_end_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_end_ue]");
        goto end;
    }
    }
    }

    multi_hop_u2_u_rel_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "multiHopU2URelUe");
    if (multi_hop_u2_u_rel_ue) {
    if (!cJSON_IsNull(multi_hop_u2_u_rel_ue)) {
    if (!cJSON_IsString(multi_hop_u2_u_rel_ue) && !cJSON_IsNull(multi_hop_u2_u_rel_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [multi_hop_u2_u_rel_ue]");
        goto end;
    }
    }
    }

    multi_hop_u2_u_end_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "multiHopU2UEndUe");
    if (multi_hop_u2_u_end_ue) {
    if (!cJSON_IsNull(multi_hop_u2_u_end_ue)) {
    if (!cJSON_IsString(multi_hop_u2_u_end_ue) && !cJSON_IsNull(multi_hop_u2_u_end_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [multi_hop_u2_u_end_ue]");
        goto end;
    }
    }
    }

    multi_hop_u2_n_rel_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "multiHopU2NRelUe");
    if (multi_hop_u2_n_rel_ue) {
    if (!cJSON_IsNull(multi_hop_u2_n_rel_ue)) {
    if (!cJSON_IsString(multi_hop_u2_n_rel_ue) && !cJSON_IsNull(multi_hop_u2_n_rel_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [multi_hop_u2_n_rel_ue]");
        goto end;
    }
    }
    }

    multi_hop_u2_n_rem_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "multiHopU2NRemUe");
    if (multi_hop_u2_n_rem_ue) {
    if (!cJSON_IsNull(multi_hop_u2_n_rem_ue)) {
    if (!cJSON_IsString(multi_hop_u2_n_rem_ue) && !cJSON_IsNull(multi_hop_u2_n_rem_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [multi_hop_u2_n_rem_ue]");
        goto end;
    }
    }
    }

    multi_hop_u2_n_interm_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "multiHopU2NIntermUe");
    if (multi_hop_u2_n_interm_ue) {
    if (!cJSON_IsNull(multi_hop_u2_n_interm_ue)) {
    if (!cJSON_IsString(multi_hop_u2_n_interm_ue) && !cJSON_IsNull(multi_hop_u2_n_interm_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [multi_hop_u2_n_interm_ue]");
        goto end;
    }
    }
    }

    ursp_influence = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "urspInfluence");
    if (ursp_influence) {
        cJSON *ursp_influence_local = NULL;
        if (!cJSON_IsArray(ursp_influence)) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [ursp_influence]");
            goto end;
        }

        ursp_influenceList = OpenAPI_list_create();

        cJSON_ArrayForEach(ursp_influence_local, ursp_influence) {
            if (!cJSON_IsObject(ursp_influence_local)) {
                ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [ursp_influence]");
                goto end;
            }
            OpenAPI_ursp_rule_request_t *ursp_influenceItem = OpenAPI_ursp_rule_request_parseFromJSON(ursp_influence_local);
            if (!ursp_influenceItem) {
                ogs_error("No ursp_influenceItem");
                goto end;
            }
            OpenAPI_list_add(ursp_influenceList, ursp_influenceItem);
        }
    }

    ursp_guidance = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "urspGuidance");
    if (ursp_guidance) {
    if (!cJSON_IsNull(ursp_guidance)) {
        cJSON *ursp_guidance_local = NULL;
        if (!cJSON_IsArray(ursp_guidance)) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [ursp_guidance]");
            goto end;
        }

        ursp_guidanceList = OpenAPI_list_create();

        cJSON_ArrayForEach(ursp_guidance_local, ursp_guidance) {
            if (!cJSON_IsObject(ursp_guidance_local)) {
                ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [ursp_guidance]");
                goto end;
            }
            OpenAPI_ursp_rule_request_t *ursp_guidanceItem = OpenAPI_ursp_rule_request_parseFromJSON(ursp_guidance_local);
            if (!ursp_guidanceItem) {
                ogs_error("No ursp_guidanceItem");
                goto end;
            }
            OpenAPI_list_add(ursp_guidanceList, ursp_guidanceItem);
        }
    }
    }

    vps_ursp_guidance = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "vpsUrspGuidance");
    if (vps_ursp_guidance) {
    if (!cJSON_IsNull(vps_ursp_guidance)) {
        cJSON *vps_ursp_guidance_local = NULL;
        if (!cJSON_IsArray(vps_ursp_guidance)) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [vps_ursp_guidance]");
            goto end;
        }

        vps_ursp_guidanceList = OpenAPI_list_create();

        cJSON_ArrayForEach(vps_ursp_guidance_local, vps_ursp_guidance) {
            if (!cJSON_IsObject(vps_ursp_guidance_local)) {
                ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [vps_ursp_guidance]");
                goto end;
            }
            OpenAPI_ursp_rule_request_t *vps_ursp_guidanceItem = OpenAPI_ursp_rule_request_parseFromJSON(vps_ursp_guidance_local);
            if (!vps_ursp_guidanceItem) {
                ogs_error("No vps_ursp_guidanceItem");
                goto end;
            }
            OpenAPI_list_add(vps_ursp_guidanceList, vps_ursp_guidanceItem);
        }
    }
    }

    tnaps = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "tnaps");
    if (tnaps) {
    if (!cJSON_IsNull(tnaps)) {
        cJSON *tnaps_local = NULL;
        if (!cJSON_IsArray(tnaps)) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [tnaps]");
            goto end;
        }

        tnapsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tnaps_local, tnaps) {
            if (!cJSON_IsObject(tnaps_local)) {
                ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [tnaps]");
                goto end;
            }
            OpenAPI_tnap_id_t *tnapsItem = OpenAPI_tnap_id_parseFromJSON(tnaps_local);
            if (!tnapsItem) {
                ogs_error("No tnapsItem");
                goto end;
            }
            OpenAPI_list_add(tnapsList, tnapsItem);
        }
    }
    }

    delivery_events = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "deliveryEvents");
    if (delivery_events) {
    if (!cJSON_IsNull(delivery_events)) {
        cJSON *delivery_events_local = NULL;
        if (!cJSON_IsArray(delivery_events)) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [delivery_events]");
            goto end;
        }

        delivery_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(delivery_events_local, delivery_events) {
            OpenAPI_event_e localEnum = OpenAPI_event_NULL;
            if (!cJSON_IsString(delivery_events_local)) {
                ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [delivery_events]");
                goto end;
            }
            localEnum = OpenAPI_event_FromString(delivery_events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"delivery_events\" is not supported. Ignoring it ...",
                         delivery_events_local->valuestring);
            } else {
                OpenAPI_list_add(delivery_eventsList, (void *)localEnum);
            }
        }
        if (delivery_eventsList->count == 0) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed: Expected delivery_eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }
    }

    polic_deliv_notif_uri = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "policDelivNotifUri");
    if (polic_deliv_notif_uri) {
    if (!cJSON_IsString(polic_deliv_notif_uri) && !cJSON_IsNull(polic_deliv_notif_uri)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [polic_deliv_notif_uri]");
        goto end;
    }
    }

    headers = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "headers");
    if (headers) {
        cJSON *headers_local = NULL;
        if (!cJSON_IsArray(headers)) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [headers]");
            goto end;
        }

        headersList = OpenAPI_list_create();

        cJSON_ArrayForEach(headers_local, headers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(headers_local)) {
                ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [headers]");
                goto end;
            }
            OpenAPI_list_add(headersList, ogs_strdup(headers_local->valuestring));
        }
    }

    param_for_ranging_sl_pos = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForRangingSlPos");
    if (param_for_ranging_sl_pos) {
    if (!cJSON_IsNull(param_for_ranging_sl_pos)) {
    if (!cJSON_IsString(param_for_ranging_sl_pos) && !cJSON_IsNull(param_for_ranging_sl_pos)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_ranging_sl_pos]");
        goto end;
    }
    }
    }

    service_parameter_data_patch_local_var = OpenAPI_service_parameter_data_patch_create (
        param_over_pc5 && cJSON_IsNull(param_over_pc5) ? true : false,
        param_over_pc5 && !cJSON_IsNull(param_over_pc5) ? ogs_strdup(param_over_pc5->valuestring) : NULL,
        param_over_uu && cJSON_IsNull(param_over_uu) ? true : false,
        param_over_uu && !cJSON_IsNull(param_over_uu) ? ogs_strdup(param_over_uu->valuestring) : NULL,
        a2x_params_pc5 && cJSON_IsNull(a2x_params_pc5) ? true : false,
        a2x_params_pc5 && !cJSON_IsNull(a2x_params_pc5) ? ogs_strdup(a2x_params_pc5->valuestring) : NULL,
        a2x_params_uu && cJSON_IsNull(a2x_params_uu) ? true : false,
        a2x_params_uu && !cJSON_IsNull(a2x_params_uu) ? ogs_strdup(a2x_params_uu->valuestring) : NULL,
        param_for_pro_se_dd && cJSON_IsNull(param_for_pro_se_dd) ? true : false,
        param_for_pro_se_dd && !cJSON_IsNull(param_for_pro_se_dd) ? ogs_strdup(param_for_pro_se_dd->valuestring) : NULL,
        param_for_pro_se_dc && cJSON_IsNull(param_for_pro_se_dc) ? true : false,
        param_for_pro_se_dc && !cJSON_IsNull(param_for_pro_se_dc) ? ogs_strdup(param_for_pro_se_dc->valuestring) : NULL,
        param_for_pro_se_u2_n_rel_ue && cJSON_IsNull(param_for_pro_se_u2_n_rel_ue) ? true : false,
        param_for_pro_se_u2_n_rel_ue && !cJSON_IsNull(param_for_pro_se_u2_n_rel_ue) ? ogs_strdup(param_for_pro_se_u2_n_rel_ue->valuestring) : NULL,
        param_for_pro_se_rem_ue && cJSON_IsNull(param_for_pro_se_rem_ue) ? true : false,
        param_for_pro_se_rem_ue && !cJSON_IsNull(param_for_pro_se_rem_ue) ? ogs_strdup(param_for_pro_se_rem_ue->valuestring) : NULL,
        param_for_pro_se_u2_u_rel_ue && cJSON_IsNull(param_for_pro_se_u2_u_rel_ue) ? true : false,
        param_for_pro_se_u2_u_rel_ue && !cJSON_IsNull(param_for_pro_se_u2_u_rel_ue) ? ogs_strdup(param_for_pro_se_u2_u_rel_ue->valuestring) : NULL,
        param_for_pro_se_end_ue && cJSON_IsNull(param_for_pro_se_end_ue) ? true : false,
        param_for_pro_se_end_ue && !cJSON_IsNull(param_for_pro_se_end_ue) ? ogs_strdup(param_for_pro_se_end_ue->valuestring) : NULL,
        multi_hop_u2_u_rel_ue && cJSON_IsNull(multi_hop_u2_u_rel_ue) ? true : false,
        multi_hop_u2_u_rel_ue && !cJSON_IsNull(multi_hop_u2_u_rel_ue) ? ogs_strdup(multi_hop_u2_u_rel_ue->valuestring) : NULL,
        multi_hop_u2_u_end_ue && cJSON_IsNull(multi_hop_u2_u_end_ue) ? true : false,
        multi_hop_u2_u_end_ue && !cJSON_IsNull(multi_hop_u2_u_end_ue) ? ogs_strdup(multi_hop_u2_u_end_ue->valuestring) : NULL,
        multi_hop_u2_n_rel_ue && cJSON_IsNull(multi_hop_u2_n_rel_ue) ? true : false,
        multi_hop_u2_n_rel_ue && !cJSON_IsNull(multi_hop_u2_n_rel_ue) ? ogs_strdup(multi_hop_u2_n_rel_ue->valuestring) : NULL,
        multi_hop_u2_n_rem_ue && cJSON_IsNull(multi_hop_u2_n_rem_ue) ? true : false,
        multi_hop_u2_n_rem_ue && !cJSON_IsNull(multi_hop_u2_n_rem_ue) ? ogs_strdup(multi_hop_u2_n_rem_ue->valuestring) : NULL,
        multi_hop_u2_n_interm_ue && cJSON_IsNull(multi_hop_u2_n_interm_ue) ? true : false,
        multi_hop_u2_n_interm_ue && !cJSON_IsNull(multi_hop_u2_n_interm_ue) ? ogs_strdup(multi_hop_u2_n_interm_ue->valuestring) : NULL,
        ursp_influence ? ursp_influenceList : NULL,
        ursp_guidance && cJSON_IsNull(ursp_guidance) ? true : false,
        ursp_guidance ? ursp_guidanceList : NULL,
        vps_ursp_guidance && cJSON_IsNull(vps_ursp_guidance) ? true : false,
        vps_ursp_guidance ? vps_ursp_guidanceList : NULL,
        tnaps && cJSON_IsNull(tnaps) ? true : false,
        tnaps ? tnapsList : NULL,
        delivery_events && cJSON_IsNull(delivery_events) ? true : false,
        delivery_events ? delivery_eventsList : NULL,
        polic_deliv_notif_uri && !cJSON_IsNull(polic_deliv_notif_uri) ? ogs_strdup(polic_deliv_notif_uri->valuestring) : NULL,
        headers ? headersList : NULL,
        param_for_ranging_sl_pos && cJSON_IsNull(param_for_ranging_sl_pos) ? true : false,
        param_for_ranging_sl_pos && !cJSON_IsNull(param_for_ranging_sl_pos) ? ogs_strdup(param_for_ranging_sl_pos->valuestring) : NULL
    );

    return service_parameter_data_patch_local_var;
end:
    if (ursp_influenceList) {
        OpenAPI_list_for_each(ursp_influenceList, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(ursp_influenceList);
        ursp_influenceList = NULL;
    }
    if (ursp_guidanceList) {
        OpenAPI_list_for_each(ursp_guidanceList, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(ursp_guidanceList);
        ursp_guidanceList = NULL;
    }
    if (vps_ursp_guidanceList) {
        OpenAPI_list_for_each(vps_ursp_guidanceList, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(vps_ursp_guidanceList);
        vps_ursp_guidanceList = NULL;
    }
    if (tnapsList) {
        OpenAPI_list_for_each(tnapsList, node) {
            OpenAPI_tnap_id_free(node->data);
        }
        OpenAPI_list_free(tnapsList);
        tnapsList = NULL;
    }
    if (delivery_eventsList) {
        OpenAPI_list_free(delivery_eventsList);
        delivery_eventsList = NULL;
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

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_copy(OpenAPI_service_parameter_data_patch_t *dst, OpenAPI_service_parameter_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_parameter_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed");
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

    OpenAPI_service_parameter_data_patch_free(dst);
    dst = OpenAPI_service_parameter_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

