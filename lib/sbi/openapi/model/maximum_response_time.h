/*
 * maximum_response_time.h
 *
 *
 */

#ifndef _OpenAPI_maximum_response_time_H_
#define _OpenAPI_maximum_response_time_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_maximum_response_time_s OpenAPI_maximum_response_time_t;
typedef struct OpenAPI_maximum_response_time_s {
    int maximum_response_time;
    struct OpenAPI_snssai_s *snssai;
    char *dnn;
    char *validity_time;
} OpenAPI_maximum_response_time_t;

OpenAPI_maximum_response_time_t *OpenAPI_maximum_response_time_create(
    int maximum_response_time,
    OpenAPI_snssai_t *snssai,
    char *dnn,
    char *validity_time
    );
void OpenAPI_maximum_response_time_free(OpenAPI_maximum_response_time_t *maximum_response_time);
OpenAPI_maximum_response_time_t *OpenAPI_maximum_response_time_parseFromJSON(cJSON *maximum_response_timeJSON);
cJSON *OpenAPI_maximum_response_time_convertToJSON(OpenAPI_maximum_response_time_t *maximum_response_time);
OpenAPI_maximum_response_time_t *OpenAPI_maximum_response_time_copy(OpenAPI_maximum_response_time_t *dst, OpenAPI_maximum_response_time_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_maximum_response_time_H_ */

