/*
 * scp_info.h
 *
 * Information of an SCP Instance
 */

#ifndef _OpenAPI_scp_info_H_
#define _OpenAPI_scp_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_reachability.h"
#include "ipv4_address_range.h"
#include "ipv6_prefix_range.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "scp_capability.h"
#include "scp_domain_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scp_info_s OpenAPI_scp_info_t;
typedef struct OpenAPI_scp_info_s {
    OpenAPI_list_t* scp_domain_info_list;
    char *scp_prefix;
    OpenAPI_list_t* scp_ports;
    OpenAPI_list_t *address_domains;
    OpenAPI_list_t *ipv4_addresses;
    OpenAPI_list_t *ipv6_prefixes;
    OpenAPI_list_t *ipv4_addr_ranges;
    OpenAPI_list_t *ipv6_prefix_ranges;
    OpenAPI_list_t *served_nf_set_id_list;
    OpenAPI_list_t *remote_plmn_list;
    OpenAPI_list_t *remote_snpn_list;
    OpenAPI_ip_reachability_e ip_reachability;
    OpenAPI_list_t *scp_capabilities;
} OpenAPI_scp_info_t;

OpenAPI_scp_info_t *OpenAPI_scp_info_create(
    OpenAPI_list_t* scp_domain_info_list,
    char *scp_prefix,
    OpenAPI_list_t* scp_ports,
    OpenAPI_list_t *address_domains,
    OpenAPI_list_t *ipv4_addresses,
    OpenAPI_list_t *ipv6_prefixes,
    OpenAPI_list_t *ipv4_addr_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges,
    OpenAPI_list_t *served_nf_set_id_list,
    OpenAPI_list_t *remote_plmn_list,
    OpenAPI_list_t *remote_snpn_list,
    OpenAPI_ip_reachability_e ip_reachability,
    OpenAPI_list_t *scp_capabilities
);
void OpenAPI_scp_info_free(OpenAPI_scp_info_t *scp_info);
OpenAPI_scp_info_t *OpenAPI_scp_info_parseFromJSON(cJSON *scp_infoJSON);
cJSON *OpenAPI_scp_info_convertToJSON(OpenAPI_scp_info_t *scp_info);
OpenAPI_scp_info_t *OpenAPI_scp_info_copy(OpenAPI_scp_info_t *dst, OpenAPI_scp_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scp_info_H_ */

