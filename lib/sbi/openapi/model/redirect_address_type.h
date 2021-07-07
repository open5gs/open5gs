/*
 * redirect_address_type.h
 *
 * 
 */

#ifndef _OpenAPI_redirect_address_type_H_
#define _OpenAPI_redirect_address_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_redirect_address_type_NULL = 0, OpenAPI_redirect_address_type_IPV4_ADDR, OpenAPI_redirect_address_type_IPV6_ADDR, OpenAPI_redirect_address_type_URL, OpenAPI_redirect_address_type_SIP_URI } OpenAPI_redirect_address_type_e;

char* OpenAPI_redirect_address_type_ToString(OpenAPI_redirect_address_type_e redirect_address_type);

OpenAPI_redirect_address_type_e OpenAPI_redirect_address_type_FromString(char* redirect_address_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redirect_address_type_H_ */

