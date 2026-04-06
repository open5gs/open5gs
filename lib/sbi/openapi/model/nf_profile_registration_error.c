
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_profile_registration_error.h"

OpenAPI_nf_profile_registration_error_t *OpenAPI_nf_profile_registration_error_create(
    char *type,
    char *title,
    bool is_status,
    int status,
    char *detail,
    char *instance,
    char *cause,
    OpenAPI_list_t *invalid_params,
    char *supported_features,
    OpenAPI_access_token_err_t *access_token_error,
    OpenAPI_access_token_req_t *access_token_request,
    char *nrf_id,
    OpenAPI_list_t *supported_api_versions,
    OpenAPI_no_profile_match_info_t *no_profile_match_info,
    OpenAPI_list_t *shared_data_ids
)
{
    OpenAPI_nf_profile_registration_error_t *nf_profile_registration_error_local_var = ogs_malloc(sizeof(OpenAPI_nf_profile_registration_error_t));
    ogs_assert(nf_profile_registration_error_local_var);

    nf_profile_registration_error_local_var->type = type;
    nf_profile_registration_error_local_var->title = title;
    nf_profile_registration_error_local_var->is_status = is_status;
    nf_profile_registration_error_local_var->status = status;
    nf_profile_registration_error_local_var->detail = detail;
    nf_profile_registration_error_local_var->instance = instance;
    nf_profile_registration_error_local_var->cause = cause;
    nf_profile_registration_error_local_var->invalid_params = invalid_params;
    nf_profile_registration_error_local_var->supported_features = supported_features;
    nf_profile_registration_error_local_var->access_token_error = access_token_error;
    nf_profile_registration_error_local_var->access_token_request = access_token_request;
    nf_profile_registration_error_local_var->nrf_id = nrf_id;
    nf_profile_registration_error_local_var->supported_api_versions = supported_api_versions;
    nf_profile_registration_error_local_var->no_profile_match_info = no_profile_match_info;
    nf_profile_registration_error_local_var->shared_data_ids = shared_data_ids;

    return nf_profile_registration_error_local_var;
}

void OpenAPI_nf_profile_registration_error_free(OpenAPI_nf_profile_registration_error_t *nf_profile_registration_error)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nf_profile_registration_error) {
        return;
    }
    if (nf_profile_registration_error->type) {
        ogs_free(nf_profile_registration_error->type);
        nf_profile_registration_error->type = NULL;
    }
    if (nf_profile_registration_error->title) {
        ogs_free(nf_profile_registration_error->title);
        nf_profile_registration_error->title = NULL;
    }
    if (nf_profile_registration_error->detail) {
        ogs_free(nf_profile_registration_error->detail);
        nf_profile_registration_error->detail = NULL;
    }
    if (nf_profile_registration_error->instance) {
        ogs_free(nf_profile_registration_error->instance);
        nf_profile_registration_error->instance = NULL;
    }
    if (nf_profile_registration_error->cause) {
        ogs_free(nf_profile_registration_error->cause);
        nf_profile_registration_error->cause = NULL;
    }
    if (nf_profile_registration_error->invalid_params) {
        OpenAPI_list_for_each(nf_profile_registration_error->invalid_params, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(nf_profile_registration_error->invalid_params);
        nf_profile_registration_error->invalid_params = NULL;
    }
    if (nf_profile_registration_error->supported_features) {
        ogs_free(nf_profile_registration_error->supported_features);
        nf_profile_registration_error->supported_features = NULL;
    }
    if (nf_profile_registration_error->access_token_error) {
        OpenAPI_access_token_err_free(nf_profile_registration_error->access_token_error);
        nf_profile_registration_error->access_token_error = NULL;
    }
    if (nf_profile_registration_error->access_token_request) {
        OpenAPI_access_token_req_free(nf_profile_registration_error->access_token_request);
        nf_profile_registration_error->access_token_request = NULL;
    }
    if (nf_profile_registration_error->nrf_id) {
        ogs_free(nf_profile_registration_error->nrf_id);
        nf_profile_registration_error->nrf_id = NULL;
    }
    if (nf_profile_registration_error->supported_api_versions) {
        OpenAPI_list_for_each(nf_profile_registration_error->supported_api_versions, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_profile_registration_error->supported_api_versions);
        nf_profile_registration_error->supported_api_versions = NULL;
    }
    if (nf_profile_registration_error->no_profile_match_info) {
        OpenAPI_no_profile_match_info_free(nf_profile_registration_error->no_profile_match_info);
        nf_profile_registration_error->no_profile_match_info = NULL;
    }
    if (nf_profile_registration_error->shared_data_ids) {
        OpenAPI_list_for_each(nf_profile_registration_error->shared_data_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_profile_registration_error->shared_data_ids);
        nf_profile_registration_error->shared_data_ids = NULL;
    }
    ogs_free(nf_profile_registration_error);
}

