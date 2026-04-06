/*
 * nrf_info_served_pcscf_info_list_value_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_pcscf_info_list_value_value_H_
#define _OpenAPI_nrf_info_served_pcscf_info_list_value_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nrf_info_served_pcscf_info_list_value_value_s OpenAPI_nrf_info_served_pcscf_info_list_value_value_t;
#include "access_type.h"
#include "identity_range.h"
#include "ipv4_address_range.h"
#include "ipv6_prefix_range.h"
#include "pcscf_info.h"
#include "plmn_id.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nrf_info_served_pcscf_info_list_value_value_s {
    OpenAPI_list_t *access_type;
    OpenAPI_list_t *dnn_list;
    char *gm_fqdn;
    OpenAPI_list_t *gm_ipv4_addresses;
    OpenAPI_list_t *gm_ipv6_addresses;
    char *mw_fqdn;
    OpenAPI_list_t *mw_ipv4_addresses;
    OpenAPI_list_t *mw_ipv6_addresses;
    OpenAPI_list_t *served_ipv4_address_ranges;
    OpenAPI_list_t *served_ipv6_prefix_ranges;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *gpsi_ranges;
    char *group_id;
    OpenAPI_list_t *serving_plmns;
};

OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *OpenAPI_nrf_info_served_pcscf_info_list_value_value_create(
    OpenAPI_list_t *access_type,
    OpenAPI_list_t *dnn_list,
    char *gm_fqdn,
    OpenAPI_list_t *gm_ipv4_addresses,
    OpenAPI_list_t *gm_ipv6_addresses,
    char *mw_fqdn,
    OpenAPI_list_t *mw_ipv4_addresses,
    OpenAPI_list_t *mw_ipv6_addresses,
    OpenAPI_list_t *served_ipv4_address_ranges,
    OpenAPI_list_t *served_ipv6_prefix_ranges,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    char *group_id,
    OpenAPI_list_t *serving_plmns
);
void OpenAPI_nrf_info_served_pcscf_info_list_value_value_free(OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *nrf_info_served_pcscf_info_list_value_value);
OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON(cJSON *nrf_info_served_pcscf_info_list_value_valueJSON);
cJSON *OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON(OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *nrf_info_served_pcscf_info_list_value_value);
OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *OpenAPI_nrf_info_served_pcscf_info_list_value_value_copy(OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *dst, OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_pcscf_info_list_value_value_H_ */

