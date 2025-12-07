/*
 * minor_location_qo_s.h
 *
 * Contain Minor Location QoS.
 */

#ifndef _OpenAPI_minor_location_qo_s_H_
#define _OpenAPI_minor_location_qo_s_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_minor_location_qo_s_s OpenAPI_minor_location_qo_s_t;
typedef struct OpenAPI_minor_location_qo_s_s {
    bool is_h_accuracy;
    float h_accuracy;
    bool is_v_accuracy;
    float v_accuracy;
} OpenAPI_minor_location_qo_s_t;

OpenAPI_minor_location_qo_s_t *OpenAPI_minor_location_qo_s_create(
    bool is_h_accuracy,
    float h_accuracy,
    bool is_v_accuracy,
    float v_accuracy
);
void OpenAPI_minor_location_qo_s_free(OpenAPI_minor_location_qo_s_t *minor_location_qo_s);
OpenAPI_minor_location_qo_s_t *OpenAPI_minor_location_qo_s_parseFromJSON(cJSON *minor_location_qo_sJSON);
cJSON *OpenAPI_minor_location_qo_s_convertToJSON(OpenAPI_minor_location_qo_s_t *minor_location_qo_s);
OpenAPI_minor_location_qo_s_t *OpenAPI_minor_location_qo_s_copy(OpenAPI_minor_location_qo_s_t *dst, OpenAPI_minor_location_qo_s_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_minor_location_qo_s_H_ */

