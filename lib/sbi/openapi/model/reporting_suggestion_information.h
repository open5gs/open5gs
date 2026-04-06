/*
 * reporting_suggestion_information.h
 *
 * Reporting Suggestion Information
 */

#ifndef _OpenAPI_reporting_suggestion_information_H_
#define _OpenAPI_reporting_suggestion_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_reporting_suggestion_information_s OpenAPI_reporting_suggestion_information_t;
#include "reporting_urgency.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_reporting_suggestion_information_s {
    OpenAPI_reporting_urgency_e reporting_urgency;
    bool is_reporting_time_info;
    int reporting_time_info;
};

OpenAPI_reporting_suggestion_information_t *OpenAPI_reporting_suggestion_information_create(
    OpenAPI_reporting_urgency_e reporting_urgency,
    bool is_reporting_time_info,
    int reporting_time_info
);
void OpenAPI_reporting_suggestion_information_free(OpenAPI_reporting_suggestion_information_t *reporting_suggestion_information);
OpenAPI_reporting_suggestion_information_t *OpenAPI_reporting_suggestion_information_parseFromJSON(cJSON *reporting_suggestion_informationJSON);
cJSON *OpenAPI_reporting_suggestion_information_convertToJSON(OpenAPI_reporting_suggestion_information_t *reporting_suggestion_information);
OpenAPI_reporting_suggestion_information_t *OpenAPI_reporting_suggestion_information_copy(OpenAPI_reporting_suggestion_information_t *dst, OpenAPI_reporting_suggestion_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_suggestion_information_H_ */

