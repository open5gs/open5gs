
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "search_result.h"

OpenAPI_search_result_t *OpenAPI_search_result_create(
    int validity_period,
    OpenAPI_list_t *nf_instances,
    OpenAPI_list_t *complete_nf_instances,
    char *search_id,
    bool is_num_nf_inst_complete,
    int num_nf_inst_complete,
    OpenAPI_preferred_search_t *preferred_search,
    char *nrf_supported_features,
    OpenAPI_list_t* nf_instance_list,
    OpenAPI_search_result_info_t *search_result_info,
    bool is_altered_priority_ind,
    int altered_priority_ind,
    OpenAPI_no_profile_match_info_t *no_profile_match_info,
    OpenAPI_list_t *ignored_query_params,
    bool is_ind_com_with_del_disc_req,
    int ind_com_with_del_disc_req,
    bool is_ind_com_wo_del_disc_req,
    int ind_com_wo_del_disc_req,
    OpenAPI_ind_com_add_info_t *ind_com_add_info
)
{
    OpenAPI_search_result_t *search_result_local_var = ogs_malloc(sizeof(OpenAPI_search_result_t));
    ogs_assert(search_result_local_var);

    search_result_local_var->validity_period = validity_period;
    search_result_local_var->nf_instances = nf_instances;
    search_result_local_var->complete_nf_instances = complete_nf_instances;
    search_result_local_var->search_id = search_id;
    search_result_local_var->is_num_nf_inst_complete = is_num_nf_inst_complete;
    search_result_local_var->num_nf_inst_complete = num_nf_inst_complete;
    search_result_local_var->preferred_search = preferred_search;
    search_result_local_var->nrf_supported_features = nrf_supported_features;
    search_result_local_var->nf_instance_list = nf_instance_list;
    search_result_local_var->search_result_info = search_result_info;
    search_result_local_var->is_altered_priority_ind = is_altered_priority_ind;
    search_result_local_var->altered_priority_ind = altered_priority_ind;
    search_result_local_var->no_profile_match_info = no_profile_match_info;
    search_result_local_var->ignored_query_params = ignored_query_params;
    search_result_local_var->is_ind_com_with_del_disc_req = is_ind_com_with_del_disc_req;
    search_result_local_var->ind_com_with_del_disc_req = ind_com_with_del_disc_req;
    search_result_local_var->is_ind_com_wo_del_disc_req = is_ind_com_wo_del_disc_req;
    search_result_local_var->ind_com_wo_del_disc_req = ind_com_wo_del_disc_req;
    search_result_local_var->ind_com_add_info = ind_com_add_info;

    return search_result_local_var;
}

