/*
 * serv_auth_info.h
 *
 * 
 */

#ifndef _OpenAPI_serv_auth_info_H_
#define _OpenAPI_serv_auth_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_serv_auth_info_NULL = 0, OpenAPI_serv_auth_info_TP_NOT_KNOWN, OpenAPI_serv_auth_info_TP_EXPIRED, OpenAPI_serv_auth_info_TP_NOT_YET_OCURRED } OpenAPI_serv_auth_info_e;

char* OpenAPI_serv_auth_info_ToString(OpenAPI_serv_auth_info_e serv_auth_info);

OpenAPI_serv_auth_info_e OpenAPI_serv_auth_info_FromString(char* serv_auth_info);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_serv_auth_info_H_ */

