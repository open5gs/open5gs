/*
 * e2e_data_vol_trans_time_per_ts.h
 *
 * Represents the E2E data volume transfer time analytics per Time Slot.
 */

#ifndef _OpenAPI_e2e_data_vol_trans_time_per_ts_H_
#define _OpenAPI_e2e_data_vol_trans_time_per_ts_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_e2e_data_vol_trans_time_per_ts_s OpenAPI_e2e_data_vol_trans_time_per_ts_t;
#include "e2e_data_vol_trans_time_per_ue.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_e2e_data_vol_trans_time_per_ts_s {
    char *ts_start;
    int ts_duration;
    OpenAPI_list_t *e2e_data_vol_trans_time_per_ue;
};

OpenAPI_e2e_data_vol_trans_time_per_ts_t *OpenAPI_e2e_data_vol_trans_time_per_ts_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_list_t *e2e_data_vol_trans_time_per_ue
);
void OpenAPI_e2e_data_vol_trans_time_per_ts_free(OpenAPI_e2e_data_vol_trans_time_per_ts_t *e2e_data_vol_trans_time_per_ts);
OpenAPI_e2e_data_vol_trans_time_per_ts_t *OpenAPI_e2e_data_vol_trans_time_per_ts_parseFromJSON(cJSON *e2e_data_vol_trans_time_per_tsJSON);
cJSON *OpenAPI_e2e_data_vol_trans_time_per_ts_convertToJSON(OpenAPI_e2e_data_vol_trans_time_per_ts_t *e2e_data_vol_trans_time_per_ts);
OpenAPI_e2e_data_vol_trans_time_per_ts_t *OpenAPI_e2e_data_vol_trans_time_per_ts_copy(OpenAPI_e2e_data_vol_trans_time_per_ts_t *dst, OpenAPI_e2e_data_vol_trans_time_per_ts_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_e2e_data_vol_trans_time_per_ts_H_ */

