/*
 * app_descriptor.h
 *
 *
 */

#ifndef _OpenAPI_app_descriptor_H_
#define _OpenAPI_app_descriptor_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_descriptor_s OpenAPI_app_descriptor_t;
typedef struct OpenAPI_app_descriptor_s {
    char *os_id;
    char *app_id;
} OpenAPI_app_descriptor_t;

OpenAPI_app_descriptor_t *OpenAPI_app_descriptor_create(
    char *os_id,
    char *app_id
    );
void OpenAPI_app_descriptor_free(OpenAPI_app_descriptor_t *app_descriptor);
OpenAPI_app_descriptor_t *OpenAPI_app_descriptor_parseFromJSON(cJSON *app_descriptorJSON);
cJSON *OpenAPI_app_descriptor_convertToJSON(OpenAPI_app_descriptor_t *app_descriptor);
OpenAPI_app_descriptor_t *OpenAPI_app_descriptor_copy(OpenAPI_app_descriptor_t *dst, OpenAPI_app_descriptor_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_descriptor_H_ */

