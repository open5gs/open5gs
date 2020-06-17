/*
 * pp_maximum_latency.h
 *
 *
 */

#ifndef _OpenAPI_pp_maximum_latency_H_
#define _OpenAPI_pp_maximum_latency_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pp_maximum_latency_s OpenAPI_pp_maximum_latency_t;
typedef struct OpenAPI_pp_maximum_latency_s {
    int maximum_latency;
    char *af_instance_id;
    int reference_id;
    char *validity_time;
} OpenAPI_pp_maximum_latency_t;

OpenAPI_pp_maximum_latency_t *OpenAPI_pp_maximum_latency_create(
    int maximum_latency,
    char *af_instance_id,
    int reference_id,
    char *validity_time
    );
void OpenAPI_pp_maximum_latency_free(OpenAPI_pp_maximum_latency_t *pp_maximum_latency);
OpenAPI_pp_maximum_latency_t *OpenAPI_pp_maximum_latency_parseFromJSON(cJSON *pp_maximum_latencyJSON);
cJSON *OpenAPI_pp_maximum_latency_convertToJSON(OpenAPI_pp_maximum_latency_t *pp_maximum_latency);
OpenAPI_pp_maximum_latency_t *OpenAPI_pp_maximum_latency_copy(OpenAPI_pp_maximum_latency_t *dst, OpenAPI_pp_maximum_latency_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_maximum_latency_H_ */

