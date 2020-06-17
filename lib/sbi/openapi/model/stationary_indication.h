/*
 * stationary_indication.h
 *
 * Possible values are - STATIONARY: Identifies the UE is stationary - MOBILE: Identifies the UE is mobile
 */

#ifndef _OpenAPI_stationary_indication_H_
#define _OpenAPI_stationary_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_stationary_indication_s OpenAPI_stationary_indication_t;
typedef struct OpenAPI_stationary_indication_s {
} OpenAPI_stationary_indication_t;

OpenAPI_stationary_indication_t *OpenAPI_stationary_indication_create(
    );
void OpenAPI_stationary_indication_free(OpenAPI_stationary_indication_t *stationary_indication);
OpenAPI_stationary_indication_t *OpenAPI_stationary_indication_parseFromJSON(cJSON *stationary_indicationJSON);
cJSON *OpenAPI_stationary_indication_convertToJSON(OpenAPI_stationary_indication_t *stationary_indication);
OpenAPI_stationary_indication_t *OpenAPI_stationary_indication_copy(OpenAPI_stationary_indication_t *dst, OpenAPI_stationary_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_stationary_indication_H_ */

