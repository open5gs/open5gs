/*
 * mms_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_mms_charging_information_H_
#define _OpenAPI_mms_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mms_charging_information_s OpenAPI_mms_charging_information_t;
#include "charging_user_location.h"
#include "mm_content_type.h"
#include "mm_originator_info.h"
#include "mm_recipient_info.h"
#include "rat_type.h"
#include "sm_priority.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mms_charging_information_s {
    struct OpenAPI_mm_originator_info_s *mm_originator_info;
    OpenAPI_list_t *mm_recipient_info_list;
    struct OpenAPI_charging_user_location_s *user_locationinfo;
    char *uetime_zone;
    OpenAPI_rat_type_e r_at_type;
    char *correlation_information;
    char *submission_time;
    struct OpenAPI_mm_content_type_s *mm_content_type;
    OpenAPI_sm_priority_e mm_priority;
    char *message_id;
    char *message_type;
    bool is_message_size;
    int message_size;
    char *message_class;
    bool is_delivery_report_requested;
    int delivery_report_requested;
    bool is_read_reply_report_requested;
    int read_reply_report_requested;
    char *applic_id;
    char *reply_applic_id;
    char *aux_applic_info;
    char *content_class;
    bool is_d_rm_content;
    int d_rm_content;
    bool is_adaptations;
    int adaptations;
    char *vas_id;
    char *vasp_id;
};

OpenAPI_mms_charging_information_t *OpenAPI_mms_charging_information_create(
    OpenAPI_mm_originator_info_t *mm_originator_info,
    OpenAPI_list_t *mm_recipient_info_list,
    OpenAPI_charging_user_location_t *user_locationinfo,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    char *correlation_information,
    char *submission_time,
    OpenAPI_mm_content_type_t *mm_content_type,
    OpenAPI_sm_priority_e mm_priority,
    char *message_id,
    char *message_type,
    bool is_message_size,
    int message_size,
    char *message_class,
    bool is_delivery_report_requested,
    int delivery_report_requested,
    bool is_read_reply_report_requested,
    int read_reply_report_requested,
    char *applic_id,
    char *reply_applic_id,
    char *aux_applic_info,
    char *content_class,
    bool is_d_rm_content,
    int d_rm_content,
    bool is_adaptations,
    int adaptations,
    char *vas_id,
    char *vasp_id
);
void OpenAPI_mms_charging_information_free(OpenAPI_mms_charging_information_t *mms_charging_information);
OpenAPI_mms_charging_information_t *OpenAPI_mms_charging_information_parseFromJSON(cJSON *mms_charging_informationJSON);
cJSON *OpenAPI_mms_charging_information_convertToJSON(OpenAPI_mms_charging_information_t *mms_charging_information);
OpenAPI_mms_charging_information_t *OpenAPI_mms_charging_information_copy(OpenAPI_mms_charging_information_t *dst, OpenAPI_mms_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mms_charging_information_H_ */

