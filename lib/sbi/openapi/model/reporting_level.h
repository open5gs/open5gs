/*
 * reporting_level.h
 *
 * Possible values are - SER_ID_LEVEL: Indicates that the usage shall be reported on service id and rating group combination level. - RAT_GR_LEVEL: Indicates that the usage shall be reported on rating group level. - SPON_CON_LEVEL: Indicates that the usage shall be reported on sponsor identity and rating group combination level.
 */

#ifndef _OpenAPI_reporting_level_H_
#define _OpenAPI_reporting_level_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reporting_level_s OpenAPI_reporting_level_t;
typedef struct OpenAPI_reporting_level_s {
} OpenAPI_reporting_level_t;

OpenAPI_reporting_level_t *OpenAPI_reporting_level_create(
    );
void OpenAPI_reporting_level_free(OpenAPI_reporting_level_t *reporting_level);
OpenAPI_reporting_level_t *OpenAPI_reporting_level_parseFromJSON(cJSON *reporting_levelJSON);
cJSON *OpenAPI_reporting_level_convertToJSON(OpenAPI_reporting_level_t *reporting_level);
OpenAPI_reporting_level_t *OpenAPI_reporting_level_copy(OpenAPI_reporting_level_t *dst, OpenAPI_reporting_level_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_level_H_ */

