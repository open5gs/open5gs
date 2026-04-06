/*
 * ecn_marking_req.h
 *
 * 
 */

#ifndef _OpenAPI_ecn_marking_req_H_
#define _OpenAPI_ecn_marking_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ecn_marking_req_NULL = 0, OpenAPI_ecn_marking_req_UL, OpenAPI_ecn_marking_req_DL, OpenAPI_ecn_marking_req_BOTH, OpenAPI_ecn_marking_req_STOP } OpenAPI_ecn_marking_req_e;

char* OpenAPI_ecn_marking_req_ToString(OpenAPI_ecn_marking_req_e ecn_marking_req);

OpenAPI_ecn_marking_req_e OpenAPI_ecn_marking_req_FromString(char* ecn_marking_req);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecn_marking_req_H_ */

