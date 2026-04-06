/*
 * clock_quality_acceptance_criterion_rm.h
 *
 * Contains a Clock Quality Acceptance Criterion.
 */

#ifndef _OpenAPI_clock_quality_acceptance_criterion_rm_H_
#define _OpenAPI_clock_quality_acceptance_criterion_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_clock_quality_acceptance_criterion_rm_s OpenAPI_clock_quality_acceptance_criterion_rm_t;
#include "clock_quality_rm.h"
#include "synchronization_state.h"
#include "time_source.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_clock_quality_acceptance_criterion_rm_s {
    bool is_synchronization_state_null;
    OpenAPI_list_t *synchronization_state;
    bool is_clock_quality_null;
    struct OpenAPI_clock_quality_rm_s *clock_quality;
    bool is_parent_time_source_null;
    OpenAPI_list_t *parent_time_source;
};

OpenAPI_clock_quality_acceptance_criterion_rm_t *OpenAPI_clock_quality_acceptance_criterion_rm_create(
    bool is_synchronization_state_null,
    OpenAPI_list_t *synchronization_state,
    bool is_clock_quality_null,
    OpenAPI_clock_quality_rm_t *clock_quality,
    bool is_parent_time_source_null,
    OpenAPI_list_t *parent_time_source
);
void OpenAPI_clock_quality_acceptance_criterion_rm_free(OpenAPI_clock_quality_acceptance_criterion_rm_t *clock_quality_acceptance_criterion_rm);
OpenAPI_clock_quality_acceptance_criterion_rm_t *OpenAPI_clock_quality_acceptance_criterion_rm_parseFromJSON(cJSON *clock_quality_acceptance_criterion_rmJSON);
cJSON *OpenAPI_clock_quality_acceptance_criterion_rm_convertToJSON(OpenAPI_clock_quality_acceptance_criterion_rm_t *clock_quality_acceptance_criterion_rm);
OpenAPI_clock_quality_acceptance_criterion_rm_t *OpenAPI_clock_quality_acceptance_criterion_rm_copy(OpenAPI_clock_quality_acceptance_criterion_rm_t *dst, OpenAPI_clock_quality_acceptance_criterion_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_clock_quality_acceptance_criterion_rm_H_ */

