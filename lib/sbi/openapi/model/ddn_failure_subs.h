/*
 * ddn_failure_subs.h
 *
 *
 */

#ifndef _OpenAPI_ddn_failure_subs_H_
#define _OpenAPI_ddn_failure_subs_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ddd_traffic_descriptor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ddn_failure_subs_s OpenAPI_ddn_failure_subs_t;
typedef struct OpenAPI_ddn_failure_subs_s {
    int ddn_failure_subs_ind;
    OpenAPI_list_t *ddd_traffic_descriptor_list;
} OpenAPI_ddn_failure_subs_t;

OpenAPI_ddn_failure_subs_t *OpenAPI_ddn_failure_subs_create(
    int ddn_failure_subs_ind,
    OpenAPI_list_t *ddd_traffic_descriptor_list
    );
void OpenAPI_ddn_failure_subs_free(OpenAPI_ddn_failure_subs_t *ddn_failure_subs);
OpenAPI_ddn_failure_subs_t *OpenAPI_ddn_failure_subs_parseFromJSON(cJSON *ddn_failure_subsJSON);
cJSON *OpenAPI_ddn_failure_subs_convertToJSON(OpenAPI_ddn_failure_subs_t *ddn_failure_subs);
OpenAPI_ddn_failure_subs_t *OpenAPI_ddn_failure_subs_copy(OpenAPI_ddn_failure_subs_t *dst, OpenAPI_ddn_failure_subs_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ddn_failure_subs_H_ */

