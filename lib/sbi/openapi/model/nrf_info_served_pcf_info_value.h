/*
 * nrf_info_served_pcf_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_pcf_info_value_H_
#define _OpenAPI_nrf_info_served_pcf_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nrf_info_served_pcf_info_value_s OpenAPI_nrf_info_served_pcf_info_value_t;
#include "a2x_capability.h"
#include "identity_range.h"
#include "pcf_info.h"
#include "pro_se_capability.h"
#include "supi_range.h"
#include "v2x_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nrf_info_served_pcf_info_value_s {
    char *group_id;
    OpenAPI_list_t *dnn_list;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t *gpsi_ranges;
    char *rx_diam_host;
    char *rx_diam_realm;
    bool is_v2x_support_ind;
    int v2x_support_ind;
    bool is_prose_support_ind;
    int prose_support_ind;
    struct OpenAPI_pro_se_capability_s *prose_capability;
    struct OpenAPI_v2x_capability_s *v2x_capability;
    bool is_a2x_support_ind;
    int a2x_support_ind;
    struct OpenAPI_a2x_capability_s *a2x_capability;
    bool is_ranging_sl_pos_support_ind;
    int ranging_sl_pos_support_ind;
    bool is_ursp_eps_support;
    int ursp_eps_support;
};

OpenAPI_nrf_info_served_pcf_info_value_t *OpenAPI_nrf_info_served_pcf_info_value_create(
    char *group_id,
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    char *rx_diam_host,
    char *rx_diam_realm,
    bool is_v2x_support_ind,
    int v2x_support_ind,
    bool is_prose_support_ind,
    int prose_support_ind,
    OpenAPI_pro_se_capability_t *prose_capability,
    OpenAPI_v2x_capability_t *v2x_capability,
    bool is_a2x_support_ind,
    int a2x_support_ind,
    OpenAPI_a2x_capability_t *a2x_capability,
    bool is_ranging_sl_pos_support_ind,
    int ranging_sl_pos_support_ind,
    bool is_ursp_eps_support,
    int ursp_eps_support
);
void OpenAPI_nrf_info_served_pcf_info_value_free(OpenAPI_nrf_info_served_pcf_info_value_t *nrf_info_served_pcf_info_value);
OpenAPI_nrf_info_served_pcf_info_value_t *OpenAPI_nrf_info_served_pcf_info_value_parseFromJSON(cJSON *nrf_info_served_pcf_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_pcf_info_value_convertToJSON(OpenAPI_nrf_info_served_pcf_info_value_t *nrf_info_served_pcf_info_value);
OpenAPI_nrf_info_served_pcf_info_value_t *OpenAPI_nrf_info_served_pcf_info_value_copy(OpenAPI_nrf_info_served_pcf_info_value_t *dst, OpenAPI_nrf_info_served_pcf_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_pcf_info_value_H_ */

