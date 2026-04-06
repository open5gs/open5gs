
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_sub_component_rm.h"

OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_create(
    OpenAPI_af_sig_protocol_e af_sig_protocol,
    bool is_ethf_descs_null,
    OpenAPI_list_t *ethf_descs,
    int f_num,
    bool is_f_descs_null,
    OpenAPI_list_t *f_descs,
    bool is_add_info_flow_descs_null,
    OpenAPI_list_t *add_info_flow_descs,
    OpenAPI_flow_status_e f_status,
    bool is_mar_bw_dl_null,
    char *mar_bw_dl,
    bool is_mar_bw_ul_null,
    char *mar_bw_ul,
    bool is_tos_tr_cl_null,
    char *tos_tr_cl,
    OpenAPI_flow_usage_e flow_usage,
    bool is_ev_subsc_null,
    OpenAPI_events_subsc_req_data_rm_t *ev_subsc,
    bool is_mpx_media_ul_infos_null,
    OpenAPI_list_t *mpx_media_ul_infos,
    bool is_mpx_media_dl_infos_null,
    OpenAPI_list_t *mpx_media_dl_infos
)
{
    OpenAPI_media_sub_component_rm_t *media_sub_component_rm_local_var = ogs_malloc(sizeof(OpenAPI_media_sub_component_rm_t));
    ogs_assert(media_sub_component_rm_local_var);

    media_sub_component_rm_local_var->af_sig_protocol = af_sig_protocol;
    media_sub_component_rm_local_var->is_ethf_descs_null = is_ethf_descs_null;
    media_sub_component_rm_local_var->ethf_descs = ethf_descs;
    media_sub_component_rm_local_var->f_num = f_num;
    media_sub_component_rm_local_var->is_f_descs_null = is_f_descs_null;
    media_sub_component_rm_local_var->f_descs = f_descs;
    media_sub_component_rm_local_var->is_add_info_flow_descs_null = is_add_info_flow_descs_null;
    media_sub_component_rm_local_var->add_info_flow_descs = add_info_flow_descs;
    media_sub_component_rm_local_var->f_status = f_status;
    media_sub_component_rm_local_var->is_mar_bw_dl_null = is_mar_bw_dl_null;
    media_sub_component_rm_local_var->mar_bw_dl = mar_bw_dl;
    media_sub_component_rm_local_var->is_mar_bw_ul_null = is_mar_bw_ul_null;
    media_sub_component_rm_local_var->mar_bw_ul = mar_bw_ul;
    media_sub_component_rm_local_var->is_tos_tr_cl_null = is_tos_tr_cl_null;
    media_sub_component_rm_local_var->tos_tr_cl = tos_tr_cl;
    media_sub_component_rm_local_var->flow_usage = flow_usage;
    media_sub_component_rm_local_var->is_ev_subsc_null = is_ev_subsc_null;
    media_sub_component_rm_local_var->ev_subsc = ev_subsc;
    media_sub_component_rm_local_var->is_mpx_media_ul_infos_null = is_mpx_media_ul_infos_null;
    media_sub_component_rm_local_var->mpx_media_ul_infos = mpx_media_ul_infos;
    media_sub_component_rm_local_var->is_mpx_media_dl_infos_null = is_mpx_media_dl_infos_null;
    media_sub_component_rm_local_var->mpx_media_dl_infos = mpx_media_dl_infos;

    return media_sub_component_rm_local_var;
}

