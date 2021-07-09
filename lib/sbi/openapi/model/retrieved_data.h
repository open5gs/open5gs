/*
 * retrieved_data.h
 *
 * 
 */

#ifndef _OpenAPI_retrieved_data_H_
#define _OpenAPI_retrieved_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "small_data_rate_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_retrieved_data_s OpenAPI_retrieved_data_t;
typedef struct OpenAPI_retrieved_data_s {
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
} OpenAPI_retrieved_data_t;

OpenAPI_retrieved_data_t *OpenAPI_retrieved_data_create(
    OpenAPI_small_data_rate_status_t *small_data_rate_status
);
void OpenAPI_retrieved_data_free(OpenAPI_retrieved_data_t *retrieved_data);
OpenAPI_retrieved_data_t *OpenAPI_retrieved_data_parseFromJSON(cJSON *retrieved_dataJSON);
cJSON *OpenAPI_retrieved_data_convertToJSON(OpenAPI_retrieved_data_t *retrieved_data);
OpenAPI_retrieved_data_t *OpenAPI_retrieved_data_copy(OpenAPI_retrieved_data_t *dst, OpenAPI_retrieved_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_retrieved_data_H_ */

