/*
 * local_offloading_mgt_info_to_ismf.h
 *
 * Information signaled by the SMF to the I-SMF for I-SMF based Local Offloading Management 
 */

#ifndef _OpenAPI_local_offloading_mgt_info_to_ismf_H_
#define _OpenAPI_local_offloading_mgt_info_to_ismf_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_local_offloading_mgt_info_to_ismf_s OpenAPI_local_offloading_mgt_info_to_ismf_t;
#include "ip_address.h"
#include "local_offloading_management_info.h"
#include "traffic_influence_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_local_offloading_mgt_info_to_ismf_s {
    bool is_local_offloading_mgt_allowed_ind;
    int local_offloading_mgt_allowed_ind;
    struct OpenAPI_ip_address_s *dns_addr;
    struct OpenAPI_ip_address_s *psa_upf_addr;
    OpenAPI_list_t *local_offloading_info_list;
    OpenAPI_list_t *offload_ids;
    struct OpenAPI_traffic_influence_info_s *traffic_influ_info;
};

OpenAPI_local_offloading_mgt_info_to_ismf_t *OpenAPI_local_offloading_mgt_info_to_ismf_create(
    bool is_local_offloading_mgt_allowed_ind,
    int local_offloading_mgt_allowed_ind,
    OpenAPI_ip_address_t *dns_addr,
    OpenAPI_ip_address_t *psa_upf_addr,
    OpenAPI_list_t *local_offloading_info_list,
    OpenAPI_list_t *offload_ids,
    OpenAPI_traffic_influence_info_t *traffic_influ_info
);
void OpenAPI_local_offloading_mgt_info_to_ismf_free(OpenAPI_local_offloading_mgt_info_to_ismf_t *local_offloading_mgt_info_to_ismf);
OpenAPI_local_offloading_mgt_info_to_ismf_t *OpenAPI_local_offloading_mgt_info_to_ismf_parseFromJSON(cJSON *local_offloading_mgt_info_to_ismfJSON);
cJSON *OpenAPI_local_offloading_mgt_info_to_ismf_convertToJSON(OpenAPI_local_offloading_mgt_info_to_ismf_t *local_offloading_mgt_info_to_ismf);
OpenAPI_local_offloading_mgt_info_to_ismf_t *OpenAPI_local_offloading_mgt_info_to_ismf_copy(OpenAPI_local_offloading_mgt_info_to_ismf_t *dst, OpenAPI_local_offloading_mgt_info_to_ismf_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_local_offloading_mgt_info_to_ismf_H_ */

