/*
 * policy_data_for_individual_ue.h
 *
 * Contains policy data for a given subscriber.
 */

#ifndef _OpenAPI_policy_data_for_individual_ue_H_
#define _OpenAPI_policy_data_for_individual_ue_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "am_policy_data.h"
#include "operator_specific_data_container.h"
#include "sm_policy_data.h"
#include "ue_policy_set.h"
#include "usage_mon_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_policy_data_for_individual_ue_s OpenAPI_policy_data_for_individual_ue_t;
typedef struct OpenAPI_policy_data_for_individual_ue_s {
    struct OpenAPI_ue_policy_set_s *ue_policy_data_set;
    struct OpenAPI_sm_policy_data_s *sm_policy_data_set;
    struct OpenAPI_am_policy_data_s *am_policy_data_set;
    OpenAPI_list_t* um_data;
    OpenAPI_list_t* operator_specific_data_set;
} OpenAPI_policy_data_for_individual_ue_t;

OpenAPI_policy_data_for_individual_ue_t *OpenAPI_policy_data_for_individual_ue_create(
    OpenAPI_ue_policy_set_t *ue_policy_data_set,
    OpenAPI_sm_policy_data_t *sm_policy_data_set,
    OpenAPI_am_policy_data_t *am_policy_data_set,
    OpenAPI_list_t* um_data,
    OpenAPI_list_t* operator_specific_data_set
);
void OpenAPI_policy_data_for_individual_ue_free(OpenAPI_policy_data_for_individual_ue_t *policy_data_for_individual_ue);
OpenAPI_policy_data_for_individual_ue_t *OpenAPI_policy_data_for_individual_ue_parseFromJSON(cJSON *policy_data_for_individual_ueJSON);
cJSON *OpenAPI_policy_data_for_individual_ue_convertToJSON(OpenAPI_policy_data_for_individual_ue_t *policy_data_for_individual_ue);
OpenAPI_policy_data_for_individual_ue_t *OpenAPI_policy_data_for_individual_ue_copy(OpenAPI_policy_data_for_individual_ue_t *dst, OpenAPI_policy_data_for_individual_ue_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_data_for_individual_ue_H_ */

