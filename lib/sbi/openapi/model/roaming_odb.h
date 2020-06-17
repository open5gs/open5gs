/*
 * roaming_odb.h
 *
 *
 */

#ifndef _OpenAPI_roaming_odb_H_
#define _OpenAPI_roaming_odb_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_roaming_odb_s OpenAPI_roaming_odb_t;
typedef struct OpenAPI_roaming_odb_s {
} OpenAPI_roaming_odb_t;

OpenAPI_roaming_odb_t *OpenAPI_roaming_odb_create(
    );
void OpenAPI_roaming_odb_free(OpenAPI_roaming_odb_t *roaming_odb);
OpenAPI_roaming_odb_t *OpenAPI_roaming_odb_parseFromJSON(cJSON *roaming_odbJSON);
cJSON *OpenAPI_roaming_odb_convertToJSON(OpenAPI_roaming_odb_t *roaming_odb);
OpenAPI_roaming_odb_t *OpenAPI_roaming_odb_copy(OpenAPI_roaming_odb_t *dst, OpenAPI_roaming_odb_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roaming_odb_H_ */

