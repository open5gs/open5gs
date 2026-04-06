/*
 * vplmn_offloading_info.h
 *
 * VPLMN Specific Offloading Information
 */

#ifndef _OpenAPI_vplmn_offloading_info_H_
#define _OpenAPI_vplmn_offloading_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vplmn_offloading_info_s OpenAPI_vplmn_offloading_info_t;
#include "fqdn_pattern_matching_rule.h"
#include "ipv4_address_range.h"
#include "ipv6_address_range.h"
#include "ipv6_prefix_range.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vplmn_offloading_info_s {
    char *offload_identifier;
    struct OpenAPI_plmn_id_s *vplmn_id;
    bool is_allowed_traffic;
    int allowed_traffic;
    OpenAPI_list_t *ipv4_address_ranges;
    OpenAPI_list_t *ipv4_addr_masks;
    OpenAPI_list_t *ipv6_address_ranges;
    OpenAPI_list_t *ipv6_prefix_ranges;
    OpenAPI_list_t *fqdn_list;
    OpenAPI_list_t *fqdn_patterns;
};

OpenAPI_vplmn_offloading_info_t *OpenAPI_vplmn_offloading_info_create(
    char *offload_identifier,
    OpenAPI_plmn_id_t *vplmn_id,
    bool is_allowed_traffic,
    int allowed_traffic,
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv4_addr_masks,
    OpenAPI_list_t *ipv6_address_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges,
    OpenAPI_list_t *fqdn_list,
    OpenAPI_list_t *fqdn_patterns
);
void OpenAPI_vplmn_offloading_info_free(OpenAPI_vplmn_offloading_info_t *vplmn_offloading_info);
OpenAPI_vplmn_offloading_info_t *OpenAPI_vplmn_offloading_info_parseFromJSON(cJSON *vplmn_offloading_infoJSON);
cJSON *OpenAPI_vplmn_offloading_info_convertToJSON(OpenAPI_vplmn_offloading_info_t *vplmn_offloading_info);
OpenAPI_vplmn_offloading_info_t *OpenAPI_vplmn_offloading_info_copy(OpenAPI_vplmn_offloading_info_t *dst, OpenAPI_vplmn_offloading_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vplmn_offloading_info_H_ */

