/*
 * e2e_data_vol_trans_time_info.h
 *
 * Represents the E2E data volume transfer time analytics information when subscribed event is \&quot;E2E_DATA_VOL_TRANS_TIME\&quot;, the \&quot;dataVlTrnsTmInfos\&quot; attribute shall be included. 
 */

#ifndef _OpenAPI_e2e_data_vol_trans_time_info_H_
#define _OpenAPI_e2e_data_vol_trans_time_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_e2e_data_vol_trans_time_info_s OpenAPI_e2e_data_vol_trans_time_info_t;
#include "e2e_data_vol_trans_time_per_ts.h"
#include "e2e_data_vol_trans_time_ue_list.h"
#include "geo_distribution_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_e2e_data_vol_trans_time_info_s {
    OpenAPI_list_t *e2e_data_vol_trans_times;
    OpenAPI_list_t *e2e_data_vol_trans_time_ue_lists;
    OpenAPI_list_t *geo_distr_infos;
    bool is_confidence;
    int confidence;
};

OpenAPI_e2e_data_vol_trans_time_info_t *OpenAPI_e2e_data_vol_trans_time_info_create(
    OpenAPI_list_t *e2e_data_vol_trans_times,
    OpenAPI_list_t *e2e_data_vol_trans_time_ue_lists,
    OpenAPI_list_t *geo_distr_infos,
    bool is_confidence,
    int confidence
);
void OpenAPI_e2e_data_vol_trans_time_info_free(OpenAPI_e2e_data_vol_trans_time_info_t *e2e_data_vol_trans_time_info);
OpenAPI_e2e_data_vol_trans_time_info_t *OpenAPI_e2e_data_vol_trans_time_info_parseFromJSON(cJSON *e2e_data_vol_trans_time_infoJSON);
cJSON *OpenAPI_e2e_data_vol_trans_time_info_convertToJSON(OpenAPI_e2e_data_vol_trans_time_info_t *e2e_data_vol_trans_time_info);
OpenAPI_e2e_data_vol_trans_time_info_t *OpenAPI_e2e_data_vol_trans_time_info_copy(OpenAPI_e2e_data_vol_trans_time_info_t *dst, OpenAPI_e2e_data_vol_trans_time_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_e2e_data_vol_trans_time_info_H_ */

