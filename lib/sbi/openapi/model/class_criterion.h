/*
 * class_criterion.h
 *
 * Indicates the dispersion class criterion for fixed, camper and/or traveller UE, and/or the  top-heavy UE dispersion class criterion. 
 */

#ifndef _OpenAPI_class_criterion_H_
#define _OpenAPI_class_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dispersion_class.h"
#include "matching_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_class_criterion_s OpenAPI_class_criterion_t;
typedef struct OpenAPI_class_criterion_s {
    struct OpenAPI_dispersion_class_s *disper_class;
    int class_threshold;
    struct OpenAPI_matching_direction_s *thres_match;
} OpenAPI_class_criterion_t;

OpenAPI_class_criterion_t *OpenAPI_class_criterion_create(
    OpenAPI_dispersion_class_t *disper_class,
    int class_threshold,
    OpenAPI_matching_direction_t *thres_match
);
void OpenAPI_class_criterion_free(OpenAPI_class_criterion_t *class_criterion);
OpenAPI_class_criterion_t *OpenAPI_class_criterion_parseFromJSON(cJSON *class_criterionJSON);
cJSON *OpenAPI_class_criterion_convertToJSON(OpenAPI_class_criterion_t *class_criterion);
OpenAPI_class_criterion_t *OpenAPI_class_criterion_copy(OpenAPI_class_criterion_t *dst, OpenAPI_class_criterion_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_class_criterion_H_ */

