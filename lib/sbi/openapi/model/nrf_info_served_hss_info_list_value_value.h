/*
 * nrf_info_served_hss_info_list_value_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_hss_info_list_value_value_H_
#define _OpenAPI_nrf_info_served_hss_info_list_value_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "hss_info.h"
#include "identity_range.h"
#include "imsi_range.h"
#include "network_node_diameter_address.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_hss_info_list_value_value_s OpenAPI_nrf_info_served_hss_info_list_value_value_t;
typedef struct OpenAPI_nrf_info_served_hss_info_list_value_value_s {
    char *group_id;
    OpenAPI_list_t *imsi_ranges;
    OpenAPI_list_t *ims_private_identity_ranges;
    OpenAPI_list_t *ims_public_identity_ranges;
    OpenAPI_list_t *msisdn_ranges;
    OpenAPI_list_t *external_group_identifiers_ranges;
    struct OpenAPI_network_node_diameter_address_s *hss_diameter_address;
} OpenAPI_nrf_info_served_hss_info_list_value_value_t;

OpenAPI_nrf_info_served_hss_info_list_value_value_t *OpenAPI_nrf_info_served_hss_info_list_value_value_create(
    char *group_id,
    OpenAPI_list_t *imsi_ranges,
    OpenAPI_list_t *ims_private_identity_ranges,
    OpenAPI_list_t *ims_public_identity_ranges,
    OpenAPI_list_t *msisdn_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_network_node_diameter_address_t *hss_diameter_address
);
void OpenAPI_nrf_info_served_hss_info_list_value_value_free(OpenAPI_nrf_info_served_hss_info_list_value_value_t *nrf_info_served_hss_info_list_value_value);
OpenAPI_nrf_info_served_hss_info_list_value_value_t *OpenAPI_nrf_info_served_hss_info_list_value_value_parseFromJSON(cJSON *nrf_info_served_hss_info_list_value_valueJSON);
cJSON *OpenAPI_nrf_info_served_hss_info_list_value_value_convertToJSON(OpenAPI_nrf_info_served_hss_info_list_value_value_t *nrf_info_served_hss_info_list_value_value);
OpenAPI_nrf_info_served_hss_info_list_value_value_t *OpenAPI_nrf_info_served_hss_info_list_value_value_copy(OpenAPI_nrf_info_served_hss_info_list_value_value_t *dst, OpenAPI_nrf_info_served_hss_info_list_value_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_hss_info_list_value_value_H_ */

