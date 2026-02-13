/*
 * reporting_area_type.h
 *
 * Indicates type of event reporting area.
 */

#ifndef _OpenAPI_reporting_area_type_H_
#define _OpenAPI_reporting_area_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "reporting_area_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reporting_area_type_s OpenAPI_reporting_area_type_t;
typedef struct OpenAPI_reporting_area_type_s {
} OpenAPI_reporting_area_type_t;

OpenAPI_reporting_area_type_t *OpenAPI_reporting_area_type_create(void);
void OpenAPI_reporting_area_type_free(OpenAPI_reporting_area_type_t *reporting_area_type);
OpenAPI_reporting_area_type_t *OpenAPI_reporting_area_type_parseFromJSON(cJSON *reporting_area_typeJSON);
cJSON *OpenAPI_reporting_area_type_convertToJSON(OpenAPI_reporting_area_type_t *reporting_area_type);
OpenAPI_reporting_area_type_t *OpenAPI_reporting_area_type_copy(OpenAPI_reporting_area_type_t *dst, OpenAPI_reporting_area_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_area_type_H_ */

