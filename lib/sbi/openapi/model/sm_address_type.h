/*
 * sm_address_type.h
 *
 * 
 */

#ifndef _OpenAPI_sm_address_type_H_
#define _OpenAPI_sm_address_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sm_address_type_NULL = 0, OpenAPI_sm_address_type_EMAIL_ADDRESS, OpenAPI_sm_address_type_MSISDN, OpenAPI_sm_address_type_IPV4_ADDRESS, OpenAPI_sm_address_type_IPV6_ADDRESS, OpenAPI_sm_address_type_NUMERIC_SHORTCODE, OpenAPI_sm_address_type_ALPHANUMERIC_SHORTCODE, OpenAPI_sm_address_type_OTHER, OpenAPI_sm_address_type_IMSI } OpenAPI_sm_address_type_e;

char* OpenAPI_sm_address_type_ToString(OpenAPI_sm_address_type_e sm_address_type);

OpenAPI_sm_address_type_e OpenAPI_sm_address_type_FromString(char* sm_address_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_address_type_H_ */

