/*
 * retrieve_data.h
 *
 * 
 */

#ifndef _OpenAPI_retrieve_data_H_
#define _OpenAPI_retrieve_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_retrieve_data_s OpenAPI_retrieve_data_t;
typedef struct OpenAPI_retrieve_data_s {
    bool is_small_data_rate_status_req;
    int small_data_rate_status_req;
} OpenAPI_retrieve_data_t;

OpenAPI_retrieve_data_t *OpenAPI_retrieve_data_create(
    bool is_small_data_rate_status_req,
    int small_data_rate_status_req
);
void OpenAPI_retrieve_data_free(OpenAPI_retrieve_data_t *retrieve_data);
OpenAPI_retrieve_data_t *OpenAPI_retrieve_data_parseFromJSON(cJSON *retrieve_dataJSON);
cJSON *OpenAPI_retrieve_data_convertToJSON(OpenAPI_retrieve_data_t *retrieve_data);
OpenAPI_retrieve_data_t *OpenAPI_retrieve_data_copy(OpenAPI_retrieve_data_t *dst, OpenAPI_retrieve_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_retrieve_data_H_ */

