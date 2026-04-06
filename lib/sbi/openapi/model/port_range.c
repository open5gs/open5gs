
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "port_range.h"

OpenAPI_port_range_t *OpenAPI_port_range_create(
    int start,
    int end
)
{
    OpenAPI_port_range_t *port_range_local_var = ogs_malloc(sizeof(OpenAPI_port_range_t));
    ogs_assert(port_range_local_var);

    port_range_local_var->start = start;
    port_range_local_var->end = end;

    return port_range_local_var;
}

void OpenAPI_port_range_free(OpenAPI_port_range_t *port_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == port_range) {
        return;
    }
    ogs_free(port_range);
}

cJSON *OpenAPI_port_range_convertToJSON(OpenAPI_port_range_t *port_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (port_range == NULL) {
        ogs_error("OpenAPI_port_range_convertToJSON() failed [PortRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "start", port_range->start) == NULL) {
        ogs_error("OpenAPI_port_range_convertToJSON() failed [start]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "end", port_range->end) == NULL) {
        ogs_error("OpenAPI_port_range_convertToJSON() failed [end]");
        goto end;
    }

end:
    return item;
}

OpenAPI_port_range_t *OpenAPI_port_range_parseFromJSON(cJSON *port_rangeJSON)
{
    OpenAPI_port_range_t *port_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *start = NULL;
    cJSON *end = NULL;
    start = cJSON_GetObjectItemCaseSensitive(port_rangeJSON, "start");
    if (!start) {
        ogs_error("OpenAPI_port_range_parseFromJSON() failed [start]");
        goto end;
    }
    if (!cJSON_IsNumber(start)) {
        ogs_error("OpenAPI_port_range_parseFromJSON() failed [start]");
        goto end;
    }

    end = cJSON_GetObjectItemCaseSensitive(port_rangeJSON, "end");
    if (!end) {
        ogs_error("OpenAPI_port_range_parseFromJSON() failed [end]");
        goto end;
    }
    if (!cJSON_IsNumber(end)) {
        ogs_error("OpenAPI_port_range_parseFromJSON() failed [end]");
        goto end;
    }

    port_range_local_var = OpenAPI_port_range_create (
        
        start->valuedouble,
        
        end->valuedouble
    );

    return port_range_local_var;
end:
    return NULL;
}

OpenAPI_port_range_t *OpenAPI_port_range_copy(OpenAPI_port_range_t *dst, OpenAPI_port_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_port_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_port_range_convertToJSON() failed");
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

    OpenAPI_port_range_free(dst);
    dst = OpenAPI_port_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

