/*
 * dispersion_ordering_criterion_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_dispersion_ordering_criterion_any_of_H_
#define _OpenAPI_dispersion_ordering_criterion_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dispersion_ordering_criterion_any_of_NULL = 0, OpenAPI_dispersion_ordering_criterion_any_of_TIME_SLOT_START, OpenAPI_dispersion_ordering_criterion_any_of_DISPERSION, OpenAPI_dispersion_ordering_criterion_any_of_CLASSIFICATION, OpenAPI_dispersion_ordering_criterion_any_of_RANKING, OpenAPI_dispersion_ordering_criterion_any_of_PERCENTILE_RANKING } OpenAPI_dispersion_ordering_criterion_any_of_e;

char* OpenAPI_dispersion_ordering_criterion_any_of_ToString(OpenAPI_dispersion_ordering_criterion_any_of_e dispersion_ordering_criterion_any_of);

OpenAPI_dispersion_ordering_criterion_any_of_e OpenAPI_dispersion_ordering_criterion_any_of_FromString(char* dispersion_ordering_criterion_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_ordering_criterion_any_of_H_ */

