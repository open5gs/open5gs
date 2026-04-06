/*
 * ngran_failure_info.h
 *
 * Indicates a NG-RAN failure event.
 */

#ifndef _OpenAPI_ngran_failure_info_H_
#define _OpenAPI_ngran_failure_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ngran_failure_info_NULL = 0, OpenAPI_ngran_failure_info_NG_RAN_NOT_REACHABLE, OpenAPI_ngran_failure_info_NG_RAN_FAILURE_WITH_RESTART, OpenAPI_ngran_failure_info_NG_RAN_FAILURE_WITHOUT_RESTART } OpenAPI_ngran_failure_info_e;

char* OpenAPI_ngran_failure_info_ToString(OpenAPI_ngran_failure_info_e ngran_failure_info);

OpenAPI_ngran_failure_info_e OpenAPI_ngran_failure_info_FromString(char* ngran_failure_info);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ngran_failure_info_H_ */

