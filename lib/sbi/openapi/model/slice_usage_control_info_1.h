/*
 * slice_usage_control_info_1.h
 *
 * The network slice usage control related information
 */

#ifndef _OpenAPI_slice_usage_control_info_1_H_
#define _OpenAPI_slice_usage_control_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_slice_usage_control_info_1_s OpenAPI_slice_usage_control_info_1_t;
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_slice_usage_control_info_1_s {
    struct OpenAPI_snssai_s *s_nssai;
    bool is_dereg_inact_timer;
    int dereg_inact_timer;
    bool is_sess_inact_timer;
    int sess_inact_timer;
};

OpenAPI_slice_usage_control_info_1_t *OpenAPI_slice_usage_control_info_1_create(
    OpenAPI_snssai_t *s_nssai,
    bool is_dereg_inact_timer,
    int dereg_inact_timer,
    bool is_sess_inact_timer,
    int sess_inact_timer
);
void OpenAPI_slice_usage_control_info_1_free(OpenAPI_slice_usage_control_info_1_t *slice_usage_control_info_1);
OpenAPI_slice_usage_control_info_1_t *OpenAPI_slice_usage_control_info_1_parseFromJSON(cJSON *slice_usage_control_info_1JSON);
cJSON *OpenAPI_slice_usage_control_info_1_convertToJSON(OpenAPI_slice_usage_control_info_1_t *slice_usage_control_info_1);
OpenAPI_slice_usage_control_info_1_t *OpenAPI_slice_usage_control_info_1_copy(OpenAPI_slice_usage_control_info_1_t *dst, OpenAPI_slice_usage_control_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_slice_usage_control_info_1_H_ */

