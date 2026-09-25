/*
 * trace_data_or_shared_trace_data_id.h
 *
 * 
 */

#ifndef _OpenAPI_trace_data_or_shared_trace_data_id_H_
#define _OpenAPI_trace_data_or_shared_trace_data_id_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_trace_data_or_shared_trace_data_id_s OpenAPI_trace_data_or_shared_trace_data_id_t;
#include "trace_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_trace_data_or_shared_trace_data_id_s {
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
    char *shared_data_id;
};

OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_create(
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *shared_data_id
);
void OpenAPI_trace_data_or_shared_trace_data_id_free(OpenAPI_trace_data_or_shared_trace_data_id_t *trace_data_or_shared_trace_data_id);
OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_parseFromJSON(cJSON *trace_data_or_shared_trace_data_idJSON);
cJSON *OpenAPI_trace_data_or_shared_trace_data_id_convertToJSON(OpenAPI_trace_data_or_shared_trace_data_id_t *trace_data_or_shared_trace_data_id);
OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_copy(OpenAPI_trace_data_or_shared_trace_data_id_t *dst, OpenAPI_trace_data_or_shared_trace_data_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trace_data_or_shared_trace_data_id_H_ */

