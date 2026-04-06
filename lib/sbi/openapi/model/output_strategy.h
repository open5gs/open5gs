/*
 * output_strategy.h
 *
 * 
 */

#ifndef _OpenAPI_output_strategy_H_
#define _OpenAPI_output_strategy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_output_strategy_NULL = 0, OpenAPI_output_strategy_BINARY, OpenAPI_output_strategy_GRADIENT } OpenAPI_output_strategy_e;

char* OpenAPI_output_strategy_ToString(OpenAPI_output_strategy_e output_strategy);

OpenAPI_output_strategy_e OpenAPI_output_strategy_FromString(char* output_strategy);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_output_strategy_H_ */

