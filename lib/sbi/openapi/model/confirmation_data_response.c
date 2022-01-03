
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "confirmation_data_response.h"

OpenAPI_confirmation_data_response_t *OpenAPI_confirmation_data_response_create(
    OpenAPI_auth_result_e auth_result,
    char *supi,
    char *kseaf
)
{
    OpenAPI_confirmation_data_response_t *confirmation_data_response_local_var = ogs_malloc(sizeof(OpenAPI_confirmation_data_response_t));
    ogs_assert(confirmation_data_response_local_var);

    confirmation_data_response_local_var->auth_result = auth_result;
    confirmation_data_response_local_var->supi = supi;
    confirmation_data_response_local_var->kseaf = kseaf;

    return confirmation_data_response_local_var;
}

void OpenAPI_confirmation_data_response_free(OpenAPI_confirmation_data_response_t *confirmation_data_response)
{
    if (NULL == confirmation_data_response) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(confirmation_data_response->supi);
    ogs_free(confirmation_data_response->kseaf);
    ogs_free(confirmation_data_response);
}

cJSON *OpenAPI_confirmation_data_response_convertToJSON(OpenAPI_confirmation_data_response_t *confirmation_data_response)
{
    cJSON *item = NULL;

    if (confirmation_data_response == NULL) {
        ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed [ConfirmationDataResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "authResult", OpenAPI_auth_result_ToString(confirmation_data_response->auth_result)) == NULL) {
        ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed [auth_result]");
        goto end;
    }

    if (confirmation_data_response->supi) {
    if (cJSON_AddStringToObject(item, "supi", confirmation_data_response->supi) == NULL) {
        ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (confirmation_data_response->kseaf) {
    if (cJSON_AddStringToObject(item, "kseaf", confirmation_data_response->kseaf) == NULL) {
        ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed [kseaf]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_confirmation_data_response_t *OpenAPI_confirmation_data_response_parseFromJSON(cJSON *confirmation_data_responseJSON)
{
    OpenAPI_confirmation_data_response_t *confirmation_data_response_local_var = NULL;
    cJSON *auth_result = cJSON_GetObjectItemCaseSensitive(confirmation_data_responseJSON, "authResult");
    if (!auth_result) {
        ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [auth_result]");
        goto end;
    }

    OpenAPI_auth_result_e auth_resultVariable;
    if (!cJSON_IsString(auth_result)) {
        ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [auth_result]");
        goto end;
    }
    auth_resultVariable = OpenAPI_auth_result_FromString(auth_result->valuestring);

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(confirmation_data_responseJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    cJSON *kseaf = cJSON_GetObjectItemCaseSensitive(confirmation_data_responseJSON, "kseaf");

    if (kseaf) {
    if (!cJSON_IsString(kseaf)) {
        ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [kseaf]");
        goto end;
    }
    }

    confirmation_data_response_local_var = OpenAPI_confirmation_data_response_create (
        auth_resultVariable,
        supi ? ogs_strdup(supi->valuestring) : NULL,
        kseaf ? ogs_strdup(kseaf->valuestring) : NULL
    );

    return confirmation_data_response_local_var;
end:
    return NULL;
}

OpenAPI_confirmation_data_response_t *OpenAPI_confirmation_data_response_copy(OpenAPI_confirmation_data_response_t *dst, OpenAPI_confirmation_data_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_confirmation_data_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed");
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

    OpenAPI_confirmation_data_response_free(dst);
    dst = OpenAPI_confirmation_data_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

