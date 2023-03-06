/*
 * dispersion_ordering_criterion.h
 *
 * Possible values are: - TIME_SLOT_START: Indicates the order of time slot start. - DISPERSION: Indicates the order of data/transaction dispersion. - CLASSIFICATION: Indicates the order of data/transaction classification. - RANKING: Indicates the order of data/transaction ranking. - PERCENTILE_RANKING: Indicates the order of data/transaction percentile ranking. 
 */

#ifndef _OpenAPI_dispersion_ordering_criterion_H_
#define _OpenAPI_dispersion_ordering_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dispersion_ordering_criterion_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dispersion_ordering_criterion_s OpenAPI_dispersion_ordering_criterion_t;
typedef struct OpenAPI_dispersion_ordering_criterion_s {
} OpenAPI_dispersion_ordering_criterion_t;

OpenAPI_dispersion_ordering_criterion_t *OpenAPI_dispersion_ordering_criterion_create(
);
void OpenAPI_dispersion_ordering_criterion_free(OpenAPI_dispersion_ordering_criterion_t *dispersion_ordering_criterion);
OpenAPI_dispersion_ordering_criterion_t *OpenAPI_dispersion_ordering_criterion_parseFromJSON(cJSON *dispersion_ordering_criterionJSON);
cJSON *OpenAPI_dispersion_ordering_criterion_convertToJSON(OpenAPI_dispersion_ordering_criterion_t *dispersion_ordering_criterion);
OpenAPI_dispersion_ordering_criterion_t *OpenAPI_dispersion_ordering_criterion_copy(OpenAPI_dispersion_ordering_criterion_t *dst, OpenAPI_dispersion_ordering_criterion_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_ordering_criterion_H_ */

