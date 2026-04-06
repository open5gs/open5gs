/*
 * header_handling_reporting_1.h
 *
 * Indicates that reporting is requested for the performed Header Handling Action. 
 */

#ifndef _OpenAPI_header_handling_reporting_1_H_
#define _OpenAPI_header_handling_reporting_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_header_handling_reporting_1_s OpenAPI_header_handling_reporting_1_t;
#include "reporting_suggestion_information_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_header_handling_reporting_1_s {
    bool is_notif_flag;
    int notif_flag;
    struct OpenAPI_reporting_suggestion_information_1_s *rep_sugg_info;
    bool is_one_time_ind;
    int one_time_ind;
};

OpenAPI_header_handling_reporting_1_t *OpenAPI_header_handling_reporting_1_create(
    bool is_notif_flag,
    int notif_flag,
    OpenAPI_reporting_suggestion_information_1_t *rep_sugg_info,
    bool is_one_time_ind,
    int one_time_ind
);
void OpenAPI_header_handling_reporting_1_free(OpenAPI_header_handling_reporting_1_t *header_handling_reporting_1);
OpenAPI_header_handling_reporting_1_t *OpenAPI_header_handling_reporting_1_parseFromJSON(cJSON *header_handling_reporting_1JSON);
cJSON *OpenAPI_header_handling_reporting_1_convertToJSON(OpenAPI_header_handling_reporting_1_t *header_handling_reporting_1);
OpenAPI_header_handling_reporting_1_t *OpenAPI_header_handling_reporting_1_copy(OpenAPI_header_handling_reporting_1_t *dst, OpenAPI_header_handling_reporting_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_header_handling_reporting_1_H_ */

