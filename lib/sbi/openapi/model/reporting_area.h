/*
 * reporting_area.h
 *
 * Indicates an area for event reporting.
 */

#ifndef _OpenAPI_reporting_area_H_
#define _OpenAPI_reporting_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecgi.h"
#include "ncgi.h"
#include "reporting_area_type.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reporting_area_s OpenAPI_reporting_area_t;
typedef struct OpenAPI_reporting_area_s {
    struct OpenAPI_reporting_area_type_s *area_type;
    struct OpenAPI_tai_s *tai;
    struct OpenAPI_ecgi_s *ecgi;
    struct OpenAPI_ncgi_s *ncgi;
} OpenAPI_reporting_area_t;

OpenAPI_reporting_area_t *OpenAPI_reporting_area_create(
    OpenAPI_reporting_area_type_t *area_type,
    OpenAPI_tai_t *tai,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_ncgi_t *ncgi
);
void OpenAPI_reporting_area_free(OpenAPI_reporting_area_t *reporting_area);
OpenAPI_reporting_area_t *OpenAPI_reporting_area_parseFromJSON(cJSON *reporting_areaJSON);
cJSON *OpenAPI_reporting_area_convertToJSON(OpenAPI_reporting_area_t *reporting_area);
OpenAPI_reporting_area_t *OpenAPI_reporting_area_copy(OpenAPI_reporting_area_t *dst, OpenAPI_reporting_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_area_H_ */

