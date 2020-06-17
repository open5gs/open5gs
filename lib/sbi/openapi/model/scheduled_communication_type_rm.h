/*
 * scheduled_communication_type_rm.h
 *
 *
 */

#ifndef _OpenAPI_scheduled_communication_type_rm_H_
#define _OpenAPI_scheduled_communication_type_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"
#include "scheduled_communication_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scheduled_communication_type_rm_s OpenAPI_scheduled_communication_type_rm_t;
typedef struct OpenAPI_scheduled_communication_type_rm_s {
} OpenAPI_scheduled_communication_type_rm_t;

OpenAPI_scheduled_communication_type_rm_t *OpenAPI_scheduled_communication_type_rm_create(
    );
void OpenAPI_scheduled_communication_type_rm_free(OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type_rm);
OpenAPI_scheduled_communication_type_rm_t *OpenAPI_scheduled_communication_type_rm_parseFromJSON(cJSON *scheduled_communication_type_rmJSON);
cJSON *OpenAPI_scheduled_communication_type_rm_convertToJSON(OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type_rm);
OpenAPI_scheduled_communication_type_rm_t *OpenAPI_scheduled_communication_type_rm_copy(OpenAPI_scheduled_communication_type_rm_t *dst, OpenAPI_scheduled_communication_type_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scheduled_communication_type_rm_H_ */

