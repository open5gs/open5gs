/*
 * privacy_parameter.h
 *
 *
 */

#ifndef _OpenAPI_privacy_parameter_H_
#define _OpenAPI_privacy_parameter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_privacy_parameter_s OpenAPI_privacy_parameter_t;
typedef struct OpenAPI_privacy_parameter_s {
    OpenAPI_list_t *ser_ids;
    char *geographical_area;
    int timer;
} OpenAPI_privacy_parameter_t;

OpenAPI_privacy_parameter_t *OpenAPI_privacy_parameter_create(
    OpenAPI_list_t *ser_ids,
    char *geographical_area,
    int timer
    );
void OpenAPI_privacy_parameter_free(OpenAPI_privacy_parameter_t *privacy_parameter);
OpenAPI_privacy_parameter_t *OpenAPI_privacy_parameter_parseFromJSON(cJSON *privacy_parameterJSON);
cJSON *OpenAPI_privacy_parameter_convertToJSON(OpenAPI_privacy_parameter_t *privacy_parameter);
OpenAPI_privacy_parameter_t *OpenAPI_privacy_parameter_copy(OpenAPI_privacy_parameter_t *dst, OpenAPI_privacy_parameter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_privacy_parameter_H_ */

