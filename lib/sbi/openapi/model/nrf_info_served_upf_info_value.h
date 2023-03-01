/*
 * nrf_info_served_upf_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_upf_info_value_H_
#define _OpenAPI_nrf_info_served_upf_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "atsss_capability.h"
#include "interface_upf_info_item.h"
#include "pdu_session_type.h"
#include "snssai_upf_info_item.h"
#include "tai.h"
#include "tai_range.h"
#include "tngf_info.h"
#include "twif_info.h"
#include "upf_info.h"
#include "w_agf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_upf_info_value_s OpenAPI_nrf_info_served_upf_info_value_t;
typedef struct OpenAPI_nrf_info_served_upf_info_value_s {
    OpenAPI_list_t *s_nssai_upf_info_list;
    OpenAPI_list_t *smf_serving_area;
    OpenAPI_list_t *interface_upf_info_list;
    bool is_iwk_eps_ind;
    int iwk_eps_ind;
    OpenAPI_list_t *pdu_session_types;
    struct OpenAPI_atsss_capability_s *atsss_capability;
    bool is_ue_ip_addr_ind;
    int ue_ip_addr_ind;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    struct OpenAPI_w_agf_info_s *w_agf_info;
    struct OpenAPI_tngf_info_s *tngf_info;
    struct OpenAPI_twif_info_s *twif_info;
    bool is_priority;
    int priority;
    bool is_redundant_gtpu;
    int redundant_gtpu;
    bool is_ipups;
    int ipups;
    bool is_data_forwarding;
    int data_forwarding;
    char *supported_pfcp_features;
} OpenAPI_nrf_info_served_upf_info_value_t;

OpenAPI_nrf_info_served_upf_info_value_t *OpenAPI_nrf_info_served_upf_info_value_create(
    OpenAPI_list_t *s_nssai_upf_info_list,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *interface_upf_info_list,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    OpenAPI_list_t *pdu_session_types,
    OpenAPI_atsss_capability_t *atsss_capability,
    bool is_ue_ip_addr_ind,
    int ue_ip_addr_ind,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_w_agf_info_t *w_agf_info,
    OpenAPI_tngf_info_t *tngf_info,
    OpenAPI_twif_info_t *twif_info,
    bool is_priority,
    int priority,
    bool is_redundant_gtpu,
    int redundant_gtpu,
    bool is_ipups,
    int ipups,
    bool is_data_forwarding,
    int data_forwarding,
    char *supported_pfcp_features
);
void OpenAPI_nrf_info_served_upf_info_value_free(OpenAPI_nrf_info_served_upf_info_value_t *nrf_info_served_upf_info_value);
OpenAPI_nrf_info_served_upf_info_value_t *OpenAPI_nrf_info_served_upf_info_value_parseFromJSON(cJSON *nrf_info_served_upf_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_upf_info_value_convertToJSON(OpenAPI_nrf_info_served_upf_info_value_t *nrf_info_served_upf_info_value);
OpenAPI_nrf_info_served_upf_info_value_t *OpenAPI_nrf_info_served_upf_info_value_copy(OpenAPI_nrf_info_served_upf_info_value_t *dst, OpenAPI_nrf_info_served_upf_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_upf_info_value_H_ */

