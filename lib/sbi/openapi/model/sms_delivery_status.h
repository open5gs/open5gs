/*
 * sms_delivery_status.h
 *
 * Represents the Status of an SMS delivery attempt.
 */

#ifndef _OpenAPI_sms_delivery_status_H_
#define _OpenAPI_sms_delivery_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sms_delivery_status_NULL = 0, OpenAPI_sms_delivery_status_SMS_DELIVERY_PENDING, OpenAPI_sms_delivery_status_SMS_DELIVERY_COMPLETED, OpenAPI_sms_delivery_status_SMS_DELIVERY_FAILED, OpenAPI_sms_delivery_status_SMS_DELIVERY_SMSF_ACCEPTED } OpenAPI_sms_delivery_status_e;

char* OpenAPI_sms_delivery_status_ToString(OpenAPI_sms_delivery_status_e sms_delivery_status);

OpenAPI_sms_delivery_status_e OpenAPI_sms_delivery_status_FromString(char* sms_delivery_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_delivery_status_H_ */

