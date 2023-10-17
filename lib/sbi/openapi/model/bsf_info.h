/*
 * bsf_info.h
 *
 * Information of a BSF NF Instance
 */

#ifndef _OpenAPI_bsf_info_H_
#define _OpenAPI_bsf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"
#include "ipv4_address_range.h"
#include "ipv6_prefix_range.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bsf_info_s OpenAPI_bsf_info_t;
typedef struct OpenAPI_bsf_info_s {
    OpenAPI_list_t *dnn_list;
    OpenAPI_list_t *ip_domain_list;
    OpenAPI_list_t *ipv4_address_ranges;
    OpenAPI_list_t *ipv6_prefix_ranges;
    char *rx_diam_host;
    char *rx_diam_realm;
    char *group_id;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *gpsi_ranges;
} OpenAPI_bsf_info_t;

OpenAPI_bsf_info_t *OpenAPI_bsf_info_create(
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *ip_domain_list,
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv6_prefix_ranges,
    char *rx_diam_host,
    char *rx_diam_realm,
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges
);
void OpenAPI_bsf_info_free(OpenAPI_bsf_info_t *bsf_info);
OpenAPI_bsf_info_t *OpenAPI_bsf_info_parseFromJSON(cJSON *bsf_infoJSON);
cJSON *OpenAPI_bsf_info_convertToJSON(OpenAPI_bsf_info_t *bsf_info);
OpenAPI_bsf_info_t *OpenAPI_bsf_info_copy(OpenAPI_bsf_info_t *dst, OpenAPI_bsf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bsf_info_H_ */

