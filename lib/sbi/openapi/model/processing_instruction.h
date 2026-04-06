/*
 * processing_instruction.h
 *
 * Contains instructions related to the processing of notifications.
 */

#ifndef _OpenAPI_processing_instruction_H_
#define _OpenAPI_processing_instruction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_processing_instruction_s OpenAPI_processing_instruction_t;
#include "dccf_event.h"
#include "parameter_processing_instruction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_processing_instruction_s {
    struct OpenAPI_dccf_event_s *event_id;
    int proc_interval;
    OpenAPI_list_t *param_proc_instructs;
};

OpenAPI_processing_instruction_t *OpenAPI_processing_instruction_create(
    OpenAPI_dccf_event_t *event_id,
    int proc_interval,
    OpenAPI_list_t *param_proc_instructs
);
void OpenAPI_processing_instruction_free(OpenAPI_processing_instruction_t *processing_instruction);
OpenAPI_processing_instruction_t *OpenAPI_processing_instruction_parseFromJSON(cJSON *processing_instructionJSON);
cJSON *OpenAPI_processing_instruction_convertToJSON(OpenAPI_processing_instruction_t *processing_instruction);
OpenAPI_processing_instruction_t *OpenAPI_processing_instruction_copy(OpenAPI_processing_instruction_t *dst, OpenAPI_processing_instruction_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_processing_instruction_H_ */

