/*
 * mbsr_location_info.h
 *
 * MBSR Location Information
 */

#ifndef _OpenAPI_mbsr_location_info_H_
#define _OpenAPI_mbsr_location_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mbsr_location_info_s OpenAPI_mbsr_location_info_t;
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mbsr_location_info_s {
    OpenAPI_list_t *mbsr_location;
    OpenAPI_list_t *mbsr_location_areas;
};

OpenAPI_mbsr_location_info_t *OpenAPI_mbsr_location_info_create(
    OpenAPI_list_t *mbsr_location,
    OpenAPI_list_t *mbsr_location_areas
);
void OpenAPI_mbsr_location_info_free(OpenAPI_mbsr_location_info_t *mbsr_location_info);
OpenAPI_mbsr_location_info_t *OpenAPI_mbsr_location_info_parseFromJSON(cJSON *mbsr_location_infoJSON);
cJSON *OpenAPI_mbsr_location_info_convertToJSON(OpenAPI_mbsr_location_info_t *mbsr_location_info);
OpenAPI_mbsr_location_info_t *OpenAPI_mbsr_location_info_copy(OpenAPI_mbsr_location_info_t *dst, OpenAPI_mbsr_location_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbsr_location_info_H_ */

