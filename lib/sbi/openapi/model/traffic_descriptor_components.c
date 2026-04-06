
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_descriptor_components.h"

OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_create(
char *value
)
{
    OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components_local_var = ogs_malloc(sizeof(OpenAPI_traffic_descriptor_components_t));
    ogs_assert(traffic_descriptor_components_local_var);

    traffic_descriptor_components_local_var->value = value;

    return traffic_descriptor_components_local_var;
}

void OpenAPI_traffic_descriptor_components_free(OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_descriptor_components) {
        return;
    }
    if (traffic_descriptor_components->value) {
        ogs_free(traffic_descriptor_components->value);
        traffic_descriptor_components->value = NULL;
    }
    ogs_free(traffic_descriptor_components);
}

cJSON *OpenAPI_traffic_descriptor_components_convertToJSON(OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_descriptor_components == NULL) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [TrafficDescriptorComponents]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_parseFromJSON(cJSON *traffic_descriptor_componentsJSON)
{
    OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    traffic_descriptor_components_local_var = OpenAPI_traffic_descriptor_components_create (
        NULL
    );

    return traffic_descriptor_components_local_var;
end:
    return NULL;
}

OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_copy(OpenAPI_traffic_descriptor_components_t *dst, OpenAPI_traffic_descriptor_components_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_descriptor_components_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed");
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

    OpenAPI_traffic_descriptor_components_free(dst);
    dst = OpenAPI_traffic_descriptor_components_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

