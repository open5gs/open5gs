
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transfer_mt_data_error.h"

OpenAPI_transfer_mt_data_error_t *OpenAPI_transfer_mt_data_error_create(
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
    bool is_remote_error,
    int remote_error,
    bool is_max_waiting_time,
    int max_waiting_time
)
{
    OpenAPI_transfer_mt_data_error_t *transfer_mt_data_error_local_var = ogs_malloc(sizeof(OpenAPI_transfer_mt_data_error_t));
    ogs_assert(transfer_mt_data_error_local_var);

    transfer_mt_data_error_local_var->type = type;
    transfer_mt_data_error_local_var->title = title;
    transfer_mt_data_error_local_var->is_status = is_status;
    transfer_mt_data_error_local_var->status = status;
    transfer_mt_data_error_local_var->detail = detail;
    transfer_mt_data_error_local_var->instance = instance;
    transfer_mt_data_error_local_var->cause = cause;
    transfer_mt_data_error_local_var->invalid_params = invalid_params;
    transfer_mt_data_error_local_var->supported_features = supported_features;
    transfer_mt_data_error_local_var->access_token_error = access_token_error;
    transfer_mt_data_error_local_var->access_token_request = access_token_request;
    transfer_mt_data_error_local_var->nrf_id = nrf_id;
    transfer_mt_data_error_local_var->is_remote_error = is_remote_error;
    transfer_mt_data_error_local_var->remote_error = remote_error;
    transfer_mt_data_error_local_var->is_max_waiting_time = is_max_waiting_time;
    transfer_mt_data_error_local_var->max_waiting_time = max_waiting_time;

    return transfer_mt_data_error_local_var;
}

void OpenAPI_transfer_mt_data_error_free(OpenAPI_transfer_mt_data_error_t *transfer_mt_data_error)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == transfer_mt_data_error) {
        return;
    }
    if (transfer_mt_data_error->type) {
        ogs_free(transfer_mt_data_error->type);
        transfer_mt_data_error->type = NULL;
    }
    if (transfer_mt_data_error->title) {
        ogs_free(transfer_mt_data_error->title);
        transfer_mt_data_error->title = NULL;
    }
    if (transfer_mt_data_error->detail) {
        ogs_free(transfer_mt_data_error->detail);
        transfer_mt_data_error->detail = NULL;
    }
    if (transfer_mt_data_error->instance) {
        ogs_free(transfer_mt_data_error->instance);
        transfer_mt_data_error->instance = NULL;
    }
    if (transfer_mt_data_error->cause) {
        ogs_free(transfer_mt_data_error->cause);
        transfer_mt_data_error->cause = NULL;
    }
    if (transfer_mt_data_error->invalid_params) {
        OpenAPI_list_for_each(transfer_mt_data_error->invalid_params, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(transfer_mt_data_error->invalid_params);
        transfer_mt_data_error->invalid_params = NULL;
    }
    if (transfer_mt_data_error->supported_features) {
        ogs_free(transfer_mt_data_error->supported_features);
        transfer_mt_data_error->supported_features = NULL;
    }
    if (transfer_mt_data_error->access_token_error) {
        OpenAPI_access_token_err_free(transfer_mt_data_error->access_token_error);
        transfer_mt_data_error->access_token_error = NULL;
    }
    if (transfer_mt_data_error->access_token_request) {
        OpenAPI_access_token_req_free(transfer_mt_data_error->access_token_request);
        transfer_mt_data_error->access_token_request = NULL;
    }
    if (transfer_mt_data_error->nrf_id) {
        ogs_free(transfer_mt_data_error->nrf_id);
        transfer_mt_data_error->nrf_id = NULL;
    }
    ogs_free(transfer_mt_data_error);
}

