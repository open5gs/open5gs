/*
 * ue_communication.h
 *
 * Represents UE communication information.
 */

#ifndef _OpenAPI_ue_communication_H_
#define _OpenAPI_ue_communication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "app_list_for_ue_comm.h"
#include "scheduled_communication_time_1.h"
#include "sess_inact_timer_for_ue_comm.h"
#include "traffic_characterization.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_communication_s OpenAPI_ue_communication_t;
typedef struct OpenAPI_ue_communication_s {
    bool is_comm_dur;
    int comm_dur;
    bool is_comm_dur_variance;
    float comm_dur_variance;
    bool is_perio_time;
    int perio_time;
    bool is_perio_time_variance;
    float perio_time_variance;
    char *ts;
    bool is_ts_variance;
    float ts_variance;
    struct OpenAPI_scheduled_communication_time_1_s *recurring_time;
    struct OpenAPI_traffic_characterization_s *traf_char;
    bool is_ratio;
    int ratio;
    bool is_perio_comm_ind;
    int perio_comm_ind;
    bool is_confidence;
    int confidence;
    struct OpenAPI_app_list_for_ue_comm_s *ana_of_app_list;
    struct OpenAPI_sess_inact_timer_for_ue_comm_s *sess_inact_timer;
} OpenAPI_ue_communication_t;

OpenAPI_ue_communication_t *OpenAPI_ue_communication_create(
    bool is_comm_dur,
    int comm_dur,
    bool is_comm_dur_variance,
    float comm_dur_variance,
    bool is_perio_time,
    int perio_time,
    bool is_perio_time_variance,
    float perio_time_variance,
    char *ts,
    bool is_ts_variance,
    float ts_variance,
    OpenAPI_scheduled_communication_time_1_t *recurring_time,
    OpenAPI_traffic_characterization_t *traf_char,
    bool is_ratio,
    int ratio,
    bool is_perio_comm_ind,
    int perio_comm_ind,
    bool is_confidence,
    int confidence,
    OpenAPI_app_list_for_ue_comm_t *ana_of_app_list,
    OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer
);
void OpenAPI_ue_communication_free(OpenAPI_ue_communication_t *ue_communication);
OpenAPI_ue_communication_t *OpenAPI_ue_communication_parseFromJSON(cJSON *ue_communicationJSON);
cJSON *OpenAPI_ue_communication_convertToJSON(OpenAPI_ue_communication_t *ue_communication);
OpenAPI_ue_communication_t *OpenAPI_ue_communication_copy(OpenAPI_ue_communication_t *dst, OpenAPI_ue_communication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_communication_H_ */

