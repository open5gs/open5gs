/*
 * message_class.h
 *
 * 
 */

#ifndef _OpenAPI_message_class_H_
#define _OpenAPI_message_class_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_message_class_s OpenAPI_message_class_t;
#include "class_identifier.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_message_class_s {
    OpenAPI_class_identifier_e class_identifier;
    char *token_text;
};

OpenAPI_message_class_t *OpenAPI_message_class_create(
    OpenAPI_class_identifier_e class_identifier,
    char *token_text
);
void OpenAPI_message_class_free(OpenAPI_message_class_t *message_class);
OpenAPI_message_class_t *OpenAPI_message_class_parseFromJSON(cJSON *message_classJSON);
cJSON *OpenAPI_message_class_convertToJSON(OpenAPI_message_class_t *message_class);
OpenAPI_message_class_t *OpenAPI_message_class_copy(OpenAPI_message_class_t *dst, OpenAPI_message_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_message_class_H_ */

