/*
 * callback_name.h
 *
 * Callback Name
 */

#ifndef _OpenAPI_callback_name_H_
#define _OpenAPI_callback_name_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_callback_name_s OpenAPI_callback_name_t;
typedef struct OpenAPI_callback_name_s {
    char *callback_type;
} OpenAPI_callback_name_t;

OpenAPI_callback_name_t *OpenAPI_callback_name_create(
    char *callback_type
);
void OpenAPI_callback_name_free(OpenAPI_callback_name_t *callback_name);
OpenAPI_callback_name_t *OpenAPI_callback_name_parseFromJSON(cJSON *callback_nameJSON);
cJSON *OpenAPI_callback_name_convertToJSON(OpenAPI_callback_name_t *callback_name);
OpenAPI_callback_name_t *OpenAPI_callback_name_copy(OpenAPI_callback_name_t *dst, OpenAPI_callback_name_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_callback_name_H_ */

