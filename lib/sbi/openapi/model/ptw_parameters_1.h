/*
 * ptw_parameters_1.h
 *
 * 
 */

#ifndef _OpenAPI_ptw_parameters_1_H_
#define _OpenAPI_ptw_parameters_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "operation_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ptw_parameters_1_s OpenAPI_ptw_parameters_1_t;
typedef struct OpenAPI_ptw_parameters_1_s {
    OpenAPI_operation_mode_e operation_mode;
    char *ptw_value;
} OpenAPI_ptw_parameters_1_t;

OpenAPI_ptw_parameters_1_t *OpenAPI_ptw_parameters_1_create(
    OpenAPI_operation_mode_e operation_mode,
    char *ptw_value
);
void OpenAPI_ptw_parameters_1_free(OpenAPI_ptw_parameters_1_t *ptw_parameters_1);
OpenAPI_ptw_parameters_1_t *OpenAPI_ptw_parameters_1_parseFromJSON(cJSON *ptw_parameters_1JSON);
cJSON *OpenAPI_ptw_parameters_1_convertToJSON(OpenAPI_ptw_parameters_1_t *ptw_parameters_1);
OpenAPI_ptw_parameters_1_t *OpenAPI_ptw_parameters_1_copy(OpenAPI_ptw_parameters_1_t *dst, OpenAPI_ptw_parameters_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ptw_parameters_1_H_ */

