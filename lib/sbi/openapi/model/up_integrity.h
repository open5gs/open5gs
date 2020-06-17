/*
 * up_integrity.h
 *
 *
 */

#ifndef _OpenAPI_up_integrity_H_
#define _OpenAPI_up_integrity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_up_integrity_s OpenAPI_up_integrity_t;
typedef struct OpenAPI_up_integrity_s {
} OpenAPI_up_integrity_t;

OpenAPI_up_integrity_t *OpenAPI_up_integrity_create(
    );
void OpenAPI_up_integrity_free(OpenAPI_up_integrity_t *up_integrity);
OpenAPI_up_integrity_t *OpenAPI_up_integrity_parseFromJSON(cJSON *up_integrityJSON);
cJSON *OpenAPI_up_integrity_convertToJSON(OpenAPI_up_integrity_t *up_integrity);
OpenAPI_up_integrity_t *OpenAPI_up_integrity_copy(OpenAPI_up_integrity_t *dst, OpenAPI_up_integrity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_integrity_H_ */

