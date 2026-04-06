/*
 * immediate_report_2.h
 *
 * Contains the immediate report
 */

#ifndef _OpenAPI_immediate_report_2_H_
#define _OpenAPI_immediate_report_2_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_immediate_report_2_s OpenAPI_immediate_report_2_t;
#include "shared_data_1.h"
#include "subscription_data_sets_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_immediate_report_2_s {
    struct OpenAPI_subscription_data_sets_1_s *subscription_data_sets;
    OpenAPI_list_t *shared_data_list;
};

OpenAPI_immediate_report_2_t *OpenAPI_immediate_report_2_create(
    OpenAPI_subscription_data_sets_1_t *subscription_data_sets,
    OpenAPI_list_t *shared_data_list
);
void OpenAPI_immediate_report_2_free(OpenAPI_immediate_report_2_t *immediate_report_2);
OpenAPI_immediate_report_2_t *OpenAPI_immediate_report_2_parseFromJSON(cJSON *immediate_report_2JSON);
cJSON *OpenAPI_immediate_report_2_convertToJSON(OpenAPI_immediate_report_2_t *immediate_report_2);
OpenAPI_immediate_report_2_t *OpenAPI_immediate_report_2_copy(OpenAPI_immediate_report_2_t *dst, OpenAPI_immediate_report_2_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_immediate_report_2_H_ */

