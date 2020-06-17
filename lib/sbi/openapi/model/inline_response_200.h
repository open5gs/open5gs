/*
 * inline_response_200.h
 *
 *
 */

#ifndef _OpenAPI_inline_response_200_H_
#define _OpenAPI_inline_response_200_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "links_value_schema.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_inline_response_200_s OpenAPI_inline_response_200_t;
typedef struct OpenAPI_inline_response_200_s {
    OpenAPI_list_t* _links;
} OpenAPI_inline_response_200_t;

OpenAPI_inline_response_200_t *OpenAPI_inline_response_200_create(
    OpenAPI_list_t* _links
    );
void OpenAPI_inline_response_200_free(OpenAPI_inline_response_200_t *inline_response_200);
OpenAPI_inline_response_200_t *OpenAPI_inline_response_200_parseFromJSON(cJSON *inline_response_200JSON);
cJSON *OpenAPI_inline_response_200_convertToJSON(OpenAPI_inline_response_200_t *inline_response_200);
OpenAPI_inline_response_200_t *OpenAPI_inline_response_200_copy(OpenAPI_inline_response_200_t *dst, OpenAPI_inline_response_200_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_inline_response_200_H_ */

