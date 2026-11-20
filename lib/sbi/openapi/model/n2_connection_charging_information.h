/*
 * n2_connection_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_n2_connection_charging_information_H_
#define _OpenAPI_n2_connection_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_n2_connection_charging_information_s OpenAPI_n2_connection_charging_information_t;
#include "area.h"
#include "charging_user_location.h"
#include "core_network_type.h"
#include "global_ran_node_id.h"
#include "nssai_map.h"
#include "ps_cell_information.h"
#include "rat_type.h"
#include "service_area_restriction.h"
#include "snssai.h"
#include "user_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_n2_connection_charging_information_s {
    int n2_connection_message_type;
    struct OpenAPI_user_information_s *user_information;
    struct OpenAPI_charging_user_location_s *user_locationinfo;
    struct OpenAPI_ps_cell_information_s *p_s_cell_information;
    char *uetime_zone;
    OpenAPI_rat_type_e r_at_type;
    bool is_amf_ue_ngap_id;
    int amf_ue_ngap_id;
    bool is_ran_ue_ngap_id;
    int ran_ue_ngap_id;
    struct OpenAPI_global_ran_node_id_s *ran_node_id;
    OpenAPI_list_t *restricted_rat_list;
    OpenAPI_list_t *forbidden_area_list;
    OpenAPI_list_t *service_area_restriction;
    OpenAPI_list_t *restricted_cn_list;
    OpenAPI_list_t *allowed_nssai;
    OpenAPI_list_t *n_ssai_map_list;
    char *rrc_est_cause;
    bool is_satellite_access_indicator;
    int satellite_access_indicator;
};

OpenAPI_n2_connection_charging_information_t *OpenAPI_n2_connection_charging_information_create(
    int n2_connection_message_type,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_locationinfo,
    OpenAPI_ps_cell_information_t *p_s_cell_information,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    bool is_amf_ue_ngap_id,
    int amf_ue_ngap_id,
    bool is_ran_ue_ngap_id,
    int ran_ue_ngap_id,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    OpenAPI_list_t *restricted_rat_list,
    OpenAPI_list_t *forbidden_area_list,
    OpenAPI_list_t *service_area_restriction,
    OpenAPI_list_t *restricted_cn_list,
    OpenAPI_list_t *allowed_nssai,
    OpenAPI_list_t *n_ssai_map_list,
    char *rrc_est_cause,
    bool is_satellite_access_indicator,
    int satellite_access_indicator
);
void OpenAPI_n2_connection_charging_information_free(OpenAPI_n2_connection_charging_information_t *n2_connection_charging_information);
OpenAPI_n2_connection_charging_information_t *OpenAPI_n2_connection_charging_information_parseFromJSON(cJSON *n2_connection_charging_informationJSON);
cJSON *OpenAPI_n2_connection_charging_information_convertToJSON(OpenAPI_n2_connection_charging_information_t *n2_connection_charging_information);
OpenAPI_n2_connection_charging_information_t *OpenAPI_n2_connection_charging_information_copy(OpenAPI_n2_connection_charging_information_t *dst, OpenAPI_n2_connection_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_connection_charging_information_H_ */

