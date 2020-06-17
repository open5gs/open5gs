/*
 * sms_support.h
 *
 *
 */

#ifndef _OpenAPI_sms_support_H_
#define _OpenAPI_sms_support_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sms_support_NULL = 0, OpenAPI_sms_support__3GPP, OpenAPI_sms_support_NON_3GPP, OpenAPI_sms_support_BOTH, OpenAPI_sms_support_NONE } OpenAPI_sms_support_e;

char* OpenAPI_sms_support_ToString(OpenAPI_sms_support_e sms_support);

OpenAPI_sms_support_e OpenAPI_sms_support_FromString(char* sms_support);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_support_H_ */

