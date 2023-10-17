
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_ordering_criterion.h"

OpenAPI_dispersion_ordering_criterion_t *OpenAPI_dispersion_ordering_criterion_create(
)
{
    OpenAPI_dispersion_ordering_criterion_t *dispersion_ordering_criterion_local_var = ogs_malloc(sizeof(OpenAPI_dispersion_ordering_criterion_t));
    ogs_assert(dispersion_ordering_criterion_local_var);


    return dispersion_ordering_criterion_local_var;
}

void OpenAPI_dispersion_ordering_criterion_free(OpenAPI_dispersion_ordering_criterion_t *dispersion_ordering_criterion)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dispersion_ordering_criterion) {
        return;
    }
    ogs_free(dispersion_ordering_criterion);
}

cJSON *OpenAPI_dispersion_ordering_criterion_convertToJSON(OpenAPI_dispersion_ordering_criterion_t *dispersion_ordering_criterion)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dispersion_ordering_criterion == NULL) {
        ogs_error("OpenAPI_dispersion_ordering_criterion_convertToJSON() failed [DispersionOrderingCriterion]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_dispersion_ordering_criterion_t *OpenAPI_dispersion_ordering_criterion_parseFromJSON(cJSON *dispersion_ordering_criterionJSON)
{
    OpenAPI_dispersion_ordering_criterion_t *dispersion_ordering_criterion_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    dispersion_ordering_criterion_local_var = OpenAPI_dispersion_ordering_criterion_create (
    );

    return dispersion_ordering_criterion_local_var;
end:
    return NULL;
}

OpenAPI_dispersion_ordering_criterion_t *OpenAPI_dispersion_ordering_criterion_copy(OpenAPI_dispersion_ordering_criterion_t *dst, OpenAPI_dispersion_ordering_criterion_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dispersion_ordering_criterion_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dispersion_ordering_criterion_convertToJSON() failed");
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

    OpenAPI_dispersion_ordering_criterion_free(dst);
    dst = OpenAPI_dispersion_ordering_criterion_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

