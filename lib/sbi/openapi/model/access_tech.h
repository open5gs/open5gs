/*
 * access_tech.h
 *
 * 
 */

#ifndef _OpenAPI_access_tech_H_
#define _OpenAPI_access_tech_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_tech_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_tech_s OpenAPI_access_tech_t;
typedef struct OpenAPI_access_tech_s {
} OpenAPI_access_tech_t;

OpenAPI_access_tech_t *OpenAPI_access_tech_create(
);
void OpenAPI_access_tech_free(OpenAPI_access_tech_t *access_tech);
OpenAPI_access_tech_t *OpenAPI_access_tech_parseFromJSON(cJSON *access_techJSON);
cJSON *OpenAPI_access_tech_convertToJSON(OpenAPI_access_tech_t *access_tech);
OpenAPI_access_tech_t *OpenAPI_access_tech_copy(OpenAPI_access_tech_t *dst, OpenAPI_access_tech_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_tech_H_ */

