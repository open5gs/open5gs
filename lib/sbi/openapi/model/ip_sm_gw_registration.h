/*
 * ip_sm_gw_registration.h
 *
 * 
 */

#ifndef _OpenAPI_ip_sm_gw_registration_H_
#define _OpenAPI_ip_sm_gw_registration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_node_diameter_address.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ip_sm_gw_registration_s OpenAPI_ip_sm_gw_registration_t;
typedef struct OpenAPI_ip_sm_gw_registration_s {
    char *ip_sm_gw_map_address;
    struct OpenAPI_network_node_diameter_address_s *ip_sm_gw_diameter_address;
    char *ipsmgw_ipv4;
    char *ipsmgw_ipv6;
    char *ipsmgw_fqdn;
    char *nf_instance_id;
    bool is_unri_indicator;
    int unri_indicator;
    OpenAPI_list_t *reset_ids;
    bool is_ip_sm_gw_sbi_sup_ind;
    int ip_sm_gw_sbi_sup_ind;
} OpenAPI_ip_sm_gw_registration_t;

OpenAPI_ip_sm_gw_registration_t *OpenAPI_ip_sm_gw_registration_create(
    char *ip_sm_gw_map_address,
    OpenAPI_network_node_diameter_address_t *ip_sm_gw_diameter_address,
    char *ipsmgw_ipv4,
    char *ipsmgw_ipv6,
    char *ipsmgw_fqdn,
    char *nf_instance_id,
    bool is_unri_indicator,
    int unri_indicator,
    OpenAPI_list_t *reset_ids,
    bool is_ip_sm_gw_sbi_sup_ind,
    int ip_sm_gw_sbi_sup_ind
);
void OpenAPI_ip_sm_gw_registration_free(OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration);
OpenAPI_ip_sm_gw_registration_t *OpenAPI_ip_sm_gw_registration_parseFromJSON(cJSON *ip_sm_gw_registrationJSON);
cJSON *OpenAPI_ip_sm_gw_registration_convertToJSON(OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration);
OpenAPI_ip_sm_gw_registration_t *OpenAPI_ip_sm_gw_registration_copy(OpenAPI_ip_sm_gw_registration_t *dst, OpenAPI_ip_sm_gw_registration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_sm_gw_registration_H_ */

