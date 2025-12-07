
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "termination_cause.h"

OpenAPI_termination_cause_t *OpenAPI_termination_cause_create(
)
{
    OpenAPI_termination_cause_t *termination_cause_local_var = ogs_malloc(sizeof(OpenAPI_termination_cause_t));
    ogs_assert(termination_cause_local_var);


    return termination_cause_local_var;
}

void OpenAPI_termination_cause_free(OpenAPI_termination_cause_t *termination_cause)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == termination_cause) {
        return;
    }
    ogs_free(termination_cause);
}

cJSON *OpenAPI_termination_cause_convertToJSON(OpenAPI_termination_cause_t *termination_cause)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (termination_cause == NULL) {
        ogs_error("OpenAPI_termination_cause_convertToJSON() failed [TerminationCause]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_termination_cause_t *OpenAPI_termination_cause_parseFromJSON(cJSON *termination_causeJSON)
{
    OpenAPI_termination_cause_t *termination_cause_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    termination_cause_local_var = OpenAPI_termination_cause_create (
    );

    return termination_cause_local_var;
end:
    return NULL;
}

OpenAPI_termination_cause_t *OpenAPI_termination_cause_copy(OpenAPI_termination_cause_t *dst, OpenAPI_termination_cause_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_termination_cause_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_termination_cause_convertToJSON() failed");
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

    OpenAPI_termination_cause_free(dst);
    dst = OpenAPI_termination_cause_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

