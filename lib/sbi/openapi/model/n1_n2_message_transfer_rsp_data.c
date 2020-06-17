
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_n2_message_transfer_rsp_data.h"

OpenAPI_n1_n2_message_transfer_rsp_data_t *OpenAPI_n1_n2_message_transfer_rsp_data_create(
    OpenAPI_n1_n2_message_transfer_cause_e cause,
    char *supported_features
    )
{
    OpenAPI_n1_n2_message_transfer_rsp_data_t *n1_n2_message_transfer_rsp_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_n1_n2_message_transfer_rsp_data_t));
    if (!n1_n2_message_transfer_rsp_data_local_var) {
        return NULL;
    }
    n1_n2_message_transfer_rsp_data_local_var->cause = cause;
    n1_n2_message_transfer_rsp_data_local_var->supported_features = supported_features;

    return n1_n2_message_transfer_rsp_data_local_var;
}

void OpenAPI_n1_n2_message_transfer_rsp_data_free(OpenAPI_n1_n2_message_transfer_rsp_data_t *n1_n2_message_transfer_rsp_data)
{
    if (NULL == n1_n2_message_transfer_rsp_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(n1_n2_message_transfer_rsp_data->supported_features);
    ogs_free(n1_n2_message_transfer_rsp_data);
}

cJSON *OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON(OpenAPI_n1_n2_message_transfer_rsp_data_t *n1_n2_message_transfer_rsp_data)
{
    cJSON *item = NULL;

    if (n1_n2_message_transfer_rsp_data == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON() failed [N1N2MessageTransferRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n1_n2_message_transfer_rsp_data->cause) {
        ogs_error("OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON() failed [cause]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "cause", OpenAPI_n1_n2_message_transfer_cause_ToString(n1_n2_message_transfer_rsp_data->cause)) == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON() failed [cause]");
        goto end;
    }

    if (n1_n2_message_transfer_rsp_data->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", n1_n2_message_transfer_rsp_data->supported_features) == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n1_n2_message_transfer_rsp_data_t *OpenAPI_n1_n2_message_transfer_rsp_data_parseFromJSON(cJSON *n1_n2_message_transfer_rsp_dataJSON)
{
    OpenAPI_n1_n2_message_transfer_rsp_data_t *n1_n2_message_transfer_rsp_data_local_var = NULL;
    cJSON *cause = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_rsp_dataJSON, "cause");
    if (!cause) {
        ogs_error("OpenAPI_n1_n2_message_transfer_rsp_data_parseFromJSON() failed [cause]");
        goto end;
    }

    OpenAPI_n1_n2_message_transfer_cause_e causeVariable;

    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_n1_n2_message_transfer_rsp_data_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_n1_n2_message_transfer_cause_FromString(cause->valuestring);

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_rsp_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_n1_n2_message_transfer_rsp_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    n1_n2_message_transfer_rsp_data_local_var = OpenAPI_n1_n2_message_transfer_rsp_data_create (
        causeVariable,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
        );

    return n1_n2_message_transfer_rsp_data_local_var;
end:
    return NULL;
}

OpenAPI_n1_n2_message_transfer_rsp_data_t *OpenAPI_n1_n2_message_transfer_rsp_data_copy(OpenAPI_n1_n2_message_transfer_rsp_data_t *dst, OpenAPI_n1_n2_message_transfer_rsp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_n2_message_transfer_rsp_data_convertToJSON() failed");
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

    OpenAPI_n1_n2_message_transfer_rsp_data_free(dst);
    dst = OpenAPI_n1_n2_message_transfer_rsp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

