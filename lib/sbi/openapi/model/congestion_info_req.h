/*
 * congestion_info_req.h
 *
 * 
 */

#ifndef _OpenAPI_congestion_info_req_H_
#define _OpenAPI_congestion_info_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_congestion_info_req_NULL = 0, OpenAPI_congestion_info_req_UL, OpenAPI_congestion_info_req_DL, OpenAPI_congestion_info_req_BOTH, OpenAPI_congestion_info_req_STOP } OpenAPI_congestion_info_req_e;

char* OpenAPI_congestion_info_req_ToString(OpenAPI_congestion_info_req_e congestion_info_req);

OpenAPI_congestion_info_req_e OpenAPI_congestion_info_req_FromString(char* congestion_info_req);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_congestion_info_req_H_ */

