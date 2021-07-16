/*
 * ue_context_release.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_release_H_
#define _OpenAPI_ue_context_release_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ng_ap_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_release_s OpenAPI_ue_context_release_t;
typedef struct OpenAPI_ue_context_release_s {
    char *supi;
    bool is_unauthenticated_supi;
    int unauthenticated_supi;
    struct OpenAPI_ng_ap_cause_s *ngap_cause;
} OpenAPI_ue_context_release_t;

OpenAPI_ue_context_release_t *OpenAPI_ue_context_release_create(
    char *supi,
    bool is_unauthenticated_supi,
    int unauthenticated_supi,
    OpenAPI_ng_ap_cause_t *ngap_cause
);
void OpenAPI_ue_context_release_free(OpenAPI_ue_context_release_t *ue_context_release);
OpenAPI_ue_context_release_t *OpenAPI_ue_context_release_parseFromJSON(cJSON *ue_context_releaseJSON);
cJSON *OpenAPI_ue_context_release_convertToJSON(OpenAPI_ue_context_release_t *ue_context_release);
OpenAPI_ue_context_release_t *OpenAPI_ue_context_release_copy(OpenAPI_ue_context_release_t *dst, OpenAPI_ue_context_release_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_release_H_ */

