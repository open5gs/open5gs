/*
 * inline_object_3.h
 *
 * 
 */

#ifndef _OpenAPI_inline_object_3_H_
#define _OpenAPI_inline_object_3_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n1_message_notification.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_inline_object_3_s OpenAPI_inline_object_3_t;
typedef struct OpenAPI_inline_object_3_s {
    struct OpenAPI_n1_message_notification_s *json_data;
    OpenAPI_binary_t* binary_data_n1_message;
} OpenAPI_inline_object_3_t;

OpenAPI_inline_object_3_t *OpenAPI_inline_object_3_create(
    OpenAPI_n1_message_notification_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message
);
void OpenAPI_inline_object_3_free(OpenAPI_inline_object_3_t *inline_object_3);
OpenAPI_inline_object_3_t *OpenAPI_inline_object_3_parseFromJSON(cJSON *inline_object_3JSON);
cJSON *OpenAPI_inline_object_3_convertToJSON(OpenAPI_inline_object_3_t *inline_object_3);
OpenAPI_inline_object_3_t *OpenAPI_inline_object_3_copy(OpenAPI_inline_object_3_t *dst, OpenAPI_inline_object_3_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_inline_object_3_H_ */

