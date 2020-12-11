/*
 * redirect_address_type.h
 *
 * Possible values are - IPV4_ADDR: Indicates that the address type is in the form of \&quot;dotted-decimal\&quot; IPv4 address. - IPV6_ADDR: Indicates that the address type is in the form of IPv6 address. - URL: Indicates that the address type is in the form of Uniform Resource Locator. - SIP_URI: Indicates that the address type is in the form of SIP Uniform Resource Identifier.
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

typedef struct OpenAPI_redirect_address_type_s OpenAPI_redirect_address_type_t;
typedef struct OpenAPI_redirect_address_type_s {
} OpenAPI_redirect_address_type_t;

OpenAPI_redirect_address_type_t *OpenAPI_redirect_address_type_create(
    );
void OpenAPI_redirect_address_type_free(OpenAPI_redirect_address_type_t *redirect_address_type);
OpenAPI_redirect_address_type_t *OpenAPI_redirect_address_type_parseFromJSON(cJSON *redirect_address_typeJSON);
cJSON *OpenAPI_redirect_address_type_convertToJSON(OpenAPI_redirect_address_type_t *redirect_address_type);
OpenAPI_redirect_address_type_t *OpenAPI_redirect_address_type_copy(OpenAPI_redirect_address_type_t *dst, OpenAPI_redirect_address_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redirect_address_type_H_ */

