/*
 * bdt_policy_status.h
 *
 * Indicates the validation status of a negotiated BDT policy.
 */

#ifndef _OpenAPI_bdt_policy_status_H_
#define _OpenAPI_bdt_policy_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "bdt_policy_status_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bdt_policy_status_s OpenAPI_bdt_policy_status_t;
typedef struct OpenAPI_bdt_policy_status_s {
} OpenAPI_bdt_policy_status_t;

OpenAPI_bdt_policy_status_t *OpenAPI_bdt_policy_status_create(
);
void OpenAPI_bdt_policy_status_free(OpenAPI_bdt_policy_status_t *bdt_policy_status);
OpenAPI_bdt_policy_status_t *OpenAPI_bdt_policy_status_parseFromJSON(cJSON *bdt_policy_statusJSON);
cJSON *OpenAPI_bdt_policy_status_convertToJSON(OpenAPI_bdt_policy_status_t *bdt_policy_status);
OpenAPI_bdt_policy_status_t *OpenAPI_bdt_policy_status_copy(OpenAPI_bdt_policy_status_t *dst, OpenAPI_bdt_policy_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bdt_policy_status_H_ */

