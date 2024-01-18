/*
 * pcf_binding_patch.h
 *
 * Identifies an Individual PCF binding used in an HTTP Patch method.
 */

#ifndef _OpenAPI_pcf_binding_patch_H_
#define _OpenAPI_pcf_binding_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_end_point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_binding_patch_s OpenAPI_pcf_binding_patch_t;
typedef struct OpenAPI_pcf_binding_patch_s {
    bool is_ipv4_addr_null;
    char *ipv4_addr;
    bool is_ip_domain_null;
    char *ip_domain;
    bool is_ipv6_prefix_null;
    char *ipv6_prefix;
    bool is_add_ipv6_prefixes_null;
    OpenAPI_list_t *add_ipv6_prefixes;
    bool is_mac_addr48_null;
    char *mac_addr48;
    bool is_add_mac_addrs_null;
    OpenAPI_list_t *add_mac_addrs;
    char *pcf_id;
    char *pcf_fqdn;
    OpenAPI_list_t *pcf_ip_end_points;
    char *pcf_diam_host;
    char *pcf_diam_realm;
} OpenAPI_pcf_binding_patch_t;

OpenAPI_pcf_binding_patch_t *OpenAPI_pcf_binding_patch_create(
    bool is_ipv4_addr_null,
    char *ipv4_addr,
    bool is_ip_domain_null,
    char *ip_domain,
    bool is_ipv6_prefix_null,
    char *ipv6_prefix,
    bool is_add_ipv6_prefixes_null,
    OpenAPI_list_t *add_ipv6_prefixes,
    bool is_mac_addr48_null,
    char *mac_addr48,
    bool is_add_mac_addrs_null,
    OpenAPI_list_t *add_mac_addrs,
    char *pcf_id,
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_diam_host,
    char *pcf_diam_realm
);
void OpenAPI_pcf_binding_patch_free(OpenAPI_pcf_binding_patch_t *pcf_binding_patch);
OpenAPI_pcf_binding_patch_t *OpenAPI_pcf_binding_patch_parseFromJSON(cJSON *pcf_binding_patchJSON);
cJSON *OpenAPI_pcf_binding_patch_convertToJSON(OpenAPI_pcf_binding_patch_t *pcf_binding_patch);
OpenAPI_pcf_binding_patch_t *OpenAPI_pcf_binding_patch_copy(OpenAPI_pcf_binding_patch_t *dst, OpenAPI_pcf_binding_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_binding_patch_H_ */

