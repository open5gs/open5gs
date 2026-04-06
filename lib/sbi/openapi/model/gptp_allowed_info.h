/*
 * gptp_allowed_info.h
 *
 * AF authorization information for gPTP
 */

#ifndef _OpenAPI_gptp_allowed_info_H_
#define _OpenAPI_gptp_allowed_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_gptp_allowed_info_s OpenAPI_gptp_allowed_info_t;
#include "clock_quality_acceptance_criterion.h"
#include "snssai.h"
#include "tai.h"
#include "temporal_validity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_gptp_allowed_info_s {
    char *dnn;
    struct OpenAPI_snssai_s *s_nssai;
    int gptp_allowed;
    OpenAPI_list_t *coverage_area;
    bool is_uu_time_sync_err_bdgt;
    int uu_time_sync_err_bdgt;
    OpenAPI_list_t *temp_vals;
    bool is_acceptable_ind;
    int acceptable_ind;
    struct OpenAPI_clock_quality_acceptance_criterion_s *clock_quality_acceptance_criteria;
};

OpenAPI_gptp_allowed_info_t *OpenAPI_gptp_allowed_info_create(
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    int gptp_allowed,
    OpenAPI_list_t *coverage_area,
    bool is_uu_time_sync_err_bdgt,
    int uu_time_sync_err_bdgt,
    OpenAPI_list_t *temp_vals,
    bool is_acceptable_ind,
    int acceptable_ind,
    OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criteria
);
void OpenAPI_gptp_allowed_info_free(OpenAPI_gptp_allowed_info_t *gptp_allowed_info);
OpenAPI_gptp_allowed_info_t *OpenAPI_gptp_allowed_info_parseFromJSON(cJSON *gptp_allowed_infoJSON);
cJSON *OpenAPI_gptp_allowed_info_convertToJSON(OpenAPI_gptp_allowed_info_t *gptp_allowed_info);
OpenAPI_gptp_allowed_info_t *OpenAPI_gptp_allowed_info_copy(OpenAPI_gptp_allowed_info_t *dst, OpenAPI_gptp_allowed_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gptp_allowed_info_H_ */

