/*
 * red_trans_exp_ordering_criterion.h
 *
 * Possible values are: - TIME_SLOT_START: Indicates the order of time slot start. - RED_TRANS_EXP: Indicates the order of Redundant Transmission Experience. 
 */

#ifndef _OpenAPI_red_trans_exp_ordering_criterion_H_
#define _OpenAPI_red_trans_exp_ordering_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "red_trans_exp_ordering_criterion_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_red_trans_exp_ordering_criterion_s OpenAPI_red_trans_exp_ordering_criterion_t;
typedef struct OpenAPI_red_trans_exp_ordering_criterion_s {
} OpenAPI_red_trans_exp_ordering_criterion_t;

OpenAPI_red_trans_exp_ordering_criterion_t *OpenAPI_red_trans_exp_ordering_criterion_create(
);
void OpenAPI_red_trans_exp_ordering_criterion_free(OpenAPI_red_trans_exp_ordering_criterion_t *red_trans_exp_ordering_criterion);
OpenAPI_red_trans_exp_ordering_criterion_t *OpenAPI_red_trans_exp_ordering_criterion_parseFromJSON(cJSON *red_trans_exp_ordering_criterionJSON);
cJSON *OpenAPI_red_trans_exp_ordering_criterion_convertToJSON(OpenAPI_red_trans_exp_ordering_criterion_t *red_trans_exp_ordering_criterion);
OpenAPI_red_trans_exp_ordering_criterion_t *OpenAPI_red_trans_exp_ordering_criterion_copy(OpenAPI_red_trans_exp_ordering_criterion_t *dst, OpenAPI_red_trans_exp_ordering_criterion_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_red_trans_exp_ordering_criterion_H_ */

