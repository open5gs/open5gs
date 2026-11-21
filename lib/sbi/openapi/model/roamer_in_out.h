/*
 * roamer_in_out.h
 *
 * 
 */

#ifndef _OpenAPI_roamer_in_out_H_
#define _OpenAPI_roamer_in_out_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_roamer_in_out_NULL = 0, OpenAPI_roamer_in_out_IN_BOUND, OpenAPI_roamer_in_out_OUT_BOUND } OpenAPI_roamer_in_out_e;

char* OpenAPI_roamer_in_out_ToString(OpenAPI_roamer_in_out_e roamer_in_out);

OpenAPI_roamer_in_out_e OpenAPI_roamer_in_out_FromString(char* roamer_in_out);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roamer_in_out_H_ */

