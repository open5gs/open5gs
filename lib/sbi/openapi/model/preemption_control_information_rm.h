/*
 * preemption_control_information_rm.h
 *
 * This data type is defined in the same way as the PreemptionControlInformation data type, but with the OpenAPI nullable property set to true.
 */

#ifndef _OpenAPI_preemption_control_information_rm_H_
#define _OpenAPI_preemption_control_information_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"
#include "preemption_control_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_preemption_control_information_rm_s OpenAPI_preemption_control_information_rm_t;
typedef struct OpenAPI_preemption_control_information_rm_s {
} OpenAPI_preemption_control_information_rm_t;

OpenAPI_preemption_control_information_rm_t *OpenAPI_preemption_control_information_rm_create(
);
void OpenAPI_preemption_control_information_rm_free(OpenAPI_preemption_control_information_rm_t *preemption_control_information_rm);
OpenAPI_preemption_control_information_rm_t *OpenAPI_preemption_control_information_rm_parseFromJSON(cJSON *preemption_control_information_rmJSON);
cJSON *OpenAPI_preemption_control_information_rm_convertToJSON(OpenAPI_preemption_control_information_rm_t *preemption_control_information_rm);
OpenAPI_preemption_control_information_rm_t *OpenAPI_preemption_control_information_rm_copy(OpenAPI_preemption_control_information_rm_t *dst, OpenAPI_preemption_control_information_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_preemption_control_information_rm_H_ */

