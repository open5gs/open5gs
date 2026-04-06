/*
 * asti_allowed_info.h
 *
 * AF authorization information for ASTI
 */

#ifndef _OpenAPI_asti_allowed_info_H_
#define _OpenAPI_asti_allowed_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_asti_allowed_info_s OpenAPI_asti_allowed_info_t;
#include "clock_quality_acceptance_criterion.h"
#include "tai.h"
#include "temporal_validity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_asti_allowed_info_s {
    int asti_allowed;
    OpenAPI_list_t *coverage_area;
    bool is_uu_time_sync_err_bdgt;
    int uu_time_sync_err_bdgt;
    OpenAPI_list_t *temp_vals;
    bool is_clock_quality_metrics_ind;
    int clock_quality_metrics_ind;
    bool is_acceptable_ind;
    int acceptable_ind;
    struct OpenAPI_clock_quality_acceptance_criterion_s *clock_quality_acceptance_criteria;
};

OpenAPI_asti_allowed_info_t *OpenAPI_asti_allowed_info_create(
    int asti_allowed,
    OpenAPI_list_t *coverage_area,
    bool is_uu_time_sync_err_bdgt,
    int uu_time_sync_err_bdgt,
    OpenAPI_list_t *temp_vals,
    bool is_clock_quality_metrics_ind,
    int clock_quality_metrics_ind,
    bool is_acceptable_ind,
    int acceptable_ind,
    OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criteria
);
void OpenAPI_asti_allowed_info_free(OpenAPI_asti_allowed_info_t *asti_allowed_info);
OpenAPI_asti_allowed_info_t *OpenAPI_asti_allowed_info_parseFromJSON(cJSON *asti_allowed_infoJSON);
cJSON *OpenAPI_asti_allowed_info_convertToJSON(OpenAPI_asti_allowed_info_t *asti_allowed_info);
OpenAPI_asti_allowed_info_t *OpenAPI_asti_allowed_info_copy(OpenAPI_asti_allowed_info_t *dst, OpenAPI_asti_allowed_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_asti_allowed_info_H_ */

