/*
 * lcs_qos_class.h
 *
 * Specifies LCS QoS class.
 */

#ifndef _OpenAPI_lcs_qos_class_H_
#define _OpenAPI_lcs_qos_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_lcs_qos_class_NULL = 0, OpenAPI_lcs_qos_class_BEST_EFFORT, OpenAPI_lcs_qos_class_ASSURED, OpenAPI_lcs_qos_class_MULTIPLE_QOS } OpenAPI_lcs_qos_class_e;

char* OpenAPI_lcs_qos_class_ToString(OpenAPI_lcs_qos_class_e lcs_qos_class);

OpenAPI_lcs_qos_class_e OpenAPI_lcs_qos_class_FromString(char* lcs_qos_class);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_qos_class_H_ */

