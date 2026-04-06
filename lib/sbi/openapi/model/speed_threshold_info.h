/*
 * speed_threshold_info.h
 *
 * UEs information whose speed is faster than the speed threshold.
 */

#ifndef _OpenAPI_speed_threshold_info_H_
#define _OpenAPI_speed_threshold_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_speed_threshold_info_s OpenAPI_speed_threshold_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_speed_threshold_info_s {
    bool is_speed_thr;
    float speed_thr;
    bool is_num_of_ue;
    int num_of_ue;
    bool is_ratio;
    int ratio;
};

OpenAPI_speed_threshold_info_t *OpenAPI_speed_threshold_info_create(
    bool is_speed_thr,
    float speed_thr,
    bool is_num_of_ue,
    int num_of_ue,
    bool is_ratio,
    int ratio
);
void OpenAPI_speed_threshold_info_free(OpenAPI_speed_threshold_info_t *speed_threshold_info);
OpenAPI_speed_threshold_info_t *OpenAPI_speed_threshold_info_parseFromJSON(cJSON *speed_threshold_infoJSON);
cJSON *OpenAPI_speed_threshold_info_convertToJSON(OpenAPI_speed_threshold_info_t *speed_threshold_info);
OpenAPI_speed_threshold_info_t *OpenAPI_speed_threshold_info_copy(OpenAPI_speed_threshold_info_t *dst, OpenAPI_speed_threshold_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_speed_threshold_info_H_ */

