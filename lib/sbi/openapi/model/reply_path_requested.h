/*
 * reply_path_requested.h
 *
 * 
 */

#ifndef _OpenAPI_reply_path_requested_H_
#define _OpenAPI_reply_path_requested_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reply_path_requested_NULL = 0, OpenAPI_reply_path_requested_NO_REPLY_PATH_SET, OpenAPI_reply_path_requested_REPLY_PATH_SET } OpenAPI_reply_path_requested_e;

char* OpenAPI_reply_path_requested_ToString(OpenAPI_reply_path_requested_e reply_path_requested);

OpenAPI_reply_path_requested_e OpenAPI_reply_path_requested_FromString(char* reply_path_requested);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reply_path_requested_H_ */

