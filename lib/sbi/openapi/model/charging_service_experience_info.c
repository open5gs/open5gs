
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_service_experience_info.h"

OpenAPI_charging_service_experience_info_t *OpenAPI_charging_service_experience_info_create(
    OpenAPI_svc_experience_t *svc_exprc,
    bool is_svc_exprc_variance,
    float svc_exprc_variance,
    OpenAPI_list_t *supis,
    OpenAPI_snssai_t *snssai,
    char *app_id,
    OpenAPI_service_experience_type_e srv_expc_type,
    OpenAPI_list_t *ue_locs,
    OpenAPI_upf_information_t *upf_info,
    char *dnai,
    OpenAPI_addr_fqdn_t *app_server_inst,
    bool is_confidence,
    int confidence,
    char *dnn,
    OpenAPI_network_area_info_t *network_area,
    char *nsi_id,
    bool is_ratio,
    int ratio,
    OpenAPI_rat_freq_information_t *rat_freq,
    OpenAPI_charging_pdu_session_info_t *pdu_ses_info
)
{
    OpenAPI_charging_service_experience_info_t *charging_service_experience_info_local_var = ogs_malloc(sizeof(OpenAPI_charging_service_experience_info_t));
    ogs_assert(charging_service_experience_info_local_var);

    charging_service_experience_info_local_var->svc_exprc = svc_exprc;
    charging_service_experience_info_local_var->is_svc_exprc_variance = is_svc_exprc_variance;
    charging_service_experience_info_local_var->svc_exprc_variance = svc_exprc_variance;
    charging_service_experience_info_local_var->supis = supis;
    charging_service_experience_info_local_var->snssai = snssai;
    charging_service_experience_info_local_var->app_id = app_id;
    charging_service_experience_info_local_var->srv_expc_type = srv_expc_type;
    charging_service_experience_info_local_var->ue_locs = ue_locs;
    charging_service_experience_info_local_var->upf_info = upf_info;
    charging_service_experience_info_local_var->dnai = dnai;
    charging_service_experience_info_local_var->app_server_inst = app_server_inst;
    charging_service_experience_info_local_var->is_confidence = is_confidence;
    charging_service_experience_info_local_var->confidence = confidence;
    charging_service_experience_info_local_var->dnn = dnn;
    charging_service_experience_info_local_var->network_area = network_area;
    charging_service_experience_info_local_var->nsi_id = nsi_id;
    charging_service_experience_info_local_var->is_ratio = is_ratio;
    charging_service_experience_info_local_var->ratio = ratio;
    charging_service_experience_info_local_var->rat_freq = rat_freq;
    charging_service_experience_info_local_var->pdu_ses_info = pdu_ses_info;

    return charging_service_experience_info_local_var;
}

void OpenAPI_charging_service_experience_info_free(OpenAPI_charging_service_experience_info_t *charging_service_experience_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_service_experience_info) {
        return;
    }
    if (charging_service_experience_info->svc_exprc) {
        OpenAPI_svc_experience_free(charging_service_experience_info->svc_exprc);
        charging_service_experience_info->svc_exprc = NULL;
    }
    if (charging_service_experience_info->supis) {
        OpenAPI_list_for_each(charging_service_experience_info->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(charging_service_experience_info->supis);
        charging_service_experience_info->supis = NULL;
    }
    if (charging_service_experience_info->snssai) {
        OpenAPI_snssai_free(charging_service_experience_info->snssai);
        charging_service_experience_info->snssai = NULL;
    }
    if (charging_service_experience_info->app_id) {
        ogs_free(charging_service_experience_info->app_id);
        charging_service_experience_info->app_id = NULL;
    }
    if (charging_service_experience_info->ue_locs) {
        OpenAPI_list_for_each(charging_service_experience_info->ue_locs, node) {
            OpenAPI_charging_location_info_free(node->data);
        }
        OpenAPI_list_free(charging_service_experience_info->ue_locs);
        charging_service_experience_info->ue_locs = NULL;
    }
    if (charging_service_experience_info->upf_info) {
        OpenAPI_upf_information_free(charging_service_experience_info->upf_info);
        charging_service_experience_info->upf_info = NULL;
    }
    if (charging_service_experience_info->dnai) {
        ogs_free(charging_service_experience_info->dnai);
        charging_service_experience_info->dnai = NULL;
    }
    if (charging_service_experience_info->app_server_inst) {
        OpenAPI_addr_fqdn_free(charging_service_experience_info->app_server_inst);
        charging_service_experience_info->app_server_inst = NULL;
    }
    if (charging_service_experience_info->dnn) {
        ogs_free(charging_service_experience_info->dnn);
        charging_service_experience_info->dnn = NULL;
    }
    if (charging_service_experience_info->network_area) {
        OpenAPI_network_area_info_free(charging_service_experience_info->network_area);
        charging_service_experience_info->network_area = NULL;
    }
    if (charging_service_experience_info->nsi_id) {
        ogs_free(charging_service_experience_info->nsi_id);
        charging_service_experience_info->nsi_id = NULL;
    }
    if (charging_service_experience_info->rat_freq) {
        OpenAPI_rat_freq_information_free(charging_service_experience_info->rat_freq);
        charging_service_experience_info->rat_freq = NULL;
    }
    if (charging_service_experience_info->pdu_ses_info) {
        OpenAPI_charging_pdu_session_info_free(charging_service_experience_info->pdu_ses_info);
        charging_service_experience_info->pdu_ses_info = NULL;
    }
    ogs_free(charging_service_experience_info);
}

