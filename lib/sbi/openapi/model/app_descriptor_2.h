/*
 * app_descriptor_2.h
 *
 * Represents an operation system and the corresponding applications.
 */

#ifndef _OpenAPI_app_descriptor_2_H_
#define _OpenAPI_app_descriptor_2_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_app_descriptor_2_s OpenAPI_app_descriptor_2_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_app_descriptor_2_s {
    char *os_id;
    OpenAPI_list_t* app_ids;
};

OpenAPI_app_descriptor_2_t *OpenAPI_app_descriptor_2_create(
    char *os_id,
    OpenAPI_list_t* app_ids
);
void OpenAPI_app_descriptor_2_free(OpenAPI_app_descriptor_2_t *app_descriptor_2);
OpenAPI_app_descriptor_2_t *OpenAPI_app_descriptor_2_parseFromJSON(cJSON *app_descriptor_2JSON);
cJSON *OpenAPI_app_descriptor_2_convertToJSON(OpenAPI_app_descriptor_2_t *app_descriptor_2);
OpenAPI_app_descriptor_2_t *OpenAPI_app_descriptor_2_copy(OpenAPI_app_descriptor_2_t *dst, OpenAPI_app_descriptor_2_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_descriptor_2_H_ */

