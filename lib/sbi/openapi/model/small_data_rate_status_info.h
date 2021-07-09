/*
 * small_data_rate_status_info.h
 *
 * 
 */

#ifndef _OpenAPI_small_data_rate_status_info_H_
#define _OpenAPI_small_data_rate_status_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "small_data_rate_status.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_small_data_rate_status_info_s OpenAPI_small_data_rate_status_info_t;
typedef struct OpenAPI_small_data_rate_status_info_s {
    struct OpenAPI_snssai_s *snssai;
    char *dnn;
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
} OpenAPI_small_data_rate_status_info_t;

OpenAPI_small_data_rate_status_info_t *OpenAPI_small_data_rate_status_info_create(
    OpenAPI_snssai_t *snssai,
    char *dnn,
    OpenAPI_small_data_rate_status_t *small_data_rate_status
);
void OpenAPI_small_data_rate_status_info_free(OpenAPI_small_data_rate_status_info_t *small_data_rate_status_info);
OpenAPI_small_data_rate_status_info_t *OpenAPI_small_data_rate_status_info_parseFromJSON(cJSON *small_data_rate_status_infoJSON);
cJSON *OpenAPI_small_data_rate_status_info_convertToJSON(OpenAPI_small_data_rate_status_info_t *small_data_rate_status_info);
OpenAPI_small_data_rate_status_info_t *OpenAPI_small_data_rate_status_info_copy(OpenAPI_small_data_rate_status_info_t *dst, OpenAPI_small_data_rate_status_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_small_data_rate_status_info_H_ */

