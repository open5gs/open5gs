/*
 * hrsbo_info_from_vplmn.h
 *
 * HR-SBO Information in request message
 */

#ifndef _OpenAPI_hrsbo_info_from_vplmn_H_
#define _OpenAPI_hrsbo_info_from_vplmn_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_hrsbo_info_from_vplmn_s OpenAPI_hrsbo_info_from_vplmn_t;
#include "eas_info_to_refresh.h"
#include "ip_address.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_hrsbo_info_from_vplmn_s {
    bool is_hrsbo_auth_req_ind;
    int hrsbo_auth_req_ind;
    struct OpenAPI_ip_address_s *v_easdf_addr;
    struct OpenAPI_ip_address_s *local_dns_addr;
    bool is_eas_rediscovery_ind;
    int eas_rediscovery_ind;
    struct OpenAPI_eas_info_to_refresh_s *eas_info_to_refresh;
    OpenAPI_list_t *stored_offload_ids;
    bool is_eas_ip_repl_support_ind;
    int eas_ip_repl_support_ind;
    OpenAPI_list_t *v_easdf_security_info;
    OpenAPI_list_t *local_dns_security_info;
};

OpenAPI_hrsbo_info_from_vplmn_t *OpenAPI_hrsbo_info_from_vplmn_create(
    bool is_hrsbo_auth_req_ind,
    int hrsbo_auth_req_ind,
    OpenAPI_ip_address_t *v_easdf_addr,
    OpenAPI_ip_address_t *local_dns_addr,
    bool is_eas_rediscovery_ind,
    int eas_rediscovery_ind,
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh,
    OpenAPI_list_t *stored_offload_ids,
    bool is_eas_ip_repl_support_ind,
    int eas_ip_repl_support_ind,
    OpenAPI_list_t *v_easdf_security_info,
    OpenAPI_list_t *local_dns_security_info
);
void OpenAPI_hrsbo_info_from_vplmn_free(OpenAPI_hrsbo_info_from_vplmn_t *hrsbo_info_from_vplmn);
OpenAPI_hrsbo_info_from_vplmn_t *OpenAPI_hrsbo_info_from_vplmn_parseFromJSON(cJSON *hrsbo_info_from_vplmnJSON);
cJSON *OpenAPI_hrsbo_info_from_vplmn_convertToJSON(OpenAPI_hrsbo_info_from_vplmn_t *hrsbo_info_from_vplmn);
OpenAPI_hrsbo_info_from_vplmn_t *OpenAPI_hrsbo_info_from_vplmn_copy(OpenAPI_hrsbo_info_from_vplmn_t *dst, OpenAPI_hrsbo_info_from_vplmn_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hrsbo_info_from_vplmn_H_ */

