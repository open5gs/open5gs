/*
 * dispersion_ordering_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_dispersion_ordering_criterion_H_
#define _OpenAPI_dispersion_ordering_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dispersion_ordering_criterion_NULL = 0, OpenAPI_dispersion_ordering_criterion_TIME_SLOT_START, OpenAPI_dispersion_ordering_criterion_DISPERSION, OpenAPI_dispersion_ordering_criterion_CLASSIFICATION, OpenAPI_dispersion_ordering_criterion_RANKING, OpenAPI_dispersion_ordering_criterion_PERCENTILE_RANKING } OpenAPI_dispersion_ordering_criterion_e;

char* OpenAPI_dispersion_ordering_criterion_ToString(OpenAPI_dispersion_ordering_criterion_e dispersion_ordering_criterion);

OpenAPI_dispersion_ordering_criterion_e OpenAPI_dispersion_ordering_criterion_FromString(char* dispersion_ordering_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_ordering_criterion_H_ */

