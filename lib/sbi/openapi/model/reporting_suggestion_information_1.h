/*
 * reporting_suggestion_information_1.h
 *
 * Reporting Suggestion Information
 */

#ifndef _OpenAPI_reporting_suggestion_information_1_H_
#define _OpenAPI_reporting_suggestion_information_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_reporting_suggestion_information_1_s OpenAPI_reporting_suggestion_information_1_t;
#include "reporting_urgency.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_reporting_suggestion_information_1_s {
    OpenAPI_reporting_urgency_e reporting_urgency;
    bool is_reporting_time_info;
    int reporting_time_info;
};

OpenAPI_reporting_suggestion_information_1_t *OpenAPI_reporting_suggestion_information_1_create(
    OpenAPI_reporting_urgency_e reporting_urgency,
    bool is_reporting_time_info,
    int reporting_time_info
);
void OpenAPI_reporting_suggestion_information_1_free(OpenAPI_reporting_suggestion_information_1_t *reporting_suggestion_information_1);
OpenAPI_reporting_suggestion_information_1_t *OpenAPI_reporting_suggestion_information_1_parseFromJSON(cJSON *reporting_suggestion_information_1JSON);
cJSON *OpenAPI_reporting_suggestion_information_1_convertToJSON(OpenAPI_reporting_suggestion_information_1_t *reporting_suggestion_information_1);
OpenAPI_reporting_suggestion_information_1_t *OpenAPI_reporting_suggestion_information_1_copy(OpenAPI_reporting_suggestion_information_1_t *dst, OpenAPI_reporting_suggestion_information_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_suggestion_information_1_H_ */

