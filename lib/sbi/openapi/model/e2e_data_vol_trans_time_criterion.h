/*
 * e2e_data_vol_trans_time_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_e2e_data_vol_trans_time_criterion_H_
#define _OpenAPI_e2e_data_vol_trans_time_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_e2e_data_vol_trans_time_criterion_NULL = 0, OpenAPI_e2e_data_vol_trans_time_criterion_E2E_DATA_VOL_TRANS_TIME } OpenAPI_e2e_data_vol_trans_time_criterion_e;

char* OpenAPI_e2e_data_vol_trans_time_criterion_ToString(OpenAPI_e2e_data_vol_trans_time_criterion_e e2e_data_vol_trans_time_criterion);

OpenAPI_e2e_data_vol_trans_time_criterion_e OpenAPI_e2e_data_vol_trans_time_criterion_FromString(char* e2e_data_vol_trans_time_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_e2e_data_vol_trans_time_criterion_H_ */

