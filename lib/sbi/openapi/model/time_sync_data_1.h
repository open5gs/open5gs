/*
 * time_sync_data_1.h
 *
 * Contains Time Synchronization Data
 */

#ifndef _OpenAPI_time_sync_data_1_H_
#define _OpenAPI_time_sync_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_time_sync_data_1_s OpenAPI_time_sync_data_1_t;
#include "clock_quality_acceptance_criterion_1.h"
#include "clock_quality_detail_level.h"
#include "tai_1.h"
#include "temporal_validity_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_time_sync_data_1_s {
    int authorized;
    bool is_uu_time_sync_err_bdgt;
    int uu_time_sync_err_bdgt;
    OpenAPI_list_t *temp_vals;
    OpenAPI_list_t *coverage_area;
    OpenAPI_clock_quality_detail_level_e clock_quality_detail_level;
    struct OpenAPI_clock_quality_acceptance_criterion_1_s *clock_quality_acceptance_criterion;
};

OpenAPI_time_sync_data_1_t *OpenAPI_time_sync_data_1_create(
    int authorized,
    bool is_uu_time_sync_err_bdgt,
    int uu_time_sync_err_bdgt,
    OpenAPI_list_t *temp_vals,
    OpenAPI_list_t *coverage_area,
    OpenAPI_clock_quality_detail_level_e clock_quality_detail_level,
    OpenAPI_clock_quality_acceptance_criterion_1_t *clock_quality_acceptance_criterion
);
void OpenAPI_time_sync_data_1_free(OpenAPI_time_sync_data_1_t *time_sync_data_1);
OpenAPI_time_sync_data_1_t *OpenAPI_time_sync_data_1_parseFromJSON(cJSON *time_sync_data_1JSON);
cJSON *OpenAPI_time_sync_data_1_convertToJSON(OpenAPI_time_sync_data_1_t *time_sync_data_1);
OpenAPI_time_sync_data_1_t *OpenAPI_time_sync_data_1_copy(OpenAPI_time_sync_data_1_t *dst, OpenAPI_time_sync_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_sync_data_1_H_ */

