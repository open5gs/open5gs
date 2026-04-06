/*
 * reporting_options_1.h
 *
 * 
 */

#ifndef _OpenAPI_reporting_options_1_H_
#define _OpenAPI_reporting_options_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_reporting_options_1_s OpenAPI_reporting_options_1_t;
#include "event_report_mode.h"
#include "muting_exception_instructions_1.h"
#include "muting_notifications_settings_1.h"
#include "notification_flag.h"
#include "var_rep_period_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_reporting_options_1_s {
    OpenAPI_event_report_mode_e report_mode;
    bool is_max_num_of_reports;
    int max_num_of_reports;
    char *expiry;
    bool is_sampling_ratio;
    int sampling_ratio;
    bool is_guard_time;
    int guard_time;
    bool is_report_period;
    int report_period;
    OpenAPI_notification_flag_e notif_flag;
    struct OpenAPI_muting_exception_instructions_1_s *muting_exc_instructions;
    struct OpenAPI_muting_notifications_settings_1_s *muting_not_settings;
    OpenAPI_list_t *var_rep_period_info;
};

OpenAPI_reporting_options_1_t *OpenAPI_reporting_options_1_create(
    OpenAPI_event_report_mode_e report_mode,
    bool is_max_num_of_reports,
    int max_num_of_reports,
    char *expiry,
    bool is_sampling_ratio,
    int sampling_ratio,
    bool is_guard_time,
    int guard_time,
    bool is_report_period,
    int report_period,
    OpenAPI_notification_flag_e notif_flag,
    OpenAPI_muting_exception_instructions_1_t *muting_exc_instructions,
    OpenAPI_muting_notifications_settings_1_t *muting_not_settings,
    OpenAPI_list_t *var_rep_period_info
);
void OpenAPI_reporting_options_1_free(OpenAPI_reporting_options_1_t *reporting_options_1);
OpenAPI_reporting_options_1_t *OpenAPI_reporting_options_1_parseFromJSON(cJSON *reporting_options_1JSON);
cJSON *OpenAPI_reporting_options_1_convertToJSON(OpenAPI_reporting_options_1_t *reporting_options_1);
OpenAPI_reporting_options_1_t *OpenAPI_reporting_options_1_copy(OpenAPI_reporting_options_1_t *dst, OpenAPI_reporting_options_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_options_1_H_ */

