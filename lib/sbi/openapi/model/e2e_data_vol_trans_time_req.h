/*
 * e2e_data_vol_trans_time_req.h
 *
 * Represents other E2E data volume transfer time analytics requirements.
 */

#ifndef _OpenAPI_e2e_data_vol_trans_time_req_H_
#define _OpenAPI_e2e_data_vol_trans_time_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_e2e_data_vol_trans_time_req_s OpenAPI_e2e_data_vol_trans_time_req_t;
#include "data_volume.h"
#include "e2e_data_vol_trans_time_criterion.h"
#include "matching_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_e2e_data_vol_trans_time_req_s {
    OpenAPI_e2e_data_vol_trans_time_criterion_e criterion;
    OpenAPI_matching_direction_e order;
    bool is_high_trans_tm_thr;
    int high_trans_tm_thr;
    bool is_low_trans_tm_thr;
    int low_trans_tm_thr;
    bool is_repeat_data_trans;
    int repeat_data_trans;
    bool is_ts_interval_data_trans;
    int ts_interval_data_trans;
    bool is_data_volume_null;
    struct OpenAPI_data_volume_s *data_volume;
    bool is_max_number_ues;
    int max_number_ues;
};

OpenAPI_e2e_data_vol_trans_time_req_t *OpenAPI_e2e_data_vol_trans_time_req_create(
    OpenAPI_e2e_data_vol_trans_time_criterion_e criterion,
    OpenAPI_matching_direction_e order,
    bool is_high_trans_tm_thr,
    int high_trans_tm_thr,
    bool is_low_trans_tm_thr,
    int low_trans_tm_thr,
    bool is_repeat_data_trans,
    int repeat_data_trans,
    bool is_ts_interval_data_trans,
    int ts_interval_data_trans,
    bool is_data_volume_null,
    OpenAPI_data_volume_t *data_volume,
    bool is_max_number_ues,
    int max_number_ues
);
void OpenAPI_e2e_data_vol_trans_time_req_free(OpenAPI_e2e_data_vol_trans_time_req_t *e2e_data_vol_trans_time_req);
OpenAPI_e2e_data_vol_trans_time_req_t *OpenAPI_e2e_data_vol_trans_time_req_parseFromJSON(cJSON *e2e_data_vol_trans_time_reqJSON);
cJSON *OpenAPI_e2e_data_vol_trans_time_req_convertToJSON(OpenAPI_e2e_data_vol_trans_time_req_t *e2e_data_vol_trans_time_req);
OpenAPI_e2e_data_vol_trans_time_req_t *OpenAPI_e2e_data_vol_trans_time_req_copy(OpenAPI_e2e_data_vol_trans_time_req_t *dst, OpenAPI_e2e_data_vol_trans_time_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_e2e_data_vol_trans_time_req_H_ */

