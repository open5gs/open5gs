/*
 * max_integrity_protected_data_rate.h
 *
 * Possible values are   - 64_KBPS   - MAX_UE_RATE
 */

#ifndef _OpenAPI_max_integrity_protected_data_rate_H_
#define _OpenAPI_max_integrity_protected_data_rate_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_max_integrity_protected_data_rate_s OpenAPI_max_integrity_protected_data_rate_t;
typedef struct OpenAPI_max_integrity_protected_data_rate_s {
} OpenAPI_max_integrity_protected_data_rate_t;

OpenAPI_max_integrity_protected_data_rate_t *OpenAPI_max_integrity_protected_data_rate_create(
    );
void OpenAPI_max_integrity_protected_data_rate_free(OpenAPI_max_integrity_protected_data_rate_t *max_integrity_protected_data_rate);
OpenAPI_max_integrity_protected_data_rate_t *OpenAPI_max_integrity_protected_data_rate_parseFromJSON(cJSON *max_integrity_protected_data_rateJSON);
cJSON *OpenAPI_max_integrity_protected_data_rate_convertToJSON(OpenAPI_max_integrity_protected_data_rate_t *max_integrity_protected_data_rate);
OpenAPI_max_integrity_protected_data_rate_t *OpenAPI_max_integrity_protected_data_rate_copy(OpenAPI_max_integrity_protected_data_rate_t *dst, OpenAPI_max_integrity_protected_data_rate_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_max_integrity_protected_data_rate_H_ */

