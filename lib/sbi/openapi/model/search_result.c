
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "search_result.h"

OpenAPI_search_result_t *OpenAPI_search_result_create(
    bool is_validity_period,
    int validity_period,
    OpenAPI_list_t *nf_instances,
    char *search_id,
    bool is_num_nf_inst_complete,
    int num_nf_inst_complete,
    OpenAPI_preferred_search_t *preferred_search,
    char *nrf_supported_features
)
{
    OpenAPI_search_result_t *search_result_local_var = ogs_malloc(sizeof(OpenAPI_search_result_t));
    ogs_assert(search_result_local_var);

    search_result_local_var->is_validity_period = is_validity_period;
    search_result_local_var->validity_period = validity_period;
    search_result_local_var->nf_instances = nf_instances;
    search_result_local_var->search_id = search_id;
    search_result_local_var->is_num_nf_inst_complete = is_num_nf_inst_complete;
    search_result_local_var->num_nf_inst_complete = num_nf_inst_complete;
    search_result_local_var->preferred_search = preferred_search;
    search_result_local_var->nrf_supported_features = nrf_supported_features;

    return search_result_local_var;
}

void OpenAPI_search_result_free(OpenAPI_search_result_t *search_result)
{
    if (NULL == search_result) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(search_result->nf_instances, node) {
        OpenAPI_nf_profile_free(node->data);
    }
    OpenAPI_list_free(search_result->nf_instances);
    ogs_free(search_result->search_id);
    OpenAPI_preferred_search_free(search_result->preferred_search);
    ogs_free(search_result->nrf_supported_features);
    ogs_free(search_result);
}

cJSON *OpenAPI_search_result_convertToJSON(OpenAPI_search_result_t *search_result)
{
    cJSON *item = NULL;

    if (search_result == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [SearchResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (search_result->is_validity_period) {
    if (cJSON_AddNumberToObject(item, "validityPeriod", search_result->validity_period) == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [validity_period]");
        goto end;
    }
    }

    cJSON *nf_instancesList = cJSON_AddArrayToObject(item, "nfInstances");
    if (nf_instancesList == NULL) {
        ogs_error("OpenAPI_search_result_convertToJSON() failed [nf_instances]");
        goto end;
    }

    OpenAPI_lnode_t *nf_instances_node;
    if (search_result->nf_instances) {
        OpenAPI_list_for_each(search_result->nf_instances, nf_instances_node) {
            cJSON *itemLocal = OpenAPI_nf_profile_convertToJSON(nf_instances_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_search_result_convertToJSON() failed [nf_instances]");
                goto end;
            }
            cJSON_AddItemToArray(nf_instancesList, itemLocal);
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

end:
    return item;
}

OpenAPI_search_result_t *OpenAPI_search_result_parseFromJSON(cJSON *search_resultJSON)
{
    OpenAPI_search_result_t *search_result_local_var = NULL;
    cJSON *validity_period = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "validityPeriod");

    if (validity_period) {
    if (!cJSON_IsNumber(validity_period)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [validity_period]");
        goto end;
    }
    }

    cJSON *nf_instances = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "nfInstances");
    if (!nf_instances) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [nf_instances]");
        goto end;
    }

    OpenAPI_list_t *nf_instancesList;
    cJSON *nf_instances_local_nonprimitive;
    if (!cJSON_IsArray(nf_instances)){
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [nf_instances]");
        goto end;
    }

    nf_instancesList = OpenAPI_list_create();

    cJSON_ArrayForEach(nf_instances_local_nonprimitive, nf_instances ) {
        if (!cJSON_IsObject(nf_instances_local_nonprimitive)) {
            ogs_error("OpenAPI_search_result_parseFromJSON() failed [nf_instances]");
            goto end;
        }
        OpenAPI_nf_profile_t *nf_instancesItem = OpenAPI_nf_profile_parseFromJSON(nf_instances_local_nonprimitive);

        if (!nf_instancesItem) {
            ogs_error("No nf_instancesItem");
            OpenAPI_list_free(nf_instancesList);
            goto end;
        }

        OpenAPI_list_add(nf_instancesList, nf_instancesItem);
    }

    cJSON *search_id = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "searchId");

    if (search_id) {
    if (!cJSON_IsString(search_id)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [search_id]");
        goto end;
    }
    }

    cJSON *num_nf_inst_complete = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "numNfInstComplete");

    if (num_nf_inst_complete) {
    if (!cJSON_IsNumber(num_nf_inst_complete)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [num_nf_inst_complete]");
        goto end;
    }
    }

    cJSON *preferred_search = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "preferredSearch");

    OpenAPI_preferred_search_t *preferred_search_local_nonprim = NULL;
    if (preferred_search) {
    preferred_search_local_nonprim = OpenAPI_preferred_search_parseFromJSON(preferred_search);
    }

    cJSON *nrf_supported_features = cJSON_GetObjectItemCaseSensitive(search_resultJSON, "nrfSupportedFeatures");

    if (nrf_supported_features) {
    if (!cJSON_IsString(nrf_supported_features)) {
        ogs_error("OpenAPI_search_result_parseFromJSON() failed [nrf_supported_features]");
        goto end;
    }
    }

    search_result_local_var = OpenAPI_search_result_create (
        validity_period ? true : false,
        validity_period ? validity_period->valuedouble : 0,
        nf_instancesList,
        search_id ? ogs_strdup(search_id->valuestring) : NULL,
        num_nf_inst_complete ? true : false,
        num_nf_inst_complete ? num_nf_inst_complete->valuedouble : 0,
        preferred_search ? preferred_search_local_nonprim : NULL,
        nrf_supported_features ? ogs_strdup(nrf_supported_features->valuestring) : NULL
    );

    return search_result_local_var;
end:
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

