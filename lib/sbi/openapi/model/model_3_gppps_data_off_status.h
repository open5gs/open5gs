/*
 * model_3_gppps_data_off_status.h
 *
 * 
 */

#ifndef _OpenAPI_model_3_gppps_data_off_status_H_
#define _OpenAPI_model_3_gppps_data_off_status_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_model_3_gppps_data_off_status_NULL = 0, OpenAPI_model_3_gppps_data_off_status_ACTIVE, OpenAPI_model_3_gppps_data_off_status_INACTIVE } OpenAPI_model_3_gppps_data_off_status_e;

char* OpenAPI_model_3_gppps_data_off_status_ToString(OpenAPI_model_3_gppps_data_off_status_e model_3_gppps_data_off_status);

OpenAPI_model_3_gppps_data_off_status_e OpenAPI_model_3_gppps_data_off_status_FromString(char* model_3_gppps_data_off_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_3_gppps_data_off_status_H_ */