cJSON *OpenAPI_nf_profile_registration_error_convertToJSON(OpenAPI_nf_profile_registration_error_t *nf_profile_registration_error)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nf_profile_registration_error == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [NFProfileRegistrationError]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nf_profile_registration_error->type) {
    if (cJSON_AddStringToObject(item, "type", nf_profile_registration_error->type) == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [type]");
        goto end;
    }
    }

    if (nf_profile_registration_error->title) {
    if (cJSON_AddStringToObject(item, "title", nf_profile_registration_error->title) == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [title]");
        goto end;
    }
    }

    if (nf_profile_registration_error->is_status) {
    if (cJSON_AddNumberToObject(item, "status", nf_profile_registration_error->status) == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [status]");
        goto end;
    }
    }

    if (nf_profile_registration_error->detail) {
    if (cJSON_AddStringToObject(item, "detail", nf_profile_registration_error->detail) == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [detail]");
        goto end;
    }
    }

    if (nf_profile_registration_error->instance) {
    if (cJSON_AddStringToObject(item, "instance", nf_profile_registration_error->instance) == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [instance]");
        goto end;
    }
    }

    if (nf_profile_registration_error->cause) {
    if (cJSON_AddStringToObject(item, "cause", nf_profile_registration_error->cause) == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (nf_profile_registration_error->invalid_params) {
    cJSON *invalid_paramsList = cJSON_AddArrayToObject(item, "invalidParams");
    if (invalid_paramsList == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [invalid_params]");
        goto end;
    }
    OpenAPI_list_for_each(nf_profile_registration_error->invalid_params, node) {
        cJSON *itemLocal = OpenAPI_invalid_param_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [invalid_params]");
            goto end;
        }
        cJSON_AddItemToArray(invalid_paramsList, itemLocal);
    }
    }

    if (nf_profile_registration_error->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", nf_profile_registration_error->supported_features) == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (nf_profile_registration_error->access_token_error) {
    cJSON *access_token_error_local_JSON = OpenAPI_access_token_err_convertToJSON(nf_profile_registration_error->access_token_error);
    if (access_token_error_local_JSON == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [access_token_error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessTokenError", access_token_error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [access_token_error]");
        goto end;
    }
    }

    if (nf_profile_registration_error->access_token_request) {
    cJSON *access_token_request_local_JSON = OpenAPI_access_token_req_convertToJSON(nf_profile_registration_error->access_token_request);
    if (access_token_request_local_JSON == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [access_token_request]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessTokenRequest", access_token_request_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [access_token_request]");
        goto end;
    }
    }

    if (nf_profile_registration_error->nrf_id) {
    if (cJSON_AddStringToObject(item, "nrfId", nf_profile_registration_error->nrf_id) == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [nrf_id]");
        goto end;
    }
    }

    if (nf_profile_registration_error->supported_api_versions) {
    cJSON *supported_api_versionsList = cJSON_AddArrayToObject(item, "supportedApiVersions");
    if (supported_api_versionsList == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [supported_api_versions]");
        goto end;
    }
    OpenAPI_list_for_each(nf_profile_registration_error->supported_api_versions, node) {
        if (cJSON_AddStringToObject(supported_api_versionsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [supported_api_versions]");
            goto end;
        }
    }
    }

    if (nf_profile_registration_error->no_profile_match_info) {
    cJSON *no_profile_match_info_local_JSON = OpenAPI_no_profile_match_info_convertToJSON(nf_profile_registration_error->no_profile_match_info);
    if (no_profile_match_info_local_JSON == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [no_profile_match_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "noProfileMatchInfo", no_profile_match_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [no_profile_match_info]");
        goto end;
    }
    }

    if (!nf_profile_registration_error->shared_data_ids) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [shared_data_ids]");
        return NULL;
    }
    cJSON *shared_data_idsList = cJSON_AddArrayToObject(item, "sharedDataIds");
    if (shared_data_idsList == NULL) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [shared_data_ids]");
        goto end;
    }
    OpenAPI_list_for_each(nf_profile_registration_error->shared_data_ids, node) {
        if (cJSON_AddStringToObject(shared_data_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed [shared_data_ids]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_nf_profile_registration_error_t *OpenAPI_nf_profile_registration_error_parseFromJSON(cJSON *nf_profile_registration_errorJSON)
{
    OpenAPI_nf_profile_registration_error_t *nf_profile_registration_error_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *type = NULL;
    cJSON *title = NULL;
    cJSON *status = NULL;
    cJSON *detail = NULL;
    cJSON *instance = NULL;
    cJSON *cause = NULL;
    cJSON *invalid_params = NULL;
    OpenAPI_list_t *invalid_paramsList = NULL;
    cJSON *supported_features = NULL;
    cJSON *access_token_error = NULL;
    OpenAPI_access_token_err_t *access_token_error_local_nonprim = NULL;
    cJSON *access_token_request = NULL;
    OpenAPI_access_token_req_t *access_token_request_local_nonprim = NULL;
    cJSON *nrf_id = NULL;
    cJSON *supported_api_versions = NULL;
    OpenAPI_list_t *supported_api_versionsList = NULL;
    cJSON *no_profile_match_info = NULL;
    OpenAPI_no_profile_match_info_t *no_profile_match_info_local_nonprim = NULL;
    cJSON *shared_data_ids = NULL;
    OpenAPI_list_t *shared_data_idsList = NULL;
    type = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "type");
    if (type) {
    if (!cJSON_IsString(type) && !cJSON_IsNull(type)) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [type]");
        goto end;
    }
    }

    title = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "title");
    if (title) {
    if (!cJSON_IsString(title) && !cJSON_IsNull(title)) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [title]");
        goto end;
    }
    }

    status = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "status");
    if (status) {
    if (!cJSON_IsNumber(status)) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [status]");
        goto end;
    }
    }

    detail = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "detail");
    if (detail) {
    if (!cJSON_IsString(detail) && !cJSON_IsNull(detail)) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [detail]");
        goto end;
    }
    }

    instance = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "instance");
    if (instance) {
    if (!cJSON_IsString(instance) && !cJSON_IsNull(instance)) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [instance]");
        goto end;
    }
    }

    cause = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause) && !cJSON_IsNull(cause)) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    invalid_params = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "invalidParams");
    if (invalid_params) {
        cJSON *invalid_params_local = NULL;
        if (!cJSON_IsArray(invalid_params)) {
            ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [invalid_params]");
            goto end;
        }

        invalid_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(invalid_params_local, invalid_params) {
            if (!cJSON_IsObject(invalid_params_local)) {
                ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [invalid_params]");
                goto end;
            }
            OpenAPI_invalid_param_t *invalid_paramsItem = OpenAPI_invalid_param_parseFromJSON(invalid_params_local);
            if (!invalid_paramsItem) {
                ogs_error("No invalid_paramsItem");
                goto end;
            }
            OpenAPI_list_add(invalid_paramsList, invalid_paramsItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    access_token_error = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "accessTokenError");
    if (access_token_error) {
    access_token_error_local_nonprim = OpenAPI_access_token_err_parseFromJSON(access_token_error);
    if (!access_token_error_local_nonprim) {
        ogs_error("OpenAPI_access_token_err_parseFromJSON failed [access_token_error]");
        goto end;
    }
    }

    access_token_request = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "accessTokenRequest");
    if (access_token_request) {
    access_token_request_local_nonprim = OpenAPI_access_token_req_parseFromJSON(access_token_request);
    if (!access_token_request_local_nonprim) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON failed [access_token_request]");
        goto end;
    }
    }

    nrf_id = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "nrfId");
    if (nrf_id) {
    if (!cJSON_IsString(nrf_id) && !cJSON_IsNull(nrf_id)) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [nrf_id]");
        goto end;
    }
    }

    supported_api_versions = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "supportedApiVersions");
    if (supported_api_versions) {
        cJSON *supported_api_versions_local = NULL;
        if (!cJSON_IsArray(supported_api_versions)) {
            ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [supported_api_versions]");
            goto end;
        }

        supported_api_versionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_api_versions_local, supported_api_versions) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supported_api_versions_local)) {
                ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [supported_api_versions]");
                goto end;
            }
            OpenAPI_list_add(supported_api_versionsList, ogs_strdup(supported_api_versions_local->valuestring));
        }
    }

    no_profile_match_info = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "noProfileMatchInfo");
    if (no_profile_match_info) {
    no_profile_match_info_local_nonprim = OpenAPI_no_profile_match_info_parseFromJSON(no_profile_match_info);
    if (!no_profile_match_info_local_nonprim) {
        ogs_error("OpenAPI_no_profile_match_info_parseFromJSON failed [no_profile_match_info]");
        goto end;
    }
    }

    shared_data_ids = cJSON_GetObjectItemCaseSensitive(nf_profile_registration_errorJSON, "sharedDataIds");
    if (!shared_data_ids) {
        ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [shared_data_ids]");
        goto end;
    }
        cJSON *shared_data_ids_local = NULL;
        if (!cJSON_IsArray(shared_data_ids)) {
            ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [shared_data_ids]");
            goto end;
        }

        shared_data_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_data_ids_local, shared_data_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(shared_data_ids_local)) {
                ogs_error("OpenAPI_nf_profile_registration_error_parseFromJSON() failed [shared_data_ids]");
                goto end;
            }
            OpenAPI_list_add(shared_data_idsList, ogs_strdup(shared_data_ids_local->valuestring));
        }

    nf_profile_registration_error_local_var = OpenAPI_nf_profile_registration_error_create (
        type && !cJSON_IsNull(type) ? ogs_strdup(type->valuestring) : NULL,
        title && !cJSON_IsNull(title) ? ogs_strdup(title->valuestring) : NULL,
        status ? true : false,
        status ? status->valuedouble : 0,
        detail && !cJSON_IsNull(detail) ? ogs_strdup(detail->valuestring) : NULL,
        instance && !cJSON_IsNull(instance) ? ogs_strdup(instance->valuestring) : NULL,
        cause && !cJSON_IsNull(cause) ? ogs_strdup(cause->valuestring) : NULL,
        invalid_params ? invalid_paramsList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        access_token_error ? access_token_error_local_nonprim : NULL,
        access_token_request ? access_token_request_local_nonprim : NULL,
        nrf_id && !cJSON_IsNull(nrf_id) ? ogs_strdup(nrf_id->valuestring) : NULL,
        supported_api_versions ? supported_api_versionsList : NULL,
        no_profile_match_info ? no_profile_match_info_local_nonprim : NULL,
        shared_data_idsList
    );

    return nf_profile_registration_error_local_var;
