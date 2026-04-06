/*
 * recur_time.h
 *
 * Contains the recurring time period
 */

#ifndef _OpenAPI_recur_time_H_
#define _OpenAPI_recur_time_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_recur_time_s OpenAPI_recur_time_t;
#include "recur_type.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_recur_time_s {
    struct OpenAPI_valid_time_period_s *recur_time_window;
    OpenAPI_recur_type_e recur_type;
    OpenAPI_list_t *recur_month;
    OpenAPI_list_t *recur_week;
    OpenAPI_list_t *recur_day;
    OpenAPI_list_t *recur_date;
    char *recur_end_time;
};

OpenAPI_recur_time_t *OpenAPI_recur_time_create(
    OpenAPI_valid_time_period_t *recur_time_window,
    OpenAPI_recur_type_e recur_type,
    OpenAPI_list_t *recur_month,
    OpenAPI_list_t *recur_week,
    OpenAPI_list_t *recur_day,
    OpenAPI_list_t *recur_date,
    char *recur_end_time
);
void OpenAPI_recur_time_free(OpenAPI_recur_time_t *recur_time);
OpenAPI_recur_time_t *OpenAPI_recur_time_parseFromJSON(cJSON *recur_timeJSON);
cJSON *OpenAPI_recur_time_convertToJSON(OpenAPI_recur_time_t *recur_time);
OpenAPI_recur_time_t *OpenAPI_recur_time_copy(OpenAPI_recur_time_t *dst, OpenAPI_recur_time_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_recur_time_H_ */

