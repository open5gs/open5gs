/*
 * rtt_flow_reference.h
 *
 * Contains the shared key with the media subcomponent that shares the subscription to  round trip time measurements in the complementary direction. 
 */

#ifndef _OpenAPI_rtt_flow_reference_H_
#define _OpenAPI_rtt_flow_reference_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rtt_flow_reference_s OpenAPI_rtt_flow_reference_t;
#include "flow_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rtt_flow_reference_s {
    OpenAPI_flow_direction_e flow_dir;
    int shared_key;
};

OpenAPI_rtt_flow_reference_t *OpenAPI_rtt_flow_reference_create(
    OpenAPI_flow_direction_e flow_dir,
    int shared_key
);
void OpenAPI_rtt_flow_reference_free(OpenAPI_rtt_flow_reference_t *rtt_flow_reference);
OpenAPI_rtt_flow_reference_t *OpenAPI_rtt_flow_reference_parseFromJSON(cJSON *rtt_flow_referenceJSON);
cJSON *OpenAPI_rtt_flow_reference_convertToJSON(OpenAPI_rtt_flow_reference_t *rtt_flow_reference);
OpenAPI_rtt_flow_reference_t *OpenAPI_rtt_flow_reference_copy(OpenAPI_rtt_flow_reference_t *dst, OpenAPI_rtt_flow_reference_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rtt_flow_reference_H_ */

