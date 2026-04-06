/*
 * ue_comm_order_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_ue_comm_order_criterion_H_
#define _OpenAPI_ue_comm_order_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_comm_order_criterion_NULL = 0, OpenAPI_ue_comm_order_criterion_START_TIME, OpenAPI_ue_comm_order_criterion_DURATION } OpenAPI_ue_comm_order_criterion_e;

char* OpenAPI_ue_comm_order_criterion_ToString(OpenAPI_ue_comm_order_criterion_e ue_comm_order_criterion);

OpenAPI_ue_comm_order_criterion_e OpenAPI_ue_comm_order_criterion_FromString(char* ue_comm_order_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_comm_order_criterion_H_ */