cJSON *OpenAPI_transfer_mt_data_error_convertToJSON(OpenAPI_transfer_mt_data_error_t *transfer_mt_data_error)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (transfer_mt_data_error == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [TransferMtDataError]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (transfer_mt_data_error->type) {
    if (cJSON_AddStringToObject(item, "type", transfer_mt_data_error->type) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [type]");
        goto end;
    }
    }

    if (transfer_mt_data_error->title) {
    if (cJSON_AddStringToObject(item, "title", transfer_mt_data_error->title) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [title]");
        goto end;
    }
    }

    if (transfer_mt_data_error->is_status) {
    if (cJSON_AddNumberToObject(item, "status", transfer_mt_data_error->status) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [status]");
        goto end;
    }
    }

    if (transfer_mt_data_error->detail) {
    if (cJSON_AddStringToObject(item, "detail", transfer_mt_data_error->detail) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [detail]");
        goto end;
    }
    }

    if (transfer_mt_data_error->instance) {
    if (cJSON_AddStringToObject(item, "instance", transfer_mt_data_error->instance) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [instance]");
        goto end;
    }
    }

    if (transfer_mt_data_error->cause) {
    if (cJSON_AddStringToObject(item, "cause", transfer_mt_data_error->cause) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (transfer_mt_data_error->invalid_params) {
    cJSON *invalid_paramsList = cJSON_AddArrayToObject(item, "invalidParams");
    if (invalid_paramsList == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [invalid_params]");
        goto end;
    }
    OpenAPI_list_for_each(transfer_mt_data_error->invalid_params, node) {
        cJSON *itemLocal = OpenAPI_invalid_param_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [invalid_params]");
            goto end;
        }
        cJSON_AddItemToArray(invalid_paramsList, itemLocal);
    }
    }

    if (transfer_mt_data_error->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", transfer_mt_data_error->supported_features) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (transfer_mt_data_error->access_token_error) {
    cJSON *access_token_error_local_JSON = OpenAPI_access_token_err_convertToJSON(transfer_mt_data_error->access_token_error);
    if (access_token_error_local_JSON == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [access_token_error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessTokenError", access_token_error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [access_token_error]");
        goto end;
    }
    }

    if (transfer_mt_data_error->access_token_request) {
    cJSON *access_token_request_local_JSON = OpenAPI_access_token_req_convertToJSON(transfer_mt_data_error->access_token_request);
    if (access_token_request_local_JSON == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [access_token_request]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessTokenRequest", access_token_request_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [access_token_request]");
        goto end;
    }
    }

    if (transfer_mt_data_error->nrf_id) {
    if (cJSON_AddStringToObject(item, "nrfId", transfer_mt_data_error->nrf_id) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [nrf_id]");
        goto end;
    }
    }

    if (transfer_mt_data_error->is_remote_error) {
    if (cJSON_AddBoolToObject(item, "remoteError", transfer_mt_data_error->remote_error) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [remote_error]");
        goto end;
    }
    }

    if (transfer_mt_data_error->is_max_waiting_time) {
    if (cJSON_AddNumberToObject(item, "maxWaitingTime", transfer_mt_data_error->max_waiting_time) == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [max_waiting_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_transfer_mt_data_error_t *OpenAPI_transfer_mt_data_error_parseFromJSON(cJSON *transfer_mt_data_errorJSON)
{
    OpenAPI_transfer_mt_data_error_t *transfer_mt_data_error_local_var = NULL;
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
    cJSON *remote_error = NULL;
    cJSON *max_waiting_time = NULL;
    type = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "type");
    if (type) {
    if (!cJSON_IsString(type) && !cJSON_IsNull(type)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [type]");
        goto end;
    }
    }

    title = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "title");
    if (title) {
    if (!cJSON_IsString(title) && !cJSON_IsNull(title)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [title]");
        goto end;
    }
    }

    status = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "status");
    if (status) {
    if (!cJSON_IsNumber(status)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [status]");
        goto end;
    }
    }

    detail = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "detail");
    if (detail) {
    if (!cJSON_IsString(detail) && !cJSON_IsNull(detail)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [detail]");
        goto end;
    }
    }

    instance = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "instance");
    if (instance) {
    if (!cJSON_IsString(instance) && !cJSON_IsNull(instance)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [instance]");
        goto end;
    }
    }

    cause = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause) && !cJSON_IsNull(cause)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    invalid_params = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "invalidParams");
    if (invalid_params) {
        cJSON *invalid_params_local = NULL;
        if (!cJSON_IsArray(invalid_params)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [invalid_params]");
            goto end;
        }

        invalid_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(invalid_params_local, invalid_params) {
            if (!cJSON_IsObject(invalid_params_local)) {
                ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [invalid_params]");
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

    supported_features = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    access_token_error = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "accessTokenError");
    if (access_token_error) {
    access_token_error_local_nonprim = OpenAPI_access_token_err_parseFromJSON(access_token_error);
    if (!access_token_error_local_nonprim) {
        ogs_error("OpenAPI_access_token_err_parseFromJSON failed [access_token_error]");
        goto end;
    }
    }

    access_token_request = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "accessTokenRequest");
    if (access_token_request) {
    access_token_request_local_nonprim = OpenAPI_access_token_req_parseFromJSON(access_token_request);
    if (!access_token_request_local_nonprim) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON failed [access_token_request]");
        goto end;
    }
    }

    nrf_id = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "nrfId");
    if (nrf_id) {
    if (!cJSON_IsString(nrf_id) && !cJSON_IsNull(nrf_id)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [nrf_id]");
        goto end;
    }
    }

    remote_error = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "remoteError");
    if (remote_error) {
    if (!cJSON_IsBool(remote_error)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [remote_error]");
        goto end;
    }
    }

    max_waiting_time = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "maxWaitingTime");
    if (max_waiting_time) {
    if (!cJSON_IsNumber(max_waiting_time)) {
        ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [max_waiting_time]");
        goto end;
    }
    }

    transfer_mt_data_error_local_var = OpenAPI_transfer_mt_data_error_create (
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
        remote_error ? true : false,
        remote_error ? remote_error->valueint : 0,
        max_waiting_time ? true : false,
        max_waiting_time ? max_waiting_time->valuedouble : 0
    );

    return transfer_mt_data_error_local_var;
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
    return NULL;
}

OpenAPI_transfer_mt_data_error_t *OpenAPI_transfer_mt_data_error_copy(OpenAPI_transfer_mt_data_error_t *dst, OpenAPI_transfer_mt_data_error_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_transfer_mt_data_error_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed");
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

    OpenAPI_transfer_mt_data_error_free(dst);
    dst = OpenAPI_transfer_mt_data_error_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

