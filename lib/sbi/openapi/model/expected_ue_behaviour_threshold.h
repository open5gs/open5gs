/*
 * expected_ue_behaviour_threshold.h
 *
 * Expected UE Behaviour Threshold
 */

#ifndef _OpenAPI_expected_ue_behaviour_threshold_H_
#define _OpenAPI_expected_ue_behaviour_threshold_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_expected_ue_behaviour_threshold_s OpenAPI_expected_ue_behaviour_threshold_t;
#include "expected_ue_behaviour_dataset.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_expected_ue_behaviour_threshold_s {
    OpenAPI_list_t *expected_ue_behaviour_datasets;
    OpenAPI_list_t *single_nssais;
    OpenAPI_list_t *dnns;
    char *confidence_level;
    char *accuracy_level;
};

OpenAPI_expected_ue_behaviour_threshold_t *OpenAPI_expected_ue_behaviour_threshold_create(
    OpenAPI_list_t *expected_ue_behaviour_datasets,
    OpenAPI_list_t *single_nssais,
    OpenAPI_list_t *dnns,
    char *confidence_level,
    char *accuracy_level
);
void OpenAPI_expected_ue_behaviour_threshold_free(OpenAPI_expected_ue_behaviour_threshold_t *expected_ue_behaviour_threshold);
OpenAPI_expected_ue_behaviour_threshold_t *OpenAPI_expected_ue_behaviour_threshold_parseFromJSON(cJSON *expected_ue_behaviour_thresholdJSON);
cJSON *OpenAPI_expected_ue_behaviour_threshold_convertToJSON(OpenAPI_expected_ue_behaviour_threshold_t *expected_ue_behaviour_threshold);
OpenAPI_expected_ue_behaviour_threshold_t *OpenAPI_expected_ue_behaviour_threshold_copy(OpenAPI_expected_ue_behaviour_threshold_t *dst, OpenAPI_expected_ue_behaviour_threshold_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_ue_behaviour_threshold_H_ */

