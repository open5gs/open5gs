/*
 * dnai_change_type.h
 *
 * Possible values are - EARLY: Early notification of UP path reconfiguration. - EARLY_LATE: Early and late notification of UP path reconfiguration. This value shall only be present in the subscription to the DNAI change event. - LATE: Late notification of UP path reconfiguration.
 */

#ifndef _OpenAPI_dnai_change_type_H_
#define _OpenAPI_dnai_change_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnai_change_type_s OpenAPI_dnai_change_type_t;
typedef struct OpenAPI_dnai_change_type_s {
} OpenAPI_dnai_change_type_t;

OpenAPI_dnai_change_type_t *OpenAPI_dnai_change_type_create(
    );
void OpenAPI_dnai_change_type_free(OpenAPI_dnai_change_type_t *dnai_change_type);
OpenAPI_dnai_change_type_t *OpenAPI_dnai_change_type_parseFromJSON(cJSON *dnai_change_typeJSON);
cJSON *OpenAPI_dnai_change_type_convertToJSON(OpenAPI_dnai_change_type_t *dnai_change_type);
OpenAPI_dnai_change_type_t *OpenAPI_dnai_change_type_copy(OpenAPI_dnai_change_type_t *dst, OpenAPI_dnai_change_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnai_change_type_H_ */

