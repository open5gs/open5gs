/*
 * location_qo_s.h
 *
 * QoS of Location request.
 */

#ifndef _OpenAPI_location_qo_s_H_
#define _OpenAPI_location_qo_s_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "lcs_qos_class.h"
#include "minor_location_qo_s.h"
#include "response_time.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_location_qo_s_s OpenAPI_location_qo_s_t;
typedef struct OpenAPI_location_qo_s_s {
    bool is_h_accuracy;
    float h_accuracy;
    bool is_v_accuracy;
    float v_accuracy;
    bool is_vertical_requested;
    int vertical_requested;
    struct OpenAPI_response_time_s *response_time;
    OpenAPI_list_t *minor_loc_qoses;
    struct OpenAPI_lcs_qos_class_s *lcs_qos_class;
} OpenAPI_location_qo_s_t;

OpenAPI_location_qo_s_t *OpenAPI_location_qo_s_create(
    bool is_h_accuracy,
    float h_accuracy,
    bool is_v_accuracy,
    float v_accuracy,
    bool is_vertical_requested,
    int vertical_requested,
    OpenAPI_response_time_t *response_time,
    OpenAPI_list_t *minor_loc_qoses,
    OpenAPI_lcs_qos_class_t *lcs_qos_class
);
void OpenAPI_location_qo_s_free(OpenAPI_location_qo_s_t *location_qo_s);
OpenAPI_location_qo_s_t *OpenAPI_location_qo_s_parseFromJSON(cJSON *location_qo_sJSON);
cJSON *OpenAPI_location_qo_s_convertToJSON(OpenAPI_location_qo_s_t *location_qo_s);
OpenAPI_location_qo_s_t *OpenAPI_location_qo_s_copy(OpenAPI_location_qo_s_t *dst, OpenAPI_location_qo_s_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_qo_s_H_ */

