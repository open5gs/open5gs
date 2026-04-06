/*
 * trace_data_or_shared_trace_data_id.h
 *
 * 
 */

#ifndef _OpenAPI_trace_data_or_shared_trace_data_id_H_
#define _OpenAPI_trace_data_or_shared_trace_data_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_trace_data_or_shared_trace_data_id_s OpenAPI_trace_data_or_shared_trace_data_id_t;
#include "job_type.h"
#include "trace_data.h"
#include "trace_depth.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_trace_data_or_shared_trace_data_id_s {
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

OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_create(
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
void OpenAPI_trace_data_or_shared_trace_data_id_free(OpenAPI_trace_data_or_shared_trace_data_id_t *trace_data_or_shared_trace_data_id);
OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_parseFromJSON(cJSON *trace_data_or_shared_trace_data_idJSON);
cJSON *OpenAPI_trace_data_or_shared_trace_data_id_convertToJSON(OpenAPI_trace_data_or_shared_trace_data_id_t *trace_data_or_shared_trace_data_id);
OpenAPI_trace_data_or_shared_trace_data_id_t *OpenAPI_trace_data_or_shared_trace_data_id_copy(OpenAPI_trace_data_or_shared_trace_data_id_t *dst, OpenAPI_trace_data_or_shared_trace_data_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trace_data_or_shared_trace_data_id_H_ */

