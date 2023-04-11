
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "extended_problem_details.h"

OpenAPI_extended_problem_details_t *OpenAPI_extended_problem_details_create(
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
    OpenAPI_acceptable_service_info_t *acceptable_serv_info
)
{
    OpenAPI_extended_problem_details_t *extended_problem_details_local_var = ogs_malloc(sizeof(OpenAPI_extended_problem_details_t));
    ogs_assert(extended_problem_details_local_var);

    extended_problem_details_local_var->type = type;
    extended_problem_details_local_var->title = title;
    extended_problem_details_local_var->is_status = is_status;
    extended_problem_details_local_var->status = status;
    extended_problem_details_local_var->detail = detail;
    extended_problem_details_local_var->instance = instance;
    extended_problem_details_local_var->cause = cause;
    extended_problem_details_local_var->invalid_params = invalid_params;
    extended_problem_details_local_var->supported_features = supported_features;
    extended_problem_details_local_var->access_token_error = access_token_error;
    extended_problem_details_local_var->access_token_request = access_token_request;
    extended_problem_details_local_var->nrf_id = nrf_id;
    extended_problem_details_local_var->acceptable_serv_info = acceptable_serv_info;

    return extended_problem_details_local_var;
}

void OpenAPI_extended_problem_details_free(OpenAPI_extended_problem_details_t *extended_problem_details)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == extended_problem_details) {
        return;
    }
    if (extended_problem_details->type) {
        ogs_free(extended_problem_details->type);
        extended_problem_details->type = NULL;
    }
    if (extended_problem_details->title) {
        ogs_free(extended_problem_details->title);
        extended_problem_details->title = NULL;
    }
    if (extended_problem_details->detail) {
        ogs_free(extended_problem_details->detail);
        extended_problem_details->detail = NULL;
    }
    if (extended_problem_details->instance) {
        ogs_free(extended_problem_details->instance);
        extended_problem_details->instance = NULL;
    }
    if (extended_problem_details->cause) {
        ogs_free(extended_problem_details->cause);
        extended_problem_details->cause = NULL;
    }
    if (extended_problem_details->invalid_params) {
        OpenAPI_list_for_each(extended_problem_details->invalid_params, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(extended_problem_details->invalid_params);
        extended_problem_details->invalid_params = NULL;
    }
    if (extended_problem_details->supported_features) {
        ogs_free(extended_problem_details->supported_features);
        extended_problem_details->supported_features = NULL;
    }
    if (extended_problem_details->access_token_error) {
        OpenAPI_access_token_err_free(extended_problem_details->access_token_error);
        extended_problem_details->access_token_error = NULL;
    }
    if (extended_problem_details->access_token_request) {
        OpenAPI_access_token_req_free(extended_problem_details->access_token_request);
        extended_problem_details->access_token_request = NULL;
    }
    if (extended_problem_details->nrf_id) {
        ogs_free(extended_problem_details->nrf_id);
        extended_problem_details->nrf_id = NULL;
    }
    if (extended_problem_details->acceptable_serv_info) {
        OpenAPI_acceptable_service_info_free(extended_problem_details->acceptable_serv_info);
        extended_problem_details->acceptable_serv_info = NULL;
    }
    ogs_free(extended_problem_details);
}

