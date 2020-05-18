
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_set_id.h"

OpenAPI_data_set_id_t *OpenAPI_data_set_id_create(
    )
{
    OpenAPI_data_set_id_t *data_set_id_local_var = OpenAPI_malloc(sizeof(OpenAPI_data_set_id_t));
    if (!data_set_id_local_var) {
        return NULL;
    }

    return data_set_id_local_var;
}

void OpenAPI_data_set_id_free(OpenAPI_data_set_id_t *data_set_id)
{
    if (NULL == data_set_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(data_set_id);
}

cJSON *OpenAPI_data_set_id_convertToJSON(OpenAPI_data_set_id_t *data_set_id)
{
    cJSON *item = NULL;

    if (data_set_id == NULL) {
        ogs_error("OpenAPI_data_set_id_convertToJSON() failed [DataSetId]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_data_set_id_t *OpenAPI_data_set_id_parseFromJSON(cJSON *data_set_idJSON)
{
    OpenAPI_data_set_id_t *data_set_id_local_var = NULL;
    data_set_id_local_var = OpenAPI_data_set_id_create (
        );

    return data_set_id_local_var;
end:
    return NULL;
}

