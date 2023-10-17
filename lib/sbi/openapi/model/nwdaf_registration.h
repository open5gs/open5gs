/*
 * nwdaf_registration.h
 *
 * The complete set of information relevant to an NWDAF serving the UE
 */

#ifndef _OpenAPI_nwdaf_registration_H_
#define _OpenAPI_nwdaf_registration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "context_info.h"
#include "event_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_registration_s OpenAPI_nwdaf_registration_t;
typedef struct OpenAPI_nwdaf_registration_s {
    char *nwdaf_instance_id;
    OpenAPI_list_t *analytics_ids;
    char *nwdaf_set_id;
    char *registration_time;
    struct OpenAPI_context_info_s *context_info;
    char *supported_features;
    OpenAPI_list_t *reset_ids;
} OpenAPI_nwdaf_registration_t;

OpenAPI_nwdaf_registration_t *OpenAPI_nwdaf_registration_create(
    char *nwdaf_instance_id,
    OpenAPI_list_t *analytics_ids,
    char *nwdaf_set_id,
    char *registration_time,
    OpenAPI_context_info_t *context_info,
    char *supported_features,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_nwdaf_registration_free(OpenAPI_nwdaf_registration_t *nwdaf_registration);
OpenAPI_nwdaf_registration_t *OpenAPI_nwdaf_registration_parseFromJSON(cJSON *nwdaf_registrationJSON);
cJSON *OpenAPI_nwdaf_registration_convertToJSON(OpenAPI_nwdaf_registration_t *nwdaf_registration);
OpenAPI_nwdaf_registration_t *OpenAPI_nwdaf_registration_copy(OpenAPI_nwdaf_registration_t *dst, OpenAPI_nwdaf_registration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_registration_H_ */

