/*
 * app_descriptor_1.h
 *
 * Represents an operation system and the corresponding applications.
 */

#ifndef _OpenAPI_app_descriptor_1_H_
#define _OpenAPI_app_descriptor_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_descriptor_1_s OpenAPI_app_descriptor_1_t;
typedef struct OpenAPI_app_descriptor_1_s {
    char *os_id;
    OpenAPI_list_t* app_ids;
} OpenAPI_app_descriptor_1_t;

OpenAPI_app_descriptor_1_t *OpenAPI_app_descriptor_1_create(
    char *os_id,
    OpenAPI_list_t* app_ids
);
void OpenAPI_app_descriptor_1_free(OpenAPI_app_descriptor_1_t *app_descriptor_1);
OpenAPI_app_descriptor_1_t *OpenAPI_app_descriptor_1_parseFromJSON(cJSON *app_descriptor_1JSON);
cJSON *OpenAPI_app_descriptor_1_convertToJSON(OpenAPI_app_descriptor_1_t *app_descriptor_1);
OpenAPI_app_descriptor_1_t *OpenAPI_app_descriptor_1_copy(OpenAPI_app_descriptor_1_t *dst, OpenAPI_app_descriptor_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_descriptor_1_H_ */

