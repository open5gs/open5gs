/*
 * as_time_distribution_param.h
 *
 * Contains the 5G acess stratum time distribution parameters.
 */

#ifndef _OpenAPI_as_time_distribution_param_H_
#define _OpenAPI_as_time_distribution_param_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_as_time_distribution_param_s OpenAPI_as_time_distribution_param_t;
#include "clock_quality_acceptance_criterion_rm.h"
#include "clock_quality_detail_level.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_as_time_distribution_param_s {
    bool is_as_time_dist_ind;
    int as_time_dist_ind;
    bool is_uu_error_budget_null;
    bool is_uu_error_budget;
    int uu_error_budget;
    OpenAPI_clock_quality_detail_level_e clk_qlt_det_lvl;
    bool is_clk_qlt_acpt_cri_null;
    struct OpenAPI_clock_quality_acceptance_criterion_rm_s *clk_qlt_acpt_cri;
};

OpenAPI_as_time_distribution_param_t *OpenAPI_as_time_distribution_param_create(
    bool is_as_time_dist_ind,
    int as_time_dist_ind,
    bool is_uu_error_budget_null,
    bool is_uu_error_budget,
    int uu_error_budget,
    OpenAPI_clock_quality_detail_level_e clk_qlt_det_lvl,
    bool is_clk_qlt_acpt_cri_null,
    OpenAPI_clock_quality_acceptance_criterion_rm_t *clk_qlt_acpt_cri
);
void OpenAPI_as_time_distribution_param_free(OpenAPI_as_time_distribution_param_t *as_time_distribution_param);
OpenAPI_as_time_distribution_param_t *OpenAPI_as_time_distribution_param_parseFromJSON(cJSON *as_time_distribution_paramJSON);
cJSON *OpenAPI_as_time_distribution_param_convertToJSON(OpenAPI_as_time_distribution_param_t *as_time_distribution_param);
OpenAPI_as_time_distribution_param_t *OpenAPI_as_time_distribution_param_copy(OpenAPI_as_time_distribution_param_t *dst, OpenAPI_as_time_distribution_param_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_as_time_distribution_param_H_ */

