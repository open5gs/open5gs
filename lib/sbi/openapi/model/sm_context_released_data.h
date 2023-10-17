/*
 * sm_context_released_data.h
 *
 * Data within Release SM Context Response
 */

#ifndef _OpenAPI_sm_context_released_data_H_
#define _OpenAPI_sm_context_released_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "apn_rate_status.h"
#include "small_data_rate_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_released_data_s OpenAPI_sm_context_released_data_t;
typedef struct OpenAPI_sm_context_released_data_s {
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
    struct OpenAPI_apn_rate_status_s *apn_rate_status;
} OpenAPI_sm_context_released_data_t;

OpenAPI_sm_context_released_data_t *OpenAPI_sm_context_released_data_create(
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status
);
void OpenAPI_sm_context_released_data_free(OpenAPI_sm_context_released_data_t *sm_context_released_data);
OpenAPI_sm_context_released_data_t *OpenAPI_sm_context_released_data_parseFromJSON(cJSON *sm_context_released_dataJSON);
cJSON *OpenAPI_sm_context_released_data_convertToJSON(OpenAPI_sm_context_released_data_t *sm_context_released_data);
OpenAPI_sm_context_released_data_t *OpenAPI_sm_context_released_data_copy(OpenAPI_sm_context_released_data_t *dst, OpenAPI_sm_context_released_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_released_data_H_ */

