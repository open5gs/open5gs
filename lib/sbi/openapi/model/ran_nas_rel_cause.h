/*
 * ran_nas_rel_cause.h
 *
 *
 */

#ifndef _OpenAPI_ran_nas_rel_cause_H_
#define _OpenAPI_ran_nas_rel_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ng_ap_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ran_nas_rel_cause_s OpenAPI_ran_nas_rel_cause_t;
typedef struct OpenAPI_ran_nas_rel_cause_s {
    struct OpenAPI_ng_ap_cause_s *ng_ap_cause;
    int _5g_mm_cause;
    int _5g_sm_cause;
} OpenAPI_ran_nas_rel_cause_t;

OpenAPI_ran_nas_rel_cause_t *OpenAPI_ran_nas_rel_cause_create(
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    int _5g_mm_cause,
    int _5g_sm_cause
    );
void OpenAPI_ran_nas_rel_cause_free(OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_cause);
OpenAPI_ran_nas_rel_cause_t *OpenAPI_ran_nas_rel_cause_parseFromJSON(cJSON *ran_nas_rel_causeJSON);
cJSON *OpenAPI_ran_nas_rel_cause_convertToJSON(OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_cause);
OpenAPI_ran_nas_rel_cause_t *OpenAPI_ran_nas_rel_cause_copy(OpenAPI_ran_nas_rel_cause_t *dst, OpenAPI_ran_nas_rel_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ran_nas_rel_cause_H_ */

