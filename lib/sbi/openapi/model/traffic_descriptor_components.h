/*
 * traffic_descriptor_components.h
 *
 * 
 */

#ifndef _OpenAPI_traffic_descriptor_components_H_
#define _OpenAPI_traffic_descriptor_components_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_descriptor_components_s OpenAPI_traffic_descriptor_components_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_descriptor_components_s {
    char *value;
};

OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_create(
char *value
);
void OpenAPI_traffic_descriptor_components_free(OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components);
OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_parseFromJSON(cJSON *traffic_descriptor_componentsJSON);
cJSON *OpenAPI_traffic_descriptor_components_convertToJSON(OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components);
OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_copy(OpenAPI_traffic_descriptor_components_t *dst, OpenAPI_traffic_descriptor_components_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_descriptor_components_H_ */

