/*
 * e2e_data_vol_trans_time_per_ue.h
 *
 * Represents the E2E data volume transfer time per UE.
 */

#ifndef _OpenAPI_e2e_data_vol_trans_time_per_ue_H_
#define _OpenAPI_e2e_data_vol_trans_time_per_ue_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_e2e_data_vol_trans_time_per_ue_s OpenAPI_e2e_data_vol_trans_time_per_ue_t;
#include "access_type.h"
#include "data_volume_transfer_time.h"
#include "network_area_info.h"
#include "rat_type.h"
#include "snssai.h"
#include "time_window.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_e2e_data_vol_trans_time_per_ue_s {
    char *supi;
    char *gpsi;
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_access_type_e access_type;
    OpenAPI_list_t *rat_types;
    char *app_id;
    struct OpenAPI_user_location_s *ue_loc;
    char *dnn;
    struct OpenAPI_network_area_info_s *spatial_validity;
    struct OpenAPI_time_window_s *validity_period;
    struct OpenAPI_data_volume_transfer_time_s *data_vol_trans_time;
};

OpenAPI_e2e_data_vol_trans_time_per_ue_t *OpenAPI_e2e_data_vol_trans_time_per_ue_create(
    char *supi,
    char *gpsi,
    OpenAPI_snssai_t *snssai,
    OpenAPI_access_type_e access_type,
    OpenAPI_list_t *rat_types,
    char *app_id,
    OpenAPI_user_location_t *ue_loc,
    char *dnn,
    OpenAPI_network_area_info_t *spatial_validity,
    OpenAPI_time_window_t *validity_period,
    OpenAPI_data_volume_transfer_time_t *data_vol_trans_time
);
void OpenAPI_e2e_data_vol_trans_time_per_ue_free(OpenAPI_e2e_data_vol_trans_time_per_ue_t *e2e_data_vol_trans_time_per_ue);
OpenAPI_e2e_data_vol_trans_time_per_ue_t *OpenAPI_e2e_data_vol_trans_time_per_ue_parseFromJSON(cJSON *e2e_data_vol_trans_time_per_ueJSON);
cJSON *OpenAPI_e2e_data_vol_trans_time_per_ue_convertToJSON(OpenAPI_e2e_data_vol_trans_time_per_ue_t *e2e_data_vol_trans_time_per_ue);
OpenAPI_e2e_data_vol_trans_time_per_ue_t *OpenAPI_e2e_data_vol_trans_time_per_ue_copy(OpenAPI_e2e_data_vol_trans_time_per_ue_t *dst, OpenAPI_e2e_data_vol_trans_time_per_ue_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_e2e_data_vol_trans_time_per_ue_H_ */