void OpenAPI_search_result_free(OpenAPI_search_result_t *search_result)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == search_result) {
        return;
    }
    if (search_result->nf_instances) {
        OpenAPI_list_for_each(search_result->nf_instances, node) {
            OpenAPI_nf_profile_free(node->data);
        }
        OpenAPI_list_free(search_result->nf_instances);
        search_result->nf_instances = NULL;
    }
    if (search_result->complete_nf_instances) {
        OpenAPI_list_for_each(search_result->complete_nf_instances, node) {
            OpenAPI_nf_profile_free(node->data);
        }
        OpenAPI_list_free(search_result->complete_nf_instances);
        search_result->complete_nf_instances = NULL;
    }
    if (search_result->search_id) {
        ogs_free(search_result->search_id);
        search_result->search_id = NULL;
    }
    if (search_result->preferred_search) {
        OpenAPI_preferred_search_free(search_result->preferred_search);
        search_result->preferred_search = NULL;
    }
    if (search_result->nrf_supported_features) {
        ogs_free(search_result->nrf_supported_features);
        search_result->nrf_supported_features = NULL;
    }
    if (search_result->nf_instance_list) {
        OpenAPI_list_for_each(search_result->nf_instance_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nf_instance_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(search_result->nf_instance_list);
        search_result->nf_instance_list = NULL;
    }
    if (search_result->search_result_info) {
        OpenAPI_search_result_info_free(search_result->search_result_info);
        search_result->search_result_info = NULL;
    }
    if (search_result->no_profile_match_info) {
        OpenAPI_no_profile_match_info_free(search_result->no_profile_match_info);
        search_result->no_profile_match_info = NULL;
    }
    if (search_result->ignored_query_params) {
        OpenAPI_list_for_each(search_result->ignored_query_params, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(search_result->ignored_query_params);
        search_result->ignored_query_params = NULL;
    }
    if (search_result->ind_com_add_info) {
        OpenAPI_ind_com_add_info_free(search_result->ind_com_add_info);
        search_result->ind_com_add_info = NULL;
    }
    ogs_free(search_result);
}

cJSON *OpenAPI_search_result_convertToJSON(OpenAPI_search_result_t *search_result)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (search_result == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [SearchResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "validityPeriod", search_result->validity_period) == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [validity_period]");
        goto end;
    }

    if (!search_result->nf_instances) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [nf_instances]");
        return NULL;
    }
    cJSON *nf_instancesList = cJSON_AddArrayToObject(item, "nfInstances");
    if (nf_instancesList == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [nf_instances]");
        goto end;
    }
    OpenAPI_list_for_each(search_result->nf_instances, node) {
        cJSON *itemLocal = OpenAPI_nf_profile_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_search_result_convertToJSON() failed [nf_instances]");
            goto end;
        }
        cJSON_AddItemToArray(nf_instancesList, itemLocal);
    }

    if (search_result->complete_nf_instances) {
    cJSON *complete_nf_instancesList = cJSON_AddArrayToObject(item, "completeNfInstances");
    if (complete_nf_instancesList == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [complete_nf_instances]");
        goto end;
    }
    OpenAPI_list_for_each(search_result->complete_nf_instances, node) {
        cJSON *itemLocal = OpenAPI_nf_profile_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_search_result_convertToJSON() failed [complete_nf_instances]");
            goto end;
        }
        cJSON_AddItemToArray(complete_nf_instancesList, itemLocal);
    }
    }

    if (search_result->search_id) {
    if (cJSON_AddStringToObject(item, "searchId", search_result->search_id) == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [search_id]");
        goto end;
    }
    }

    if (search_result->is_num_nf_inst_complete) {
    if (cJSON_AddNumberToObject(item, "numNfInstComplete", search_result->num_nf_inst_complete) == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [num_nf_inst_complete]");
        goto end;
    }
    }

    if (search_result->preferred_search) {
    cJSON *preferred_search_local_JSON = OpenAPI_preferred_search_convertToJSON(search_result->preferred_search);
    if (preferred_search_local_JSON == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [preferred_search]");
        goto end;
    }
    cJSON_AddItemToObject(item, "preferredSearch", preferred_search_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [preferred_search]");
        goto end;
    }
    }

    if (search_result->nrf_supported_features) {
    if (cJSON_AddStringToObject(item, "nrfSupportedFeatures", search_result->nrf_supported_features) == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [nrf_supported_features]");
        goto end;
    }
    }

    if (search_result->nf_instance_list) {
    cJSON *nf_instance_list = cJSON_AddObjectToObject(item, "nfInstanceList");
    if (nf_instance_list == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [nf_instance_list]");
        goto end;
    }
    cJSON *localMapObject = nf_instance_list;
    if (search_result->nf_instance_list) {
        OpenAPI_list_for_each(search_result->nf_instance_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_search_result_convertToJSON() failed [nf_instance_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_search_result_convertToJSON() failed [nf_instance_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nf_instance_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_search_result_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (search_result->search_result_info) {
    cJSON *search_result_info_local_JSON = OpenAPI_search_result_info_convertToJSON(search_result->search_result_info);
    if (search_result_info_local_JSON == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [search_result_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "searchResultInfo", search_result_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [search_result_info]");
        goto end;
    }
    }

    if (search_result->is_altered_priority_ind) {
    if (cJSON_AddBoolToObject(item, "alteredPriorityInd", search_result->altered_priority_ind) == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [altered_priority_ind]");
        goto end;
    }
    }

    if (search_result->no_profile_match_info) {
    cJSON *no_profile_match_info_local_JSON = OpenAPI_no_profile_match_info_convertToJSON(search_result->no_profile_match_info);
    if (no_profile_match_info_local_JSON == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [no_profile_match_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "noProfileMatchInfo", no_profile_match_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [no_profile_match_info]");
        goto end;
    }
    }

    if (search_result->ignored_query_params) {
    cJSON *ignored_query_paramsList = cJSON_AddArrayToObject(item, "ignoredQueryParams");
    if (ignored_query_paramsList == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [ignored_query_params]");
        goto end;
    }
    OpenAPI_list_for_each(search_result->ignored_query_params, node) {
        if (cJSON_AddStringToObject(ignored_query_paramsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_search_result_convertToJSON() failed [ignored_query_params]");
            goto end;
        }
    }
    }

    if (search_result->is_ind_com_with_del_disc_req) {
    if (cJSON_AddBoolToObject(item, "indComWithDelDiscReq", search_result->ind_com_with_del_disc_req) == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [ind_com_with_del_disc_req]");
        goto end;
    }
    }

    if (search_result->is_ind_com_wo_del_disc_req) {
    if (cJSON_AddBoolToObject(item, "indComWoDelDiscReq", search_result->ind_com_wo_del_disc_req) == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [ind_com_wo_del_disc_req]");
        goto end;
    }
    }

    if (search_result->ind_com_add_info) {
    cJSON *ind_com_add_info_local_JSON = OpenAPI_ind_com_add_info_convertToJSON(search_result->ind_com_add_info);
    if (ind_com_add_info_local_JSON == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [ind_com_add_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "indComAddInfo", ind_com_add_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [ind_com_add_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_search_result_t *OpenAPI_search_result_parseFromJSON(cJSON *search_resultJSON)
{
    OpenAPI_search_result_t *search_result_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *validity_period = NULL;
    cJSON *nf_instances = NULL;
    OpenAPI_list_t *nf_instancesList = NULL;
    cJSON *complete_nf_instances = NULL;
    OpenAPI_list_t *complete_nf_instancesList = NULL;
    cJSON *search_id = NULL;
    cJSON *num_nf_inst_complete = NULL;
    cJSON *preferred_search = NULL;
    OpenAPI_preferred_search_t *preferred_search_local_nonprim = NULL;
    cJSON *nrf_supported_features = NULL;
    cJSON *nf_instance_list = NULL;
    OpenAPI_list_t *nf_instance_listList = NULL;
    cJSON *search_result_info = NULL;
    OpenAPI_search_result_info_t *search_result_info_local_nonprim = NULL;
    cJSON *altered_priority_ind = NULL;
    cJSON *no_profile_match_info = NULL;
    OpenAPI_no_profile_match_info_t *no_profile_match_info_local_nonprim = NULL;
    cJSON *ignored_query_params = NULL;
    OpenAPI_list_t *ignored_query_paramsList = NULL;
    cJSON *ind_com_with_del_disc_req = NULL;
    cJSON *ind_com_wo_del_disc_req = NULL;
    cJSON *ind_com_add_info = NULL;
    OpenAPI_ind_com_add_info_t *ind_com_add_info_local_nonprim = NULL;
    validity_period = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "validityPeriod");
    if (!validity_period) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [validity_period]");
        goto end;
    }
    if (!cJSON_IsNumber(validity_period)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [validity_period]");
        goto end;
    }

    nf_instances = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "nfInstances");
    if (!nf_instances) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [nf_instances]");
        goto end;
    }
        cJSON *nf_instances_local = NULL;
        if (!cJSON_IsArray(nf_instances)) {
            ogs_error("OpenAPI_search_result_parseFromJSON() failed [nf_instances]");
            goto end;
        }

        nf_instancesList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_instances_local, nf_instances) {
            if (!cJSON_IsObject(nf_instances_local)) {
                ogs_error("OpenAPI_search_result_parseFromJSON() failed [nf_instances]");
                goto end;
            }
            OpenAPI_nf_profile_t *nf_instancesItem = OpenAPI_nf_profile_parseFromJSON(nf_instances_local);
            if (!nf_instancesItem) {
                ogs_error("No nf_instancesItem");
                goto end;
            }
            OpenAPI_list_add(nf_instancesList, nf_instancesItem);
        }

    complete_nf_instances = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "completeNfInstances");
    if (complete_nf_instances) {
        cJSON *complete_nf_instances_local = NULL;
        if (!cJSON_IsArray(complete_nf_instances)) {
            ogs_error("OpenAPI_search_result_parseFromJSON() failed [complete_nf_instances]");
            goto end;
        }

        complete_nf_instancesList = OpenAPI_list_create();

        cJSON_ArrayForEach(complete_nf_instances_local, complete_nf_instances) {
            if (!cJSON_IsObject(complete_nf_instances_local)) {
                ogs_error("OpenAPI_search_result_parseFromJSON() failed [complete_nf_instances]");
                goto end;
            }
            OpenAPI_nf_profile_t *complete_nf_instancesItem = OpenAPI_nf_profile_parseFromJSON(complete_nf_instances_local);
            if (!complete_nf_instancesItem) {
                ogs_error("No complete_nf_instancesItem");
                goto end;
            }
            OpenAPI_list_add(complete_nf_instancesList, complete_nf_instancesItem);
        }
    }

    search_id = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "searchId");
    if (search_id) {
    if (!cJSON_IsString(search_id) && !cJSON_IsNull(search_id)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [search_id]");
        goto end;
    }
    }

    num_nf_inst_complete = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "numNfInstComplete");
    if (num_nf_inst_complete) {
    if (!cJSON_IsNumber(num_nf_inst_complete)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [num_nf_inst_complete]");
        goto end;
    }
    }

    preferred_search = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "preferredSearch");
    if (preferred_search) {
    preferred_search_local_nonprim = OpenAPI_preferred_search_parseFromJSON(preferred_search);
    if (!preferred_search_local_nonprim) {
        ogs_error("OpenAPI_preferred_search_parseFromJSON failed [preferred_search]");
        goto end;
    }
    }

    nrf_supported_features = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "nrfSupportedFeatures");
    if (nrf_supported_features) {
    if (!cJSON_IsString(nrf_supported_features) && !cJSON_IsNull(nrf_supported_features)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [nrf_supported_features]");
        goto end;
    }
    }

    nf_instance_list = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "nfInstanceList");
    if (nf_instance_list) {
        cJSON *nf_instance_list_local_map = NULL;
        if (!cJSON_IsObject(nf_instance_list) && !cJSON_IsNull(nf_instance_list)) {
            ogs_error("OpenAPI_search_result_parseFromJSON() failed [nf_instance_list]");
            goto end;
        }
        if (cJSON_IsObject(nf_instance_list)) {
            nf_instance_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(nf_instance_list_local_map, nf_instance_list) {
                cJSON *localMapObject = nf_instance_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nf_instance_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_search_result_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(nf_instance_listList, localMapKeyPair);
            }
        }
    }

    search_result_info = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "searchResultInfo");
    if (search_result_info) {
    search_result_info_local_nonprim = OpenAPI_search_result_info_parseFromJSON(search_result_info);
    if (!search_result_info_local_nonprim) {
        ogs_error("OpenAPI_search_result_info_parseFromJSON failed [search_result_info]");
        goto end;
    }
    }

    altered_priority_ind = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "alteredPriorityInd");
    if (altered_priority_ind) {
    if (!cJSON_IsBool(altered_priority_ind)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [altered_priority_ind]");
        goto end;
    }
    }

    no_profile_match_info = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "noProfileMatchInfo");
    if (no_profile_match_info) {
    no_profile_match_info_local_nonprim = OpenAPI_no_profile_match_info_parseFromJSON(no_profile_match_info);
    if (!no_profile_match_info_local_nonprim) {
        ogs_error("OpenAPI_no_profile_match_info_parseFromJSON failed [no_profile_match_info]");
        goto end;
    }
    }

    ignored_query_params = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "ignoredQueryParams");
    if (ignored_query_params) {
        cJSON *ignored_query_params_local = NULL;
        if (!cJSON_IsArray(ignored_query_params)) {
            ogs_error("OpenAPI_search_result_parseFromJSON() failed [ignored_query_params]");
            goto end;
        }

        ignored_query_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ignored_query_params_local, ignored_query_params) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ignored_query_params_local)) {
                ogs_error("OpenAPI_search_result_parseFromJSON() failed [ignored_query_params]");
                goto end;
            }
            OpenAPI_list_add(ignored_query_paramsList, ogs_strdup(ignored_query_params_local->valuestring));
        }
    }

    ind_com_with_del_disc_req = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "indComWithDelDiscReq");
    if (ind_com_with_del_disc_req) {
    if (!cJSON_IsBool(ind_com_with_del_disc_req)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [ind_com_with_del_disc_req]");
        goto end;
    }
    }

    ind_com_wo_del_disc_req = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "indComWoDelDiscReq");
    if (ind_com_wo_del_disc_req) {
    if (!cJSON_IsBool(ind_com_wo_del_disc_req)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [ind_com_wo_del_disc_req]");
        goto end;
    }
    }

    ind_com_add_info = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "indComAddInfo");
    if (ind_com_add_info) {
    ind_com_add_info_local_nonprim = OpenAPI_ind_com_add_info_parseFromJSON(ind_com_add_info);
    if (!ind_com_add_info_local_nonprim) {
        ogs_error("OpenAPI_ind_com_add_info_parseFromJSON failed [ind_com_add_info]");
        goto end;
    }
    }

    search_result_local_var = OpenAPI_search_result_create (
        
        validity_period->valuedouble,
        nf_instancesList,
        complete_nf_instances ? complete_nf_instancesList : NULL,
        search_id && !cJSON_IsNull(search_id) ? ogs_strdup(search_id->valuestring) : NULL,
        num_nf_inst_complete ? true : false,
        num_nf_inst_complete ? num_nf_inst_complete->valuedouble : 0,
        preferred_search ? preferred_search_local_nonprim : NULL,
        nrf_supported_features && !cJSON_IsNull(nrf_supported_features) ? ogs_strdup(nrf_supported_features->valuestring) : NULL,
        nf_instance_list ? nf_instance_listList : NULL,
        search_result_info ? search_result_info_local_nonprim : NULL,
        altered_priority_ind ? true : false,
        altered_priority_ind ? altered_priority_ind->valueint : 0,
        no_profile_match_info ? no_profile_match_info_local_nonprim : NULL,
        ignored_query_params ? ignored_query_paramsList : NULL,
        ind_com_with_del_disc_req ? true : false,
        ind_com_with_del_disc_req ? ind_com_with_del_disc_req->valueint : 0,
        ind_com_wo_del_disc_req ? true : false,
        ind_com_wo_del_disc_req ? ind_com_wo_del_disc_req->valueint : 0,
        ind_com_add_info ? ind_com_add_info_local_nonprim : NULL
    );

    return search_result_local_var;
