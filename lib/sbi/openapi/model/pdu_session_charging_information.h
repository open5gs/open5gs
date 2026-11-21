/*
 * pdu_session_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_session_charging_information_H_
#define _OpenAPI_pdu_session_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_session_charging_information_s OpenAPI_pdu_session_charging_information_t;
#include "call_info.h"
#include "charging_user_location.h"
#include "pdu_session_information.h"
#include "presence_info.h"
#include "ran_secondary_rat_usage_report.h"
#include "user_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_session_charging_information_s {
    bool is_charging_id;
    int charging_id;
    char *s_m_fcharging_id;
    bool is_home_provided_charging_id;
    int home_provided_charging_id;
    char *s_mf_home_provided_charging_id;
    struct OpenAPI_user_information_s *user_information;
    struct OpenAPI_charging_user_location_s *user_locationinfo;
    bool is_i_ms_session_information_null;
    struct OpenAPI_call_info_s *i_ms_session_information;
    struct OpenAPI_charging_user_location_s *m_apdu_non3_gpp_user_location_info;
    char *non3_gpp_user_location_time;
    char *m_apdu_non3_gpp_user_location_time;
    OpenAPI_list_t* presence_reporting_area_information;
    char *uetime_zone;
    struct OpenAPI_pdu_session_information_s *pdu_session_information;
    bool is_unit_count_inactivity_timer;
    int unit_count_inactivity_timer;
    struct OpenAPI_ran_secondary_rat_usage_report_s *r_an_secondary_rat_usage_report;
};

OpenAPI_pdu_session_charging_information_t *OpenAPI_pdu_session_charging_information_create(
    bool is_charging_id,
    int charging_id,
    char *s_m_fcharging_id,
    bool is_home_provided_charging_id,
    int home_provided_charging_id,
    char *s_mf_home_provided_charging_id,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_locationinfo,
    bool is_i_ms_session_information_null,
    OpenAPI_call_info_t *i_ms_session_information,
    OpenAPI_charging_user_location_t *m_apdu_non3_gpp_user_location_info,
    char *non3_gpp_user_location_time,
    char *m_apdu_non3_gpp_user_location_time,
    OpenAPI_list_t* presence_reporting_area_information,
    char *uetime_zone,
    OpenAPI_pdu_session_information_t *pdu_session_information,
    bool is_unit_count_inactivity_timer,
    int unit_count_inactivity_timer,
    OpenAPI_ran_secondary_rat_usage_report_t *r_an_secondary_rat_usage_report
);
void OpenAPI_pdu_session_charging_information_free(OpenAPI_pdu_session_charging_information_t *pdu_session_charging_information);
OpenAPI_pdu_session_charging_information_t *OpenAPI_pdu_session_charging_information_parseFromJSON(cJSON *pdu_session_charging_informationJSON);
cJSON *OpenAPI_pdu_session_charging_information_convertToJSON(OpenAPI_pdu_session_charging_information_t *pdu_session_charging_information);
OpenAPI_pdu_session_charging_information_t *OpenAPI_pdu_session_charging_information_copy(OpenAPI_pdu_session_charging_information_t *dst, OpenAPI_pdu_session_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_charging_information_H_ */

