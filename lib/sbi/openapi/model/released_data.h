/*
 * released_data.h
 *
 * Data within Release Response
 */

#ifndef _OpenAPI_released_data_H_
#define _OpenAPI_released_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "apn_rate_status.h"
#include "n4_information.h"
#include "small_data_rate_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_released_data_s OpenAPI_released_data_t;
typedef struct OpenAPI_released_data_s {
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
    struct OpenAPI_apn_rate_status_s *apn_rate_status;
    struct OpenAPI_n4_information_s *n4_info;
    struct OpenAPI_n4_information_s *n4_info_ext1;
    struct OpenAPI_n4_information_s *n4_info_ext2;
} OpenAPI_released_data_t;

OpenAPI_released_data_t *OpenAPI_released_data_create(
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2
);
void OpenAPI_released_data_free(OpenAPI_released_data_t *released_data);
OpenAPI_released_data_t *OpenAPI_released_data_parseFromJSON(cJSON *released_dataJSON);
cJSON *OpenAPI_released_data_convertToJSON(OpenAPI_released_data_t *released_data);
OpenAPI_released_data_t *OpenAPI_released_data_copy(OpenAPI_released_data_t *dst, OpenAPI_released_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_released_data_H_ */

