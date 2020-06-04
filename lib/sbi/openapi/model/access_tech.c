
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_tech.h"

OpenAPI_access_tech_t *OpenAPI_access_tech_create(
    )
{
    OpenAPI_access_tech_t *access_tech_local_var = OpenAPI_malloc(sizeof(OpenAPI_access_tech_t));
    if (!access_tech_local_var) {
        return NULL;
    }

    return access_tech_local_var;
}

void OpenAPI_access_tech_free(OpenAPI_access_tech_t *access_tech)
{
    if (NULL == access_tech) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(access_tech);
}

cJSON *OpenAPI_access_tech_convertToJSON(OpenAPI_access_tech_t *access_tech)
{
    cJSON *item = NULL;

    if (access_tech == NULL) {
        ogs_error("OpenAPI_access_tech_convertToJSON() failed [AccessTech]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_access_tech_t *OpenAPI_access_tech_parseFromJSON(cJSON *access_techJSON)
{
    OpenAPI_access_tech_t *access_tech_local_var = NULL;
    access_tech_local_var = OpenAPI_access_tech_create (
        );

    return access_tech_local_var;
end:
    return NULL;
}

