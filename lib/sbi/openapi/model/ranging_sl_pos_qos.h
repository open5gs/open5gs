/*
 * ranging_sl_pos_qos.h
 *
 * Contains Ranging and Sidelink Positioning QoS
 */

#ifndef _OpenAPI_ranging_sl_pos_qos_H_
#define _OpenAPI_ranging_sl_pos_qos_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_pos_qos_s OpenAPI_ranging_sl_pos_qos_t;
#include "lcs_qos_class.h"
#include "response_time.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_pos_qos_s {
    bool is_h_accuracy;
    float h_accuracy;
    bool is_v_accuracy;
    float v_accuracy;
    bool is_relative_h_accuracy;
    float relative_h_accuracy;
    bool is_relative_v_accuracy;
    float relative_v_accuracy;
    bool is_distance_accuracy;
    float distance_accuracy;
    bool is_direction_accuracy;
    float direction_accuracy;
    bool is_vertical_requested;
    int vertical_requested;
    OpenAPI_response_time_e response_time;
    OpenAPI_lcs_qos_class_e lcs_qos_class;
    bool is_ranging_sl_pos_priority_level;
    int ranging_sl_pos_priority_level;
    bool is_ranging_sl_pos_delay_budget;
    int ranging_sl_pos_delay_budget;
};

OpenAPI_ranging_sl_pos_qos_t *OpenAPI_ranging_sl_pos_qos_create(
    bool is_h_accuracy,
    float h_accuracy,
    bool is_v_accuracy,
    float v_accuracy,
    bool is_relative_h_accuracy,
    float relative_h_accuracy,
    bool is_relative_v_accuracy,
    float relative_v_accuracy,
    bool is_distance_accuracy,
    float distance_accuracy,
    bool is_direction_accuracy,
    float direction_accuracy,
    bool is_vertical_requested,
    int vertical_requested,
    OpenAPI_response_time_e response_time,
    OpenAPI_lcs_qos_class_e lcs_qos_class,
    bool is_ranging_sl_pos_priority_level,
    int ranging_sl_pos_priority_level,
    bool is_ranging_sl_pos_delay_budget,
    int ranging_sl_pos_delay_budget
);
void OpenAPI_ranging_sl_pos_qos_free(OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos);
OpenAPI_ranging_sl_pos_qos_t *OpenAPI_ranging_sl_pos_qos_parseFromJSON(cJSON *ranging_sl_pos_qosJSON);
cJSON *OpenAPI_ranging_sl_pos_qos_convertToJSON(OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos);
OpenAPI_ranging_sl_pos_qos_t *OpenAPI_ranging_sl_pos_qos_copy(OpenAPI_ranging_sl_pos_qos_t *dst, OpenAPI_ranging_sl_pos_qos_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_pos_qos_H_ */

