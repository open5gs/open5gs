/*
 * trace_data_1.h
 *
 * contains Trace control and configuration parameters.
 */

#ifndef _OpenAPI_trace_data_1_H_
#define _OpenAPI_trace_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_trace_data_1_s OpenAPI_trace_data_1_t;
#include "job_type.h"
#include "trace_depth.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_trace_data_1_s {
    char *trace_ref;
    OpenAPI_trace_depth_e trace_depth;
    char *ne_type_list;
    char *event_list;
    char *collection_entity_ipv4_addr;
    char *collection_entity_ipv6_addr;
    char *trace_reporting_consumer_uri;
    char *interface_list;
    OpenAPI_job_type_e job_type;
};

OpenAPI_trace_data_1_t *OpenAPI_trace_data_1_create(
    char *trace_ref,
    OpenAPI_trace_depth_e trace_depth,
    char *ne_type_list,
    char *event_list,
    char *collection_entity_ipv4_addr,
    char *collection_entity_ipv6_addr,
    char *trace_reporting_consumer_uri,
    char *interface_list,
    OpenAPI_job_type_e job_type
);
void OpenAPI_trace_data_1_free(OpenAPI_trace_data_1_t *trace_data_1);
OpenAPI_trace_data_1_t *OpenAPI_trace_data_1_parseFromJSON(cJSON *trace_data_1JSON);
cJSON *OpenAPI_trace_data_1_convertToJSON(OpenAPI_trace_data_1_t *trace_data_1);
OpenAPI_trace_data_1_t *OpenAPI_trace_data_1_copy(OpenAPI_trace_data_1_t *dst, OpenAPI_trace_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trace_data_1_H_ */

