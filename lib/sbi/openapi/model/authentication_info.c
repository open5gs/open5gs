
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authentication_info.h"

OpenAPI_authentication_info_t *OpenAPI_authentication_info_create(
    char *supi_or_suci,
    char *serving_network_name,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    char *pei,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *udm_group_id,
    char *routing_indicator,
    OpenAPI_list_t *cell_cag_info,
    bool is_n5gc_ind,
    int n5gc_ind,
    char *supported_features,
    bool is_nswo_ind,
    int nswo_ind,
    bool is_disaster_roaming_ind,
    int disaster_roaming_ind,
    bool is_onboarding_ind,
    int onboarding_ind
)
{
    OpenAPI_authentication_info_t *authentication_info_local_var = ogs_malloc(sizeof(OpenAPI_authentication_info_t));
    ogs_assert(authentication_info_local_var);

    authentication_info_local_var->supi_or_suci = supi_or_suci;
    authentication_info_local_var->serving_network_name = serving_network_name;
    authentication_info_local_var->resynchronization_info = resynchronization_info;
    authentication_info_local_var->pei = pei;
    authentication_info_local_var->is_trace_data_null = is_trace_data_null;
    authentication_info_local_var->trace_data = trace_data;
    authentication_info_local_var->udm_group_id = udm_group_id;
    authentication_info_local_var->routing_indicator = routing_indicator;
    authentication_info_local_var->cell_cag_info = cell_cag_info;
    authentication_info_local_var->is_n5gc_ind = is_n5gc_ind;
    authentication_info_local_var->n5gc_ind = n5gc_ind;
    authentication_info_local_var->supported_features = supported_features;
    authentication_info_local_var->is_nswo_ind = is_nswo_ind;
    authentication_info_local_var->nswo_ind = nswo_ind;
    authentication_info_local_var->is_disaster_roaming_ind = is_disaster_roaming_ind;
    authentication_info_local_var->disaster_roaming_ind = disaster_roaming_ind;
    authentication_info_local_var->is_onboarding_ind = is_onboarding_ind;
    authentication_info_local_var->onboarding_ind = onboarding_ind;

    return authentication_info_local_var;
}

void OpenAPI_authentication_info_free(OpenAPI_authentication_info_t *authentication_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == authentication_info) {
        return;
    }
    if (authentication_info->supi_or_suci) {
        ogs_free(authentication_info->supi_or_suci);
        authentication_info->supi_or_suci = NULL;
    }
    if (authentication_info->serving_network_name) {
        ogs_free(authentication_info->serving_network_name);
        authentication_info->serving_network_name = NULL;
    }
    if (authentication_info->resynchronization_info) {
        OpenAPI_resynchronization_info_free(authentication_info->resynchronization_info);
        authentication_info->resynchronization_info = NULL;
    }
    if (authentication_info->pei) {
        ogs_free(authentication_info->pei);
        authentication_info->pei = NULL;
    }
    if (authentication_info->trace_data) {
        OpenAPI_trace_data_free(authentication_info->trace_data);
        authentication_info->trace_data = NULL;
    }
    if (authentication_info->udm_group_id) {
        ogs_free(authentication_info->udm_group_id);
        authentication_info->udm_group_id = NULL;
    }
    if (authentication_info->routing_indicator) {
        ogs_free(authentication_info->routing_indicator);
        authentication_info->routing_indicator = NULL;
    }
    if (authentication_info->cell_cag_info) {
        OpenAPI_list_for_each(authentication_info->cell_cag_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(authentication_info->cell_cag_info);
        authentication_info->cell_cag_info = NULL;
    }
    if (authentication_info->supported_features) {
        ogs_free(authentication_info->supported_features);
        authentication_info->supported_features = NULL;
    }
    ogs_free(authentication_info);
}

