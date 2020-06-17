/*
 * edrx_parameters.h
 *
 *
 */

#ifndef _OpenAPI_edrx_parameters_H_
#define _OpenAPI_edrx_parameters_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_edrx_parameters_s OpenAPI_edrx_parameters_t;
typedef struct OpenAPI_edrx_parameters_s {
    OpenAPI_rat_type_e rat_type;
    char *edrx_value;
} OpenAPI_edrx_parameters_t;

OpenAPI_edrx_parameters_t *OpenAPI_edrx_parameters_create(
    OpenAPI_rat_type_e rat_type,
    char *edrx_value
    );
void OpenAPI_edrx_parameters_free(OpenAPI_edrx_parameters_t *edrx_parameters);
OpenAPI_edrx_parameters_t *OpenAPI_edrx_parameters_parseFromJSON(cJSON *edrx_parametersJSON);
cJSON *OpenAPI_edrx_parameters_convertToJSON(OpenAPI_edrx_parameters_t *edrx_parameters);
OpenAPI_edrx_parameters_t *OpenAPI_edrx_parameters_copy(OpenAPI_edrx_parameters_t *dst, OpenAPI_edrx_parameters_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_edrx_parameters_H_ */

