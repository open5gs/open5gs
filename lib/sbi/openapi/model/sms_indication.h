/*
 * sms_indication.h
 *
 * 
 */

#ifndef _OpenAPI_sms_indication_H_
#define _OpenAPI_sms_indication_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sms_indication_NULL = 0, OpenAPI_sms_indication_SMS_SUPPORTED, OpenAPI_sms_indication_SMS_NOT_SUPPORTED } OpenAPI_sms_indication_e;

char* OpenAPI_sms_indication_ToString(OpenAPI_sms_indication_e sms_indication);

OpenAPI_sms_indication_e OpenAPI_sms_indication_FromString(char* sms_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_indication_H_ */

