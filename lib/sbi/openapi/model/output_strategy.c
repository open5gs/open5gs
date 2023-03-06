
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "output_strategy.h"

OpenAPI_output_strategy_t *OpenAPI_output_strategy_create(
)
{
    OpenAPI_output_strategy_t *output_strategy_local_var = ogs_malloc(sizeof(OpenAPI_output_strategy_t));
    ogs_assert(output_strategy_local_var);


    return output_strategy_local_var;
}

void OpenAPI_output_strategy_free(OpenAPI_output_strategy_t *output_strategy)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == output_strategy) {
        return;
    }
    ogs_free(output_strategy);
}

cJSON *OpenAPI_output_strategy_convertToJSON(OpenAPI_output_strategy_t *output_strategy)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (output_strategy == NULL) {
        ogs_error("OpenAPI_output_strategy_convertToJSON() failed [OutputStrategy]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_output_strategy_t *OpenAPI_output_strategy_parseFromJSON(cJSON *output_strategyJSON)
{
    OpenAPI_output_strategy_t *output_strategy_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    output_strategy_local_var = OpenAPI_output_strategy_create (
    );

    return output_strategy_local_var;
end:
    return NULL;
}

OpenAPI_output_strategy_t *OpenAPI_output_strategy_copy(OpenAPI_output_strategy_t *dst, OpenAPI_output_strategy_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_output_strategy_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_output_strategy_convertToJSON() failed");
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

    OpenAPI_output_strategy_free(dst);
    dst = OpenAPI_output_strategy_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

