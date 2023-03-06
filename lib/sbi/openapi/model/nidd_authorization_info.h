/*
 * nidd_authorization_info.h
 *
 * Information related to active NIDD Authorizations
 */

#ifndef _OpenAPI_nidd_authorization_info_H_
#define _OpenAPI_nidd_authorization_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "authorization_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nidd_authorization_info_s OpenAPI_nidd_authorization_info_t;
typedef struct OpenAPI_nidd_authorization_info_s {
    OpenAPI_list_t *nidd_authorization_list;
} OpenAPI_nidd_authorization_info_t;

OpenAPI_nidd_authorization_info_t *OpenAPI_nidd_authorization_info_create(
    OpenAPI_list_t *nidd_authorization_list
);
void OpenAPI_nidd_authorization_info_free(OpenAPI_nidd_authorization_info_t *nidd_authorization_info);
OpenAPI_nidd_authorization_info_t *OpenAPI_nidd_authorization_info_parseFromJSON(cJSON *nidd_authorization_infoJSON);
cJSON *OpenAPI_nidd_authorization_info_convertToJSON(OpenAPI_nidd_authorization_info_t *nidd_authorization_info);
OpenAPI_nidd_authorization_info_t *OpenAPI_nidd_authorization_info_copy(OpenAPI_nidd_authorization_info_t *dst, OpenAPI_nidd_authorization_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nidd_authorization_info_H_ */

