/*
 * location_info.h
 *
 * Represents UE location information.
 */

#ifndef _OpenAPI_location_info_H_
#define _OpenAPI_location_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_location_info_s OpenAPI_location_info_t;
typedef struct OpenAPI_location_info_s {
    struct OpenAPI_user_location_s *loc;
    bool is_ratio;
    int ratio;
    bool is_confidence;
    int confidence;
} OpenAPI_location_info_t;

OpenAPI_location_info_t *OpenAPI_location_info_create(
    OpenAPI_user_location_t *loc,
    bool is_ratio,
    int ratio,
    bool is_confidence,
    int confidence
);
void OpenAPI_location_info_free(OpenAPI_location_info_t *location_info);
OpenAPI_location_info_t *OpenAPI_location_info_parseFromJSON(cJSON *location_infoJSON);
cJSON *OpenAPI_location_info_convertToJSON(OpenAPI_location_info_t *location_info);
OpenAPI_location_info_t *OpenAPI_location_info_copy(OpenAPI_location_info_t *dst, OpenAPI_location_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_info_H_ */

