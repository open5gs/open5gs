/*
 * data_ind.h
 *
 * 
 */

#ifndef _OpenAPI_data_ind_H_
#define _OpenAPI_data_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_data_ind_NULL = 0, OpenAPI_data_ind_PFD, OpenAPI_data_ind_IPTV, OpenAPI_data_ind_BDT, OpenAPI_data_ind_SVC_PARAM, OpenAPI_data_ind_AM, OpenAPI_data_ind_DNAI_EAS, OpenAPI_data_ind_REQ_QOS, OpenAPI_data_ind_ECS, OpenAPI_data_ind_N3GDEV } OpenAPI_data_ind_e;

char* OpenAPI_data_ind_ToString(OpenAPI_data_ind_e data_ind);

OpenAPI_data_ind_e OpenAPI_data_ind_FromString(char* data_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_ind_H_ */

