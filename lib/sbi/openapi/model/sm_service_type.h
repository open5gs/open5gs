/*
 * sm_service_type.h
 *
 * 
 */

#ifndef _OpenAPI_sm_service_type_H_
#define _OpenAPI_sm_service_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sm_service_type_NULL = 0, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_CONTENT_PROCESSING, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_FORWARDING, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_FORWARDING_MULTIPLE_SUBSCRIPTIONS, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_FILTERING, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_RECEIPT, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_NETWORK_STORAGE, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_TO_MULTIPLE_DESTINATIONS, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_VIRTUAL_PRIVATE_NETWORK_VPN, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_AUTO_REPLY, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_PERSONAL_SIGNATURE, OpenAPI_sm_service_type_VAS4SMS_SHORT_MESSAGE_DEFERRED_DELIVERY } OpenAPI_sm_service_type_e;

char* OpenAPI_sm_service_type_ToString(OpenAPI_sm_service_type_e sm_service_type);

OpenAPI_sm_service_type_e OpenAPI_sm_service_type_FromString(char* sm_service_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_service_type_H_ */

