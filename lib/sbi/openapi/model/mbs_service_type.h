/*
 * mbs_service_type.h
 *
 * Indicates the MBS service type of an MBS session
 */

#ifndef _OpenAPI_mbs_service_type_H_
#define _OpenAPI_mbs_service_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_mbs_service_type_NULL = 0, OpenAPI_mbs_service_type_MULTICAST, OpenAPI_mbs_service_type_BROADCAST } OpenAPI_mbs_service_type_e;

char* OpenAPI_mbs_service_type_ToString(OpenAPI_mbs_service_type_e mbs_service_type);

OpenAPI_mbs_service_type_e OpenAPI_mbs_service_type_FromString(char* mbs_service_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_service_type_H_ */

