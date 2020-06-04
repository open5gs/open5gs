
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_confidentiality.h"

OpenAPI_up_confidentiality_t *OpenAPI_up_confidentiality_create(
    )
{
    OpenAPI_up_confidentiality_t *up_confidentiality_local_var = OpenAPI_malloc(sizeof(OpenAPI_up_confidentiality_t));
    if (!up_confidentiality_local_var) {
        return NULL;
    }

    return up_confidentiality_local_var;
}

void OpenAPI_up_confidentiality_free(OpenAPI_up_confidentiality_t *up_confidentiality)
{
    if (NULL == up_confidentiality) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(up_confidentiality);
}

cJSON *OpenAPI_up_confidentiality_convertToJSON(OpenAPI_up_confidentiality_t *up_confidentiality)
{
    cJSON *item = NULL;

    if (up_confidentiality == NULL) {
        ogs_error("OpenAPI_up_confidentiality_convertToJSON() failed [UpConfidentiality]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_up_confidentiality_t *OpenAPI_up_confidentiality_parseFromJSON(cJSON *up_confidentialityJSON)
{
    OpenAPI_up_confidentiality_t *up_confidentiality_local_var = NULL;
    up_confidentiality_local_var = OpenAPI_up_confidentiality_create (
        );

    return up_confidentiality_local_var;
end:
    return NULL;
}

