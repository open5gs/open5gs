
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_level.h"

OpenAPI_reporting_level_t *OpenAPI_reporting_level_create(
    )
{
    OpenAPI_reporting_level_t *reporting_level_local_var = OpenAPI_malloc(sizeof(OpenAPI_reporting_level_t));
    if (!reporting_level_local_var) {
        return NULL;
    }

    return reporting_level_local_var;
}

void OpenAPI_reporting_level_free(OpenAPI_reporting_level_t *reporting_level)
{
    if (NULL == reporting_level) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(reporting_level);
}

cJSON *OpenAPI_reporting_level_convertToJSON(OpenAPI_reporting_level_t *reporting_level)
{
    cJSON *item = NULL;

    if (reporting_level == NULL) {
        ogs_error("OpenAPI_reporting_level_convertToJSON() failed [ReportingLevel]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_reporting_level_t *OpenAPI_reporting_level_parseFromJSON(cJSON *reporting_levelJSON)
{
    OpenAPI_reporting_level_t *reporting_level_local_var = NULL;
    reporting_level_local_var = OpenAPI_reporting_level_create (
        );

    return reporting_level_local_var;
end:
    return NULL;
}

OpenAPI_reporting_level_t *OpenAPI_reporting_level_copy(OpenAPI_reporting_level_t *dst, OpenAPI_reporting_level_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reporting_level_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reporting_level_convertToJSON() failed");
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

    OpenAPI_reporting_level_free(dst);
    dst = OpenAPI_reporting_level_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

