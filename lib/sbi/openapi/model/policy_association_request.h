/*
 * policy_association_request.h
 *
 * Information which the NF service consumer provides when requesting the creation of a policy association. The serviveName property corresponds to the serviceName in the main body of the specification.
 */

#ifndef _OpenAPI_policy_association_request_H_
#define _OpenAPI_policy_association_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "ambr.h"
#include "guami.h"
#include "mapping_of_snssai.h"
#include "plmn_id_nid.h"
#include "rat_type.h"
#include "service_area_restriction.h"
#include "snssai.h"
#include "trace_data.h"
#include "user_location.h"
#include "wireline_service_area_restriction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_policy_association_request_s OpenAPI_policy_association_request_t;
typedef struct OpenAPI_policy_association_request_s {
    char *notification_uri;
    OpenAPI_list_t *alt_notif_ipv4_addrs;
    OpenAPI_list_t *alt_notif_ipv6_addrs;
    char *supi;
    char *gpsi;
    OpenAPI_access_type_e access_type;
    OpenAPI_list_t *access_types;
    char *pei;
    struct OpenAPI_user_location_s *user_loc;
    char *time_zone;
    struct OpenAPI_plmn_id_nid_s *serving_plmn;
    OpenAPI_rat_type_e rat_type;
    OpenAPI_list_t *rat_types;
    OpenAPI_list_t *group_ids;
    struct OpenAPI_service_area_restriction_s *serv_area_res;
    struct OpenAPI_wireline_service_area_restriction_s *wl_serv_area_res;
    int rfsp;
    struct OpenAPI_ambr_s *ue_ambr;
    struct OpenAPI_ambr_s *rg_tmbr;
    OpenAPI_list_t *allowed_snssais;
    OpenAPI_list_t *mapping_snssais;
    OpenAPI_list_t *n3g_allowed_snssais;
    struct OpenAPI_guami_s *guami;
    char *servive_name;
    struct OpenAPI_trace_data_s *trace_req;
    char *supp_feat;
} OpenAPI_policy_association_request_t;

OpenAPI_policy_association_request_t *OpenAPI_policy_association_request_create(
    char *notification_uri,
    OpenAPI_list_t *alt_notif_ipv4_addrs,
    OpenAPI_list_t *alt_notif_ipv6_addrs,
    char *supi,
    char *gpsi,
    OpenAPI_access_type_e access_type,
    OpenAPI_list_t *access_types,
    char *pei,
    OpenAPI_user_location_t *user_loc,
    char *time_zone,
    OpenAPI_plmn_id_nid_t *serving_plmn,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_list_t *rat_types,
    OpenAPI_list_t *group_ids,
    OpenAPI_service_area_restriction_t *serv_area_res,
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res,
    int rfsp,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_ambr_t *rg_tmbr,
    OpenAPI_list_t *allowed_snssais,
    OpenAPI_list_t *mapping_snssais,
    OpenAPI_list_t *n3g_allowed_snssais,
    OpenAPI_guami_t *guami,
    char *servive_name,
    OpenAPI_trace_data_t *trace_req,
    char *supp_feat
    );
void OpenAPI_policy_association_request_free(OpenAPI_policy_association_request_t *policy_association_request);
OpenAPI_policy_association_request_t *OpenAPI_policy_association_request_parseFromJSON(cJSON *policy_association_requestJSON);
cJSON *OpenAPI_policy_association_request_convertToJSON(OpenAPI_policy_association_request_t *policy_association_request);
OpenAPI_policy_association_request_t *OpenAPI_policy_association_request_copy(OpenAPI_policy_association_request_t *dst, OpenAPI_policy_association_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_association_request_H_ */