void OpenAPI_media_sub_component_rm_free(OpenAPI_media_sub_component_rm_t *media_sub_component_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == media_sub_component_rm) {
        return;
    }
    if (media_sub_component_rm->ethf_descs) {
        OpenAPI_list_for_each(media_sub_component_rm->ethf_descs, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(media_sub_component_rm->ethf_descs);
        media_sub_component_rm->ethf_descs = NULL;
    }
    if (media_sub_component_rm->f_descs) {
        OpenAPI_list_for_each(media_sub_component_rm->f_descs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(media_sub_component_rm->f_descs);
        media_sub_component_rm->f_descs = NULL;
    }
    if (media_sub_component_rm->add_info_flow_descs) {
        OpenAPI_list_for_each(media_sub_component_rm->add_info_flow_descs, node) {
            OpenAPI_add_flow_description_info_free(node->data);
        }
        OpenAPI_list_free(media_sub_component_rm->add_info_flow_descs);
        media_sub_component_rm->add_info_flow_descs = NULL;
    }
    if (media_sub_component_rm->mar_bw_dl) {
        ogs_free(media_sub_component_rm->mar_bw_dl);
        media_sub_component_rm->mar_bw_dl = NULL;
    }
    if (media_sub_component_rm->mar_bw_ul) {
        ogs_free(media_sub_component_rm->mar_bw_ul);
        media_sub_component_rm->mar_bw_ul = NULL;
    }
    if (media_sub_component_rm->tos_tr_cl) {
        ogs_free(media_sub_component_rm->tos_tr_cl);
        media_sub_component_rm->tos_tr_cl = NULL;
    }
    if (media_sub_component_rm->ev_subsc) {
        OpenAPI_events_subsc_req_data_rm_free(media_sub_component_rm->ev_subsc);
        media_sub_component_rm->ev_subsc = NULL;
    }
    if (media_sub_component_rm->mpx_media_ul_infos) {
        OpenAPI_list_for_each(media_sub_component_rm->mpx_media_ul_infos, node) {
            OpenAPI_mpx_media_info_free(node->data);
        }
        OpenAPI_list_free(media_sub_component_rm->mpx_media_ul_infos);
        media_sub_component_rm->mpx_media_ul_infos = NULL;
    }
    if (media_sub_component_rm->mpx_media_dl_infos) {
        OpenAPI_list_for_each(media_sub_component_rm->mpx_media_dl_infos, node) {
            OpenAPI_mpx_media_info_free(node->data);
        }
        OpenAPI_list_free(media_sub_component_rm->mpx_media_dl_infos);
        media_sub_component_rm->mpx_media_dl_infos = NULL;
    }
    ogs_free(media_sub_component_rm);
}

cJSON *OpenAPI_media_sub_component_rm_convertToJSON(OpenAPI_media_sub_component_rm_t *media_sub_component_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (media_sub_component_rm == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [MediaSubComponentRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (media_sub_component_rm->af_sig_protocol != OpenAPI_af_sig_protocol_NULL) {
    if (cJSON_AddStringToObject(item, "afSigProtocol", OpenAPI_af_sig_protocol_ToString(media_sub_component_rm->af_sig_protocol)) == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [af_sig_protocol]");
        goto end;
    }
    }

    if (media_sub_component_rm->ethf_descs) {
    cJSON *ethf_descsList = cJSON_AddArrayToObject(item, "ethfDescs");
    if (ethf_descsList == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [ethf_descs]");
        goto end;
    }
    OpenAPI_list_for_each(media_sub_component_rm->ethf_descs, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [ethf_descs]");
            goto end;
        }
        cJSON_AddItemToArray(ethf_descsList, itemLocal);
    }
    } else if (media_sub_component_rm->is_ethf_descs_null) {
        if (cJSON_AddNullToObject(item, "ethfDescs") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [ethf_descs]");
            goto end;
        }
    }

    if (cJSON_AddNumberToObject(item, "fNum", media_sub_component_rm->f_num) == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [f_num]");
        goto end;
    }

    if (media_sub_component_rm->f_descs) {
    cJSON *f_descsList = cJSON_AddArrayToObject(item, "fDescs");
    if (f_descsList == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [f_descs]");
        goto end;
    }
    OpenAPI_list_for_each(media_sub_component_rm->f_descs, node) {
        if (cJSON_AddStringToObject(f_descsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [f_descs]");
            goto end;
        }
    }
    } else if (media_sub_component_rm->is_f_descs_null) {
        if (cJSON_AddNullToObject(item, "fDescs") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [f_descs]");
            goto end;
        }
    }

    if (media_sub_component_rm->add_info_flow_descs) {
    cJSON *add_info_flow_descsList = cJSON_AddArrayToObject(item, "addInfoFlowDescs");
    if (add_info_flow_descsList == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [add_info_flow_descs]");
        goto end;
    }
    OpenAPI_list_for_each(media_sub_component_rm->add_info_flow_descs, node) {
        cJSON *itemLocal = OpenAPI_add_flow_description_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [add_info_flow_descs]");
            goto end;
        }
        cJSON_AddItemToArray(add_info_flow_descsList, itemLocal);
    }
    } else if (media_sub_component_rm->is_add_info_flow_descs_null) {
        if (cJSON_AddNullToObject(item, "addInfoFlowDescs") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [add_info_flow_descs]");
            goto end;
        }
    }

    if (media_sub_component_rm->f_status != OpenAPI_flow_status_NULL) {
    if (cJSON_AddStringToObject(item, "fStatus", OpenAPI_flow_status_ToString(media_sub_component_rm->f_status)) == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [f_status]");
        goto end;
    }
    }

    if (media_sub_component_rm->mar_bw_dl) {
    if (cJSON_AddStringToObject(item, "marBwDl", media_sub_component_rm->mar_bw_dl) == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mar_bw_dl]");
        goto end;
    }
    } else if (media_sub_component_rm->is_mar_bw_dl_null) {
        if (cJSON_AddNullToObject(item, "marBwDl") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mar_bw_dl]");
            goto end;
        }
    }

    if (media_sub_component_rm->mar_bw_ul) {
    if (cJSON_AddStringToObject(item, "marBwUl", media_sub_component_rm->mar_bw_ul) == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mar_bw_ul]");
        goto end;
    }
    } else if (media_sub_component_rm->is_mar_bw_ul_null) {
        if (cJSON_AddNullToObject(item, "marBwUl") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mar_bw_ul]");
            goto end;
        }
    }

    if (media_sub_component_rm->tos_tr_cl) {
    if (cJSON_AddStringToObject(item, "tosTrCl", media_sub_component_rm->tos_tr_cl) == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [tos_tr_cl]");
        goto end;
    }
    } else if (media_sub_component_rm->is_tos_tr_cl_null) {
        if (cJSON_AddNullToObject(item, "tosTrCl") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [tos_tr_cl]");
            goto end;
        }
    }

    if (media_sub_component_rm->flow_usage != OpenAPI_flow_usage_NULL) {
    if (cJSON_AddStringToObject(item, "flowUsage", OpenAPI_flow_usage_ToString(media_sub_component_rm->flow_usage)) == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [flow_usage]");
        goto end;
    }
    }

    if (media_sub_component_rm->ev_subsc) {
    cJSON *ev_subsc_local_JSON = OpenAPI_events_subsc_req_data_rm_convertToJSON(media_sub_component_rm->ev_subsc);
    if (ev_subsc_local_JSON == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [ev_subsc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "evSubsc", ev_subsc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [ev_subsc]");
        goto end;
    }
    } else if (media_sub_component_rm->is_ev_subsc_null) {
        if (cJSON_AddNullToObject(item, "evSubsc") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [ev_subsc]");
            goto end;
        }
    }

    if (media_sub_component_rm->mpx_media_ul_infos) {
    cJSON *mpx_media_ul_infosList = cJSON_AddArrayToObject(item, "mpxMediaUlInfos");
    if (mpx_media_ul_infosList == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mpx_media_ul_infos]");
        goto end;
    }
    OpenAPI_list_for_each(media_sub_component_rm->mpx_media_ul_infos, node) {
        cJSON *itemLocal = OpenAPI_mpx_media_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mpx_media_ul_infos]");
            goto end;
        }
        cJSON_AddItemToArray(mpx_media_ul_infosList, itemLocal);
    }
    } else if (media_sub_component_rm->is_mpx_media_ul_infos_null) {
        if (cJSON_AddNullToObject(item, "mpxMediaUlInfos") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mpx_media_ul_infos]");
            goto end;
        }
    }

    if (media_sub_component_rm->mpx_media_dl_infos) {
    cJSON *mpx_media_dl_infosList = cJSON_AddArrayToObject(item, "mpxMediaDlInfos");
    if (mpx_media_dl_infosList == NULL) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mpx_media_dl_infos]");
        goto end;
    }
    OpenAPI_list_for_each(media_sub_component_rm->mpx_media_dl_infos, node) {
        cJSON *itemLocal = OpenAPI_mpx_media_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mpx_media_dl_infos]");
            goto end;
        }
        cJSON_AddItemToArray(mpx_media_dl_infosList, itemLocal);
    }
    } else if (media_sub_component_rm->is_mpx_media_dl_infos_null) {
        if (cJSON_AddNullToObject(item, "mpxMediaDlInfos") == NULL) {
            ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed [mpx_media_dl_infos]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_parseFromJSON(cJSON *media_sub_component_rmJSON)
{
    OpenAPI_media_sub_component_rm_t *media_sub_component_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_sig_protocol = NULL;
    OpenAPI_af_sig_protocol_e af_sig_protocolVariable = 0;
    cJSON *ethf_descs = NULL;
    OpenAPI_list_t *ethf_descsList = NULL;
    cJSON *f_num = NULL;
    cJSON *f_descs = NULL;
    OpenAPI_list_t *f_descsList = NULL;
    cJSON *add_info_flow_descs = NULL;
    OpenAPI_list_t *add_info_flow_descsList = NULL;
    cJSON *f_status = NULL;
    OpenAPI_flow_status_e f_statusVariable = 0;
    cJSON *mar_bw_dl = NULL;
    cJSON *mar_bw_ul = NULL;
    cJSON *tos_tr_cl = NULL;
    cJSON *flow_usage = NULL;
    OpenAPI_flow_usage_e flow_usageVariable = 0;
    cJSON *ev_subsc = NULL;
    OpenAPI_events_subsc_req_data_rm_t *ev_subsc_local_nonprim = NULL;
    cJSON *mpx_media_ul_infos = NULL;
    OpenAPI_list_t *mpx_media_ul_infosList = NULL;
    cJSON *mpx_media_dl_infos = NULL;
    OpenAPI_list_t *mpx_media_dl_infosList = NULL;
    af_sig_protocol = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "afSigProtocol");
    if (af_sig_protocol) {
    if (!cJSON_IsString(af_sig_protocol)) {
        ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [af_sig_protocol]");
        goto end;
    }
    af_sig_protocolVariable = OpenAPI_af_sig_protocol_FromString(af_sig_protocol->valuestring);
    }

    ethf_descs = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "ethfDescs");
    if (ethf_descs) {
    if (!cJSON_IsNull(ethf_descs)) {
        cJSON *ethf_descs_local = NULL;
        if (!cJSON_IsArray(ethf_descs)) {
            ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [ethf_descs]");
            goto end;
        }

        ethf_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ethf_descs_local, ethf_descs) {
            if (!cJSON_IsObject(ethf_descs_local)) {
                ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [ethf_descs]");
                goto end;
            }
            OpenAPI_eth_flow_description_t *ethf_descsItem = OpenAPI_eth_flow_description_parseFromJSON(ethf_descs_local);
            if (!ethf_descsItem) {
                ogs_error("No ethf_descsItem");
                goto end;
            }
            OpenAPI_list_add(ethf_descsList, ethf_descsItem);
        }
    }
    }

    f_num = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "fNum");
    if (!f_num) {
        ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [f_num]");
        goto end;
    }
    if (!cJSON_IsNumber(f_num)) {
        ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [f_num]");
        goto end;
    }

    f_descs = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "fDescs");
    if (f_descs) {
    if (!cJSON_IsNull(f_descs)) {
        cJSON *f_descs_local = NULL;
        if (!cJSON_IsArray(f_descs)) {
            ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [f_descs]");
            goto end;
        }

        f_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(f_descs_local, f_descs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(f_descs_local)) {
                ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [f_descs]");
                goto end;
            }
            OpenAPI_list_add(f_descsList, ogs_strdup(f_descs_local->valuestring));
        }
    }
    }

    add_info_flow_descs = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "addInfoFlowDescs");
    if (add_info_flow_descs) {
    if (!cJSON_IsNull(add_info_flow_descs)) {
        cJSON *add_info_flow_descs_local = NULL;
        if (!cJSON_IsArray(add_info_flow_descs)) {
            ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [add_info_flow_descs]");
            goto end;
        }

        add_info_flow_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_info_flow_descs_local, add_info_flow_descs) {
            if (!cJSON_IsObject(add_info_flow_descs_local)) {
                ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [add_info_flow_descs]");
                goto end;
            }
            OpenAPI_add_flow_description_info_t *add_info_flow_descsItem = OpenAPI_add_flow_description_info_parseFromJSON(add_info_flow_descs_local);
            if (!add_info_flow_descsItem) {
                ogs_error("No add_info_flow_descsItem");
                goto end;
            }
            OpenAPI_list_add(add_info_flow_descsList, add_info_flow_descsItem);
        }
    }
    }

    f_status = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "fStatus");
    if (f_status) {
    if (!cJSON_IsString(f_status)) {
        ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [f_status]");
        goto end;
    }
    f_statusVariable = OpenAPI_flow_status_FromString(f_status->valuestring);
    }

    mar_bw_dl = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "marBwDl");
    if (mar_bw_dl) {
    if (!cJSON_IsNull(mar_bw_dl)) {
    if (!cJSON_IsString(mar_bw_dl) && !cJSON_IsNull(mar_bw_dl)) {
        ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [mar_bw_dl]");
        goto end;
    }
    }
    }

    mar_bw_ul = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "marBwUl");
    if (mar_bw_ul) {
    if (!cJSON_IsNull(mar_bw_ul)) {
    if (!cJSON_IsString(mar_bw_ul) && !cJSON_IsNull(mar_bw_ul)) {
        ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [mar_bw_ul]");
        goto end;
    }
    }
    }

    tos_tr_cl = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "tosTrCl");
    if (tos_tr_cl) {
    if (!cJSON_IsNull(tos_tr_cl)) {
    if (!cJSON_IsString(tos_tr_cl) && !cJSON_IsNull(tos_tr_cl)) {
        ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [tos_tr_cl]");
        goto end;
    }
    }
    }

    flow_usage = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "flowUsage");
    if (flow_usage) {
    if (!cJSON_IsString(flow_usage)) {
        ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [flow_usage]");
        goto end;
    }
    flow_usageVariable = OpenAPI_flow_usage_FromString(flow_usage->valuestring);
    }

    ev_subsc = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "evSubsc");
    if (ev_subsc) {
    if (!cJSON_IsNull(ev_subsc)) {
    ev_subsc_local_nonprim = OpenAPI_events_subsc_req_data_rm_parseFromJSON(ev_subsc);
    if (!ev_subsc_local_nonprim) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON failed [ev_subsc]");
        goto end;
    }
    }
    }

    mpx_media_ul_infos = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "mpxMediaUlInfos");
    if (mpx_media_ul_infos) {
    if (!cJSON_IsNull(mpx_media_ul_infos)) {
        cJSON *mpx_media_ul_infos_local = NULL;
        if (!cJSON_IsArray(mpx_media_ul_infos)) {
            ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [mpx_media_ul_infos]");
            goto end;
        }

        mpx_media_ul_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(mpx_media_ul_infos_local, mpx_media_ul_infos) {
            if (!cJSON_IsObject(mpx_media_ul_infos_local)) {
                ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [mpx_media_ul_infos]");
                goto end;
            }
            OpenAPI_mpx_media_info_t *mpx_media_ul_infosItem = OpenAPI_mpx_media_info_parseFromJSON(mpx_media_ul_infos_local);
            if (!mpx_media_ul_infosItem) {
                ogs_error("No mpx_media_ul_infosItem");
                goto end;
            }
            OpenAPI_list_add(mpx_media_ul_infosList, mpx_media_ul_infosItem);
        }
    }
    }

    mpx_media_dl_infos = cJSON_GetObjectItemCaseSensitive(media_sub_component_rmJSON, "mpxMediaDlInfos");
    if (mpx_media_dl_infos) {
    if (!cJSON_IsNull(mpx_media_dl_infos)) {
        cJSON *mpx_media_dl_infos_local = NULL;
        if (!cJSON_IsArray(mpx_media_dl_infos)) {
            ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [mpx_media_dl_infos]");
            goto end;
        }

        mpx_media_dl_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(mpx_media_dl_infos_local, mpx_media_dl_infos) {
            if (!cJSON_IsObject(mpx_media_dl_infos_local)) {
                ogs_error("OpenAPI_media_sub_component_rm_parseFromJSON() failed [mpx_media_dl_infos]");
                goto end;
            }
            OpenAPI_mpx_media_info_t *mpx_media_dl_infosItem = OpenAPI_mpx_media_info_parseFromJSON(mpx_media_dl_infos_local);
            if (!mpx_media_dl_infosItem) {
                ogs_error("No mpx_media_dl_infosItem");
                goto end;
            }
            OpenAPI_list_add(mpx_media_dl_infosList, mpx_media_dl_infosItem);
        }
    }
    }

    media_sub_component_rm_local_var = OpenAPI_media_sub_component_rm_create (
        af_sig_protocol ? af_sig_protocolVariable : 0,
        ethf_descs && cJSON_IsNull(ethf_descs) ? true : false,
        ethf_descs ? ethf_descsList : NULL,
        
        f_num->valuedouble,
        f_descs && cJSON_IsNull(f_descs) ? true : false,
        f_descs ? f_descsList : NULL,
        add_info_flow_descs && cJSON_IsNull(add_info_flow_descs) ? true : false,
        add_info_flow_descs ? add_info_flow_descsList : NULL,
        f_status ? f_statusVariable : 0,
        mar_bw_dl && cJSON_IsNull(mar_bw_dl) ? true : false,
        mar_bw_dl && !cJSON_IsNull(mar_bw_dl) ? ogs_strdup(mar_bw_dl->valuestring) : NULL,
        mar_bw_ul && cJSON_IsNull(mar_bw_ul) ? true : false,
        mar_bw_ul && !cJSON_IsNull(mar_bw_ul) ? ogs_strdup(mar_bw_ul->valuestring) : NULL,
        tos_tr_cl && cJSON_IsNull(tos_tr_cl) ? true : false,
        tos_tr_cl && !cJSON_IsNull(tos_tr_cl) ? ogs_strdup(tos_tr_cl->valuestring) : NULL,
        flow_usage ? flow_usageVariable : 0,
        ev_subsc && cJSON_IsNull(ev_subsc) ? true : false,
        ev_subsc ? ev_subsc_local_nonprim : NULL,
        mpx_media_ul_infos && cJSON_IsNull(mpx_media_ul_infos) ? true : false,
        mpx_media_ul_infos ? mpx_media_ul_infosList : NULL,
        mpx_media_dl_infos && cJSON_IsNull(mpx_media_dl_infos) ? true : false,
        mpx_media_dl_infos ? mpx_media_dl_infosList : NULL
    );

    return media_sub_component_rm_local_var;
