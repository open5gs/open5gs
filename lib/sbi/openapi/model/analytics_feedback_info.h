/*
 * analytics_feedback_info.h
 *
 * Analytics feedback information.
 */

#ifndef _OpenAPI_analytics_feedback_info_H_
#define _OpenAPI_analytics_feedback_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_analytics_feedback_info_s OpenAPI_analytics_feedback_info_t;
#include "nwdaf_event.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_analytics_feedback_info_s {
    OpenAPI_list_t *action_times;
    OpenAPI_list_t *used_ana_types;
    bool is_impact_ind;
    int impact_ind;
};

OpenAPI_analytics_feedback_info_t *OpenAPI_analytics_feedback_info_create(
    OpenAPI_list_t *action_times,
    OpenAPI_list_t *used_ana_types,
    bool is_impact_ind,
    int impact_ind
);
void OpenAPI_analytics_feedback_info_free(OpenAPI_analytics_feedback_info_t *analytics_feedback_info);
OpenAPI_analytics_feedback_info_t *OpenAPI_analytics_feedback_info_parseFromJSON(cJSON *analytics_feedback_infoJSON);
cJSON *OpenAPI_analytics_feedback_info_convertToJSON(OpenAPI_analytics_feedback_info_t *analytics_feedback_info);
OpenAPI_analytics_feedback_info_t *OpenAPI_analytics_feedback_info_copy(OpenAPI_analytics_feedback_info_t *dst, OpenAPI_analytics_feedback_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_feedback_info_H_ */

