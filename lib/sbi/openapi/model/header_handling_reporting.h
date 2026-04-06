/*
 * header_handling_reporting.h
 *
 * Indicates that reporting is requested for the performed Header Handling Action. 
 */

#ifndef _OpenAPI_header_handling_reporting_H_
#define _OpenAPI_header_handling_reporting_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_header_handling_reporting_s OpenAPI_header_handling_reporting_t;
#include "reporting_suggestion_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_header_handling_reporting_s {
    bool is_notif_flag;
    int notif_flag;
    struct OpenAPI_reporting_suggestion_information_s *rep_sugg_info;
    bool is_one_time_ind;
    int one_time_ind;
};

OpenAPI_header_handling_reporting_t *OpenAPI_header_handling_reporting_create(
    bool is_notif_flag,
    int notif_flag,
    OpenAPI_reporting_suggestion_information_t *rep_sugg_info,
    bool is_one_time_ind,
    int one_time_ind
);
void OpenAPI_header_handling_reporting_free(OpenAPI_header_handling_reporting_t *header_handling_reporting);
OpenAPI_header_handling_reporting_t *OpenAPI_header_handling_reporting_parseFromJSON(cJSON *header_handling_reportingJSON);
cJSON *OpenAPI_header_handling_reporting_convertToJSON(OpenAPI_header_handling_reporting_t *header_handling_reporting);
OpenAPI_header_handling_reporting_t *OpenAPI_header_handling_reporting_copy(OpenAPI_header_handling_reporting_t *dst, OpenAPI_header_handling_reporting_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_header_handling_reporting_H_ */

