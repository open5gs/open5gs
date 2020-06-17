/*
 * ptw_parameters.h
 *
 *
 */

#ifndef _OpenAPI_ptw_parameters_H_
#define _OpenAPI_ptw_parameters_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "operation_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ptw_parameters_s OpenAPI_ptw_parameters_t;
typedef struct OpenAPI_ptw_parameters_s {
    OpenAPI_operation_mode_e operation_mode;
    char *ptw_value;
} OpenAPI_ptw_parameters_t;

OpenAPI_ptw_parameters_t *OpenAPI_ptw_parameters_create(
    OpenAPI_operation_mode_e operation_mode,
    char *ptw_value
    );
void OpenAPI_ptw_parameters_free(OpenAPI_ptw_parameters_t *ptw_parameters);
OpenAPI_ptw_parameters_t *OpenAPI_ptw_parameters_parseFromJSON(cJSON *ptw_parametersJSON);
cJSON *OpenAPI_ptw_parameters_convertToJSON(OpenAPI_ptw_parameters_t *ptw_parameters);
OpenAPI_ptw_parameters_t *OpenAPI_ptw_parameters_copy(OpenAPI_ptw_parameters_t *dst, OpenAPI_ptw_parameters_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ptw_parameters_H_ */

