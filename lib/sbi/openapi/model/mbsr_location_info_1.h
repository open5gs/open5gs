/*
 * mbsr_location_info_1.h
 *
 * MBSR Location Information
 */

#ifndef _OpenAPI_mbsr_location_info_1_H_
#define _OpenAPI_mbsr_location_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mbsr_location_info_1_s OpenAPI_mbsr_location_info_1_t;
#include "tai_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mbsr_location_info_1_s {
    OpenAPI_list_t *mbsr_location;
    OpenAPI_list_t *mbsr_location_areas;
};

OpenAPI_mbsr_location_info_1_t *OpenAPI_mbsr_location_info_1_create(
    OpenAPI_list_t *mbsr_location,
    OpenAPI_list_t *mbsr_location_areas
);
void OpenAPI_mbsr_location_info_1_free(OpenAPI_mbsr_location_info_1_t *mbsr_location_info_1);
OpenAPI_mbsr_location_info_1_t *OpenAPI_mbsr_location_info_1_parseFromJSON(cJSON *mbsr_location_info_1JSON);
cJSON *OpenAPI_mbsr_location_info_1_convertToJSON(OpenAPI_mbsr_location_info_1_t *mbsr_location_info_1);
OpenAPI_mbsr_location_info_1_t *OpenAPI_mbsr_location_info_1_copy(OpenAPI_mbsr_location_info_1_t *dst, OpenAPI_mbsr_location_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbsr_location_info_1_H_ */

