/*
 * hss_auth_type.h
 *
 *
 */

#ifndef _OpenAPI_hss_auth_type_H_
#define _OpenAPI_hss_auth_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hss_auth_type_s OpenAPI_hss_auth_type_t;
typedef struct OpenAPI_hss_auth_type_s {
} OpenAPI_hss_auth_type_t;

OpenAPI_hss_auth_type_t *OpenAPI_hss_auth_type_create(
    );
void OpenAPI_hss_auth_type_free(OpenAPI_hss_auth_type_t *hss_auth_type);
OpenAPI_hss_auth_type_t *OpenAPI_hss_auth_type_parseFromJSON(cJSON *hss_auth_typeJSON);
cJSON *OpenAPI_hss_auth_type_convertToJSON(OpenAPI_hss_auth_type_t *hss_auth_type);
OpenAPI_hss_auth_type_t *OpenAPI_hss_auth_type_copy(OpenAPI_hss_auth_type_t *dst, OpenAPI_hss_auth_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_auth_type_H_ */