end:
    if (nf_instancesList) {
        OpenAPI_list_for_each(nf_instancesList, node) {
            OpenAPI_nf_profile_free(node->data);
        }
        OpenAPI_list_free(nf_instancesList);
        nf_instancesList = NULL;
    }
    if (complete_nf_instancesList) {
        OpenAPI_list_for_each(complete_nf_instancesList, node) {
            OpenAPI_nf_profile_free(node->data);
        }
        OpenAPI_list_free(complete_nf_instancesList);
        complete_nf_instancesList = NULL;
    }
    if (preferred_search_local_nonprim) {
        OpenAPI_preferred_search_free(preferred_search_local_nonprim);
        preferred_search_local_nonprim = NULL;
    }
    if (nf_instance_listList) {
        OpenAPI_list_for_each(nf_instance_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nf_instance_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nf_instance_listList);
        nf_instance_listList = NULL;
    }
    if (search_result_info_local_nonprim) {
        OpenAPI_search_result_info_free(search_result_info_local_nonprim);
        search_result_info_local_nonprim = NULL;
    }
    if (no_profile_match_info_local_nonprim) {
        OpenAPI_no_profile_match_info_free(no_profile_match_info_local_nonprim);
        no_profile_match_info_local_nonprim = NULL;
    }
    if (ignored_query_paramsList) {
        OpenAPI_list_for_each(ignored_query_paramsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ignored_query_paramsList);
        ignored_query_paramsList = NULL;
    }
    if (ind_com_add_info_local_nonprim) {
        OpenAPI_ind_com_add_info_free(ind_com_add_info_local_nonprim);
        ind_com_add_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_search_result_t *OpenAPI_search_result_copy(OpenAPI_search_result_t *dst, OpenAPI_search_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_search_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed");
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

    OpenAPI_search_result_free(dst);
    dst = OpenAPI_search_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

