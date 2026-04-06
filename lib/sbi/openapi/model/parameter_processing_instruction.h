/*
 * parameter_processing_instruction.h
 *
 * Contains an event parameter name and the respective event parameter values and sets of attributes to be used in summarized reports. 
 */

#ifndef _OpenAPI_parameter_processing_instruction_H_
#define _OpenAPI_parameter_processing_instruction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_parameter_processing_instruction_s OpenAPI_parameter_processing_instruction_t;
#include "aggregation_level.h"
#include "any_type.h"
#include "network_area_info.h"
#include "summarization_attribute.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_parameter_processing_instruction_s {
    char *name;
    OpenAPI_list_t *values;
    OpenAPI_list_t *sum_attrs;
    OpenAPI_aggregation_level_e aggr_level;
    OpenAPI_list_t *supis;
    bool is_temporal_aggr_level;
    int temporal_aggr_level;
    OpenAPI_list_t *areas;
};

OpenAPI_parameter_processing_instruction_t *OpenAPI_parameter_processing_instruction_create(
    char *name,
    OpenAPI_list_t *values,
    OpenAPI_list_t *sum_attrs,
    OpenAPI_aggregation_level_e aggr_level,
    OpenAPI_list_t *supis,
    bool is_temporal_aggr_level,
    int temporal_aggr_level,
    OpenAPI_list_t *areas
);
void OpenAPI_parameter_processing_instruction_free(OpenAPI_parameter_processing_instruction_t *parameter_processing_instruction);
OpenAPI_parameter_processing_instruction_t *OpenAPI_parameter_processing_instruction_parseFromJSON(cJSON *parameter_processing_instructionJSON);
cJSON *OpenAPI_parameter_processing_instruction_convertToJSON(OpenAPI_parameter_processing_instruction_t *parameter_processing_instruction);
OpenAPI_parameter_processing_instruction_t *OpenAPI_parameter_processing_instruction_copy(OpenAPI_parameter_processing_instruction_t *dst, OpenAPI_parameter_processing_instruction_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_parameter_processing_instruction_H_ */

