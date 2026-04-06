/*
 * time_sync_data.h
 *
 * Contains Time Synchronization Data
 */

#ifndef _OpenAPI_time_sync_data_H_
#define _OpenAPI_time_sync_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_time_sync_data_s OpenAPI_time_sync_data_t;
#include "clock_quality_acceptance_criterion.h"
#include "clock_quality_detail_level.h"
#include "tai.h"
#include "temporal_validity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_time_sync_data_s {
    int authorized;
    bool is_uu_time_sync_err_bdgt;
    int uu_time_sync_err_bdgt;
    OpenAPI_list_t *temp_vals;
    OpenAPI_list_t *coverage_area;
    OpenAPI_clock_quality_detail_level_e clock_quality_detail_level;
    struct OpenAPI_clock_quality_acceptance_criterion_s *clock_quality_acceptance_criterion;
};

OpenAPI_time_sync_data_t *OpenAPI_time_sync_data_create(
    int authorized,
    bool is_uu_time_sync_err_bdgt,
    int uu_time_sync_err_bdgt,
    OpenAPI_list_t *temp_vals,
    OpenAPI_list_t *coverage_area,
    OpenAPI_clock_quality_detail_level_e clock_quality_detail_level,
    OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criterion
);
void OpenAPI_time_sync_data_free(OpenAPI_time_sync_data_t *time_sync_data);
OpenAPI_time_sync_data_t *OpenAPI_time_sync_data_parseFromJSON(cJSON *time_sync_dataJSON);
cJSON *OpenAPI_time_sync_data_convertToJSON(OpenAPI_time_sync_data_t *time_sync_data);
OpenAPI_time_sync_data_t *OpenAPI_time_sync_data_copy(OpenAPI_time_sync_data_t *dst, OpenAPI_time_sync_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_sync_data_H_ */

