/*
 * operator_managed.h
 *
 * Possible values are - OPERATOR_MANAGED: The radio parameters are \&quot;operator managed\&quot;. - NON_OPERATOR_MANAGED: The radio parameters are \&quot;non-operator managed\&quot;.
 */

#ifndef _OpenAPI_operator_managed_H_
#define _OpenAPI_operator_managed_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_operator_managed_s OpenAPI_operator_managed_t;
typedef struct OpenAPI_operator_managed_s {
} OpenAPI_operator_managed_t;

OpenAPI_operator_managed_t *OpenAPI_operator_managed_create(
    );
void OpenAPI_operator_managed_free(OpenAPI_operator_managed_t *operator_managed);
OpenAPI_operator_managed_t *OpenAPI_operator_managed_parseFromJSON(cJSON *operator_managedJSON);
cJSON *OpenAPI_operator_managed_convertToJSON(OpenAPI_operator_managed_t *operator_managed);
OpenAPI_operator_managed_t *OpenAPI_operator_managed_copy(OpenAPI_operator_managed_t *dst, OpenAPI_operator_managed_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_operator_managed_H_ */

