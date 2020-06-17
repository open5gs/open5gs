
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ma_release_indication.h"

OpenAPI_ma_release_indication_t *OpenAPI_ma_release_indication_create(
    )
{
    OpenAPI_ma_release_indication_t *ma_release_indication_local_var = OpenAPI_malloc(sizeof(OpenAPI_ma_release_indication_t));
    if (!ma_release_indication_local_var) {
        return NULL;
    }

    return ma_release_indication_local_var;
}

void OpenAPI_ma_release_indication_free(OpenAPI_ma_release_indication_t *ma_release_indication)
{
    if (NULL == ma_release_indication) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ma_release_indication);
}

cJSON *OpenAPI_ma_release_indication_convertToJSON(OpenAPI_ma_release_indication_t *ma_release_indication)
{
    cJSON *item = NULL;

    if (ma_release_indication == NULL) {
        ogs_error("OpenAPI_ma_release_indication_convertToJSON() failed [MaReleaseIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_ma_release_indication_t *OpenAPI_ma_release_indication_parseFromJSON(cJSON *ma_release_indicationJSON)
{
    OpenAPI_ma_release_indication_t *ma_release_indication_local_var = NULL;
    ma_release_indication_local_var = OpenAPI_ma_release_indication_create (
        );

    return ma_release_indication_local_var;
end:
    return NULL;
}

OpenAPI_ma_release_indication_t *OpenAPI_ma_release_indication_copy(OpenAPI_ma_release_indication_t *dst, OpenAPI_ma_release_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ma_release_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ma_release_indication_convertToJSON() failed");
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

    OpenAPI_ma_release_indication_free(dst);
    dst = OpenAPI_ma_release_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

