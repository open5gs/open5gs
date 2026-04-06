/*
 * clock_quality_acceptance_criterion.h
 *
 * Contains a Clock Quality Acceptance Criterion.
 */

#ifndef _OpenAPI_clock_quality_acceptance_criterion_H_
#define _OpenAPI_clock_quality_acceptance_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_clock_quality_acceptance_criterion_s OpenAPI_clock_quality_acceptance_criterion_t;
#include "clock_quality.h"
#include "synchronization_state.h"
#include "time_source.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_clock_quality_acceptance_criterion_s {
    OpenAPI_list_t *synchronization_state;
    struct OpenAPI_clock_quality_s *clock_quality;
    OpenAPI_list_t *parent_time_source;
};

OpenAPI_clock_quality_acceptance_criterion_t *OpenAPI_clock_quality_acceptance_criterion_create(
    OpenAPI_list_t *synchronization_state,
    OpenAPI_clock_quality_t *clock_quality,
    OpenAPI_list_t *parent_time_source
);
void OpenAPI_clock_quality_acceptance_criterion_free(OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criterion);
OpenAPI_clock_quality_acceptance_criterion_t *OpenAPI_clock_quality_acceptance_criterion_parseFromJSON(cJSON *clock_quality_acceptance_criterionJSON);
cJSON *OpenAPI_clock_quality_acceptance_criterion_convertToJSON(OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criterion);
OpenAPI_clock_quality_acceptance_criterion_t *OpenAPI_clock_quality_acceptance_criterion_copy(OpenAPI_clock_quality_acceptance_criterion_t *dst, OpenAPI_clock_quality_acceptance_criterion_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_clock_quality_acceptance_criterion_H_ */

