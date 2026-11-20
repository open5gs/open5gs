/*
 * registration_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_registration_charging_information_H_
#define _OpenAPI_registration_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_registration_charging_information_s OpenAPI_registration_charging_information_t;
#include "alternative_nssai_map.h"
#include "charging_user_location.h"
#include "global_ran_node_id.h"
#include "mico_mode_indication.h"
#include "nssai_map.h"
#include "plmn_id_nid.h"
#include "ps_cell_information.h"
#include "rat_type.h"
#include "registration_message_type.h"
#include "service_area_restriction.h"
#include "sms_indication.h"
#include "snssai.h"
#include "tai.h"
#include "user_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_registration_charging_information_s {
    OpenAPI_registration_message_type_e registration_messagetype;
    struct OpenAPI_user_information_s *user_information;
    struct OpenAPI_charging_user_location_s *user_locationinfo;
    struct OpenAPI_ps_cell_information_s *p_s_cell_information;
    char *uetime_zone;
    OpenAPI_rat_type_e r_at_type;
    char *_5_gmm_capability;
    OpenAPI_mico_mode_indication_e m_ico_mode_indication;
    OpenAPI_sms_indication_e sms_indication;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *service_area_restriction;
    OpenAPI_list_t *requested_nssai;
    OpenAPI_list_t *allowed_nssai;
    OpenAPI_list_t *rejected_nssai;
    OpenAPI_list_t *n_ssai_map_list;
    OpenAPI_list_t *alternative_nssai_map;
    bool is_amf_ue_ngap_id;
    int amf_ue_ngap_id;
    bool is_ran_ue_ngap_id;
    int ran_ue_ngap_id;
    struct OpenAPI_global_ran_node_id_s *ran_node_id;
    struct OpenAPI_plmn_id_nid_s *s_npnid;
    OpenAPI_list_t *c_agid_list;
    bool is_satellite_access_indicator;
    int satellite_access_indicator;
    char *service_level_aa;
};

OpenAPI_registration_charging_information_t *OpenAPI_registration_charging_information_create(
    OpenAPI_registration_message_type_e registration_messagetype,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_locationinfo,
    OpenAPI_ps_cell_information_t *p_s_cell_information,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    char *_5_gmm_capability,
    OpenAPI_mico_mode_indication_e m_ico_mode_indication,
    OpenAPI_sms_indication_e sms_indication,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *service_area_restriction,
    OpenAPI_list_t *requested_nssai,
    OpenAPI_list_t *allowed_nssai,
    OpenAPI_list_t *rejected_nssai,
    OpenAPI_list_t *n_ssai_map_list,
    OpenAPI_list_t *alternative_nssai_map,
    bool is_amf_ue_ngap_id,
    int amf_ue_ngap_id,
    bool is_ran_ue_ngap_id,
    int ran_ue_ngap_id,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    OpenAPI_plmn_id_nid_t *s_npnid,
    OpenAPI_list_t *c_agid_list,
    bool is_satellite_access_indicator,
    int satellite_access_indicator,
    char *service_level_aa
);
void OpenAPI_registration_charging_information_free(OpenAPI_registration_charging_information_t *registration_charging_information);
OpenAPI_registration_charging_information_t *OpenAPI_registration_charging_information_parseFromJSON(cJSON *registration_charging_informationJSON);
cJSON *OpenAPI_registration_charging_information_convertToJSON(OpenAPI_registration_charging_information_t *registration_charging_information);
OpenAPI_registration_charging_information_t *OpenAPI_registration_charging_information_copy(OpenAPI_registration_charging_information_t *dst, OpenAPI_registration_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_charging_information_H_ */

