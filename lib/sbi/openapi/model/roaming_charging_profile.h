/*
 * roaming_charging_profile.h
 *
 * 
 */

#ifndef _OpenAPI_roaming_charging_profile_H_
#define _OpenAPI_roaming_charging_profile_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "partial_record_method.h"
#include "trigger.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_roaming_charging_profile_s OpenAPI_roaming_charging_profile_t;
typedef struct OpenAPI_roaming_charging_profile_s {
    OpenAPI_list_t *triggers;
    struct OpenAPI_partial_record_method_s *partial_record_method;
} OpenAPI_roaming_charging_profile_t;

OpenAPI_roaming_charging_profile_t *OpenAPI_roaming_charging_profile_create(
    OpenAPI_list_t *triggers,
    OpenAPI_partial_record_method_t *partial_record_method
);
void OpenAPI_roaming_charging_profile_free(OpenAPI_roaming_charging_profile_t *roaming_charging_profile);
OpenAPI_roaming_charging_profile_t *OpenAPI_roaming_charging_profile_parseFromJSON(cJSON *roaming_charging_profileJSON);
cJSON *OpenAPI_roaming_charging_profile_convertToJSON(OpenAPI_roaming_charging_profile_t *roaming_charging_profile);
OpenAPI_roaming_charging_profile_t *OpenAPI_roaming_charging_profile_copy(OpenAPI_roaming_charging_profile_t *dst, OpenAPI_roaming_charging_profile_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roaming_charging_profile_H_ */

