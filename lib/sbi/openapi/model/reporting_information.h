/*
 * reporting_information.h
 *
 * Represents the type of reporting that the subscription requires.
 */

#ifndef _OpenAPI_reporting_information_H_
#define _OpenAPI_reporting_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_reporting_information_s OpenAPI_reporting_information_t;
#include "muting_exception_instructions.h"
#include "muting_notifications_settings.h"
#include "notification_flag.h"
#include "notification_method_1.h"
#include "partitioning_criteria.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_reporting_information_s {
    bool is_imm_rep;
    int imm_rep;
    OpenAPI_notification_method_1_e notif_method;
    bool is_max_report_nbr;
    int max_report_nbr;
    char *mon_dur;
    bool is_rep_period;
    int rep_period;
    bool is_samp_ratio;
    int samp_ratio;
    OpenAPI_list_t *partition_criteria;
    bool is_grp_rep_time;
    int grp_rep_time;
    OpenAPI_notification_flag_e notif_flag;
    struct OpenAPI_muting_exception_instructions_s *notif_flag_instruct;
    struct OpenAPI_muting_notifications_settings_s *muting_setting;
};

OpenAPI_reporting_information_t *OpenAPI_reporting_information_create(
    bool is_imm_rep,
    int imm_rep,
    OpenAPI_notification_method_1_e notif_method,
    bool is_max_report_nbr,
    int max_report_nbr,
    char *mon_dur,
    bool is_rep_period,
    int rep_period,
    bool is_samp_ratio,
    int samp_ratio,
    OpenAPI_list_t *partition_criteria,
    bool is_grp_rep_time,
    int grp_rep_time,
    OpenAPI_notification_flag_e notif_flag,
    OpenAPI_muting_exception_instructions_t *notif_flag_instruct,
    OpenAPI_muting_notifications_settings_t *muting_setting
);
void OpenAPI_reporting_information_free(OpenAPI_reporting_information_t *reporting_information);
OpenAPI_reporting_information_t *OpenAPI_reporting_information_parseFromJSON(cJSON *reporting_informationJSON);
cJSON *OpenAPI_reporting_information_convertToJSON(OpenAPI_reporting_information_t *reporting_information);
OpenAPI_reporting_information_t *OpenAPI_reporting_information_copy(OpenAPI_reporting_information_t *dst, OpenAPI_reporting_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_information_H_ */

