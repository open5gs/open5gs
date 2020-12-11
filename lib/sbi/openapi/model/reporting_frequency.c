
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_frequency.h"

OpenAPI_reporting_frequency_t *OpenAPI_reporting_frequency_create(
    )
{
    OpenAPI_reporting_frequency_t *reporting_frequency_local_var = OpenAPI_malloc(sizeof(OpenAPI_reporting_frequency_t));
    if (!reporting_frequency_local_var) {
        return NULL;
    }

    return reporting_frequency_local_var;
}

void OpenAPI_reporting_frequency_free(OpenAPI_reporting_frequency_t *reporting_frequency)
{
    if (NULL == reporting_frequency) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(reporting_frequency);
}

cJSON *OpenAPI_reporting_frequency_convertToJSON(OpenAPI_reporting_frequency_t *reporting_frequency)
{
    cJSON *item = NULL;

    if (reporting_frequency == NULL) {
        ogs_error("OpenAPI_reporting_frequency_convertToJSON() failed [ReportingFrequency]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_reporting_frequency_t *OpenAPI_reporting_frequency_parseFromJSON(cJSON *reporting_frequencyJSON)
{
    OpenAPI_reporting_frequency_t *reporting_frequency_local_var = NULL;
    reporting_frequency_local_var = OpenAPI_reporting_frequency_create (
        );

    return reporting_frequency_local_var;
end:
    return NULL;
}

OpenAPI_reporting_frequency_t *OpenAPI_reporting_frequency_copy(OpenAPI_reporting_frequency_t *dst, OpenAPI_reporting_frequency_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reporting_frequency_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reporting_frequency_convertToJSON() failed");
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

    OpenAPI_reporting_frequency_free(dst);
    dst = OpenAPI_reporting_frequency_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