end:
    if (invalid_paramsList) {
        OpenAPI_list_for_each(invalid_paramsList, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(invalid_paramsList);
        invalid_paramsList = NULL;
    }
    if (access_token_error_local_nonprim) {
        OpenAPI_access_token_err_free(access_token_error_local_nonprim);
        access_token_error_local_nonprim = NULL;
    }
    if (access_token_request_local_nonprim) {
        OpenAPI_access_token_req_free(access_token_request_local_nonprim);
        access_token_request_local_nonprim = NULL;
    }
    if (supported_api_versionsList) {
        OpenAPI_list_for_each(supported_api_versionsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supported_api_versionsList);
        supported_api_versionsList = NULL;
    }
    if (no_profile_match_info_local_nonprim) {
        OpenAPI_no_profile_match_info_free(no_profile_match_info_local_nonprim);
        no_profile_match_info_local_nonprim = NULL;
    }
    if (shared_data_idsList) {
        OpenAPI_list_for_each(shared_data_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(shared_data_idsList);
        shared_data_idsList = NULL;
    }
    return NULL;
}

OpenAPI_nf_profile_registration_error_t *OpenAPI_nf_profile_registration_error_copy(OpenAPI_nf_profile_registration_error_t *dst, OpenAPI_nf_profile_registration_error_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_profile_registration_error_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_profile_registration_error_convertToJSON() failed");
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

    OpenAPI_nf_profile_registration_error_free(dst);
    dst = OpenAPI_nf_profile_registration_error_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

