/*
 * up_confidentiality.h
 *
 *
 */

#ifndef _OpenAPI_up_confidentiality_H_
#define _OpenAPI_up_confidentiality_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_up_confidentiality_s OpenAPI_up_confidentiality_t;
typedef struct OpenAPI_up_confidentiality_s {
} OpenAPI_up_confidentiality_t;

OpenAPI_up_confidentiality_t *OpenAPI_up_confidentiality_create(
    );
void OpenAPI_up_confidentiality_free(OpenAPI_up_confidentiality_t *up_confidentiality);
OpenAPI_up_confidentiality_t *OpenAPI_up_confidentiality_parseFromJSON(cJSON *up_confidentialityJSON);
cJSON *OpenAPI_up_confidentiality_convertToJSON(OpenAPI_up_confidentiality_t *up_confidentiality);
OpenAPI_up_confidentiality_t *OpenAPI_up_confidentiality_copy(OpenAPI_up_confidentiality_t *dst, OpenAPI_up_confidentiality_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_confidentiality_H_ */

