/*
 * nrf_info_served_smf_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_smf_info_value_H_
#define _OpenAPI_nrf_info_served_smf_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "ip_addr.h"
#include "smf_info.h"
#include "snssai_smf_info_item.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_smf_info_value_s OpenAPI_nrf_info_served_smf_info_value_t;
typedef struct OpenAPI_nrf_info_served_smf_info_value_s {
    OpenAPI_list_t *s_nssai_smf_info_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    char *pgw_fqdn;
    OpenAPI_list_t *pgw_ip_addr_list;
    OpenAPI_list_t *access_type;
    bool is_priority;
    int priority;
    bool is_vsmf_support_ind;
    int vsmf_support_ind;
    OpenAPI_list_t *pgw_fqdn_list;
    bool is_smf_onboarding_capability;
    int smf_onboarding_capability;
    bool is_ismf_support_ind;
    int ismf_support_ind;
    bool is_smf_uprp_capability;
    int smf_uprp_capability;
} OpenAPI_nrf_info_served_smf_info_value_t;

OpenAPI_nrf_info_served_smf_info_value_t *OpenAPI_nrf_info_served_smf_info_value_create(
    OpenAPI_list_t *s_nssai_smf_info_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    char *pgw_fqdn,
    OpenAPI_list_t *pgw_ip_addr_list,
    OpenAPI_list_t *access_type,
    bool is_priority,
    int priority,
    bool is_vsmf_support_ind,
    int vsmf_support_ind,
    OpenAPI_list_t *pgw_fqdn_list,
    bool is_smf_onboarding_capability,
    int smf_onboarding_capability,
    bool is_ismf_support_ind,
    int ismf_support_ind,
    bool is_smf_uprp_capability,
    int smf_uprp_capability
);
void OpenAPI_nrf_info_served_smf_info_value_free(OpenAPI_nrf_info_served_smf_info_value_t *nrf_info_served_smf_info_value);
OpenAPI_nrf_info_served_smf_info_value_t *OpenAPI_nrf_info_served_smf_info_value_parseFromJSON(cJSON *nrf_info_served_smf_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_smf_info_value_convertToJSON(OpenAPI_nrf_info_served_smf_info_value_t *nrf_info_served_smf_info_value);
OpenAPI_nrf_info_served_smf_info_value_t *OpenAPI_nrf_info_served_smf_info_value_copy(OpenAPI_nrf_info_served_smf_info_value_t *dst, OpenAPI_nrf_info_served_smf_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_smf_info_value_H_ */

