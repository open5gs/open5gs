/*
 * gnss_positioning_method_and_usage.h
 *
 * Indicates the usage of a Global Navigation Satellite System (GNSS) positioning method.
 */

#ifndef _OpenAPI_gnss_positioning_method_and_usage_H_
#define _OpenAPI_gnss_positioning_method_and_usage_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gnss_id.h"
#include "positioning_mode.h"
#include "usage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_gnss_positioning_method_and_usage_s OpenAPI_gnss_positioning_method_and_usage_t;
typedef struct OpenAPI_gnss_positioning_method_and_usage_s {
    struct OpenAPI_positioning_mode_s *mode;
    struct OpenAPI_gnss_id_s *gnss;
    struct OpenAPI_usage_s *usage;
} OpenAPI_gnss_positioning_method_and_usage_t;

OpenAPI_gnss_positioning_method_and_usage_t *OpenAPI_gnss_positioning_method_and_usage_create(
    OpenAPI_positioning_mode_t *mode,
    OpenAPI_gnss_id_t *gnss,
    OpenAPI_usage_t *usage
);
void OpenAPI_gnss_positioning_method_and_usage_free(OpenAPI_gnss_positioning_method_and_usage_t *gnss_positioning_method_and_usage);
OpenAPI_gnss_positioning_method_and_usage_t *OpenAPI_gnss_positioning_method_and_usage_parseFromJSON(cJSON *gnss_positioning_method_and_usageJSON);
cJSON *OpenAPI_gnss_positioning_method_and_usage_convertToJSON(OpenAPI_gnss_positioning_method_and_usage_t *gnss_positioning_method_and_usage);
OpenAPI_gnss_positioning_method_and_usage_t *OpenAPI_gnss_positioning_method_and_usage_copy(OpenAPI_gnss_positioning_method_and_usage_t *dst, OpenAPI_gnss_positioning_method_and_usage_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gnss_positioning_method_and_usage_H_ */

