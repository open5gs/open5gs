/*
 * sm_comm_failure_filter.h
 *
 * Filter for Session Management Communication Failure
 */

#ifndef _OpenAPI_sm_comm_failure_filter_H_
#define _OpenAPI_sm_comm_failure_filter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sm_comm_failure_filter_s OpenAPI_sm_comm_failure_filter_t;
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sm_comm_failure_filter_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
};

OpenAPI_sm_comm_failure_filter_t *OpenAPI_sm_comm_failure_filter_create(
    char *dnn,
    OpenAPI_snssai_t *snssai
);
void OpenAPI_sm_comm_failure_filter_free(OpenAPI_sm_comm_failure_filter_t *sm_comm_failure_filter);
OpenAPI_sm_comm_failure_filter_t *OpenAPI_sm_comm_failure_filter_parseFromJSON(cJSON *sm_comm_failure_filterJSON);
cJSON *OpenAPI_sm_comm_failure_filter_convertToJSON(OpenAPI_sm_comm_failure_filter_t *sm_comm_failure_filter);
OpenAPI_sm_comm_failure_filter_t *OpenAPI_sm_comm_failure_filter_copy(OpenAPI_sm_comm_failure_filter_t *dst, OpenAPI_sm_comm_failure_filter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_comm_failure_filter_H_ */

