
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pws_error_data.h"

OpenAPI_pws_error_data_t *OpenAPI_pws_error_data_create(
    int namf_cause
    )
{
    OpenAPI_pws_error_data_t *pws_error_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_pws_error_data_t));
    if (!pws_error_data_local_var) {
        return NULL;
    }
    pws_error_data_local_var->namf_cause = namf_cause;

    return pws_error_data_local_var;
}

void OpenAPI_pws_error_data_free(OpenAPI_pws_error_data_t *pws_error_data)
{
    if (NULL == pws_error_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pws_error_data);
}

cJSON *OpenAPI_pws_error_data_convertToJSON(OpenAPI_pws_error_data_t *pws_error_data)
{
    cJSON *item = NULL;

    if (pws_error_data == NULL) {
        ogs_error("OpenAPI_pws_error_data_convertToJSON() failed [PWSErrorData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pws_error_data->namf_cause) {
        ogs_error("OpenAPI_pws_error_data_convertToJSON() failed [namf_cause]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "namfCause", pws_error_data->namf_cause) == NULL) {
        ogs_error("OpenAPI_pws_error_data_convertToJSON() failed [namf_cause]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pws_error_data_t *OpenAPI_pws_error_data_parseFromJSON(cJSON *pws_error_dataJSON)
{
    OpenAPI_pws_error_data_t *pws_error_data_local_var = NULL;
    cJSON *namf_cause = cJSON_GetObjectItemCaseSensitive(pws_error_dataJSON, "namfCause");
    if (!namf_cause) {
        ogs_error("OpenAPI_pws_error_data_parseFromJSON() failed [namf_cause]");
        goto end;
    }


    if (!cJSON_IsNumber(namf_cause)) {
        ogs_error("OpenAPI_pws_error_data_parseFromJSON() failed [namf_cause]");
        goto end;
    }

    pws_error_data_local_var = OpenAPI_pws_error_data_create (
        namf_cause->valuedouble
        );

    return pws_error_data_local_var;
end:
    return NULL;
}

OpenAPI_pws_error_data_t *OpenAPI_pws_error_data_copy(OpenAPI_pws_error_data_t *dst, OpenAPI_pws_error_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pws_error_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pws_error_data_convertToJSON() failed");
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

    OpenAPI_pws_error_data_free(dst);
    dst = OpenAPI_pws_error_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

