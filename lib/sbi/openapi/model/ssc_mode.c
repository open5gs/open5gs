
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ssc_mode.h"

OpenAPI_ssc_mode_t *OpenAPI_ssc_mode_create(
    )
{
    OpenAPI_ssc_mode_t *ssc_mode_local_var = OpenAPI_malloc(sizeof(OpenAPI_ssc_mode_t));
    if (!ssc_mode_local_var) {
        return NULL;
    }

    return ssc_mode_local_var;
}

void OpenAPI_ssc_mode_free(OpenAPI_ssc_mode_t *ssc_mode)
{
    if (NULL == ssc_mode) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ssc_mode);
}

cJSON *OpenAPI_ssc_mode_convertToJSON(OpenAPI_ssc_mode_t *ssc_mode)
{
    cJSON *item = NULL;

    if (ssc_mode == NULL) {
        ogs_error("OpenAPI_ssc_mode_convertToJSON() failed [SscMode]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_ssc_mode_t *OpenAPI_ssc_mode_parseFromJSON(cJSON *ssc_modeJSON)
{
    OpenAPI_ssc_mode_t *ssc_mode_local_var = NULL;
    ssc_mode_local_var = OpenAPI_ssc_mode_create (
        );

    return ssc_mode_local_var;
end:
    return NULL;
}

