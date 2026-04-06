/*
 * expected_ue_behaviour_extension.h
 *
 * Contains Expected UE Behaviour Parameters
 */

#ifndef _OpenAPI_expected_ue_behaviour_extension_H_
#define _OpenAPI_expected_ue_behaviour_extension_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_expected_ue_behaviour_extension_s OpenAPI_expected_ue_behaviour_extension_t;
#include "expected_ue_behaviour_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_expected_ue_behaviour_extension_s {
    char *af_instance_id;
    int reference_id;
    OpenAPI_list_t* expected_ue_behaviour_data;
    char *mtc_provider_information;
};

OpenAPI_expected_ue_behaviour_extension_t *OpenAPI_expected_ue_behaviour_extension_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_list_t* expected_ue_behaviour_data,
    char *mtc_provider_information
);
void OpenAPI_expected_ue_behaviour_extension_free(OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension);
OpenAPI_expected_ue_behaviour_extension_t *OpenAPI_expected_ue_behaviour_extension_parseFromJSON(cJSON *expected_ue_behaviour_extensionJSON);
cJSON *OpenAPI_expected_ue_behaviour_extension_convertToJSON(OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension);
OpenAPI_expected_ue_behaviour_extension_t *OpenAPI_expected_ue_behaviour_extension_copy(OpenAPI_expected_ue_behaviour_extension_t *dst, OpenAPI_expected_ue_behaviour_extension_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_ue_behaviour_extension_H_ */

