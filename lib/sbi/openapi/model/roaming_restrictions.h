/*
 * roaming_restrictions.h
 *
 * Indicates if access is allowed to a given serving network, e.g. a PLMN (MCC, MNC) or an  SNPN (MCC, MNC, NID). 
 */

#ifndef _OpenAPI_roaming_restrictions_H_
#define _OpenAPI_roaming_restrictions_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_roaming_restrictions_s OpenAPI_roaming_restrictions_t;
typedef struct OpenAPI_roaming_restrictions_s {
    bool is_access_allowed;
    int access_allowed;
} OpenAPI_roaming_restrictions_t;

OpenAPI_roaming_restrictions_t *OpenAPI_roaming_restrictions_create(
    bool is_access_allowed,
    int access_allowed
);
void OpenAPI_roaming_restrictions_free(OpenAPI_roaming_restrictions_t *roaming_restrictions);
OpenAPI_roaming_restrictions_t *OpenAPI_roaming_restrictions_parseFromJSON(cJSON *roaming_restrictionsJSON);
cJSON *OpenAPI_roaming_restrictions_convertToJSON(OpenAPI_roaming_restrictions_t *roaming_restrictions);
OpenAPI_roaming_restrictions_t *OpenAPI_roaming_restrictions_copy(OpenAPI_roaming_restrictions_t *dst, OpenAPI_roaming_restrictions_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roaming_restrictions_H_ */

