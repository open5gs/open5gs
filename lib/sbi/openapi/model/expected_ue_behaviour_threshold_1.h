/*
 * expected_ue_behaviour_threshold_1.h
 *
 * Expected UE Behaviour Threshold
 */

#ifndef _OpenAPI_expected_ue_behaviour_threshold_1_H_
#define _OpenAPI_expected_ue_behaviour_threshold_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_expected_ue_behaviour_threshold_1_s OpenAPI_expected_ue_behaviour_threshold_1_t;
#include "expected_ue_behaviour_dataset.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_expected_ue_behaviour_threshold_1_s {
    OpenAPI_list_t *expected_ue_behaviour_datasets;
    OpenAPI_list_t *single_nssais;
    OpenAPI_list_t *dnns;
    char *confidence_level;
    char *accuracy_level;
};

OpenAPI_expected_ue_behaviour_threshold_1_t *OpenAPI_expected_ue_behaviour_threshold_1_create(
    OpenAPI_list_t *expected_ue_behaviour_datasets,
    OpenAPI_list_t *single_nssais,
    OpenAPI_list_t *dnns,
    char *confidence_level,
    char *accuracy_level
);
void OpenAPI_expected_ue_behaviour_threshold_1_free(OpenAPI_expected_ue_behaviour_threshold_1_t *expected_ue_behaviour_threshold_1);
OpenAPI_expected_ue_behaviour_threshold_1_t *OpenAPI_expected_ue_behaviour_threshold_1_parseFromJSON(cJSON *expected_ue_behaviour_threshold_1JSON);
cJSON *OpenAPI_expected_ue_behaviour_threshold_1_convertToJSON(OpenAPI_expected_ue_behaviour_threshold_1_t *expected_ue_behaviour_threshold_1);
OpenAPI_expected_ue_behaviour_threshold_1_t *OpenAPI_expected_ue_behaviour_threshold_1_copy(OpenAPI_expected_ue_behaviour_threshold_1_t *dst, OpenAPI_expected_ue_behaviour_threshold_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_ue_behaviour_threshold_1_H_ */

