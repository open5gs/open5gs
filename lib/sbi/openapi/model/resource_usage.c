
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resource_usage.h"

OpenAPI_resource_usage_t *OpenAPI_resource_usage_create(
    bool is_cpu_usage,
    int cpu_usage,
    bool is_memory_usage,
    int memory_usage,
    bool is_storage_usage,
    int storage_usage
)
{
    OpenAPI_resource_usage_t *resource_usage_local_var = ogs_malloc(sizeof(OpenAPI_resource_usage_t));
    ogs_assert(resource_usage_local_var);

    resource_usage_local_var->is_cpu_usage = is_cpu_usage;
    resource_usage_local_var->cpu_usage = cpu_usage;
    resource_usage_local_var->is_memory_usage = is_memory_usage;
    resource_usage_local_var->memory_usage = memory_usage;
    resource_usage_local_var->is_storage_usage = is_storage_usage;
    resource_usage_local_var->storage_usage = storage_usage;

    return resource_usage_local_var;
}

void OpenAPI_resource_usage_free(OpenAPI_resource_usage_t *resource_usage)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == resource_usage) {
        return;
    }
    ogs_free(resource_usage);
}

cJSON *OpenAPI_resource_usage_convertToJSON(OpenAPI_resource_usage_t *resource_usage)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (resource_usage == NULL) {
        ogs_error("OpenAPI_resource_usage_convertToJSON() failed [ResourceUsage]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (resource_usage->is_cpu_usage) {
    if (cJSON_AddNumberToObject(item, "cpuUsage", resource_usage->cpu_usage) == NULL) {
        ogs_error("OpenAPI_resource_usage_convertToJSON() failed [cpu_usage]");
        goto end;
    }
    }

    if (resource_usage->is_memory_usage) {
    if (cJSON_AddNumberToObject(item, "memoryUsage", resource_usage->memory_usage) == NULL) {
        ogs_error("OpenAPI_resource_usage_convertToJSON() failed [memory_usage]");
        goto end;
    }
    }

    if (resource_usage->is_storage_usage) {
    if (cJSON_AddNumberToObject(item, "storageUsage", resource_usage->storage_usage) == NULL) {
        ogs_error("OpenAPI_resource_usage_convertToJSON() failed [storage_usage]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_resource_usage_t *OpenAPI_resource_usage_parseFromJSON(cJSON *resource_usageJSON)
{
    OpenAPI_resource_usage_t *resource_usage_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cpu_usage = NULL;
    cJSON *memory_usage = NULL;
    cJSON *storage_usage = NULL;
    cpu_usage = cJSON_GetObjectItemCaseSensitive(resource_usageJSON, "cpuUsage");
    if (cpu_usage) {
    if (!cJSON_IsNumber(cpu_usage)) {
        ogs_error("OpenAPI_resource_usage_parseFromJSON() failed [cpu_usage]");
        goto end;
    }
    }

    memory_usage = cJSON_GetObjectItemCaseSensitive(resource_usageJSON, "memoryUsage");
    if (memory_usage) {
    if (!cJSON_IsNumber(memory_usage)) {
        ogs_error("OpenAPI_resource_usage_parseFromJSON() failed [memory_usage]");
        goto end;
    }
    }

    storage_usage = cJSON_GetObjectItemCaseSensitive(resource_usageJSON, "storageUsage");
    if (storage_usage) {
    if (!cJSON_IsNumber(storage_usage)) {
        ogs_error("OpenAPI_resource_usage_parseFromJSON() failed [storage_usage]");
        goto end;
    }
    }

    resource_usage_local_var = OpenAPI_resource_usage_create (
        cpu_usage ? true : false,
        cpu_usage ? cpu_usage->valuedouble : 0,
        memory_usage ? true : false,
        memory_usage ? memory_usage->valuedouble : 0,
        storage_usage ? true : false,
        storage_usage ? storage_usage->valuedouble : 0
    );

    return resource_usage_local_var;
end:
    return NULL;
}

OpenAPI_resource_usage_t *OpenAPI_resource_usage_copy(OpenAPI_resource_usage_t *dst, OpenAPI_resource_usage_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_resource_usage_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_resource_usage_convertToJSON() failed");
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

    OpenAPI_resource_usage_free(dst);
    dst = OpenAPI_resource_usage_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

