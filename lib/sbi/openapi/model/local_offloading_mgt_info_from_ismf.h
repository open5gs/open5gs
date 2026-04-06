/*
 * local_offloading_mgt_info_from_ismf.h
 *
 * Information signaled by the I-SMF to the SMF for I-SMF based Local Offloading Management 
 */

#ifndef _OpenAPI_local_offloading_mgt_info_from_ismf_H_
#define _OpenAPI_local_offloading_mgt_info_from_ismf_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_local_offloading_mgt_info_from_ismf_s OpenAPI_local_offloading_mgt_info_from_ismf_t;
#include "eas_info_to_refresh.h"
#include "ip_address.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_local_offloading_mgt_info_from_ismf_s {
    bool is_local_offloading_mgt_allowed_ind;
    int local_offloading_mgt_allowed_ind;
    struct OpenAPI_ip_address_s *easdf_addr;
    OpenAPI_list_t *easdf_security_info;
    bool is_eas_rediscovery_ind;
    int eas_rediscovery_ind;
    struct OpenAPI_eas_info_to_refresh_s *eas_info_to_refresh;
    OpenAPI_list_t *stored_offload_ids;
};

OpenAPI_local_offloading_mgt_info_from_ismf_t *OpenAPI_local_offloading_mgt_info_from_ismf_create(
    bool is_local_offloading_mgt_allowed_ind,
    int local_offloading_mgt_allowed_ind,
    OpenAPI_ip_address_t *easdf_addr,
    OpenAPI_list_t *easdf_security_info,
    bool is_eas_rediscovery_ind,
    int eas_rediscovery_ind,
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh,
    OpenAPI_list_t *stored_offload_ids
);
void OpenAPI_local_offloading_mgt_info_from_ismf_free(OpenAPI_local_offloading_mgt_info_from_ismf_t *local_offloading_mgt_info_from_ismf);
OpenAPI_local_offloading_mgt_info_from_ismf_t *OpenAPI_local_offloading_mgt_info_from_ismf_parseFromJSON(cJSON *local_offloading_mgt_info_from_ismfJSON);
cJSON *OpenAPI_local_offloading_mgt_info_from_ismf_convertToJSON(OpenAPI_local_offloading_mgt_info_from_ismf_t *local_offloading_mgt_info_from_ismf);
OpenAPI_local_offloading_mgt_info_from_ismf_t *OpenAPI_local_offloading_mgt_info_from_ismf_copy(OpenAPI_local_offloading_mgt_info_from_ismf_t *dst, OpenAPI_local_offloading_mgt_info_from_ismf_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_local_offloading_mgt_info_from_ismf_H_ */

