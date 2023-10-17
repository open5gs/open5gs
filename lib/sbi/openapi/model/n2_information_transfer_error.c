
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_information_transfer_error.h"

OpenAPI_n2_information_transfer_error_t *OpenAPI_n2_information_transfer_error_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_pws_error_data_t *pws_error_info
)
{
    OpenAPI_n2_information_transfer_error_t *n2_information_transfer_error_local_var = ogs_malloc(sizeof(OpenAPI_n2_information_transfer_error_t));
    ogs_assert(n2_information_transfer_error_local_var);

    n2_information_transfer_error_local_var->error = error;
    n2_information_transfer_error_local_var->pws_error_info = pws_error_info;

    return n2_information_transfer_error_local_var;
}

void OpenAPI_n2_information_transfer_error_free(OpenAPI_n2_information_transfer_error_t *n2_information_transfer_error)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_information_transfer_error) {
        return;
    }
    if (n2_information_transfer_error->error) {
        OpenAPI_problem_details_free(n2_information_transfer_error->error);
        n2_information_transfer_error->error = NULL;
    }
    if (n2_information_transfer_error->pws_error_info) {
        OpenAPI_pws_error_data_free(n2_information_transfer_error->pws_error_info);
        n2_information_transfer_error->pws_error_info = NULL;
    }
    ogs_free(n2_information_transfer_error);
}

cJSON *OpenAPI_n2_information_transfer_error_convertToJSON(OpenAPI_n2_information_transfer_error_t *n2_information_transfer_error)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_information_transfer_error == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_error_convertToJSON() failed [N2InformationTransferError]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n2_information_transfer_error->error) {
        ogs_error("OpenAPI_n2_information_transfer_error_convertToJSON() failed [error]");
        return NULL;
    }
    cJSON *error_local_JSON = OpenAPI_problem_details_convertToJSON(n2_information_transfer_error->error);
    if (error_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_error_convertToJSON() failed [error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "error", error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_error_convertToJSON() failed [error]");
        goto end;
    }

    if (n2_information_transfer_error->pws_error_info) {
    cJSON *pws_error_info_local_JSON = OpenAPI_pws_error_data_convertToJSON(n2_information_transfer_error->pws_error_info);
    if (pws_error_info_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_error_convertToJSON() failed [pws_error_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pwsErrorInfo", pws_error_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_error_convertToJSON() failed [pws_error_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n2_information_transfer_error_t *OpenAPI_n2_information_transfer_error_parseFromJSON(cJSON *n2_information_transfer_errorJSON)
{
    OpenAPI_n2_information_transfer_error_t *n2_information_transfer_error_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *error = NULL;
    OpenAPI_problem_details_t *error_local_nonprim = NULL;
    cJSON *pws_error_info = NULL;
    OpenAPI_pws_error_data_t *pws_error_info_local_nonprim = NULL;
    error = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_errorJSON, "error");
    if (!error) {
        ogs_error("OpenAPI_n2_information_transfer_error_parseFromJSON() failed [error]");
        goto end;
    }
    error_local_nonprim = OpenAPI_problem_details_parseFromJSON(error);
    if (!error_local_nonprim) {
        ogs_error("OpenAPI_problem_details_parseFromJSON failed [error]");
        goto end;
    }

    pws_error_info = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_errorJSON, "pwsErrorInfo");
    if (pws_error_info) {
    pws_error_info_local_nonprim = OpenAPI_pws_error_data_parseFromJSON(pws_error_info);
    if (!pws_error_info_local_nonprim) {
        ogs_error("OpenAPI_pws_error_data_parseFromJSON failed [pws_error_info]");
        goto end;
    }
    }

    n2_information_transfer_error_local_var = OpenAPI_n2_information_transfer_error_create (
        error_local_nonprim,
        pws_error_info ? pws_error_info_local_nonprim : NULL
    );

    return n2_information_transfer_error_local_var;
end:
    if (error_local_nonprim) {
        OpenAPI_problem_details_free(error_local_nonprim);
        error_local_nonprim = NULL;
    }
    if (pws_error_info_local_nonprim) {
        OpenAPI_pws_error_data_free(pws_error_info_local_nonprim);
        pws_error_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n2_information_transfer_error_t *OpenAPI_n2_information_transfer_error_copy(OpenAPI_n2_information_transfer_error_t *dst, OpenAPI_n2_information_transfer_error_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_information_transfer_error_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_information_transfer_error_convertToJSON() failed");
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

    OpenAPI_n2_information_transfer_error_free(dst);
    dst = OpenAPI_n2_information_transfer_error_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

