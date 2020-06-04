/*
 * scheduled_communication_type.h
 *
 *
 */

#ifndef _OpenAPI_scheduled_communication_type_H_
#define _OpenAPI_scheduled_communication_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scheduled_communication_type_s OpenAPI_scheduled_communication_type_t;
typedef struct OpenAPI_scheduled_communication_type_s {
} OpenAPI_scheduled_communication_type_t;

OpenAPI_scheduled_communication_type_t *OpenAPI_scheduled_communication_type_create(
    );
void OpenAPI_scheduled_communication_type_free(OpenAPI_scheduled_communication_type_t *scheduled_communication_type);
OpenAPI_scheduled_communication_type_t *OpenAPI_scheduled_communication_type_parseFromJSON(cJSON *scheduled_communication_typeJSON);
cJSON *OpenAPI_scheduled_communication_type_convertToJSON(OpenAPI_scheduled_communication_type_t *scheduled_communication_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scheduled_communication_type_H_ */

