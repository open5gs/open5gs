/*
 * inline_object_1.h
 *
 *
 */

#ifndef _OpenAPI_inline_object_1_H_
#define _OpenAPI_inline_object_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n1_message_notification.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_inline_object_1_s OpenAPI_inline_object_1_t;
typedef struct OpenAPI_inline_object_1_s {
    struct OpenAPI_n1_message_notification_s *json_data;
    OpenAPI_binary_t* binary_data_n1_message;
} OpenAPI_inline_object_1_t;

OpenAPI_inline_object_1_t *OpenAPI_inline_object_1_create(
    OpenAPI_n1_message_notification_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message
    );
void OpenAPI_inline_object_1_free(OpenAPI_inline_object_1_t *inline_object_1);
OpenAPI_inline_object_1_t *OpenAPI_inline_object_1_parseFromJSON(cJSON *inline_object_1JSON);
cJSON *OpenAPI_inline_object_1_convertToJSON(OpenAPI_inline_object_1_t *inline_object_1);
OpenAPI_inline_object_1_t *OpenAPI_inline_object_1_copy(OpenAPI_inline_object_1_t *dst, OpenAPI_inline_object_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_inline_object_1_H_ */

