/*
 * recur_time_1.h
 *
 * Contains the recurring time period
 */

#ifndef _OpenAPI_recur_time_1_H_
#define _OpenAPI_recur_time_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_recur_time_1_s OpenAPI_recur_time_1_t;
#include "recur_type.h"
#include "valid_time_period_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_recur_time_1_s {
    struct OpenAPI_valid_time_period_1_s *recur_time_window;
    OpenAPI_recur_type_e recur_type;
    OpenAPI_list_t *recur_month;
    OpenAPI_list_t *recur_week;
    OpenAPI_list_t *recur_day;
    OpenAPI_list_t *recur_date;
    char *recur_end_time;
};

OpenAPI_recur_time_1_t *OpenAPI_recur_time_1_create(
    OpenAPI_valid_time_period_1_t *recur_time_window,
    OpenAPI_recur_type_e recur_type,
    OpenAPI_list_t *recur_month,
    OpenAPI_list_t *recur_week,
    OpenAPI_list_t *recur_day,
    OpenAPI_list_t *recur_date,
    char *recur_end_time
);
void OpenAPI_recur_time_1_free(OpenAPI_recur_time_1_t *recur_time_1);
OpenAPI_recur_time_1_t *OpenAPI_recur_time_1_parseFromJSON(cJSON *recur_time_1JSON);
cJSON *OpenAPI_recur_time_1_convertToJSON(OpenAPI_recur_time_1_t *recur_time_1);
OpenAPI_recur_time_1_t *OpenAPI_recur_time_1_copy(OpenAPI_recur_time_1_t *dst, OpenAPI_recur_time_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_recur_time_1_H_ */

