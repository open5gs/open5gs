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
#include "atsss_capability.h"
#include "interface_upf_info_item.h"
#include "pdu_session_type.h"
#include "snssai_upf_info_item.h"
#include "tai.h"
#include "tngf_info.h"
#include "twif_info.h"
#include "w_agf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_upf_info_s OpenAPI_upf_info_t;
typedef struct OpenAPI_upf_info_s {
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
} OpenAPI_upf_info_t;

OpenAPI_upf_info_t *OpenAPI_upf_info_create(
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
    int data_forwarding
);
void OpenAPI_upf_info_free(OpenAPI_upf_info_t *upf_info);
OpenAPI_upf_info_t *OpenAPI_upf_info_parseFromJSON(cJSON *upf_infoJSON);
cJSON *OpenAPI_upf_info_convertToJSON(OpenAPI_upf_info_t *upf_info);
OpenAPI_upf_info_t *OpenAPI_upf_info_copy(OpenAPI_upf_info_t *dst, OpenAPI_upf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_info_H_ */

