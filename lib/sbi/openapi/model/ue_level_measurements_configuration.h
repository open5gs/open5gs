/*
 * ue_level_measurements_configuration.h
 *
 * 5GC UE Level Measurements configuration.
 */

#ifndef _OpenAPI_ue_level_measurements_configuration_H_
#define _OpenAPI_ue_level_measurements_configuration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_level_measurements_configuration_s OpenAPI_ue_level_measurements_configuration_t;
#include "job_type.h"
#include "measurement_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_level_measurements_configuration_s {
    OpenAPI_job_type_e job_type;
    OpenAPI_list_t *ue_level_measurements_list;
    bool is_granularity_period;
    int granularity_period;
};

OpenAPI_ue_level_measurements_configuration_t *OpenAPI_ue_level_measurements_configuration_create(
    OpenAPI_job_type_e job_type,
    OpenAPI_list_t *ue_level_measurements_list,
    bool is_granularity_period,
    int granularity_period
);
void OpenAPI_ue_level_measurements_configuration_free(OpenAPI_ue_level_measurements_configuration_t *ue_level_measurements_configuration);
OpenAPI_ue_level_measurements_configuration_t *OpenAPI_ue_level_measurements_configuration_parseFromJSON(cJSON *ue_level_measurements_configurationJSON);
cJSON *OpenAPI_ue_level_measurements_configuration_convertToJSON(OpenAPI_ue_level_measurements_configuration_t *ue_level_measurements_configuration);
OpenAPI_ue_level_measurements_configuration_t *OpenAPI_ue_level_measurements_configuration_copy(OpenAPI_ue_level_measurements_configuration_t *dst, OpenAPI_ue_level_measurements_configuration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_level_measurements_configuration_H_ */

