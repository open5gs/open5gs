/*
 * threshold_value.h
 *
 * Indicates the threshold value(s) for RTT and/or Packet Loss Rate.
 */

#ifndef _OpenAPI_threshold_value_H_
#define _OpenAPI_threshold_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_threshold_value_s OpenAPI_threshold_value_t;
typedef struct OpenAPI_threshold_value_s {
    bool is_rtt_thres_null;
    bool is_rtt_thres;
    int rtt_thres;
    bool is_plr_thres_null;
    bool is_plr_thres;
    int plr_thres;
} OpenAPI_threshold_value_t;

OpenAPI_threshold_value_t *OpenAPI_threshold_value_create(
    bool is_rtt_thres_null,
    bool is_rtt_thres,
    int rtt_thres,
    bool is_plr_thres_null,
    bool is_plr_thres,
    int plr_thres
);
void OpenAPI_threshold_value_free(OpenAPI_threshold_value_t *threshold_value);
OpenAPI_threshold_value_t *OpenAPI_threshold_value_parseFromJSON(cJSON *threshold_valueJSON);
cJSON *OpenAPI_threshold_value_convertToJSON(OpenAPI_threshold_value_t *threshold_value);
OpenAPI_threshold_value_t *OpenAPI_threshold_value_copy(OpenAPI_threshold_value_t *dst, OpenAPI_threshold_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_threshold_value_H_ */

