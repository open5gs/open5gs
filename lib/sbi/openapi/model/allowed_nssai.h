/*
 * allowed_nssai.h
 *
 * Contains an array of allowed S-NSSAI that constitute the allowed NSSAI information for the authorized network slice information 
 */

#ifndef _OpenAPI_allowed_nssai_H_
#define _OpenAPI_allowed_nssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "allowed_snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_allowed_nssai_s OpenAPI_allowed_nssai_t;
typedef struct OpenAPI_allowed_nssai_s {
    OpenAPI_list_t *allowed_snssai_list;
    OpenAPI_access_type_e access_type;
} OpenAPI_allowed_nssai_t;

OpenAPI_allowed_nssai_t *OpenAPI_allowed_nssai_create(
    OpenAPI_list_t *allowed_snssai_list,
    OpenAPI_access_type_e access_type
);
void OpenAPI_allowed_nssai_free(OpenAPI_allowed_nssai_t *allowed_nssai);
OpenAPI_allowed_nssai_t *OpenAPI_allowed_nssai_parseFromJSON(cJSON *allowed_nssaiJSON);
cJSON *OpenAPI_allowed_nssai_convertToJSON(OpenAPI_allowed_nssai_t *allowed_nssai);
OpenAPI_allowed_nssai_t *OpenAPI_allowed_nssai_copy(OpenAPI_allowed_nssai_t *dst, OpenAPI_allowed_nssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allowed_nssai_H_ */

