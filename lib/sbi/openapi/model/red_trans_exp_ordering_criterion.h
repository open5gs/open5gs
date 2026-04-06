/*
 * red_trans_exp_ordering_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_red_trans_exp_ordering_criterion_H_
#define _OpenAPI_red_trans_exp_ordering_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_red_trans_exp_ordering_criterion_NULL = 0, OpenAPI_red_trans_exp_ordering_criterion_TIME_SLOT_START, OpenAPI_red_trans_exp_ordering_criterion_RED_TRANS_EXP } OpenAPI_red_trans_exp_ordering_criterion_e;

char* OpenAPI_red_trans_exp_ordering_criterion_ToString(OpenAPI_red_trans_exp_ordering_criterion_e red_trans_exp_ordering_criterion);

OpenAPI_red_trans_exp_ordering_criterion_e OpenAPI_red_trans_exp_ordering_criterion_FromString(char* red_trans_exp_ordering_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_red_trans_exp_ordering_criterion_H_ */

