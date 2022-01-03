
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_dnn_data.h"

OpenAPI_sm_policy_dnn_data_t *OpenAPI_sm_policy_dnn_data_create(
    char *dnn,
    OpenAPI_list_t *allowed_services,
    OpenAPI_list_t *subsc_cats,
    char *gbr_ul,
    char *gbr_dl,
    bool is_adc_support,
    int adc_support,
    bool is_subsc_spending_limits,
    int subsc_spending_limits,
    bool is_ipv4_index,
    int ipv4_index,
    bool is_ipv6_index,
    int ipv6_index,
    bool is_offline,
    int offline,
    bool is_online,
    int online,
    OpenAPI_charging_information_t *chf_info,
    OpenAPI_list_t* ref_um_data_limit_ids,
    bool is_mps_priority,
    int mps_priority,
    bool is_mcs_priority,
    int mcs_priority,
    bool is_ims_signalling_prio,
    int ims_signalling_prio,
    bool is_mps_priority_level,
    int mps_priority_level,
    bool is_mcs_priority_level,
    int mcs_priority_level,
    OpenAPI_list_t* pra_infos,
    OpenAPI_list_t* bdt_ref_ids,
    bool is_loc_rout_not_allowed,
    int loc_rout_not_allowed
)
{
    OpenAPI_sm_policy_dnn_data_t *sm_policy_dnn_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_policy_dnn_data_t));
    ogs_assert(sm_policy_dnn_data_local_var);

    sm_policy_dnn_data_local_var->dnn = dnn;
    sm_policy_dnn_data_local_var->allowed_services = allowed_services;
    sm_policy_dnn_data_local_var->subsc_cats = subsc_cats;
    sm_policy_dnn_data_local_var->gbr_ul = gbr_ul;
    sm_policy_dnn_data_local_var->gbr_dl = gbr_dl;
    sm_policy_dnn_data_local_var->is_adc_support = is_adc_support;
    sm_policy_dnn_data_local_var->adc_support = adc_support;
    sm_policy_dnn_data_local_var->is_subsc_spending_limits = is_subsc_spending_limits;
    sm_policy_dnn_data_local_var->subsc_spending_limits = subsc_spending_limits;
    sm_policy_dnn_data_local_var->is_ipv4_index = is_ipv4_index;
    sm_policy_dnn_data_local_var->ipv4_index = ipv4_index;
    sm_policy_dnn_data_local_var->is_ipv6_index = is_ipv6_index;
    sm_policy_dnn_data_local_var->ipv6_index = ipv6_index;
    sm_policy_dnn_data_local_var->is_offline = is_offline;
    sm_policy_dnn_data_local_var->offline = offline;
    sm_policy_dnn_data_local_var->is_online = is_online;
    sm_policy_dnn_data_local_var->online = online;
    sm_policy_dnn_data_local_var->chf_info = chf_info;
    sm_policy_dnn_data_local_var->ref_um_data_limit_ids = ref_um_data_limit_ids;
    sm_policy_dnn_data_local_var->is_mps_priority = is_mps_priority;
    sm_policy_dnn_data_local_var->mps_priority = mps_priority;
    sm_policy_dnn_data_local_var->is_mcs_priority = is_mcs_priority;
    sm_policy_dnn_data_local_var->mcs_priority = mcs_priority;
    sm_policy_dnn_data_local_var->is_ims_signalling_prio = is_ims_signalling_prio;
    sm_policy_dnn_data_local_var->ims_signalling_prio = ims_signalling_prio;
    sm_policy_dnn_data_local_var->is_mps_priority_level = is_mps_priority_level;
    sm_policy_dnn_data_local_var->mps_priority_level = mps_priority_level;
    sm_policy_dnn_data_local_var->is_mcs_priority_level = is_mcs_priority_level;
    sm_policy_dnn_data_local_var->mcs_priority_level = mcs_priority_level;
    sm_policy_dnn_data_local_var->pra_infos = pra_infos;
    sm_policy_dnn_data_local_var->bdt_ref_ids = bdt_ref_ids;
    sm_policy_dnn_data_local_var->is_loc_rout_not_allowed = is_loc_rout_not_allowed;
    sm_policy_dnn_data_local_var->loc_rout_not_allowed = loc_rout_not_allowed;

    return sm_policy_dnn_data_local_var;
}

