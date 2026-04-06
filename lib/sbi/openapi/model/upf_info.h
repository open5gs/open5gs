/*
 * upf_info.h
 *
 * Information of an UPF NF Instance
 */

#ifndef _OpenAPI_upf_info_H_
#define _OpenAPI_upf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_upf_info_s OpenAPI_upf_info_t;
#include "atsss_capability.h"
#include "delay_measurement_protocol.h"
#include "epdg_info.h"
#include "event_type.h"
#include "interface_upf_info_item.h"
#include "model_2_g3_g_location_area.h"
#include "model_2_g3_g_location_area_range.h"
#include "pdu_session_type.h"
#include "snssai_upf_info_item.h"
#include "tai.h"
#include "tai_range.h"
#include "tngf_info.h"
#include "twif_info.h"
#include "upf_packet_inspection_functionality.h"
#include "w_agf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_upf_info_s {
    OpenAPI_list_t *s_nssai_upf_info_list;
    OpenAPI_list_t *smf_serving_area;
    OpenAPI_list_t *interface_upf_info_list;
    OpenAPI_list_t* n6_tunnel_info_list;
    bool is_iwk_eps_ind;
    int iwk_eps_ind;
    bool is_sxa_ind;
    int sxa_ind;
    OpenAPI_list_t *pdu_session_types;
    struct OpenAPI_atsss_capability_s *atsss_capability;
    bool is_ue_ip_addr_ind;
    int ue_ip_addr_ind;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    bool is_w_agf_info_null;
    struct OpenAPI_w_agf_info_s *w_agf_info;
    bool is_tngf_info_null;
    struct OpenAPI_tngf_info_s *tngf_info;
    bool is_twif_info_null;
    struct OpenAPI_twif_info_s *twif_info;
    OpenAPI_list_t *preferred_epdg_info_list;
    OpenAPI_list_t *preferred_w_agf_info_list;
    OpenAPI_list_t *preferred_tngf_info_list;
    OpenAPI_list_t *preferred_twif_info_list;
    bool is_priority;
    int priority;
    bool is_redundant_gtpu;
    int redundant_gtpu;
    bool is_ipups;
    int ipups;
    bool is_data_forwarding;
    int data_forwarding;
    char *supported_pfcp_features;
    OpenAPI_list_t *upf_events;
    OpenAPI_list_t *op_config_caps;
    OpenAPI_list_t *packet_inspection_functionalities;
    OpenAPI_list_t *n6_delay_meast_protocs;
    bool is_geran_utran_ind;
    int geran_utran_ind;
    OpenAPI_list_t *_2g3g_location_area_list;
    OpenAPI_list_t *_2g3g_location_area_range_list;
};

OpenAPI_upf_info_t *OpenAPI_upf_info_create(
    OpenAPI_list_t *s_nssai_upf_info_list,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *interface_upf_info_list,
    OpenAPI_list_t* n6_tunnel_info_list,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    bool is_sxa_ind,
    int sxa_ind,
    OpenAPI_list_t *pdu_session_types,
    OpenAPI_atsss_capability_t *atsss_capability,
    bool is_ue_ip_addr_ind,
    int ue_ip_addr_ind,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    bool is_w_agf_info_null,
    OpenAPI_w_agf_info_t *w_agf_info,
    bool is_tngf_info_null,
    OpenAPI_tngf_info_t *tngf_info,
    bool is_twif_info_null,
    OpenAPI_twif_info_t *twif_info,
    OpenAPI_list_t *preferred_epdg_info_list,
    OpenAPI_list_t *preferred_w_agf_info_list,
    OpenAPI_list_t *preferred_tngf_info_list,
    OpenAPI_list_t *preferred_twif_info_list,
    bool is_priority,
    int priority,
    bool is_redundant_gtpu,
    int redundant_gtpu,
    bool is_ipups,
    int ipups,
    bool is_data_forwarding,
    int data_forwarding,
    char *supported_pfcp_features,
    OpenAPI_list_t *upf_events,
    OpenAPI_list_t *op_config_caps,
    OpenAPI_list_t *packet_inspection_functionalities,
    OpenAPI_list_t *n6_delay_meast_protocs,
    bool is_geran_utran_ind,
    int geran_utran_ind,
    OpenAPI_list_t *_2g3g_location_area_list,
    OpenAPI_list_t *_2g3g_location_area_range_list
);
void OpenAPI_upf_info_free(OpenAPI_upf_info_t *upf_info);
OpenAPI_upf_info_t *OpenAPI_upf_info_parseFromJSON(cJSON *upf_infoJSON);
cJSON *OpenAPI_upf_info_convertToJSON(OpenAPI_upf_info_t *upf_info);
OpenAPI_upf_info_t *OpenAPI_upf_info_copy(OpenAPI_upf_info_t *dst, OpenAPI_upf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_info_H_ */

