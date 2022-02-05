
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authorized_network_slice_info.h"

OpenAPI_authorized_network_slice_info_t *OpenAPI_authorized_network_slice_info_create(
    OpenAPI_list_t *allowed_nssai_list,
    OpenAPI_list_t *configured_nssai,
    char *target_amf_set,
    OpenAPI_list_t *candidate_amf_list,
    OpenAPI_list_t *rejected_nssai_in_plmn,
    OpenAPI_list_t *rejected_nssai_in_ta,
    OpenAPI_nsi_information_t *nsi_information,
    char *supported_features,
    char *nrf_amf_set,
    char *nrf_amf_set_nf_mgt_uri,
    char *nrf_amf_set_access_token_uri,
    char *target_amf_service_set
)
{
    OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info_local_var = ogs_malloc(sizeof(OpenAPI_authorized_network_slice_info_t));
    ogs_assert(authorized_network_slice_info_local_var);

    authorized_network_slice_info_local_var->allowed_nssai_list = allowed_nssai_list;
    authorized_network_slice_info_local_var->configured_nssai = configured_nssai;
    authorized_network_slice_info_local_var->target_amf_set = target_amf_set;
    authorized_network_slice_info_local_var->candidate_amf_list = candidate_amf_list;
    authorized_network_slice_info_local_var->rejected_nssai_in_plmn = rejected_nssai_in_plmn;
    authorized_network_slice_info_local_var->rejected_nssai_in_ta = rejected_nssai_in_ta;
    authorized_network_slice_info_local_var->nsi_information = nsi_information;
    authorized_network_slice_info_local_var->supported_features = supported_features;
    authorized_network_slice_info_local_var->nrf_amf_set = nrf_amf_set;
    authorized_network_slice_info_local_var->nrf_amf_set_nf_mgt_uri = nrf_amf_set_nf_mgt_uri;
    authorized_network_slice_info_local_var->nrf_amf_set_access_token_uri = nrf_amf_set_access_token_uri;
    authorized_network_slice_info_local_var->target_amf_service_set = target_amf_service_set;

    return authorized_network_slice_info_local_var;
}

