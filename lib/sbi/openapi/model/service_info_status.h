/*
 * service_info_status.h
 *
 * 
 */

#ifndef _OpenAPI_service_info_status_H_
#define _OpenAPI_service_info_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_service_info_status_NULL = 0, OpenAPI_service_info_status__FINAL, OpenAPI_service_info_status_PRELIMINARY } OpenAPI_service_info_status_e;

char* OpenAPI_service_info_status_ToString(OpenAPI_service_info_status_e service_info_status);

OpenAPI_service_info_status_e OpenAPI_service_info_status_FromString(char* service_info_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_info_status_H_ */

