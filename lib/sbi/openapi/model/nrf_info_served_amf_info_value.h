/*
 * nrf_info_served_amf_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_amf_info_value_H_
#define _OpenAPI_nrf_info_served_amf_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nrf_info_served_amf_info_value_s OpenAPI_nrf_info_served_amf_info_value_t;
#include "amf_event_type.h"
#include "amf_info.h"
#include "guami.h"
#include "n2_interface_amf_info.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nrf_info_served_amf_info_value_s {
    char *amf_set_id;
    char *amf_region_id;
    OpenAPI_list_t *guami_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *backup_info_amf_failure;
    OpenAPI_list_t *backup_info_amf_removal;
    bool is_n2_interface_amf_info_null;
    struct OpenAPI_n2_interface_amf_info_s *n2_interface_amf_info;
    bool is_amf_onboarding_capability;
    int amf_onboarding_capability;
    bool is_high_latency_com;
    int high_latency_com;
    OpenAPI_list_t *amf_events;
    OpenAPI_list_t *pra_id_list;
    bool is_mobile_iab_ind_null;
    bool is_mobile_iab_ind;
    int mobile_iab_ind;
};

OpenAPI_nrf_info_served_amf_info_value_t *OpenAPI_nrf_info_served_amf_info_value_create(
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *backup_info_amf_failure,
    OpenAPI_list_t *backup_info_amf_removal,
    bool is_n2_interface_amf_info_null,
    OpenAPI_n2_interface_amf_info_t *n2_interface_amf_info,
    bool is_amf_onboarding_capability,
    int amf_onboarding_capability,
    bool is_high_latency_com,
    int high_latency_com,
    OpenAPI_list_t *amf_events,
    OpenAPI_list_t *pra_id_list,
    bool is_mobile_iab_ind_null,
    bool is_mobile_iab_ind,
    int mobile_iab_ind
);
void OpenAPI_nrf_info_served_amf_info_value_free(OpenAPI_nrf_info_served_amf_info_value_t *nrf_info_served_amf_info_value);
OpenAPI_nrf_info_served_amf_info_value_t *OpenAPI_nrf_info_served_amf_info_value_parseFromJSON(cJSON *nrf_info_served_amf_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_amf_info_value_convertToJSON(OpenAPI_nrf_info_served_amf_info_value_t *nrf_info_served_amf_info_value);
OpenAPI_nrf_info_served_amf_info_value_t *OpenAPI_nrf_info_served_amf_info_value_copy(OpenAPI_nrf_info_served_amf_info_value_t *dst, OpenAPI_nrf_info_served_amf_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_amf_info_value_H_ */

