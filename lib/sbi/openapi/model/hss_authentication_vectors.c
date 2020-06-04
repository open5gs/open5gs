
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_authentication_vectors.h"

OpenAPI_hss_authentication_vectors_t *OpenAPI_hss_authentication_vectors_create(
    )
{
    OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors_local_var = OpenAPI_malloc(sizeof(OpenAPI_hss_authentication_vectors_t));
    if (!hss_authentication_vectors_local_var) {
        return NULL;
    }

    return hss_authentication_vectors_local_var;
}

void OpenAPI_hss_authentication_vectors_free(OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors)
{
    if (NULL == hss_authentication_vectors) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hss_authentication_vectors);
}

cJSON *OpenAPI_hss_authentication_vectors_convertToJSON(OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors)
{
    cJSON *item = NULL;

    if (hss_authentication_vectors == NULL) {
        ogs_error("OpenAPI_hss_authentication_vectors_convertToJSON() failed [HssAuthenticationVectors]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_hss_authentication_vectors_t *OpenAPI_hss_authentication_vectors_parseFromJSON(cJSON *hss_authentication_vectorsJSON)
{
    OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors_local_var = NULL;
    hss_authentication_vectors_local_var = OpenAPI_hss_authentication_vectors_create (
        );

    return hss_authentication_vectors_local_var;
end:
    return NULL;
}

