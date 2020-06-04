
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "periodicity.h"

OpenAPI_periodicity_t *OpenAPI_periodicity_create(
    )
{
    OpenAPI_periodicity_t *periodicity_local_var = OpenAPI_malloc(sizeof(OpenAPI_periodicity_t));
    if (!periodicity_local_var) {
        return NULL;
    }

    return periodicity_local_var;
}

void OpenAPI_periodicity_free(OpenAPI_periodicity_t *periodicity)
{
    if (NULL == periodicity) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(periodicity);
}

cJSON *OpenAPI_periodicity_convertToJSON(OpenAPI_periodicity_t *periodicity)
{
    cJSON *item = NULL;

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
    periodicity_local_var = OpenAPI_periodicity_create (
        );

    return periodicity_local_var;
end:
    return NULL;
}

