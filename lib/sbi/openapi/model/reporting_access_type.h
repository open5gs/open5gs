/*
 * reporting_access_type.h
 *
 * Specifies access types of event reporting.
 */

#ifndef _OpenAPI_reporting_access_type_H_
#define _OpenAPI_reporting_access_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "reporting_access_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reporting_access_type_s OpenAPI_reporting_access_type_t;
typedef struct OpenAPI_reporting_access_type_s {
} OpenAPI_reporting_access_type_t;

OpenAPI_reporting_access_type_t *OpenAPI_reporting_access_type_create(void);
void OpenAPI_reporting_access_type_free(OpenAPI_reporting_access_type_t *reporting_access_type);
OpenAPI_reporting_access_type_t *OpenAPI_reporting_access_type_parseFromJSON(cJSON *reporting_access_typeJSON);
cJSON *OpenAPI_reporting_access_type_convertToJSON(OpenAPI_reporting_access_type_t *reporting_access_type);
OpenAPI_reporting_access_type_t *OpenAPI_reporting_access_type_copy(OpenAPI_reporting_access_type_t *dst, OpenAPI_reporting_access_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_access_type_H_ */

