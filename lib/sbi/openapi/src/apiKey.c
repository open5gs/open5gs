#include <stdlib.h>
#include <string.h>
#include "../include/keyValuePair.h"

OpenAPI_map_t *OpenAPI_map_create(char *key, void *value)
{
    OpenAPI_map_t *OpenAPI_map = NULL;

    ogs_assert(key);

    OpenAPI_map = ogs_malloc(sizeof(OpenAPI_map_t));
    ogs_assert(OpenAPI_map);
    OpenAPI_map->key = key;
    OpenAPI_map->value = value;

    return OpenAPI_map;
}

OpenAPI_map_t *OpenAPI_map_create_allocate(char *key, double value)
{
    double* boolpointer = ogs_malloc(sizeof(value));
    ogs_assert(boolpointer);
    memcpy(boolpointer, &value, sizeof(value));
    return OpenAPI_map_create(key, boolpointer);
}

void OpenAPI_map_free(OpenAPI_map_t *OpenAPI_map)
{
    ogs_free(OpenAPI_map);
}