end:
    if (ethf_descsList) {
        OpenAPI_list_for_each(ethf_descsList, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(ethf_descsList);
        ethf_descsList = NULL;
    }
    if (f_descsList) {
        OpenAPI_list_for_each(f_descsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(f_descsList);
        f_descsList = NULL;
    }
    if (add_info_flow_descsList) {
        OpenAPI_list_for_each(add_info_flow_descsList, node) {
            OpenAPI_add_flow_description_info_free(node->data);
        }
        OpenAPI_list_free(add_info_flow_descsList);
        add_info_flow_descsList = NULL;
    }
    if (ev_subsc_local_nonprim) {
        OpenAPI_events_subsc_req_data_rm_free(ev_subsc_local_nonprim);
        ev_subsc_local_nonprim = NULL;
    }
    if (mpx_media_ul_infosList) {
        OpenAPI_list_for_each(mpx_media_ul_infosList, node) {
            OpenAPI_mpx_media_info_free(node->data);
        }
        OpenAPI_list_free(mpx_media_ul_infosList);
        mpx_media_ul_infosList = NULL;
    }
    if (mpx_media_dl_infosList) {
        OpenAPI_list_for_each(mpx_media_dl_infosList, node) {
            OpenAPI_mpx_media_info_free(node->data);
        }
        OpenAPI_list_free(mpx_media_dl_infosList);
        mpx_media_dl_infosList = NULL;
    }
    return NULL;
}

OpenAPI_media_sub_component_rm_t *OpenAPI_media_sub_component_rm_copy(OpenAPI_media_sub_component_rm_t *dst, OpenAPI_media_sub_component_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_media_sub_component_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_media_sub_component_rm_convertToJSON() failed");
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

    OpenAPI_media_sub_component_rm_free(dst);
    dst = OpenAPI_media_sub_component_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

