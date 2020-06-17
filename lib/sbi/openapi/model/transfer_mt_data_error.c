
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transfer_mt_data_error.h"

OpenAPI_transfer_mt_data_error_t *OpenAPI_transfer_mt_data_error_create(
    char *type,
    char *title,
    int status,
    char *detail,
    char *instance,
    char *cause,
    OpenAPI_list_t *invalid_params,
    char *supported_features,
    int max_waiting_time
    )
{
    OpenAPI_transfer_mt_data_error_t *transfer_mt_data_error_local_var = OpenAPI_malloc(sizeof(OpenAPI_transfer_mt_data_error_t));
    if (!transfer_mt_data_error_local_var) {
        return NULL;
    }
    transfer_mt_data_error_local_var->type = type;
    transfer_mt_data_error_local_var->title = title;
    transfer_mt_data_error_local_var->status = status;
    transfer_mt_data_error_local_var->detail = detail;
    transfer_mt_data_error_local_var->instance = instance;
    transfer_mt_data_error_local_var->cause = cause;
    transfer_mt_data_error_local_var->invalid_params = invalid_params;
    transfer_mt_data_error_local_var->supported_features = supported_features;
    transfer_mt_data_error_local_var->max_waiting_time = max_waiting_time;

    return transfer_mt_data_error_local_var;
}

void OpenAPI_transfer_mt_data_error_free(OpenAPI_transfer_mt_data_error_t *transfer_mt_data_error)
{
    if (NULL == transfer_mt_data_error) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(transfer_mt_data_error->type);
    ogs_free(transfer_mt_data_error->title);
    ogs_free(transfer_mt_data_error->detail);
    ogs_free(transfer_mt_data_error->instance);
    ogs_free(transfer_mt_data_error->cause);
    OpenAPI_list_for_each(transfer_mt_data_error->invalid_params, node) {
        OpenAPI_invalid_param_free(node->data);
    }
    OpenAPI_list_free(transfer_mt_data_error->invalid_params);
    ogs_free(transfer_mt_data_error->supported_features);
    ogs_free(transfer_mt_data_error);
}

cJSON *OpenAPI_transfer_mt_data_error_convertToJSON(OpenAPI_transfer_mt_data_error_t *transfer_mt_data_error)
{
    cJSON *item = NULL;

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

    if (transfer_mt_data_error->status) {
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

        OpenAPI_lnode_t *invalid_params_node;
        if (transfer_mt_data_error->invalid_params) {
            OpenAPI_list_for_each(transfer_mt_data_error->invalid_params, invalid_params_node) {
                cJSON *itemLocal = OpenAPI_invalid_param_convertToJSON(invalid_params_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [invalid_params]");
                    goto end;
                }
                cJSON_AddItemToArray(invalid_paramsList, itemLocal);
            }
        }
    }

    if (transfer_mt_data_error->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", transfer_mt_data_error->supported_features) == NULL) {
            ogs_error("OpenAPI_transfer_mt_data_error_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (transfer_mt_data_error->max_waiting_time) {
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
    cJSON *type = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "type");

    if (type) {
        if (!cJSON_IsString(type)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [type]");
            goto end;
        }
    }

    cJSON *title = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "title");

    if (title) {
        if (!cJSON_IsString(title)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [title]");
            goto end;
        }
    }

    cJSON *status = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "status");

    if (status) {
        if (!cJSON_IsNumber(status)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [status]");
            goto end;
        }
    }

    cJSON *detail = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "detail");

    if (detail) {
        if (!cJSON_IsString(detail)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [detail]");
            goto end;
        }
    }

    cJSON *instance = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "instance");

    if (instance) {
        if (!cJSON_IsString(instance)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [instance]");
            goto end;
        }
    }

    cJSON *cause = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "cause");

    if (cause) {
        if (!cJSON_IsString(cause)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [cause]");
            goto end;
        }
    }

    cJSON *invalid_params = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "invalidParams");

    OpenAPI_list_t *invalid_paramsList;
    if (invalid_params) {
        cJSON *invalid_params_local_nonprimitive;
        if (!cJSON_IsArray(invalid_params)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [invalid_params]");
            goto end;
        }

        invalid_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(invalid_params_local_nonprimitive, invalid_params ) {
            if (!cJSON_IsObject(invalid_params_local_nonprimitive)) {
                ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [invalid_params]");
                goto end;
            }
            OpenAPI_invalid_param_t *invalid_paramsItem = OpenAPI_invalid_param_parseFromJSON(invalid_params_local_nonprimitive);

            OpenAPI_list_add(invalid_paramsList, invalid_paramsItem);
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *max_waiting_time = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_errorJSON, "maxWaitingTime");

    if (max_waiting_time) {
        if (!cJSON_IsNumber(max_waiting_time)) {
            ogs_error("OpenAPI_transfer_mt_data_error_parseFromJSON() failed [max_waiting_time]");
            goto end;
        }
    }

    transfer_mt_data_error_local_var = OpenAPI_transfer_mt_data_error_create (
        type ? ogs_strdup(type->valuestring) : NULL,
        title ? ogs_strdup(title->valuestring) : NULL,
        status ? status->valuedouble : 0,
        detail ? ogs_strdup(detail->valuestring) : NULL,
        instance ? ogs_strdup(instance->valuestring) : NULL,
        cause ? ogs_strdup(cause->valuestring) : NULL,
        invalid_params ? invalid_paramsList : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        max_waiting_time ? max_waiting_time->valuedouble : 0
        );

    return transfer_mt_data_error_local_var;
end:
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

