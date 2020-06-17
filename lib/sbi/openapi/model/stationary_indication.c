
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stationary_indication.h"

OpenAPI_stationary_indication_t *OpenAPI_stationary_indication_create(
    )
{
    OpenAPI_stationary_indication_t *stationary_indication_local_var = OpenAPI_malloc(sizeof(OpenAPI_stationary_indication_t));
    if (!stationary_indication_local_var) {
        return NULL;
    }

    return stationary_indication_local_var;
}

void OpenAPI_stationary_indication_free(OpenAPI_stationary_indication_t *stationary_indication)
{
    if (NULL == stationary_indication) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(stationary_indication);
}

cJSON *OpenAPI_stationary_indication_convertToJSON(OpenAPI_stationary_indication_t *stationary_indication)
{
    cJSON *item = NULL;

    if (stationary_indication == NULL) {
        ogs_error("OpenAPI_stationary_indication_convertToJSON() failed [StationaryIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_stationary_indication_t *OpenAPI_stationary_indication_parseFromJSON(cJSON *stationary_indicationJSON)
{
    OpenAPI_stationary_indication_t *stationary_indication_local_var = NULL;
    stationary_indication_local_var = OpenAPI_stationary_indication_create (
        );

    return stationary_indication_local_var;
end:
    return NULL;
}

OpenAPI_stationary_indication_t *OpenAPI_stationary_indication_copy(OpenAPI_stationary_indication_t *dst, OpenAPI_stationary_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_stationary_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_stationary_indication_convertToJSON() failed");
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

    OpenAPI_stationary_indication_free(dst);
    dst = OpenAPI_stationary_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

