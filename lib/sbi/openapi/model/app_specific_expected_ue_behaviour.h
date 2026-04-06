/*
 * app_specific_expected_ue_behaviour.h
 *
 * Contains Application Specific Expected UE Behaviour Data
 */

#ifndef _OpenAPI_app_specific_expected_ue_behaviour_H_
#define _OpenAPI_app_specific_expected_ue_behaviour_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_app_specific_expected_ue_behaviour_s OpenAPI_app_specific_expected_ue_behaviour_t;
#include "app_specific_expected_ue_behaviour_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_app_specific_expected_ue_behaviour_s {
    char *af_instance_id;
    int reference_id;
    OpenAPI_list_t* app_specific_expected_ue_behaviour_data;
};

OpenAPI_app_specific_expected_ue_behaviour_t *OpenAPI_app_specific_expected_ue_behaviour_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_list_t* app_specific_expected_ue_behaviour_data
);
void OpenAPI_app_specific_expected_ue_behaviour_free(OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour);
OpenAPI_app_specific_expected_ue_behaviour_t *OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON(cJSON *app_specific_expected_ue_behaviourJSON);
cJSON *OpenAPI_app_specific_expected_ue_behaviour_convertToJSON(OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour);
OpenAPI_app_specific_expected_ue_behaviour_t *OpenAPI_app_specific_expected_ue_behaviour_copy(OpenAPI_app_specific_expected_ue_behaviour_t *dst, OpenAPI_app_specific_expected_ue_behaviour_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_specific_expected_ue_behaviour_H_ */

