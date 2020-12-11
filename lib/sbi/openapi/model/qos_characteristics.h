/*
 * qos_characteristics.h
 *
 *
 */

#ifndef _OpenAPI_qos_characteristics_H_
#define _OpenAPI_qos_characteristics_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "qos_resource_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_characteristics_s OpenAPI_qos_characteristics_t;
typedef struct OpenAPI_qos_characteristics_s {
    int _5qi;
    struct OpenAPI_qos_resource_type_s *resource_type;
    int priority_level;
    int packet_delay_budget;
    char *packet_error_rate;
    int averaging_window;
    int max_data_burst_vol;
    int ext_max_data_burst_vol;
} OpenAPI_qos_characteristics_t;

OpenAPI_qos_characteristics_t *OpenAPI_qos_characteristics_create(
    int _5qi,
    OpenAPI_qos_resource_type_t *resource_type,
    int priority_level,
    int packet_delay_budget,
    char *packet_error_rate,
    int averaging_window,
    int max_data_burst_vol,
    int ext_max_data_burst_vol
    );
void OpenAPI_qos_characteristics_free(OpenAPI_qos_characteristics_t *qos_characteristics);
OpenAPI_qos_characteristics_t *OpenAPI_qos_characteristics_parseFromJSON(cJSON *qos_characteristicsJSON);
cJSON *OpenAPI_qos_characteristics_convertToJSON(OpenAPI_qos_characteristics_t *qos_characteristics);
OpenAPI_qos_characteristics_t *OpenAPI_qos_characteristics_copy(OpenAPI_qos_characteristics_t *dst, OpenAPI_qos_characteristics_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_characteristics_H_ */

