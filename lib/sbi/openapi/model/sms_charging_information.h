/*
 * sms_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_sms_charging_information_H_
#define _OpenAPI_sms_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sms_charging_information_s OpenAPI_sms_charging_information_t;
#include "charging_user_location.h"
#include "delivery_report_requested.h"
#include "message_class.h"
#include "originator_info.h"
#include "rat_type.h"
#include "recipient_info.h"
#include "reply_path_requested.h"
#include "roamer_in_out.h"
#include "sm_message_type.h"
#include "sm_priority.h"
#include "sm_service_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sms_charging_information_s {
    struct OpenAPI_originator_info_s *originator_info;
    OpenAPI_list_t *recipient_info;
    char *user_equipment_info;
    OpenAPI_roamer_in_out_e roamer_in_out;
    struct OpenAPI_charging_user_location_s *user_locationinfo;
    char *uetime_zone;
    OpenAPI_rat_type_e r_at_type;
    char *s_msc_address;
    bool is_s_m_data_coding_scheme;
    int s_m_data_coding_scheme;
    OpenAPI_sm_message_type_e s_m_message_type;
    OpenAPI_reply_path_requested_e s_m_reply_path_requested;
    char *s_m_user_data_header;
    char *s_m_status;
    char *s_m_discharge_time;
    bool is_numberof_messages_sent;
    int numberof_messages_sent;
    OpenAPI_sm_service_type_e s_m_service_type;
    bool is_s_m_sequence_number;
    int s_m_sequence_number;
    bool is_s_m_sresult;
    int s_m_sresult;
    char *submission_time;
    OpenAPI_sm_priority_e s_m_priority;
    char *message_reference;
    bool is_message_size;
    int message_size;
    struct OpenAPI_message_class_s *message_class;
    OpenAPI_delivery_report_requested_e delivery_report_requested;
};

OpenAPI_sms_charging_information_t *OpenAPI_sms_charging_information_create(
    OpenAPI_originator_info_t *originator_info,
    OpenAPI_list_t *recipient_info,
    char *user_equipment_info,
    OpenAPI_roamer_in_out_e roamer_in_out,
    OpenAPI_charging_user_location_t *user_locationinfo,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    char *s_msc_address,
    bool is_s_m_data_coding_scheme,
    int s_m_data_coding_scheme,
    OpenAPI_sm_message_type_e s_m_message_type,
    OpenAPI_reply_path_requested_e s_m_reply_path_requested,
    char *s_m_user_data_header,
    char *s_m_status,
    char *s_m_discharge_time,
    bool is_numberof_messages_sent,
    int numberof_messages_sent,
    OpenAPI_sm_service_type_e s_m_service_type,
    bool is_s_m_sequence_number,
    int s_m_sequence_number,
    bool is_s_m_sresult,
    int s_m_sresult,
    char *submission_time,
    OpenAPI_sm_priority_e s_m_priority,
    char *message_reference,
    bool is_message_size,
    int message_size,
    OpenAPI_message_class_t *message_class,
    OpenAPI_delivery_report_requested_e delivery_report_requested
);
void OpenAPI_sms_charging_information_free(OpenAPI_sms_charging_information_t *sms_charging_information);
OpenAPI_sms_charging_information_t *OpenAPI_sms_charging_information_parseFromJSON(cJSON *sms_charging_informationJSON);
cJSON *OpenAPI_sms_charging_information_convertToJSON(OpenAPI_sms_charging_information_t *sms_charging_information);
OpenAPI_sms_charging_information_t *OpenAPI_sms_charging_information_copy(OpenAPI_sms_charging_information_t *dst, OpenAPI_sms_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_charging_information_H_ */

