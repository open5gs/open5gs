
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "unavailable_access_indication.h"

OpenAPI_unavailable_access_indication_t *OpenAPI_unavailable_access_indication_create(
    )
{
    OpenAPI_unavailable_access_indication_t *unavailable_access_indication_local_var = OpenAPI_malloc(sizeof(OpenAPI_unavailable_access_indication_t));
    if (!unavailable_access_indication_local_var) {
        return NULL;
    }

    return unavailable_access_indication_local_var;
}

void OpenAPI_unavailable_access_indication_free(OpenAPI_unavailable_access_indication_t *unavailable_access_indication)
{
    if (NULL == unavailable_access_indication) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(unavailable_access_indication);
}

cJSON *OpenAPI_unavailable_access_indication_convertToJSON(OpenAPI_unavailable_access_indication_t *unavailable_access_indication)
{
    cJSON *item = NULL;

    if (unavailable_access_indication == NULL) {
        ogs_error("OpenAPI_unavailable_access_indication_convertToJSON() failed [UnavailableAccessIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_unavailable_access_indication_t *OpenAPI_unavailable_access_indication_parseFromJSON(cJSON *unavailable_access_indicationJSON)
{
    OpenAPI_unavailable_access_indication_t *unavailable_access_indication_local_var = NULL;
    unavailable_access_indication_local_var = OpenAPI_unavailable_access_indication_create (
        );

    return unavailable_access_indication_local_var;
end:
    return NULL;
}

OpenAPI_unavailable_access_indication_t *OpenAPI_unavailable_access_indication_copy(OpenAPI_unavailable_access_indication_t *dst, OpenAPI_unavailable_access_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_unavailable_access_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_unavailable_access_indication_convertToJSON() failed");
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

    OpenAPI_unavailable_access_indication_free(dst);
    dst = OpenAPI_unavailable_access_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

