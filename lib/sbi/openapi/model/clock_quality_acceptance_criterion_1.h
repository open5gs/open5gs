/*
 * clock_quality_acceptance_criterion_1.h
 *
 * Contains a Clock Quality Acceptance Criterion.
 */

#ifndef _OpenAPI_clock_quality_acceptance_criterion_1_H_
#define _OpenAPI_clock_quality_acceptance_criterion_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_clock_quality_acceptance_criterion_1_s OpenAPI_clock_quality_acceptance_criterion_1_t;
#include "clock_quality_1.h"
#include "synchronization_state.h"
#include "time_source.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_clock_quality_acceptance_criterion_1_s {
    OpenAPI_list_t *synchronization_state;
    struct OpenAPI_clock_quality_1_s *clock_quality;
    OpenAPI_list_t *parent_time_source;
};

OpenAPI_clock_quality_acceptance_criterion_1_t *OpenAPI_clock_quality_acceptance_criterion_1_create(
    OpenAPI_list_t *synchronization_state,
    OpenAPI_clock_quality_1_t *clock_quality,
    OpenAPI_list_t *parent_time_source
);
void OpenAPI_clock_quality_acceptance_criterion_1_free(OpenAPI_clock_quality_acceptance_criterion_1_t *clock_quality_acceptance_criterion_1);
OpenAPI_clock_quality_acceptance_criterion_1_t *OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON(cJSON *clock_quality_acceptance_criterion_1JSON);
cJSON *OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON(OpenAPI_clock_quality_acceptance_criterion_1_t *clock_quality_acceptance_criterion_1);
OpenAPI_clock_quality_acceptance_criterion_1_t *OpenAPI_clock_quality_acceptance_criterion_1_copy(OpenAPI_clock_quality_acceptance_criterion_1_t *dst, OpenAPI_clock_quality_acceptance_criterion_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_clock_quality_acceptance_criterion_1_H_ */

