
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "psa_indication.h"

OpenAPI_psa_indication_t *OpenAPI_psa_indication_create(
    )
{
    OpenAPI_psa_indication_t *psa_indication_local_var = OpenAPI_malloc(sizeof(OpenAPI_psa_indication_t));
    if (!psa_indication_local_var) {
        return NULL;
    }

    return psa_indication_local_var;
}

void OpenAPI_psa_indication_free(OpenAPI_psa_indication_t *psa_indication)
{
    if (NULL == psa_indication) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(psa_indication);
}

cJSON *OpenAPI_psa_indication_convertToJSON(OpenAPI_psa_indication_t *psa_indication)
{
    cJSON *item = NULL;

    if (psa_indication == NULL) {
        ogs_error("OpenAPI_psa_indication_convertToJSON() failed [PsaIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_psa_indication_t *OpenAPI_psa_indication_parseFromJSON(cJSON *psa_indicationJSON)
{
    OpenAPI_psa_indication_t *psa_indication_local_var = NULL;
    psa_indication_local_var = OpenAPI_psa_indication_create (
        );

    return psa_indication_local_var;
end:
    return NULL;
}

OpenAPI_psa_indication_t *OpenAPI_psa_indication_copy(OpenAPI_psa_indication_t *dst, OpenAPI_psa_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_psa_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_psa_indication_convertToJSON() failed");
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

    OpenAPI_psa_indication_free(dst);
    dst = OpenAPI_psa_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

