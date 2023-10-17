
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_ind.h"

OpenAPI_data_ind_t *OpenAPI_data_ind_create(
)
{
    OpenAPI_data_ind_t *data_ind_local_var = ogs_malloc(sizeof(OpenAPI_data_ind_t));
    ogs_assert(data_ind_local_var);


    return data_ind_local_var;
}

void OpenAPI_data_ind_free(OpenAPI_data_ind_t *data_ind)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == data_ind) {
        return;
    }
    ogs_free(data_ind);
}

cJSON *OpenAPI_data_ind_convertToJSON(OpenAPI_data_ind_t *data_ind)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_lnode_t *node = NULL;
    data_ind_local_var = OpenAPI_data_ind_create (
    );

    return data_ind_local_var;
end:
    return NULL;
}

OpenAPI_data_ind_t *OpenAPI_data_ind_copy(OpenAPI_data_ind_t *dst, OpenAPI_data_ind_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_data_ind_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_data_ind_convertToJSON() failed");
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

    OpenAPI_data_ind_free(dst);
    dst = OpenAPI_data_ind_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

