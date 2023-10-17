/*
 * resource_usage.h
 *
 * The current usage of the virtual resources assigned to the NF instances belonging to a  particular network slice instance. 
 */

#ifndef _OpenAPI_resource_usage_H_
#define _OpenAPI_resource_usage_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_resource_usage_s OpenAPI_resource_usage_t;
typedef struct OpenAPI_resource_usage_s {
    bool is_cpu_usage;
    int cpu_usage;
    bool is_memory_usage;
    int memory_usage;
    bool is_storage_usage;
    int storage_usage;
} OpenAPI_resource_usage_t;

OpenAPI_resource_usage_t *OpenAPI_resource_usage_create(
    bool is_cpu_usage,
    int cpu_usage,
    bool is_memory_usage,
    int memory_usage,
    bool is_storage_usage,
    int storage_usage
);
void OpenAPI_resource_usage_free(OpenAPI_resource_usage_t *resource_usage);
OpenAPI_resource_usage_t *OpenAPI_resource_usage_parseFromJSON(cJSON *resource_usageJSON);
cJSON *OpenAPI_resource_usage_convertToJSON(OpenAPI_resource_usage_t *resource_usage);
OpenAPI_resource_usage_t *OpenAPI_resource_usage_copy(OpenAPI_resource_usage_t *dst, OpenAPI_resource_usage_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_resource_usage_H_ */

