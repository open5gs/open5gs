/*
 * mbsr_operation_allowed.h
 *
 * Indicates whether the UE is allowed for MBSR operation
 */

#ifndef _OpenAPI_mbsr_operation_allowed_H_
#define _OpenAPI_mbsr_operation_allowed_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mbsr_operation_allowed_s OpenAPI_mbsr_operation_allowed_t;
#include "mbsr_location_info.h"
#include "mbsr_time_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mbsr_operation_allowed_s {
    bool is_mbsr_operation_allowed_ind;
    int mbsr_operation_allowed_ind;
    struct OpenAPI_mbsr_location_info_s *mbsr_location_info;
    struct OpenAPI_mbsr_time_info_s *mbsr_time_info;
};

OpenAPI_mbsr_operation_allowed_t *OpenAPI_mbsr_operation_allowed_create(
    bool is_mbsr_operation_allowed_ind,
    int mbsr_operation_allowed_ind,
    OpenAPI_mbsr_location_info_t *mbsr_location_info,
    OpenAPI_mbsr_time_info_t *mbsr_time_info
);
void OpenAPI_mbsr_operation_allowed_free(OpenAPI_mbsr_operation_allowed_t *mbsr_operation_allowed);
OpenAPI_mbsr_operation_allowed_t *OpenAPI_mbsr_operation_allowed_parseFromJSON(cJSON *mbsr_operation_allowedJSON);
cJSON *OpenAPI_mbsr_operation_allowed_convertToJSON(OpenAPI_mbsr_operation_allowed_t *mbsr_operation_allowed);
OpenAPI_mbsr_operation_allowed_t *OpenAPI_mbsr_operation_allowed_copy(OpenAPI_mbsr_operation_allowed_t *dst, OpenAPI_mbsr_operation_allowed_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbsr_operation_allowed_H_ */

