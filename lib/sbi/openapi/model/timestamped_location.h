/*
 * timestamped_location.h
 *
 * Timestamped location of the UE.
 */

#ifndef _OpenAPI_timestamped_location_H_
#define _OpenAPI_timestamped_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_timestamped_location_s OpenAPI_timestamped_location_t;
#include "location_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_timestamped_location_s {
    char *ts;
    struct OpenAPI_location_info_s *loc_info;
    char *supi;
    char *gpsi;
};

OpenAPI_timestamped_location_t *OpenAPI_timestamped_location_create(
    char *ts,
    OpenAPI_location_info_t *loc_info,
    char *supi,
    char *gpsi
);
void OpenAPI_timestamped_location_free(OpenAPI_timestamped_location_t *timestamped_location);
OpenAPI_timestamped_location_t *OpenAPI_timestamped_location_parseFromJSON(cJSON *timestamped_locationJSON);
cJSON *OpenAPI_timestamped_location_convertToJSON(OpenAPI_timestamped_location_t *timestamped_location);
OpenAPI_timestamped_location_t *OpenAPI_timestamped_location_copy(OpenAPI_timestamped_location_t *dst, OpenAPI_timestamped_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_timestamped_location_H_ */

