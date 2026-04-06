/*
 * ue_access_behavior_report_item.h
 *
 * Report Item for UE Access Behavior Trends event.
 */

#ifndef _OpenAPI_ue_access_behavior_report_item_H_
#define _OpenAPI_ue_access_behavior_report_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_access_behavior_report_item_s OpenAPI_ue_access_behavior_report_item_t;
#include "access_state_transition_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_access_behavior_report_item_s {
    OpenAPI_access_state_transition_type_e state_transition_type;
    int spacing;
    bool is_spacing_var;
    float spacing_var;
    int duration;
    bool is_duration_var;
    float duration_var;
    bool is_occurrences;
    int occurrences;
    char *timestamp;
};

OpenAPI_ue_access_behavior_report_item_t *OpenAPI_ue_access_behavior_report_item_create(
    OpenAPI_access_state_transition_type_e state_transition_type,
    int spacing,
    bool is_spacing_var,
    float spacing_var,
    int duration,
    bool is_duration_var,
    float duration_var,
    bool is_occurrences,
    int occurrences,
    char *timestamp
);
void OpenAPI_ue_access_behavior_report_item_free(OpenAPI_ue_access_behavior_report_item_t *ue_access_behavior_report_item);
OpenAPI_ue_access_behavior_report_item_t *OpenAPI_ue_access_behavior_report_item_parseFromJSON(cJSON *ue_access_behavior_report_itemJSON);
cJSON *OpenAPI_ue_access_behavior_report_item_convertToJSON(OpenAPI_ue_access_behavior_report_item_t *ue_access_behavior_report_item);
OpenAPI_ue_access_behavior_report_item_t *OpenAPI_ue_access_behavior_report_item_copy(OpenAPI_ue_access_behavior_report_item_t *dst, OpenAPI_ue_access_behavior_report_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_access_behavior_report_item_H_ */

