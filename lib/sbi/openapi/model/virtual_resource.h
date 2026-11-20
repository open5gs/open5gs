/*
 * virtual_resource.h
 *
 * 
 */

#ifndef _OpenAPI_virtual_resource_H_
#define _OpenAPI_virtual_resource_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_virtual_resource_s OpenAPI_virtual_resource_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_virtual_resource_s {
    bool is_virtual_memory;
    int virtual_memory;
    bool is_virtual_disk;
    int virtual_disk;
    char *virtual_cpu;
    char *vnfd_id;
};

OpenAPI_virtual_resource_t *OpenAPI_virtual_resource_create(
    bool is_virtual_memory,
    int virtual_memory,
    bool is_virtual_disk,
    int virtual_disk,
    char *virtual_cpu,
    char *vnfd_id
);
void OpenAPI_virtual_resource_free(OpenAPI_virtual_resource_t *virtual_resource);
OpenAPI_virtual_resource_t *OpenAPI_virtual_resource_parseFromJSON(cJSON *virtual_resourceJSON);
cJSON *OpenAPI_virtual_resource_convertToJSON(OpenAPI_virtual_resource_t *virtual_resource);
OpenAPI_virtual_resource_t *OpenAPI_virtual_resource_copy(OpenAPI_virtual_resource_t *dst, OpenAPI_virtual_resource_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_virtual_resource_H_ */

