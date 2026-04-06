/*
 * access_token_req_1.h
 *
 * Contains information related to the access token request
 */

#ifndef _OpenAPI_access_token_req_1_H_
#define _OpenAPI_access_token_req_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_access_token_req_1_s OpenAPI_access_token_req_1_t;
#include "ml_model_inter_ind_1.h"
#include "nf_type.h"
#include "nwdaf_event.h"
#include "plmn_id.h"
#include "plmn_id_nid_1.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_access_token_req_1_GRANTTYPE_NULL = 0, OpenAPI_access_token_req_1_GRANTTYPE_client_credentials } OpenAPI_access_token_req_1_grant_type_e;

char* OpenAPI_access_token_req_1_grant_type_ToString(OpenAPI_access_token_req_1_grant_type_e grant_type);

OpenAPI_access_token_req_1_grant_type_e OpenAPI_access_token_req_1_grant_type_FromString(char* grant_type);
struct OpenAPI_access_token_req_1_s {
    OpenAPI_access_token_req_1_grant_type_e grant_type;
    char *nf_instance_id;
    OpenAPI_nf_type_e nf_type;
    OpenAPI_nf_type_e target_nf_type;
    char *scope;
    char *target_nf_instance_id;
    struct OpenAPI_plmn_id_s *requester_plmn;
    OpenAPI_list_t *requester_plmn_list;
    OpenAPI_list_t *requester_snssai_list;
    char *requester_fqdn;
    OpenAPI_list_t *requester_snpn_list;
    struct OpenAPI_plmn_id_s *target_plmn;
    struct OpenAPI_plmn_id_nid_1_s *target_snpn;
    OpenAPI_list_t *target_snssai_list;
    OpenAPI_list_t *target_nsi_list;
    char *target_nf_set_id;
    char *target_nf_service_set_id;
    char *hnrf_access_token_uri;
    char *source_nf_instance_id;
    char *vendor_id;
    OpenAPI_list_t *analytics_ids;
    OpenAPI_list_t *requester_inter_ind_list;
    char *source_vendor_id;
    char *af_id;
};

OpenAPI_access_token_req_1_t *OpenAPI_access_token_req_1_create(
    OpenAPI_access_token_req_1_grant_type_e grant_type,
    char *nf_instance_id,
    OpenAPI_nf_type_e nf_type,
    OpenAPI_nf_type_e target_nf_type,
    char *scope,
    char *target_nf_instance_id,
    OpenAPI_plmn_id_t *requester_plmn,
    OpenAPI_list_t *requester_plmn_list,
    OpenAPI_list_t *requester_snssai_list,
    char *requester_fqdn,
    OpenAPI_list_t *requester_snpn_list,
    OpenAPI_plmn_id_t *target_plmn,
    OpenAPI_plmn_id_nid_1_t *target_snpn,
    OpenAPI_list_t *target_snssai_list,
    OpenAPI_list_t *target_nsi_list,
    char *target_nf_set_id,
    char *target_nf_service_set_id,
    char *hnrf_access_token_uri,
    char *source_nf_instance_id,
    char *vendor_id,
    OpenAPI_list_t *analytics_ids,
    OpenAPI_list_t *requester_inter_ind_list,
    char *source_vendor_id,
    char *af_id
);
void OpenAPI_access_token_req_1_free(OpenAPI_access_token_req_1_t *access_token_req_1);
OpenAPI_access_token_req_1_t *OpenAPI_access_token_req_1_parseFromJSON(cJSON *access_token_req_1JSON);
cJSON *OpenAPI_access_token_req_1_convertToJSON(OpenAPI_access_token_req_1_t *access_token_req_1);
OpenAPI_access_token_req_1_t *OpenAPI_access_token_req_1_copy(OpenAPI_access_token_req_1_t *dst, OpenAPI_access_token_req_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_token_req_1_H_ */

