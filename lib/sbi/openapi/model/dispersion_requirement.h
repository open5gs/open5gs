/*
 * dispersion_requirement.h
 *
 * Represents the dispersion analytics requirements.
 */

#ifndef _OpenAPI_dispersion_requirement_H_
#define _OpenAPI_dispersion_requirement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "class_criterion.h"
#include "dispersion_ordering_criterion.h"
#include "dispersion_type.h"
#include "matching_direction.h"
#include "ranking_criterion.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dispersion_requirement_s OpenAPI_dispersion_requirement_t;
typedef struct OpenAPI_dispersion_requirement_s {
    struct OpenAPI_dispersion_type_s *disper_type;
    OpenAPI_list_t *class_criters;
    OpenAPI_list_t *rank_criters;
    struct OpenAPI_dispersion_ordering_criterion_s *disp_order_criter;
    struct OpenAPI_matching_direction_s *order;
} OpenAPI_dispersion_requirement_t;

OpenAPI_dispersion_requirement_t *OpenAPI_dispersion_requirement_create(
    OpenAPI_dispersion_type_t *disper_type,
    OpenAPI_list_t *class_criters,
    OpenAPI_list_t *rank_criters,
    OpenAPI_dispersion_ordering_criterion_t *disp_order_criter,
    OpenAPI_matching_direction_t *order
);
void OpenAPI_dispersion_requirement_free(OpenAPI_dispersion_requirement_t *dispersion_requirement);
OpenAPI_dispersion_requirement_t *OpenAPI_dispersion_requirement_parseFromJSON(cJSON *dispersion_requirementJSON);
cJSON *OpenAPI_dispersion_requirement_convertToJSON(OpenAPI_dispersion_requirement_t *dispersion_requirement);
OpenAPI_dispersion_requirement_t *OpenAPI_dispersion_requirement_copy(OpenAPI_dispersion_requirement_t *dst, OpenAPI_dispersion_requirement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_requirement_H_ */

