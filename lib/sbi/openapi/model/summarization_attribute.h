/*
 * summarization_attribute.h
 *
 * 
 */

#ifndef _OpenAPI_summarization_attribute_H_
#define _OpenAPI_summarization_attribute_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_summarization_attribute_NULL = 0, OpenAPI_summarization_attribute_SPACING, OpenAPI_summarization_attribute_DURATION, OpenAPI_summarization_attribute_OCCURRENCES, OpenAPI_summarization_attribute_AVG_VAR, OpenAPI_summarization_attribute_FREQ_VAL, OpenAPI_summarization_attribute_MIN_MAX } OpenAPI_summarization_attribute_e;

char* OpenAPI_summarization_attribute_ToString(OpenAPI_summarization_attribute_e summarization_attribute);

OpenAPI_summarization_attribute_e OpenAPI_summarization_attribute_FromString(char* summarization_attribute);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_summarization_attribute_H_ */