void OpenAPI_sm_policy_dnn_data_free(OpenAPI_sm_policy_dnn_data_t *sm_policy_dnn_data)
{
    if (NULL == sm_policy_dnn_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_policy_dnn_data->dnn);
    OpenAPI_list_for_each(sm_policy_dnn_data->allowed_services, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_policy_dnn_data->allowed_services);
    OpenAPI_list_for_each(sm_policy_dnn_data->subsc_cats, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_policy_dnn_data->subsc_cats);
    ogs_free(sm_policy_dnn_data->gbr_ul);
    ogs_free(sm_policy_dnn_data->gbr_dl);
    OpenAPI_charging_information_free(sm_policy_dnn_data->chf_info);
    OpenAPI_list_for_each(sm_policy_dnn_data->ref_um_data_limit_ids, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_limit_id_to_monitoring_key_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_dnn_data->ref_um_data_limit_ids);
    OpenAPI_list_for_each(sm_policy_dnn_data->pra_infos, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_presence_info_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_dnn_data->pra_infos);
    OpenAPI_list_for_each(sm_policy_dnn_data->bdt_ref_ids, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_dnn_data->bdt_ref_ids);
    ogs_free(sm_policy_dnn_data);
}

cJSON *OpenAPI_sm_policy_dnn_data_convertToJSON(OpenAPI_sm_policy_dnn_data_t *sm_policy_dnn_data)
{
    cJSON *item = NULL;

    if (sm_policy_dnn_data == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [SmPolicyDnnData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "dnn", sm_policy_dnn_data->dnn) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [dnn]");
        goto end;
    }

    if (sm_policy_dnn_data->allowed_services) {
    cJSON *allowed_services = cJSON_AddArrayToObject(item, "allowedServices");
    if (allowed_services == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [allowed_services]");
        goto end;
    }

    OpenAPI_lnode_t *allowed_services_node;
    OpenAPI_list_for_each(sm_policy_dnn_data->allowed_services, allowed_services_node)  {
    if (cJSON_AddStringToObject(allowed_services, "", (char*)allowed_services_node->data) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [allowed_services]");
        goto end;
    }
                    }
    }

    if (sm_policy_dnn_data->subsc_cats) {
    cJSON *subsc_cats = cJSON_AddArrayToObject(item, "subscCats");
    if (subsc_cats == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [subsc_cats]");
        goto end;
    }

    OpenAPI_lnode_t *subsc_cats_node;
    OpenAPI_list_for_each(sm_policy_dnn_data->subsc_cats, subsc_cats_node)  {
    if (cJSON_AddStringToObject(subsc_cats, "", (char*)subsc_cats_node->data) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [subsc_cats]");
        goto end;
    }
                    }
    }

    if (sm_policy_dnn_data->gbr_ul) {
    if (cJSON_AddStringToObject(item, "gbrUl", sm_policy_dnn_data->gbr_ul) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [gbr_ul]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->gbr_dl) {
    if (cJSON_AddStringToObject(item, "gbrDl", sm_policy_dnn_data->gbr_dl) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [gbr_dl]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_adc_support) {
    if (cJSON_AddBoolToObject(item, "adcSupport", sm_policy_dnn_data->adc_support) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [adc_support]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_subsc_spending_limits) {
    if (cJSON_AddBoolToObject(item, "subscSpendingLimits", sm_policy_dnn_data->subsc_spending_limits) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [subsc_spending_limits]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_ipv4_index) {
    if (cJSON_AddNumberToObject(item, "ipv4Index", sm_policy_dnn_data->ipv4_index) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [ipv4_index]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_ipv6_index) {
    if (cJSON_AddNumberToObject(item, "ipv6Index", sm_policy_dnn_data->ipv6_index) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [ipv6_index]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_offline) {
    if (cJSON_AddBoolToObject(item, "offline", sm_policy_dnn_data->offline) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [offline]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_online) {
    if (cJSON_AddBoolToObject(item, "online", sm_policy_dnn_data->online) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [online]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->chf_info) {
    cJSON *chf_info_local_JSON = OpenAPI_charging_information_convertToJSON(sm_policy_dnn_data->chf_info);
    if (chf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [chf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "chfInfo", chf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [chf_info]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->ref_um_data_limit_ids) {
    cJSON *ref_um_data_limit_ids = cJSON_AddObjectToObject(item, "refUmDataLimitIds");
    if (ref_um_data_limit_ids == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [ref_um_data_limit_ids]");
        goto end;
    }
    cJSON *localMapObject = ref_um_data_limit_ids;
    OpenAPI_lnode_t *ref_um_data_limit_ids_node;
    if (sm_policy_dnn_data->ref_um_data_limit_ids) {
        OpenAPI_list_for_each(sm_policy_dnn_data->ref_um_data_limit_ids, ref_um_data_limit_ids_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)ref_um_data_limit_ids_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_limit_id_to_monitoring_key_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [ref_um_data_limit_ids]");
            goto end;
        }
        cJSON_AddItemToObject(ref_um_data_limit_ids, localKeyValue->key, itemLocal);
            }
        }
    }

    if (sm_policy_dnn_data->is_mps_priority) {
    if (cJSON_AddBoolToObject(item, "mpsPriority", sm_policy_dnn_data->mps_priority) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [mps_priority]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_mcs_priority) {
    if (cJSON_AddBoolToObject(item, "mcsPriority", sm_policy_dnn_data->mcs_priority) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [mcs_priority]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_ims_signalling_prio) {
    if (cJSON_AddBoolToObject(item, "imsSignallingPrio", sm_policy_dnn_data->ims_signalling_prio) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [ims_signalling_prio]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_mps_priority_level) {
    if (cJSON_AddNumberToObject(item, "mpsPriorityLevel", sm_policy_dnn_data->mps_priority_level) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [mps_priority_level]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->is_mcs_priority_level) {
    if (cJSON_AddNumberToObject(item, "mcsPriorityLevel", sm_policy_dnn_data->mcs_priority_level) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [mcs_priority_level]");
        goto end;
    }
    }

    if (sm_policy_dnn_data->pra_infos) {
    cJSON *pra_infos = cJSON_AddObjectToObject(item, "praInfos");
    if (pra_infos == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [pra_infos]");
        goto end;
    }
    cJSON *localMapObject = pra_infos;
    OpenAPI_lnode_t *pra_infos_node;
    if (sm_policy_dnn_data->pra_infos) {
        OpenAPI_list_for_each(sm_policy_dnn_data->pra_infos, pra_infos_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pra_infos_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [pra_infos]");
            goto end;
        }
        cJSON_AddItemToObject(pra_infos, localKeyValue->key, itemLocal);
            }
        }
    }

    if (sm_policy_dnn_data->bdt_ref_ids) {
    cJSON *bdt_ref_ids = cJSON_AddObjectToObject(item, "bdtRefIds");
    if (bdt_ref_ids == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [bdt_ref_ids]");
        goto end;
    }
    cJSON *localMapObject = bdt_ref_ids;
    OpenAPI_lnode_t *bdt_ref_ids_node;
    if (sm_policy_dnn_data->bdt_ref_ids) {
        OpenAPI_list_for_each(sm_policy_dnn_data->bdt_ref_ids, bdt_ref_ids_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)bdt_ref_ids_node->data;
            }
        }
    }

    if (sm_policy_dnn_data->is_loc_rout_not_allowed) {
    if (cJSON_AddBoolToObject(item, "locRoutNotAllowed", sm_policy_dnn_data->loc_rout_not_allowed) == NULL) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed [loc_rout_not_allowed]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_policy_dnn_data_t *OpenAPI_sm_policy_dnn_data_parseFromJSON(cJSON *sm_policy_dnn_dataJSON)
{
    OpenAPI_sm_policy_dnn_data_t *sm_policy_dnn_data_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *allowed_services = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "allowedServices");

    OpenAPI_list_t *allowed_servicesList;
    if (allowed_services) {
    cJSON *allowed_services_local;
    if (!cJSON_IsArray(allowed_services)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [allowed_services]");
        goto end;
    }
    allowed_servicesList = OpenAPI_list_create();

    cJSON_ArrayForEach(allowed_services_local, allowed_services) {
    if (!cJSON_IsString(allowed_services_local)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [allowed_services]");
        goto end;
    }
    OpenAPI_list_add(allowed_servicesList , ogs_strdup(allowed_services_local->valuestring));
    }
    }

    cJSON *subsc_cats = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "subscCats");

    OpenAPI_list_t *subsc_catsList;
    if (subsc_cats) {
    cJSON *subsc_cats_local;
    if (!cJSON_IsArray(subsc_cats)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [subsc_cats]");
        goto end;
    }
    subsc_catsList = OpenAPI_list_create();

    cJSON_ArrayForEach(subsc_cats_local, subsc_cats) {
    if (!cJSON_IsString(subsc_cats_local)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [subsc_cats]");
        goto end;
    }
    OpenAPI_list_add(subsc_catsList , ogs_strdup(subsc_cats_local->valuestring));
    }
    }

    cJSON *gbr_ul = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "gbrUl");

    if (gbr_ul) {
    if (!cJSON_IsString(gbr_ul)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [gbr_ul]");
        goto end;
    }
    }

    cJSON *gbr_dl = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "gbrDl");

    if (gbr_dl) {
    if (!cJSON_IsString(gbr_dl)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [gbr_dl]");
        goto end;
    }
    }

    cJSON *adc_support = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "adcSupport");

    if (adc_support) {
    if (!cJSON_IsBool(adc_support)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [adc_support]");
        goto end;
    }
    }

    cJSON *subsc_spending_limits = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "subscSpendingLimits");

    if (subsc_spending_limits) {
    if (!cJSON_IsBool(subsc_spending_limits)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [subsc_spending_limits]");
        goto end;
    }
    }

    cJSON *ipv4_index = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "ipv4Index");

    if (ipv4_index) {
    if (!cJSON_IsNumber(ipv4_index)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [ipv4_index]");
        goto end;
    }
    }

    cJSON *ipv6_index = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "ipv6Index");

    if (ipv6_index) {
    if (!cJSON_IsNumber(ipv6_index)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [ipv6_index]");
        goto end;
    }
    }

    cJSON *offline = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "offline");

    if (offline) {
    if (!cJSON_IsBool(offline)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [offline]");
        goto end;
    }
    }

    cJSON *online = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "online");

    if (online) {
    if (!cJSON_IsBool(online)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [online]");
        goto end;
    }
    }

    cJSON *chf_info = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "chfInfo");

    OpenAPI_charging_information_t *chf_info_local_nonprim = NULL;
    if (chf_info) {
    chf_info_local_nonprim = OpenAPI_charging_information_parseFromJSON(chf_info);
    }

    cJSON *ref_um_data_limit_ids = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "refUmDataLimitIds");

    OpenAPI_list_t *ref_um_data_limit_idsList;
    if (ref_um_data_limit_ids) {
    cJSON *ref_um_data_limit_ids_local_map;
    if (!cJSON_IsObject(ref_um_data_limit_ids)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [ref_um_data_limit_ids]");
        goto end;
    }
    ref_um_data_limit_idsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(ref_um_data_limit_ids_local_map, ref_um_data_limit_ids) {
        cJSON *localMapObject = ref_um_data_limit_ids_local_map;
        if (cJSON_IsObject(ref_um_data_limit_ids_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_limit_id_to_monitoring_key_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(ref_um_data_limit_ids_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [ref_um_data_limit_ids]");
            goto end;
        }
        OpenAPI_list_add(ref_um_data_limit_idsList , localMapKeyPair);
    }
    }

    cJSON *mps_priority = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "mpsPriority");

    if (mps_priority) {
    if (!cJSON_IsBool(mps_priority)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [mps_priority]");
        goto end;
    }
    }

    cJSON *mcs_priority = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "mcsPriority");

    if (mcs_priority) {
    if (!cJSON_IsBool(mcs_priority)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [mcs_priority]");
        goto end;
    }
    }

    cJSON *ims_signalling_prio = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "imsSignallingPrio");

    if (ims_signalling_prio) {
    if (!cJSON_IsBool(ims_signalling_prio)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [ims_signalling_prio]");
        goto end;
    }
    }

    cJSON *mps_priority_level = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "mpsPriorityLevel");

    if (mps_priority_level) {
    if (!cJSON_IsNumber(mps_priority_level)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [mps_priority_level]");
        goto end;
    }
    }

    cJSON *mcs_priority_level = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "mcsPriorityLevel");

    if (mcs_priority_level) {
    if (!cJSON_IsNumber(mcs_priority_level)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [mcs_priority_level]");
        goto end;
    }
    }

    cJSON *pra_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "praInfos");

    OpenAPI_list_t *pra_infosList;
    if (pra_infos) {
    cJSON *pra_infos_local_map;
    if (!cJSON_IsObject(pra_infos)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [pra_infos]");
        goto end;
    }
    pra_infosList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(pra_infos_local_map, pra_infos) {
        cJSON *localMapObject = pra_infos_local_map;
        if (cJSON_IsObject(pra_infos_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(pra_infos_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [pra_infos]");
            goto end;
        }
        OpenAPI_list_add(pra_infosList , localMapKeyPair);
    }
    }

    cJSON *bdt_ref_ids = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "bdtRefIds");

    OpenAPI_list_t *bdt_ref_idsList;
    if (bdt_ref_ids) {
    cJSON *bdt_ref_ids_local_map;
    if (!cJSON_IsObject(bdt_ref_ids)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [bdt_ref_ids]");
        goto end;
    }
    bdt_ref_idsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(bdt_ref_ids_local_map, bdt_ref_ids) {
        cJSON *localMapObject = bdt_ref_ids_local_map;
        OpenAPI_list_add(bdt_ref_idsList , localMapKeyPair);
    }
    }

    cJSON *loc_rout_not_allowed = cJSON_GetObjectItemCaseSensitive(sm_policy_dnn_dataJSON, "locRoutNotAllowed");

    if (loc_rout_not_allowed) {
    if (!cJSON_IsBool(loc_rout_not_allowed)) {
        ogs_error("OpenAPI_sm_policy_dnn_data_parseFromJSON() failed [loc_rout_not_allowed]");
        goto end;
    }
    }

    sm_policy_dnn_data_local_var = OpenAPI_sm_policy_dnn_data_create (
        ogs_strdup(dnn->valuestring),
        allowed_services ? allowed_servicesList : NULL,
        subsc_cats ? subsc_catsList : NULL,
        gbr_ul ? ogs_strdup(gbr_ul->valuestring) : NULL,
        gbr_dl ? ogs_strdup(gbr_dl->valuestring) : NULL,
        adc_support ? true : false,
        adc_support ? adc_support->valueint : 0,
        subsc_spending_limits ? true : false,
        subsc_spending_limits ? subsc_spending_limits->valueint : 0,
        ipv4_index ? true : false,
        ipv4_index ? ipv4_index->valuedouble : 0,
        ipv6_index ? true : false,
        ipv6_index ? ipv6_index->valuedouble : 0,
        offline ? true : false,
        offline ? offline->valueint : 0,
        online ? true : false,
        online ? online->valueint : 0,
        chf_info ? chf_info_local_nonprim : NULL,
        ref_um_data_limit_ids ? ref_um_data_limit_idsList : NULL,
        mps_priority ? true : false,
        mps_priority ? mps_priority->valueint : 0,
        mcs_priority ? true : false,
        mcs_priority ? mcs_priority->valueint : 0,
        ims_signalling_prio ? true : false,
        ims_signalling_prio ? ims_signalling_prio->valueint : 0,
        mps_priority_level ? true : false,
        mps_priority_level ? mps_priority_level->valuedouble : 0,
        mcs_priority_level ? true : false,
        mcs_priority_level ? mcs_priority_level->valuedouble : 0,
        pra_infos ? pra_infosList : NULL,
        bdt_ref_ids ? bdt_ref_idsList : NULL,
        loc_rout_not_allowed ? true : false,
        loc_rout_not_allowed ? loc_rout_not_allowed->valueint : 0
    );

    return sm_policy_dnn_data_local_var;
end:
    return NULL;
}

OpenAPI_sm_policy_dnn_data_t *OpenAPI_sm_policy_dnn_data_copy(OpenAPI_sm_policy_dnn_data_t *dst, OpenAPI_sm_policy_dnn_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_dnn_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_dnn_data_convertToJSON() failed");
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

    OpenAPI_sm_policy_dnn_data_free(dst);
    dst = OpenAPI_sm_policy_dnn_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