void OpenAPI_authorized_network_slice_info_free(OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info)
{
    if (NULL == authorized_network_slice_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(authorized_network_slice_info->allowed_nssai_list, node) {
        OpenAPI_allowed_nssai_free(node->data);
    }
    OpenAPI_list_free(authorized_network_slice_info->allowed_nssai_list);
    OpenAPI_list_for_each(authorized_network_slice_info->configured_nssai, node) {
        OpenAPI_configured_snssai_free(node->data);
    }
    OpenAPI_list_free(authorized_network_slice_info->configured_nssai);
    ogs_free(authorized_network_slice_info->target_amf_set);
    OpenAPI_list_for_each(authorized_network_slice_info->candidate_amf_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(authorized_network_slice_info->candidate_amf_list);
    OpenAPI_list_for_each(authorized_network_slice_info->rejected_nssai_in_plmn, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(authorized_network_slice_info->rejected_nssai_in_plmn);
    OpenAPI_list_for_each(authorized_network_slice_info->rejected_nssai_in_ta, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(authorized_network_slice_info->rejected_nssai_in_ta);
    OpenAPI_nsi_information_free(authorized_network_slice_info->nsi_information);
    ogs_free(authorized_network_slice_info->supported_features);
    ogs_free(authorized_network_slice_info->nrf_amf_set);
    ogs_free(authorized_network_slice_info->nrf_amf_set_nf_mgt_uri);
    ogs_free(authorized_network_slice_info->nrf_amf_set_access_token_uri);
    ogs_free(authorized_network_slice_info->target_amf_service_set);
    ogs_free(authorized_network_slice_info);
}

cJSON *OpenAPI_authorized_network_slice_info_convertToJSON(OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info)
{
    cJSON *item = NULL;

    if (authorized_network_slice_info == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [AuthorizedNetworkSliceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (authorized_network_slice_info->allowed_nssai_list) {
    cJSON *allowed_nssai_listList = cJSON_AddArrayToObject(item, "allowedNssaiList");
    if (allowed_nssai_listList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [allowed_nssai_list]");
        goto end;
    }

    OpenAPI_lnode_t *allowed_nssai_list_node;
    if (authorized_network_slice_info->allowed_nssai_list) {
        OpenAPI_list_for_each(authorized_network_slice_info->allowed_nssai_list, allowed_nssai_list_node) {
            cJSON *itemLocal = OpenAPI_allowed_nssai_convertToJSON(allowed_nssai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [allowed_nssai_list]");
                goto end;
            }
            cJSON_AddItemToArray(allowed_nssai_listList, itemLocal);
        }
    }
    }

    if (authorized_network_slice_info->configured_nssai) {
    cJSON *configured_nssaiList = cJSON_AddArrayToObject(item, "configuredNssai");
    if (configured_nssaiList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [configured_nssai]");
        goto end;
    }

    OpenAPI_lnode_t *configured_nssai_node;
    if (authorized_network_slice_info->configured_nssai) {
        OpenAPI_list_for_each(authorized_network_slice_info->configured_nssai, configured_nssai_node) {
            cJSON *itemLocal = OpenAPI_configured_snssai_convertToJSON(configured_nssai_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [configured_nssai]");
                goto end;
            }
            cJSON_AddItemToArray(configured_nssaiList, itemLocal);
        }
    }
    }

    if (authorized_network_slice_info->target_amf_set) {
    if (cJSON_AddStringToObject(item, "targetAmfSet", authorized_network_slice_info->target_amf_set) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [target_amf_set]");
        goto end;
    }
    }

    if (authorized_network_slice_info->candidate_amf_list) {
    cJSON *candidate_amf_list = cJSON_AddArrayToObject(item, "candidateAmfList");
    if (candidate_amf_list == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [candidate_amf_list]");
        goto end;
    }

    OpenAPI_lnode_t *candidate_amf_list_node;
    OpenAPI_list_for_each(authorized_network_slice_info->candidate_amf_list, candidate_amf_list_node)  {
    if (cJSON_AddStringToObject(candidate_amf_list, "", (char*)candidate_amf_list_node->data) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [candidate_amf_list]");
        goto end;
    }
                    }
    }

    if (authorized_network_slice_info->rejected_nssai_in_plmn) {
    cJSON *rejected_nssai_in_plmnList = cJSON_AddArrayToObject(item, "rejectedNssaiInPlmn");
    if (rejected_nssai_in_plmnList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [rejected_nssai_in_plmn]");
        goto end;
    }

    OpenAPI_lnode_t *rejected_nssai_in_plmn_node;
    if (authorized_network_slice_info->rejected_nssai_in_plmn) {
        OpenAPI_list_for_each(authorized_network_slice_info->rejected_nssai_in_plmn, rejected_nssai_in_plmn_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(rejected_nssai_in_plmn_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [rejected_nssai_in_plmn]");
                goto end;
            }
            cJSON_AddItemToArray(rejected_nssai_in_plmnList, itemLocal);
        }
    }
    }

    if (authorized_network_slice_info->rejected_nssai_in_ta) {
    cJSON *rejected_nssai_in_taList = cJSON_AddArrayToObject(item, "rejectedNssaiInTa");
    if (rejected_nssai_in_taList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [rejected_nssai_in_ta]");
        goto end;
    }

    OpenAPI_lnode_t *rejected_nssai_in_ta_node;
    if (authorized_network_slice_info->rejected_nssai_in_ta) {
        OpenAPI_list_for_each(authorized_network_slice_info->rejected_nssai_in_ta, rejected_nssai_in_ta_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(rejected_nssai_in_ta_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [rejected_nssai_in_ta]");
                goto end;
            }
            cJSON_AddItemToArray(rejected_nssai_in_taList, itemLocal);
        }
    }
    }

    if (authorized_network_slice_info->nsi_information) {
    cJSON *nsi_information_local_JSON = OpenAPI_nsi_information_convertToJSON(authorized_network_slice_info->nsi_information);
    if (nsi_information_local_JSON == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nsi_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nsiInformation", nsi_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nsi_information]");
        goto end;
    }
    }

    if (authorized_network_slice_info->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", authorized_network_slice_info->supported_features) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (authorized_network_slice_info->nrf_amf_set) {
    if (cJSON_AddStringToObject(item, "nrfAmfSet", authorized_network_slice_info->nrf_amf_set) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nrf_amf_set]");
        goto end;
    }
    }

    if (authorized_network_slice_info->nrf_amf_set_nf_mgt_uri) {
    if (cJSON_AddStringToObject(item, "nrfAmfSetNfMgtUri", authorized_network_slice_info->nrf_amf_set_nf_mgt_uri) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nrf_amf_set_nf_mgt_uri]");
        goto end;
    }
    }

    if (authorized_network_slice_info->nrf_amf_set_access_token_uri) {
    if (cJSON_AddStringToObject(item, "nrfAmfSetAccessTokenUri", authorized_network_slice_info->nrf_amf_set_access_token_uri) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nrf_amf_set_access_token_uri]");
        goto end;
    }
    }

    if (authorized_network_slice_info->target_amf_service_set) {
    if (cJSON_AddStringToObject(item, "targetAmfServiceSet", authorized_network_slice_info->target_amf_service_set) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [target_amf_service_set]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_authorized_network_slice_info_t *OpenAPI_authorized_network_slice_info_parseFromJSON(cJSON *authorized_network_slice_infoJSON)
{
    OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info_local_var = NULL;
    cJSON *allowed_nssai_list = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "allowedNssaiList");

    OpenAPI_list_t *allowed_nssai_listList;
    if (allowed_nssai_list) {
    cJSON *allowed_nssai_list_local_nonprimitive;
    if (!cJSON_IsArray(allowed_nssai_list)){
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [allowed_nssai_list]");
        goto end;
    }

    allowed_nssai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(allowed_nssai_list_local_nonprimitive, allowed_nssai_list ) {
        if (!cJSON_IsObject(allowed_nssai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [allowed_nssai_list]");
            goto end;
        }
        OpenAPI_allowed_nssai_t *allowed_nssai_listItem = OpenAPI_allowed_nssai_parseFromJSON(allowed_nssai_list_local_nonprimitive);

        if (!allowed_nssai_listItem) {
            ogs_error("No allowed_nssai_listItem");
            OpenAPI_list_free(allowed_nssai_listList);
            goto end;
        }

        OpenAPI_list_add(allowed_nssai_listList, allowed_nssai_listItem);
    }
    }

    cJSON *configured_nssai = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "configuredNssai");

    OpenAPI_list_t *configured_nssaiList;
    if (configured_nssai) {
    cJSON *configured_nssai_local_nonprimitive;
    if (!cJSON_IsArray(configured_nssai)){
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [configured_nssai]");
        goto end;
    }

    configured_nssaiList = OpenAPI_list_create();

    cJSON_ArrayForEach(configured_nssai_local_nonprimitive, configured_nssai ) {
        if (!cJSON_IsObject(configured_nssai_local_nonprimitive)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [configured_nssai]");
            goto end;
        }
        OpenAPI_configured_snssai_t *configured_nssaiItem = OpenAPI_configured_snssai_parseFromJSON(configured_nssai_local_nonprimitive);

        if (!configured_nssaiItem) {
            ogs_error("No configured_nssaiItem");
            OpenAPI_list_free(configured_nssaiList);
            goto end;
        }

        OpenAPI_list_add(configured_nssaiList, configured_nssaiItem);
    }
    }

    cJSON *target_amf_set = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "targetAmfSet");

    if (target_amf_set) {
    if (!cJSON_IsString(target_amf_set)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [target_amf_set]");
        goto end;
    }
    }

    cJSON *candidate_amf_list = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "candidateAmfList");

    OpenAPI_list_t *candidate_amf_listList;
    if (candidate_amf_list) {
    cJSON *candidate_amf_list_local;
    if (!cJSON_IsArray(candidate_amf_list)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [candidate_amf_list]");
        goto end;
    }
    candidate_amf_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(candidate_amf_list_local, candidate_amf_list) {
    if (!cJSON_IsString(candidate_amf_list_local)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [candidate_amf_list]");
        goto end;
    }
    OpenAPI_list_add(candidate_amf_listList , ogs_strdup(candidate_amf_list_local->valuestring));
    }
    }

    cJSON *rejected_nssai_in_plmn = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "rejectedNssaiInPlmn");

    OpenAPI_list_t *rejected_nssai_in_plmnList;
    if (rejected_nssai_in_plmn) {
    cJSON *rejected_nssai_in_plmn_local_nonprimitive;
    if (!cJSON_IsArray(rejected_nssai_in_plmn)){
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [rejected_nssai_in_plmn]");
        goto end;
    }

    rejected_nssai_in_plmnList = OpenAPI_list_create();

    cJSON_ArrayForEach(rejected_nssai_in_plmn_local_nonprimitive, rejected_nssai_in_plmn ) {
        if (!cJSON_IsObject(rejected_nssai_in_plmn_local_nonprimitive)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [rejected_nssai_in_plmn]");
            goto end;
        }
        OpenAPI_snssai_t *rejected_nssai_in_plmnItem = OpenAPI_snssai_parseFromJSON(rejected_nssai_in_plmn_local_nonprimitive);

        if (!rejected_nssai_in_plmnItem) {
            ogs_error("No rejected_nssai_in_plmnItem");
            OpenAPI_list_free(rejected_nssai_in_plmnList);
            goto end;
        }

        OpenAPI_list_add(rejected_nssai_in_plmnList, rejected_nssai_in_plmnItem);
    }
    }

    cJSON *rejected_nssai_in_ta = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "rejectedNssaiInTa");

    OpenAPI_list_t *rejected_nssai_in_taList;
    if (rejected_nssai_in_ta) {
    cJSON *rejected_nssai_in_ta_local_nonprimitive;
    if (!cJSON_IsArray(rejected_nssai_in_ta)){
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [rejected_nssai_in_ta]");
        goto end;
    }

    rejected_nssai_in_taList = OpenAPI_list_create();

    cJSON_ArrayForEach(rejected_nssai_in_ta_local_nonprimitive, rejected_nssai_in_ta ) {
        if (!cJSON_IsObject(rejected_nssai_in_ta_local_nonprimitive)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [rejected_nssai_in_ta]");
            goto end;
        }
        OpenAPI_snssai_t *rejected_nssai_in_taItem = OpenAPI_snssai_parseFromJSON(rejected_nssai_in_ta_local_nonprimitive);

        if (!rejected_nssai_in_taItem) {
            ogs_error("No rejected_nssai_in_taItem");
            OpenAPI_list_free(rejected_nssai_in_taList);
            goto end;
        }

        OpenAPI_list_add(rejected_nssai_in_taList, rejected_nssai_in_taItem);
    }
    }

    cJSON *nsi_information = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nsiInformation");

    OpenAPI_nsi_information_t *nsi_information_local_nonprim = NULL;
    if (nsi_information) {
    nsi_information_local_nonprim = OpenAPI_nsi_information_parseFromJSON(nsi_information);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *nrf_amf_set = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nrfAmfSet");

    if (nrf_amf_set) {
    if (!cJSON_IsString(nrf_amf_set)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nrf_amf_set]");
        goto end;
    }
    }

    cJSON *nrf_amf_set_nf_mgt_uri = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nrfAmfSetNfMgtUri");

    if (nrf_amf_set_nf_mgt_uri) {
    if (!cJSON_IsString(nrf_amf_set_nf_mgt_uri)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nrf_amf_set_nf_mgt_uri]");
        goto end;
    }
    }

    cJSON *nrf_amf_set_access_token_uri = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nrfAmfSetAccessTokenUri");

    if (nrf_amf_set_access_token_uri) {
    if (!cJSON_IsString(nrf_amf_set_access_token_uri)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nrf_amf_set_access_token_uri]");
        goto end;
    }
    }

    cJSON *target_amf_service_set = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "targetAmfServiceSet");

    if (target_amf_service_set) {
    if (!cJSON_IsString(target_amf_service_set)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [target_amf_service_set]");
        goto end;
    }
    }

    authorized_network_slice_info_local_var = OpenAPI_authorized_network_slice_info_create (
        allowed_nssai_list ? allowed_nssai_listList : NULL,
        configured_nssai ? configured_nssaiList : NULL,
        target_amf_set ? ogs_strdup(target_amf_set->valuestring) : NULL,
        candidate_amf_list ? candidate_amf_listList : NULL,
        rejected_nssai_in_plmn ? rejected_nssai_in_plmnList : NULL,
        rejected_nssai_in_ta ? rejected_nssai_in_taList : NULL,
        nsi_information ? nsi_information_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        nrf_amf_set ? ogs_strdup(nrf_amf_set->valuestring) : NULL,
        nrf_amf_set_nf_mgt_uri ? ogs_strdup(nrf_amf_set_nf_mgt_uri->valuestring) : NULL,
        nrf_amf_set_access_token_uri ? ogs_strdup(nrf_amf_set_access_token_uri->valuestring) : NULL,
        target_amf_service_set ? ogs_strdup(target_amf_service_set->valuestring) : NULL
    );

    return authorized_network_slice_info_local_var;
end:
    return NULL;
}

OpenAPI_authorized_network_slice_info_t *OpenAPI_authorized_network_slice_info_copy(OpenAPI_authorized_network_slice_info_t *dst, OpenAPI_authorized_network_slice_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authorized_network_slice_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed");
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

    OpenAPI_authorized_network_slice_info_free(dst);
    dst = OpenAPI_authorized_network_slice_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