cJSON *OpenAPI_authentication_info_convertToJSON(OpenAPI_authentication_info_t *authentication_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (authentication_info == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [AuthenticationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!authentication_info->supi_or_suci) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [supi_or_suci]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supiOrSuci", authentication_info->supi_or_suci) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [supi_or_suci]");
        goto end;
    }

    if (!authentication_info->serving_network_name) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [serving_network_name]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "servingNetworkName", authentication_info->serving_network_name) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [serving_network_name]");
        goto end;
    }

    if (authentication_info->resynchronization_info) {
    cJSON *resynchronization_info_local_JSON = OpenAPI_resynchronization_info_convertToJSON(authentication_info->resynchronization_info);
    if (resynchronization_info_local_JSON == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [resynchronization_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "resynchronizationInfo", resynchronization_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [resynchronization_info]");
        goto end;
    }
    }

    if (authentication_info->pei) {
    if (cJSON_AddStringToObject(item, "pei", authentication_info->pei) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (authentication_info->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(authentication_info->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (authentication_info->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (authentication_info->udm_group_id) {
    if (cJSON_AddStringToObject(item, "udmGroupId", authentication_info->udm_group_id) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [udm_group_id]");
        goto end;
    }
    }

    if (authentication_info->routing_indicator) {
    if (cJSON_AddStringToObject(item, "routingIndicator", authentication_info->routing_indicator) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [routing_indicator]");
        goto end;
    }
    }

    if (authentication_info->cell_cag_info) {
    cJSON *cell_cag_infoList = cJSON_AddArrayToObject(item, "cellCagInfo");
    if (cell_cag_infoList == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [cell_cag_info]");
        goto end;
    }
    OpenAPI_list_for_each(authentication_info->cell_cag_info, node) {
        if (cJSON_AddStringToObject(cell_cag_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_authentication_info_convertToJSON() failed [cell_cag_info]");
            goto end;
        }
    }
    }

    if (authentication_info->is_n5gc_ind) {
    if (cJSON_AddBoolToObject(item, "n5gcInd", authentication_info->n5gc_ind) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [n5gc_ind]");
        goto end;
    }
    }

    if (authentication_info->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", authentication_info->supported_features) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (authentication_info->is_nswo_ind) {
    if (cJSON_AddBoolToObject(item, "nswoInd", authentication_info->nswo_ind) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [nswo_ind]");
        goto end;
    }
    }

    if (authentication_info->is_disaster_roaming_ind) {
    if (cJSON_AddBoolToObject(item, "disasterRoamingInd", authentication_info->disaster_roaming_ind) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    if (authentication_info->is_onboarding_ind) {
    if (cJSON_AddBoolToObject(item, "onboardingInd", authentication_info->onboarding_ind) == NULL) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed [onboarding_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_authentication_info_t *OpenAPI_authentication_info_parseFromJSON(cJSON *authentication_infoJSON)
{
    OpenAPI_authentication_info_t *authentication_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi_or_suci = NULL;
    cJSON *serving_network_name = NULL;
    cJSON *resynchronization_info = NULL;
    OpenAPI_resynchronization_info_t *resynchronization_info_local_nonprim = NULL;
    cJSON *pei = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *udm_group_id = NULL;
    cJSON *routing_indicator = NULL;
    cJSON *cell_cag_info = NULL;
    OpenAPI_list_t *cell_cag_infoList = NULL;
    cJSON *n5gc_ind = NULL;
    cJSON *supported_features = NULL;
    cJSON *nswo_ind = NULL;
    cJSON *disaster_roaming_ind = NULL;
    cJSON *onboarding_ind = NULL;
    supi_or_suci = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "supiOrSuci");
    if (!supi_or_suci) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [supi_or_suci]");
        goto end;
    }
    if (!cJSON_IsString(supi_or_suci)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [supi_or_suci]");
        goto end;
    }

    serving_network_name = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "servingNetworkName");
    if (!serving_network_name) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [serving_network_name]");
        goto end;
    }
    if (!cJSON_IsString(serving_network_name)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [serving_network_name]");
        goto end;
    }

    resynchronization_info = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "resynchronizationInfo");
    if (resynchronization_info) {
    resynchronization_info_local_nonprim = OpenAPI_resynchronization_info_parseFromJSON(resynchronization_info);
    if (!resynchronization_info_local_nonprim) {
        ogs_error("OpenAPI_resynchronization_info_parseFromJSON failed [resynchronization_info]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    udm_group_id = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "udmGroupId");
    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id) && !cJSON_IsNull(udm_group_id)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    routing_indicator = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "routingIndicator");
    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator) && !cJSON_IsNull(routing_indicator)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    cell_cag_info = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "cellCagInfo");
    if (cell_cag_info) {
        cJSON *cell_cag_info_local = NULL;
        if (!cJSON_IsArray(cell_cag_info)) {
            ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [cell_cag_info]");
            goto end;
        }

        cell_cag_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(cell_cag_info_local, cell_cag_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(cell_cag_info_local)) {
                ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [cell_cag_info]");
                goto end;
            }
            OpenAPI_list_add(cell_cag_infoList, ogs_strdup(cell_cag_info_local->valuestring));
        }
    }

    n5gc_ind = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "n5gcInd");
    if (n5gc_ind) {
    if (!cJSON_IsBool(n5gc_ind)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [n5gc_ind]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    nswo_ind = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "nswoInd");
    if (nswo_ind) {
    if (!cJSON_IsBool(nswo_ind)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [nswo_ind]");
        goto end;
    }
    }

    disaster_roaming_ind = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "disasterRoamingInd");
    if (disaster_roaming_ind) {
    if (!cJSON_IsBool(disaster_roaming_ind)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    onboarding_ind = cJSON_GetObjectItemCaseSensitive(authentication_infoJSON, "onboardingInd");
    if (onboarding_ind) {
    if (!cJSON_IsBool(onboarding_ind)) {
        ogs_error("OpenAPI_authentication_info_parseFromJSON() failed [onboarding_ind]");
        goto end;
    }
    }

    authentication_info_local_var = OpenAPI_authentication_info_create (
        ogs_strdup(supi_or_suci->valuestring),
        ogs_strdup(serving_network_name->valuestring),
        resynchronization_info ? resynchronization_info_local_nonprim : NULL,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL,
        udm_group_id && !cJSON_IsNull(udm_group_id) ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator && !cJSON_IsNull(routing_indicator) ? ogs_strdup(routing_indicator->valuestring) : NULL,
        cell_cag_info ? cell_cag_infoList : NULL,
        n5gc_ind ? true : false,
        n5gc_ind ? n5gc_ind->valueint : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        nswo_ind ? true : false,
        nswo_ind ? nswo_ind->valueint : 0,
        disaster_roaming_ind ? true : false,
        disaster_roaming_ind ? disaster_roaming_ind->valueint : 0,
        onboarding_ind ? true : false,
        onboarding_ind ? onboarding_ind->valueint : 0
    );

    return authentication_info_local_var;
end:
    if (resynchronization_info_local_nonprim) {
        OpenAPI_resynchronization_info_free(resynchronization_info_local_nonprim);
        resynchronization_info_local_nonprim = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    if (cell_cag_infoList) {
        OpenAPI_list_for_each(cell_cag_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cell_cag_infoList);
        cell_cag_infoList = NULL;
    }
    return NULL;
}

OpenAPI_authentication_info_t *OpenAPI_authentication_info_copy(OpenAPI_authentication_info_t *dst, OpenAPI_authentication_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authentication_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authentication_info_convertToJSON() failed");
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

    OpenAPI_authentication_info_free(dst);
    dst = OpenAPI_authentication_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

