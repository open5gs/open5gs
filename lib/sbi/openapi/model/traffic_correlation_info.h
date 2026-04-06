/*
 * traffic_correlation_info.h
 *
 * Contains the information for traffic correlation. 
 */

#ifndef _OpenAPI_traffic_correlation_info_H_
#define _OpenAPI_traffic_correlation_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_correlation_info_s OpenAPI_traffic_correlation_info_t;
#include "correlation_type.h"
#include "fqdn_pattern_matching_rule.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_correlation_info_s {
    OpenAPI_correlation_type_e corr_type;
    char *tfc_corr_id;
    bool is_com_eas_ipv4_addr_null;
    char *com_eas_ipv4_addr;
    bool is_com_eas_ipv6_addr_null;
    char *com_eas_ipv6_addr;
    bool is_fqdn_range_null;
    OpenAPI_list_t *fqdn_range;
    bool is_notif_uri_null;
    char *notif_uri;
    bool is_notif_corr_id_null;
    char *notif_corr_id;
};

OpenAPI_traffic_correlation_info_t *OpenAPI_traffic_correlation_info_create(
    OpenAPI_correlation_type_e corr_type,
    char *tfc_corr_id,
    bool is_com_eas_ipv4_addr_null,
    char *com_eas_ipv4_addr,
    bool is_com_eas_ipv6_addr_null,
    char *com_eas_ipv6_addr,
    bool is_fqdn_range_null,
    OpenAPI_list_t *fqdn_range,
    bool is_notif_uri_null,
    char *notif_uri,
    bool is_notif_corr_id_null,
    char *notif_corr_id
);
void OpenAPI_traffic_correlation_info_free(OpenAPI_traffic_correlation_info_t *traffic_correlation_info);
OpenAPI_traffic_correlation_info_t *OpenAPI_traffic_correlation_info_parseFromJSON(cJSON *traffic_correlation_infoJSON);
cJSON *OpenAPI_traffic_correlation_info_convertToJSON(OpenAPI_traffic_correlation_info_t *traffic_correlation_info);
OpenAPI_traffic_correlation_info_t *OpenAPI_traffic_correlation_info_copy(OpenAPI_traffic_correlation_info_t *dst, OpenAPI_traffic_correlation_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_correlation_info_H_ */

