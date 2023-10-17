/*
 * output_strategy.h
 *
 * Possible values are: - BINARY: Indicates that the analytics shall only be reported when the requested level of accuracy is reached within a cycle of periodic notification. - GRADIENT: Indicates that the analytics shall be reported according with the periodicity irrespective of whether the requested level of accuracy has been reached or not. 
 */

#ifndef _OpenAPI_output_strategy_H_
#define _OpenAPI_output_strategy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "output_strategy_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_output_strategy_s OpenAPI_output_strategy_t;
typedef struct OpenAPI_output_strategy_s {
} OpenAPI_output_strategy_t;

OpenAPI_output_strategy_t *OpenAPI_output_strategy_create(
);
void OpenAPI_output_strategy_free(OpenAPI_output_strategy_t *output_strategy);
OpenAPI_output_strategy_t *OpenAPI_output_strategy_parseFromJSON(cJSON *output_strategyJSON);
cJSON *OpenAPI_output_strategy_convertToJSON(OpenAPI_output_strategy_t *output_strategy);
OpenAPI_output_strategy_t *OpenAPI_output_strategy_copy(OpenAPI_output_strategy_t *dst, OpenAPI_output_strategy_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_output_strategy_H_ */

