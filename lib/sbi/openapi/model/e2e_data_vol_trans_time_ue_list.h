/*
 * e2e_data_vol_trans_time_ue_list.h
 *
 * Contains the list of UEs classified based on experience level of E2E Data Volume Transfer  Time  
 */

#ifndef _OpenAPI_e2e_data_vol_trans_time_ue_list_H_
#define _OpenAPI_e2e_data_vol_trans_time_ue_list_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_e2e_data_vol_trans_time_ue_list_s OpenAPI_e2e_data_vol_trans_time_ue_list_t;
#include "network_area_info.h"
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_e2e_data_vol_trans_time_ue_list_s {
    OpenAPI_list_t *high_level;
    OpenAPI_list_t *medium_level;
    OpenAPI_list_t *low_level;
    bool is_low_ratio;
    int low_ratio;
    bool is_medium_ratio;
    int medium_ratio;
    bool is_high_ratio;
    int high_ratio;
    struct OpenAPI_network_area_info_s *spatial_validity;
    struct OpenAPI_time_window_s *validity_period;
};

OpenAPI_e2e_data_vol_trans_time_ue_list_t *OpenAPI_e2e_data_vol_trans_time_ue_list_create(
    OpenAPI_list_t *high_level,
    OpenAPI_list_t *medium_level,
    OpenAPI_list_t *low_level,
    bool is_low_ratio,
    int low_ratio,
    bool is_medium_ratio,
    int medium_ratio,
    bool is_high_ratio,
    int high_ratio,
    OpenAPI_network_area_info_t *spatial_validity,
    OpenAPI_time_window_t *validity_period
);
void OpenAPI_e2e_data_vol_trans_time_ue_list_free(OpenAPI_e2e_data_vol_trans_time_ue_list_t *e2e_data_vol_trans_time_ue_list);
OpenAPI_e2e_data_vol_trans_time_ue_list_t *OpenAPI_e2e_data_vol_trans_time_ue_list_parseFromJSON(cJSON *e2e_data_vol_trans_time_ue_listJSON);
cJSON *OpenAPI_e2e_data_vol_trans_time_ue_list_convertToJSON(OpenAPI_e2e_data_vol_trans_time_ue_list_t *e2e_data_vol_trans_time_ue_list);
OpenAPI_e2e_data_vol_trans_time_ue_list_t *OpenAPI_e2e_data_vol_trans_time_ue_list_copy(OpenAPI_e2e_data_vol_trans_time_ue_list_t *dst, OpenAPI_e2e_data_vol_trans_time_ue_list_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_e2e_data_vol_trans_time_ue_list_H_ */

