/*
 * lcs_priority.h
 *
 * Indicates priority of the LCS client.
 */

#ifndef _OpenAPI_lcs_priority_H_
#define _OpenAPI_lcs_priority_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lcs_priority_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lcs_priority_s OpenAPI_lcs_priority_t;
typedef struct OpenAPI_lcs_priority_s {
} OpenAPI_lcs_priority_t;

OpenAPI_lcs_priority_t *OpenAPI_lcs_priority_create(void);
void OpenAPI_lcs_priority_free(OpenAPI_lcs_priority_t *lcs_priority);
OpenAPI_lcs_priority_t *OpenAPI_lcs_priority_parseFromJSON(cJSON *lcs_priorityJSON);
cJSON *OpenAPI_lcs_priority_convertToJSON(OpenAPI_lcs_priority_t *lcs_priority);
OpenAPI_lcs_priority_t *OpenAPI_lcs_priority_copy(OpenAPI_lcs_priority_t *dst, OpenAPI_lcs_priority_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_priority_H_ */

