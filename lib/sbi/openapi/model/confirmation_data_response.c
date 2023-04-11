
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "confirmation_data_response.h"

OpenAPI_confirmation_data_response_t *OpenAPI_confirmation_data_response_create(
    OpenAPI_auth_result_e auth_result,
    char *supi,
    char *kseaf,
    OpenAPI_list_t *pvs_info
)
{
    OpenAPI_confirmation_data_response_t *confirmation_data_response_local_var = ogs_malloc(sizeof(OpenAPI_confirmation_data_response_t));
    ogs_assert(confirmation_data_response_local_var);

    confirmation_data_response_local_var->auth_result = auth_result;
    confirmation_data_response_local_var->supi = supi;
    confirmation_data_response_local_var->kseaf = kseaf;
    confirmation_data_response_local_var->pvs_info = pvs_info;

    return confirmation_data_response_local_var;
}

void OpenAPI_confirmation_data_response_free(OpenAPI_confirmation_data_response_t *confirmation_data_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == confirmation_data_response) {
        return;
    }
    if (confirmation_data_response->supi) {
        ogs_free(confirmation_data_response->supi);
        confirmation_data_response->supi = NULL;
    }
    if (confirmation_data_response->kseaf) {
        ogs_free(confirmation_data_response->kseaf);
        confirmation_data_response->kseaf = NULL;
    }
    if (confirmation_data_response->pvs_info) {
        OpenAPI_list_for_each(confirmation_data_response->pvs_info, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(confirmation_data_response->pvs_info);
        confirmation_data_response->pvs_info = NULL;
    }
    ogs_free(confirmation_data_response);
}

cJSON *OpenAPI_confirmation_data_response_convertToJSON(OpenAPI_confirmation_data_response_t *confirmation_data_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (confirmation_data_response == NULL) {
        ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed [ConfirmationDataResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (confirmation_data_response->auth_result == OpenAPI_auth_result_NULL) {
        ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed [auth_result]");
        return NULL;
    }
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

    if (confirmation_data_response->pvs_info) {
    cJSON *pvs_infoList = cJSON_AddArrayToObject(item, "pvsInfo");
    if (pvs_infoList == NULL) {
        ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed [pvs_info]");
        goto end;
    }
    OpenAPI_list_for_each(confirmation_data_response->pvs_info, node) {
        cJSON *itemLocal = OpenAPI_server_addressing_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_confirmation_data_response_convertToJSON() failed [pvs_info]");
            goto end;
        }
        cJSON_AddItemToArray(pvs_infoList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_confirmation_data_response_t *OpenAPI_confirmation_data_response_parseFromJSON(cJSON *confirmation_data_responseJSON)
{
    OpenAPI_confirmation_data_response_t *confirmation_data_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *auth_result = NULL;
    OpenAPI_auth_result_e auth_resultVariable = 0;
    cJSON *supi = NULL;
    cJSON *kseaf = NULL;
    cJSON *pvs_info = NULL;
    OpenAPI_list_t *pvs_infoList = NULL;
    auth_result = cJSON_GetObjectItemCaseSensitive(confirmation_data_responseJSON, "authResult");
    if (!auth_result) {
        ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [auth_result]");
        goto end;
    }
    if (!cJSON_IsString(auth_result)) {
        ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [auth_result]");
        goto end;
    }
    auth_resultVariable = OpenAPI_auth_result_FromString(auth_result->valuestring);

    supi = cJSON_GetObjectItemCaseSensitive(confirmation_data_responseJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    kseaf = cJSON_GetObjectItemCaseSensitive(confirmation_data_responseJSON, "kseaf");
    if (kseaf) {
    if (!cJSON_IsString(kseaf) && !cJSON_IsNull(kseaf)) {
        ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [kseaf]");
        goto end;
    }
    }

    pvs_info = cJSON_GetObjectItemCaseSensitive(confirmation_data_responseJSON, "pvsInfo");
    if (pvs_info) {
        cJSON *pvs_info_local = NULL;
        if (!cJSON_IsArray(pvs_info)) {
            ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [pvs_info]");
            goto end;
        }

        pvs_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(pvs_info_local, pvs_info) {
            if (!cJSON_IsObject(pvs_info_local)) {
                ogs_error("OpenAPI_confirmation_data_response_parseFromJSON() failed [pvs_info]");
                goto end;
            }
            OpenAPI_server_addressing_info_t *pvs_infoItem = OpenAPI_server_addressing_info_parseFromJSON(pvs_info_local);
            if (!pvs_infoItem) {
                ogs_error("No pvs_infoItem");
                goto end;
            }
            OpenAPI_list_add(pvs_infoList, pvs_infoItem);
        }
    }

    confirmation_data_response_local_var = OpenAPI_confirmation_data_response_create (
        auth_resultVariable,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        kseaf && !cJSON_IsNull(kseaf) ? ogs_strdup(kseaf->valuestring) : NULL,
        pvs_info ? pvs_infoList : NULL
    );

    return confirmation_data_response_local_var;
end:
    if (pvs_infoList) {
        OpenAPI_list_for_each(pvs_infoList, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(pvs_infoList);
        pvs_infoList = NULL;
    }
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

