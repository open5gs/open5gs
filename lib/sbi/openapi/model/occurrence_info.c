
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "occurrence_info.h"

OpenAPI_occurrence_info_t *OpenAPI_occurrence_info_create(
)
{
    OpenAPI_occurrence_info_t *occurrence_info_local_var = ogs_malloc(sizeof(OpenAPI_occurrence_info_t));
    ogs_assert(occurrence_info_local_var);


    return occurrence_info_local_var;
}

void OpenAPI_occurrence_info_free(OpenAPI_occurrence_info_t *occurrence_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == occurrence_info) {
        return;
    }
    ogs_free(occurrence_info);
}

cJSON *OpenAPI_occurrence_info_convertToJSON(OpenAPI_occurrence_info_t *occurrence_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (occurrence_info == NULL) {
        ogs_error("OpenAPI_occurrence_info_convertToJSON() failed [OccurrenceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_occurrence_info_t *OpenAPI_occurrence_info_parseFromJSON(cJSON *occurrence_infoJSON)
{
    OpenAPI_occurrence_info_t *occurrence_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    occurrence_info_local_var = OpenAPI_occurrence_info_create (
    );

    return occurrence_info_local_var;
end:
    return NULL;
}

OpenAPI_occurrence_info_t *OpenAPI_occurrence_info_copy(OpenAPI_occurrence_info_t *dst, OpenAPI_occurrence_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_occurrence_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_occurrence_info_convertToJSON() failed");
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

    OpenAPI_occurrence_info_free(dst);
    dst = OpenAPI_occurrence_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

