/*
 * reporting_options.h
 *
 * 
 */

#ifndef _OpenAPI_reporting_options_H_
#define _OpenAPI_reporting_options_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_reporting_options_s OpenAPI_reporting_options_t;
#include "event_report_mode.h"
#include "muting_exception_instructions.h"
#include "muting_notifications_settings.h"
#include "notification_flag.h"
#include "var_rep_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_reporting_options_s {
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
    struct OpenAPI_muting_exception_instructions_s *muting_exc_instructions;
    struct OpenAPI_muting_notifications_settings_s *muting_not_settings;
    OpenAPI_list_t *var_rep_period_info;
};

OpenAPI_reporting_options_t *OpenAPI_reporting_options_create(
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
    OpenAPI_muting_exception_instructions_t *muting_exc_instructions,
    OpenAPI_muting_notifications_settings_t *muting_not_settings,
    OpenAPI_list_t *var_rep_period_info
);
void OpenAPI_reporting_options_free(OpenAPI_reporting_options_t *reporting_options);
OpenAPI_reporting_options_t *OpenAPI_reporting_options_parseFromJSON(cJSON *reporting_optionsJSON);
cJSON *OpenAPI_reporting_options_convertToJSON(OpenAPI_reporting_options_t *reporting_options);
OpenAPI_reporting_options_t *OpenAPI_reporting_options_copy(OpenAPI_reporting_options_t *dst, OpenAPI_reporting_options_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_options_H_ */

