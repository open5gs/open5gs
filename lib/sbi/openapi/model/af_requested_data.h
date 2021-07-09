/*
 * af_requested_data.h
 *
 * 
 */

#ifndef _OpenAPI_af_requested_data_H_
#define _OpenAPI_af_requested_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_af_requested_data_NULL = 0, OpenAPI_af_requested_data_UE_IDENTITY } OpenAPI_af_requested_data_e;

char* OpenAPI_af_requested_data_ToString(OpenAPI_af_requested_data_e af_requested_data);

OpenAPI_af_requested_data_e OpenAPI_af_requested_data_FromString(char* af_requested_data);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_requested_data_H_ */

