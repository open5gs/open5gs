/*
 * network_perf_info.h
 *
 * 
 */

#ifndef _OpenAPI_network_perf_info_H_
#define _OpenAPI_network_perf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_network_perf_info_s OpenAPI_network_perf_info_t;
#include "network_area_info.h"
#include "network_perf_type.h"
#include "resource_usage_requirement.h"
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_network_perf_info_s {
    struct OpenAPI_network_area_info_s *network_area;
    OpenAPI_network_perf_type_e nw_perf_type;
    struct OpenAPI_time_window_s *ana_period;
    bool is_relative_ratio;
    int relative_ratio;
    bool is_absolute_num;
    int absolute_num;
    struct OpenAPI_resource_usage_requirement_s *rsc_usg_req;
    bool is_confidence;
    int confidence;
};

OpenAPI_network_perf_info_t *OpenAPI_network_perf_info_create(
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_network_perf_type_e nw_perf_type,
    OpenAPI_time_window_t *ana_period,
    bool is_relative_ratio,
    int relative_ratio,
    bool is_absolute_num,
    int absolute_num,
    OpenAPI_resource_usage_requirement_t *rsc_usg_req,
    bool is_confidence,
    int confidence
);
void OpenAPI_network_perf_info_free(OpenAPI_network_perf_info_t *network_perf_info);
OpenAPI_network_perf_info_t *OpenAPI_network_perf_info_parseFromJSON(cJSON *network_perf_infoJSON);
cJSON *OpenAPI_network_perf_info_convertToJSON(OpenAPI_network_perf_info_t *network_perf_info);
OpenAPI_network_perf_info_t *OpenAPI_network_perf_info_copy(OpenAPI_network_perf_info_t *dst, OpenAPI_network_perf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_perf_info_H_ */

