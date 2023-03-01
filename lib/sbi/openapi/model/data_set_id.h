/*
 * data_set_id.h
 *
 * 
 */

#ifndef _OpenAPI_data_set_id_H_
#define _OpenAPI_data_set_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_data_set_id_NULL = 0, OpenAPI_data_set_id_SUBSCRIPTION, OpenAPI_data_set_id_POLICY, OpenAPI_data_set_id_EXPOSURE, OpenAPI_data_set_id_APPLICATION, OpenAPI_data_set_id_A_PFD, OpenAPI_data_set_id_A_AFTI, OpenAPI_data_set_id_A_IPTV, OpenAPI_data_set_id_A_BDT, OpenAPI_data_set_id_A_SPD, OpenAPI_data_set_id_A_EASD, OpenAPI_data_set_id_A_AMI, OpenAPI_data_set_id_P_UE, OpenAPI_data_set_id_P_SCD, OpenAPI_data_set_id_P_BDT, OpenAPI_data_set_id_P_PLMNUE, OpenAPI_data_set_id_P_NSSCD } OpenAPI_data_set_id_e;

char* OpenAPI_data_set_id_ToString(OpenAPI_data_set_id_e data_set_id);

OpenAPI_data_set_id_e OpenAPI_data_set_id_FromString(char* data_set_id);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_set_id_H_ */

