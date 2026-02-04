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
#include "lcs_qos_class_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lcs_qos_class_s OpenAPI_lcs_qos_class_t;
typedef struct OpenAPI_lcs_qos_class_s {
} OpenAPI_lcs_qos_class_t;

OpenAPI_lcs_qos_class_t *OpenAPI_lcs_qos_class_create(void);
void OpenAPI_lcs_qos_class_free(OpenAPI_lcs_qos_class_t *lcs_qos_class);
OpenAPI_lcs_qos_class_t *OpenAPI_lcs_qos_class_parseFromJSON(cJSON *lcs_qos_classJSON);
cJSON *OpenAPI_lcs_qos_class_convertToJSON(OpenAPI_lcs_qos_class_t *lcs_qos_class);
OpenAPI_lcs_qos_class_t *OpenAPI_lcs_qos_class_copy(OpenAPI_lcs_qos_class_t *dst, OpenAPI_lcs_qos_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_qos_class_H_ */

