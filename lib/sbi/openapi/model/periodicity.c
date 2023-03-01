
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "periodicity.h"

OpenAPI_periodicity_t *OpenAPI_periodicity_create(
)
{
    OpenAPI_periodicity_t *periodicity_local_var = ogs_malloc(sizeof(OpenAPI_periodicity_t));
    ogs_assert(periodicity_local_var);


    return periodicity_local_var;
}

void OpenAPI_periodicity_free(OpenAPI_periodicity_t *periodicity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == periodicity) {
        return;
    }
    ogs_free(periodicity);
}

cJSON *OpenAPI_periodicity_convertToJSON(OpenAPI_periodicity_t *periodicity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (periodicity == NULL) {
        ogs_error("OpenAPI_periodicity_convertToJSON() failed [Periodicity]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_periodicity_t *OpenAPI_periodicity_parseFromJSON(cJSON *periodicityJSON)
{
    OpenAPI_periodicity_t *periodicity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    periodicity_local_var = OpenAPI_periodicity_create (
    );

    return periodicity_local_var;
end:
    return NULL;
}

OpenAPI_periodicity_t *OpenAPI_periodicity_copy(OpenAPI_periodicity_t *dst, OpenAPI_periodicity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_periodicity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_periodicity_convertToJSON() failed");
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

    OpenAPI_periodicity_free(dst);
    dst = OpenAPI_periodicity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

