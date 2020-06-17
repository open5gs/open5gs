/*
 * unavailable_access_indication.h
 *
 * Possible values are   - 3GA_UNAVAILABLE   - N3GA_UNAVAILABLE
 */

#ifndef _OpenAPI_unavailable_access_indication_H_
#define _OpenAPI_unavailable_access_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_unavailable_access_indication_s OpenAPI_unavailable_access_indication_t;
typedef struct OpenAPI_unavailable_access_indication_s {
} OpenAPI_unavailable_access_indication_t;

OpenAPI_unavailable_access_indication_t *OpenAPI_unavailable_access_indication_create(
    );
void OpenAPI_unavailable_access_indication_free(OpenAPI_unavailable_access_indication_t *unavailable_access_indication);
OpenAPI_unavailable_access_indication_t *OpenAPI_unavailable_access_indication_parseFromJSON(cJSON *unavailable_access_indicationJSON);
cJSON *OpenAPI_unavailable_access_indication_convertToJSON(OpenAPI_unavailable_access_indication_t *unavailable_access_indication);
OpenAPI_unavailable_access_indication_t *OpenAPI_unavailable_access_indication_copy(OpenAPI_unavailable_access_indication_t *dst, OpenAPI_unavailable_access_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_unavailable_access_indication_H_ */

