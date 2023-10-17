/*
 * network_perf_info.h
 *
 * Represents the network performance information.
 */

#ifndef _OpenAPI_network_perf_info_H_
#define _OpenAPI_network_perf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_area_info.h"
#include "network_perf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_network_perf_info_s OpenAPI_network_perf_info_t;
typedef struct OpenAPI_network_perf_info_s {
    struct OpenAPI_network_area_info_s *network_area;
    struct OpenAPI_network_perf_type_s *nw_perf_type;
    bool is_relative_ratio;
    int relative_ratio;
    bool is_absolute_num;
    int absolute_num;
    bool is_confidence;
    int confidence;
} OpenAPI_network_perf_info_t;

OpenAPI_network_perf_info_t *OpenAPI_network_perf_info_create(
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_network_perf_type_t *nw_perf_type,
    bool is_relative_ratio,
    int relative_ratio,
    bool is_absolute_num,
    int absolute_num,
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

