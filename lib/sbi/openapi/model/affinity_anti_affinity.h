/*
 * affinity_anti_affinity.h
 *
 * 
 */

#ifndef _OpenAPI_affinity_anti_affinity_H_
#define _OpenAPI_affinity_anti_affinity_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_affinity_anti_affinity_s OpenAPI_affinity_anti_affinity_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_affinity_anti_affinity_s {
    OpenAPI_list_t *affinity_eas;
    OpenAPI_list_t *anti_affinity_eas;
};

OpenAPI_affinity_anti_affinity_t *OpenAPI_affinity_anti_affinity_create(
    OpenAPI_list_t *affinity_eas,
    OpenAPI_list_t *anti_affinity_eas
);
void OpenAPI_affinity_anti_affinity_free(OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity);
OpenAPI_affinity_anti_affinity_t *OpenAPI_affinity_anti_affinity_parseFromJSON(cJSON *affinity_anti_affinityJSON);
cJSON *OpenAPI_affinity_anti_affinity_convertToJSON(OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity);
OpenAPI_affinity_anti_affinity_t *OpenAPI_affinity_anti_affinity_copy(OpenAPI_affinity_anti_affinity_t *dst, OpenAPI_affinity_anti_affinity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_affinity_anti_affinity_H_ */

