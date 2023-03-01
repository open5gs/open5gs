/*
 * abnormal_behaviour.h
 *
 * Represents the abnormal behaviour information.
 */

#ifndef _OpenAPI_abnormal_behaviour_H_
#define _OpenAPI_abnormal_behaviour_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "additional_measurement.h"
#include "exception.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_abnormal_behaviour_s OpenAPI_abnormal_behaviour_t;
typedef struct OpenAPI_abnormal_behaviour_s {
    OpenAPI_list_t *supis;
    struct OpenAPI_exception_s *excep;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    bool is_ratio;
    int ratio;
    bool is_confidence;
    int confidence;
    struct OpenAPI_additional_measurement_s *addt_meas_info;
} OpenAPI_abnormal_behaviour_t;

OpenAPI_abnormal_behaviour_t *OpenAPI_abnormal_behaviour_create(
    OpenAPI_list_t *supis,
    OpenAPI_exception_t *excep,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    bool is_ratio,
    int ratio,
    bool is_confidence,
    int confidence,
    OpenAPI_additional_measurement_t *addt_meas_info
);
void OpenAPI_abnormal_behaviour_free(OpenAPI_abnormal_behaviour_t *abnormal_behaviour);
OpenAPI_abnormal_behaviour_t *OpenAPI_abnormal_behaviour_parseFromJSON(cJSON *abnormal_behaviourJSON);
cJSON *OpenAPI_abnormal_behaviour_convertToJSON(OpenAPI_abnormal_behaviour_t *abnormal_behaviour);
OpenAPI_abnormal_behaviour_t *OpenAPI_abnormal_behaviour_copy(OpenAPI_abnormal_behaviour_t *dst, OpenAPI_abnormal_behaviour_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_abnormal_behaviour_H_ */

