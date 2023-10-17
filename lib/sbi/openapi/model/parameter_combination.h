/*
 * parameter_combination.h
 *
 * Represents the combination used by the BSF to check whether there is an existing PCF binding  information. 
 */

#ifndef _OpenAPI_parameter_combination_H_
#define _OpenAPI_parameter_combination_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_parameter_combination_s OpenAPI_parameter_combination_t;
typedef struct OpenAPI_parameter_combination_s {
    char *supi;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
} OpenAPI_parameter_combination_t;

OpenAPI_parameter_combination_t *OpenAPI_parameter_combination_create(
    char *supi,
    char *dnn,
    OpenAPI_snssai_t *snssai
);
void OpenAPI_parameter_combination_free(OpenAPI_parameter_combination_t *parameter_combination);
OpenAPI_parameter_combination_t *OpenAPI_parameter_combination_parseFromJSON(cJSON *parameter_combinationJSON);
cJSON *OpenAPI_parameter_combination_convertToJSON(OpenAPI_parameter_combination_t *parameter_combination);
OpenAPI_parameter_combination_t *OpenAPI_parameter_combination_copy(OpenAPI_parameter_combination_t *dst, OpenAPI_parameter_combination_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_parameter_combination_H_ */

