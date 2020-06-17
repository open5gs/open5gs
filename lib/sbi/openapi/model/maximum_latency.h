/*
 * maximum_latency.h
 *
 *
 */

#ifndef _OpenAPI_maximum_latency_H_
#define _OpenAPI_maximum_latency_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_maximum_latency_s OpenAPI_maximum_latency_t;
typedef struct OpenAPI_maximum_latency_s {
    int maximum_latency;
    struct OpenAPI_snssai_s *snssai;
    char *dnn;
    char *validity_time;
} OpenAPI_maximum_latency_t;

OpenAPI_maximum_latency_t *OpenAPI_maximum_latency_create(
    int maximum_latency,
    OpenAPI_snssai_t *snssai,
    char *dnn,
    char *validity_time
    );
void OpenAPI_maximum_latency_free(OpenAPI_maximum_latency_t *maximum_latency);
OpenAPI_maximum_latency_t *OpenAPI_maximum_latency_parseFromJSON(cJSON *maximum_latencyJSON);
cJSON *OpenAPI_maximum_latency_convertToJSON(OpenAPI_maximum_latency_t *maximum_latency);
OpenAPI_maximum_latency_t *OpenAPI_maximum_latency_copy(OpenAPI_maximum_latency_t *dst, OpenAPI_maximum_latency_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_maximum_latency_H_ */

