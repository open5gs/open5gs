
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "virtual_resource.h"

OpenAPI_virtual_resource_t *OpenAPI_virtual_resource_create(
    bool is_virtual_memory,
    int virtual_memory,
    bool is_virtual_disk,
    int virtual_disk,
    char *virtual_cpu,
    char *vnfd_id
)
{
    OpenAPI_virtual_resource_t *virtual_resource_local_var = ogs_malloc(sizeof(OpenAPI_virtual_resource_t));
    ogs_assert(virtual_resource_local_var);

    virtual_resource_local_var->is_virtual_memory = is_virtual_memory;
    virtual_resource_local_var->virtual_memory = virtual_memory;
    virtual_resource_local_var->is_virtual_disk = is_virtual_disk;
    virtual_resource_local_var->virtual_disk = virtual_disk;
    virtual_resource_local_var->virtual_cpu = virtual_cpu;
    virtual_resource_local_var->vnfd_id = vnfd_id;

    return virtual_resource_local_var;
}

void OpenAPI_virtual_resource_free(OpenAPI_virtual_resource_t *virtual_resource)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == virtual_resource) {
        return;
    }
    if (virtual_resource->virtual_cpu) {
        ogs_free(virtual_resource->virtual_cpu);
        virtual_resource->virtual_cpu = NULL;
    }
    if (virtual_resource->vnfd_id) {
        ogs_free(virtual_resource->vnfd_id);
        virtual_resource->vnfd_id = NULL;
    }
    ogs_free(virtual_resource);
}

cJSON *OpenAPI_virtual_resource_convertToJSON(OpenAPI_virtual_resource_t *virtual_resource)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (virtual_resource == NULL) {
        ogs_error("OpenAPI_virtual_resource_convertToJSON() failed [VirtualResource]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (virtual_resource->is_virtual_memory) {
    if (cJSON_AddNumberToObject(item, "virtualMemory", virtual_resource->virtual_memory) == NULL) {
        ogs_error("OpenAPI_virtual_resource_convertToJSON() failed [virtual_memory]");
        goto end;
    }
    }

    if (virtual_resource->is_virtual_disk) {
    if (cJSON_AddNumberToObject(item, "virtualDisk", virtual_resource->virtual_disk) == NULL) {
        ogs_error("OpenAPI_virtual_resource_convertToJSON() failed [virtual_disk]");
        goto end;
    }
    }

    if (virtual_resource->virtual_cpu) {
    if (cJSON_AddStringToObject(item, "virtualCPU", virtual_resource->virtual_cpu) == NULL) {
        ogs_error("OpenAPI_virtual_resource_convertToJSON() failed [virtual_cpu]");
        goto end;
    }
    }

    if (virtual_resource->vnfd_id) {
    if (cJSON_AddStringToObject(item, "vnfdId", virtual_resource->vnfd_id) == NULL) {
        ogs_error("OpenAPI_virtual_resource_convertToJSON() failed [vnfd_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_virtual_resource_t *OpenAPI_virtual_resource_parseFromJSON(cJSON *virtual_resourceJSON)
{
    OpenAPI_virtual_resource_t *virtual_resource_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *virtual_memory = NULL;
    cJSON *virtual_disk = NULL;
    cJSON *virtual_cpu = NULL;
    cJSON *vnfd_id = NULL;
    virtual_memory = cJSON_GetObjectItemCaseSensitive(virtual_resourceJSON, "virtualMemory");
    if (virtual_memory) {
    if (!cJSON_IsNumber(virtual_memory)) {
        ogs_error("OpenAPI_virtual_resource_parseFromJSON() failed [virtual_memory]");
        goto end;
    }
    }

    virtual_disk = cJSON_GetObjectItemCaseSensitive(virtual_resourceJSON, "virtualDisk");
    if (virtual_disk) {
    if (!cJSON_IsNumber(virtual_disk)) {
        ogs_error("OpenAPI_virtual_resource_parseFromJSON() failed [virtual_disk]");
        goto end;
    }
    }

    virtual_cpu = cJSON_GetObjectItemCaseSensitive(virtual_resourceJSON, "virtualCPU");
    if (virtual_cpu) {
    if (!cJSON_IsString(virtual_cpu) && !cJSON_IsNull(virtual_cpu)) {
        ogs_error("OpenAPI_virtual_resource_parseFromJSON() failed [virtual_cpu]");
        goto end;
    }
    }

    vnfd_id = cJSON_GetObjectItemCaseSensitive(virtual_resourceJSON, "vnfdId");
    if (vnfd_id) {
    if (!cJSON_IsString(vnfd_id) && !cJSON_IsNull(vnfd_id)) {
        ogs_error("OpenAPI_virtual_resource_parseFromJSON() failed [vnfd_id]");
        goto end;
    }
    }

    virtual_resource_local_var = OpenAPI_virtual_resource_create (
        virtual_memory ? true : false,
        virtual_memory ? virtual_memory->valuedouble : 0,
        virtual_disk ? true : false,
        virtual_disk ? virtual_disk->valuedouble : 0,
        virtual_cpu && !cJSON_IsNull(virtual_cpu) ? ogs_strdup(virtual_cpu->valuestring) : NULL,
        vnfd_id && !cJSON_IsNull(vnfd_id) ? ogs_strdup(vnfd_id->valuestring) : NULL
    );

    return virtual_resource_local_var;
end:
    return NULL;
}

OpenAPI_virtual_resource_t *OpenAPI_virtual_resource_copy(OpenAPI_virtual_resource_t *dst, OpenAPI_virtual_resource_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_virtual_resource_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_virtual_resource_convertToJSON() failed");
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

    OpenAPI_virtual_resource_free(dst);
    dst = OpenAPI_virtual_resource_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

