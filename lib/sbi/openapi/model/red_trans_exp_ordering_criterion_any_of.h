/*
 * red_trans_exp_ordering_criterion_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_red_trans_exp_ordering_criterion_any_of_H_
#define _OpenAPI_red_trans_exp_ordering_criterion_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_red_trans_exp_ordering_criterion_any_of_NULL = 0, OpenAPI_red_trans_exp_ordering_criterion_any_of_TIME_SLOT_START, OpenAPI_red_trans_exp_ordering_criterion_any_of_RED_TRANS_EXP } OpenAPI_red_trans_exp_ordering_criterion_any_of_e;

char* OpenAPI_red_trans_exp_ordering_criterion_any_of_ToString(OpenAPI_red_trans_exp_ordering_criterion_any_of_e red_trans_exp_ordering_criterion_any_of);

OpenAPI_red_trans_exp_ordering_criterion_any_of_e OpenAPI_red_trans_exp_ordering_criterion_any_of_FromString(char* red_trans_exp_ordering_criterion_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_red_trans_exp_ordering_criterion_any_of_H_ */

