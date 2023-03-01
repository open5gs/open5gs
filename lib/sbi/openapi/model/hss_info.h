/*
 * hss_info.h
 *
 * Information of an HSS NF Instance
 */

#ifndef _OpenAPI_hss_info_H_
#define _OpenAPI_hss_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"
#include "imsi_range.h"
#include "network_node_diameter_address.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hss_info_s OpenAPI_hss_info_t;
typedef struct OpenAPI_hss_info_s {
    char *group_id;
    OpenAPI_list_t *imsi_ranges;
    OpenAPI_list_t *ims_private_identity_ranges;
    OpenAPI_list_t *ims_public_identity_ranges;
    OpenAPI_list_t *msisdn_ranges;
    OpenAPI_list_t *external_group_identifiers_ranges;
    struct OpenAPI_network_node_diameter_address_s *hss_diameter_address;
} OpenAPI_hss_info_t;

OpenAPI_hss_info_t *OpenAPI_hss_info_create(
    char *group_id,
    OpenAPI_list_t *imsi_ranges,
    OpenAPI_list_t *ims_private_identity_ranges,
    OpenAPI_list_t *ims_public_identity_ranges,
    OpenAPI_list_t *msisdn_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_network_node_diameter_address_t *hss_diameter_address
);
void OpenAPI_hss_info_free(OpenAPI_hss_info_t *hss_info);
OpenAPI_hss_info_t *OpenAPI_hss_info_parseFromJSON(cJSON *hss_infoJSON);
cJSON *OpenAPI_hss_info_convertToJSON(OpenAPI_hss_info_t *hss_info);
OpenAPI_hss_info_t *OpenAPI_hss_info_copy(OpenAPI_hss_info_t *dst, OpenAPI_hss_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_info_H_ */

