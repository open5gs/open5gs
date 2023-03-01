/*
 * nwdaf_registration_modification.h
 *
 * Contains attributes of NwdafRegistration that can be modified using PATCH
 */

#ifndef _OpenAPI_nwdaf_registration_modification_H_
#define _OpenAPI_nwdaf_registration_modification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "event_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_registration_modification_s OpenAPI_nwdaf_registration_modification_t;
typedef struct OpenAPI_nwdaf_registration_modification_s {
    char *nwdaf_instance_id;
    char *nwdaf_set_id;
    OpenAPI_list_t *analytics_ids;
    char *supported_features;
} OpenAPI_nwdaf_registration_modification_t;

OpenAPI_nwdaf_registration_modification_t *OpenAPI_nwdaf_registration_modification_create(
    char *nwdaf_instance_id,
    char *nwdaf_set_id,
    OpenAPI_list_t *analytics_ids,
    char *supported_features
);
void OpenAPI_nwdaf_registration_modification_free(OpenAPI_nwdaf_registration_modification_t *nwdaf_registration_modification);
OpenAPI_nwdaf_registration_modification_t *OpenAPI_nwdaf_registration_modification_parseFromJSON(cJSON *nwdaf_registration_modificationJSON);
cJSON *OpenAPI_nwdaf_registration_modification_convertToJSON(OpenAPI_nwdaf_registration_modification_t *nwdaf_registration_modification);
OpenAPI_nwdaf_registration_modification_t *OpenAPI_nwdaf_registration_modification_copy(OpenAPI_nwdaf_registration_modification_t *dst, OpenAPI_nwdaf_registration_modification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_registration_modification_H_ */

