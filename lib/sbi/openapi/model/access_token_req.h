/*
 * access_token_req.h
 *
 * Contains information related to the access token request
 */

#ifndef _OpenAPI_access_token_req_H_
#define _OpenAPI_access_token_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_token_req_s OpenAPI_access_token_req_t;
typedef enum { OpenAPI_access_token_req_GRANTTYPE_NULL = 0, OpenAPI_access_token_req_GRANTTYPE_client_credentials } OpenAPI_access_token_req_grant_type_e;

char* OpenAPI_access_token_req_grant_type_ToString(OpenAPI_access_token_req_grant_type_e grant_type);

OpenAPI_access_token_req_grant_type_e OpenAPI_access_token_req_grant_type_FromString(char* grant_type);
typedef struct OpenAPI_access_token_req_s {
    OpenAPI_access_token_req_grant_type_e grant_type;
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
    OpenAPI_list_t *target_snssai_list;
    OpenAPI_list_t *target_nsi_list;
    char *target_nf_set_id;
    char *target_nf_service_set_id;
} OpenAPI_access_token_req_t;

OpenAPI_access_token_req_t *OpenAPI_access_token_req_create(
    OpenAPI_access_token_req_grant_type_e grant_type,
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
    OpenAPI_list_t *target_snssai_list,
    OpenAPI_list_t *target_nsi_list,
    char *target_nf_set_id,
    char *target_nf_service_set_id
);
void OpenAPI_access_token_req_free(OpenAPI_access_token_req_t *access_token_req);
OpenAPI_access_token_req_t *OpenAPI_access_token_req_parseFromJSON(cJSON *access_token_reqJSON);
cJSON *OpenAPI_access_token_req_convertToJSON(OpenAPI_access_token_req_t *access_token_req);
OpenAPI_access_token_req_t *OpenAPI_access_token_req_copy(OpenAPI_access_token_req_t *dst, OpenAPI_access_token_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_token_req_H_ */

