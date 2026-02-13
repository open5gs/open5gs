/*
 * lcs_qos_class_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_lcs_qos_class_any_of_H_
#define _OpenAPI_lcs_qos_class_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_lcs_qos_class_any_of_NULL = 0, OpenAPI_lcs_qos_class_any_of_BEST_EFFORT, OpenAPI_lcs_qos_class_any_of_ASSURED, OpenAPI_lcs_qos_class_any_of_MULTIPLE_QOS } OpenAPI_lcs_qos_class_any_of_e;

char* OpenAPI_lcs_qos_class_any_of_ToString(OpenAPI_lcs_qos_class_any_of_e lcs_qos_class_any_of);

OpenAPI_lcs_qos_class_any_of_e OpenAPI_lcs_qos_class_any_of_FromString(char* lcs_qos_class_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_qos_class_any_of_H_ */

