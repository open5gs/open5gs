
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_authentication_vectors.h"

OpenAPI_hss_authentication_vectors_t *OpenAPI_hss_authentication_vectors_create(
)
{
    OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors_local_var = ogs_malloc(sizeof(OpenAPI_hss_authentication_vectors_t));
    ogs_assert(hss_authentication_vectors_local_var);


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

OpenAPI_hss_authentication_vectors_t *OpenAPI_hss_authentication_vectors_copy(OpenAPI_hss_authentication_vectors_t *dst, OpenAPI_hss_authentication_vectors_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hss_authentication_vectors_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hss_authentication_vectors_convertToJSON() failed");
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

    OpenAPI_hss_authentication_vectors_free(dst);
    dst = OpenAPI_hss_authentication_vectors_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

