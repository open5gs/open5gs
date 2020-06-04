
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "confirmation_data.h"

OpenAPI_confirmation_data_t *OpenAPI_confirmation_data_create(
    char *res_star
    )
{
    OpenAPI_confirmation_data_t *confirmation_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_confirmation_data_t));
    if (!confirmation_data_local_var) {
        return NULL;
    }
    confirmation_data_local_var->res_star = res_star;

    return confirmation_data_local_var;
}

void OpenAPI_confirmation_data_free(OpenAPI_confirmation_data_t *confirmation_data)
{
    if (NULL == confirmation_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(confirmation_data->res_star);
    ogs_free(confirmation_data);
}

cJSON *OpenAPI_confirmation_data_convertToJSON(OpenAPI_confirmation_data_t *confirmation_data)
{
    cJSON *item = NULL;

    if (confirmation_data == NULL) {
        ogs_error("OpenAPI_confirmation_data_convertToJSON() failed [ConfirmationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!confirmation_data->res_star) {
        ogs_error("OpenAPI_confirmation_data_convertToJSON() failed [res_star]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "resStar", confirmation_data->res_star) == NULL) {
        ogs_error("OpenAPI_confirmation_data_convertToJSON() failed [res_star]");
        goto end;
    }

end:
    return item;
}

OpenAPI_confirmation_data_t *OpenAPI_confirmation_data_parseFromJSON(cJSON *confirmation_dataJSON)
{
    OpenAPI_confirmation_data_t *confirmation_data_local_var = NULL;
    cJSON *res_star = cJSON_GetObjectItemCaseSensitive(confirmation_dataJSON, "resStar");
    if (!res_star) {
        ogs_error("OpenAPI_confirmation_data_parseFromJSON() failed [res_star]");
        goto end;
    }


    if (!cJSON_IsString(res_star)) {
        ogs_error("OpenAPI_confirmation_data_parseFromJSON() failed [res_star]");
        goto end;
    }

    confirmation_data_local_var = OpenAPI_confirmation_data_create (
        ogs_strdup(res_star->valuestring)
        );

    return confirmation_data_local_var;
end:
    return NULL;
}

