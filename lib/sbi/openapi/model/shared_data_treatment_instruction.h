/*
 * shared_data_treatment_instruction.h
 *
 * 
 */

#ifndef _OpenAPI_shared_data_treatment_instruction_H_
#define _OpenAPI_shared_data_treatment_instruction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_shared_data_treatment_instruction_NULL = 0, OpenAPI_shared_data_treatment_instruction_USE_IF_NO_CLASH, OpenAPI_shared_data_treatment_instruction_OVERWRITE, OpenAPI_shared_data_treatment_instruction_MAX, OpenAPI_shared_data_treatment_instruction_MIN } OpenAPI_shared_data_treatment_instruction_e;

char* OpenAPI_shared_data_treatment_instruction_ToString(OpenAPI_shared_data_treatment_instruction_e shared_data_treatment_instruction);

OpenAPI_shared_data_treatment_instruction_e OpenAPI_shared_data_treatment_instruction_FromString(char* shared_data_treatment_instruction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_shared_data_treatment_instruction_H_ */

