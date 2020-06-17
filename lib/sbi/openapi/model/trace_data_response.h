/*
 * trace_data_response.h
 *
 *
 */

#ifndef _OpenAPI_trace_data_response_H_
#define _OpenAPI_trace_data_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "trace_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_trace_data_response_s OpenAPI_trace_data_response_t;
typedef struct OpenAPI_trace_data_response_s {
    struct OpenAPI_trace_data_s *trace_data;
    char *shared_trace_data_id;
} OpenAPI_trace_data_response_t;

OpenAPI_trace_data_response_t *OpenAPI_trace_data_response_create(
    OpenAPI_trace_data_t *trace_data,
    char *shared_trace_data_id
    );
void OpenAPI_trace_data_response_free(OpenAPI_trace_data_response_t *trace_data_response);
OpenAPI_trace_data_response_t *OpenAPI_trace_data_response_parseFromJSON(cJSON *trace_data_responseJSON);
cJSON *OpenAPI_trace_data_response_convertToJSON(OpenAPI_trace_data_response_t *trace_data_response);
OpenAPI_trace_data_response_t *OpenAPI_trace_data_response_copy(OpenAPI_trace_data_response_t *dst, OpenAPI_trace_data_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trace_data_response_H_ */