cJSON *OpenAPI_charging_service_experience_info_convertToJSON(OpenAPI_charging_service_experience_info_t *charging_service_experience_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_service_experience_info == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [ChargingServiceExperienceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!charging_service_experience_info->svc_exprc) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [svc_exprc]");
        return NULL;
    }
    cJSON *svc_exprc_local_JSON = OpenAPI_svc_experience_convertToJSON(charging_service_experience_info->svc_exprc);
    if (svc_exprc_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [svc_exprc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "svcExprc", svc_exprc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [svc_exprc]");
        goto end;
    }

    if (charging_service_experience_info->is_svc_exprc_variance) {
    if (cJSON_AddNumberToObject(item, "svcExprcVariance", charging_service_experience_info->svc_exprc_variance) == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [svc_exprc_variance]");
        goto end;
    }
    }

    if (charging_service_experience_info->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(charging_service_experience_info->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (charging_service_experience_info->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(charging_service_experience_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (charging_service_experience_info->app_id) {
    if (cJSON_AddStringToObject(item, "appId", charging_service_experience_info->app_id) == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (charging_service_experience_info->srv_expc_type != OpenAPI_service_experience_type_NULL) {
    if (cJSON_AddStringToObject(item, "srvExpcType", OpenAPI_service_experience_type_ToString(charging_service_experience_info->srv_expc_type)) == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [srv_expc_type]");
        goto end;
    }
    }

    if (charging_service_experience_info->ue_locs) {
    cJSON *ue_locsList = cJSON_AddArrayToObject(item, "ueLocs");
    if (ue_locsList == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [ue_locs]");
        goto end;
    }
    OpenAPI_list_for_each(charging_service_experience_info->ue_locs, node) {
        cJSON *itemLocal = OpenAPI_charging_location_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [ue_locs]");
            goto end;
        }
        cJSON_AddItemToArray(ue_locsList, itemLocal);
    }
    }

    if (charging_service_experience_info->upf_info) {
    cJSON *upf_info_local_JSON = OpenAPI_upf_information_convertToJSON(charging_service_experience_info->upf_info);
    if (upf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [upf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upfInfo", upf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [upf_info]");
        goto end;
    }
    }

    if (charging_service_experience_info->dnai) {
    if (cJSON_AddStringToObject(item, "dnai", charging_service_experience_info->dnai) == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [dnai]");
        goto end;
    }
    }

    if (charging_service_experience_info->app_server_inst) {
    cJSON *app_server_inst_local_JSON = OpenAPI_addr_fqdn_convertToJSON(charging_service_experience_info->app_server_inst);
    if (app_server_inst_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [app_server_inst]");
        goto end;
    }
    cJSON_AddItemToObject(item, "appServerInst", app_server_inst_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [app_server_inst]");
        goto end;
    }
    }

    if (charging_service_experience_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", charging_service_experience_info->confidence) == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

    if (charging_service_experience_info->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", charging_service_experience_info->dnn) == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (charging_service_experience_info->network_area) {
    cJSON *network_area_local_JSON = OpenAPI_network_area_info_convertToJSON(charging_service_experience_info->network_area);
    if (network_area_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [network_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkArea", network_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [network_area]");
        goto end;
    }
    }

    if (charging_service_experience_info->nsi_id) {
    if (cJSON_AddStringToObject(item, "nsiId", charging_service_experience_info->nsi_id) == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [nsi_id]");
        goto end;
    }
    }

    if (charging_service_experience_info->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", charging_service_experience_info->ratio) == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [ratio]");
        goto end;
    }
    }

    if (charging_service_experience_info->rat_freq) {
    cJSON *rat_freq_local_JSON = OpenAPI_rat_freq_information_convertToJSON(charging_service_experience_info->rat_freq);
    if (rat_freq_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [rat_freq]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ratFreq", rat_freq_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [rat_freq]");
        goto end;
    }
    }

    if (charging_service_experience_info->pdu_ses_info) {
    cJSON *pdu_ses_info_local_JSON = OpenAPI_charging_pdu_session_info_convertToJSON(charging_service_experience_info->pdu_ses_info);
    if (pdu_ses_info_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [pdu_ses_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSesInfo", pdu_ses_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed [pdu_ses_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_service_experience_info_t *OpenAPI_charging_service_experience_info_parseFromJSON(cJSON *charging_service_experience_infoJSON)
{
    OpenAPI_charging_service_experience_info_t *charging_service_experience_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *svc_exprc = NULL;
    OpenAPI_svc_experience_t *svc_exprc_local_nonprim = NULL;
    cJSON *svc_exprc_variance = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *app_id = NULL;
    cJSON *srv_expc_type = NULL;
    OpenAPI_service_experience_type_e srv_expc_typeVariable = 0;
    cJSON *ue_locs = NULL;
    OpenAPI_list_t *ue_locsList = NULL;
    cJSON *upf_info = NULL;
    OpenAPI_upf_information_t *upf_info_local_nonprim = NULL;
    cJSON *dnai = NULL;
    cJSON *app_server_inst = NULL;
    OpenAPI_addr_fqdn_t *app_server_inst_local_nonprim = NULL;
    cJSON *confidence = NULL;
    cJSON *dnn = NULL;
    cJSON *network_area = NULL;
    OpenAPI_network_area_info_t *network_area_local_nonprim = NULL;
    cJSON *nsi_id = NULL;
    cJSON *ratio = NULL;
    cJSON *rat_freq = NULL;
    OpenAPI_rat_freq_information_t *rat_freq_local_nonprim = NULL;
    cJSON *pdu_ses_info = NULL;
    OpenAPI_charging_pdu_session_info_t *pdu_ses_info_local_nonprim = NULL;
    svc_exprc = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "svcExprc");
    if (!svc_exprc) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [svc_exprc]");
        goto end;
    }
    svc_exprc_local_nonprim = OpenAPI_svc_experience_parseFromJSON(svc_exprc);
    if (!svc_exprc_local_nonprim) {
        ogs_error("OpenAPI_svc_experience_parseFromJSON failed [svc_exprc]");
        goto end;
    }

    svc_exprc_variance = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "svcExprcVariance");
    if (svc_exprc_variance) {
    if (!cJSON_IsNumber(svc_exprc_variance)) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [svc_exprc_variance]");
        goto end;
    }
    }

    supis = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    srv_expc_type = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "srvExpcType");
    if (srv_expc_type) {
    if (!cJSON_IsString(srv_expc_type)) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [srv_expc_type]");
        goto end;
    }
    srv_expc_typeVariable = OpenAPI_service_experience_type_FromString(srv_expc_type->valuestring);
    }

    ue_locs = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "ueLocs");
    if (ue_locs) {
        cJSON *ue_locs_local = NULL;
        if (!cJSON_IsArray(ue_locs)) {
            ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [ue_locs]");
            goto end;
        }

        ue_locsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_locs_local, ue_locs) {
            if (!cJSON_IsObject(ue_locs_local)) {
                ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [ue_locs]");
                goto end;
            }
            OpenAPI_charging_location_info_t *ue_locsItem = OpenAPI_charging_location_info_parseFromJSON(ue_locs_local);
            if (!ue_locsItem) {
                ogs_error("No ue_locsItem");
                goto end;
            }
            OpenAPI_list_add(ue_locsList, ue_locsItem);
        }
    }

    upf_info = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "upfInfo");
    if (upf_info) {
    upf_info_local_nonprim = OpenAPI_upf_information_parseFromJSON(upf_info);
    if (!upf_info_local_nonprim) {
        ogs_error("OpenAPI_upf_information_parseFromJSON failed [upf_info]");
        goto end;
    }
    }

    dnai = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "dnai");
    if (dnai) {
    if (!cJSON_IsString(dnai) && !cJSON_IsNull(dnai)) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [dnai]");
        goto end;
    }
    }

    app_server_inst = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "appServerInst");
    if (app_server_inst) {
    app_server_inst_local_nonprim = OpenAPI_addr_fqdn_parseFromJSON(app_server_inst);
    if (!app_server_inst_local_nonprim) {
        ogs_error("OpenAPI_addr_fqdn_parseFromJSON failed [app_server_inst]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    network_area = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "networkArea");
    if (network_area) {
    network_area_local_nonprim = OpenAPI_network_area_info_parseFromJSON(network_area);
    if (!network_area_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [network_area]");
        goto end;
    }
    }

    nsi_id = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "nsiId");
    if (nsi_id) {
    if (!cJSON_IsString(nsi_id) && !cJSON_IsNull(nsi_id)) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [nsi_id]");
        goto end;
    }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    rat_freq = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "ratFreq");
    if (rat_freq) {
    rat_freq_local_nonprim = OpenAPI_rat_freq_information_parseFromJSON(rat_freq);
    if (!rat_freq_local_nonprim) {
        ogs_error("OpenAPI_rat_freq_information_parseFromJSON failed [rat_freq]");
        goto end;
    }
    }

    pdu_ses_info = cJSON_GetObjectItemCaseSensitive(charging_service_experience_infoJSON, "pduSesInfo");
    if (pdu_ses_info) {
    pdu_ses_info_local_nonprim = OpenAPI_charging_pdu_session_info_parseFromJSON(pdu_ses_info);
    if (!pdu_ses_info_local_nonprim) {
        ogs_error("OpenAPI_charging_pdu_session_info_parseFromJSON failed [pdu_ses_info]");
        goto end;
    }
    }

    charging_service_experience_info_local_var = OpenAPI_charging_service_experience_info_create (
        svc_exprc_local_nonprim,
        svc_exprc_variance ? true : false,
        svc_exprc_variance ? svc_exprc_variance->valuedouble : 0,
        supis ? supisList : NULL,
        snssai ? snssai_local_nonprim : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        srv_expc_type ? srv_expc_typeVariable : 0,
        ue_locs ? ue_locsList : NULL,
        upf_info ? upf_info_local_nonprim : NULL,
        dnai && !cJSON_IsNull(dnai) ? ogs_strdup(dnai->valuestring) : NULL,
        app_server_inst ? app_server_inst_local_nonprim : NULL,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        network_area ? network_area_local_nonprim : NULL,
        nsi_id && !cJSON_IsNull(nsi_id) ? ogs_strdup(nsi_id->valuestring) : NULL,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0,
        rat_freq ? rat_freq_local_nonprim : NULL,
        pdu_ses_info ? pdu_ses_info_local_nonprim : NULL
    );

    return charging_service_experience_info_local_var;
end:
    if (svc_exprc_local_nonprim) {
        OpenAPI_svc_experience_free(svc_exprc_local_nonprim);
        svc_exprc_local_nonprim = NULL;
    }
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (ue_locsList) {
        OpenAPI_list_for_each(ue_locsList, node) {
            OpenAPI_charging_location_info_free(node->data);
        }
        OpenAPI_list_free(ue_locsList);
        ue_locsList = NULL;
    }
    if (upf_info_local_nonprim) {
        OpenAPI_upf_information_free(upf_info_local_nonprim);
        upf_info_local_nonprim = NULL;
    }
    if (app_server_inst_local_nonprim) {
        OpenAPI_addr_fqdn_free(app_server_inst_local_nonprim);
        app_server_inst_local_nonprim = NULL;
    }
    if (network_area_local_nonprim) {
        OpenAPI_network_area_info_free(network_area_local_nonprim);
        network_area_local_nonprim = NULL;
    }
    if (rat_freq_local_nonprim) {
        OpenAPI_rat_freq_information_free(rat_freq_local_nonprim);
        rat_freq_local_nonprim = NULL;
    }
    if (pdu_ses_info_local_nonprim) {
        OpenAPI_charging_pdu_session_info_free(pdu_ses_info_local_nonprim);
        pdu_ses_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_charging_service_experience_info_t *OpenAPI_charging_service_experience_info_copy(OpenAPI_charging_service_experience_info_t *dst, OpenAPI_charging_service_experience_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_service_experience_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_service_experience_info_convertToJSON() failed");
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

    OpenAPI_charging_service_experience_info_free(dst);
    dst = OpenAPI_charging_service_experience_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

