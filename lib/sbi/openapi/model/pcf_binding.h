/*
 * pcf_binding.h
 *
 * Identifies an Individual PCF for a PDU Session binding.
 */

#ifndef _OpenAPI_pcf_binding_H_
#define _OpenAPI_pcf_binding_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "binding_level.h"
#include "ip_end_point.h"
#include "parameter_combination.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcf_binding_s OpenAPI_pcf_binding_t;
typedef struct OpenAPI_pcf_binding_s {
    char *supi;
    char *gpsi;
    char *ipv4_addr;
    char *ipv6_prefix;
    OpenAPI_list_t *add_ipv6_prefixes;
    char *ip_domain;
    char *mac_addr48;
    OpenAPI_list_t *add_mac_addrs;
    char *dnn;
    char *pcf_fqdn;
    OpenAPI_list_t *pcf_ip_end_points;
    char *pcf_diam_host;
    char *pcf_diam_realm;
    char *pcf_sm_fqdn;
    OpenAPI_list_t *pcf_sm_ip_end_points;
    struct OpenAPI_snssai_s *snssai;
    char *supp_feat;
    char *pcf_id;
    char *pcf_set_id;
    char *recovery_time;
    struct OpenAPI_parameter_combination_s *para_com;
    OpenAPI_binding_level_e bind_level;
    OpenAPI_list_t *ipv4_frame_route_list;
    OpenAPI_list_t *ipv6_frame_route_list;
} OpenAPI_pcf_binding_t;

OpenAPI_pcf_binding_t *OpenAPI_pcf_binding_create(
    char *supi,
    char *gpsi,
    char *ipv4_addr,
    char *ipv6_prefix,
    OpenAPI_list_t *add_ipv6_prefixes,
    char *ip_domain,
    char *mac_addr48,
    OpenAPI_list_t *add_mac_addrs,
    char *dnn,
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_diam_host,
    char *pcf_diam_realm,
    char *pcf_sm_fqdn,
    OpenAPI_list_t *pcf_sm_ip_end_points,
    OpenAPI_snssai_t *snssai,
    char *supp_feat,
    char *pcf_id,
    char *pcf_set_id,
    char *recovery_time,
    OpenAPI_parameter_combination_t *para_com,
    OpenAPI_binding_level_e bind_level,
    OpenAPI_list_t *ipv4_frame_route_list,
    OpenAPI_list_t *ipv6_frame_route_list
);
void OpenAPI_pcf_binding_free(OpenAPI_pcf_binding_t *pcf_binding);
OpenAPI_pcf_binding_t *OpenAPI_pcf_binding_parseFromJSON(cJSON *pcf_bindingJSON);
cJSON *OpenAPI_pcf_binding_convertToJSON(OpenAPI_pcf_binding_t *pcf_binding);
OpenAPI_pcf_binding_t *OpenAPI_pcf_binding_copy(OpenAPI_pcf_binding_t *dst, OpenAPI_pcf_binding_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcf_binding_H_ */

