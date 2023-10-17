/*
 * stationary_indication_rm.h
 *
 * This enumeration is defined in the same way as the &#39;StationaryIndication&#39; enumeration, but with the OpenAPI &#39;nullable: true&#39; property.\&quot; 
 */

#ifndef _OpenAPI_stationary_indication_rm_H_
#define _OpenAPI_stationary_indication_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"
#include "stationary_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_stationary_indication_rm_s OpenAPI_stationary_indication_rm_t;
typedef struct OpenAPI_stationary_indication_rm_s {
} OpenAPI_stationary_indication_rm_t;

OpenAPI_stationary_indication_rm_t *OpenAPI_stationary_indication_rm_create(
);
void OpenAPI_stationary_indication_rm_free(OpenAPI_stationary_indication_rm_t *stationary_indication_rm);
OpenAPI_stationary_indication_rm_t *OpenAPI_stationary_indication_rm_parseFromJSON(cJSON *stationary_indication_rmJSON);
cJSON *OpenAPI_stationary_indication_rm_convertToJSON(OpenAPI_stationary_indication_rm_t *stationary_indication_rm);
OpenAPI_stationary_indication_rm_t *OpenAPI_stationary_indication_rm_copy(OpenAPI_stationary_indication_rm_t *dst, OpenAPI_stationary_indication_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_stationary_indication_rm_H_ */

