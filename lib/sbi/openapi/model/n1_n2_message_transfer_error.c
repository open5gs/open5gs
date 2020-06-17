
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_n2_message_transfer_error.h"

OpenAPI_n1_n2_message_transfer_error_t *OpenAPI_n1_n2_message_transfer_error_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_n1_n2_msg_txfr_err_detail_t *err_info
    )
{
    OpenAPI_n1_n2_message_transfer_error_t *n1_n2_message_transfer_error_local_var = OpenAPI_malloc(sizeof(OpenAPI_n1_n2_message_transfer_error_t));
    if (!n1_n2_message_transfer_error_local_var) {
        return NULL;
    }
    n1_n2_message_transfer_error_local_var->error = error;
    n1_n2_message_transfer_error_local_var->err_info = err_info;

    return n1_n2_message_transfer_error_local_var;
}

void OpenAPI_n1_n2_message_transfer_error_free(OpenAPI_n1_n2_message_transfer_error_t *n1_n2_message_transfer_error)
{
    if (NULL == n1_n2_message_transfer_error) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_problem_details_free(n1_n2_message_transfer_error->error);
    OpenAPI_n1_n2_msg_txfr_err_detail_free(n1_n2_message_transfer_error->err_info);
    ogs_free(n1_n2_message_transfer_error);
}

cJSON *OpenAPI_n1_n2_message_transfer_error_convertToJSON(OpenAPI_n1_n2_message_transfer_error_t *n1_n2_message_transfer_error)
{
    cJSON *item = NULL;

    if (n1_n2_message_transfer_error == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_error_convertToJSON() failed [N1N2MessageTransferError]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n1_n2_message_transfer_error->error) {
        ogs_error("OpenAPI_n1_n2_message_transfer_error_convertToJSON() failed [error]");
        goto end;
    }
    cJSON *error_local_JSON = OpenAPI_problem_details_convertToJSON(n1_n2_message_transfer_error->error);
    if (error_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_error_convertToJSON() failed [error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "error", error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_error_convertToJSON() failed [error]");
        goto end;
    }

    if (n1_n2_message_transfer_error->err_info) {
        cJSON *err_info_local_JSON = OpenAPI_n1_n2_msg_txfr_err_detail_convertToJSON(n1_n2_message_transfer_error->err_info);
        if (err_info_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_error_convertToJSON() failed [err_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "errInfo", err_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_n2_message_transfer_error_convertToJSON() failed [err_info]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n1_n2_message_transfer_error_t *OpenAPI_n1_n2_message_transfer_error_parseFromJSON(cJSON *n1_n2_message_transfer_errorJSON)
{
    OpenAPI_n1_n2_message_transfer_error_t *n1_n2_message_transfer_error_local_var = NULL;
    cJSON *error = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_errorJSON, "error");
    if (!error) {
        ogs_error("OpenAPI_n1_n2_message_transfer_error_parseFromJSON() failed [error]");
        goto end;
    }

    OpenAPI_problem_details_t *error_local_nonprim = NULL;

    error_local_nonprim = OpenAPI_problem_details_parseFromJSON(error);

    cJSON *err_info = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_errorJSON, "errInfo");

    OpenAPI_n1_n2_msg_txfr_err_detail_t *err_info_local_nonprim = NULL;
    if (err_info) {
        err_info_local_nonprim = OpenAPI_n1_n2_msg_txfr_err_detail_parseFromJSON(err_info);
    }

    n1_n2_message_transfer_error_local_var = OpenAPI_n1_n2_message_transfer_error_create (
        error_local_nonprim,
        err_info ? err_info_local_nonprim : NULL
        );

    return n1_n2_message_transfer_error_local_var;
end:
    return NULL;
}

OpenAPI_n1_n2_message_transfer_error_t *OpenAPI_n1_n2_message_transfer_error_copy(OpenAPI_n1_n2_message_transfer_error_t *dst, OpenAPI_n1_n2_message_transfer_error_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_n2_message_transfer_error_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_n2_message_transfer_error_convertToJSON() failed");
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

    OpenAPI_n1_n2_message_transfer_error_free(dst);
    dst = OpenAPI_n1_n2_message_transfer_error_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

