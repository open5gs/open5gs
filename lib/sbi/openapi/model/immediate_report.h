/*
 * immediate_report.h
 *
 * Contains the immediate report
 */

#ifndef _OpenAPI_immediate_report_H_
#define _OpenAPI_immediate_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_immediate_report_s OpenAPI_immediate_report_t;
#include "shared_data.h"
#include "subscription_data_sets.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_immediate_report_s {
    struct OpenAPI_subscription_data_sets_s *subscription_data_sets;
    OpenAPI_list_t *shared_data_list;
};

OpenAPI_immediate_report_t *OpenAPI_immediate_report_create(
    OpenAPI_subscription_data_sets_t *subscription_data_sets,
    OpenAPI_list_t *shared_data_list
);
void OpenAPI_immediate_report_free(OpenAPI_immediate_report_t *immediate_report);
OpenAPI_immediate_report_t *OpenAPI_immediate_report_parseFromJSON(cJSON *immediate_reportJSON);
cJSON *OpenAPI_immediate_report_convertToJSON(OpenAPI_immediate_report_t *immediate_report);
OpenAPI_immediate_report_t *OpenAPI_immediate_report_copy(OpenAPI_immediate_report_t *dst, OpenAPI_immediate_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_immediate_report_H_ */

