/*
 * lp_hap_type.h
 *
 * Type of Low Power and/or High Accuracy Positioning
 */

#ifndef _OpenAPI_lp_hap_type_H_
#define _OpenAPI_lp_hap_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_lp_hap_type_NULL = 0, OpenAPI_lp_hap_type_LOW_POW_HIGH_ACCU_POS } OpenAPI_lp_hap_type_e;

char* OpenAPI_lp_hap_type_ToString(OpenAPI_lp_hap_type_e lp_hap_type);

OpenAPI_lp_hap_type_e OpenAPI_lp_hap_type_FromString(char* lp_hap_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lp_hap_type_H_ */

