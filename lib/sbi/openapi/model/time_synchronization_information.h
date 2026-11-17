/*
 * time_synchronization_information.h
 *
 * 
 */

#ifndef _OpenAPI_time_synchronization_information_H_
#define _OpenAPI_time_synchronization_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_time_synchronization_information_s OpenAPI_time_synchronization_information_t;
#include "clock_quality.h"
#include "synchronization_state.h"
#include "tai.h"
#include "time_distribution_method.h"
#include "time_source.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_time_synchronization_information_s {
    OpenAPI_time_distribution_method_e distribution_method;
    bool is_t_s_ntime_domain_number;
    int t_s_ntime_domain_number;
    bool is_temporal_validity_information;
    int temporal_validity_information;
    OpenAPI_list_t *spatial_validity_information;
    bool is_time_synchronization_error_budget;
    int time_synchronization_error_budget;
    OpenAPI_synchronization_state_e synchronization_state;
    struct OpenAPI_clock_quality_s *clock_quality;
    OpenAPI_time_source_e parent_time_source;
};

OpenAPI_time_synchronization_information_t *OpenAPI_time_synchronization_information_create(
    OpenAPI_time_distribution_method_e distribution_method,
    bool is_t_s_ntime_domain_number,
    int t_s_ntime_domain_number,
    bool is_temporal_validity_information,
    int temporal_validity_information,
    OpenAPI_list_t *spatial_validity_information,
    bool is_time_synchronization_error_budget,
    int time_synchronization_error_budget,
    OpenAPI_synchronization_state_e synchronization_state,
    OpenAPI_clock_quality_t *clock_quality,
    OpenAPI_time_source_e parent_time_source
);
void OpenAPI_time_synchronization_information_free(OpenAPI_time_synchronization_information_t *time_synchronization_information);
OpenAPI_time_synchronization_information_t *OpenAPI_time_synchronization_information_parseFromJSON(cJSON *time_synchronization_informationJSON);
cJSON *OpenAPI_time_synchronization_information_convertToJSON(OpenAPI_time_synchronization_information_t *time_synchronization_information);
OpenAPI_time_synchronization_information_t *OpenAPI_time_synchronization_information_copy(OpenAPI_time_synchronization_information_t *dst, OpenAPI_time_synchronization_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_synchronization_information_H_ */

