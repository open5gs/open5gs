/*
 * eas_info_to_refresh.h
 *
 * EAS information to be refreshed for EAS re-discovery
 */

#ifndef _OpenAPI_eas_info_to_refresh_H_
#define _OpenAPI_eas_info_to_refresh_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eas_info_to_refresh_s OpenAPI_eas_info_to_refresh_t;
#include "ipv4_address_range.h"
#include "ipv6_address_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eas_info_to_refresh_s {
    OpenAPI_list_t *ipv4_address_ranges;
    OpenAPI_list_t *ipv6_address_ranges;
    OpenAPI_list_t *fqdn_list;
};

OpenAPI_eas_info_to_refresh_t *OpenAPI_eas_info_to_refresh_create(
    OpenAPI_list_t *ipv4_address_ranges,
    OpenAPI_list_t *ipv6_address_ranges,
    OpenAPI_list_t *fqdn_list
);
void OpenAPI_eas_info_to_refresh_free(OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh);
OpenAPI_eas_info_to_refresh_t *OpenAPI_eas_info_to_refresh_parseFromJSON(cJSON *eas_info_to_refreshJSON);
cJSON *OpenAPI_eas_info_to_refresh_convertToJSON(OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh);
OpenAPI_eas_info_to_refresh_t *OpenAPI_eas_info_to_refresh_copy(OpenAPI_eas_info_to_refresh_t *dst, OpenAPI_eas_info_to_refresh_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eas_info_to_refresh_H_ */

