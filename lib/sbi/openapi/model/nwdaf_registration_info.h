/*
 * nwdaf_registration_info.h
 *
 * List of NwdafRegistration
 */

#ifndef _OpenAPI_nwdaf_registration_info_H_
#define _OpenAPI_nwdaf_registration_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nwdaf_registration.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_registration_info_s OpenAPI_nwdaf_registration_info_t;
typedef struct OpenAPI_nwdaf_registration_info_s {
    OpenAPI_list_t *nwdaf_registration_list;
} OpenAPI_nwdaf_registration_info_t;

OpenAPI_nwdaf_registration_info_t *OpenAPI_nwdaf_registration_info_create(
    OpenAPI_list_t *nwdaf_registration_list
);
void OpenAPI_nwdaf_registration_info_free(OpenAPI_nwdaf_registration_info_t *nwdaf_registration_info);
OpenAPI_nwdaf_registration_info_t *OpenAPI_nwdaf_registration_info_parseFromJSON(cJSON *nwdaf_registration_infoJSON);
cJSON *OpenAPI_nwdaf_registration_info_convertToJSON(OpenAPI_nwdaf_registration_info_t *nwdaf_registration_info);
OpenAPI_nwdaf_registration_info_t *OpenAPI_nwdaf_registration_info_copy(OpenAPI_nwdaf_registration_info_t *dst, OpenAPI_nwdaf_registration_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_registration_info_H_ */

