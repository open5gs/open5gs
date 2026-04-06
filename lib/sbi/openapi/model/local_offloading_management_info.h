/*
 * local_offloading_management_info.h
 *
 * Local Offloading Management Information
 */

#ifndef _OpenAPI_local_offloading_management_info_H_
#define _OpenAPI_local_offloading_management_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_local_offloading_management_info_s OpenAPI_local_offloading_management_info_t;
#include "fqdn_pattern_matching_rule.h"
#include "ipv4_address_range.h"
#include "ipv6_address_range.h"
#include "ipv6_prefix_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_local_offloading_management_info_s {
    char *offload_identifier;
    bool is_allowed_traffic;
    int allowed_traffic;
    OpenAPI_list_t *ipv4_address_ranges;
    OpenAPI_list_t *ipv4_addr_masks;
    OpenAPI_list_t *ipv6_address_ranges;
    OpenAPI_list_t *ipv6_prefix_ranges;
    OpenAPI_list_t *fqdn_list;
    OpenAPI_list_t *fqdn_patterns;
};

OpenAPI_local_offloading_management_info_t *OpenAPI_local_offloading_management_info_create(
    char *offload_identifier,
    bool is_allowed_traffic,
    int allowed_traffic,
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv4_addr_masks,
    OpenAPI_list_t *ipv6_address_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges,
    OpenAPI_list_t *fqdn_list,
    OpenAPI_list_t *fqdn_patterns
);
void OpenAPI_local_offloading_management_info_free(OpenAPI_local_offloading_management_info_t *local_offloading_management_info);
OpenAPI_local_offloading_management_info_t *OpenAPI_local_offloading_management_info_parseFromJSON(cJSON *local_offloading_management_infoJSON);
cJSON *OpenAPI_local_offloading_management_info_convertToJSON(OpenAPI_local_offloading_management_info_t *local_offloading_management_info);
OpenAPI_local_offloading_management_info_t *OpenAPI_local_offloading_management_info_copy(OpenAPI_local_offloading_management_info_t *dst, OpenAPI_local_offloading_management_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_local_offloading_management_info_H_ */

