/*
 * nef_cond.h
 *
 * Subscription to a set of NF Instances (NEFs), identified by Event ID(s) provided by AF.
 */

#ifndef _OpenAPI_nef_cond_H_
#define _OpenAPI_nef_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nef_cond_s OpenAPI_nef_cond_t;
typedef struct OpenAPI_nef_cond_s {
    OpenAPI_list_t *event_ids;
} OpenAPI_nef_cond_t;

OpenAPI_nef_cond_t *OpenAPI_nef_cond_create(
    OpenAPI_list_t *event_ids
    );
void OpenAPI_nef_cond_free(OpenAPI_nef_cond_t *nef_cond);
OpenAPI_nef_cond_t *OpenAPI_nef_cond_parseFromJSON(cJSON *nef_condJSON);
cJSON *OpenAPI_nef_cond_convertToJSON(OpenAPI_nef_cond_t *nef_cond);
OpenAPI_nef_cond_t *OpenAPI_nef_cond_copy(OpenAPI_nef_cond_t *dst, OpenAPI_nef_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nef_cond_H_ */

