/*
 * trace_data.h
 *
 * 
 */

#ifndef _OpenAPI_trace_data_H_
#define _OpenAPI_trace_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "trace_depth.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_trace_data_s OpenAPI_trace_data_t;
typedef struct OpenAPI_trace_data_s {
    char *trace_ref;
    OpenAPI_trace_depth_e trace_depth;
    char *ne_type_list;
    char *event_list;
    char *collection_entity_ipv4_addr;
    char *collection_entity_ipv6_addr;
    char *interface_list;
} OpenAPI_trace_data_t;

OpenAPI_trace_data_t *OpenAPI_trace_data_create(
    char *trace_ref,
    OpenAPI_trace_depth_e trace_depth,
    char *ne_type_list,
    char *event_list,
    char *collection_entity_ipv4_addr,
    char *collection_entity_ipv6_addr,
    char *interface_list
);
void OpenAPI_trace_data_free(OpenAPI_trace_data_t *trace_data);
OpenAPI_trace_data_t *OpenAPI_trace_data_parseFromJSON(cJSON *trace_dataJSON);
cJSON *OpenAPI_trace_data_convertToJSON(OpenAPI_trace_data_t *trace_data);
OpenAPI_trace_data_t *OpenAPI_trace_data_copy(OpenAPI_trace_data_t *dst, OpenAPI_trace_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trace_data_H_ */

