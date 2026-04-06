/*
 * app_specific_expected_ue_behaviour_data_1.h
 *
 * Application specific expected UE behaviour data
 */

#ifndef _OpenAPI_app_specific_expected_ue_behaviour_data_1_H_
#define _OpenAPI_app_specific_expected_ue_behaviour_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_app_specific_expected_ue_behaviour_data_1_s OpenAPI_app_specific_expected_ue_behaviour_data_1_t;
#include "flow_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_app_specific_expected_ue_behaviour_data_1_s {
    char *app_id;
    OpenAPI_list_t *traffic_filters;
    bool is_expected_inactivity_time;
    int expected_inactivity_time;
    char *validity_time;
    char *confidence_level;
    char *accuracy_level;
};

OpenAPI_app_specific_expected_ue_behaviour_data_1_t *OpenAPI_app_specific_expected_ue_behaviour_data_1_create(
    char *app_id,
    OpenAPI_list_t *traffic_filters,
    bool is_expected_inactivity_time,
    int expected_inactivity_time,
    char *validity_time,
    char *confidence_level,
    char *accuracy_level
);
void OpenAPI_app_specific_expected_ue_behaviour_data_1_free(OpenAPI_app_specific_expected_ue_behaviour_data_1_t *app_specific_expected_ue_behaviour_data_1);
OpenAPI_app_specific_expected_ue_behaviour_data_1_t *OpenAPI_app_specific_expected_ue_behaviour_data_1_parseFromJSON(cJSON *app_specific_expected_ue_behaviour_data_1JSON);
cJSON *OpenAPI_app_specific_expected_ue_behaviour_data_1_convertToJSON(OpenAPI_app_specific_expected_ue_behaviour_data_1_t *app_specific_expected_ue_behaviour_data_1);
OpenAPI_app_specific_expected_ue_behaviour_data_1_t *OpenAPI_app_specific_expected_ue_behaviour_data_1_copy(OpenAPI_app_specific_expected_ue_behaviour_data_1_t *dst, OpenAPI_app_specific_expected_ue_behaviour_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_specific_expected_ue_behaviour_data_1_H_ */

