/*
 * edrx_parameters_1.h
 *
 * 
 */

#ifndef _OpenAPI_edrx_parameters_1_H_
#define _OpenAPI_edrx_parameters_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_edrx_parameters_1_s OpenAPI_edrx_parameters_1_t;
typedef struct OpenAPI_edrx_parameters_1_s {
    OpenAPI_rat_type_e rat_type;
    char *edrx_value;
} OpenAPI_edrx_parameters_1_t;

OpenAPI_edrx_parameters_1_t *OpenAPI_edrx_parameters_1_create(
    OpenAPI_rat_type_e rat_type,
    char *edrx_value
);
void OpenAPI_edrx_parameters_1_free(OpenAPI_edrx_parameters_1_t *edrx_parameters_1);
OpenAPI_edrx_parameters_1_t *OpenAPI_edrx_parameters_1_parseFromJSON(cJSON *edrx_parameters_1JSON);
cJSON *OpenAPI_edrx_parameters_1_convertToJSON(OpenAPI_edrx_parameters_1_t *edrx_parameters_1);
OpenAPI_edrx_parameters_1_t *OpenAPI_edrx_parameters_1_copy(OpenAPI_edrx_parameters_1_t *dst, OpenAPI_edrx_parameters_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_edrx_parameters_1_H_ */

