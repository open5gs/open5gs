/*
 * charging_redirect_address_type.h
 *
 * 
 */

#ifndef _OpenAPI_charging_redirect_address_type_H_
#define _OpenAPI_charging_redirect_address_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_charging_redirect_address_type_NULL = 0, OpenAPI_charging_redirect_address_type_IPV4, OpenAPI_charging_redirect_address_type_IPV6, OpenAPI_charging_redirect_address_type_URL, OpenAPI_charging_redirect_address_type_URI } OpenAPI_charging_redirect_address_type_e;

char* OpenAPI_charging_redirect_address_type_ToString(OpenAPI_charging_redirect_address_type_e charging_redirect_address_type);

OpenAPI_charging_redirect_address_type_e OpenAPI_charging_redirect_address_type_FromString(char* charging_redirect_address_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_redirect_address_type_H_ */

