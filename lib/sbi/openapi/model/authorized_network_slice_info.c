
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
    OpenAPI_list_t* nrf_oauth2_required,
    char *target_amf_service_set,
    OpenAPI_list_t *target_nssai,
    OpenAPI_list_t *nsag_infos
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
    authorized_network_slice_info_local_var->nrf_oauth2_required = nrf_oauth2_required;
    authorized_network_slice_info_local_var->target_amf_service_set = target_amf_service_set;
    authorized_network_slice_info_local_var->target_nssai = target_nssai;
    authorized_network_slice_info_local_var->nsag_infos = nsag_infos;

    return authorized_network_slice_info_local_var;
}

void OpenAPI_authorized_network_slice_info_free(OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == authorized_network_slice_info) {
        return;
    }
    if (authorized_network_slice_info->allowed_nssai_list) {
        OpenAPI_list_for_each(authorized_network_slice_info->allowed_nssai_list, node) {
            OpenAPI_allowed_nssai_free(node->data);
        }
        OpenAPI_list_free(authorized_network_slice_info->allowed_nssai_list);
        authorized_network_slice_info->allowed_nssai_list = NULL;
    }
    if (authorized_network_slice_info->configured_nssai) {
        OpenAPI_list_for_each(authorized_network_slice_info->configured_nssai, node) {
            OpenAPI_configured_snssai_free(node->data);
        }
        OpenAPI_list_free(authorized_network_slice_info->configured_nssai);
        authorized_network_slice_info->configured_nssai = NULL;
    }
    if (authorized_network_slice_info->target_amf_set) {
        ogs_free(authorized_network_slice_info->target_amf_set);
        authorized_network_slice_info->target_amf_set = NULL;
    }
    if (authorized_network_slice_info->candidate_amf_list) {
        OpenAPI_list_for_each(authorized_network_slice_info->candidate_amf_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(authorized_network_slice_info->candidate_amf_list);
        authorized_network_slice_info->candidate_amf_list = NULL;
    }
    if (authorized_network_slice_info->rejected_nssai_in_plmn) {
        OpenAPI_list_for_each(authorized_network_slice_info->rejected_nssai_in_plmn, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(authorized_network_slice_info->rejected_nssai_in_plmn);
        authorized_network_slice_info->rejected_nssai_in_plmn = NULL;
    }
    if (authorized_network_slice_info->rejected_nssai_in_ta) {
        OpenAPI_list_for_each(authorized_network_slice_info->rejected_nssai_in_ta, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(authorized_network_slice_info->rejected_nssai_in_ta);
        authorized_network_slice_info->rejected_nssai_in_ta = NULL;
    }
    if (authorized_network_slice_info->nsi_information) {
        OpenAPI_nsi_information_free(authorized_network_slice_info->nsi_information);
        authorized_network_slice_info->nsi_information = NULL;
    }
    if (authorized_network_slice_info->supported_features) {
        ogs_free(authorized_network_slice_info->supported_features);
        authorized_network_slice_info->supported_features = NULL;
    }
    if (authorized_network_slice_info->nrf_amf_set) {
        ogs_free(authorized_network_slice_info->nrf_amf_set);
        authorized_network_slice_info->nrf_amf_set = NULL;
    }
    if (authorized_network_slice_info->nrf_amf_set_nf_mgt_uri) {
        ogs_free(authorized_network_slice_info->nrf_amf_set_nf_mgt_uri);
        authorized_network_slice_info->nrf_amf_set_nf_mgt_uri = NULL;
    }
    if (authorized_network_slice_info->nrf_amf_set_access_token_uri) {
        ogs_free(authorized_network_slice_info->nrf_amf_set_access_token_uri);
        authorized_network_slice_info->nrf_amf_set_access_token_uri = NULL;
    }
    if (authorized_network_slice_info->nrf_oauth2_required) {
        OpenAPI_list_for_each(authorized_network_slice_info->nrf_oauth2_required, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(authorized_network_slice_info->nrf_oauth2_required);
        authorized_network_slice_info->nrf_oauth2_required = NULL;
    }
    if (authorized_network_slice_info->target_amf_service_set) {
        ogs_free(authorized_network_slice_info->target_amf_service_set);
        authorized_network_slice_info->target_amf_service_set = NULL;
    }
    if (authorized_network_slice_info->target_nssai) {
        OpenAPI_list_for_each(authorized_network_slice_info->target_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(authorized_network_slice_info->target_nssai);
        authorized_network_slice_info->target_nssai = NULL;
    }
    if (authorized_network_slice_info->nsag_infos) {
        OpenAPI_list_for_each(authorized_network_slice_info->nsag_infos, node) {
            OpenAPI_nsag_info_free(node->data);
        }
        OpenAPI_list_free(authorized_network_slice_info->nsag_infos);
        authorized_network_slice_info->nsag_infos = NULL;
    }
    ogs_free(authorized_network_slice_info);
}

cJSON *OpenAPI_authorized_network_slice_info_convertToJSON(OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(authorized_network_slice_info->allowed_nssai_list, node) {
        cJSON *itemLocal = OpenAPI_allowed_nssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [allowed_nssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_nssai_listList, itemLocal);
    }
    }

    if (authorized_network_slice_info->configured_nssai) {
    cJSON *configured_nssaiList = cJSON_AddArrayToObject(item, "configuredNssai");
    if (configured_nssaiList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [configured_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(authorized_network_slice_info->configured_nssai, node) {
        cJSON *itemLocal = OpenAPI_configured_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [configured_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(configured_nssaiList, itemLocal);
    }
    }

    if (authorized_network_slice_info->target_amf_set) {
    if (cJSON_AddStringToObject(item, "targetAmfSet", authorized_network_slice_info->target_amf_set) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [target_amf_set]");
        goto end;
    }
    }

    if (authorized_network_slice_info->candidate_amf_list) {
    cJSON *candidate_amf_listList = cJSON_AddArrayToObject(item, "candidateAmfList");
    if (candidate_amf_listList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [candidate_amf_list]");
        goto end;
    }
    OpenAPI_list_for_each(authorized_network_slice_info->candidate_amf_list, node) {
        if (cJSON_AddStringToObject(candidate_amf_listList, "", (char*)node->data) == NULL) {
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
    OpenAPI_list_for_each(authorized_network_slice_info->rejected_nssai_in_plmn, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [rejected_nssai_in_plmn]");
            goto end;
        }
        cJSON_AddItemToArray(rejected_nssai_in_plmnList, itemLocal);
    }
    }

    if (authorized_network_slice_info->rejected_nssai_in_ta) {
    cJSON *rejected_nssai_in_taList = cJSON_AddArrayToObject(item, "rejectedNssaiInTa");
    if (rejected_nssai_in_taList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [rejected_nssai_in_ta]");
        goto end;
    }
    OpenAPI_list_for_each(authorized_network_slice_info->rejected_nssai_in_ta, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [rejected_nssai_in_ta]");
            goto end;
        }
        cJSON_AddItemToArray(rejected_nssai_in_taList, itemLocal);
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

    if (authorized_network_slice_info->nrf_oauth2_required) {
    cJSON *nrf_oauth2_required = cJSON_AddObjectToObject(item, "nrfOauth2Required");
    if (nrf_oauth2_required == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nrf_oauth2_required]");
        goto end;
    }
    cJSON *localMapObject = nrf_oauth2_required;
    if (authorized_network_slice_info->nrf_oauth2_required) {
        OpenAPI_list_for_each(authorized_network_slice_info->nrf_oauth2_required, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nrf_oauth2_required]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nrf_oauth2_required]");
                goto end;
            }
            if (cJSON_AddBoolToObject(localMapObject, localKeyValue->key, (uintptr_t)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (authorized_network_slice_info->target_amf_service_set) {
    if (cJSON_AddStringToObject(item, "targetAmfServiceSet", authorized_network_slice_info->target_amf_service_set) == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [target_amf_service_set]");
        goto end;
    }
    }

    if (authorized_network_slice_info->target_nssai) {
    cJSON *target_nssaiList = cJSON_AddArrayToObject(item, "targetNssai");
    if (target_nssaiList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [target_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(authorized_network_slice_info->target_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [target_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(target_nssaiList, itemLocal);
    }
    }

    if (authorized_network_slice_info->nsag_infos) {
    cJSON *nsag_infosList = cJSON_AddArrayToObject(item, "nsagInfos");
    if (nsag_infosList == NULL) {
        ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nsag_infos]");
        goto end;
    }
    OpenAPI_list_for_each(authorized_network_slice_info->nsag_infos, node) {
        cJSON *itemLocal = OpenAPI_nsag_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_authorized_network_slice_info_convertToJSON() failed [nsag_infos]");
            goto end;
        }
        cJSON_AddItemToArray(nsag_infosList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_authorized_network_slice_info_t *OpenAPI_authorized_network_slice_info_parseFromJSON(cJSON *authorized_network_slice_infoJSON)
{
    OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *allowed_nssai_list = NULL;
    OpenAPI_list_t *allowed_nssai_listList = NULL;
    cJSON *configured_nssai = NULL;
    OpenAPI_list_t *configured_nssaiList = NULL;
    cJSON *target_amf_set = NULL;
    cJSON *candidate_amf_list = NULL;
    OpenAPI_list_t *candidate_amf_listList = NULL;
    cJSON *rejected_nssai_in_plmn = NULL;
    OpenAPI_list_t *rejected_nssai_in_plmnList = NULL;
    cJSON *rejected_nssai_in_ta = NULL;
    OpenAPI_list_t *rejected_nssai_in_taList = NULL;
    cJSON *nsi_information = NULL;
    OpenAPI_nsi_information_t *nsi_information_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *nrf_amf_set = NULL;
    cJSON *nrf_amf_set_nf_mgt_uri = NULL;
    cJSON *nrf_amf_set_access_token_uri = NULL;
    cJSON *nrf_oauth2_required = NULL;
    OpenAPI_list_t *nrf_oauth2_requiredList = NULL;
    cJSON *target_amf_service_set = NULL;
    cJSON *target_nssai = NULL;
    OpenAPI_list_t *target_nssaiList = NULL;
    cJSON *nsag_infos = NULL;
    OpenAPI_list_t *nsag_infosList = NULL;
    allowed_nssai_list = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "allowedNssaiList");
    if (allowed_nssai_list) {
        cJSON *allowed_nssai_list_local = NULL;
        if (!cJSON_IsArray(allowed_nssai_list)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [allowed_nssai_list]");
            goto end;
        }

        allowed_nssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nssai_list_local, allowed_nssai_list) {
            if (!cJSON_IsObject(allowed_nssai_list_local)) {
                ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [allowed_nssai_list]");
                goto end;
            }
            OpenAPI_allowed_nssai_t *allowed_nssai_listItem = OpenAPI_allowed_nssai_parseFromJSON(allowed_nssai_list_local);
            if (!allowed_nssai_listItem) {
                ogs_error("No allowed_nssai_listItem");
                goto end;
            }
            OpenAPI_list_add(allowed_nssai_listList, allowed_nssai_listItem);
        }
    }

    configured_nssai = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "configuredNssai");
    if (configured_nssai) {
        cJSON *configured_nssai_local = NULL;
        if (!cJSON_IsArray(configured_nssai)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [configured_nssai]");
            goto end;
        }

        configured_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(configured_nssai_local, configured_nssai) {
            if (!cJSON_IsObject(configured_nssai_local)) {
                ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [configured_nssai]");
                goto end;
            }
            OpenAPI_configured_snssai_t *configured_nssaiItem = OpenAPI_configured_snssai_parseFromJSON(configured_nssai_local);
            if (!configured_nssaiItem) {
                ogs_error("No configured_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(configured_nssaiList, configured_nssaiItem);
        }
    }

    target_amf_set = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "targetAmfSet");
    if (target_amf_set) {
    if (!cJSON_IsString(target_amf_set) && !cJSON_IsNull(target_amf_set)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [target_amf_set]");
        goto end;
    }
    }

    candidate_amf_list = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "candidateAmfList");
    if (candidate_amf_list) {
        cJSON *candidate_amf_list_local = NULL;
        if (!cJSON_IsArray(candidate_amf_list)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [candidate_amf_list]");
            goto end;
        }

        candidate_amf_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(candidate_amf_list_local, candidate_amf_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(candidate_amf_list_local)) {
                ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [candidate_amf_list]");
                goto end;
            }
            OpenAPI_list_add(candidate_amf_listList, ogs_strdup(candidate_amf_list_local->valuestring));
        }
    }

    rejected_nssai_in_plmn = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "rejectedNssaiInPlmn");
    if (rejected_nssai_in_plmn) {
        cJSON *rejected_nssai_in_plmn_local = NULL;
        if (!cJSON_IsArray(rejected_nssai_in_plmn)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [rejected_nssai_in_plmn]");
            goto end;
        }

        rejected_nssai_in_plmnList = OpenAPI_list_create();

        cJSON_ArrayForEach(rejected_nssai_in_plmn_local, rejected_nssai_in_plmn) {
            if (!cJSON_IsObject(rejected_nssai_in_plmn_local)) {
                ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [rejected_nssai_in_plmn]");
                goto end;
            }
            OpenAPI_snssai_t *rejected_nssai_in_plmnItem = OpenAPI_snssai_parseFromJSON(rejected_nssai_in_plmn_local);
            if (!rejected_nssai_in_plmnItem) {
                ogs_error("No rejected_nssai_in_plmnItem");
                goto end;
            }
            OpenAPI_list_add(rejected_nssai_in_plmnList, rejected_nssai_in_plmnItem);
        }
    }

    rejected_nssai_in_ta = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "rejectedNssaiInTa");
    if (rejected_nssai_in_ta) {
        cJSON *rejected_nssai_in_ta_local = NULL;
        if (!cJSON_IsArray(rejected_nssai_in_ta)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [rejected_nssai_in_ta]");
            goto end;
        }

        rejected_nssai_in_taList = OpenAPI_list_create();

        cJSON_ArrayForEach(rejected_nssai_in_ta_local, rejected_nssai_in_ta) {
            if (!cJSON_IsObject(rejected_nssai_in_ta_local)) {
                ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [rejected_nssai_in_ta]");
                goto end;
            }
            OpenAPI_snssai_t *rejected_nssai_in_taItem = OpenAPI_snssai_parseFromJSON(rejected_nssai_in_ta_local);
            if (!rejected_nssai_in_taItem) {
                ogs_error("No rejected_nssai_in_taItem");
                goto end;
            }
            OpenAPI_list_add(rejected_nssai_in_taList, rejected_nssai_in_taItem);
        }
    }

    nsi_information = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nsiInformation");
    if (nsi_information) {
    nsi_information_local_nonprim = OpenAPI_nsi_information_parseFromJSON(nsi_information);
    if (!nsi_information_local_nonprim) {
        ogs_error("OpenAPI_nsi_information_parseFromJSON failed [nsi_information]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    nrf_amf_set = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nrfAmfSet");
    if (nrf_amf_set) {
    if (!cJSON_IsString(nrf_amf_set) && !cJSON_IsNull(nrf_amf_set)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nrf_amf_set]");
        goto end;
    }
    }

    nrf_amf_set_nf_mgt_uri = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nrfAmfSetNfMgtUri");
    if (nrf_amf_set_nf_mgt_uri) {
    if (!cJSON_IsString(nrf_amf_set_nf_mgt_uri) && !cJSON_IsNull(nrf_amf_set_nf_mgt_uri)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nrf_amf_set_nf_mgt_uri]");
        goto end;
    }
    }

    nrf_amf_set_access_token_uri = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nrfAmfSetAccessTokenUri");
    if (nrf_amf_set_access_token_uri) {
    if (!cJSON_IsString(nrf_amf_set_access_token_uri) && !cJSON_IsNull(nrf_amf_set_access_token_uri)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nrf_amf_set_access_token_uri]");
        goto end;
    }
    }

    nrf_oauth2_required = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nrfOauth2Required");
    if (nrf_oauth2_required) {
        cJSON *nrf_oauth2_required_local_map = NULL;
        if (!cJSON_IsObject(nrf_oauth2_required) && !cJSON_IsNull(nrf_oauth2_required)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nrf_oauth2_required]");
            goto end;
        }
        if (cJSON_IsObject(nrf_oauth2_required)) {
            nrf_oauth2_requiredList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(nrf_oauth2_required_local_map, nrf_oauth2_required) {
                cJSON *localMapObject = nrf_oauth2_required_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsBool(localMapObject)) {
                    ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                localInt = (int *)ogs_calloc(1, sizeof(int));
                if (!localInt) {
                    ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localInt = localMapObject->valueint;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localInt);
                OpenAPI_list_add(nrf_oauth2_requiredList, localMapKeyPair);
            }
        }
    }

    target_amf_service_set = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "targetAmfServiceSet");
    if (target_amf_service_set) {
    if (!cJSON_IsString(target_amf_service_set) && !cJSON_IsNull(target_amf_service_set)) {
        ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [target_amf_service_set]");
        goto end;
    }
    }

    target_nssai = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "targetNssai");
    if (target_nssai) {
        cJSON *target_nssai_local = NULL;
        if (!cJSON_IsArray(target_nssai)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [target_nssai]");
            goto end;
        }

        target_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(target_nssai_local, target_nssai) {
            if (!cJSON_IsObject(target_nssai_local)) {
                ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [target_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *target_nssaiItem = OpenAPI_snssai_parseFromJSON(target_nssai_local);
            if (!target_nssaiItem) {
                ogs_error("No target_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(target_nssaiList, target_nssaiItem);
        }
    }

    nsag_infos = cJSON_GetObjectItemCaseSensitive(authorized_network_slice_infoJSON, "nsagInfos");
    if (nsag_infos) {
        cJSON *nsag_infos_local = NULL;
        if (!cJSON_IsArray(nsag_infos)) {
            ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nsag_infos]");
            goto end;
        }

        nsag_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsag_infos_local, nsag_infos) {
            if (!cJSON_IsObject(nsag_infos_local)) {
                ogs_error("OpenAPI_authorized_network_slice_info_parseFromJSON() failed [nsag_infos]");
                goto end;
            }
            OpenAPI_nsag_info_t *nsag_infosItem = OpenAPI_nsag_info_parseFromJSON(nsag_infos_local);
            if (!nsag_infosItem) {
                ogs_error("No nsag_infosItem");
                goto end;
            }
            OpenAPI_list_add(nsag_infosList, nsag_infosItem);
        }
    }

    authorized_network_slice_info_local_var = OpenAPI_authorized_network_slice_info_create (
        allowed_nssai_list ? allowed_nssai_listList : NULL,
        configured_nssai ? configured_nssaiList : NULL,
        target_amf_set && !cJSON_IsNull(target_amf_set) ? ogs_strdup(target_amf_set->valuestring) : NULL,
        candidate_amf_list ? candidate_amf_listList : NULL,
        rejected_nssai_in_plmn ? rejected_nssai_in_plmnList : NULL,
        rejected_nssai_in_ta ? rejected_nssai_in_taList : NULL,
        nsi_information ? nsi_information_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        nrf_amf_set && !cJSON_IsNull(nrf_amf_set) ? ogs_strdup(nrf_amf_set->valuestring) : NULL,
        nrf_amf_set_nf_mgt_uri && !cJSON_IsNull(nrf_amf_set_nf_mgt_uri) ? ogs_strdup(nrf_amf_set_nf_mgt_uri->valuestring) : NULL,
        nrf_amf_set_access_token_uri && !cJSON_IsNull(nrf_amf_set_access_token_uri) ? ogs_strdup(nrf_amf_set_access_token_uri->valuestring) : NULL,
        nrf_oauth2_required ? nrf_oauth2_requiredList : NULL,
        target_amf_service_set && !cJSON_IsNull(target_amf_service_set) ? ogs_strdup(target_amf_service_set->valuestring) : NULL,
        target_nssai ? target_nssaiList : NULL,
        nsag_infos ? nsag_infosList : NULL
    );

    return authorized_network_slice_info_local_var;
end:
    if (allowed_nssai_listList) {
        OpenAPI_list_for_each(allowed_nssai_listList, node) {
            OpenAPI_allowed_nssai_free(node->data);
        }
        OpenAPI_list_free(allowed_nssai_listList);
        allowed_nssai_listList = NULL;
    }
    if (configured_nssaiList) {
        OpenAPI_list_for_each(configured_nssaiList, node) {
            OpenAPI_configured_snssai_free(node->data);
        }
        OpenAPI_list_free(configured_nssaiList);
        configured_nssaiList = NULL;
    }
    if (candidate_amf_listList) {
        OpenAPI_list_for_each(candidate_amf_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(candidate_amf_listList);
        candidate_amf_listList = NULL;
    }
    if (rejected_nssai_in_plmnList) {
        OpenAPI_list_for_each(rejected_nssai_in_plmnList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(rejected_nssai_in_plmnList);
        rejected_nssai_in_plmnList = NULL;
    }
    if (rejected_nssai_in_taList) {
        OpenAPI_list_for_each(rejected_nssai_in_taList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(rejected_nssai_in_taList);
        rejected_nssai_in_taList = NULL;
    }
    if (nsi_information_local_nonprim) {
        OpenAPI_nsi_information_free(nsi_information_local_nonprim);
        nsi_information_local_nonprim = NULL;
    }
    if (nrf_oauth2_requiredList) {
        OpenAPI_list_for_each(nrf_oauth2_requiredList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_oauth2_requiredList);
        nrf_oauth2_requiredList = NULL;
    }
    if (target_nssaiList) {
        OpenAPI_list_for_each(target_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(target_nssaiList);
        target_nssaiList = NULL;
    }
    if (nsag_infosList) {
        OpenAPI_list_for_each(nsag_infosList, node) {
            OpenAPI_nsag_info_free(node->data);
        }
        OpenAPI_list_free(nsag_infosList);
        nsag_infosList = NULL;
    }
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

