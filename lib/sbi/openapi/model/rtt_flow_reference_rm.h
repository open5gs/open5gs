/*
 * rtt_flow_reference_rm.h
 *
 * It is defined as the RttFlowRerence data type but with the OpenAPI nullable true property. 
 */

#ifndef _OpenAPI_rtt_flow_reference_rm_H_
#define _OpenAPI_rtt_flow_reference_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rtt_flow_reference_rm_s OpenAPI_rtt_flow_reference_rm_t;
#include "flow_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rtt_flow_reference_rm_s {
    OpenAPI_flow_direction_e flow_dir;
    int shared_key;
};

OpenAPI_rtt_flow_reference_rm_t *OpenAPI_rtt_flow_reference_rm_create(
    OpenAPI_flow_direction_e flow_dir,
    int shared_key
);
void OpenAPI_rtt_flow_reference_rm_free(OpenAPI_rtt_flow_reference_rm_t *rtt_flow_reference_rm);
OpenAPI_rtt_flow_reference_rm_t *OpenAPI_rtt_flow_reference_rm_parseFromJSON(cJSON *rtt_flow_reference_rmJSON);
cJSON *OpenAPI_rtt_flow_reference_rm_convertToJSON(OpenAPI_rtt_flow_reference_rm_t *rtt_flow_reference_rm);
OpenAPI_rtt_flow_reference_rm_t *OpenAPI_rtt_flow_reference_rm_copy(OpenAPI_rtt_flow_reference_rm_t *dst, OpenAPI_rtt_flow_reference_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rtt_flow_reference_rm_H_ */

