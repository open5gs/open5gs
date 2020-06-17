/*
 * ng_ap_cause.h
 *
 *
 */

#ifndef _OpenAPI_ng_ap_cause_H_
#define _OpenAPI_ng_ap_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ng_ap_cause_s OpenAPI_ng_ap_cause_t;
typedef struct OpenAPI_ng_ap_cause_s {
    int group;
    int value;
} OpenAPI_ng_ap_cause_t;

OpenAPI_ng_ap_cause_t *OpenAPI_ng_ap_cause_create(
    int group,
    int value
    );
void OpenAPI_ng_ap_cause_free(OpenAPI_ng_ap_cause_t *ng_ap_cause);
OpenAPI_ng_ap_cause_t *OpenAPI_ng_ap_cause_parseFromJSON(cJSON *ng_ap_causeJSON);
cJSON *OpenAPI_ng_ap_cause_convertToJSON(OpenAPI_ng_ap_cause_t *ng_ap_cause);
OpenAPI_ng_ap_cause_t *OpenAPI_ng_ap_cause_copy(OpenAPI_ng_ap_cause_t *dst, OpenAPI_ng_ap_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ng_ap_cause_H_ */

