/*
 * positioning_method_and_usage.h
 *
 * Indicates the usage of a positioning method.
 */

#ifndef _OpenAPI_positioning_method_and_usage_H_
#define _OpenAPI_positioning_method_and_usage_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "positioning_method.h"
#include "positioning_mode.h"
#include "usage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_positioning_method_and_usage_s OpenAPI_positioning_method_and_usage_t;
typedef struct OpenAPI_positioning_method_and_usage_s {
    struct OpenAPI_positioning_method_s *method;
    struct OpenAPI_positioning_mode_s *mode;
    struct OpenAPI_usage_s *usage;
    bool is_method_code;
    int method_code;
} OpenAPI_positioning_method_and_usage_t;

OpenAPI_positioning_method_and_usage_t *OpenAPI_positioning_method_and_usage_create(
    OpenAPI_positioning_method_t *method,
    OpenAPI_positioning_mode_t *mode,
    OpenAPI_usage_t *usage,
    bool is_method_code,
    int method_code
);
void OpenAPI_positioning_method_and_usage_free(OpenAPI_positioning_method_and_usage_t *positioning_method_and_usage);
OpenAPI_positioning_method_and_usage_t *OpenAPI_positioning_method_and_usage_parseFromJSON(cJSON *positioning_method_and_usageJSON);
cJSON *OpenAPI_positioning_method_and_usage_convertToJSON(OpenAPI_positioning_method_and_usage_t *positioning_method_and_usage);
OpenAPI_positioning_method_and_usage_t *OpenAPI_positioning_method_and_usage_copy(OpenAPI_positioning_method_and_usage_t *dst, OpenAPI_positioning_method_and_usage_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_positioning_method_and_usage_H_ */

