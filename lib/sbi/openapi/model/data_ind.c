
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_ind.h"

OpenAPI_data_ind_t *OpenAPI_data_ind_create(
    )
{
    OpenAPI_data_ind_t *data_ind_local_var = OpenAPI_malloc(sizeof(OpenAPI_data_ind_t));
    if (!data_ind_local_var) {
        return NULL;
    }

    return data_ind_local_var;
}

void OpenAPI_data_ind_free(OpenAPI_data_ind_t *data_ind)
{
    if (NULL == data_ind) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(data_ind);
}

cJSON *OpenAPI_data_ind_convertToJSON(OpenAPI_data_ind_t *data_ind)
{
    cJSON *item = NULL;

    if (data_ind == NULL) {
        ogs_error("OpenAPI_data_ind_convertToJSON() failed [DataInd]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_data_ind_t *OpenAPI_data_ind_parseFromJSON(cJSON *data_indJSON)
{
    OpenAPI_data_ind_t *data_ind_local_var = NULL;
    data_ind_local_var = OpenAPI_data_ind_create (
        );

    return data_ind_local_var;
end:
    return NULL;
}