cJSON *OpenAPI_extended_problem_details_convertToJSON(OpenAPI_extended_problem_details_t *extended_problem_details)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (extended_problem_details == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [ExtendedProblemDetails]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (extended_problem_details->type) {
    if (cJSON_AddStringToObject(item, "type", extended_problem_details->type) == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [type]");
        goto end;
    }
    }

    if (extended_problem_details->title) {
    if (cJSON_AddStringToObject(item, "title", extended_problem_details->title) == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [title]");
        goto end;
    }
    }

    if (extended_problem_details->is_status) {
    if (cJSON_AddNumberToObject(item, "status", extended_problem_details->status) == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [status]");
        goto end;
    }
    }

    if (extended_problem_details->detail) {
    if (cJSON_AddStringToObject(item, "detail", extended_problem_details->detail) == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [detail]");
        goto end;
    }
    }

    if (extended_problem_details->instance) {
    if (cJSON_AddStringToObject(item, "instance", extended_problem_details->instance) == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [instance]");
        goto end;
    }
    }

    if (extended_problem_details->cause) {
    if (cJSON_AddStringToObject(item, "cause", extended_problem_details->cause) == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (extended_problem_details->invalid_params) {
    cJSON *invalid_paramsList = cJSON_AddArrayToObject(item, "invalidParams");
    if (invalid_paramsList == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [invalid_params]");
        goto end;
    }
    OpenAPI_list_for_each(extended_problem_details->invalid_params, node) {
        cJSON *itemLocal = OpenAPI_invalid_param_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [invalid_params]");
            goto end;
        }
        cJSON_AddItemToArray(invalid_paramsList, itemLocal);
    }
    }

    if (extended_problem_details->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", extended_problem_details->supported_features) == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (extended_problem_details->access_token_error) {
    cJSON *access_token_error_local_JSON = OpenAPI_access_token_err_convertToJSON(extended_problem_details->access_token_error);
    if (access_token_error_local_JSON == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [access_token_error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessTokenError", access_token_error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [access_token_error]");
        goto end;
    }
    }

    if (extended_problem_details->access_token_request) {
    cJSON *access_token_request_local_JSON = OpenAPI_access_token_req_convertToJSON(extended_problem_details->access_token_request);
    if (access_token_request_local_JSON == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [access_token_request]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessTokenRequest", access_token_request_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [access_token_request]");
        goto end;
    }
    }

    if (extended_problem_details->nrf_id) {
    if (cJSON_AddStringToObject(item, "nrfId", extended_problem_details->nrf_id) == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [nrf_id]");
        goto end;
    }
    }

    if (extended_problem_details->acceptable_serv_info) {
    cJSON *acceptable_serv_info_local_JSON = OpenAPI_acceptable_service_info_convertToJSON(extended_problem_details->acceptable_serv_info);
    if (acceptable_serv_info_local_JSON == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [acceptable_serv_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "acceptableServInfo", acceptable_serv_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed [acceptable_serv_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_extended_problem_details_t *OpenAPI_extended_problem_details_parseFromJSON(cJSON *extended_problem_detailsJSON)
{
    OpenAPI_extended_problem_details_t *extended_problem_details_local_var = NULL;
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
    cJSON *acceptable_serv_info = NULL;
    OpenAPI_acceptable_service_info_t *acceptable_serv_info_local_nonprim = NULL;
    type = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "type");
    if (type) {
    if (!cJSON_IsString(type) && !cJSON_IsNull(type)) {
        ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [type]");
        goto end;
    }
    }

    title = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "title");
    if (title) {
    if (!cJSON_IsString(title) && !cJSON_IsNull(title)) {
        ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [title]");
        goto end;
    }
    }

    status = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "status");
    if (status) {
    if (!cJSON_IsNumber(status)) {
        ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [status]");
        goto end;
    }
    }

    detail = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "detail");
    if (detail) {
    if (!cJSON_IsString(detail) && !cJSON_IsNull(detail)) {
        ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [detail]");
        goto end;
    }
    }

    instance = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "instance");
    if (instance) {
    if (!cJSON_IsString(instance) && !cJSON_IsNull(instance)) {
        ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [instance]");
        goto end;
    }
    }

    cause = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause) && !cJSON_IsNull(cause)) {
        ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    invalid_params = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "invalidParams");
    if (invalid_params) {
        cJSON *invalid_params_local = NULL;
        if (!cJSON_IsArray(invalid_params)) {
            ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [invalid_params]");
            goto end;
        }

        invalid_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(invalid_params_local, invalid_params) {
            if (!cJSON_IsObject(invalid_params_local)) {
                ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [invalid_params]");
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

    supported_features = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    access_token_error = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "accessTokenError");
    if (access_token_error) {
    access_token_error_local_nonprim = OpenAPI_access_token_err_parseFromJSON(access_token_error);
    if (!access_token_error_local_nonprim) {
        ogs_error("OpenAPI_access_token_err_parseFromJSON failed [access_token_error]");
        goto end;
    }
    }

    access_token_request = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "accessTokenRequest");
    if (access_token_request) {
    access_token_request_local_nonprim = OpenAPI_access_token_req_parseFromJSON(access_token_request);
    if (!access_token_request_local_nonprim) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON failed [access_token_request]");
        goto end;
    }
    }

    nrf_id = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "nrfId");
    if (nrf_id) {
    if (!cJSON_IsString(nrf_id) && !cJSON_IsNull(nrf_id)) {
        ogs_error("OpenAPI_extended_problem_details_parseFromJSON() failed [nrf_id]");
        goto end;
    }
    }

    acceptable_serv_info = cJSON_GetObjectItemCaseSensitive(extended_problem_detailsJSON, "acceptableServInfo");
    if (acceptable_serv_info) {
    acceptable_serv_info_local_nonprim = OpenAPI_acceptable_service_info_parseFromJSON(acceptable_serv_info);
    if (!acceptable_serv_info_local_nonprim) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON failed [acceptable_serv_info]");
        goto end;
    }
    }

    extended_problem_details_local_var = OpenAPI_extended_problem_details_create (
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
        acceptable_serv_info ? acceptable_serv_info_local_nonprim : NULL
    );

    return extended_problem_details_local_var;
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
    if (acceptable_serv_info_local_nonprim) {
        OpenAPI_acceptable_service_info_free(acceptable_serv_info_local_nonprim);
        acceptable_serv_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_extended_problem_details_t *OpenAPI_extended_problem_details_copy(OpenAPI_extended_problem_details_t *dst, OpenAPI_extended_problem_details_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_extended_problem_details_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_extended_problem_details_convertToJSON() failed");
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

    OpenAPI_extended_problem_details_free(dst);
    dst = OpenAPI_extended_problem_details_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

