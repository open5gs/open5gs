/*
 * qos_policy_assist_sets_per_ts.h
 *
 * The QoS and policy assistance parameter sets per Time Slot.
 */

#ifndef _OpenAPI_qos_policy_assist_sets_per_ts_H_
#define _OpenAPI_qos_policy_assist_sets_per_ts_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_policy_assist_sets_per_ts_s OpenAPI_qos_policy_assist_sets_per_ts_t;
#include "qos_policy_assist_set.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_policy_assist_sets_per_ts_s {
    char *ts_start;
    int ts_duration;
    OpenAPI_list_t *qos_pol_assist_sets;
    bool is_confidence;
    int confidence;
};

OpenAPI_qos_policy_assist_sets_per_ts_t *OpenAPI_qos_policy_assist_sets_per_ts_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_list_t *qos_pol_assist_sets,
    bool is_confidence,
    int confidence
);
void OpenAPI_qos_policy_assist_sets_per_ts_free(OpenAPI_qos_policy_assist_sets_per_ts_t *qos_policy_assist_sets_per_ts);
OpenAPI_qos_policy_assist_sets_per_ts_t *OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON(cJSON *qos_policy_assist_sets_per_tsJSON);
cJSON *OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON(OpenAPI_qos_policy_assist_sets_per_ts_t *qos_policy_assist_sets_per_ts);
OpenAPI_qos_policy_assist_sets_per_ts_t *OpenAPI_qos_policy_assist_sets_per_ts_copy(OpenAPI_qos_policy_assist_sets_per_ts_t *dst, OpenAPI_qos_policy_assist_sets_per_ts_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_policy_assist_sets_per_ts_H_ */

