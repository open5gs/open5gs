/*
 * nwdaf_cond.h
 *
 * Subscription to a set of NF Instances (NWDAFs), identified by Analytics ID(s).
 */

#ifndef _OpenAPI_nwdaf_cond_H_
#define _OpenAPI_nwdaf_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_cond_s OpenAPI_nwdaf_cond_t;
typedef struct OpenAPI_nwdaf_cond_s {
    OpenAPI_list_t *analytics_ids;
} OpenAPI_nwdaf_cond_t;

OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_create(
    OpenAPI_list_t *analytics_ids
    );
void OpenAPI_nwdaf_cond_free(OpenAPI_nwdaf_cond_t *nwdaf_cond);
OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_parseFromJSON(cJSON *nwdaf_condJSON);
cJSON *OpenAPI_nwdaf_cond_convertToJSON(OpenAPI_nwdaf_cond_t *nwdaf_cond);
OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_copy(OpenAPI_nwdaf_cond_t *dst, OpenAPI_nwdaf_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_cond_H_ */

