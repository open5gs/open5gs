/*
 * policy_association_update_request.h
 *
 * Represents information that the NF service consumer provides when requesting the update of a policy association. 
 */

#ifndef _OpenAPI_policy_association_update_request_H_
#define _OpenAPI_policy_association_update_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_policy_association_update_request_s OpenAPI_policy_association_update_request_t;
#include "access_type.h"
#include "ambr.h"
#include "guami.h"
#include "mapping_of_snssai.h"
#include "nwdaf_data.h"
#include "partially_allowed_snssai.h"
#include "presence_info.h"
#include "rat_type.h"
#include "request_trigger.h"
#include "service_area_restriction.h"
#include "slice_repl_outcome_info.h"
#include "smf_selection_data.h"
#include "snssai.h"
#include "snssai_part_rejected.h"
#include "snssai_replace_info.h"
#include "trace_data.h"
#include "ue_slice_mbr.h"
#include "user_location.h"
#include "wireline_service_area_restriction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_policy_association_update_request_s {
    char *notification_uri;
    OpenAPI_list_t *alt_notif_ipv4_addrs;
    OpenAPI_list_t *alt_notif_ipv6_addrs;
    OpenAPI_list_t *alt_notif_fqdns;
    OpenAPI_list_t *triggers;
    struct OpenAPI_service_area_restriction_s *serv_area_res;
    struct OpenAPI_wireline_service_area_restriction_s *wl_serv_area_res;
    bool is_rfsp;
    int rfsp;
    bool is_smf_sel_info_null;
    struct OpenAPI_smf_selection_data_s *smf_sel_info;
    struct OpenAPI_ambr_s *ue_ambr;
    OpenAPI_list_t *ue_slice_mbrs;
    OpenAPI_list_t* pra_statuses;
    struct OpenAPI_user_location_s *user_loc;
    OpenAPI_list_t *allowed_snssais;
    OpenAPI_list_t* part_allowed_nssai;
    OpenAPI_list_t* snssais_part_rejected;
    OpenAPI_list_t *rejected_snssais;
    OpenAPI_list_t *pending_nssai;
    OpenAPI_list_t *target_snssais;
    OpenAPI_list_t *mapping_snssais;
    bool is_snssai_repl_infos_null;
    OpenAPI_list_t *snssai_repl_infos;
    OpenAPI_list_t *access_types;
    OpenAPI_list_t *rat_types;
    OpenAPI_list_t *n3g_allowed_snssais;
    OpenAPI_list_t *unavail_snssais;
    bool is_trace_req_null;
    struct OpenAPI_trace_data_s *trace_req;
    struct OpenAPI_guami_s *guami;
    bool is_nwdaf_datas_null;
    OpenAPI_list_t *nwdaf_datas;
    struct OpenAPI_slice_repl_outcome_info_s *af_slice_repl_out;
    bool is_enrg_sav_ind;
    int enrg_sav_ind;
    char *supp_feat;
};

OpenAPI_policy_association_update_request_t *OpenAPI_policy_association_update_request_create(
    char *notification_uri,
    OpenAPI_list_t *alt_notif_ipv4_addrs,
    OpenAPI_list_t *alt_notif_ipv6_addrs,
    OpenAPI_list_t *alt_notif_fqdns,
    OpenAPI_list_t *triggers,
    OpenAPI_service_area_restriction_t *serv_area_res,
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res,
    bool is_rfsp,
    int rfsp,
    bool is_smf_sel_info_null,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_list_t *ue_slice_mbrs,
    OpenAPI_list_t* pra_statuses,
    OpenAPI_user_location_t *user_loc,
    OpenAPI_list_t *allowed_snssais,
    OpenAPI_list_t* part_allowed_nssai,
    OpenAPI_list_t* snssais_part_rejected,
    OpenAPI_list_t *rejected_snssais,
    OpenAPI_list_t *pending_nssai,
    OpenAPI_list_t *target_snssais,
    OpenAPI_list_t *mapping_snssais,
    bool is_snssai_repl_infos_null,
    OpenAPI_list_t *snssai_repl_infos,
    OpenAPI_list_t *access_types,
    OpenAPI_list_t *rat_types,
    OpenAPI_list_t *n3g_allowed_snssais,
    OpenAPI_list_t *unavail_snssais,
    bool is_trace_req_null,
    OpenAPI_trace_data_t *trace_req,
    OpenAPI_guami_t *guami,
    bool is_nwdaf_datas_null,
    OpenAPI_list_t *nwdaf_datas,
    OpenAPI_slice_repl_outcome_info_t *af_slice_repl_out,
    bool is_enrg_sav_ind,
    int enrg_sav_ind,
    char *supp_feat
);
void OpenAPI_policy_association_update_request_free(OpenAPI_policy_association_update_request_t *policy_association_update_request);
OpenAPI_policy_association_update_request_t *OpenAPI_policy_association_update_request_parseFromJSON(cJSON *policy_association_update_requestJSON);
cJSON *OpenAPI_policy_association_update_request_convertToJSON(OpenAPI_policy_association_update_request_t *policy_association_update_request);
OpenAPI_policy_association_update_request_t *OpenAPI_policy_association_update_request_copy(OpenAPI_policy_association_update_request_t *dst, OpenAPI_policy_association_update_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_association_update_request_H_ */

