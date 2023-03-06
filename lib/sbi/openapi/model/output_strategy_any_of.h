/*
 * output_strategy_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_output_strategy_any_of_H_
#define _OpenAPI_output_strategy_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_output_strategy_any_of_NULL = 0, OpenAPI_output_strategy_any_of_BINARY, OpenAPI_output_strategy_any_of_GRADIENT } OpenAPI_output_strategy_any_of_e;

char* OpenAPI_output_strategy_any_of_ToString(OpenAPI_output_strategy_any_of_e output_strategy_any_of);

OpenAPI_output_strategy_any_of_e OpenAPI_output_strategy_any_of_FromString(char* output_strategy_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_output_strategy_any_of_H_ */

