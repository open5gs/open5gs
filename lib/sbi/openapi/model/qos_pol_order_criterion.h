/*
 * qos_pol_order_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_qos_pol_order_criterion_H_
#define _OpenAPI_qos_pol_order_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_pol_order_criterion_NULL = 0, OpenAPI_qos_pol_order_criterion_QOE, OpenAPI_qos_pol_order_criterion_USAGE_DURATION, OpenAPI_qos_pol_order_criterion_NUMBER_OF_USAGES } OpenAPI_qos_pol_order_criterion_e;

char* OpenAPI_qos_pol_order_criterion_ToString(OpenAPI_qos_pol_order_criterion_e qos_pol_order_criterion);

OpenAPI_qos_pol_order_criterion_e OpenAPI_qos_pol_order_criterion_FromString(char* qos_pol_order_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_pol_order_criterion_H_ */

